#!/usr/bin/env python3
"""
MQTT tests for the ESP Wordclock firmware.

These tests talk to a real MQTT broker and a running clock. Install the only
runtime dependency with:

    python3 -m pip install paho-mqtt

Example:

    python3 tools/mqtt_smoke_test.py --host 192.168.1.10 --topic ESPWordclock

The tests intentionally use the public MQTT API rather than Arduino internals:
the firmware logic depends on PubSubClient, retained broker messages, and the
device event loop, so the MQTT contract is the useful thing to verify here.
"""

from __future__ import annotations

import argparse
import json
import queue
import sys
import time
import unittest
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable

mqtt: Any = None


def load_paho() -> None:
    global mqtt
    try:
        import paho.mqtt.client as paho_mqtt
    except ImportError as exc:  # pragma: no cover - exercised only without paho
        raise SystemExit(
            "Missing dependency: paho-mqtt\n"
            "Install it with: python3 -m pip install paho-mqtt"
        ) from exc
    mqtt = paho_mqtt


@dataclass
class Settings:
    host: str
    port: int
    topic: str
    username: str | None
    password: str | None
    timeout: float
    ha_prefix: str
    skip_discovery: bool


class MqttProbe:
    def __init__(self, settings: Settings) -> None:
        self.settings = settings
        self.messages: "queue.Queue[tuple[str, bytes, bool]]" = queue.Queue()
        self.seen: list[tuple[str, bytes, bool]] = []

        client_id = f"wordclock-test-{int(time.time())}"
        try:
            self.client = mqtt.Client(
                mqtt.CallbackAPIVersion.VERSION2, client_id=client_id
            )
        except AttributeError:
            self.client = mqtt.Client(client_id=client_id)

        if settings.username:
            self.client.username_pw_set(settings.username, settings.password)

        self.client.on_connect = self._on_connect
        self.connected = False
        self.connect_error: str | None = None
        self.client.on_message = self._on_message

    @staticmethod
    def _reason_code_value(reason_code: Any) -> int:
        if hasattr(reason_code, "value"):
            return int(reason_code.value)
        try:
            return int(reason_code)
        except TypeError:
            return int(getattr(reason_code, "rc", 0))

    def _on_connect(self, client: mqtt.Client, _userdata: Any, *args: Any) -> None:
        reason_code = args[1] if len(args) >= 2 else args[0]
        if self._reason_code_value(reason_code) != 0:
            self.connect_error = f"MQTT connect failed: {reason_code}"
            return
        self.connected = True
        client.subscribe(f"{self.settings.topic}/#")
        client.subscribe(f"{self.settings.ha_prefix}/#")

    def _on_message(
        self,
        _client: mqtt.Client,
        _userdata: Any,
        message: mqtt.MQTTMessage,
    ) -> None:
        item = (message.topic, bytes(message.payload), bool(message.retain))
        self.seen.append(item)
        self.messages.put(item)

    def connect(self) -> None:
        rc = self.client.connect(self.settings.host, self.settings.port, keepalive=30)
        if rc != mqtt.MQTT_ERR_SUCCESS:
            raise RuntimeError(f"MQTT connect failed before CONNACK: rc={rc}")

        self.client.loop_start()
        deadline = time.time() + self.settings.timeout
        while time.time() < deadline:
            if self.connected:
                return
            if self.connect_error:
                raise RuntimeError(self.connect_error)
            time.sleep(0.05)
        raise RuntimeError(
            "Timed out connecting to MQTT broker. "
            "If this broker requires auth, pass --username and --password."
        )

    def close(self) -> None:
        self.client.loop_stop()
        self.client.disconnect()

    def publish(self, topic: str, payload: str, retain: bool = False) -> None:
        result = self.client.publish(topic, payload, retain=retain)
        result.wait_for_publish(timeout=5)
        if result.rc != mqtt.MQTT_ERR_SUCCESS:
            raise AssertionError(f"Publish failed for {topic}: rc={result.rc}")

    def wait_for(
        self,
        predicate: Callable[[str, bytes, bool], bool],
        timeout: float | None = None,
    ) -> tuple[str, bytes, bool]:
        deadline = time.time() + (timeout or self.settings.timeout)
        for item in list(self.seen):
            if predicate(*item):
                return item

        while time.time() < deadline:
            try:
                item = self.messages.get(timeout=0.2)
            except queue.Empty:
                continue
            if predicate(*item):
                return item
        raise AssertionError("Timed out waiting for MQTT message")

    def wait_payload(
        self,
        topic: str,
        predicate: Callable[[bytes], bool] = lambda _payload: True,
        timeout: float | None = None,
    ) -> bytes:
        _topic, payload, _retain = self.wait_for(
            lambda msg_topic, msg_payload, _retain: msg_topic == topic
            and predicate(msg_payload),
            timeout,
        )
        return payload

    def wait_json(
        self,
        topic: str,
        predicate: Callable[[dict[str, Any]], bool] = lambda _doc: True,
        timeout: float | None = None,
    ) -> dict[str, Any]:
        def is_matching_json(payload: bytes) -> bool:
            try:
                doc = json.loads(payload.decode("utf-8"))
            except json.JSONDecodeError:
                return False
            return isinstance(doc, dict) and predicate(doc)

        payload = self.wait_payload(topic, is_matching_json, timeout)
        return json.loads(payload.decode("utf-8"))


class WordclockMqttTests(unittest.TestCase):
    probe: MqttProbe
    settings: Settings

    @property
    def base(self) -> str:
        return self.settings.topic

    def test_01_availability_is_online(self) -> None:
        payload = self.probe.wait_payload(f"{self.base}/availability")
        self.assertEqual(payload.decode("utf-8"), "online")

    def test_02_light_status_schema(self) -> None:
        self.probe.publish(f"{self.base}/cmd", json.dumps({"state": "ON"}))
        status = self.probe.wait_json(f"{self.base}/status")

        self.assertIn(status.get("state"), {"ON", "OFF"})
        self.assertIsInstance(status.get("brightness"), int)
        self.assertGreaterEqual(status["brightness"], 0)
        self.assertLessEqual(status["brightness"], 255)
        self.assertEqual(status.get("color_mode"), "hs")

        color = status.get("color")
        self.assertIsInstance(color, dict)
        self.assertGreaterEqual(color.get("h"), 0)
        self.assertLessEqual(color.get("h"), 360)
        self.assertGreaterEqual(color.get("s"), 0)
        self.assertLessEqual(color.get("s"), 100)

    def test_03_state_command_roundtrip(self) -> None:
        self.probe.publish(f"{self.base}/cmd", json.dumps({"state": "OFF"}))
        self.probe.wait_json(
            f"{self.base}/status", lambda doc: doc.get("state") == "OFF"
        )

        self.probe.publish(f"{self.base}/cmd", json.dumps({"state": "ON"}))
        self.probe.wait_json(
            f"{self.base}/status", lambda doc: doc.get("state") == "ON"
        )

    def test_04_color_command_roundtrip(self) -> None:
        self.probe.publish(
            f"{self.base}/cmd", json.dumps({"color": {"h": 180, "s": 50}})
        )
        status = self.probe.wait_json(
            f"{self.base}/status",
            lambda doc: doc.get("color", {}).get("h") == 180
            and doc.get("color", {}).get("s") == 50,
        )
        self.assertEqual(status["color"]["h"], 180)
        self.assertEqual(status["color"]["s"], 50)

    def test_05_scrolling_text_roundtrip(self) -> None:
        text = "MQTT TEST"
        self.probe.publish(
            f"{self.base}/scrolltext/set", json.dumps({"scrolling_text": text})
        )
        state = self.probe.wait_json(
            f"{self.base}/scrolltext/state",
            lambda doc: doc.get("scrolling_text") == text,
        )
        self.assertEqual(state["scrolling_text"], text)

    def test_06_effect_speed_roundtrip(self) -> None:
        self.probe.publish(f"{self.base}/effect_speed/set", "5")
        payload = self.probe.wait_payload(
            f"{self.base}/effect_speed/state",
            lambda value: value.decode("utf-8") == "5",
        )
        self.assertEqual(payload.decode("utf-8"), "5")

    def test_07_auto_brightness_switch_roundtrip(self) -> None:
        self.probe.publish(f"{self.base}/auto_brightness/set", "OFF")
        payload = self.probe.wait_payload(
            f"{self.base}/auto_brightness/state",
            lambda value: value.decode("utf-8") == "OFF",
        )
        self.assertEqual(payload.decode("utf-8"), "OFF")

    def test_08_diagnostics_schema(self) -> None:
        diagnostics = self.probe.wait_json(f"{self.base}/diagnostics", timeout=35)
        self.assertIn("lux", diagnostics)
        self.assertIn("led_gain", diagnostics)
        self.assertIn("adc_value", diagnostics)
        self.assertIn("adc_raw", diagnostics)

    def test_09_discovery_payloads_if_retained(self) -> None:
        if self.settings.skip_discovery:
            self.skipTest("Discovery validation disabled by --skip-discovery")

        configs = [
            (topic, payload)
            for topic, payload, _retain in self.probe.seen
            if topic.startswith(f"{self.settings.ha_prefix}/")
            and topic.endswith("/config")
            and payload
        ]
        if not configs:
            self.skipTest("No retained Home Assistant discovery configs received")

        decoded = []
        for topic, payload in configs:
            try:
                doc = json.loads(payload.decode("utf-8"))
            except json.JSONDecodeError as exc:
                self.fail(f"Discovery config on {topic} is not valid JSON: {exc}")
            decoded.append((topic, doc))

        light = [
            doc
            for topic, doc in decoded
            if f"/light/" in topic and doc.get("state_topic") == f"{self.base}/status"
        ]
        self.assertTrue(light, "Missing matching Home Assistant light discovery config")

        light_config = light[0]
        self.assertEqual(light_config.get("schema"), "json")
        self.assertEqual(
            light_config.get("availability_topic"), f"{self.base}/availability"
        )

        supported_modes = light_config.get("supported_color_modes", [])
        legacy_color_mode = light_config.get("color_mode") is True
        self.assertTrue(
            "hs" in supported_modes or legacy_color_mode,
            "Light discovery must advertise HS color support",
        )

        text = [doc for topic, doc in decoded if f"/text/" in topic]
        if text:
            self.assertLessEqual(text[0].get("max", 999), 29)


def parse_args(argv: list[str]) -> Settings:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="localhost", help="MQTT broker host")
    parser.add_argument("--port", type=int, default=1883, help="MQTT broker port")
    parser.add_argument("--topic", default="ESPWordclock", help="Clock base topic")
    parser.add_argument("--username", help="MQTT username")
    parser.add_argument("--password", help="MQTT password")
    parser.add_argument(
        "--password-file",
        help="Read MQTT password from a file instead of the command line",
    )
    parser.add_argument(
        "--timeout", type=float, default=15.0, help="Default wait timeout in seconds"
    )
    parser.add_argument(
        "--ha-prefix",
        default="homeassistant",
        help="Home Assistant discovery prefix",
    )
    parser.add_argument(
        "--skip-discovery",
        action="store_true",
        help="Skip retained Home Assistant discovery config checks",
    )
    args = parser.parse_args(argv)
    password = args.password
    if args.password_file:
        password = Path(args.password_file).read_text(encoding="utf-8").strip()

    return Settings(
        host=args.host,
        port=args.port,
        topic=args.topic.rstrip("/"),
        username=args.username,
        password=password,
        timeout=args.timeout,
        ha_prefix=args.ha_prefix.rstrip("/"),
        skip_discovery=args.skip_discovery,
    )


def main(argv: list[str]) -> int:
    settings = parse_args(argv)
    load_paho()
    probe = MqttProbe(settings)
    probe.connect()

    WordclockMqttTests.settings = settings
    WordclockMqttTests.probe = probe
    try:
        suite = unittest.defaultTestLoader.loadTestsFromTestCase(WordclockMqttTests)
        result = unittest.TextTestRunner(verbosity=2).run(suite)
        return 0 if result.wasSuccessful() else 1
    finally:
        probe.close()


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
