TEMPLATE = app
TARGET = calc_qt
INCLUDEPATH += ..
QT += widgets
CONFIG += c++14


HEADERS += calc.h                      \
           $$INCLUDEPATH/common.hpp    \
           $$INCLUDEPATH/calcOptr.hpp  \
           $$INCLUDEPATH/str.hpp       \
           $$INCLUDEPATH/calcError.hpp \
           ui_calc.h

FORMS += calc.ui
SOURCES += calc.cpp main.cpp $$INCLUDEPATH/calcOptr.cpp $$INCLUDEPATH/str.cpp $$INCLUDEPATH/calcError.cpp
LIBS += -lQt5Widgets
