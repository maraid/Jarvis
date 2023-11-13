#include "telnet_logger.h"

// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(23);

TelnetLogger Log;

void TelnetLogger::begin() {
  telnetServer.begin();
  telnetServer.setNoDelay(true);

}

bool TelnetLogger::connected() {
    return serverClient && serverClient.connected();
}

void TelnetLogger::run() 
{
  if (telnetServer.hasClient()) 
  {
    if (!connected()) 
    {
      if (serverClient) 
      {
        serverClient.stop();
      }
      serverClient = telnetServer.available();
      serverClient.println("Connected to Jarvis");
    }
  }
}