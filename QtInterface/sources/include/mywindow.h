#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <myplot.h>
#include <QElapsedTimer>
#include <QDebug>
#include <QTimer>
#include <QLCDNumber>
#include <fstream>
#include <QSound>
#include <mysensor.h>
#include <myserialport.h>

class myWindow : public QTabWidget
{
    Q_OBJECT

public:
    myWindow();
    QWidget *setupBasicTab();
    QWidget *setupAdvancedTab();
    QWidget *setupOptionTab();
    QWidget *setupScenarioTab();
    void stopScenario();
    std::vector<mySensor*> getSensor();
    mySerialPort *getSerialPort();

public slots:
    void startAlarm();
    void startScenario();
    void updateScenario();

private:
    std::vector<mySensor*> m_sensors;
    mySerialPort *m_serialPort;
    int m_lineread;
    QFrame *alarmBackground;
    QSound *alarmSound;
    QTimer *m_ScenarioTimer;

    // Scenario attributes
    QLineEdit *m_scenarioBrowse;
    QPushButton *m_scenarioSearch;
    std::vector<mySensor*> m_scenarioSensors;
    QGridLayout *gridLayoutScenarioSensor;
    QLabel *m_scenarioLabel;
};

#endif // MYWINDOW_H
