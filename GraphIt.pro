QT     += core gui widgets

QMAKE_CXXFLAGS += -bigobj

CONFIG += c++17
INCLUDEPATH += include external

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    src/buttons.cpp \
    src/function.cpp \
    src/graphlayer.cpp \
    src/mainwin.cpp

HEADERS += \
    external/exprtk.h \
    include/buttons.h \
    include/function.h \
    include/graphlayer.h \
    include/mainwin.h

RESOURCES += \
        resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
