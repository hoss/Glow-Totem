#include <Arduino.h>
#include <Trace.h>
// #include <Adafruit_INA219.h>

// Adafruit_INA219 _powerMonitor;


class PowerMonitor
{
    byte _status{};
    int c1val{};

public:
    void setStatus(byte newStatus);
    byte getStatus();
};

void PowerMonitor::setStatus(byte newStatus)
{
    _status = newStatus;
};

byte PowerMonitor::getStatus()
{
    return _status;
};
