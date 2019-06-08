TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        controller.cpp \
        csvwatchlist.cpp \
        filerepository.cpp \
        htmlwatchlist.cpp \
        main.cpp \
        movie.cpp \
        repository.cpp \
        tests.cpp \
        ui.cpp \
        utils.cpp

HEADERS += \
    controller.h \
    csvwatchlist.h \
    exceptions.h \
    filerepository.h \
    htmlwatchlist.h \
    movie.h \
    movievalidator.h \
    repository.h \
    tests.h \
    ui.h \
    utils.h
