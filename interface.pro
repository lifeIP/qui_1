QT       += widgets

CONFIG   += c++11
TEMPLATE = app
TARGET   = interface

# Все артефакты сборки (бинарник, объектные файлы, moc/ui/rcc)
# будут складываться в подпапку build относitelьно корня проекта
DESTDIR     = build
OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
RCC_DIR     = build/rcc
UI_DIR      = build/ui

# Открывать консоль для отладки (вывод qDebug и других сообщений)
CONFIG += console

SOURCES += main.cpp \
           activity.cpp \
           values.cpp \
           widgets/statusbarwidget.cpp \
           widgets/bottomnavigationbar.cpp \
           widgets/iconbuttonwidget.cpp \
           widgets/textbuttonwidget.cpp \
           widgets/selector.cpp \
           widgets/selector-button.cpp \
           pages/dopingpagewidget.cpp \
           pages/mainpagewidget.cpp \
           pages/finalpagewidget.cpp \
           pages/autotpagewidget.cpp \
           pages/vacuumpagewidget.cpp

HEADERS += activity.h \
           values.h \
           widgets/statusbarwidget.h \
           widgets/bottomnavigationbar.h \
           widgets/iconbuttonwidget.h \
           widgets/textbuttonwidget.h \
           widgets/selector.hpp \
           widgets/selector-button.hpp \
           pages/dopingpagewidget.h \
           pages/mainpagewidget.h \
           pages/finalpagewidget.h \
           pages/autotpagewidget.h \
           pages/vacuumpagewidget.h