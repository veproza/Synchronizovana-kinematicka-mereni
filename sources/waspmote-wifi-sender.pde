/*
 *  Based on Waspmote WIFI_14 Example
 *
 */

// Include WIFI library
#include <WaspWIFI.h>

// choose socket (SELECT USER'S SOCKET)
///////////////////////////////////////
uint8_t socket=SOCKET0;
///////////////////////////////////////

// WiFi AP settings (CHANGE TO USER'S AP)
/////////////////////////////////
#define ESSID ""
#define AUTHKEY ""
/////////////////////////////////

#define IP_ADDRESS "192.168.6.117"
#define REMOTE_PORT 9988
#define LOCAL_PORT 9988

#define DATA_LENGTH 2048
char data[DATA_LENGTH];

#define BATCH_LENGTH 128
bool status = false;

void setup()
{
  // Switch ON the WiFi module on the desired socket
  if( WIFI.ON(socket) == 1 )
  {
    USB.println(F("Wifi switched ON"));
  }
  else
  {
    USB.println(F("Wifi did not initialize correctly"));
  }

  // 1. Configure the transport protocol (UDP, TCP, FTP, HTTP...)
  WIFI.setConnectionOptions(CLIENT);
  // 2. Configure the way the modules will resolve the IP address.
  WIFI.setDHCPoptions(DHCP_ON);

  // *** Wifi Protected Access 2 (WPA 2) ***
  // 3. Sets WPA2-PSK encryptation // 1-64 Character
  WIFI.setAuthKey(WPA2, AUTHKEY);

  // 4. Configure how to connect the AP
  WIFI.setJoinMode(MANUAL);
  // 5. Store Values
  WIFI.storeData();
  // Call join the AP
  if (WIFI.join(ESSID))
  {
    USB.println(F("joined AP"));
    status = true;
    // Displays Access Point status.
    USB.println(F("\n----------------------"));
    USB.println(F("AP Status:"));
    USB.println(F("----------------------"));
    WIFI.getAPstatus();

    // Displays IP settings.
    USB.println(F("\n----------------------"));
    USB.println(F("IP Settings:"));
    USB.println(F("----------------------"));
    WIFI.getIP();
    USB.println();
  }
  else
  {
    USB.println(F("not joined"));
  }
  for(int j = 0; j < DATA_LENGTH; ++j) {
    data[j] = 33 + (j % 92);
  }
}


void loop()
{
  if(status == true) {
    if (WIFI.setTCPclient(IP_ADDRESS, REMOTE_PORT, LOCAL_PORT))
    {
//        USB.println(F("TCP client set"));
        unsigned long times[BATCH_LENGTH];
        for(int i = 0; i < BATCH_LENGTH; ++i) {
          unsigned long t1 = millis();
          WIFI.send(data);
          unsigned long t2 = millis();
          times[i] = t2 - t1;
        }
        unsigned long t1 = millis();
        WIFI.close();
        unsigned long t2 = millis();
        for(int i = 0; i < BATCH_LENGTH; ++i) {
          USB.println(times[i]);
        }
        USB.print("\t");
        USB.println(t2 - t1);

    }
    else
    {
        USB.println(F("TCP client NOT set"));
    }
  }
}

