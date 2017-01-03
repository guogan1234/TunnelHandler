#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T14:06:25
#
#-------------------------------------------------

QT       += core gui printsupport sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Parse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    findmaxmin.cpp \
    worker.cpp \
    form.cpp \
    formtest.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    findmaxmin.h \
    worker.h \
    form.h \
    formtest.h

FORMS    += mainwindow.ui \
    form.ui \
    formtest.ui


INCLUDEPATH += D:\software\opencv2.4.4\build\include\opencv2\
    D:\software\opencv2.4.4\build\include\opencv2\
    D:\software\opencv2.4.4\build\include\
    D:\software\opencv2.4.4\build\include\opencv2\highgui\
    D:\software\opencv2.4.4\build\include\opencv2\core\
    D:\software\openvv2.4.4\build\include\opencv2\imgproc\

# 包含目前openCV版本的全部DLL

LIBS += -LD:\software\opencv2.4.4\build\x86\vc10\lib -lopencv_core244 -lopencv_highgui244 -lopencv_calib3d244\
    -lopencv_contrib244 -lopencv_features2d244 -lopencv_flann244 -lopencv_gpu244 -lopencv_imgproc244 -lopencv_legacy244\
    -lopencv_ml244 -lopencv_nonfree244 -lopencv_objdetect244 -lopencv_photo244 -lopencv_stitching244 -lopencv_ts244\
    -lopencv_video244 -lopencv_videostab244


