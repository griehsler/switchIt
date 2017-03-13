#include <WiFiUdp.h>
#include <functional>

WiFiUDP UDP;
IPAddress ipMulti(239, 255, 255, 250);
unsigned int portMulti = 1900;      // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet

String serial;
String persistentUuid;
String deviceName = "Licht";

boolean connectUDP()
{
  boolean state = false;

  Serial.print("Connecting to UDP ... ");

  if (UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti))
  {
    Serial.println("successful");
    state = true;
  }
  else
  {
    Serial.println("failed!");
  }

  return state;
}

void respondToSearch() {
#ifdef DEBUG
  Serial.print("Responding to search request back to ");
  Serial.print(UDP.remoteIP());
  Serial.print(":");
  Serial.println(UDP.remotePort());
#endif

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

  String response =
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + String(s) + ":" + String(httpServerPort) + "/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: urn:Belkin:device:**\r\n"
    "USN: uuid:" + persistentUuid + "::urn:Belkin:device:**\r\n"
    "X-User-Agent: redsonic\r\n\r\n";

  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(response.c_str());
  UDP.endPacket();

#ifdef DEBUG
  Serial.println("Response sent");
#endif
}

void handleUPNP()
{
  int packetSize = UDP.parsePacket();

  if (packetSize)
  {
    int len = UDP.read(packetBuffer, 255);
    if (len > 0)
      packetBuffer[len] = 0;

    String request = packetBuffer;

#ifdef FULLDEBUG
    Serial.println("");
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");

    IPAddress remote = UDP.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
        Serial.print(".");
    }

    Serial.print(", port ");
    Serial.println(UDP.remotePort());
    Serial.println("Request:");
    Serial.println(request);
#endif

    if (request.indexOf('M-SEARCH') > 0 && request.indexOf("urn:Belkin:device:**") > 0)
    {
      respondToSearch();
    }
  }
}

void prepareIds()
{
  uint32_t chipId = ESP.getChipId();
  char uuid[64];
  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
            (uint16_t) ((chipId >> 16) & 0xff),
            (uint16_t) ((chipId >>  8) & 0xff),
            (uint16_t)   chipId        & 0xff);

  serial = String(uuid);
  persistentUuid = "Socket-1_0-" + serial;
}

void extendWebServer()
{
  server.on("/upnp/control/basicevent1", HTTP_POST, []()
  {
    String request = server.arg(0);

#ifdef DEBUG
    Serial.println("Responding to  /upnp/control/basicevent1 ...");

    for (int x = 0; x <= server.args(); x++) {
      Serial.println(server.arg(x));
    }

    Serial.print("request:");
    Serial.println(request);
#endif

    if (request.indexOf("<BinaryState>1</BinaryState>") > 0) {
      Serial.println("Got Turn on request via UPNP");
      executeCommand(CMD_ON);
    }

    if (request.indexOf("<BinaryState>0</BinaryState>") > 0) {
      Serial.println("Got Turn off request via UPNP");
      executeCommand(CMD_OFF);
    }

    server.send(200, "text/plain", "");
  }
           );

  server.on("/eventservice.xml", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Responding to eventservice.xml ...");
#endif

    String eventservice_xml = "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
                              "<actionList>"
                              "<action>"
                              "<name>SetBinaryState</name>"
                              "<argumentList>"
                              "<argument>"
                              "<retval/>"
                              "<name>BinaryState</name>"
                              "<relatedStateVariable>BinaryState</relatedStateVariable>"
                              "<direction>in</direction>"
                              "</argument>"
                              "</argumentList>"
                              "<serviceStateTable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>BinaryState</name>"
                              "<dataType>Boolean</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "<stateVariable sendEvents=\"yes\">"
                              "<name>level</name>"
                              "<dataType>string</dataType>"
                              "<defaultValue>0</defaultValue>"
                              "</stateVariable>"
                              "</serviceStateTable>"
                              "</action>"
                              "</scpd>\r\n"
                              "\r\n";

    server.send(200, "text/plain", eventservice_xml.c_str());
  }
           );

  server.on("/setup.xml", HTTP_GET, []()
  {
#ifdef DEBUG
    Serial.println("Responding to setup.xml ...");
#endif

    IPAddress localIP = WiFi.localIP();
    char s[16];
    sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

    String setup_xml = "<?xml version=\"1.0\"?>"
                       "<root>"
                       "<device>"
                       "<deviceType>urn:Belkin:device:controllee:1</deviceType>"
                       "<friendlyName>" + deviceName + "</friendlyName>"
                       "<manufacturer>Belkin International Inc.</manufacturer>"
                       "<modelName>Emulated Socket</modelName>"
                       "<modelNumber>3.1415</modelNumber>"
                       "<UDN>uuid:" + persistentUuid + "</UDN>"
                       "<serialNumber>221517K0101769</serialNumber>"
                       "<binaryState>0</binaryState>"
                       "<serviceList>"
                       "<service>"
                       "<serviceType>urn:Belkin:service:basicevent:1</serviceType>"
                       "<serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
                       "<controlURL>/upnp/control/basicevent1</controlURL>"
                       "<eventSubURL>/upnp/event/basicevent1</eventSubURL>"
                       "<SCPDURL>/eventservice.xml</SCPDURL>"
                       "</service>"
                       "</serviceList>"
                       "</device>"
                       "</root>\r\n"
                       "\r\n";

    server.send(200, "text/xml", setup_xml.c_str());

#ifdef DEBUG
    Serial.println("Sending:");
    Serial.println(setup_xml);
#endif
  }
           );
}

