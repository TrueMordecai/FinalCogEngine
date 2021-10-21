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
    foreach (const QSerialPortInfo &portToConnect, QSerialPortInfo::availablePorts()) { // Check every port
        if (portToConnect.portName().contains("ttyACM")) { // If found (sensors must begin with ttyACM)
            m_arduinoSerialPort->setPort(portToConnect); // Connect
            setupSerialPort(); // Setup
            bool isOpen = m_arduinoSerialPort->open(QIODevice::ReadWrite); // Try to open
            if (!isOpen) { // If its not open then 0 Sensors availible
                m_isConnected = false;
                return (0);
            }
            m_arduinoSerialPort->waitForReadyRead(); // Wait a line to be read
            std::string buffer = m_arduinoSerialPort->readLine().toStdString();
            int count = 0;
            for (int i = 0; buffer[i] != '\n'; i++) { // Count ; occurences
                if (buffer[i] == ';')
                    count++;
            }
            m_isConnected = true;
            return (count);
        }
    }
    m_isConnected = false; // If no port starting with ttyACM is found
    return (0);
}

void mySerialPort::parseNewArduinoLine(void)
{
    QByteArray buffer = m_arduinoSerialPort->readLine();
    QVector<QByteArray> values = buffer.split(';').toVector();
    std::vector<mySensor*> sensors = this->m_myWindow->getSensor();
    for (int i = 0; i != values.size() - 1; i++) {
        sensors[i]->updateLcd(values[i].toInt(), true);
    }
}

void mySerialPort::parseNewCsvLine(void)
{
    QString to_parse = this->m_myCsv->getNewLine();
    if (to_parse.isNull())
        return;
    QStringList vectorTabString;
    vectorTabString = to_parse.split(";");
    for (int i = 0; i != this->m_sensorNumber; i++)
        this->m_myWindow->getSensor()[i]->updateLcd(vectorTabString[i].toInt(), true);
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
