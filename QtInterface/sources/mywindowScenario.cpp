#include "mywindow.h"

void myWindow::stopScenario()
{
    this->m_ScenarioTimer->stop();
    this->m_scenarioLabel->setText("Scenario ended.");
}

void myWindow::updateScenario()
{
    QString string = this->m_serialPort->getCsv()->getNewLine();
    if (string == nullptr) {
        stopScenario();
        return;
    }
    QStringList datas = string.split(";");
    int i = 0;
    for (; i != datas.size() - 1; i++) {
        this->m_scenarioSensors[i]->setLcdValue(datas[i].toInt());
        this->m_scenarioSensors[i]->displayLcdValue(true);
    }
    QPalette palette;
    for (;i != 10; i++) {
        palette = this->m_scenarioSensors[i]->getLcd()->palette();
        palette.setColor(QPalette::Normal, QPalette::Background, Qt::yellow);
        this->m_scenarioSensors[i]->getLcd()->setPalette(palette);
        this->m_scenarioSensors[i]->displayLcdValue(false);
        this->m_scenarioSensors[i]->getLcd()->setEnabled(false);
    }
}

QWidget *myWindow::setupScenarioTab()
{
    this->gridLayoutScenarioSensor = new QGridLayout;
    QWidget *tab = new QWidget;
    QVBoxLayout *vertical = new QVBoxLayout;
    QFormLayout *searchBar = new QFormLayout;
    QGridLayout *grid = new QGridLayout;
    this->m_scenarioLabel = new QLabel;
    this->m_scenarioBrowse = new QLineEdit;
    this->m_scenarioSearch = new QPushButton;
    this->m_scenarioSearch->setMaximumWidth(185);
    this->m_scenarioBrowse->setMinimumWidth(300);
    this->m_scenarioBrowse->setText("Scenario path");
    this->m_scenarioSearch->setText("Launch");
    this->m_scenarioLabel->setText("in case of wrong path, a DEFAULT scenario is read.");
    connect(this->m_scenarioSearch, SIGNAL(pressed()), this, SLOT(startScenario()));
    for (int i = 0; i != 10; i++)
        if (i < 5)
            grid->addWidget(m_scenarioSensors[i]->getLcd(), i, 0, 1, 1);
        else
            grid->addWidget(m_scenarioSensors[i]->getLcd(), i - 5, 1, 1, 1);
    searchBar->addRow(this->m_scenarioBrowse, this->m_scenarioSearch);
    searchBar->addRow(this->m_scenarioLabel);
    vertical->addLayout(searchBar);
    vertical->addLayout(grid);
    vertical->addLayout(this->gridLayoutScenarioSensor);
    tab->setLayout(vertical);
    return (tab);
}
