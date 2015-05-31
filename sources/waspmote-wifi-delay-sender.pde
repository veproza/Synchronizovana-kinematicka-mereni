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
  }
  else
  {
    USB.println(F("not joined"));
  }
}


void loop()
{
  if(status == true) {
    if (WIFI.setTCPclient(IP_ADDRESS, REMOTE_PORT, LOCAL_PORT))
    {


        for(int i = 0; i < BATCH_LENGTH; ++i) {
          unsigned long t1 = millis();
          char number[20];
          Utils.long2array(t1, number);
          WIFI.send(number);
          USB.print(".");
          delay(1000);
        }
        WIFI.close();
    }
    else
    {
        USB.println(F("TCP client NOT set"));
    }
  }
}

