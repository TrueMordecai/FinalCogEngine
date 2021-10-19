#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <vector>
#include <myplot.h>
#include <QSound>
#include <QTimer>
#include <QObject>
#include <QLCDNumber>

class Capteur:public QObject
{
    Q_OBJECT

public:
    Capteur();
    std::vector<int> getCapteur() const;
    void setCapteur(const std::vector<int> &value);
    std::vector<int> getCapteurAverage() const;
    void setCapteurAverage(const std::vector<int> &value);
    bool getAlarme1() const;
    void setAlarme1(bool value);
    bool getAlarme2() const;
    void setAlarme2(bool value);
    std::vector<int>average();
    void addNewValue(int value);
    void filtrageToMaxValue2000();
    void filtrageDataCapteur();
    void processAlarme();
    bool getHasObject() const;
    void setHasObject(bool value);
    int getDistanceLim() const;
    void setDistanceLim(int value);
    QTimer *getVariationFinish() const;
    void setVariationFinish(QTimer *value);
    bool getIsVariationFinish() const;
    void setIsVariationFinish(bool value);

public slots:
    void UpdateVariationFinish();

private:
    std::vector<int> m_capteur;
    std::vector<int>m_capteurAverage;
    std::vector<int>m_vitesse;
    std::vector<int>m_MoyVitesse;
    bool m_alarme1;
    bool m_alarme2;
    bool hasObject;
    int tresholdVal = 2000;
    int cpt_tresholdVal;
    int cpt_normalVal;
    int distanceLim;
    //MyPlot plot= MyPlot();
    QSound *sound=new QSound("alarme.wav");
    bool isVariationFinish;
    QTimer* variationFinish;

    QLCDNumber *lcd;
};

#endif // CAPTEUR_H
