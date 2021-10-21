#include "mysensor.h"
#include <QPalette>
#include <QDebug>

mySensor::mySensor()
{
    this->m_lcd = new QLCDNumber;
    this->m_lcd->setFixedSize(200, 40);
    this->m_optionActivateSensor = new QCheckBox;
    this->m_optionActivateSensor->setText("Activate");
    this->m_optionActivateSensor->setChecked(true);
    this->m_optionEditLimit = new QSpinBox;
    this->m_optionEditLimit->setMinimum(500);
    this->m_optionEditLimit->setMaximum(3000);
    this->m_optionEditLimit->setToolTip("Minimum distance to alarm");
    this->m_optionEditLimit->setValue(800);
    this->m_optionEditLimit->setMinimumWidth(260);
}

void mySensor::displayLcdValue(bool autoTreatment)
{
    QPalette palette;
    if (this->m_lcdValue < this->m_optionEditLimit->value() && autoTreatment) {
        palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::red);
    }
    if (this->m_lcdValue > this->m_optionEditLimit->value() && this->m_lcdValue < 1300 && autoTreatment) {
        palette.setColor(QPalette::Normal, QPalette::Foreground, QColor(255, 127, 0));
    }
    this->m_lcd->setPalette(palette);
    this->m_lcd->display(this->m_lcdValue);
}

void mySensor::updateLastValues(int value)
{
     if (this->m_lastValues.size() == 50) {
         this->m_lastValues.pop_front();
         QString buf;
         for (int i = 0; i < this->m_lastValues.size(); i++) {
             buf.append(std::to_string(m_lastValues[i]).c_str());
             buf += ";";
         }
         qDebug() << buf;
     }
     this->m_lastValues.push_back(value);
}

void mySensor::updateLcd(int value, bool autoTreatment)
{
    setLcdValue(value);
    updateLastValues(value);
    displayLcdValue(autoTreatment);
}

QSpinBox *mySensor::getOptionSpinBox()
{
    return (m_optionEditLimit);
}

QCheckBox *mySensor::getOptionCheckBox()
{
    return (m_optionActivateSensor);
}

QLCDNumber *mySensor::getLcd()
{
    return (this->m_lcd);
}

void mySensor::setLcdValue(int value)
{
    if (!this->m_optionActivateSensor->isChecked()) {
        this->m_lcd->setEnabled(false);
    } else
        this->m_lcd->setEnabled(true);
    this->m_lcdValue = value;
}

int mySensor::getLcdValue()
{
    return (this->m_lcdValue);
}
