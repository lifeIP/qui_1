QT       += widgets

CONFIG   += c++11
QMAKE_CXXFLAGS += -finput-charset=UTF-8
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
           errorlogio.cpp \
           activity.cpp \
           values.cpp \
           control/controlthread.cpp \
           control/debugconsole.cpp \
           widgets/statusbarwidget.cpp \
           widgets/bottomnavigationbar.cpp \
           widgets/iconbuttonwidget.cpp \
           widgets/textbuttonwidget.cpp \
           widgets/selector.cpp \
           widgets/doorselector.cpp \
           widgets/selector-button.cpp \
           widgets/parametereditdialog.cpp \
           pages/dopingpagewidget.cpp \
           pages/mainpagewidget.cpp \
           pages/finalpagewidget.cpp \
           pages/autotpagewidget.cpp \
           pages/autodopingpagewidget.cpp \
           pages/vacuumpagewidget.cpp \
           pages/gaspanelpagewidget.cpp \
           pages/settingspagewidget.cpp \
           pages/errorlogpagewidget.cpp \
           pages/errorarchivepagewidget.cpp

HEADERS += errorlogio.h \
           activity.h \
           values.h \
           control/controlthread.h \
           control/debugconsole.h \
           widgets/statusbarwidget.h \
           widgets/bottomnavigationbar.h \
           widgets/iconbuttonwidget.h \
           widgets/textbuttonwidget.h \
           widgets/selector.hpp \
           widgets/doorselector.hpp \
           widgets/selector-button.hpp \
           widgets/parametereditdialog.h \
           pages/dopingpagewidget.h \
           pages/mainpagewidget.h \
           pages/finalpagewidget.h \
           pages/autotpagewidget.h \
           pages/autodopingpagewidget.h \
           pages/vacuumpagewidget.h \
           pages/gaspanelpagewidget.h \
           pages/settingspagewidget.h \
           pages/errorlogpagewidget.h \
           pages/errorarchivepagewidget.h