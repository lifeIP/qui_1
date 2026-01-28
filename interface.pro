QT       += widgets

CONFIG   += c++11
TEMPLATE = app
TARGET   = interface

SOURCES += main.cpp \
           statusbarwidget.cpp \
           bottomnavigationbar.cpp \
           dopingpagewidget.cpp \
           pages/mainpagewidget.cpp

HEADERS += statusbarwidget.h \
           bottomnavigationbar.h \
           dopingpagewidget.h \
           pages/mainpagewidget.h