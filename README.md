# NTPClient

Connect to an NTP server from an ESP32, and show the time

I just added a few extra functions to get the date as well as time.


## Example Ardunio Sketch

ntp_test.ino

```cpp

#include <WiFi.h>
#include "NTPClient.h"
#include "WiFiUdp.h"
#include "Time.h"

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char *ntpServer = "pool.ntp.org"; // Select your favorite NTP server pool
const int timeZone = 10; // 10 is Australia, East coast - NOTE: NO DST in this library

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer);

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();

  timeClient.setTimeOffset(3600 * timeZone);
  timeClient.setUpdateInterval(60000); // 1 minute

}
 
void loop() {
  tm_struct tm;
  while (1) {
    if (!timeClient.isTimeSet()) {
      Serial.println("Forcing NTP Update");
      timeClient.forceUpdate();
      delay(1500);
    }
    timeClient.update();
    if (timeClient.getTime(&tm, 0) >= 0) {
      Serial.println("Year: " + String(tm.year));
      Serial.println("Month: " + String(tm.month));
      Serial.println("Day: " + String(tm.day));
      Serial.println("Hour: " + String(tm.hour));
      Serial.println("Minute: " + String(tm.minute));
      Serial.println("Seconds: " + String(tm.second));
      Serial.println("ISO? " + timeClient.getISODate());
      Serial.println("Format 24hr : " + timeClient.getFormattedTime());
      Serial.println("Format 12hr : " + timeClient.getFormattedTime(0, true));
    }
    delay(1000);
  }  
}

```
