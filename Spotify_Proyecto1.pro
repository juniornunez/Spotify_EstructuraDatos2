QT += core widgets
QT += multimedia

CONFIG += c++17

SOURCES += \
    addplaylistsongs.cpp \
    addsingleui.cpp \
    admindata.cpp \
    adminmenuui.cpp \
    adminprofileui.cpp \
    artistcardwidget.cpp \
    artistsettingsui.cpp \
    horizontalsongcard.cpp \
    main.cpp \
    loginUI.cpp \
    managesongsui.cpp \
    playbarui.cpp \
    playlistui.cpp \
    questionui.cpp \
    registeradminui.cpp \
    registerui.cpp \
    songcardwidget.cpp \
    songdata.cpp \
    trendingui.cpp \
    userdata.cpp \
    usermenuui.cpp \
    userprofileui.cpp

HEADERS += \
    addplaylistsongs.h \
    addsingleui.h \
    admindata.h \
    adminmenuui.h \
    adminprofileui.h \
    artistcardwidget.h \
    artistsettingsui.h \
    horizontalsongcard.h \
    loginUI.h \
    managesongsui.h \
    playbarui.h \
    playlistui.h \
    questionui.h \
    registeradminui.h \
    registerui.h \
    songcardwidget.h \
    songdata.h \
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
