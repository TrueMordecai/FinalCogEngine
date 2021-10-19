#include "serialport.h"

SerialPort::SerialPort(QObject *parent, QElapsedTimer &_timer):r_timer(_timer)
{
    m_capteur1 = new Capteur();
    m_capteur2 = new Capteur();
    m_capteur3 = new Capteur();
    capteurList.push_back(m_capteur1);
    capteurList.push_back(m_capteur2);
    capteurList.push_back(m_capteur3);
    timer2.start();
    timerNoMove.start();
    isVariationFinish=true;
    m_conteurVariation=50;
    m_arduinoSerialPort = new QSerialPort();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.portName().contains("ttyACM")) {
            m_arduinoSerialPort->setPort(info);
        }
    }
    m_arduinoSerialPort->setBaudRate(QSerialPort::Baud9600);
    m_arduinoSerialPort->setDataBits(QSerialPort::Data8);
    m_arduinoSerialPort->setParity(QSerialPort::NoParity);
    m_arduinoSerialPort->setStopBits(QSerialPort::OneStop);
    m_arduinoSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    _timer.start();
    connect(m_arduinoSerialPort,SIGNAL(readyRead()),this,SLOT(SlotTraitementArduino()));
    bool openPortSuccess = m_arduinoSerialPort->open(QIODevice::ReadWrite);
    if (!openPortSuccess) {
        qDebug()<<"pas réussi à se connecter";
        return;
    }
    qDebug() << " QSerialPort initialized, entering loop...";
    qDebug()<<"timer SLOT :         "<<timer2.elapsed()<<endl;
    timer2.restart();
}

SerialPort::~SerialPort() {
    m_quit = true;
    m_arduinoSerialPort->close();
}

void SerialPort::SlotTraitementArduino()
{
    if (scenarioCSV == false) {
        int numberOfBytesInput=m_arduinoSerialPort->bytesAvailable();
        numberOfBytes += numberOfBytesInput;
        QByteArray datas = m_arduinoSerialPort->readLine();
        compteur++;
        QString dataStringInput = QString(datas);
        dataString += dataStringInput;
        QStringList vectorTabString;
        qDebug() << "data bytes array size : " << datas.size();
        qDebug() << "string size : " << dataString.size();
        qDebug() << "data string input" << dataStringInput;
        qDebug() << "data string step 1 : " << dataString;
        qDebug() << "cond : " << dataString.contains('\n');
        if (dataString.contains('\n')) {
            ///Pour plusieurs capteurs:
            vectorTabString=dataString.split(";");
            numberCapteur=vectorTabString.size();
            qDebug() << numberCapteur;
            for (int i = 0; i < numberCapteur; i++) {
                capteurList[i]->addNewValue(vectorTabString[i].toInt());
                ///Verifie qu'il y a au moins une valeur dans les vecteurs
                if (capteurList[i]->getCapteur().size()<1)
                    return;
                capteurList[i]->filtrageToMaxValue2000();
                ////Filtrage du signal
                capteurList[i]->filtrageDataCapteur();
                ////Filtrage des alarmes
                qDebug() << "Enter Process alarm";
                /// CRASH POUR UNE RAISON OBSCUR
                //capteurList[i]->processAlarme();
            }
            dataString ="";
            numberOfBytes = 0;
            compteur2++;
        }
    }
    if (alreadyReadCSV==false&& scenarioCSV==true&& numberOfPressButton==1) {
        resultCSV = read_csv("data/coucou.csv");
        alreadyReadCSV = true;
        qDebug()<<"J'AI LU LE CSV!!!"<<alreadyReadCSV;
    }
    qDebug()<<"Temps du Slot"<<timerTemps.nsecsElapsed();
    r_timer.restart();
    timerTemps.start();
}

void SerialPort::SlotProcessPostData()
{
    ////modifier fonctions Process Data
    /*
    ProcessPostData(vec1,capteur2->getCapteur(),capteur1->getCapteur());
    //    qDebug()<<"SlotProcessData";*/
}

void SerialPort::fifo(std::vector<int> &vect)
{
    if (vect.size()==1000) {
        vect.erase(vect.begin());
    }
}

std::vector<Capteur *> SerialPort::getCapteurList() const
{
    return capteurList;
}

void SerialPort::setCapteurList(const std::vector<Capteur *> &value)
{
    capteurList = value;
}

QSound *SerialPort::getSound() const
{
    return sound;
}

void SerialPort::setSound(QSound *value)
{
    sound = value;
}

bool SerialPort::getHasObject() const
{
    return hasObject;
}

void SerialPort::setHasObject(bool value)
{
    hasObject = value;
}

int SerialPort::getConteurVariation() const
{
    return m_conteurVariation;
}

void SerialPort::setConteurVariation(int conteurVariation)
{
    m_conteurVariation = conteurVariation;
}

int SerialPort::getNumberOfPressButton() const
{
    return numberOfPressButton;
}

void SerialPort::setNumberOfPressButton(int value)
{
    numberOfPressButton = value;
}

int SerialPort::getCont() const
{
    return cont;
}

void SerialPort::setCont(int value)
{
    cont = value;
}

std::vector<std::vector<int> > SerialPort::getResultCSV() const
{
    return resultCSV;
}

void SerialPort::setResultCSV(const std::vector<std::vector<int> > &value)
{
    resultCSV = value;
}

bool SerialPort::getScenarioCSV() const
{
    return scenarioCSV;
}

void SerialPort::setScenarioCSV(bool value)
{
    scenarioCSV = value;
}

int SerialPort::getDistanceLim() const
{
    return distanceLim;
}

void SerialPort::setDistanceLim(int value)
{
    distanceLim = value;
}

QElapsedTimer SerialPort::getTimerNoMove() const
{
    return timerNoMove;
}

void SerialPort::setTimerNoMove(const QElapsedTimer &value)
{
    timerNoMove = value;
}

QSerialPort *SerialPort::getArduinoSerialPort() const
{
    return m_arduinoSerialPort;
}

void SerialPort::setArduinoSerialPort(QSerialPort *arduinoSerialPort)
{
    m_arduinoSerialPort = arduinoSerialPort;
}

void SerialPort::ProcessPostData(std::vector<int>vect1,std::vector<int>vect2,std::vector<int>vect3,std::vector<int>vect4) {
////Faire fonctionner pour 3 capteurs(fonctionne sur un seul)
}

std::vector<std::vector<int>> SerialPort::read_csv(std::string filename) {
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<int>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size
    // Create an intput filestream object
    std::fstream myFile(filename);
    std::vector<QString> vec1String;
    std::vector<QString> vec2String;
    std::vector<QString> vec3String;
    std::vector<QString> vecDataString;
    std::vector<int> vec1;
    std::vector<int> vec2;
    std::vector<int> vec3;
    int compteur=0;
    if (myFile) {
        std::string data;
        std::string entete;
        std::string delimiteur=";";
        // getline(myFile,entete);
        while(getline(myFile,data,';')) {
            std::string datas=data;
            // qDebug()<<data.data()<<endl;
            vecDataString.push_back(datas.data());
        }
        qDebug()<<vecDataString.size();
        for(int i=0;i<vecDataString.size();i++) {
            if (i>=3) {
                if (i%3==2) {//colonne vec1
                    vec1.push_back(vecDataString.at(i).toInt());
                }
                if (i%3==1) {//colonne vec3
                    vec3.push_back(vecDataString[i].toInt());
                }
                if (i%3==0) {//colonne vec2
                    vec2.push_back(vecDataString[i].toInt());
                    compteur++;
                }
            }
        }
        std::vector<std::vector<int>> vecFinal= {vec1,vec2,vec3};
        return vecFinal;
    } else {
        qDebug()<<"Can't OPEN the CSV File"<<endl;
    }
    // Send column names to the stream
    // Close the file
    myFile.close();
}


