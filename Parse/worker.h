#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QStringList>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

signals:

public slots:
    void FindFullPath(QString parentPath,int key);

    void showDestList();

public:
    QStringList destList;
};

#endif // WORKER_H
