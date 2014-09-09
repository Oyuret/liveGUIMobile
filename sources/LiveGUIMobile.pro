#-------------------------------------------------
#
# Project created by QtCreator 2014-08-10T00:34:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LiveGUIMobile
TEMPLATE = app


SOURCES += \
    src/abstracthandler.cpp \
    src/browsingwidget.cpp \
    src/favoriteitemwidget.cpp \
    src/favoritewidget.cpp \
    src/game.cpp \
    src/gamesitemdelegate.cpp \
    src/gameswidget.cpp \
    src/mainwindow.cpp \
    src/networkmanager.cpp \
    src/previewstreamwidget.cpp \
    src/remoteplayer.cpp \
    src/service.cpp \
    src/stream.cpp \
    src/streamitemwidget.cpp \
    src/streamswidget.cpp \
    src/twitchhandler.cpp \
    main.cpp \
    src/azubuhandler.cpp

HEADERS  += \
    include/abstracthandler.h \
    include/browsingwidget.h \
    include/favoriteitemwidget.h \
    include/favoritewidget.h \
    include/game.h \
    include/gamesitemdelegate.h \
    include/gameswidget.h \
    include/mainwindow.h \
    include/networkmanager.h \
    include/previewstreamwidget.h \
    include/remoteplayer.h \
    include/service.h \
    include/stream.h \
    include/streamitemwidget.h \
    include/streamswidget.h \
    include/twitchhandler.h \
    include/azubuhandler.h

FORMS    += \
    ui/browsingwidget.ui \
    ui/favoriteitemwidget.ui \
    ui/favoritewidget.ui \
    ui/gameswidget.ui \
    ui/mainwindow.ui \
    ui/previewstreamwidget.ui \
    ui/streamitemwidget.ui \
    ui/streamswidget.ui

CONFIG += mobility
MOBILITY = 

INCLUDEPATH += include

RESOURCES += \
    qrc/icons.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

