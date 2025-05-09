QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CC = gcc
QMAKE_CFLAGS += -std=c11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$files(view/*.cpp, true) \
           $$files(model/*.c, true) \
           main.cpp

HEADERS += $$files(view/*.h, true) \
           $$files(model/*.h, true) \
           $$files(3rdparty/*.h, true)

FORMS += \
    view/widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    view/resourses.qrc

DISTFILES +=
