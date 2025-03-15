QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    console.cpp \
    globalclass.cpp \
    helpwidget.cpp \
    main.cpp \
    mainwidget.cpp \
    protocoltransferwidget.cpp \
    sendmultiplemessageswidget.cpp \
    sendsinglemessagewidget.cpp \
    serialportinfo.cpp \
    serialportwidget.cpp \
    slavethread.cpp

HEADERS += \
    console.h \
    globalclass.h \
    helpwidget.h \
    mainwidget.h \
    protocoltransferwidget.h \
    sendmultiplemessageswidget.h \
    sendsinglemessagewidget.h \
    serialportinfo.h \
    serialportwidget.h \
    slavethread.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
