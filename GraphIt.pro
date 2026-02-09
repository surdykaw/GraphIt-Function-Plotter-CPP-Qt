QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -bigobj

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    function.cpp \
    graphlayer.cpp \
    main.cpp \
    mainwin.cpp

HEADERS += \
    exprtk.h \
    function.h \
    graphlayer.h \
    mainwin.h

DISTFILES += \
        rsc/arrowUp.png \
        rsc/arrowDown.png \
        rsc/mainAxArrUp.png \
        rsc/mainAxArrRight.png \
        rsc/minusRounded.png \
        rsc/minusRounded_40.png \
        rsc/plusCircle.png \
        rsc/plusCircle_40.png



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

