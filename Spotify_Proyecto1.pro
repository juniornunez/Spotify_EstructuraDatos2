QT += core widgets
QT += multimedia
QT += charts
CONFIG += c++17

SOURCES += \
    addalbumui.cpp \
    addplaylistsongs.cpp \
    addsingleui.cpp \
    admindata.cpp \
    adminmenuui.cpp \
    adminprofileui.cpp \
    albumcardwidget.cpp \
    artistcardwidget.cpp \
    artistsettingsui.cpp \
    horizontalsongcard.cpp \
    main.cpp \
    loginUI.cpp \
    playbarui.cpp \
    playlistdisplayui.cpp \
    questionui.cpp \
    registeradminui.cpp \
    registerui.cpp \
    songcardwidget.cpp \
    songdata.cpp \
    songraterui.cpp \
    trendingui.cpp \
    userdata.cpp \
    usermenuui.cpp \
    userprofileui.cpp

HEADERS += \
    addalbumui.h \
    addplaylistsongs.h \
    addsingleui.h \
    admindata.h \
    adminmenuui.h \
    adminprofileui.h \
    albumcardwidget.h \
    artistcardwidget.h \
    artistsettingsui.h \
    horizontalsongcard.h \
    loginUI.h \
    managesongsui.h \
    playbarui.h \
    playlistdisplayui.h \
    questionui.h \
    registeradminui.h \
    registerui.h \
    songcardwidget.h \
    songdata.h \
    songraterui.h \
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
