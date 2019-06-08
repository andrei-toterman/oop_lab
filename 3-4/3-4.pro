TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    controller.c \
    material.c \
    material_repository.c \
    operation.c \
    ui.c

HEADERS += \
    controller.h \
    material.h \
    material_repository.h \
    operation.h \
    ui.h
