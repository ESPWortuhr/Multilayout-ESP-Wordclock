#pragma once
// ===========================================================
// Callback Funktion von MQTT. Die Funktion wird aufgerufen
// wenn ein Wert empfangen wurde.
// ===========================================================
void MQTT_callback(char *topic, byte *payload, unsigned int length)
{
	//{"state": "on", "brightness": 255, "color": [255, 255, 255], "effect": "rainbow"}
	StaticJsonDocument<200> doc;

	Serial.print("Received message [");
	Serial.print(topic);
	Serial.print("] ");
	char msg[length + 1];
	for (int i = 0; i < length; i++)
	{
		Serial.print((char) payload[i]);
		msg[i] = (char) payload[i];
	}
	Serial.println();

	msg[length] = '\0';
	Serial.println(msg);

	DeserializationError error = deserializeJson(doc, msg);

	// Test if parsing succeeds.
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
		return;
	}

	if (doc["state"] == 'ON'){
		const char* str_buff = doc["effect"];
		std::string str_buffer = str_buff;
		if (str_buffer == "Wortuhr") {
				G.prog = COMMAND_MODE_WORD_CLOCK;
			}
		else if (str_buffer == "Sekundenanzeige") {
				G.prog = COMMAND_MODE_SECONDS;
			}
		else if (str_buffer == "Laufschrift") {
				G.prog = COMMAND_MODE_MARQUEE;
			}
		else if (str_buffer == "Regenbogen") {
				G.prog = COMMAND_MODE_RAINBOW;
			}
		else if (str_buffer == "Farbwechsel") {
				G.prog = COMMAND_MODE_CHANGE;
			}
		else if (str_buffer == "Farbe") {
				G.prog = COMMAND_MODE_COLOR;
			}
		G.rgb[Foreground][0] = doc["color"][0];
		G.rgb[Foreground][1] = doc["color"][1];
		G.rgb[Foreground][2] = doc["color"][2];
		//G.rgb[Foreground][3] = doc["White"];
	}
	else {
		G.rgb[Foreground][0] = 0;
		G.rgb[Foreground][1] = 0;
		G.rgb[Foreground][2] = 0;
		G.rgb[Foreground][3] = 0;
	}

}

void MQTT_reconnect()
{
	mqttClient.subscribe("/Wortuhr");
	Serial.println("MQTT Connected...");
}
