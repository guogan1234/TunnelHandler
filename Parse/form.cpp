#include "form.h"
#include "ui_Form.h"

#include <QFileDialog>
#include <QDir>
#include <QSqlQuery>
#include <QMessageBox>

QList<QString> dbPath;
extern int DBLinkCount;
extern double refer_ratio;
extern double scale;
extern double heightPixel;
extern double widthPixel;

QList<QPointF> leftScanner;
QList<QPointF> rightScanner;
QMap<int,QString> scannerMap;//原始数据
QList<QPointF> scannerList;//合并后数据
QMap<double,double> scannerX;//key=X,value=Y;
QMap<double,double> scannerY;//key=Y,value=X;

QMap<int,CeJuLun> CJLMap;//有效测距轮数据
QMap<int,CeJuLun>::iterator iter;
QList<CeJuLun> CJLList;
QList<Section> sectList;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    InitParams();
}

Form::~Form()
{
    delete ui;
}

void Form::on_start_btn_clicked()
{
    QString rootPath = QFileDialog::getExistingDirectory(this,"Dir",".");
    qDebug()<<"rootPath:"<<rootPath;
    ui->root_LE->setText(rootPath);

    //遍历所有DB，获取DB路径
    QDir dir(rootPath);
    QFileInfoList infoList = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (QFileInfo info, infoList) {
        QString dirPath = info.filePath();
        qDebug()<<"dirPath:"<<dirPath;
        QDir dir2(dirPath);
        QFileInfoList infoList2 = dir2.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
        foreach (QFileInfo info2, infoList2) {
            QString DBDirPath = info2.filePath();
            qDebug()<<"DBDirPath:"<<DBDirPath;
            QString DBPath = DBDirPath + "/123.db";
            if(dir2.exists(DBPath))
            {
                qDebug()<<"DBPath:"<<DBPath;
                dbPath.append(DBPath);
            }
        }
    }
    ui->start_btn->setEnabled(false);
    ui->handleDir_LE->setText(dbPath.at(0));
    openDB(dbPath.at(0));
    QueryData(1);//默认先找进洞
    MergeData();
    ShowData();
    HandleData();
}

void Form::openDB(QString DBPath)
{
    qDebug()<<"openDB:"<<DBPath;
    db = QSqlDatabase::addDatabase("QSQLITE",QString::number(DBLinkCount));
    DBLinkCount++;
    db.setHostName(".");
    db.setDatabaseName(DBPath);
    if(!db.open())
    {
        qDebug()<<"DBError:"<<db.lastError();
    }
    QSqlQuery query(db);
    QString strSql1 = "select count(No) from LaserScanner";
    if(query.exec(strSql1))
    {
        if(query.next())
        {
            count = query.value(0).toInt();
            qDebug()<<"count:"<<count;
        }
    }
}

//flag=1，找进洞；flag=2，找出洞
void Form::QueryData(int flag)
{
    QSqlQuery query(db);
    QString strSql2;
    if(flag == 1)
    {
        strSql2 = "select * from LaserScanner order by [date] asc";
    }
    else if(flag == 2)
    {
       strSql2 = "select * from LaserScanner order by [date] desc";
    }
    if(query.exec(strSql2))
    {
        int id1;
        if(query.seek(seekIndex))
        {
            id1 = query.value("DeviceID").toInt();
            QString data = query.value("Data").toString();
            if(findFlag == 1)
            {
                inTime = query.value("Date").toString();
            }
            else if(findFlag == 2)
            {
                outTime = query.value("Date").toString();
            }
            ParseData(data,id1);
        }
        if(query.seek(seekIndex+1))
        {
            if(seekIndex+1 >= count)
            {
                QMessageBox::information(this,"End(Odd)!","Search Finished!!!");
                return;
            }
            int id = query.value("DeviceID").toInt();
            if(id == id1)//若2次的设备ID一样，补处理
            {
                qDebug()<<"####"<<seekIndex;
                return;
            }
            else{
                QString data = query.value("Data").toString();
                ParseData(data,id);
            }
        }
    }
}

void Form::InitParams()
{
    //判断进出洞
    seekIndex = 0;

    leftStartAngle = 45;
    rightStartAngle = -135;

    offsetX = 30;
    rightToLeftAngle = 135;//(-135)+((541-1)*0.5);

    limit_minX = -12000;
    limit_maxY = 7000;
    limit_meanY = 3000;

    bAuto = false;
    findFlag = 1;//默认寻找进洞

    ui->plot->xAxis->setRange(-14000,8000);
    ui->plot->yAxis->setRange(-8000,14000);

    tm = new QTimer(this);
    tm->setInterval(1000);
    connect(tm,SIGNAL(timeout()),this,SLOT(TimeOutHandleFunc()));

    //计算测距轮
    bIn = false;
    bOut = false;
    prevDis = 0;
    scale = 0.1;

    referRatio = refer_ratio;//待初始化
    real_Pixel = widthPixel * scale;//待初始化

    startLevel = 0;
    dt = 50;
}

void Form::ParseData(QString data, int id)
{
    if(id == 1)//右边的
    {
        QStringList list = data.split(" ");
        qDebug()<<"data1:"<<data<<list.length();
        for(int i = 0;i<list.length()-6;i++)
        {
            if(i == 0)
            {
                continue;
            }
            bool b = false;
            int len = list[i].toInt(&b,16);
            double angle = rightStartAngle + (i - 1)*0.5;
            double radian = qDegreesToRadians(angle);
            double cos = qCos(radian);
            double sin = qSin(radian);
            qDebug()<<"len:"<<len<<angle<<cos<<sin;
            double x,y;
            x = len * cos;
            y = len * sin;
            QPointF pt(x,y);
            qDebug()<<"pt:"<<pt;
            rightScanner.append(pt);
        }
    }
    else if(id == 2)
    {
        QStringList list = data.split(" ");
        qDebug()<<"data2:"<<data<<list.length();
        for(int i = 0;i<list.length()-6;i++)
        {
            if(i == 0)
            {
                continue;
            }
            bool b = false;
            int len = list[i].toInt(&b,16);
            double angle = leftStartAngle + (i - 1)*0.5;
            double radian = qDegreesToRadians(angle);
            double cos = qCos(radian);
            double sin = qSin(radian);
            qDebug()<<"len:"<<len<<angle<<cos<<sin;
            double x,y;
            x = len * cos;
            y = len * sin;
            QPointF pt(x,y);
            qDebug()<<"pt:"<<pt;
            leftScanner.append(pt);
        }
    }
}

void Form::MergeData()
{
    for(int i = 0;i < rightScanner.length();i++)
    {
        double angle = rightStartAngle + i*0.5;
        qDebug()<<"Merge1:"<<angle<<i;
        if(angle <= rightToLeftAngle)
        {
            qDebug()<<"1111:"<<angle;
            QPointF pt = rightScanner.at(i);
            scannerList.append(pt);
        }
    }
    for(int i = 0;i < leftScanner.length();i++)
    {
        double angle = leftStartAngle + i*0.5;
        qDebug()<<"Merge2:"<<angle;
        if(angle > rightToLeftAngle)
        {
            qDebug()<<"2222:"<<angle<<i;
            QPointF pt = leftScanner.at(i);
            scannerList.append(pt);
        }
    }
}

QVector<double> vX1,vY1;
QVector<double> vX2,vY2;
QVector<double> vX3,vY3;

void Form::ShowData()
{
    //清除上一次显示数据
    vX1.clear();
    vY1.clear();
    vX2.clear();
    vY2.clear();
    vX3.clear();
    vY3.clear();

    foreach (QPointF pt, leftScanner) {
        double x = pt.x();
        double y = pt.y();
        vX1.append(x);
        vY1.append(y);
    }
    foreach (QPointF pt, rightScanner) {
        double x = pt.x();
        double y = pt.y();
        vX2.append(x);
        vY2.append(y);
    }
    foreach (QPointF pt, scannerList) {
        double x = pt.x();
        double y = pt.y();
        vX3.append(x);
        vY3.append(y);
    }
    //显示
    ui->plot->addGraph();
    ui->plot->graph(0)->setName("Left");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(0)->setPen(QPen(Qt::yellow));
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(0)->setData(vX1,vY1);
//    ui->plot->graph(0)->rescaleAxes();

    ui->plot->addGraph();
    ui->plot->graph(1)->setName("Right");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(1)->setPen(QPen(Qt::green));
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(1)->setData(vX2,vY2);

    ui->plot->addGraph();
    ui->plot->graph(2)->setName("Merge");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(2)->setPen(QPen(Qt::red));
    ui->plot->graph(2)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    ui->plot->graph(2)->setData(vX3,vY3);

    ui->plot->replot();
}

void Form::HandleData()
{
    int ptCount = 0;
    double totalY = 0;
    foreach (QPointF pt, scannerList) {
        double x = pt.x();
        double y = pt.y();
        if(y >= 0)
        {
            ptCount++;
            totalY += y;
            scannerX.insertMulti(x,y);
            scannerY.insertMulti(y,x);
        }
    }
    int length = scannerList.length();
    double meanY = totalY/ptCount;
    double minX = scannerX.firstKey();
    double maxY = scannerY.lastKey();
    ui->meanY_LE->setText(QString::number(meanY));
    ui->minX_LE->setText(QString::number(minX));
    ui->maxY_LE->setText(QString::number(maxY));
    //判断是否满足条件
    if(minX > limit_minX)
    {
        qDebug()<<"###"<<minX;
        ui->minX_CB->setChecked(true);
    }
    else{
        ui->minX_CB->setChecked(false);
    }
    if(maxY < limit_maxY)
    {
        qDebug()<<"###"<<maxY;
        ui->maxY_CB->setChecked(true);
    }
    else{
        ui->maxY_CB->setChecked(false);
    }
    if(meanY > limit_meanY)
    {
        qDebug()<<"###"<<meanY;
        ui->meanY_CB->setChecked(true);
    }
    else{
        ui->meanY_CB->setChecked(false);
    }
    if(minX > limit_minX && maxY < limit_maxY && meanY > limit_meanY)
    {
        ui->all_CB->setChecked(true);
        if(findFlag == 1)
        {
            ui->inTime_LE->setText(inTime);
        }
        else if(findFlag == 2)
        {
            ui->outTime_LE->setText(outTime);
        }
        //停止计时器
        tm->stop();
    }
    else{
        ui->all_CB->setChecked(false);
    }
}

void Form::UpdateDBTime()
{
    qDebug()<<"UpdateDBTime..."<<referRatio;
    QSqlQuery query(db);
    QString strSql;
    strSql = "select * from Trigger order by No asc";
    if(query.exec(strSql))
    {
        int prevWarpPixel = 0;
        while(query.next())
        {
            int id = query.value("No").toInt();
            QString strTime = query.value("Date").toString();
            //计算距离增量
            double dis = query.value("Distance").toDouble();
            qDebug()<<"###"<<id<<strTime<<dis;
            double disAdd;
            disAdd = dis - prevDis;
            prevDis = dis;
            //计算下一个距离增量
            double nextDis = 0;
            if(query.next())
            {
                nextDis = query.value("Distance").toDouble();
            }
            if(query.previous())
            {
                ;
            }
            double nextDisAdd = nextDis - dis;

            //计算时间差值
            QDateTime time = QDateTime::fromString(strTime,"yyyyMMdd-hhmmss.zzz");
            qint64 mSec = time.toMSecsSinceEpoch();
            QDateTime timeIn = QDateTime::fromString(inTime,"yyyy/MM/dd hh:mm:ss.zzz");
            qint64 mSecIn = timeIn.toMSecsSinceEpoch();
            QDateTime timeOut = QDateTime::fromString(outTime,"yyyy/MM/dd hh:mm:ss.zzz");
            qint64 mSecOut = timeOut.toMSecsSinceEpoch();

            qint64 msSumIn = mSec - mSecIn;
            qint64 msSumOut = mSec - mSecOut;
//            qDebug()<<"msSum:"<<msSumIn<<msSumOut;

            if(msSumIn >= 0 && msSumOut <= 0)//洞内
            {
                if(!bIn)//进洞
                {
                    bIn = true;
                    disLevel = dis;
                }
                CeJuLun temp;
                temp.id = id;
                temp.distance = dis;
                temp.disAdd = disAdd;
                temp.halfDisAdd = disAdd/2;
                double disFrom0_temp = dis - disLevel;
                temp.disFrom0 = disFrom0_temp;
                temp.startMile = disFrom0_temp - disAdd/2;
                temp.endMile = disFrom0_temp + nextDisAdd/2;
                temp.realMile = -1;//无效数据
                temp.leftCutPixel = -1;//无效数据
                temp.rightCutPixel = -1;//无效数据
                if(temp.startMile <= 0 && temp.endMile >= 0)//进洞第一行数据
                {
                    temp.realMile = 0;
                    temp.leftCutPixel = ((0 - temp.startMile)*1000/refer_ratio)*scale;
                    temp.rightCutPixel = -1;//无效数据
                    qDebug()<<"index0:"<<temp.id<<temp.startMile<<temp.endMile<<temp.realMile;
                    temp.validMile = (nextDisAdd + disAdd)/2;
                    double temp0 = ((nextDisAdd/2 + disAdd/2)*1000/refer_ratio)*scale;
                    temp.validPixel = qRound(temp0);
                    temp.realPixel = qRound(real_Pixel);
    //                double temp1 = real_Pixel - temp0;
                    temp.warpPixel = 0;
                    prevWarpPixel = real_Pixel - temp0;
                }
                else{
                    int startIndex = temp.startMile/dt;
                    int endIndex = temp.endMile/dt;
                    qDebug()<<"index:"<<temp.id<<startIndex<<endIndex<<temp.startMile<<temp.endMile<<temp.realMile;
                    if(endIndex - startIndex == 1)//拼接段节点
                    {
                        temp.realMile = endIndex * dt;
                        if(endIndex%2 == 1)
                        {
//                            qDebug()<<"1111:"<<temp.endMile - endIndex*dt<<(temp.endMile - endIndex*dt)*1000<<(temp.endMile - endIndex*dt)*1000/refer_ratio;
                            temp.rightCutPixel = ((temp.endMile - endIndex*dt)*1000/refer_ratio)*scale;
                            temp.leftCutPixel = ((endIndex*dt - temp.startMile)*1000/refer_ratio)*scale;
                        }
                        else if(endIndex%2 == 0)
                        {
                            temp.leftCutPixel = ((endIndex*dt - temp.startMile)*1000/refer_ratio)*scale;
                            temp.rightCutPixel = ((temp.endMile - endIndex*dt)*1000/refer_ratio)*scale;
                        }
                    }
                    else{
                        temp.realMile = -1;//无效数据
                    }
                    qDebug()<<"index2:"<<temp.id<<startIndex<<endIndex<<temp.startMile<<temp.endMile<<temp.realMile<<temp.leftCutPixel<<temp.rightCutPixel;
                    temp.validMile = (nextDisAdd + disAdd)/2;
                    double temp0 = ((nextDisAdd/2 + disAdd/2)*1000/refer_ratio)*scale;
                    temp.validPixel = qRound(temp0);
                    temp.realPixel = qRound(real_Pixel);
    //                double temp1 = real_Pixel - temp0;
                    temp.warpPixel = prevWarpPixel;
                    prevWarpPixel = temp.realPixel - temp.validPixel;
                }
//                temp.validMile = (nextDisAdd + disAdd)/2;
//                double temp0 = ((nextDisAdd/2 + disAdd/2)*1000/refer_ratio)*scale;
//                temp.validPixel = qRound(temp0);
//                temp.realPixel = qRound(real_Pixel);
//                double temp1 = real_Pixel - temp0;
//                temp.warpPixel = temp.realPixel - temp.validPixel;
                qDebug()<<"index3:"<<temp.id<<temp.startMile<<temp.endMile<<temp.realMile<<temp.leftCutPixel<<temp.rightCutPixel;
                //CJLMap.insertMulti(id,temp);
                CJLList.append(temp);
            }
            if(msSumOut >= 0)//出洞
            {
                ;
            }
        }
        double SDLen = CJLList.last().distance - CJLList.first().distance;
        ui->SDLen_LE->setText(QString::number(SDLen));
    }
}

void Form::ShowCJLData()
{
    qDebug()<<"ShowCJLData..."<<CJLMap.size()<<CJLList.length();
    /*
    for(iter = CJLMap.begin();iter!=CJLMap.end();iter++)
    {
        CeJuLun temp;
        temp = iter.value();
        qDebug()<<iter.key()<<":"<<temp.id<<temp.disAdd<<temp.disFrom0<<temp.halfDisAdd<<temp.startMile<<temp.endMile<<temp.validMile<<temp.validPixel<<temp.realPixel<<temp.warpPixel<<temp.realMile<<temp.leftCutPixel<<temp.rightCutPixel;
    }*/
    foreach (CeJuLun temp, CJLList) {
        qDebug()<<temp.id<<temp.disAdd<<temp.disFrom0<<temp.halfDisAdd<<temp.startMile<<temp.endMile<<temp.validMile<<temp.validPixel<<temp.realPixel<<temp.warpPixel<<temp.realMile<<temp.leftCutPixel<<temp.rightCutPixel;
    }
}

void Form::GetUsedCJLData()
{
    int i = 0;
    bool bFirst = false;
    Section sect;
    foreach (CeJuLun temp, CJLList) {
        i++;
        if(temp.realMile != -1)
        {
            if(!bFirst)
            {
                sect.startMile = temp.realMile;
                sect.startImgIndex = temp.id;
                sect.realStartMile = temp.startMile;
                sect.leftCutPixel = qRound(temp.leftCutPixel);
                bFirst = true;
            }
            else
            {
                sect.endMile = temp.realMile;
                sect.endImgIndex = temp.id;
                sect.realEndMile = temp.endMile;
                sect.rightCutPixel = qRound(temp.rightCutPixel);
                sect.warpPixel = temp.warpPixel;
                qDebug()<<"Get:"<<temp.id<<temp.warpPixel<<temp.rightCutPixel<<temp.realPixel<<temp.validPixel<<temp.warpPixel;
                sectList.append(sect);
                sect.startMile = temp.realMile;
                sect.startImgIndex = temp.id;
                sect.realStartMile = temp.startMile;
                sect.leftCutPixel = qRound(temp.leftCutPixel);
            }
        }
    }
    CeJuLun last = CJLList.last();
    sect.endMile = last.endMile;
    sect.endImgIndex = last.id;
    sect.realEndMile = last.endMile;
    sect.rightCutPixel = qRound(last.rightCutPixel);
    sect.warpPixel = last.warpPixel;
    sectList.append(sect);
    foreach (Section s, sectList) {
        qDebug()<<"List:"<<s.startMile<<s.endMile<<s.startImgIndex<<s.endImgIndex<<s.realStartMile<<s.realEndMile<<s.warpPixel<<s.leftCutPixel<<s.rightCutPixel;
    }
}

void Form::on_change_btn_clicked()
{
    leftScanner.clear();
    rightScanner.clear();
    scannerList.clear();
    scannerX.clear();
    scannerY.clear();
    //清除显示图层
    ui->plot->clearGraphs();
    //下一环
    seekIndex = seekIndex + 2;
    ui->dataIndex->setText(QString::number(seekIndex + 1));
    if(seekIndex >= count)
    {
        QMessageBox::information(this,"End!","Search Finished!!!");
        return;
    }
    if(findFlag == 1)
    {
        QueryData(findFlag);
    }
    else if(findFlag == 2)
    {
        QueryData(findFlag);
    }
    MergeData();
    ShowData();
    HandleData();
}

void Form::on_pushButton_2_clicked()
{
    int index = ui->dataIndex->text().toInt();

    leftScanner.clear();
    rightScanner.clear();
    scannerList.clear();
    scannerX.clear();
    scannerY.clear();
    //清除显示图层
    ui->plot->clearGraphs();
    //下一环
    seekIndex = index - 1;
    if(seekIndex >= count)
    {
        QMessageBox::information(this,"End!","Search Finished!!!");
        return;
    }
    if(findFlag == 1)
    {
        QueryData(findFlag);
    }
    else if(findFlag == 2)
    {
        QueryData(findFlag);
    }
    MergeData();
    ShowData();
    HandleData();
}

void Form::on_pushButton_3_clicked()
{
    ui->widget->setEnabled(false);
    //开启计时器
    tm->start();
}

void Form::TimeOutHandleFunc()
{
    on_change_btn_clicked();
}

void Form::on_pushButton_4_clicked()
{
    ui->widget->setEnabled(false);
    findFlag = 2;
    seekIndex = -2;//数据索引重置(-2+2=0)
    //
//    ui->inTime_LE->setText("");
    //
    tm->start();
}

void Form::on_pushButton_5_clicked()
{
    UpdateDBTime();
    ShowCJLData();
    GetUsedCJLData();
}
