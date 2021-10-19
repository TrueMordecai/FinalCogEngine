#ifndef MYSENSOR_H
#define MYSENSOR_H

#include <QLCDNumber>

class mySensor : public QObject
{
    Q_OBJECT

public:
    mySensor();
    mySensor(int lcd_value);
    int getLcdValue();
    QLCDNumber *getLcd();
    void setLcdValue(int value);
    void displayLcdValue(bool autoTreatment = false);

private:
    QLCDNumber *m_lcd;
    int m_lcdValue = 0;
};

#endif // MYSENSOR_H
