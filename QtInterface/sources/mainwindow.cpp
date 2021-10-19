#include "mainwindow.h"
#include <QtWidgets>
#include <unistd.h>

MainWindow::MainWindow()
{
    qDebug() << "Serial begin";
    mySerialPort = new SerialPort(this,m_timer);
    buttonPushed = false;
    InitInterface();
    scenarioCSV = false;
    m_displayTimer  =  new QTimer();
    m_displayTimer->setInterval(30);
    m_displayTimer->start();
    sleep(1);
    connect(m_displayTimer, SIGNAL(timeout()), this, SLOT(on_lcdNumber_overflow()));
}

void MainWindow::InitInterface()
{
    QGridLayout *main_layout = new QGridLayout;

    labelCapt1 = new QLabel();
    labelCapt2 = new QLabel();
    labelCapt3 = new QLabel();
    labelCapt1->setText("Capteur1");
    labelCapt2->setText("Capteur2");
    labelCapt3->setText("Capteur3");

    lcd = new QLCDNumber();
    lcd2 = new QLCDNumber();
    lcd3 = new QLCDNumber();

    alerte1 = new QFrame();
    alerte1->setFixedSize(100,210);
    alerte1->setStyleSheet("background-color: blue");

    QFormLayout *form = new QFormLayout;
    form->addRow("Sensor 1:", lcd);
    form->addRow("Sensor 2:", lcd2);
    form->addRow("Sensor 3:", lcd3);

    QGroupBox *groupbox = new QGroupBox;
    groupbox->setTitle("Sensor");
    groupbox->setLayout(form);


    main_layout->addWidget(groupbox);

    this->setLayout(main_layout);
}

void MainWindow::traitementMainWindow()
{
    connect(m_displayTimer, SIGNAL(timeout()), this, SLOT(on_lcdNumber_overflow()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_lcdNumber_overflow()
{
    qDebug("ezr");
    int integerToDisplay1 = mySerialPort->getCapteurList()[0]->getCapteur()[mySerialPort->getCapteurList()[0]->getCapteur().size()- 1];
    int integerToDisplay2 = mySerialPort->getCapteurList()[1]->getCapteur()[mySerialPort->getCapteurList()[0]->getCapteur().size()- 1];
    int integerToDisplay3 = mySerialPort->getCapteurList()[2]->getCapteur()[mySerialPort->getCapteurList()[0]->getCapteur().size()- 1];
    lcd2->display(integerToDisplay2);
    lcd->display(integerToDisplay1);
    lcd3->display(integerToDisplay3);
    isGreen = true;
    isRed = false;
    isOrange = false;
    isViolet = false;
    for (int i = 0;i<mySerialPort->getCapteurList().size();i++) {
        if (mySerialPort->getCapteurList()[i]->getAlarme1() != 0) {
            isRed = true;
        }
        if (mySerialPort->getCapteurList()[i]->getHasObject()) {
            isViolet = true;
            isRed = false;
        }
        if (mySerialPort->getCapteurList()[i]->getAlarme2())
            isOrange = true;
        else
            isGreen=true;
            qDebug() << "end loop";
        }
    if(isGreen&&isRed)
        alerte1->setStyleSheet("background-color: red");
    else if(isGreen&&isViolet)
        alerte1->setStyleSheet("background-color: violet");
    else if(isViolet&&isOrange)
        alerte1->setStyleSheet("background-color: violet");
    else if(isGreen&&isOrange)
        alerte1->setStyleSheet("background-color: orange");
    else if(isViolet&&isRed)
        alerte1->setStyleSheet("background-color: red");
    else if(isRed)
        alerte1->setStyleSheet("background-color: red");
    else if(isOrange)
        alerte1->setStyleSheet("background-color: orange");
    else if(isViolet)
        alerte1->setStyleSheet("background-color: violet");
    else if(isGreen)
        alerte1->setStyleSheet("background-color: green");
        qDebug() << "step 2";
    //} else {
        ////Faire fonctionner quand SlotProcessData() est adapté pour les 3capteurs
        /*
//        qDebug()<<"enter lcd1 SLOT else";
//        qDebug()<<"Get vec average:   "<<mySerialPort->getVecAverage1().size();
        int integerToDisplay = mySerialPort->getCapteurList().at(0)->getCapteurAverage()[mySerialPort->getCont()-1];
//        qDebug()<<"enter lcd1 SLOT else"<<integerToDisplay;
        lcd2->display(integerToDisplay);
        if(mySerialPort->getCapteurList().at(0)->getAlarme1()){
            alerte1->setStyleSheet("background-color: red");
        }
        else{
            alerte1->setStyleSheet("background-color:green");
        }


        if(mySerialPort->getCapteurList().at(0)->getAlarme2()){
            alerte1->setStyleSheet("background-color: orange");

        }
        else{
            alerte1->setStyleSheet("background-color: green");
        }
        //    }
*/
      //  }

    //    ui->lcdNumber->display(983);
//    qDebug()<<"out lcd1 SLOT";
}

void MainWindow::write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int>>> dataset){
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<int>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size
    // Create an output filestream object
    std::ofstream myFile(filename);
    // Send column names to the stream
    for(int j = 0; j < dataset.size(); ++j)
    {
        myFile << dataset.at(j).first;
        if(j != dataset.size() - 1) myFile << ";"; // No comma at end of line
    }
    myFile << "\n";
    // Send data to the stream
    for(int i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for(int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).second.at(i);
            if(j != dataset.size() - 1) myFile << ";"; // No comma at end of line
        }
        myFile << ";\n";
    }
    // Close the file
    myFile.close();
}
