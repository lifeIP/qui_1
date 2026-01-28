#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QFrame>
#include <QTime>
#include <QDate>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("Панель управления");
        setMinimumSize(1000, 700);

        // Центральный виджет
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Корневой вертикальный лэйаут
        QVBoxLayout *rootLayout = new QVBoxLayout(centralWidget);
        rootLayout->setSpacing(20);
        rootLayout->setContentsMargins(20, 20, 20, 20);

        // Панель статуса (общая для всех страниц)
        createStatusBar(rootLayout);

        // Стек страниц
        stackedWidget = new QStackedWidget(this);
        rootLayout->addWidget(stackedWidget, 1);

        // Страница "Главный" (заглушка)
        stackedWidget->addWidget(createSimplePage("Главный"));

        // Страница "Легирование" с текущим интерфейсом
        QWidget *dopingPage = new QWidget();
        QVBoxLayout *dopingLayout = new QVBoxLayout(dopingPage);
        dopingLayout->setSpacing(20);
        createDopingSection(dopingLayout);
        createFlowPressureSection(dopingLayout);
        dopingLayout->addStretch();
        stackedWidget->addWidget(dopingPage);

        // Остальные страницы (заглушки)
        stackedWidget->addWidget(createSimplePage("Финальные"));
        stackedWidget->addWidget(createSimplePage("Автотяга"));
        stackedWidget->addWidget(createSimplePage("Вакуум"));
        stackedWidget->addWidget(createSimplePage("Газ. панель"));
        stackedWidget->addWidget(createSimplePage("Настройки"));

        // Нижняя навигация
        createBottomNavigation(rootLayout);

        // Активная страница по умолчанию — "Легирование"
        setActivePage(1);

        // Таймер для обновления времени
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
        timer->start(1000);
        updateTime();
    }

private slots:
    void updateTime()
    {
        const QTime t = QTime::currentTime();
        const QDate d = QDate::currentDate();

        if (timeLabel)
            timeLabel->setText(t.toString("hh:mm:ss"));
        if (bottomTimeLabel)
            bottomTimeLabel->setText(t.toString("hh:mm"));
        if (bottomDateLabel)
            bottomDateLabel->setText(d.toString("dd/MM/yy"));
    }

    void onArgonStop()
    {
        argonStopButton->setText(argonStopButton->text() == "СТОП" ? "ПУСК" : "СТОП");
        argonStopButton->setStyleSheet(argonStopButton->text() == "СТОП" 
            ? "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }"
            : "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }");
    }

    void onInjectionStart()
    {
        injectionStartButton->setText(injectionStartButton->text() == "ПУСК" ? "СТОП" : "ПУСК");
        injectionStartButton->setStyleSheet(injectionStartButton->text() == "ПУСК"
            ? "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }"
            : "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }");
    }

    void onPhosphorusSelect()
    {
        phosphorusButton->setStyleSheet("QPushButton { background-color: #34495e; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
        diboraneButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
    }

    void onDiboraneSelect()
    {
        diboraneButton->setStyleSheet("QPushButton { background-color: #34495e; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
        phosphorusButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
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

    QLabel *timeLabel;
    QLabel *bottomTimeLabel;
    QLabel *bottomDateLabel;

    QPushButton *argonStopButton;
    QPushButton *injectionStartButton;
    QPushButton *phosphorusButton;
    QPushButton *diboraneButton;

    QStackedWidget *stackedWidget;

    // Кнопки навигации
    QPushButton *navMainButton;
    QPushButton *navDopingButton;
    QPushButton *navFinalButton;
    QPushButton *navAutotButton;
    QPushButton *navVacuumButton;
    QPushButton *navGasPanelButton;
    QPushButton *navSettingsButton;

    void createStatusBar(QVBoxLayout *mainLayout)
    {
        QFrame *statusFrame = new QFrame();
        statusFrame->setStyleSheet("QFrame { background-color: #ecf0f1; border-radius: 5px; padding: 10px; }");
        QHBoxLayout *statusLayout = new QHBoxLayout(statusFrame);
        statusLayout->setContentsMargins(15, 10, 15, 10);

        // Время
        timeLabel = new QLabel();
        timeLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; }");
        statusLayout->addWidget(timeLabel);

        statusLayout->addStretch();

        // Красная точка и статус
        QLabel *statusDot = new QLabel("●");
        statusDot->setStyleSheet("QLabel { color: #e74c3c; font-size: 20px; }");
        statusLayout->addWidget(statusDot);

        QLabel *statusText = new QLabel("Нет связи с контроллером");
        statusText->setStyleSheet("QLabel { font-size: 16px; color: #2c3e50; }");
        statusLayout->addWidget(statusText);

        statusLayout->addStretch();

        // Кнопка сброса
        QPushButton *resetButton = new QPushButton("Сбросить");
        resetButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; font-size: 14px; padding: 8px 20px; }");
        statusLayout->addWidget(resetButton);

        mainLayout->addWidget(statusFrame);
    }

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

    QFrame* createCard(const QString &title, const QString &value, QWidget *parent = nullptr)
    {
        QFrame *card = new QFrame(parent);
        card->setStyleSheet("QFrame { background-color: white; border-radius: 10px; padding: 20px; }");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(10);

        QLabel *titleLabel = new QLabel(title);
        titleLabel->setStyleSheet("QLabel { font-size: 16px; color: #2c3e50; font-weight: bold; }");
        cardLayout->addWidget(titleLabel);

        QLabel *valueLabel = new QLabel(value);
        valueLabel->setStyleSheet("QLabel { font-size: 24px; color: #34495e; font-weight: bold; }");
        cardLayout->addWidget(valueLabel);

        cardLayout->addStretch();

        return card;
    }

    void createDopingSection(QVBoxLayout *mainLayout)
    {
        QLabel *sectionTitle = new QLabel("Легирование");
        sectionTitle->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
        mainLayout->addWidget(sectionTitle);

        QFrame *dopingFrame = new QFrame();
        QGridLayout *gridLayout = new QGridLayout(dopingFrame);
        gridLayout->setSpacing(15);
        gridLayout->setContentsMargins(0, 0, 0, 0);

        // Аргон
        QFrame *argonCard = createCard("Аргон", "0.5 л/мин");
        argonCard->setMinimumHeight(150);
        QVBoxLayout *argonLayout = qobject_cast<QVBoxLayout*>(argonCard->layout());
        argonStopButton = new QPushButton("СТОП");
        argonStopButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }");
        connect(argonStopButton, &QPushButton::clicked, this, &MainWindow::onArgonStop);
        argonLayout->addWidget(argonStopButton);
        gridLayout->addWidget(argonCard, 0, 0);

        // Легирующий газ
        QFrame *dopingGasCard = createCard("Легирующий газ", "0.0 мл/мин");
        dopingGasCard->setMinimumHeight(150);
        QVBoxLayout *dopingGasLayout = qobject_cast<QVBoxLayout*>(dopingGasCard->layout());
        QHBoxLayout *gasButtonsLayout = new QHBoxLayout();
        gasButtonsLayout->setSpacing(10);
        phosphorusButton = new QPushButton("Фосфор");
        phosphorusButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
        connect(phosphorusButton, &QPushButton::clicked, this, &MainWindow::onPhosphorusSelect);
        diboraneButton = new QPushButton("Диборан");
        diboraneButton->setStyleSheet("QPushButton { background-color: #34495e; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
        connect(diboraneButton, &QPushButton::clicked, this, &MainWindow::onDiboraneSelect);
        gasButtonsLayout->addWidget(phosphorusButton);
        gasButtonsLayout->addWidget(diboraneButton);
        dopingGasLayout->addLayout(gasButtonsLayout);
        gridLayout->addWidget(dopingGasCard, 0, 1);

        // Впрыск
        QFrame *injectionCard = createCard("Впрыск", "0.0 л/мин");
        injectionCard->setMinimumHeight(150);
        QVBoxLayout *injectionLayout = qobject_cast<QVBoxLayout*>(injectionCard->layout());
        injectionStartButton = new QPushButton("ПУСК");
        injectionStartButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }");
        connect(injectionStartButton, &QPushButton::clicked, this, &MainWindow::onInjectionStart);
        injectionLayout->addWidget(injectionStartButton);
        gridLayout->addWidget(injectionCard, 1, 0);

        // Продув
        QFrame *purgeCard = createCard("Продув", "0.0 бар");
        purgeCard->setMinimumHeight(150);
        gridLayout->addWidget(purgeCard, 1, 1);

        mainLayout->addWidget(dopingFrame);
    }

    void createFlowPressureSection(QVBoxLayout *mainLayout)
    {
        QLabel *sectionTitle = new QLabel("Регулировка расхода и давления");
        sectionTitle->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
        mainLayout->addWidget(sectionTitle);

        QFrame *flowFrame = new QFrame();
        QHBoxLayout *flowLayout = new QHBoxLayout(flowFrame);
        flowLayout->setSpacing(15);
        flowLayout->setContentsMargins(0, 0, 0, 0);

        QFrame *argonCard = createCard("Аргон", "0.0 л/мин");
        argonCard->setMinimumHeight(80);
        QFrame *dopingGasCard = createCard("Легирующий газ", "0.0 мл/мин");
        dopingGasCard->setMinimumHeight(80);
        QFrame *injectionCard = createCard("Впрыск", "0.0 мл/мин");
        injectionCard->setMinimumHeight(80);
        QFrame *purgeCard = createCard("Продув", "0.0 бар");
        purgeCard->setMinimumHeight(80);

        flowLayout->addWidget(argonCard);
        flowLayout->addWidget(dopingGasCard);
        flowLayout->addWidget(injectionCard);
        flowLayout->addWidget(purgeCard);

        mainLayout->addWidget(flowFrame);
    }

    void createBottomNavigation(QVBoxLayout *rootLayout)
    {
        QFrame *navFrame = new QFrame();
        navFrame->setStyleSheet("QFrame { background-color: #111111; }");
        navFrame->setMinimumHeight(110);
        QHBoxLayout *navLayout = new QHBoxLayout(navFrame);
        navLayout->setContentsMargins(20, 10, 20, 10);
        navLayout->setSpacing(30);

        // Левая часть — кнопки навигации
        QHBoxLayout *buttonsLayout = new QHBoxLayout();
        buttonsLayout->setSpacing(25);

        auto makeNavButton = [&](const QString &text, int pageIndex) -> QPushButton* {
            QPushButton *btn = new QPushButton(text);
            btn->setCheckable(true);
            btn->setProperty("pageIndex", pageIndex);
            btn->setStyleSheet(
                "QPushButton { color: #bdc3c7; background-color: #222222; border: none; font-size: 16px; padding: 16px 10px; border-radius: 8px; }"
                "QPushButton:pressed { background-color: #333333; }"
                "QPushButton:checked { color: white; background-color: #ffffff; border-radius: 8px; }");
            btn->setMinimumSize(110, 70);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            connect(btn, &QPushButton::clicked, this, &MainWindow::onNavButtonClicked);
            return btn;
        };

        navMainButton      = makeNavButton("Главный",     PageMain);
        navDopingButton    = makeNavButton("Легирование", PageDoping);
        navFinalButton     = makeNavButton("Финальные",   PageFinal);
        navAutotButton     = makeNavButton("Автотяга",    PageAutot);
        navVacuumButton    = makeNavButton("Вакуум",      PageVacuum);
        navGasPanelButton  = makeNavButton("Газ. панель", PageGasPanel);
        navSettingsButton  = makeNavButton("Настройки",   PageSettings);

        buttonsLayout->addWidget(navMainButton);
        buttonsLayout->addWidget(navDopingButton);
        buttonsLayout->addWidget(navFinalButton);
        buttonsLayout->addWidget(navAutotButton);
        buttonsLayout->addWidget(navVacuumButton);
        buttonsLayout->addWidget(navGasPanelButton);
        buttonsLayout->addWidget(navSettingsButton);

        navLayout->addLayout(buttonsLayout, 1);

        // Правая часть — время и дата
        QVBoxLayout *dateTimeLayout = new QVBoxLayout();
        dateTimeLayout->setSpacing(0);

        bottomTimeLabel = new QLabel("00:00");
        bottomTimeLabel->setStyleSheet("QLabel { color: white; font-size: 20px; font-weight: bold; }");
        bottomTimeLabel->setAlignment(Qt::AlignRight);

        bottomDateLabel = new QLabel("01/01/25");
        bottomDateLabel->setStyleSheet("QLabel { color: #bdc3c7; font-size: 12px; }");
        bottomDateLabel->setAlignment(Qt::AlignRight);

        dateTimeLayout->addWidget(bottomTimeLabel);
        dateTimeLayout->addWidget(bottomDateLabel);

        navLayout->addLayout(dateTimeLayout);

        rootLayout->addWidget(navFrame);
    }

    void setActivePage(int pageIndex)
    {
        if (!stackedWidget)
            return;

        stackedWidget->setCurrentIndex(pageIndex);

        // Обновляем состояние кнопок навигации
        QList<QPushButton*> buttons = {
            navMainButton,
            navDopingButton,
            navFinalButton,
            navAutotButton,
            navVacuumButton,
            navGasPanelButton,
            navSettingsButton
        };

        for (QPushButton *btn : buttons) {
            if (!btn) continue;
            int idx = btn->property("pageIndex").toInt();
            btn->setChecked(idx == pageIndex);
        }
    }

private slots:
    void onNavButtonClicked()
    {
        QPushButton *btn = qobject_cast<QPushButton*>(sender());
        if (!btn)
            return;
        int pageIndex = btn->property("pageIndex").toInt();
        setActivePage(pageIndex);
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
