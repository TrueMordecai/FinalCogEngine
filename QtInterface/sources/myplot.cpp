/*

#include <matplotlibcpp.h>
#include "myplot.h"

MyPlot::MyPlot()
{

}

std::vector<int> MyPlot::addPlot(std::vector<int> vec1, int value)
{

    std::vector<int> compteur;
    int compteurA=0;

    qDebug()<<"new vector:";
    std::vector<int> vectorValue(vec1);
    for(int i=0;i<vectorValue.size();i++){
        qDebug()<<vectorValue[i];
        compteurA++;
        compteur.push_back(compteurA);

    }

    vectorValue.push_back(value);
    compteurA++;
    compteur.push_back(compteurA);
    qDebug()<<"Ajout fait"<<endl;
    for(int i=0;i<vectorValue.size();i++){
        qDebug()<<vectorValue[i];
    }


//    matplotlibcpp::plot(vectorValue);



    return vectorValue;


}
std::vector<int>MyPlot:: average(std::vector<int>vectPoint,int nbrPoints){
    MovingAverage averageMove=MovingAverage(nbrPoints);
    std::vector<int> vectAverage;
    for(int i=0;i<vectPoint.size();i++){
        averageMove.add(vectPoint[i]);
        vectAverage.push_back( averageMove.avg());
    }
    return vectAverage;
}
*/
