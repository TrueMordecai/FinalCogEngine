#include "myserialport.h"
#include <QTimer>
#include "mywindow.h"
#include <QObject>
#include <unistd.h>

mySerialPort::mySerialPort()
{
    int sensorDetected = this->tryConection();
    if (this->m_isConnected) {
        for (int i = 0; i != sensorDetected; i++) {
            this->m_myWindow->getSensor().push_back(new mySensor);
        }
        connect(m_arduinoSerialPort, SIGNAL(readyRead()), this, SLOT(parseNewArduinoLine()));
    }
    this->setSensorNumber(sensorDetected);
}

void mySerialPort::setupSerialPort()
{
    this->m_arduinoSerialPort->setBaudRate(QSerialPort::Baud9600);
    this->m_arduinoSerialPort->setDataBits(QSerialPort::Data8);
    this->m_arduinoSerialPort->setParity(QSerialPort::NoParity);
    this->m_arduinoSerialPort->setStopBits(QSerialPort::OneStop);
    this->m_arduinoSerialPort->setFlowControl(QSerialPort::NoFlowControl);
}

int mySerialPort::tryConection()
{
    m_arduinoSerialPort = new QSerialPort();
    foreach (const QSerialPortInfo &portToConnect, QSerialPortInfo::availablePorts()) {
        if (portToConnect.portName().contains("ttyACM")) {
            m_arduinoSerialPort->setPort(portToConnect);
            setupSerialPort();
            bool isOpen = m_arduinoSerialPort->open(QIODevice::ReadWrite);
            if (!isOpen) {
                m_isConnected = false;
                return (0);
            }
            m_arduinoSerialPort->waitForReadyRead();
            std::string buffer = m_arduinoSerialPort->readLine().toStdString();
            qDebug() << buffer.c_str();
            int count = 0;
            for (int i = 0; buffer[i] != '\n'; i++) {
                if (buffer[i] == ';')
                    count++;
            }
            qDebug() << buffer.c_str();
            m_isConnected = true;
            return (count);
        }
    }
    m_isConnected = false;
    return (0);
}

void mySerialPort::parseNewArduinoLine(void)
{
    QByteArray buffer = m_arduinoSerialPort->readLine();
    QVector<QByteArray> values = buffer.split(';').toVector();
    std::vector<mySensor*> sensors = this->m_myWindow->getSensor();
    for (int i = 0; i != values.size() - 1; i++) {
        sensors[i]->setLcdValue(values[i].toInt());
        sensors[i]->displayLcdValue();
    }
}

void mySerialPort::updateLcdValue(int sensor_index, int value)
{
    this->m_myWindow->getSensor()[sensor_index]->setLcdValue(value);
    this->m_myWindow->getSensor()[sensor_index]->displayLcdValue();
}

void mySerialPort::parseNewCsvLine(void)
{
    QString to_parse = this->m_myCsv->getNewLine();
    if (to_parse.isNull())
        return;
    QStringList vectorTabString;
    vectorTabString = to_parse.split(";");
    for (int i = 0; i != this->m_sensorNumber; i++)
        updateLcdValue(i, vectorTabString[i].toInt());
}

int mySerialPort::countSensorFromCsv()
{
    int count = 0;
    std::string buffer = this->m_myCsv->getFileStream();
    for (int i = 0; buffer[i] != '\n'; i++) {
        if (buffer[i] == ';')
            count++;
    }
    return (count);
}

int mySerialPort::countSensorFromArduino()
{
    int count = 0;
    std::string buffer = this->m_myCsv->getFileStream();
    for (int i = 0; buffer[i] != '\n'; i++) {
        if (buffer[i] == ';')
            count++;
    }
    return (count);
}

myCsv *mySerialPort::getCsv()
{
    return (this->m_myCsv);
}

void mySerialPort::setSensorNumber(int i)
{
    this->m_sensorNumber = i;
}

mySerialPort::mySerialPort(myWindow *window)
{
    this->m_myWindow = window;
    int sensorDetected = this->tryConection();
    this->m_myCsv = new myCsv("");
    if (this->m_isConnected) {
        for (int i = 0; i != sensorDetected; i++) {
            this->m_myWindow->getSensor().push_back(new mySensor);
        }
        connect(m_arduinoSerialPort, SIGNAL(readyRead()), this, SLOT(parseNewArduinoLine()));
    }
    this->setSensorNumber(sensorDetected);
}

bool mySerialPort::getIsConnected()
{
    return (this->m_isConnected);
}

int mySerialPort::getSensorNumber()
{
    return (this->m_sensorNumber);
}
