#include "capteur.h"
#include <QDebug>

Capteur::Capteur()
{
    variationFinish= new QTimer();
    variationFinish->setInterval(1000);
    isVariationFinish=true;
    connect(variationFinish,SIGNAL(timeout()),this,SLOT(UpdateVariationFinish()));

    lcd = new QLCDNumber;
}

std::vector<int> Capteur::getCapteur() const
{
    return m_capteur;
}

void Capteur::setCapteur(const std::vector<int> &value)
{
    m_capteur=value;
}

std::vector<int> Capteur::getCapteurAverage() const
{
    return m_capteurAverage;
}

void Capteur::setCapteurAverage(const std::vector<int> &value)
{
    m_capteurAverage=value;
}

bool Capteur::getAlarme1() const
{
    return m_alarme1;
}

void Capteur::setAlarme1(bool value)
{
    m_alarme1 = value;
}

bool Capteur::getAlarme2() const
{
    return m_alarme2;
}

void Capteur::setAlarme2(bool value)
{
    m_alarme2 = value;
}

void Capteur::addNewValue(int value)
{

    this->m_capteur.push_back(value);

}

void Capteur::filtrageToMaxValue2000(){

    //FILTRAGE VAL 2000
    if (m_capteur[m_capteur.size() - 1] >= tresholdVal) {
        cpt_tresholdVal++;
        cpt_normalVal = 0;
    } else {
        cpt_normalVal++;
        cpt_tresholdVal = 0;
    }
    if (m_capteur.size() > 5) {
        if (m_capteur[m_capteur.size() - 1] < tresholdVal && m_capteur[m_capteur.size() - 2] >= tresholdVal && cpt_normalVal < 5) {
            m_capteur[m_capteur.size() - 1] = tresholdVal;
        } else if (m_capteur[m_capteur.size() - 1] >= tresholdVal && m_capteur[m_capteur.size() - 2] < tresholdVal && cpt_tresholdVal < 5) {
            m_capteur[m_capteur.size() - 1] = m_capteur[m_capteur.size()-2];
        } else if (m_capteur[m_capteur.size() - 1] >= tresholdVal) {
            m_capteur[m_capteur.size() - 1] = tresholdVal;
        }
    }
    if (m_capteur[m_capteur.size() - 1] == 0)
        m_capteur[m_capteur.size() - 1] = m_capteur[m_capteur.size() - 2];
}

/// FILTRAGE DATA CAPTEUR RETOURN SET TOUJOURS HASOBJECT A TRUE
void Capteur::filtrageDataCapteur()
{
    hasObject = true;
    return;
    distanceLim = 800;
    m_capteurAverage[0] = 100;
    int vitesseInt = m_capteur[m_capteur.size() - 1] - m_capteur[m_capteur.size() - 2];
    m_vitesse.push_back(vitesseInt);
    if (m_capteurAverage[m_capteurAverage.size()]<=distanceLim){
        if (abs(abs(m_MoyVitesse[m_MoyVitesse.size() - 2]) - abs(m_MoyVitesse[m_MoyVitesse.size() - 1])) < 50 && abs(m_MoyVitesse[m_MoyVitesse.size() -2 ]) - abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) >0 ) {
            hasObject = true;
        } else if (abs(abs(m_MoyVitesse[m_MoyVitesse.size() - 2]) - abs(m_MoyVitesse[m_MoyVitesse.size() - 1])) >= 50) {
            hasObject = false;
        }
    }
}

void Capteur::processAlarme()
{
    if (m_capteurAverage[m_capteurAverage.size() - 1]<=distanceLim && !hasObject) {
        m_alarme1=true;
        sound->play();
    } else if (m_capteurAverage[m_capteurAverage.size() - 1] <= distanceLim && hasObject) {
        m_alarme1=false;
    } else {
        m_alarme1=false;
    }
    if (m_capteur[m_capteur.size() - 1] < 1000) {
        if (abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) >= 30 && abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) <= 500) {
            m_alarme2 = true;
            isVariationFinish = false;
            variationFinish->start();
        } else if (isVariationFinish== true) {
            m_alarme2=false;
        }
    } else if (m_capteur[m_capteur.size() - 1]<1200){
        if (abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) >= 20 && abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) <= 500){
            m_alarme2=true;
            isVariationFinish= false;
            //VARIATIONFINISH RESTART
            variationFinish->start();
        }
        else if (isVariationFinish== true){
            m_alarme2=false;
        }
    } else {
        if (abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) >= 80 && abs(m_MoyVitesse[m_MoyVitesse.size() - 1]) <= 600){
            m_alarme2=true;
            isVariationFinish= false;
            //VARIATIONFINISH RESTART
            variationFinish->start();
        } else if (isVariationFinish== true) {
            m_alarme2=false;
        }
    }
}

bool Capteur::getHasObject() const
{
    return hasObject;
}

void Capteur::setHasObject(bool value)
{
    hasObject = value;
}

int Capteur::getDistanceLim() const
{
    return distanceLim;
}

void Capteur::setDistanceLim(int value)
{
    distanceLim = value;
}

QTimer *Capteur::getVariationFinish() const
{
    return variationFinish;
}

void Capteur::setVariationFinish(QTimer *value)
{
    variationFinish = value;
}

bool Capteur::getIsVariationFinish() const
{
    return isVariationFinish;
}

void Capteur::setIsVariationFinish(bool value)
{
    isVariationFinish = value;
}
void Capteur::UpdateVariationFinish()
{
    isVariationFinish=true;
    qDebug()<<"Slot variation";
}
