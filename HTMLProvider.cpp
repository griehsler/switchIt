#include "HTMLProvider.h"

HTMLProvider::HTMLProvider(Settings *settings)
{
  _settings = settings;
}

String HTMLProvider::commonHead()
{
  return
    "  <head>"
    "    <meta charset=""utf-8"">"
    "    <meta http-equiv=""X-UA-Compatible"" content=""IE=edge"">"
    "    <meta name=""viewport"" content=""width=device-width, initial-scale=1"">"
    "    <link rel=""stylesheet"" href=""https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css"" integrity=""sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u"" crossorigin=""anonymous"">"
    "    <link rel=""stylesheet"" href=""https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css"" integrity=""sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp"" crossorigin=""anonymous"">"
    "    <script src=""https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js""></script>"
    "  </head>";  
}

String HTMLProvider::getIndexHtml()
{
  return
    "<html>"
    + commonHead() +
    "  <body>"
    "    <div class=\"container text-center\">"
    "      <h3>Wi-Fi Light Switch</h3>"
    "      <form action=\"/command\" method=\"get\">"
    "        <input type=\"hidden\" name=\"name\" value=\"switch\" />"
    "        <input type=\"submit\" value=\"Switch\" class=\"btn btn-default btn-lg\" />"
    "      </form>"
    "      <br/>"
    "      <a href=\"/config\">Configure</a>"
    "      <br/>"
    "      <a href=\"/update\">Update Firmware</a>"
    "    </div>"
    "  </body>"
    "</html>";
}

String HTMLProvider::getConfigHtml(String message, String dummyPassword)
{
  return
    "<html>"
    + commonHead() +
    "  <body>"
    "    <div class=\"container\">"
    "      <h2>" + message + "</h2>"
    "      <form action=\"/config\" method=\"post\">"
    "        <fieldset class=\"form-group\">"
    "          <legend>Wi-Fi settings</legend>"
    "          SSID: <br><input type=\"text\" class=\"form-control\" name=\"ssid\" value=\"" + _settings->otherAPSSID + "\"><br>"
    "          Password: <br><input type=\"password\" class=\"form-control\" name=\"password\" value=\"" + dummyPassword + "\"><br>"
    "        </fieldset>"
    "        <fieldset class=\"form-group\">"
    "          <legend>Device Settings</legend>"
    "          Device Name:<br/><input type=\"text\" class=\"form-control\" name=\"devicename\" value=\"" + _settings->deviceName + "\"/><br/>"
    "          HostName:<br/><input type=\"text\" class=\"form-control\" name=\"hostname\" value=\"" + _settings->hostName + "\"/></br/>"
    "          Invert Switch:<br/><input type=\"checkbox\" class=\"form-check-input\" name=\"invertswitch\" value=\"enabled\" " + toChecked( _settings->invertSwitch) + "></br/>"
    "        </fieldset>"
    "        <fieldset class=\"form-group\">"
    "          <legend>Physical Button</legend>"
    "          <input type=\"radio\" class=\"form-check-input\" id=\"bt1\" name=\"buttonmode\" value=\"" + _settings->BUTTON_DISABLED + "\" " + toChecked(_settings->buttonMode == _settings->BUTTON_DISABLED) + "><label class=\"form-check-label\" for=\"bt1\">Disabled</label><br/> "
    "          <input type=\"radio\" class=\"form-check-input\" id=\"bt2\" name=\"buttonmode\" value=\"" + _settings->BUTTON_TOUCH + "\" " + toChecked(_settings->buttonMode == _settings->BUTTON_TOUCH) + "><label class=\"form-check-label\" for=\"bt2\">Touch Sensor</label><br/> "
    "          <input type=\"radio\" class=\"form-check-input\" id=\"bt3\" name=\"buttonmode\" value=\"" + _settings->BUTTON_SWITCH + "\" " + toChecked(_settings->buttonMode == _settings->BUTTON_SWITCH) + "><label class=\"form-check-label\" for=\"bt3\">Switch</label><br/> "
    "        </fieldset>"
    "        <fieldset class=\"form-group\">"
    "          <legend>Syslog Client</legend>"
    "          Enabled:<br/><input type=\"checkbox\" class=\"form-check-input\" name=\"syslogenabled\" value=\"enabled\" " + toChecked(_settings->syslogEnabled) + "><br/>"
    "          Server Host:<br/><input type=\"text\" class=\"form-control\" name=\"syslogserver\" value=\"" + _settings->syslogServer + "\"><br/>"
    "          Server Port:<br/><input type=\"number\" class=\"form-control\" name=\"syslogserverport\" value=\"" + _settings->syslogServerPort + "\"><br/>"
    "        </fieldset>"
    "        <fieldset class=\"form-group\">"
    "          <legend>MQTT</legend>"
    "          Enabled:<br/><input type=\"checkbox\" class=\"form-check-input\" name=\"mqttenabled\" value=\"enabled\" " + toChecked(_settings->mqttEnabled) + "><br/>"
    "          Broker Host:<br/><input type=\"text\" class=\"form-control\" name=\"mqttserver\" value=\"" + _settings->mqttServer + "\"><br/>"
    "          Broker Port:<br/><input type=\"number\" class=\"form-control\" name=\"mqttserverport\" value=\"" + _settings->mqttServerPort + "\"><br/>"
    "          Username:<br/><input type=\"text\" class=\"form-control\" name=\"mqttusername\" value=\"" + _settings->mqttUserName + "\"><br/>"
    "          Password:<br/><input type=\"password\" class=\"form-control\" name=\"mqttpassword\" value=\"" + dummyPassword + "\"><br/>"
    "          Subscribe Topic:<br/><input type=\"text\" class=\"form-control\" name=\"mqttsubscribe\" value=\"" + _settings->mqttSubscribeTopic + "\"><br/>"
    "          Publish Topic:<br/><input type=\"text\" class=\"form-control\" name=\"mqttpublish\" value=\"" + _settings->mqttPublishTopic + "\"><br/>"
    "        </fieldset>"
    "        <fieldset class=\"form-group\">"
    "          <legend>Developer Settings</legend>"
    "          Emulate Relay:<br/><input type=\"checkbox\" class=\"form-check-input\" name=\"emulaterelay\" value=\"enabled\" " + toChecked(_settings->emulateRelay) + "><br/>"
    "        </fieldset>"
    "        <input type=\"hidden\" name=\"operation\" value=\"store\"/>"
    "        <input type=\"submit\" value=\"Save\" class=\"btn btn-primary\">"
    "      </form>"
    "      <form action=\"/config\" method=\"post\">"
    "        <input type=\"hidden\" name=\"operation\" value=\"reset\"/>"
    "        <input type=\"submit\" value=\"Factory Reset\" class=\"btn btn-warning\"/>"
    "      </form>"
    "    </div>"
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
