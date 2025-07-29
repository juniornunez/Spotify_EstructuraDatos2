QT += core widgets

CONFIG += c++17

SOURCES += \
    admindata.cpp \
    main.cpp \
    loginUI.cpp \
    questionui.cpp \
    registeradminui.cpp \
    registerui.cpp \
    userdata.cpp

HEADERS += \
    admindata.h \
    loginUI.h \
    questionui.h \
    registeradminui.h \
    registerui.h \
    userdata.h

FORMS += \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
