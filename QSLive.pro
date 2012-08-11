TEMPLATE = app
QT += core gui phonon network xml
HEADERS += header/centralwidget.h \
    header/channellist.h \
    header/xmlparser.h \
    header/myVideoWidget.h \
    header/spprocess.h \
    header/utils.h \
    header/mainwindow.h \
    header/settingdialog.h \
    header/channeldialog.h \
    header/treewidget.h

SOURCES += source/centralwidget.cpp \
    source/channellist.cpp \
    source/main.cpp \
    source/xmlparser.cpp \
    source/myvideowidget.cpp \
    source/spprocess.cpp \
    source/utils.cpp \
    source/mainwindow.cpp \
    source/settingdialog.cpp \
    source/channeldialog.cpp \
    source/treewidget.cpp

RESOURCES += \
    rsc.qrc

FORMS += \
    form/settingdialog.ui \
    form/channeldialog.ui
TRANSLATIONS += resource/locale/zh_CN.ts

TARGET = qslive
DESTDIR = bin
unix {
target.files = bin/qslive
target.path = /usr/local/bin
locales.files += resource/locale/zh_CN.qm
locales.path = /usr/local/share/qslive/locales
desktop.files += resource/QSLive.desktop
desktop.path = /usr/local/share/applications
INSTALLS += target \
    locales \
    desktop
}
