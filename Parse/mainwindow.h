#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "findmaxmin.h"
#include "form.h"
#include "formtest.h"

#define PI 3.1415926

//相机参数
struct CameraInfo{
    int PJ_ID;//实际拼接编号
    int CameraID;//相机编号
    double srcAngle;
    double destAngle;
    double focusLen;
    QPointF centerPt;
    double senseHeight;
    double senseWidth;
    //计算得出的结果
    QPointF leftPt;
    QPointF rightPt;
    QPointF focusPt;
    double shootStart;//拍摄起始角度
    double shootEnd;//拍摄结束角度
    QPointF leftIntersect;//左直线相交点
    QPointF rightIntersect;//右直线相交点
};

struct Sect{
    QPointF leftPt;
    QPointF rightPt;
    int pixel;//重叠区域像素
    double srcPixel;//原始相机对应图像分辨率
    double scale;//相机对应图像缩放比例
};

//病害点区域
struct Area{
    int startIndex;
    int endIndex;
};

class Intersect
{
public:
    Intersect() {}

    QPointF left;
    QPointF right;
    QPointF focus;

    QList<QPointF> intersectPts;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_change_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_11_clicked();

    void on_handleCJL_btn_clicked();

    void on_pushButton_21_clicked();

private:
    Ui::MainWindow *ui;

    int cameraIndex;//相机索引
    int graphIndex;//图层索引
//    int heightPixel;//计算分辨率所用的图片某一边的像素值

    QString scannerDBPath;
    int scannerDataIndex;//哪一行扫描仪数据

    double leftScannerAngle;
    double rightScannerAngle;
    double rightToLeftAngle;//从右半边到左半边数据衔接的角度，即右半边扫描的最大角度

    double offsetX;//一个扫描仪数据X平移offsetX毫米
    double offsetY;//一个扫描仪数据Y平移offsetY毫米
    double limitY;//底部Y坐标阀值
    double distanceMin;//标记异常点间距阀值
    int indexDistance;//异常点索引间距阀值
    double rangeUnderY;//补竖线点Y间隔
    int underCount;//补竖线点个数

    void Init();
    void InitParams();
    void InitSubCfg();
    void InitPlot();

    void GetScannerDataByDB();
    void GetCameraInfoByExcel();

    void LengthToCoordinate(QString value,int id);
    QPointF IntToCoordinate(int value,double angle,int id);

    void writeDataToExcel(QList<QPointF> data,int type);

    //计算病害点
    void LoopFind(int index);

    //显示传感器中心点
    void showCameraCenterPts();

    //获取每个数据点角度(相对于焦点)，并获取范围内数据点
    void GetEachDataAngleToFocus();
    //显示拍摄范围内数据点
    void showInShootData();
    //拟合直线
    bool analogLine(QList<QPointF> &points,double* a,double* b,double* c);

    FindMaxMin* find;
//    Form* ff;
};

#endif // MAINWINDOW_H
