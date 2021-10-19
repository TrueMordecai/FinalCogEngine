#include "mysensor.h"
#include <QPalette>
#include <QDebug>

mySensor::mySensor()
{
    this->m_lcd = new QLCDNumber;
    this->m_lcd->setFixedSize(200, 40);
}

void mySensor::displayLcdValue(bool autoTreatment)
{
    QPalette palette;
    if (this->m_lcdValue < 800 && autoTreatment) {
        palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::red);
    }
    this->m_lcd->setPalette(palette);
    this->m_lcd->display(this->m_lcdValue);
}

/// For testing purpose only
mySensor::mySensor(int lcd_value)
{
    this->m_lcdValue = lcd_value;
    this->m_lcd = new QLCDNumber;
}

QLCDNumber *mySensor::getLcd()
{
    return (this->m_lcd);
}

void mySensor::setLcdValue(int value)
{
    this->m_lcdValue = value;
}

int mySensor::getLcdValue()
{
    return (this->m_lcdValue);
}
