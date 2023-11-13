#ifndef TELNET_LOGGER_H
#define TELNET_LOGGER_H

#include <ESP8266WiFi.h>

class TelnetLogger {
public:
    // kluge
    void print_hex(unsigned x) {
      if (connected()) {
        serverClient.print(x, HEX);
      }
    }

    template<class ...Args>
    void print(Args... args) {
      if (connected()) {
        print_raw(args...);
      }
    }

    template<class ...Args>
    void println(Args... args) {
      print(args..., '\n');
    }

    void begin();
    void run();
    bool connected();

private:
    WiFiClient serverClient;

    template<typename first_t, class ...Args>
    void print_raw(first_t const & arg, Args... args) {
      serverClient.print(arg);
      print_raw(args...);
    }

    void print_raw() {}

    template<class ...Args>
    void print_raw(char arg, Args... args) {
        if (arg == '\n')
          serverClient.println();
        else
          serverClient.print(arg);
        print_raw(args...);
    }
};

extern TelnetLogger Log;

#endif  // TELNET_LOGGER_H