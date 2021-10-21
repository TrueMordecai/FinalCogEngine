#ifndef MYSENSOR_H
#define MYSENSOR_H

#include <QLCDNumber>
#include <QSpinBox>
#include <QCheckBox>

class mySensor : public QObject
{
    Q_OBJECT
public:
    mySensor();
    int getLcdValue();
    QLCDNumber *getLcd();
    void setLcdValue(int value);
    void displayLcdValue(bool autoTreatment = false);
    void updateLastValues(int value);
    void updateLcd(int value, bool autoTreatment = false);
    QSpinBox *getOptionSpinBox();
    QCheckBox *getOptionCheckBox();

private:
    QLCDNumber *m_lcd;
    QList<int> m_lastValues;
    int m_lcdValue = 0;
    bool m_isOperational = true;
    int m_hardLimit = 800;
    QSpinBox *m_optionEditLimit;
    QCheckBox *m_optionActivateSensor;
};

#endif // MYSENSOR_H
