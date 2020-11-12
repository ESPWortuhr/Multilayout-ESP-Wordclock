#pragma once

//---------------------------------------------------------
// WLAN-Status
//---------------------------------------------------------
char wstatus[7][25] = {

		"WL_IDLE_STATUS", "WL_NO_SSID_AVAIL", "WL_SCAN_COMPLETED", "WL_CONNECTED", "WL_CONNECT_FAILED", "WL_CONNECTION_LOST", "WL_DISCONNECTED"};
// WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
// WL_IDLE_STATUS      = 0,
// WL_NO_SSID_AVAIL    = 1,
// WL_SCAN_COMPLETED   = 2,
// WL_CONNECTED        = 3,
// WL_CONNECT_FAILED   = 4,
// WL_CONNECTION_LOST  = 5,
// WL_DISCONNECTED     = 6
//---------------------------------------------------------
// WIFI_EVENT_STAMODE_CONNECTED = 0,
// WIFI_EVENT_STAMODE_DISCONNECTED
// WIFI_EVENT_STAMODE_AUTHMODE_CHANGE
// WIFI_EVENT_STAMODE_GOT_IP
// WIFI_EVENT_STAMODE_DHCP_TIMEOUT
// WIFI_EVENT_SOFTAPMODE_STACONNECTED
//---------------------------------------------------------

//------------------------------------------------------------------------------

String WiFiScan(bool html)
{
	//-------------------------------------
	// Scan Network
	//-------------------------------------

	uint8_t n = WiFi.scanNetworks();
	String htmlwlan, linewlan, cssid;
	int indices[n], swap, quality;

	if (n == 0)
	{
		Serial.println("no networks found");
		if (html == true)
		{
			htmlwlan += F("<div>Es wurden keine WLAN Netzwerke gefunden</div>");
		}
		else
		{
			delay(10);
		}
	}
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		if (html == true)
		{
			//sort networks
			for (uint8_t i = 0; i < n; i++)
			{
				indices[i] = i;
			}
			for (uint8_t i = 0; i < n; i++)
			{
				for (uint8_t j = i + 1; j < n; j++)
				{
					if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i]))
					{
						swap = indices[i];
						indices[i] = indices[j];
						indices[j] = swap; //hab den Swap Befehl nicht gefunden
					}
				}
			}
			// remove duplicates ( must be RSSI sorted )
			for (uint8_t i = 0; i < n; i++)
			{
				if (indices[i] == -1) { continue; }
				cssid = WiFi.SSID(indices[i]);
				for (int j = i + 1; j < n; j++)
				{
					if (cssid == WiFi.SSID(indices[j]))
					{
						indices[j] = -1; // set dup aps to index -1
					}
				}
			}
		}
		for (uint8_t i = 0; i < n; ++i)
		{
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			if (html == true)
			{
				if (indices[i] == -1) { continue; } // skip dups
				linewlan = F("<div><a href='#' onclick='wlanlists(this)'>{s}</a>&nbsp;<span class='wlanlistr {l}'>{r}%</span></div>");
				linewlan.replace("{s}", WiFi.SSID(indices[i]));
				if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE)
				{
					linewlan.replace("{l}", "wlanlistl");
				}
				else
				{
					linewlan.replace("{l}", "");
				}
				quality = WiFi.RSSI(indices[i]);
				if (quality <= -100)
				{
					quality = 0;
				}
				else if (quality >= -50)
				{
					quality = 100;
				}
				else
				{
					quality = 2 * (quality + 100);
				}
				linewlan.replace("{r}", String(quality));
				htmlwlan += linewlan;
			}
			else
			{

				delay(100);
				if (wlan_ssid == false)
				{
					wlan_ssid = true;
					for (int k = 0; WiFi.SSID(i)[k] != '\0'; k++)
					{
						if (WiFi.SSID(i)[k] != G.ssid[k]) { wlan_ssid = false; }
					}
				}
			}
		}
	}
	wlan_status = WiFi.status();
	Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
	Serial.printf("WLAN-SSID vorhanden: %d\n", wlan_ssid);
	return htmlwlan;
	//-------------------------------------
}

//------------------------------------------------------------------------------

void WiFiStart_WPS()
{
	Serial.println("WPS Configuration Started");
	bool wpsSuccess = WiFi.beginWPSConfig();
	String buffer;
	if (wpsSuccess)
	{
		String newSSID = WiFi.SSID();
		String newPasswd = WiFi.psk();
		if (newSSID.length() > 0)
		{
			Serial.printf("WPS done. Successfully logged in to SSID ' %s'", newSSID.c_str());
			strncpy(G.ssid, newSSID.c_str(), sizeof(G.ssid));
			Serial.print("New-Wifi-SSID:  ");
			Serial.println(G.ssid);
			strncpy(G.passwd, newPasswd.c_str(), sizeof(G.passwd));
			Serial.print("New-Wifi-Password:  ");
			Serial.println(G.passwd);
		}
	}
	else
	{
		Serial.println("WPS Pairing failed");
	}
}

//------------------------------------------------------------------------------

void WiFiStart_Client()
{
	unsigned int count = 0;
	// Connect to WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(G.ssid);
	Serial.println(G.passwd);

	WiFi.disconnect();
	// WLAN-Mode Clinet stetzen
	WiFi.mode(WIFI_STA);
	WiFi.begin((const char *) G.ssid, (const char *) G.passwd);
	while (count < 30)
	{
		if (WiFi.status() != WL_CONNECTED)
		{
			count++;
			delay(500);
			Serial.print(".");
		}
		else
		{
			break;
		}
	}
	if (WiFi.status() == WL_CONNECTED)
	{  // Als Clinet connected
		Serial.println("");
		Serial.println("WiFi-Client connected");

		char ip_adress[20];
		sprintf(ip_adress, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
		Serial.println(ip_adress);
		Serial.println("");
		// IP-Adresse als Laufschrift anzeigen
		if (show_ip == true) { zeigeip(ip_adress); }

		wlan_client = true;
	}

}

//------------------------------------------------------------------------------

void WiFiCheck_AP_Status()
{
	// Is AP-Status > 0 ... ON.
	if (AP_Status > 0 && AP_Status != 6)
	{
		AP_Status++;
		if (AP_Status == 6) // Five Interations (Minutes)
		{
			G.conf = COMMAND_SET_WIFI_DISABLED;
		}
	}
}

//------------------------------------------------------------------------------

void WiFiStart_AP()
{
	// WLAN-Mode AccessPoint stetzen
	Serial.println("");
	Serial.println("Setup WiFi Access-Point");

	WiFi.disconnect();
	delay(300);
	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid_ap, password_ap);

	char buf[20];
	sprintf(buf, "IP:%d.%d.%d.%d", WiFi.softAPIP()[0], WiFi.softAPIP()[1], WiFi.softAPIP()[2], WiFi.softAPIP()[3]);
	Serial.print("AccessPoint ");
	Serial.println(buf);
	Serial.println("");
	zeigeip(buf);

	wlan_client = false;
}

//------------------------------------------------------------------------------

void WiFiReconnect()
{
	// WLAN reconnect
	wlan_status = WiFi.status();
	Serial.printf("WLAN-Status: %s\n", wstatus[wlan_status]);
	//WiFi.reconnect();
	WiFi.disconnect();
	delay(300);
	WiFi.mode(WIFI_STA);
	WiFi.begin((const char *) G.ssid, (const char *) G.passwd);
	delay(300);

	Serial.printf("WLAN-Restart-Status: %s\n", wstatus[wlan_status]);
}

//------------------------------------------------------------------------------

void WlanStart()
{
	Serial.printf("\n-- Begin WlanStart -- \n");

	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	delay(300);
	WiFiScan(false);
	if (wlan_ssid == true)
	{
		WiFiStart_Client();
	}
	//-------------------------------------
	// Zeitanfrage beim NTP-Server
	//-------------------------------------
	if (wlan_client == true)
	{
		AP_Status = 0;  //OFF
		timeClient.begin();
		delay(100);
		timeClient.update();
		delay(100);
		Serial.println(timeClient.getFormattedTime());

		unix_time = timeClient.getEpochTime();
		if (externalRTC == true)
		{
			RTC.adjust(DateTime(unix_time));
		}
	}
	else if (wlan_client == false)
	{
		AP_Status = 1;  // ON > 0
		WiFiStart_AP();
		if (externalRTC == true)
		{
			unix_time = RTC.now().unixtime();
		}
	}
	// Zeit setzen
	setTime(unix_time);
	Serial.printf("-- Ende  WlanStart -- \n\n");
}

//------------------------------------------------------------------------------

void WiFiEvent(WiFiEvent_t event)
{
	Serial.printf("[WiFi-event] event: %d\n", event);

	switch (event)
	{
		case WIFI_EVENT_STAMODE_GOT_IP:
			Serial.println("WiFi connected");
			Serial.println("IP address: ");
			Serial.println(WiFi.localIP());
			break;
		case WIFI_EVENT_STAMODE_DISCONNECTED:
			Serial.println("WiFi lost connection");
			break;
		default:
			break;
	}
}
