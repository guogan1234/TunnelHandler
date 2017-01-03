#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QAxObject>
#include <QThread>

#include <QFileDialog>
#include "worker.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

//处理数据不能用QMap，不能用X坐标排序，废弃
QMap<double,double> srcData;//扫描仪合并后处理数据，key为X坐标，value为Y坐标
QMap<double,double>::iterator srcDataIter;

QList<QPointF> srcDataList;//扫描仪合并后处理数据,已平移右半边和剔除Y<0数据
//QList<Intersect*> destData;//相交结果数据，key为相机编号，value为数据(需释放)
QList<Intersect> destData;
QMap<int,QList<QPointF> > scannerData;//左右扫描仪数据，key为左右扫描仪编号，value为解析后数据(未平移)
QMap<int,QList<QPointF> >::Iterator scannerDataIter;
QList<int> BadIndex;//异常点索引
QList<Area> ErrorArea;//剔除区域索引

QList<CameraInfo> cameraInfoList;//相机参数
QList<Sect> sectInfoList;//相交点集合，与相机参数一一对应
QMap<double,int> pixelMap;//相机分辨率集合,key为分辨率值，value为相机索引

//整合测距轮
int DBLinkCount = 1;
extern QList<CeJuLun> CJLList;
extern QList<Section> sectList;
double refer_ratio;
double scale;
double heightPixel;
double widthPixel;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    ui->pushButton_5->setVisible(false);
    ui->pushButton_17->setVisible(false);
    InitParams();
    InitSubCfg();
    GetScannerDataByDB();
}

void MainWindow::InitParams()
{
    cameraIndex = 0;
    graphIndex = -1;//从-1开始
    scannerDBPath = QApplication::applicationDirPath() + "/123.db";
//    qDebug()<<scannerDBPath;
    scannerDataIndex = 1226;//第501条数据

    leftScannerAngle = 45;//从45度，逆时针扫描271度，间隔0.5度(541个有效数据点)
    rightScannerAngle = -135;//从-135度，逆时针扫描271度，间隔0.5度(541个有效数据点)
    rightToLeftAngle = 135;//(-135)+((541-1)*0.5);

    limitY = -1200;//剔除Y<0的数据
    offsetX = 30;//平移30mm
//    offsetX = 0.03;//平移30mm
//    offsetY = 0.03;
    distanceMin = 100;//标记间距小于100mm的数据点
//    distanceMin = 0.1;//标记间距小于100mm的数据点
    indexDistance = 20;//索引间隔阀值
    rangeUnderY = 50;//竖线点Y间隔100mm
    underCount = 50;//竖线补点个数

    heightPixel = 2560;
    widthPixel = 2048;
    find = new FindMaxMin(this);
}

void MainWindow::InitSubCfg()
{
    ui->plot->xAxis->setLabel("X");
    ui->plot->yAxis->setLabel("Y");
//    ui->plot->legend->setVisible(true);
}

void MainWindow::InitPlot()
{
    ui->plot->xAxis->setRange(-8000,4500);
    ui->plot->yAxis->setRange(-2500,5500);
//    ui->plot->xAxis->setRange(-8,4.5);
//    ui->plot->yAxis->setRange(-2.5,5.5);
    ui->plot->replot();
}

void MainWindow::GetScannerDataByDB()
{
//    qDebug()<<"11111";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",QString::number(DBLinkCount));
    DBLinkCount++;
    db.setHostName(".");
    db.setDatabaseName("./Data/123.db");
//    db.setConnectOptions();
    if(!db.open())
    {
        qDebug()<<"Open DB Failed!!!";
    }

    QSqlQuery query(db);
    QString strSql = "select * from LaserScanner order by No asc";
    if(query.exec(strSql))
    {
        if(query.seek(scannerDataIndex))
        {
            int id = query.value("No").toInt();
            int devId = query.value("DeviceID").toInt();
            qDebug()<<"id:"<<id;
            qDebug()<<"devId:"<<devId;
            QString temp = query.value("Data").toString();
            qDebug()<<"temp:"<<temp;
            LengthToCoordinate(temp,devId);
        }
        if(query.seek(scannerDataIndex + 1))
        {
            int id = query.value("No").toInt();
            int devId = query.value("DeviceID").toInt();
            qDebug()<<"id:"<<id;
            qDebug()<<"devId:"<<devId;
            QString temp = query.value("Data").toString();
            qDebug()<<"temp:"<<temp;
            LengthToCoordinate(temp,devId);
        }
    }
}

void MainWindow::GetCameraInfoByExcel()
{
    QAxObject excel("Excel.Application");
    excel.setProperty("Visble",false);
    QAxObject* workbooks = excel.querySubObject("WorkBooks");
    QAxObject* workbook = workbooks->querySubObject("Open(QString,QVariant,QVariant)", "F:\\QtMyProjectTest\\Parse\\Parse\\Data\\CameraInfo.xlsx",3,true);
    QAxObject* worksheet1 = workbook->querySubObject("Worksheets(int)",1);

    QAxObject* usedRange = worksheet1->querySubObject("UsedRange");
    QAxObject* rows = usedRange->querySubObject("Rows");
    QAxObject* columns = usedRange->querySubObject("Columns");

    int intRowStart = usedRange->property("Row").toInt();
    int intColStart = usedRange->property("Column").toInt();
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    qDebug()<<intRowStart<<intColStart<<intRows<<intCols;
    for (int i = intRowStart; i <= 30; i++)
    {
        CameraInfo* temp = new CameraInfo;
        for (int j = intColStart; j <= intColStart + 9; j++)
        {
            QAxObject * range = worksheet1->querySubObject("Cells(int,int)", i, j );
            qDebug() << i << j << range->property("Value");
            switch (j) {
            case 1:
                temp->CameraID = range->property("Value").toInt();
                break;
            case 2:
                temp->srcAngle = range->property("Value").toDouble();
                break;
            case 3:
                temp->destAngle = range->property("Value").toDouble();
                break;
            case 4:
                temp->PJ_ID = range->property("Value").toDouble();
                break;
            case 5:
                temp->focusLen = range->property("Value").toDouble();
                break;
            case 6:
            {
                double x = range->property("Value").toDouble();
                temp->centerPt.setX(x);
            }
                break;
            case 7:
            {
                double y = range->property("Value").toDouble();
                temp->centerPt.setY(y);
            }
                break;
            case 8:
                temp->senseHeight = range->property("Value").toDouble();
                break;
            case 9:
                temp->senseWidth = range->property("Value").toDouble();
                break;
            }
        }
        cameraInfoList.append(*temp);
    }

    workbook->dynamicCall("Close (Boolean)", true);
    excel.dynamicCall("Quit (void)");
}

void MainWindow::LengthToCoordinate(QString value, int id)
{
    QStringList list = value.split(" ");
    qDebug()<<"Len:"<<list.length();
    //判断数据起始角度
    double startAngle;
    if(id == 1)//1:右边的
    {
        startAngle = rightScannerAngle;
    }
    else if(id == 2)//2:左边的
    {
        startAngle = leftScannerAngle;
    }
    QList<QPointF> parseData;
    //长度转坐标
    for(int i = 0;i<list.length() - 6;i++)
    {
        if(i == 0)//第一个数据为数据长度值，不处理存储
        {
            continue;
        }
        QString temp = list[i];
        bool b;
        int length = temp.toInt(&b,16);
        double angle;
        angle = startAngle + (i-1)*0.5;
        QPointF pt = IntToCoordinate(length,angle,id);
        qDebug()<<id<<"pt"<<pt<<i;
        parseData.append(pt);
    }
    scannerData.insert(id,parseData);
}

QPointF MainWindow::IntToCoordinate(int value, double angle, int id)
{
    QPointF pt;

    qDebug()<<"Start:"<<value<<angle;
    qreal real = qDegreesToRadians(angle);
    qreal cos = qCos(real);
    qreal sin = qSin(real);
    qDebug()<<"###"<<cos<<sin;
    double x = value*qCos(real);
    double y = value*qSin(real);
    qDebug()<<"xy:"<<x<<y;
    pt.setX(x);
    pt.setY(y);
//    pt.setX(x/1000);//以米(m)为单位
//    pt.setY(y/1000);
    return pt;
}

void MainWindow::writeDataToExcel(QList<QPointF> data, int type)
{
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false);
    QAxObject * workbooks = excel.querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");
    QAxObject * workbook = excel.querySubObject("ActiveWorkBook");
    QAxObject * worksheets = workbook->querySubObject("WorkSheets");
    int intCount = worksheets->property("Count").toInt();
    qDebug()<<intCount;
    QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", 1);

//    QMap<double,double>::iterator iter;
//    int i = 1;
    for(int i = 0;i<data.length();i++)
    {
//        double x = iter.key();
//        double y = iter.value();
        QPointF pt = data[i];
        double x = pt.x();
        double y = pt.y();

        QString paramX,paramY;
        paramX = "Cells(" + QString::number(i+1) + ",1)";
        paramY = "Cells(" + QString::number(i+1) + ",2)";
        QByteArray baX;
        QByteArray baY;
        baX = paramX.toLatin1();
        baY = paramY.toLatin1();
        char* chX;
        char* chY;
        chX = baX.data();
        chY = baY.data();
        qDebug()<<chX<<chY;
        QAxObject * rangeX = worksheet->querySubObject(chX);
        bool b1 = rangeX->setProperty("Value",QVariant(x));
        qDebug()<<b1;
        QAxObject * rangeY = worksheet->querySubObject(chY);
        bool b2 = rangeY->setProperty("Value",QVariant(y));
        qDebug()<<b2;

//        i++;
        qDebug()<<i;
    }
    QString fileName;
    if(type == 1)
    {
        fileName = "F:\\QtMyProjectTest\\ScannerDataParse2\\Parse\\Data\\1.xlsx";
    }
    else if(type == 2)
    {
        fileName = "F:\\QtMyProjectTest\\ScannerDataParse2\\Parse\\Data\\2.xlsx";
    }
    else if(type == 0)
    {
        fileName = "F:\\QtMyProjectTest\\ScannerDataParse2\\Parse\\Data\\data.xlsx";
    }
    //保存时，提供的参数路径，需为Window格式，否则不会生成
    workbook->dynamicCall("SaveAs (const QString&)", fileName);
    workbook->dynamicCall("Close (Boolean)", true);
    excel.dynamicCall("Quit (void)");
}

void MainWindow::on_pushButton_9_clicked()
{
    GetCameraInfoByExcel();
    showCameraCenterPts();
}

//显示id=1解析数据(右半边)
void MainWindow::on_pushButton_2_clicked()
{
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Scanner1");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::black));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    QVector<double> vX,vY;
    QList<QPointF> temp;
    for(scannerDataIter = scannerData.begin();scannerDataIter != scannerData.end();scannerDataIter++)
    {
        if(scannerDataIter.key() == 1)
        {
            temp = scannerDataIter.value();
        }
    }
    foreach (QPointF pt, temp) {
        double x = pt.x();
        x = x + offsetX;//扫描仪1的数据x平移140mm
        double y = pt.y();
        vX.append(x);
        vY.append(y);
    }
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

//显示id=2解析数据
void MainWindow::on_pushButton_6_clicked()
{
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Scanner2");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::blue));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 1));
    QVector<double> vX,vY;
    QList<QPointF> temp;
    for(scannerDataIter = scannerData.begin();scannerDataIter != scannerData.end();scannerDataIter++)
    {
        if(scannerDataIter.key() == 2)
        {
            temp = scannerDataIter.value();
        }
    }
    foreach (QPointF pt, temp) {
        double x = pt.x();
        double y = pt.y();
        vX.append(x);
        vY.append(y);
    }
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

void MainWindow::on_pushButton_clicked()
{
    cameraIndex++;
    showInShootData();
    on_pushButton_8_clicked();
}

//剔除Y<0
void MainWindow::on_pushButton_3_clicked()
{
    //获取显示的数据
    QVector<double> vX,vY;
    QList<QPointF> temp;
    for(scannerDataIter = scannerData.begin();scannerDataIter != scannerData.end();scannerDataIter++)
    {
        int id = scannerDataIter.key();
        temp = scannerDataIter.value();
        /*
        //左右扫描仪数据直接合并，再根据X排序
        //此方法有问题，异常点的X值可能偏大，新的排序会导致打乱原有顺序，而剔除算法是根据前后数据点距离判断异常点
        foreach (QPointF pt, temp) {
            double x = pt.x();
            if(id == 1)
            {
                x = x + offsetX;
            }
            double y = pt.y();
            if(y >= limitY)
            {
                vX.append(x);
                vY.append(y);
                srcData.insert(x,y);//插入剔除Y<0的数据
            }
        }*/
        qDebug()<<"++++"<<id<<temp.length();
        //根据角度合并
        //右半边数据(id=1)一定先处理,提取右半边数据
        for(int i = 0;i<temp.length();i++) {
            QPointF pt = temp[i];
            double x = pt.x();
            double y = pt.y();
            if(id == 1)
            {
                double angle = this->rightScannerAngle + i*0.5;
                qDebug()<<"===="<<angle<<x<<y;
                x = x + offsetX;
                if(y >= limitY)//y >= limitY
                {
                    qDebug()<<"1111:"<<i<<angle<<x<<y;
                    vX.append(x);
                    vY.append(y);
                    QPointF destPt;
                    destPt.setX(x);
                    destPt.setY(y);
                    srcDataList.append(destPt);
                }
            }
            if(id == 2)
            {
                double angle = this->leftScannerAngle + i*0.5;                
                if(angle > rightToLeftAngle && y >= limitY)//只提取与右半边衔接的左半边数据，插入剔除Y<0数据
                {
                    qDebug()<<"2222:"<<i<<angle<<x<<y;
                    vX.append(x);
                    vY.append(y);
                    srcDataList.append(pt);
                }
            }
        }
        qDebug()<<"len:"<<srcDataList.length();
    }
    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Merge");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::red));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

//测试
void MainWindow::on_pushButton_5_clicked()
{
    double d = qDegreesToRadians(180.0);
    double r = qSin(d);
    qDebug()<<"r:"<<r;

    QVector<double> vX;
    QVector<double> vY;
    for(int i = 0;i<srcDataList.length();i++)
    {
        QPointF temp = srcDataList[i];
        double y = temp.y();
        vX.append(i);
        vY.append(y);
    }
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->graph(0)->setName("Test");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(0)->setPen(QPen(Qt::red));
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(0)->setData(vX,vY);
    ui->plot->graph(0)->rescaleAxes();
    ui->plot->replot();
}

//标记异常点
void MainWindow::on_pushButton_4_clicked()
{
    //存储所标记的异常点
    QVector<double> vX,vY;
    //计算每2个数据点距离，标记异常点
    int length = srcDataList.length();
    for(int i = 0;i<= length-2;i++)
    {
        QPointF prevPt;
        prevPt = srcDataList[i];
        QPointF nextPt;
        nextPt = srcDataList[i+1];
        double xParam = nextPt.x() - prevPt.x();
        double yParam = nextPt.y() - prevPt.y();
        qDebug()<<"Param:"<<xParam<<yParam;
        double xx = qPow(nextPt.x() - prevPt.x(),2);
        double yy = qPow(nextPt.y() - prevPt.y(),2);
        double ll = qSqrt(xx+yy);
        qDebug()<<"ll:"<<ll<<xx<<yy<<xx+yy<<i;
        if(ll>distanceMin)
        {
            double x = nextPt.x();
            double y = nextPt.y();
            vX.append(x);
            vY.append(y);
            BadIndex.append(i+1);
        }
    }
    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Exception");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::green));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

//更改Plot的X坐标显示范围
void MainWindow::on_pushButton_10_clicked()
{
    int min = ui->XMin->text().toInt();
    int max = ui->XMax->text().toInt();
    ui->plot->xAxis->setRange(min,max);
    ui->plot->replot();
}

//更改Plot的Y坐标显示范围
void MainWindow::on_pushButton_14_clicked()
{
    int min = ui->YMin->text().toInt();
    int max = ui->YMax->text().toInt();
    ui->plot->yAxis->setRange(min,max);
    ui->plot->replot();
}

void MainWindow::on_pushButton_15_clicked()
{
    InitPlot();
}

void MainWindow::on_change_clicked()
{
    //清除所有图层和集合数据
    ui->plot->clearGraphs();
    scannerData.clear();
    srcDataList.clear();
    BadIndex.clear();
    ErrorArea.clear();

    //获取下一份扫描仪数据
    scannerDataIndex += 2;
    GetScannerDataByDB();

    //显示
    on_pushButton_2_clicked();//显示id=1扫描仪数据
    QThread::sleep(1);
    on_pushButton_6_clicked();//显示id=2扫描仪数据
    QThread::sleep(1);
    on_pushButton_3_clicked();//剔除Y<0
    QThread::sleep(1);
    on_pushButton_4_clicked();//标记异常点
}

//写入Excel
void MainWindow::on_pushButton_17_clicked()
{
    writeDataToExcel(srcDataList,0);//扫描仪合并数据(已平移)

    for(scannerDataIter = scannerData.begin();scannerDataIter != scannerData.end();scannerDataIter++)
    {
        int id = scannerDataIter.key();
        QList<QPointF> temp = scannerDataIter.value();
        writeDataToExcel(temp,id);//左右扫描仪原始数据
    }
}

void MainWindow::LoopFind(int index)
{
    int start = BadIndex[index];
    qDebug()<<"LoopFind:"<<index<<start;
    int len = BadIndex.length();
    for(int i = 0;i<len;i++)
    {
        index++;
        if(index >= len)
        {
            return;
        }
        else if(index == len - 1)//遍历到最后一个元素点
        {
            int temp = BadIndex[index];
            if(temp - start <= indexDistance)
            {
                int end = temp;
                qDebug()<<"1:"<<start<<end<<index;
                Area area;
                area.startIndex = start;
                area.endIndex = end;
                ErrorArea.append(area);
            }
            else if(temp - start > indexDistance)
            {
                int end = BadIndex[index-1];
                if(end != start)//排除起始和结束为同一个数据点的情况
                {
                    qDebug()<<"2:"<<start<<end<<index;
                    Area area;
                    area.startIndex = start;
                    area.endIndex = end;
                    ErrorArea.append(area);
                }
            }
        }
        else
        {
            int temp = BadIndex[index];
            if(temp - start <= indexDistance)
            {
                continue;
            }
            else if(temp - start > indexDistance)
            {
                int end = BadIndex[index-1];
                if(end != start)//排除起始和结束为同一个数据点的情况
                {
                    qDebug()<<"3:"<<start<<end<<index;
                    Area area;
                    area.startIndex = start;
                    area.endIndex = end;
                    ErrorArea.append(area);
                }

                LoopFind(index);
                return;//结束前一次循环
            }
        }
    }
}

void MainWindow::showCameraCenterPts()
{
    QVector<double> vX,vY;
    foreach (CameraInfo info, cameraInfoList) {
        double x = info.centerPt.x();
        double y = info.centerPt.y();
        vX.append(x);
        vY.append(y);
    }
    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Camera");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::red));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

//计算病害点区域
void MainWindow::on_pushButton_18_clicked()
{
    //打印原始存储索引值
    qDebug()<<"--------TiChu---------";
    foreach (int i, BadIndex) {
        qDebug()<<i;
    }
    //计算病害点
    LoopFind(0);
    foreach (Area a, ErrorArea) {
        qDebug()<<"Area:"<<a.startIndex<<a.endIndex;
    }
}

//替换病害区域
void MainWindow::on_pushButton_19_clicked()
{
    QVector<double> vX,vY;
    foreach (Area a, ErrorArea) {
        int start = a.startIndex - 1;
        int end = a.endIndex + 1;
        int count = a.endIndex - a.startIndex + 1;//替换点个数
        QPointF startPt = srcDataList[start];
        QPointF endPt = srcDataList[end];
        double x1_x2 = endPt.x() - startPt.x();//小于0
        double rangeX = x1_x2/count;
        double y1_y2 = endPt.y() - startPt.y();
        double rangeY = y1_y2/count;
        for(int i = start + 1;i<end;i++)
        {
            double x = startPt.x() + (i - start)*rangeX;
            double y = startPt.y() + (i - start)*rangeY;
            vX.append(x);
            vY.append(y);
            QPointF temp(x,y);
            srcDataList.replace(i,temp);
        }
    }
    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Error");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::yellow));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

//补左右竖线
void MainWindow::on_pushButton_16_clicked()
{
    QVector<double> vX,vY;
    QPointF leftPt = srcDataList.last();
    QPointF rightPt = srcDataList.first();
    double leftY = leftPt.y();
    double leftX = leftPt.x();
    double rightY = rightPt.y();
    double rightX = rightPt.x();
    //补左竖线
    for(int i = 0;i<underCount;i++)
    {
        double y = leftY - rangeUnderY*(i+1);
        double x = leftX;
        vX.append(x);
        vY.append(y);
        QPointF temp(x,y);
        srcDataList.append(temp);
    }
    //补右竖线
    for(int i = 0;i<underCount;i++)
    {
        double y = rightY - rangeUnderY*(i+1);
        double x = rightX;
        vX.append(x);
        vY.append(y);
        QPointF temp(x,y);
        srcDataList.append(temp);
    }
    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("UnderY");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::blue));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();
    InitPlot();
}

void MainWindow::on_pushButton_7_clicked()
{
    QVector<double> vX,vY;//左端点
    QVector<double> vX2,vY2;//右端点
    QVector<double> vX3,vY3;//焦点
    for (int i = 0;i<cameraInfoList.length();i++) {
        CameraInfo info;
        info = cameraInfoList[i];
        double x = info.centerPt.x();
        double y = info.centerPt.y();
        double a = info.senseHeight;
        double angle = info.destAngle;
        double rad = qDegreesToRadians(angle);
        qDebug()<<"Camera:"<<x<<y<<a<<angle<<rad<<i;
        //C1.x=C.x- (a/2)*cos(θ)  C1.y=C.y- (a/2)*sin(θ)
        double x1,y1;
        double cos = qCos(rad);
        double sin = qSin(rad);
        qDebug()<<"cos:"<<cos<<"sin:"<<sin;
        x1 = x - (a/2)*cos;
        y1 = y - (a/2)*sin;
        cameraInfoList[i].leftPt.setX(x1);
        cameraInfoList[i].leftPt.setY(y1);
        vX.append(x1);
        vY.append(y1);
        qDebug()<<"left:"<<x1<<y1;
        //C2.x=C.x+ (a/2)*cos(θ)  C2.y=C.y+ (a/2)*sin(θ)
        double x2,y2;
        x2 = x + (a/2)*cos;
        y2 = y + (a/2)*sin;
        cameraInfoList[i].rightPt.setX(x2);
        cameraInfoList[i].rightPt.setY(y2);
        vX2.append(x2);
        vY2.append(y2);
        qDebug()<<"right:"<<x2<<y2;
        //焦点
        //F.x=C.x+f*cos(θ+pi/2)  F.y=C.y+f*sin(θ+pi/2)
        double focusX,focusY;
        double f = info.focusLen;
        double rad_2 = rad + PI/2;
        double cos2 = qCos(rad_2);
        double sin2 = qSin(rad_2);
        qDebug()<<"focusPrev:"<<f<<rad_2<<cos2<<sin2;
        focusX = x + f*cos2;
        focusY = y + f*sin2;
        cameraInfoList[i].focusPt.setX(focusX);
        cameraInfoList[i].focusPt.setY(focusY);
        vX3.append(focusX);
        vY3.append(focusY);
        qDebug()<<"focus:"<<focusX<<focusY;
        //计算起始和结束拍摄角度
        //θ1=θ+pi/2-atan((a/2)/f))
        double atan = qAtan((a/2)/f);
        cameraInfoList[i].shootStart = rad + PI/2 - atan;
        //θ2=θ+pi/2+atan((a/2)/f))
        cameraInfoList[i].shootEnd = rad + PI/2 + atan;
    }
    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("LeftRight");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::blue));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();
//    ui->plot->replot();

    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("LeftRight");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::green));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX2,vY2);
    ui->plot->graph(graphIndex)->rescaleAxes();

    //显示
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("Focus");
    //设置数据点之间连接线风格和画笔颜色
    ui->plot->graph(graphIndex)->setPen(QPen(Qt::red));
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX3,vY3);
    ui->plot->graph(graphIndex)->rescaleAxes();

    InitPlot();
}

void MainWindow::GetEachDataAngleToFocus()
{
    for(int i = 0;i<cameraInfoList.length();i++)
    {
        CameraInfo info;
        info = cameraInfoList[i];
        double focusX,focusY;
        focusX = info.focusPt.x();
        focusY = info.focusPt.y();
        double startAngle = info.shootStart;
        double endAngle = info.shootEnd;
        int id = info.PJ_ID;
        qDebug()<<"###"<<id<<focusX<<focusY<<startAngle<<endAngle;
//        Intersect* intersect = new Intersect();//存储范围内数据点
        Intersect intersect;
        //计算每个数据点
        //ktheta(i)=atan((Y(i)-F.y)/(X(i)-F.x))
        for(int j = 0;j<srcDataList.length();j++) {
            QPointF pt;
            pt = srcDataList[j];
            double dataX = pt.x();
            double dataY = pt.y();
            double x = dataX - focusX;
            double y = dataY - focusY;
            double ktheta;
            double laserKtheta;//最终数据点角度
            if(x == 0)//分母为0
            {
                qDebug()<<"ktheta:x=0"<<j;
            }
            else{
                ktheta = qAtan(y/x);
                qDebug()<<"ktheta:"<<ktheta<<id;
                if(y<= 0 && ktheta <= 0)
                {
                    laserKtheta = 2*PI + ktheta;
                }
                else if(y <= 0 && ktheta > 0)
                {
                    laserKtheta = PI + ktheta;
                }
                else if(y > 0 && ktheta >= 0)
                {
                    laserKtheta = 2*PI + ktheta;
                }
                else if(y > 0 && ktheta < 0)
                {
                    laserKtheta = PI + ktheta;
                }
                else{
                    qDebug()<<"Error laserKtheta!!!";
                }
                qDebug()<<"laserKtheta:"<<laserKtheta<<id;
            }
            //获取范围内数据点
            if(laserKtheta >= startAngle && laserKtheta <= endAngle)
            {
                qDebug()<<"InShoot:"<<id<<pt;
//                intersect->intersectPts.append(pt);
                intersect.intersectPts.append(pt);
            }
        }
        destData.append(intersect);
    }
}

void MainWindow::showInShootData()
{
    if(cameraIndex >= destData.length())
    {
        qDebug()<<"showInShootData():Out of Range!";
        return;
    }
    QVector<double> vX,vY;
//    Intersect* temp = destData[cameraIndex];
//    QList<QPointF> list = temp->intersectPts;
    Intersect temp = destData[cameraIndex];
    QList<QPointF> list = temp.intersectPts;
    qDebug()<<"showInShootData:"<<cameraIndex<<list.length();
    foreach (QPointF pt, list) {
        double x = pt.x();
        double y = pt.y();
        vX.append(x);
        vY.append(y);
    }
    ui->plot->addGraph();
    graphIndex++;
    ui->plot->graph(graphIndex)->setName("InShoot");
    //设置数据点之间连接线风格和画笔颜色
    if(cameraIndex%2 == 0)
    {
        ui->plot->graph(graphIndex)->setPen(QPen(Qt::green));
    }
    else{
        ui->plot->graph(graphIndex)->setPen(QPen(Qt::yellow));
    }
    ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsNone);
    //设置数据点风格(包括尺寸Size)
    ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
    ui->plot->graph(graphIndex)->setData(vX,vY);
    ui->plot->graph(graphIndex)->rescaleAxes();

    InitPlot();
}

//计算每个激光数据的角度(相对于焦点)，并获取范围内的数据点
void MainWindow::on_pushButton_12_clicked()
{
    //计算
    GetEachDataAngleToFocus();
    //显示
    showInShootData();
}

//拟合直线
bool MainWindow::analogLine(QList<QPointF> &points, double *a, double *b, double *c)
{
    int size = points.size();
    qDebug()<<"analogLine..."<<size;
    if(size < 2)
    {
        a = 0;
        b = 0;
        c = 0;
        return false;
    }
    double x_mean = 0;
    double y_mean = 0;
    for(int i = 0; i < size; i++)
    {
        x_mean += points[i].x();
        y_mean += points[i].y();
    }
    x_mean /= size;
    y_mean /= size; //至此，计算出了 x y 的均值
    qDebug()<<"mean:"<<x_mean<<y_mean;

    double Dxx = 0, Dxy = 0, Dyy = 0;

    for(int i = 0; i < size; i++)
    {
        Dxx += (points[i].x() - x_mean) * (points[i].x() - x_mean);
        Dxy += (points[i].x() - x_mean) * (points[i].y() - y_mean);
        Dyy += (points[i].y() - y_mean) * (points[i].y() - y_mean);
    }
    qDebug()<<"dxy:"<<Dxx<<Dxy<<Dyy;
    double lambda = ( (Dxx + Dyy) - sqrt( (Dxx - Dyy) * (Dxx - Dyy) + 4 * Dxy * Dxy) ) / 2.0;
    double den = sqrt( Dxy * Dxy + (lambda - Dxx) * (lambda - Dxx) );
    qDebug()<<"den:"<<lambda<<den;
    if(den != 0)
    {
        *a = Dxy / den;
        *b = (lambda - Dxx) / den;
        double aa,bb;
        aa = *a;
        bb = *b;
        *c = - aa * x_mean - bb * y_mean;
    }
    else{
        qDebug()<<"den==0";
        return false;
    }
    return true;
}

//范围内数据点拟合直线，并计算相交点
void MainWindow::on_pushButton_13_clicked()
{
    for(int i = 0;i<destData.length();i++)
    {
        //拟合直线ax+by+c=0
        double a = 0;
        double b = 0;
        double c = 0;
        qDebug()<<"###"<<destData.length()<<i;
//        double *a = 0;//初始化，防止系统随机给予为野指针
        Intersect sect = destData[i];
        QList<QPointF> temp = sect.intersectPts;
        if(temp.length() <= 0)
        {
            qDebug()<<"List is Empty!!!";
            return;
        }
        foreach (QPointF pt, temp) {
            qDebug()<<"pt:"<<pt;
        }
        bool bo = analogLine(temp,&a,&b,&c);
        if(bo)//ax+by+c=0;
        {
            qDebug()<<"abc:"<<a<<b<<c;
        }
        else//x=c;x-c=0;
        {
            QPointF pt = temp[0];
            double x = pt.x();
            a = 1;
            b = 0;
            c = -x;
        }
        qDebug()<<"abc2:"<<a<<b<<c;//ax+by+c=0
        //左端点与焦点直线y1=k1*x+b1,k1=(y1-y)/(x1-x),b1=y1-k1*x1;
        double k1,b1;
        CameraInfo camera;
        camera = cameraInfoList[i];
        double y = camera.focusPt.y();
        double x = camera.focusPt.x();
        double y1 = camera.leftPt.y();
        double x1 = camera.leftPt.x();
        k1 = (y1-y)/(x1-x);
        b1 = y1 - k1*x1;
        qDebug()<<"y1:"<<k1<<b1;
        //右端点与焦点直线y2=k2*x+b2;
        double k2,b2;
        double y2 = camera.rightPt.y();
        double x2 = camera.rightPt.x();
        k2 = (y2-y)/(x2-x);
        b2 = y2 - k2*x2;
        //计算左直线与拟合直线相交点
        //ax+by+c=0;y1=k1*x+b1;
        //ax+b(k1*x+b1)+c=0;(a+k1*b)x+(b*b1+c)=0;x=-(b*b1+c)/(a+k1*b);y=k1*x+b1;
        double param1 = a+k1*b;
        double intersect1_x;
        double intersect1_y;
        if(param1 != 0)
        {
            intersect1_x = -(b*b1+c)/(a+k1*b);
            intersect1_y = k1*intersect1_x + b1;
        }
        else{
            qDebug()<<"intersect1:param1=0!!!";
        }
        cameraInfoList[i].leftIntersect.setX(intersect1_x);
        cameraInfoList[i].leftIntersect.setY(intersect1_y);
        Sect sectInfo;
        sectInfo.leftPt.setX(intersect1_x);
        sectInfo.leftPt.setY(intersect1_y);
        //计算右直线与拟合直线相交点
        double param2 = a+k2*b;
        double intersect2_x;
        double intersect2_y;
        if(param2 != 0)
        {
            intersect2_x = -(b*b2+c)/(a+k2*b);
            intersect2_y = k2*intersect2_x + b2;
        }
        else{
            qDebug()<<"intersect2:param2=0!!!";
        }
        cameraInfoList[i].rightIntersect.setX(intersect2_x);
        cameraInfoList[i].rightIntersect.setY(intersect2_y);
        sectInfo.rightPt.setX(intersect2_x);
        sectInfo.rightPt.setY(intersect2_y);
        sectInfoList.append(sectInfo);
    }
}

//显示相交点
void MainWindow::on_pushButton_8_clicked()
{
//    foreach (CameraInfo info, cameraInfoList) {
    {
        CameraInfo info = cameraInfoList[cameraIndex];
        QVector<double> vX1,vY1,vX2,vY2;
        double x1,x,x2;
        double y1,y,y2;
        x1 = info.leftPt.x();
        x = info.focusPt.x();
        x2 = info.leftIntersect.x();
        vX1.append(x1);
        vX1.append(x);
        vX1.append(x2);
        y1 = info.leftPt.y();
        y = info.focusPt.y();
        y2 = info.leftIntersect.y();
        vY1.append(y1);
        vY1.append(y);
        vY1.append(y2);

        double x1_2,x_2,x2_2;
        double y1_2,y_2,y2_2;
        x1_2 = info.rightPt.x();
        x_2 = info.focusPt.x();
        x2_2 = info.rightIntersect.x();
        vX2.append(x1_2);
        vX2.append(x_2);
        vX2.append(x2_2);
        y1_2 = info.rightPt.y();
        y_2 = info.focusPt.y();
        y2_2 = info.rightIntersect.y();
        vY2.append(y1_2);
        vY2.append(y_2);
        vY2.append(y2_2);
        //显示(left为红，right为绿)
        ui->plot->addGraph();
        graphIndex++;
        ui->plot->graph(graphIndex)->setName("Intersect1");
        //设置数据点之间连接线风格和画笔颜色
        ui->plot->graph(graphIndex)->setPen(QPen(Qt::red));
        ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsLine);
        //设置数据点风格(包括尺寸Size)
        ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
        ui->plot->graph(graphIndex)->setData(vX1,vY1);
        ui->plot->graph(graphIndex)->rescaleAxes();

        //显示(left为红，right为绿)
        ui->plot->addGraph();
        graphIndex++;
        ui->plot->graph(graphIndex)->setName("Intersect2");
        //设置数据点之间连接线风格和画笔颜色
        ui->plot->graph(graphIndex)->setPen(QPen(Qt::green));
        ui->plot->graph(graphIndex)->setLineStyle(QCPGraph::lsLine);
        //设置数据点风格(包括尺寸Size)
        ui->plot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
        ui->plot->graph(graphIndex)->setData(vX2,vY2);
        ui->plot->graph(graphIndex)->rescaleAxes();

        InitPlot();
    }
}

//计算裁剪重叠区域
void MainWindow::on_pushButton_20_clicked()
{
    qDebug()<<"###"<<sectInfoList.length();
    //计算每个相机图片分辨率，获取最小分辨率
    for(int i = 0;i<sectInfoList.length();i++) {
        Sect sect = sectInfoList[i];
        QPointF pt1,pt2;
        pt1 = sect.leftPt;
        pt2 = sect.rightPt;
        double pow1 = qPow(pt2.x() - pt1.x(),2);
        double pow2 = qPow(pt2.y() - pt1.y(),2);
        double dis = qSqrt(pow1 + pow2);
        double imgPixel = dis/heightPixel;
        qDebug()<<"imgPixel:"<<pow1<<pow2<<dis<<imgPixel;
        pixelMap.insert(imgPixel,i);
        sectInfoList[i].srcPixel = imgPixel;
    }
    refer_ratio = pixelMap.firstKey();//最小分辨率
    //计算分辨率缩放比例
    for(int i = 0;i<sectInfoList.length();i++)
    {
        qDebug()<<"srcPixel:"<<sectInfoList[i].srcPixel<<refer_ratio;
        double k;
        k = sectInfoList[i].srcPixel/refer_ratio;
        sectInfoList[i].scale = k;
        qDebug()<<"scale:"<<sectInfoList[i].scale;

        find->scaleData.insertMulti(k,i);
    }
    //计算重叠区域所占像素
    sectInfoList[0].pixel = 0;//默认重叠区域算到下一个，第一个永远为0
    for(int i = 0;i<sectInfoList.length()- 1;i++)
    {
        Sect sect1 = sectInfoList[i];
        //排序2个相交点
        QPointF pt1,pt2;//pt1在左，pt2在右(相交点从左到右排序)
        if(sect1.leftPt.x() <= sect1.rightPt.x())
        {
            pt1 = sect1.leftPt;
            pt2 = sect1.rightPt;
        }
        else{
            pt1 = sect1.rightPt;
            pt2 = sect1.leftPt;
        }
        Sect sect2 = sectInfoList[i+1];
        //排序2个相交点
        QPointF pt3,pt4;
        if(sect2.leftPt.x() <= sect2.rightPt.x())
        {
            pt3 = sect2.leftPt;
            pt4 = sect2.rightPt;
        }
        else{
            pt3 = sect2.rightPt;
            pt4 = sect2.leftPt;
        }
        //判断是否有重叠区域
        qDebug()<<"pt:"<<pt2<<pt3<<i;
        qDebug()<<"pt:"<<pt1<<pt4;
        if(pt3.x() >= pt2.x())//无重叠区域
        {
            sectInfoList[i+1].pixel = 50;
            qDebug()<<"pixel:0";
        }
        else{
            qDebug()<<"sect1:"<<sect1.leftPt<<sect1.rightPt;//left为红，right为绿
            qDebug()<<"sect2:"<<sect2.leftPt<<sect2.rightPt;
            double powX,powY;
            powX = qPow(sect2.rightPt.x() - sect1.leftPt.x(),2);
            powY = qPow(sect2.rightPt.y() - sect1.leftPt.y(),2);
            double wrap_dis = qSqrt(powX + powY);
            int temp = wrap_dis/refer_ratio;//像素值取整
            qDebug()<<"pixel:"<<powX<<powY<<powX+powY<<wrap_dis<<temp;
            sectInfoList[i+1].pixel = temp;
        }
    }   
}

//图像上下拼接
Mat Mosaic_ud(Mat imgL,Mat imgR,int move)
{
    //左边图的高度和宽度
    int H1=imgL.rows;
    int W1=imgL.cols;
    qDebug()<<H1;

    //右边图的高度和宽度
    int H2=imgR.rows;
    int W2=imgR.cols;

    //计算拼接图的宽度，并声明新图result
    int new_row=H1+H2-move;
    int new_col=W1;
    Mat result(new_row,new_col,CV_8UC(imgL.channels()));

    // 将左图放在result中
    Mat imgROIL=result(Rect(0,0,W1,H1));
    imgL.copyTo(imgROIL);

    //将右图放在result中
    Mat imgROIR=result(Rect(0,H1-move,W2,H2));
    imgR.copyTo(imgROIR);
    //imshow("imgR",imgR);
    //将左图的ROI区域选出
    Mat ROI_L=imgL(Rect(0,H1-move,W1,move));

    //将右图的ROI区域选出
    Mat ROI_R=imgR(Rect(0,0,W2,move));

    //将两个区域进行融合,利用线性叠加

    for(int j=0,alpha=move;j<move;++j,alpha--)
    {
        Mat img_col_L=ROI_L(Rect(0,j,W1,1));//选取左图的j行
        Mat img_col_R=ROI_R(Rect(0,j,W2,1));//选取右图的j行

        Mat dst;
        double beta = double(alpha)/double(move);

        double gamma=1.0-beta;
        addWeighted( img_col_L, beta, img_col_R, gamma, 0.0, dst);
        Mat Mid_ROI=result(Rect(0,H1-move+j,W1,1));
        dst.copyTo(Mid_ROI);
    }
    return result;
}


//图像左右拼接
Mat Mosaic_lr(Mat imgL,Mat imgR,int move)
{
    //左边图的高度和宽度
    int H1=imgL.rows;
    int W1=imgL.cols;

    //右边图的高度和宽度
    int H2=imgR.rows;
    int W2=imgR.cols;

    //计算拼接图的宽度，并声明新图result
    int new_row=H1;
    int new_col=W1+W2-move;
    Mat result(new_row,new_col,CV_8UC(imgL.channels()));

    // 将左图放在result中
    Mat imgROIL=result(Rect(0,0,W1,H1));
    imgL.copyTo(imgROIL);

    //将右图放在result中
    Mat imgROIR=result(Rect(W1-move,0,W2,H1));
    imgR.copyTo(imgROIR);

    //将左图的ROI区域选出
    Mat ROI_L=imgL(Rect(W1-move,0,move,H1));

    //将右图的ROI区域选出
    Mat ROI_R=imgR(Rect(0,0,move,H2));

    //将两个区域进行融合,利用线性叠加

    for(int j=0,alpha=move;j<move;++j,alpha--)
    {
        Mat img_col_L=ROI_L(Rect(j,0,1,H1));//选取左图的j列
        Mat img_col_R=ROI_R(Rect(j,0,1,H2));//选取右图的j列
        Mat dst;
        double beta = double(alpha)/double(move);
        double gamma=1.0-beta;
        addWeighted(img_col_L, beta, img_col_R, gamma, 0.0, dst);
        Mat Mid_ROI=result(Rect(W1-move+j,0,1,H1));
        dst.copyTo(Mid_ROI);
    }
    return result;
}


//图像顺时针旋转90度
Mat img_rote90(Mat img)
{
    transpose(img,img);
    flip(img,img,1);
    return img;
}

//根据计算参数对图像进行缩放(以最大尺寸作为标准,图像居中,空白区域补白)
Mat img_Scale(Mat img,double scale,int Max_Col,QList<int> First_Col,int k)
{
    cv::resize(img,img,Size(),scale,scale,CV_INTER_LINEAR);

    Mat temp(img.rows,Max_Col,img.depth());

    int firstcol=First_Col.at(k);
    qDebug()<<"firstcol"<<firstcol;
    Mat imgROIL=temp(Rect(firstcol,0,img.cols,img.rows));
    img.copyTo(imgROIL);
    return temp;
}
void MainWindow::on_pushButton_11_clicked()
{
    //显示最终结果(缩放倍数、重叠像素)
    for(int i = 0;i<sectInfoList.length();i++)
    {
        qDebug()<<"res:"<<sectInfoList[i].srcPixel<<sectInfoList[i].scale<<sectInfoList[i].pixel<<i;
    }

    //显示最终结果(缩放倍数、重叠像素)
    for(int i = 0;i<sectInfoList.length();i++)
    {
        qDebug()<<"res:"<<sectInfoList[i].srcPixel<<sectInfoList[i].scale<<sectInfoList[i].pixel<<i;
    }

    QString rootPath = QFileDialog::getExistingDirectory(this,"Dir",".");
    qDebug()<<"###"<<rootPath;
    //Worker* worker = new Worker();

 //   double Scale_img=0.2;//对原始图像的缩放倍数
    double Scale_max = find->GetDoubleMax();//获取缩放尺寸的最大值
    qDebug()<<"Scale_max:"<<Scale_max;
    int Max_Col=Scale_max*widthPixel*scale+1.5;//获取图像宽度的最大值（此处加1.5的目的是为了防止起始位置为0)

    //对图像进行倍数缩放并居中
    QList<int> First_Col;//左侧填补区域起始像素
    QList<int> Last_Col;//包含图像内容最右侧的像素
    QList<int> img_height;//每个相机的图像在环向图中的位置(起始行）
    QList<int> imgScale_height;//每个相机缩放后的高度
    QList<int> imgScale_width;//每个相机缩放后的宽度
    for(int j=0;j<30;j++)
    {
        double scale_j=sectInfoList[j].scale;
        int  left_col=(Max_Col-scale_j*widthPixel*scale)*0.5+0.5;
        First_Col.append(left_col);
        int  scale_height=scale_j*heightPixel*scale+0.5;
        int  scale_width =scale_j*widthPixel*scale+0.5;
        int  right_col=left_col+scale_width;
        imgScale_height.append(scale_height);
        imgScale_width.append(scale_width);
        Last_Col.append(right_col);
        if (j==0)
        {
            img_height.append(0);
        }
        else
        {
            int temp=img_height.at(j-1)+imgScale_height.at(j)-sectInfoList[j].pixel*scale;
            img_height.append(temp);
            qDebug()<<"img_height"<<j<<" "<<img_height.at(j);
            //qDebug()<<"img_height-0"<<img_height.at(0);
        }
    }
    QList<int> First_COL_Copy=First_Col;
    //排序后获取左侧填补最大值
    qSort(First_COL_Copy.begin(),First_COL_Copy.end());
    for(int i=0;i<First_COL_Copy.size();i++)
    {
        qDebug()<<"i:"<<i<<"First_Col"<<First_COL_Copy.at(i);
    }

    int Max_First_col=First_COL_Copy.at(First_COL_Copy.size()-1);
    qDebug()<<"Max_First_col"<<Max_First_col;

    QList<int> Last_COL_Copy=Last_Col;
    //排序后获取右侧(有效图像）的最小值
    qSort(Last_COL_Copy.begin(),Last_COL_Copy.end());
    int Min_Last_col=Last_COL_Copy.at(0);
    qDebug()<<"Min_Last_col"<<Min_Last_col;

    qDebug()<<"Max_Col"<<Max_Col;
    //i是图像编号
    //获取环向拼接的起始和结束图像索引
    int first_img=sectList[0].startImgIndex;
    int last_img=sectList[sectList.size()-1].endImgIndex;
//    for(int i=first_img;i<last_img+1;i++)
   for(int i=500;i<501;i++)
    {
        Worker worker;
        Mat OpImg;
//        qDebug()<<"1"<<i;
        worker.FindFullPath(rootPath,i);

        qDebug()<<"Length:"<<worker.destList.length();
        //j是相机编号
        for(int j=0;j<worker.destList.length();j++)
//      for(int j=0;j<1;j++)
        {
            QString qStr1 = worker.destList.at(j);
            qDebug()<<qStr1;
            char* ch1 = qStr1.toLocal8Bit().data();
            string str1 = ch1;

            Mat imgR=imread(str1,0);
            cv::resize(imgR,imgR,Size(),scale,scale,1);//对图像进行尺寸缩放
            qDebug()<<"imgR"<<imgR.rows<<imgR.cols;
            Mat imgR90=img_rote90(imgR);
            qDebug()<<"imgR_rote"<<imgR90.rows<<imgR90.cols;
//          imwrite("H://123.jpg",imgR90);
            double scale_j=sectInfoList[j].scale;
            imgR90=img_Scale(imgR90,scale_j,Max_Col,First_Col,j);
            qDebug()<<"imgR90"<<imgR90.rows<<imgR90.cols;
//          qDebug()<<worker.destList.at(j);
            int move1=sectInfoList[j].pixel;
            qDebug()<<move1;
            int move2=(int)(move1*scale);
            qDebug()<<"move2"<<move2;
            //对图像上下拼接
            if (j==0)
                imgR90.copyTo(OpImg);
            else
                OpImg=Mosaic_ud(OpImg,imgR90,move2);
            imgR.release();
        }
        Mat OpImg_cut=OpImg(Rect(Max_First_col,0,(Min_Last_col-Max_First_col),OpImg.rows));
        QString img_name=QString("H:\\pano\\%1.jpg").arg(i);
        imwrite(img_name.toStdString(),OpImg_cut);
    }

    //对图像进行纵向拼接
    for(int i=0;i<sectList.size();i++)
 //  for(int i=0;i<1;i++)
   {
       Mat Final_pano;//对拼接段进行声明
       int startImg_NUM=sectList[i].startImgIndex;
       int endImg_NUM=sectList[i].endImgIndex;
       int left_cut=sectList[i].leftCutPixel;
       int right_cut=sectList[i].rightCutPixel;
       int unnecessary_pixel=sectList[i].warpPixel;
      for(int k1=startImg_NUM;k1<endImg_NUM+1;k1++)
 //       for(int k1=startImg_NUM;k1<529;k1++)
        {
            qDebug()<<"K1"<<k1;
            QString img_name_temp=QString("H:\\pano\\%1.jpg").arg(k1);

            Mat temp=imread(img_name_temp.toStdString(),0);
            flip(temp,temp,1);


            if(k1==startImg_NUM)
                Final_pano=temp;
            else
            {
                int wrap_pixel=CJLList[k1-startImg_NUM-1].warpPixel;
                Final_pano=Mosaic_lr(Final_pano,temp,wrap_pixel);
                qDebug()<<"wrapPixel"<<wrap_pixel;
            }
            temp.release();
        }
 //        Mat Final_pano_cut=Final_pano(Rect(left_cut,0,(Final_pano.cols-left_cut-right_cut-unnecessary_pixel),Final_pano.rows));
//        Mat Final_pano_cut=Final_pano(Rect(left_cut,0,(Final_pano.cols),Final_pano.rows));
        QString img_name_final=QString("H:\\pano_all\\%1-%2.jpg").arg(startImg_NUM).arg(endImg_NUM);
        imwrite(img_name_final.toStdString(), Final_pano);
    }
}

void MainWindow::on_handleCJL_btn_clicked()
{
    Form* form1 = new Form();//需释放
    form1->show();
}

void MainWindow::on_pushButton_21_clicked()
{
    foreach (CeJuLun temp, CJLList) {
        qDebug()<<"CJL:"<<temp.id<<temp.disAdd<<temp.disFrom0<<temp.halfDisAdd<<temp.startMile<<temp.endMile<<temp.validMile<<temp.validPixel<<temp.realPixel<<temp.warpPixel<<temp.realMile<<temp.leftCutPixel<<temp.rightCutPixel;
    }
    foreach (Section s, sectList) {
        qDebug()<<"Section:"<<s.startMile<<s.endMile<<s.startImgIndex<<s.endImgIndex<<s.realStartMile<<s.realEndMile<<s.warpPixel<<s.leftCutPixel<<s.rightCutPixel;
    }
}
