#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <QKeyEvent>

#include "widgets/statusbarwidget.h"
#include "widgets/bottomnavigationbar.h"
#include "pages/dopingpagewidget.h"
#include "pages/mainpagewidget.h"
#include "pages/finalpagewidget.h"
#include "pages/autotpagewidget.h"
#include "pages/autodopingpagewidget.h"
#include "pages/vacuumpagewidget.h"
#include "pages/gaspanelpagewidget.h"
#include "pages/settingspagewidget.h"
#include "control/controlthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("Панель управления");
        setMinimumSize(768, 1024);
        resize(768, 1024);

        // Центральный виджет
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Корневой вертикальный лэйаут без отступов по краям
        QVBoxLayout *rootLayout = new QVBoxLayout(centralWidget);
        rootLayout->setSpacing(0);
        rootLayout->setContentsMargins(0, 0, 0, 0);

        // Панель статуса (общая для всех страниц)
        statusBar = new StatusBarWidget(this);
        rootLayout->addWidget(statusBar);

        // Стек страниц
        stackedWidget = new QStackedWidget(this);
        rootLayout->addWidget(stackedWidget, 1);

        // Страница "Главный"
        stackedWidget->addWidget(new MainPageWidget(this));

        // Страница "Легирование" с текущим интерфейсом
        DopingPageWidget *dopingPage = new DopingPageWidget(this);
        stackedWidget->addWidget(dopingPage);

        // Страница "Финальные"
        stackedWidget->addWidget(new FinalPageWidget(this));

        // Страница "Автотяга"
        stackedWidget->addWidget(new AutotPageWidget(this));

        // Страница "Автолегирование" (настройка автолегирования)
        autodopingPage = new AutodopingPageWidget(this);
        stackedWidget->addWidget(autodopingPage);

        // Остальные страницы
        stackedWidget->addWidget(new VacuumPageWidget(this));
        stackedWidget->addWidget(new GasPanelPageWidget(this));
        stackedWidget->addWidget(new SettingsPageWidget(this));

        // Нижняя навигация
        bottomNav = new BottomNavigationBar(this);
        rootLayout->addWidget(bottomNav);

        connect(bottomNav, &BottomNavigationBar::pageSelected,
                this, &MainWindow::setActivePage);

        // Переход на страницу автолегирования из страницы легирования
        connect(dopingPage, &DopingPageWidget::openAutodopingRequested,
                this, &MainWindow::openAutodopingPage);

        // Активная страница по умолчанию — "Легирование"
        setActivePage(0);

        // Таймер для обновления времени
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
        timer->start(1000);
        updateTime();

        // Запуск приложения в full-screen режиме
        showFullScreen();
    }

private slots:
    void updateTime()
    {
        const QTime t = QTime::currentTime();
        const QDate d = QDate::currentDate();

        if (statusBar)
            statusBar->setTime(t);
        if (bottomNav)
            bottomNav->setDateTime(t, d);
    }

private:
    // индексы страниц в QStackedWidget
    enum PageIndex {
        PageMain = 0,
        PageDoping = 1,
        PageFinal = 2,
        PageAutot = 3,
        PageVacuum = 4,
        PageGasPanel = 5,
        PageSettings = 6
    };

    StatusBarWidget *statusBar;
    BottomNavigationBar *bottomNav;

    QStackedWidget *stackedWidget;
    QWidget *autodopingPage = nullptr;

    QWidget* createSimplePage(const QString &title)
    {
        QWidget *page = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(page);
        layout->setContentsMargins(0, 0, 0, 0);
        QLabel *label = new QLabel(title);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("QLabel { font-size: 24px; color: #bdc3c7; }");
        layout->addStretch();
        layout->addWidget(label);
        layout->addStretch();
        return page;
    }

    void setActivePage(int pageIndex)
    {
        if (!stackedWidget)
            return;

        stackedWidget->setCurrentIndex(pageIndex);
        if (bottomNav)
            bottomNav->setActivePage(pageIndex);
    }

    // Открыть страницу "Автолегирование" без изменения активной вкладки навигации
    void openAutodopingPage()
    {
        if (!stackedWidget || !autodopingPage)
            return;
        stackedWidget->setCurrentWidget(autodopingPage);
        // Нижняя навигация остаётся на странице "Легирование"
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_F11) {
            if (isFullScreen()) {
                showNormal();
                setFixedSize(768, 1024);
            } else {
                setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
                showFullScreen();
            }
            event->accept();
            return;
        }

        QMainWindow::keyPressEvent(event);
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Запуск управляющего потока
    ControlThread *controlThread = new ControlThread();
    controlThread->start();

    MainWindow window;
    window.show();

    int result = app.exec();

    // Остановка управляющего потока
    if (controlThread) {
        controlThread->stop();
        controlThread->wait();
        delete controlThread;
    }

    return result;
}
