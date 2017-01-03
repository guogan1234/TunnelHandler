#include "worker.h"
#include <QDir>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::FindFullPath(QString parentPath, int key)
{
    qDebug()<<"parentPath:"<<parentPath;
    QDir d(parentPath);
    QFileInfoList dirList = d.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs , QDir::LocaleAware);
    if(dirList.length() <= 0)
    {
        qDebug()<<"Dir has no subDir!";
        return;
    }
    QMap<int,QString> map;
    QMap<int,QString>::iterator iter;
    foreach (QFileInfo file, dirList) {
        QString key = file.fileName();
        int i_key = key.toInt();
        QString value = file.filePath();
        map.insert(i_key,value);
    }
    /*
    for(iter = map.begin();iter != map.end();iter++)
    {
        qDebug()<<"key:"<<iter.key()<<"value:"<<iter.value();
    }*/
    for(iter = map.begin();iter != map.end();iter++){
        QString dirPath2 = iter.value();
//        qDebug()<<"dirPath2:"<<dirPath2;
        QDir dir2(dirPath2);
        QFileInfoList dir2_List = dir2.entryInfoList(QDir::Files);
        foreach (QFileInfo file, dir2_List) {
            QString filePath = file.filePath();
            QString fileName = file.fileName();
            QStringList fileList = fileName.split('-');
            if(fileList.length() !=4 )
            {
                qDebug()<<"File Name is Wrong!";
            }
            else{
                QString strKeyWithJpg = fileList[3];
                QStringList keyList = strKeyWithJpg.split('.');
                if(keyList.length() != 2)
                {
                    qDebug()<<"File JPG Name is Wrong!";
                }
                else{
                    QString strKey = keyList[0];
                    int i_key = strKey.toInt();
                    if(i_key == key)
                    {
                        destList.append(filePath);
                    }
                }
            }
        }
    }
}

void Worker::showDestList()
{
    qDebug()<<"len:"<<destList.length();
    foreach (QString str, destList) {
        qDebug()<<"destList:"<<str;
    }
}
