#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTimer>

struct CeJuLun{
    int id;
    double distance;
    double disAdd;//距离增量
    double disFrom0;//离洞口距离
    double halfDisAdd;//距离增量的一半
    double startMile;
    double endMile;
    double validMile;
    int validPixel;
    int realPixel;
    int warpPixel;//重叠区域像素
//    int imageFlag;
    double leftCutPixel;
    double rightCutPixel;
    double realMile;//拼接段标志距离
};

struct Section{
    int startMile;
    int endMile;
    int startImgIndex;
    int endImgIndex;
    double realStartMile;
    double realEndMile;
    int warpPixel;
    int leftCutPixel;
    int rightCutPixel;
};

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void on_start_btn_clicked();

    void on_change_btn_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void TimeOutHandleFunc();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Form *ui;

    QSqlDatabase db;
    int count;
    int seekIndex;
    double leftStartAngle;
    double rightStartAngle;
    double offsetX;//一个扫描仪数据X平移offsetX毫米
    double rightToLeftAngle;

    //
    double limit_minX;
    double limit_maxY;
    double limit_meanY;

    QString inTime;
    QString outTime;

    bool bAuto;
    QTimer* tm;
    int findFlag;

    void InitParams();
    void openDB(QString DBPath);
    void QueryData(int flag);
    void ParseData(QString data,int id);
    void MergeData();
    void ShowData();
    void HandleData();

    //测距轮
    double disLevel;
    bool bIn;
    bool bOut;
    double prevDis;

    double referRatio;
//    double scale;
    double real_Pixel;
    double dt;
    double startLevel;//进洞起始点，可能不为0
    void UpdateDBTime();
    void ShowCJLData();
    void GetUsedCJLData();
};

#endif // MAINWINDOW_H
