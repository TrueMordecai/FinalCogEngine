QT += core gui
QT += widgets
QT += serialport
QT += multimedia

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=                      \
    mycsv.cpp           \
    mysensor.cpp        \
    myserialport.cpp    \
    mywindow.cpp        \
    main.cpp                    \
    mywindowScenario.cpp

HEADERS +=                      \
    include/mycsv.h             \
    include/mysensor.h          \
    include/myserialport.h      \
    include/mywindow.h          \

INCLUDEPATH += include/
