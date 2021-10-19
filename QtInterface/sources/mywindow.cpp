#include "mywindow.h"
#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <boost/algorithm/string.hpp>

QWidget *myWindow::setupBasicTab()
{
    QWidget *tab = new QWidget;
    this->alarmBackground = new QFrame(tab);

    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(startAlarm()));
    alarmBackground->setStyleSheet("background-color: red");
    timer->start();
    return (alarmBackground);
}

QWidget *myWindow::setupAdvancedTab()
{
    QGridLayout *grid = new QGridLayout;
    QWidget *tab = new QWidget;
    QLabel *logo = new QLabel;

    /// ENLEVEZ LE PATH EN BRUT !!!!
    logo->setPixmap(QPixmap("../logo.jpg"));
    grid->addWidget(logo, 0, 1, 2, 2, Qt::AlignCenter);

    for (int i = 0; i != this->m_serialPort->getSensorNumber(); i++ ) {
        QVBoxLayout *vbox = new QVBoxLayout; // Layout for name + lcd
        QLabel *label = new QLabel; // Label for name of lcd

        // Overall settings
        label->setText((("Sensor ") + std::to_string(i + 1)).c_str()); // for "Sensor $i" label
        this->m_sensors[i]->getLcd()->setFixedSize(100, 40);
        this->m_sensors[i]->getLcd()->display(this->m_sensors[i]->getLcdValue()); /// TEMPORARY

        // Set Vbox layout
        vbox->addWidget(label);
        vbox->addWidget(this->m_sensors[i]->getLcd());
        vbox->setSpacing(0);

        // if not enough sensor, show them only on one row
        if (this->m_serialPort->getSensorNumber() <= 4) {
            grid->addLayout(vbox, 2, i, 1, 1);
            continue;
        }
        // Else showraddLayout(vbox, 1, 0, 1, 1);
        else if (i == this->m_serialPort->getSensorNumber() - 1) {
            grid->addLayout(vbox, 1, i - 2, 1, 1);
        } else {
            grid->addLayout(vbox, 2, i - 1, 1, 1);
        }
    }
    tab->setLayout(grid);
    return (tab);
}

void myWindow::startScenario()
{
    this->m_serialPort->getCsv()->setFilename(this->m_scenarioBrowse->text().toStdString());
    this->m_serialPort->getCsv()->readFile();
    this->m_ScenarioTimer->start();
    QString string = "Playing : ";
    QString buffer = QString(this->m_serialPort->getCsv()->getFilename().c_str());
    QStringList split = buffer.split('/');
    string.append(split[split.size() - 1]);
    if (this->m_serialPort->getCsv()->getFilename() == this->m_serialPort->getCsv()->getDefaultScenario())
        string.append(" [default]");
    this->m_scenarioLabel->setText(string);
    this->m_scenarioLabel->show();
}

void myWindow::startAlarm()
{
    for (int i = 0; i != this->getSerialPort()->getSensorNumber(); i++) {
        if (this->getSensor()[1]->getLcdValue() < 100) {
            this->alarmBackground->setStyleSheet("background-color: red");
            //if (alarmSound->isFinished())
                //alarmSound->play();
        } else {
            this->alarmBackground->setStyleSheet("background-color: green");
        }
    }
}

QWidget *myWindow::setupOptionTab()
{
    QLabel *l = new QLabel;
    return (l);
}

myWindow::myWindow()
{
    this->m_serialPort = new mySerialPort(this);
    this->m_ScenarioTimer = new QTimer;
    this->m_ScenarioTimer->setInterval(100);
    connect(this->m_ScenarioTimer, SIGNAL(timeout()), this, SLOT(updateScenario()));
    for (int i = 0; i != 10; i++)
        m_scenarioSensors.push_back(new mySensor);
    alarmSound = new QSound("../alarme1.wav");
    for (int i = 0; i != this->m_serialPort->getSensorNumber(); i++ )
        m_sensors.push_back(new mySensor(i));
    //this->m_serialPort->doConnectLcd();

    // Initialize all tabs
    this->addTab(setupBasicTab(), "Basic");
    this->addTab(setupAdvancedTab(), "Advanced");
    this->addTab(setupOptionTab(), "Option");
    this->addTab(setupScenarioTab(), "Scenario");
}

std::vector<mySensor*> myWindow::getSensor()
{
    return (this->m_sensors);
}

mySerialPort *myWindow::getSerialPort()
{
    return (this->m_serialPort);
}
