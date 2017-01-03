#ifndef FINDMAXMIN_H
#define FINDMAXMIN_H

#include <QObject>
#include <QMap>
#include <QDebug>

class FindMaxMin : public QObject
{
    Q_OBJECT
public:
    explicit FindMaxMin(QObject *parent = 0);

    QMap<double,int> scaleData;

    double GetDoubleMax();
    double GetDoubleMin();
signals:

public slots:

};

#endif // FINDMAXMIN_H
