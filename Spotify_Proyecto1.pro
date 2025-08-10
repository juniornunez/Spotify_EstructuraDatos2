QT += core widgets
QT += multimedia

CONFIG += c++17

SOURCES += \
    addsingleui.cpp \
    admindata.cpp \
    adminmenuui.cpp \
    adminprofileui.cpp \
    artistcardwidget.cpp \
    artistsettingsui.cpp \
    main.cpp \
    loginUI.cpp \
    playbarui.cpp \
    questionui.cpp \
    registeradminui.cpp \
    registerui.cpp \
    songcardwidget.cpp \
    trendingui.cpp \
    userdata.cpp \
    usermenuui.cpp \
    userprofileui.cpp

HEADERS += \
    addsingleui.h \
    admindata.h \
    adminmenuui.h \
    adminprofileui.h \
    artistcardwidget.h \
    artistsettingsui.h \
    loginUI.h \
    playbarui.h \
    questionui.h \
    registeradminui.h \
    registerui.h \
    songcardwidget.h \
    trendingui.h \
    userdata.h \
    usermenuui.h \
    userprofileui.h

FORMS += \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
