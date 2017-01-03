#include "findmaxmin.h"

QMap<double,int>::iterator scaleDataIter;

FindMaxMin::FindMaxMin(QObject *parent) :
    QObject(parent)
{
}

double FindMaxMin::GetDoubleMax()
{
    double temp;
    if(scaleData.size() <= 0)
    {
        qDebug()<<"FindMaxMin:Data Len=0!!!";
    }
    /*
    for(scaleDataIter = scaleData.begin();scaleDataIter != scaleData.end();scaleDataIter++)
    {
        qDebug()<<"scaleData:"<<scaleDataIter.key()<<scaleDataIter.value();
    }*/
    temp = scaleData.lastKey();
    return temp;
}

double FindMaxMin::GetDoubleMin()
{
    double temp;
    if(scaleData.size() <= 0)
    {
        qDebug()<<"FindMaxMin:Data Len=0!!!";
    }
    temp = scaleData.firstKey();
    return temp;
}
