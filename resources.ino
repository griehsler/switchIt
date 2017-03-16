String getIndexHtml()
{
  return
    "<html>"
    "  <body>"
    "   <H1>WIFI Light Switch</H1>"
    "    <form action=\"/command\" method=\"get\">"
    "      <input type=\"hidden\" name=\"name\" value=\"switch\" />"
    "      <input type=\"submit\" value=\"Switch\" />"
    "    </form>"
    "    <br/>"
    "    <a href=\"/config\">Configure</a>"
    "    <br/>"
    "    <a href=\"/update\">Update Firmware</a>"
    "  </body>"
    "</html>";
}

String getConfigHtml(String message)
{
  return
    "<html>"
    "  <body>"
    "    <h2>" + message + "</h2>"
    "    <form action=\"/config\" method=\"post\">"
    "      SSID: <br><input type=\"text\" name=\"ssid\" value=\"" + otherAPSSID + "\"><br>"
    "      Password: <br><input type=\"password\" name=\"password\"><br>"
    "      <input type=\"hidden\" name=\"kind\" value=\"wifi\"/>"
    "      <input type=\"submit\" value=\"Save\">"
    "    </form>"
    "    <form action=\"/config\" method=\"post\">"
    "      Device Name:<br/><input type=\"text\" name=\"devicename\" value=\"" + deviceName + "\"/><br/>"
    "      HostName:<br/>http://<input type=\"text\" name=\"hostname\" value=\"" + hostName + "\"/>.local</br/>"
    "      <input type=\"hidden\" name=\"kind\" value=\"identity\"/>"
    "      <input type=\"submit\" value=\"Save\">"
    "    </form>"
    "    <form action=\"/config\" method=\"post\">"
    "      <input type=\"hidden\" name=\"kind\" value=\"reset\"/>"
    "      <input type=\"submit\" value=\"Factory Reset\"/>"
    "    </form>"
    "  </body>"
    "</html>";
}

String getEventServiceXml()
{
  return
    "<?scpd xmlns=\"urn:Belkin:service-1-0\"?>"
    "  <actionList>"
    "    <action>"
    "      <name>SetBinaryState</name>"
    "      <argumentList>"
    "        <argument>"
    "          <retval/>"
    "          <name>BinaryState</name>"
    "          <relatedStateVariable>BinaryState</relatedStateVariable>"
    "          <direction>in</direction>"
    "        </argument>"
    "      </argumentList>"
    "      <serviceStateTable>"
    "        <stateVariable sendEvents=\"yes\">"
    "          <name>BinaryState</name>"
    "          <dataType>Boolean</dataType>"
    "          <defaultValue>0</defaultValue>"
    "        </stateVariable>"
    "        <stateVariable sendEvents=\"yes\">"
    "          <name>level</name>"
    "          <dataType>string</dataType>"
    "          <defaultValue>0</defaultValue>"
    "        </stateVariable>"
    "      </serviceStateTable>"
    "    </action>"
    "  </actionList>"
    "</scpd>\r\n\r\n";
}

String getSetupXml(String name, String uuid, String serialNumber)
{
  return
    "<?xml version=\"1.0\"?>"
    "<root>"
    "  <device>"
    "    <deviceType>urn:Belkin:device:controllee:1</deviceType>"
    "    <friendlyName>" + name + "</friendlyName>"
    "    <manufacturer>Belkin International Inc.</manufacturer>"
    "    <modelName>Emulated Socket</modelName>"
    "    <modelNumber>3.1415</modelNumber>"
    "    <UDN>uuid:" + uuid + "</UDN>"
    "    <serialNumber>" + serialNumber + "</serialNumber>"
    "    <binaryState>0</binaryState>"
    "    <serviceList>"
    "      <service>"
    "        <serviceType>urn:Belkin:service:basicevent:1</serviceType>"
    "        <serviceId>urn:Belkin:serviceId:basicevent1</serviceId>"
    "        <controlURL>/upnp/control/basicevent1</controlURL>"
    "        <eventSubURL>/upnp/event/basicevent1</eventSubURL>"
    "        <SCPDURL>/eventservice.xml</SCPDURL>"
    "      </service>"
    "    </serviceList>"
    "  </device>"
    "</root>\r\n\r\n";
}

String getSsdpSearchResponse(String serverUrl, String uuid, String st)
{
  return
    "HTTP/1.1 200 OK\r\n"
    "CACHE-CONTROL: max-age=86400\r\n"
    "DATE: Fri, 15 Apr 2016 04:56:29 GMT\r\n"
    "EXT:\r\n"
    "LOCATION: http://" + serverUrl + "/setup.xml\r\n"
    "OPT: \"http://schemas.upnp.org/upnp/1/0/\"; ns=01\r\n"
    "01-NLS: b9200ebb-736d-4b93-bf03-835149d13983\r\n"
    "SERVER: Unspecified, UPnP/1.0, Unspecified\r\n"
    "ST: " + st + "\r\n"
    "USN: uuid:" + uuid + "::" + st + "\r\n"
    "X-User-Agent: redsonic\r\n\r\n";
}
