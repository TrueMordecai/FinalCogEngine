#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <fstream>
#include <bitset>
#include <QTime>
#include <QString>
#include <QDataStream>
#include <QObject>
#include <QThread>
#include <myplot.h>
#include <QElapsedTimer>
#include <QTimer>
#include<QtMultimedia/QSound>
#include <capteur.h>


class SerialPort:public QObject
{

    Q_OBJECT

public:

    SerialPort();
    SerialPort(QObject *parent, QElapsedTimer& _timer);
    ~SerialPort();
    std::vector<int> getVec1() const;
    void setVec1(const std::vector<int> &value);
    std::vector<int> getVec3() const;
    void setVec3(const std::vector<int> &value);
    std::vector<int> getVec2() const;
    void setVec2(const std::vector<int> &value);
    std::vector<int> getAlerte() const;
    void setAlerte(const std::vector<int> &value);
    QSerialPort *getArduinoSerialPort() const;
    void setArduinoSerialPort(QSerialPort *arduinoSerialPort);
    bool isObject();
    QElapsedTimer getTimerNoMove() const;
    void setTimerNoMove(const QElapsedTimer &value);
    int getDistanceLim() const;
    void setDistanceLim(int value);
    std::vector<int> getVecAverage2() const;
    void setVecAverage2(const std::vector<int> &value);
    std::vector<int> getVecAverage1() const;
    void setVecAverage1(const std::vector<int> &value);
    std::vector<int> getVitesse2() const;
    void setVitesse2(const std::vector<int> &value);
    std::vector<int> getVitesse1() const;
    void setVitesse1(const std::vector<int> &value);
    std::vector<int> getMoyVitesse2() const;
    void setMoyVitesse2(const std::vector<int> &value);
    std::vector<int> getAlerte2() const;
    void setAlerte2(const std::vector<int> &value);
    std::vector<std::vector<int> > read_csv(std::string filename);
    bool getScenarioCSV() const;
    void setScenarioCSV(bool value);
    void ProcessPostData(std::vector<int> vect1, std::vector<int> vect2, std::vector<int> vect3, std::vector<int> vect4);
    std::vector<std::vector<int> > getResultCSV() const;
    void setResultCSV(const std::vector<std::vector<int> > &value);
    int getCont() const;
    void setCont(int value);
    int getNumberOfPressButton() const;
    void setNumberOfPressButton(int value);
    int getConteurVariation() const;
    void setConteurVariation(int conteurVariation);
    bool getHasObject() const;
    void setHasObject(bool value);
    QSound *getSound() const;
    void setSound(QSound *value);
    Capteur *getCapteur2() const;
    void setCapteur2(Capteur *value);
    Capteur *getCapteur1() const;
    void setCapteur1(Capteur *value);
    std::vector<Capteur *> getCapteurList() const;
    void setCapteurList(const std::vector<Capteur *> &value);

public slots:
    void SlotTraitementArduino();
    void SlotProcessPostData();

private:
    void fifo(std::vector<int>& vect);
    QSerialPort *m_arduinoSerialPort;
    std::vector<int> tabSensorInt;
    std::vector<int>tabSensor2Int;
    std::vector<QString> tabSensor;
    std::vector<QString> tabSensor2;
    int compteur=0;
    QString dataString = "";
    int numberOfBytes= 0;
    std::vector<std::vector<int>> resultCSV;
    int distanceLim;
    QElapsedTimer timer2=QElapsedTimer();
    QElapsedTimer timerNoMove;
    QElapsedTimer timerCaptObstr=QElapsedTimer();
    QElapsedTimer timerTemps=QElapsedTimer();
    QElapsedTimer &r_timer;
    QTimer* variationFinish;
    QTimer* soundTimer = new QTimer();
    int compteur2=0;
    int cont;
    int m_conteurVariation;
    //filtrage brut
    int tresholdVal;
    int cpt_tresholdVal;
    int cpt_normalVal;
    int numberOfPressButton=0;
    bool alreadyReadCSV=false;
    bool isVariationFinish;
    bool m_quit =false;
    bool alerteOn=false;
    bool hasObject;
    bool scenarioCSV=false;
    int numberCapteur=0;
    Capteur *m_capteur1;
    Capteur *m_capteur2;
    Capteur *m_capteur3;
    QSound *sound=new QSound("alarme.wav");
    std::vector<Capteur*> capteurList;
};
#endif // SERIALPORT_H
