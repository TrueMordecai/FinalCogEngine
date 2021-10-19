#include "mywidget.h"
#include <QLCDNumber>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *labelCapt1=new QLabel();
    labelCapt1->setText("Capteur1");
    QLabel *labelCapt2=new QLabel();
    labelCapt2->setText("Capteur2");
    QLCDNumber *lcd = new QLCDNumber();
    lcd->display(4);
//  lcd->resize(220,280);
    lcd->setMaximumSize(81,41);
    lcd->setMinimumSize(81,41);
    lcd->setSegmentStyle(QLCDNumber::Filled);
    QLCDNumber *lcd2 = new QLCDNumber();
    lcd2->display(10);
//  lcd->resize(220,280);
    lcd2->setMaximumSize(81,41);
    lcd2->setMinimumSize(81,41);
    lcd2->setSegmentStyle(QLCDNumber::Filled);
    QLCDNumber *lcd3=new QLCDNumber();
    lcd3->setMaximumSize(81,41);
    lcd3->setMinimumSize(81,41);
    lcd3->setSegmentStyle(QLCDNumber::Filled);
    QFrame *alerte1=new QFrame();
    alerte1->setMinimumSize(100,110);
    alerte1->setMaximumSize(100,110);
    alerte1->resize(100,110);
    alerte1->setStyleSheet("background-color: red");
    QFrame *alerte2=new QFrame();
    alerte2->setMinimumSize(100,110);
    alerte2->setMaximumSize(100,110);
    alerte2->resize(100,200);
    alerte2->setStyleSheet("background-color: orange");
    QGridLayout *layout = new QGridLayout();
//  layout->addWidget(quit);
    layout->addWidget(labelCapt1,0,0);
    layout->addWidget(labelCapt2,0,2);
    layout->addWidget(lcd,1,0);
    layout->addWidget(lcd2,1,2);
    layout->addWidget(alerte1,4,0);
    layout->addWidget(alerte2,4,2);
    QHBoxLayout *hbox=new QHBoxLayout();
    hbox->addWidget(lcd3);
    layout->addLayout(hbox,2,1);
    setLayout(layout);
}
