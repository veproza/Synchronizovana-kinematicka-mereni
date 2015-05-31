#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
SoftwareSerial dbgSerial(6, 7); // RX, TX

class ESP
{
  public:
    ESP(int rstPin);
    void init(Stream *serial);
    bool waitFor(String expected);
    bool waitFor(String expected, unsigned long timeout);
    bool startServer(int port);
    bool sendData(char data[], unsigned int len);
    bool sendData(byte data[], unsigned int len);
    bool sendSendHeader(unsigned int length);
    bool closeConnection();
    bool closeConnection(char connectionId);
    void drain();
  private:
    void reset();
    int _rstPin;
    char currentConnectionId;
    char ipdString[5];
    uint8_t ipdPosition;
    void checkIpd(char currentByte);
    Stream *_serial;
};

ESP::ESP(int rstPin)
{
  _rstPin = rstPin;
  currentConnectionId = '0';
  ipdString[0] = '+';
  ipdString[1] = 'I';
  ipdString[2] = 'P';
  ipdString[3] = 'D';
  ipdString[4] = ',';
}

void ESP::init(Stream *serial)
{
  _serial = serial;
  pinMode(_rstPin, OUTPUT);
  reset();

}
void ESP::reset()
{
  digitalWrite(_rstPin, LOW);
  delay(100);
  digitalWrite(_rstPin, HIGH);
  waitFor("ready", 50000);

}
bool ESP::startServer(int port)
{
  _serial->println("AT+CIPMUX=1");
  if (false == waitFor("OK")) {
    return false;
  }
  _serial->print("AT+CIPSERVER=1,");
  _serial->println(port);
  if (false == waitFor("OK")) {
    return false;
  }
  drain();
  return true;
}

bool ESP::sendData(char data[], unsigned int len)
{
  bool result = sendSendHeader(len);
  if (!result) {
    return false;
  }
  _serial->print(data);
  return waitFor("\r\nSEND OK", 10000);
}
bool ESP::sendData(byte data[], unsigned int len)
{
  bool result = sendSendHeader(len);
  if (!result) {
    return false;
  }
  for (int i = 0; i < len; i++) {
    _serial->write(data[i]);
  }
  return waitFor("\r\nSEND OK", 10000);
}
bool ESP::sendSendHeader(unsigned int length)
{
  _serial->print("AT+CIPSEND=");
  _serial->print(currentConnectionId);
  _serial->print(',');
  _serial->println(length);
  return waitFor(">", 2500);
}
bool ESP::closeConnection()
{
  return closeConnection(currentConnectionId);
}
bool ESP::closeConnection(char connectionId)
{
  _serial->print("AT+CIPCLOSE=");
  _serial->println(connectionId);
  return true;
  //return waitFor("Unlink");
}
void ESP::drain()
{
  while (_serial->available()) {
    _serial->read();
  }
}
bool ESP::waitFor(String expected)
{
  waitFor(expected, 100000);
}
bool ESP::waitFor(String expected, unsigned long timeout)
{
  unsigned long t0 = millis();
  int position = 0;
  int length = expected.length();
  char currentByte;
  char expectedText[127];
  expected.toCharArray(expectedText, length + 1);
  do {
    if (_serial->available()) {
      currentByte = _serial->read();
      checkIpd(currentByte);
      if (currentByte == expectedText[position]) {
        position++;
        if (position == length) {
          return true;
        }
      } else {
        position = 0;
      }
    }
  } while ((millis() - t0) < timeout);
  return false;
}

void ESP::checkIpd(char currentByte)
{
  if (ipdPosition == 5) { // currentByte is now connection ID
    if (currentByte != '0') {
      closeConnection(currentByte);
    }
    ipdPosition = 0;
  }
  if (currentByte == ipdString[ipdPosition]) {
    ipdPosition++;
  } else {
    ipdPosition = 0;
  }
}

bool control = false;

ESP esp(2); // DIO to RST pin
void setup()
{
  dbgSerial.begin(115200);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  dbgSerial.println("Init");
  esp.init(&Serial);
  SD.begin(10);
  if (esp.startServer(80)) {
    dbgSerial.println("Ready.");
  };
}


void loop()
{
  while (Serial.available()) {
    bool result = esp.waitFor("GET ", 200);
    if (result) {
      char address[64];
      if (getRequestPath(address, 64))
      {
        handleAddress(address);
      }
    }
  }
  delay(500);
}
void handleAddress(char address[])
{
  if (0 == strcmp(address, "/status")) {
    dbgSerial.println("STATUS");
    char data[] = "HTTP/1.0 200 OK\r\n\r\nOK";
    esp.sendData(data, strlen(data));
    esp.closeConnection();
  } else if (0 == strcmp(address, "/webservice/control")) {
    char data[] = "HTTP/1.0 200 OK\r\n\r\n0";
    if(control) {
      data[23] = '1';
    }
    esp.sendData(data, strlen(data));
    esp.closeConnection();
  } else if (0 == strcmp(address, "/webservice/control/update?control=1")) {
    control = true;
    char data[] = "HTTP/1.0 200 OK\r\n\r\nOK";
    esp.sendData(data, strlen(data));
    esp.closeConnection();
  } else if (0 == strcmp(address, "/webservice/control/update?control=0")) {
    control = false;
    char data[] = "HTTP/1.0 200 OK\r\n\r\nOK";
    esp.sendData(data, strlen(data));
    esp.closeConnection();
  } else if (0 == strcmp(address, "/")) {
    char prefix[] = "HTTP/1.0 200 OK\r\nContent-type:text/html\r\n\r\n<ul>";
    esp.sendData(prefix, strlen(prefix));
    listFiles();
    char postfix[] = "</ul>";
    esp.sendData(postfix, strlen(postfix));
    esp.closeConnection();
  } else if (address[0] == '/' && address[1] == 'f' && address[2] == '/') {
    String addressString(address);
    char fileAddress[32];
    addressString.substring(3).toCharArray(fileAddress, addressString.length() - 3 + 1);
    sendFile(fileAddress);
    esp.closeConnection();
  } else {
    dbgSerial.println("Unknown");
    char data[] = "HTTP/1.0 404\r\n\r\n";
    esp.sendData(data, strlen(data));
    esp.closeConnection();
  }
}
void sendFile(char * address)
{
  File file = SD.open(address);
  if (file) {
    char prefix[] = "HTTP/1.0 200 OK\r\nContent-type:application/octet-stream\r\n\r\n";
    // TODO: Content-length goes here
    esp.sendData(prefix, strlen(prefix));
    uint8_t counter = 0;
    byte data[32];
    dbgSerial.print("A");
    while (file.available()) {
      data[counter] = file.read();
      counter++;
      if (counter == 32) {
        bool result;
        result = esp.sendData(data, counter);
        counter = 0;
      }
    }
    dbgSerial.print("B");
    if (counter) {
      esp.sendData(data, counter);
    }
    dbgSerial.print("C");
  } else {
    char data[] = "HTTP/1.0 404\r\n\r\n";
    esp.sendData(data, strlen(data));
  }
}

void listFiles()
{
  File root = SD.open("/");
  File entry;
  while (1) {
    entry = root.openNextFile();
    if (!entry) {
      break;
    }
    if (!entry.isDirectory()) {
      String link = "<li><a href='/f/";
      char linkCharArray[64];

      link += entry.name();
      link += "'>";
      link += entry.name();
      link += "</a></li>";
      uint8_t len = link.length();
      link.toCharArray(linkCharArray, len + 1);
      esp.sendData(linkCharArray, len);
    }
  }
  root.close();
  Serial.println("Done");
}

bool getRequestPath(char data[], uint8_t len)
{
  char current;
  int position = 0;
  unsigned long t0 = millis();
  while (millis() - t0 < 200) {
    while (Serial.available()) {
      current = Serial.read();
      if (current == ' ') {
        data[position] = '\0';
        return true;
      }
      if (position == len) {
        return false;
      }
      data[position] = current;
      position++;
    }
  }
  return false;
}
