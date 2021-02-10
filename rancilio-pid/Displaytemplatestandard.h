
/********************************************************
    send data to display - Standardtemplate
******************************************************/


void printScreen() 
{
  if (
  (HEATINGLOGO > 0 && kaltstart == 1) ||
  (OFFLINEGLOGO == 1 && pidON == 0)   ||
  (SHOTTIMER == 1 && bezugsZeit > 0) || 
  (SHOTTIMER == 1 && millis() >= bezugszeit_last_Millis && bezugszeit_last_Millis+brewswitchDelay >= millis())) // sobald der Brühschalter umgelegt wird, brewswitchDelay abgelaufen
  return;
  unsigned long currentMillisDisplay = millis();
  if (currentMillisDisplay - previousMillisDisplay >= intervalDisplay) {
    previousMillisDisplay = currentMillisDisplay;
    if (!sensorError) {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_profont11_tf); // set font
      u8g2.drawXBMP(0, 0, logo_width, logo_height, logo_bits_u8g2);   //draw temp icon
      u8g2.setCursor(32, 14);
      u8g2.print("Ist :  ");
      u8g2.print(Input, 1);
      u8g2.print(" ");
      u8g2.print((char)176);
      u8g2.print("C");
      u8g2.setCursor(32, 24);
      u8g2.print("Soll:  ");
      u8g2.print(setPoint, 1);
      u8g2.print(" ");
      u8g2.print((char)176);
      u8g2.print("C");

      // Draw heat bar
      u8g2.drawLine(15, 58, 117, 58);
      u8g2.drawLine(15, 58, 15, 61);
      u8g2.drawLine(117, 58, 117, 61);

      u8g2.drawLine(16, 59, (Output / 10) + 16, 59);
      u8g2.drawLine(16, 60, (Output / 10) + 16, 60);
      u8g2.drawLine(15, 61, 117, 61);

      //draw current temp in icon
      if (fabs(Input  - setPoint) < 0.3) {
        if (isrCounter < 500) {
          u8g2.drawLine(9, 48, 9, 58 - (Input  / 2));
          u8g2.drawLine(10, 48, 10, 58 - (Input  / 2));
          u8g2.drawLine(11, 48, 11, 58 - (Input  / 2));
          u8g2.drawLine(12, 48, 12, 58 - (Input  / 2));
          u8g2.drawLine(13, 48, 13, 58 - (Input  / 2));
        }
      } else if (Input > 106) {
        u8g2.drawLine(9, 48, 9, 5);
        u8g2.drawLine(10, 48, 10, 4);
        u8g2.drawLine(11, 48, 11, 3);
        u8g2.drawLine(12, 48, 12, 4);
        u8g2.drawLine(13, 48, 13, 5);
      } else {
        u8g2.drawLine(9, 48, 9, 58 - (Input  / 2));
        u8g2.drawLine(10, 48, 10, 58 - (Input  / 2));
        u8g2.drawLine(11, 48, 11, 58 - (Input  / 2));
        u8g2.drawLine(12, 48, 12, 58 - (Input  / 2));
        u8g2.drawLine(13, 48, 13, 58 - (Input  / 2));
      }

      //draw setPoint line
      u8g2.drawLine(18, 58 - (setPoint / 2), 23, 58 - (setPoint / 2));

      // PID Werte ueber heatbar
      u8g2.setCursor(40, 48);

      u8g2.print(bPID.GetKp(), 0); // P
      u8g2.print("|");
      if (bPID.GetKi() != 0) {
        u8g2.print(bPID.GetKp() / bPID.GetKi(), 0);;
      } // I
      else
      {
        u8g2.print("0");
      }
      u8g2.print("|");
      u8g2.print(bPID.GetKd() / bPID.GetKp(), 0); // D
      u8g2.setCursor(98, 48);
      if (Output < 99) {
        u8g2.print(Output / 10, 1);
      } else {
        u8g2.print(Output / 10, 0);
      }
      u8g2.print("%");

      // Brew
      u8g2.setCursor(32, 34);
      u8g2.print("Brew:  ");
      u8g2.print(bezugsZeit / 1000, 0);
      u8g2.print("/");
      if (ONLYPID == 1) {
        u8g2.print(brewtimersoftware, 0);             // deaktivieren wenn Preinfusion ( // voransetzen )
      }
      else
      {
        u8g2.print(totalbrewtime / 1000, 1);            // aktivieren wenn Preinfusion und eine Nachkommastelle oder alternativ keine
      }
      //draw box
      u8g2.drawFrame(0, 0, 128, 64);

      // Für Statusinfos
      u8g2.drawFrame(32, 0, 84, 12);
      if (Offlinemodus == 0) {
        getSignalStrength();
        if (WiFi.status() == WL_CONNECTED) {
          u8g2.drawXBMP(40, 2, 8, 8, antenna_OK_u8g2);
          for (int b = 0; b <= bars; b++) {
            u8g2.drawVLine(45 + (b * 2), 10 - (b * 2), b * 2);
          }
        } else {
          u8g2.drawXBMP(40, 2, 8, 8, antenna_NOK_u8g2);
          u8g2.setCursor(88, 2);
          u8g2.print("RC: ");
          u8g2.print(wifiReconnects);
        }
        if (Blynk.connected()) {
          u8g2.drawXBMP(60, 2, 11, 8, blynk_OK_u8g2);
        } else {
          u8g2.drawXBMP(60, 2, 8, 8, blynk_NOK_u8g2);
        }
        if (MQTT == 1) {
          if (mqtt.connect(hostname, mqtt_username, mqtt_password)) { 
            u8g2.setCursor(77, 2);
            u8g2.print("MQTT");
          } else {
            u8g2.setCursor(77, 2);
            u8g2.print("");
          }
        }
      } else {
        u8g2.setCursor(40, 2);
        u8g2.print("Offlinemodus");
      }
      u8g2.sendBuffer();
    }
  }
}
