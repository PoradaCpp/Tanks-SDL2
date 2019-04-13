TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mainwindow.cpp \
    texture.cpp \
    surface.cpp \
    button.cpp \
    startpage.cpp \
    text3d.cpp \
    tankarrow.cpp \
    state.cpp \
    renderer.cpp \
    font.cpp \
    startpageinitdata.cpp \
    createmappageinitdata.cpp \
    createmappage.cpp \
    buildmap.cpp

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
    mainwindow.h \
    texture.h \
    surface.h \
    button.h \
    page.h \
    startpage.h \
    text3d.h \
    dynamicobject.h \
    tankarrow.h \
    state.h \
    movealgorithm.h \
    displayedobject.h \
    renderer.h \
    font.h \
    startpageinitdata.h \
    initdata.h \
    createmappageinitdata.h \
    createmappage.h \
    buildmap.h
