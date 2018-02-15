#ifndef OLEDTEMP_H
#define OLEDTEMP_H

#include <WString.h>

class OLEDTemp {
  public:
    OLEDTemp();
    ~OLEDTemp();
    void setupDisplay();
    void displayWiFiInfo(String ssid, String ipAdress);
    void print(String msg);
}
#endif
