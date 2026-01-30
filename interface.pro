QT       += widgets

CONFIG   += c++11
TEMPLATE = app
TARGET   = interface

SOURCES += main.cpp \
           widgets/statusbarwidget.cpp \
           widgets/bottomnavigationbar.cpp \
           widgets/iconbuttonwidget.cpp \
           widgets/textbuttonwidget.cpp \
           pages/dopingpagewidget.cpp \
           pages/mainpagewidget.cpp

HEADERS += widgets/statusbarwidget.h \
           widgets/bottomnavigationbar.h \
           widgets/iconbuttonwidget.h \
           widgets/textbuttonwidget.h \
           pages/dopingpagewidget.h \
           pages/mainpagewidget.h