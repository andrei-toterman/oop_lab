TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    movie.cpp \
    controller.cpp \
    ui.cpp \
    repository.cpp \
    tests.cpp

HEADERS += \
    movie.h \
    dynamicarray.h \
    controller.h \
    ui.h \
    repository.h \
    tests.h
