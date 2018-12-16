#-------------------------------------------------
#
# Project created by QtCreator 2017-06-07T11:08:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT +=printsupport

TARGET = JsonEditor
TEMPLATE = app


SOURCES += main.cpp\
    JsonCoreOperation.cpp \
    HandleJson.cpp \
    TreeView.cpp \
    World.cpp \
    Child.cpp

HEADERS  += \
    JsonCoreOperation.h \
    HandleJson.h \
    TreeView.h \
    World.h \
    Child.h

RESOURCES += \
    myword.qrc

