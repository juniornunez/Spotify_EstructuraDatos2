QT += core widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    loginUI.cpp \
    registerui.cpp \
    userdata.cpp

HEADERS += \
    loginUI.h \
    registerui.h \
    userdata.h

FORMS += \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
