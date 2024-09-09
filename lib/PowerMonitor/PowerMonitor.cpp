#include <Arduino.h>
#include <Trace.h>
#include <Adafruit_INA219.h>

class PowerMonitor
{
    // const uint DELAY_BETWEEN_UPDATES = 6000;
    uint32_t _timeOfLastUpdate = 0;
    Adafruit_INA219 _powerMonitor;
    bool _powerMonitorHasBegun = false;
    float _totalPowerConsumed = 0;
    const float SECONDS_IN_AN_HOUR = 3600.0; // float to help with calcs
    const uint BATTERY_CAPACITY = 74000.0; // mWh
public:
    void init();
    String getPowerUse();
};

void PowerMonitor::init()
{
    if (!_powerMonitor.begin())
    {
        // ("Failed to find INA219 chip to use as _powerMonitor");
    }
    else
        _powerMonitorHasBegun = true;
    // timer.every(8000, getPowerUse);
}

String PowerMonitor::getPowerUse()
{
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    shuntvoltage = _powerMonitor.getShuntVoltage_mV();
    busvoltage = _powerMonitor.getBusVoltage_V();
    current_mA = _powerMonitor.getCurrent_mA();
    power_mW = _powerMonitor.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    uint32_t timeNow = millis();
    float secondsSinceLastUpdate = (timeNow - _timeOfLastUpdate) / 1000.0;
    float mWsUsedSinceLastUpdate = power_mW * secondsSinceLastUpdate;
    float mWhUsedSinceLastUpdate = mWsUsedSinceLastUpdate / SECONDS_IN_AN_HOUR;
    _timeOfLastUpdate = timeNow;
    _totalPowerConsumed += mWhUsedSinceLastUpdate;
    float percBatteryConsumed = _totalPowerConsumed/BATTERY_CAPACITY;

    String displayTxt = "\n\n";
    //   displayTxt += ("Bus Voltage:   ");
    //   displayTxt += (busvoltage);
    //   displayTxt += (" V\n");
    //   displayTxt += ("Shunt Voltage: ");
    //   displayTxt += (shuntvoltage);
    //   displayTxt += (" mV\n");
    displayTxt += ("Load Voltage:  ");
    displayTxt += (loadvoltage);
    displayTxt += (" V\n");
    // displayTxt += ("Current:       ");
    // displayTxt += (current_mA);
    // displayTxt += (" mA\n");
    displayTxt += ("Power:         ");
    displayTxt += (power_mW);
    displayTxt += (" mW\n");
    // displayTxt += ("mWsUsedSinceLastUpdate:   ");
    // displayTxt += (mWsUsedSinceLastUpdate);
    // displayTxt += (" mWs\n");
    displayTxt += ("Total power:   ");
    displayTxt += (_totalPowerConsumed);
    displayTxt += (" mWh\n");
    displayTxt += ("Battery used:  ");
    displayTxt += (percBatteryConsumed);
    displayTxt += ("%");
    // displayTxt += ("secondsSinceLastUpdate:   ");
    // displayTxt += (secondsSinceLastUpdate);
    // displayTxt += (" seconds\n");

    return (displayTxt);
}
