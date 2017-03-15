String getIndexHtml()
{
  return
    "<html>\
      <body>\
        <H1>WIFI Light Switch</H1>\
        <form action=\"/command\" method=\"get\">\
          <input type=\"hidden\" name=\"name\" value=\"switch\" />\
          <input type=\"submit\" value=\"Switch\" />\
        </form>\
        <br/>\
        <a href=\"/config\">Configure</a>\
        <br/>\
        <a href=\"/update\">Update Firmware</a>\
      </body>\
    </html>";
}

String getConfigHtml(String message)
{
  return
    "<html>\
      <body>\
        <h2>" + message + "</h2>\
        <form action=\"/config\" method=\"post\">\
          SSID: <br><input type=\"text\" name=\"ssid\" value=\"" + otherAPSSID + "\"><br>\
          Password: <br><input type=\"password\" name=\"password\"><br>\
          <input type=\"hidden\" name=\"kind\" value=\"wifi\"/>\
          <input type=\"submit\" value=\"Save\">\
        </form>\
        <form action=\"/config\" method=\"post\">\
          Device Name:<br/><input type=\"text\" name=\"devicename\" value=\"" + deviceName + "\"/><br/>\
          HostName:<br/>http://<input type=\"text\" name=\"hostname\" value=\"" + hostName + "\"/>.local</br/>\
          <input type=\"hidden\" name=\"kind\" value=\"identity\"/>\
          <input type=\"submit\" value=\"Save\">\
        </form>\
        <form action=\"/config\" method=\"post\">\
          <input type=\"hidden\" name=\"kind\" value=\"reset\"/>\
          <input type=\"submit\" value=\"Factory Reset\"/>\
        </form>\
      </body>\
    </html>";
}

