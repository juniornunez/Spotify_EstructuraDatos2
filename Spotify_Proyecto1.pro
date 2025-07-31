QT += core widgets

CONFIG += c++17

SOURCES += \
    admindata.cpp \
    adminmenuui.cpp \
    adminprofileui.cpp \
    artistsettingsui.cpp \
    main.cpp \
    loginUI.cpp \
    questionui.cpp \
    registeradminui.cpp \
    registerui.cpp \
    userdata.cpp \
    usermenuui.cpp

HEADERS += \
    admindata.h \
    adminmenuui.h \
    adminprofileui.h \
    artistsettingsui.h \
    loginUI.h \
    questionui.h \
    registeradminui.h \
    registerui.h \
    userdata.h \
    usermenuui.h

FORMS += \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
