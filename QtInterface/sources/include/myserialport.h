#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include "mycsv.h"
#include <QTimer>
#include <QtWidgets>
#include <QSerialPort>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <fstream>
#include <bitset>
#include <QTime>
#include <QString>
#include <QDataStream>

class myWindow;

class mySerialPort : public QWidget
{
    Q_OBJECT
public:

    mySerialPort();
    mySerialPort(myWindow *winow);


    bool getIsConnected();
    int getSensorNumber();
    void setSensorNumber(int i);
    int tryConection();
    void doConnectLcd();
    int countSensorFromCsv();
    int countSensorFromArduino();
    void updateLcdValue();
    void setupSerialPort();
    myCsv *getCsv();

public slots :
    void updateLcdValue(int sensor_index, int value);
    void parseNewCsvLine(void);
    void parseNewArduinoLine(void);

private:
    myWindow *m_myWindow;
    myCsv *m_myCsv;
    int m_sensorNumber;
    bool m_isConnected;
    QTimer *m_updateTimer;
    QSerialPort *m_arduinoSerialPort;
};

#endif // MYSERIALPORT_H
