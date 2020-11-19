#pragma once

// ===========================================================
// Callback Funktion von MQTT. Die Funktion wird aufgerufen
// wenn ein Wert empfangen wurde.
// ===========================================================
void MQTT_callback(char *topic, byte *payload, unsigned int length)
{
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

	if (strcmp(msg, "on") == 0)
	{
        Serial.println("MQTT ON");
	}
	else if (strcmp(msg, "off") == 0)
	{
        Serial.println("MQTT Off");
	}
}

void MQTT_reconnect()
{
	mqttClient.subscribe("/Wortuhr");
	Serial.println("MQTT Connected...");
}
