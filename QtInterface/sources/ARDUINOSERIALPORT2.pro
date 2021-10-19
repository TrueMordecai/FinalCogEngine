QT += core gui
QT += widgets
QT += serialport
QT += multimedia

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=          \
    capteur.cpp     \
    include/mycsv.cpp \
    include/mysensor.cpp \
    include/myserialport.cpp \
    include/mywindow.cpp \
    main.cpp        \
    mainwindow.cpp  \
    mywidget.cpp    \
    serialport.cpp

HEADERS +=                  \
    include/capteur.h       \
    include/mainwindow.h    \
    include/mycsv.h \
    include/mysensor.h \
    include/myserialport.h \
    include/mywidget.h      \
    include/mywindow.h \
    include/serialport.h

INCLUDEPATH += /usr/include/opencv4
INCLUDEPATH += /usr/include/python3.8
INCLUDEPATH += include/

LIBS += `pkg-config --cflags --libs opencv4`
LIBS += $(shell pkg-config opencv --libs)
DISTFILES += alarme.wav
