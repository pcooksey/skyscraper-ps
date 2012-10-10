#-------------------------------------------------
#
# Project created by QtCreator 2012-06-27T14:33:51
#
#-------------------------------------------------

QT       += core gui

TARGET = Skyscraper-gui
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    openfile.cpp \
    Skyscraper/SkyScraper.cc \
    Skyscraper/Puzzle.cpp \
    Skyscraper/PartialSolver.cpp \
    lcdentry.cpp \
    rowgenerator.cpp

HEADERS  += mainwindow.h \
    openfile.h \
    Skyscraper/SkyScraper.hh \
    Skyscraper/Puzzle.hh \
    Skyscraper/PartialSolver.hh \
    lcdentry.h \
    rowgenerator.h

FORMS    += mainwindow.ui \
    openfile.ui \
    rowgenerator.ui
