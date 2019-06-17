TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    animation.cpp \
    gameengine.cpp \
    mainwindow.cpp \
    playersheart.cpp \
    random_uint32_t.cpp \
    surface.cpp \
    startpage.cpp \
    tank.cpp \
    tankofenemy.cpp \
    tankofplayer.cpp \
    tankproperties.cpp \
    tankshell.cpp \
    text3d.cpp \
    tankarrow.cpp \
    state.cpp \
    renderer.cpp \
    font.cpp \
    startpageinitdata.cpp \
    createmappageinitdata.cpp \
    createmappage.cpp \
    buildmap.cpp \
    mapobject.cpp \
    map.cpp \
    texture.cpp \
    buttonnamed.cpp \
    button.cpp \
    gamepageinitdata.cpp \
    gamepage.cpp

# Command
# -L[Directory path of "lib" folder] -lSDL2
LIBS += -LC://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2-2.0.9//i686-w64-mingw32//lib -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2
LIBS += -LC://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2_image-2.0.4//i686-w64-mingw32//lib -lSDL2_image
LIBS += -LC://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2_ttf-2.0.14//i686-w64-mingw32//lib -lSDL2_ttf
LIBS += -LC://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2_mixer-2.0.4//i686-w64-mingw32//lib -lSDL2_mixer

# [Directory of "include"]
INCLUDEPATH += C://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2-2.0.9//i686-w64-mingw32//include//SDL2
INCLUDEPATH += C://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2_image-2.0.4//i686-w64-mingw32//include//SDL2
INCLUDEPATH += C://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2_ttf-2.0.14//i686-w64-mingw32//include//SDL2
INCLUDEPATH += C://Users//Workstation//Documents//CPP//mingw_dev_lib//SDL2_mixer-2.0.4//i686-w64-mingw32//include//SDL2

HEADERS += \
    animation.h \
    dynamicobject.h \
    gameengine.h \
    mainwindow.h \
    pageinitdata.h \
    playersheart.h \
    random_uint32_t.h \
    surface.h \
    page.h \
    startpage.h \
    tank.h \
    tankofenemy.h \
    tankofplayer.h \
    tankproperties.h \
    tankshell.h \
    text3d.h \
    tankarrow.h \
    state.h \
    movealgorithm.h \
    displayedobject.h \
    renderer.h \
    font.h \
    startpageinitdata.h \
    createmappageinitdata.h \
    createmappage.h \
    buildmap.h \
    mapobject.h \
    map.h \
    texture.h \
    buttonnamed.h \
    button.h \
    gamepageinitdata.h \
    gamepage.h
