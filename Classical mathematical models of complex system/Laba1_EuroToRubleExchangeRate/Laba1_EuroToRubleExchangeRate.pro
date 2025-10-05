QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx code for Application Qt project.
QXLSX_PARENTPATH = ./../../QXlsx-master/QXlsx/
QXLSX_HEADERPATH = ./../../QXlsx-master/QXlsx/header/
QXLSX_SOURCEPATH = ./../../QXlsx-master/QXlsx/source/
include(./../../QXlsx-master/QXlsx/QXlsx.pri)

SOURCES += \
    formregression.cpp \
    main.cpp \
    mainwindow.cpp \
    myfuncs.cpp \
    qcustomplot.cpp

HEADERS += \
    RegressionValues.h \
    formregression.h \
    mainwindow.h \
    myfuncs.h \
    qcustomplot.h

FORMS += \
    formregression.ui \
    mainwindow.ui

win32:RC_FILE = ico_resource.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
