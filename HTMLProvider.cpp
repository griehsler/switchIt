#include "HTMLProvider.h"

HTMLProvider::HTMLProvider(Settings *settings)
{
  _settings = settings;
}

String HTMLProvider::getIndexHtml()
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

String HTMLProvider::getConfigHtml(String message, String dummyPassword)
{
  return
    "<html>"
    "  <body>"
    "    <h2>" + message + "</h2>"
    "    <form action=\"/config\" method=\"post\">"
    "      <fieldset>"
    "        <legend>WiFi settings</legend>"
    "        SSID: <br><input type=\"text\" name=\"ssid\" value=\"" + _settings->otherAPSSID + "\"><br>"
    "        Password: <br><input type=\"password\" name=\"password\" value=\"" + dummyPassword + "\"><br>"
    "      </fieldset>"
    "      <fieldset>"
    "        <legend>Device settings</legend>"
    "        Device Name:<br/><input type=\"text\" name=\"devicename\" value=\"" + _settings->deviceName + "\"/><br/>"
    "        HostName:<br/><input type=\"text\" name=\"hostname\" value=\"" + _settings->hostName + "\"/></br/>"
    "      </fieldset>"
    "      <fieldset>"
    "        <legend>Physical Button</legend>"
    "        <input type=\"radio\" id=\"bt1\" name=\"buttonmode\" value=\"" + _settings->BUTTON_DISABLED + "\" " + toChecked(_settings->buttonMode == _settings->BUTTON_DISABLED) + "><label for=\"bt1\">Disabled</label><br/> "
    "        <input type=\"radio\" id=\"bt2\" name=\"buttonmode\" value=\"" + _settings->BUTTON_TOUCH + "\" " + toChecked(_settings->buttonMode == _settings->BUTTON_TOUCH) + "><label for=\"bt2\">Touch Sensor</label><br/> "
    "        <input type=\"radio\" id=\"bt3\" name=\"buttonmode\" value=\"" + _settings->BUTTON_SWITCH + "\" " + toChecked(_settings->buttonMode == _settings->BUTTON_SWITCH) + "><label for=\"bt3\">Switch</label><br/> "
    "      </fieldset>"
    "      <fieldset>"
    "        <legend>MQTT</legend>"
    "        Enabled:<br/><input type=\"checkbox\" name=\"mqttenabled\" value=\"enabled\" " + toChecked(_settings->mqttEnabled) + "><br/>"
    "        Broker Host:<br/><input type=\"text\" name=\"mqttserver\" value=\"" + _settings->mqttServer + "\"><br/>"
    "        Broker Port:<br/><input type=\"number\" name=\"mqttserverport\" value=\"" + _settings->mqttServerPort + "\"><br/>"
    "        Username:<br/><input type=\"text\" name=\"mqttusername\" value=\"" + _settings->mqttUserName + "\"><br/>"
    "        Password:<br/><input type=\"password\" name=\"mqttpassword\" value=\"" + dummyPassword + "\"><br/>"
    "        Subscribe Topic:<br/><input type=\"text\" name=\"mqttsubscribe\" value=\"" + _settings->mqttSubscribeTopic + "\"><br/>"
    "        Publish Topic:<br/><input type=\"text\" name=\"mqttpublish\" value=\"" + _settings->mqttPublishTopic + "\"><br/>"
    "      </fieldset>"
    "      <fieldset>"
    "        <legend>Developer Settings</legend>"
    "        Emulate Relay:<br/><input type=\"checkbox\" name=\"emulaterelay\" value=\"enabled\" " + toChecked(_settings->emulateRelay) + "><br/>"
    "      </fieldset>"
    "      <input type=\"hidden\" name=\"operation\" value=\"store\"/>"
    "      <input type=\"submit\" value=\"Save\">"
    "    </form>"
    "    <form action=\"/config\" method=\"post\">"
    "      <input type=\"hidden\" name=\"operation\" value=\"reset\"/>"
    "      <input type=\"submit\" value=\"Factory Reset\"/>"
    "    </form>"
    "  </body>"
    "</html>";
}

String HTMLProvider::getEventServiceXml()
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

String HTMLProvider::getSetupXml(String name, String uuid, String serialNumber)
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

String HTMLProvider::getSsdpSearchResponse(String serverUrl, String uuid, String st)
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

String HTMLProvider::toChecked(bool value)
{
  if (value)
    return "checked=\"checked\"";

  return "";
}
