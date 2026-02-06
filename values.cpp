#include "values.h"

#include <QDebug>
#include <QLabel>

namespace Values {

// ============================================================================
// Главная страница (Main Page)
// ============================================================================

// Хранилище указателей на виджеты
static QLabel *xyOffsetXLabel = nullptr;
static QLabel *xyOffsetYLabel = nullptr;

static QLabel *coilOffsetLabel = nullptr;
static QLabel *coilOscillationsLabel = nullptr;

static QLabel *upperSpindleXOffsetLabel = nullptr;
static QLabel *upperSpindleSpeedLabel = nullptr;
static QLabel *upperSpindlePositionLabel = nullptr;

static QLabel *lowerSpindleXOffsetLabel = nullptr;
static QLabel *lowerSpindleSpeedLabel = nullptr;
static QLabel *lowerSpindlePositionLabel = nullptr;

static QLabel *gridAmpLabel = nullptr;
static QLabel *pValueLabel = nullptr;
static QLabel *iValueLabel = nullptr;
static QLabel *uValueLabel = nullptr;

static QLabel *generatorPercentLabel = nullptr;
static QLabel *stopwatchPercentLabel = nullptr;

// ============================================================================
// Страница Финальные (Final Page)
// ============================================================================

static QLabel *finalGrowthRateLabel = nullptr;
static QLabel *finalDiameterLabel = nullptr;
static QLabel *finalLowSpeedLabel = nullptr;
static QLabel *finalPolysiliconDiameterLabel = nullptr;
static QLabel *finalHighSpeedLabel = nullptr;

// ============================================================================
// Страница Легирования (Doping Page)
// ============================================================================

static QLabel *dopingArgonLabel = nullptr;
static QLabel *dopingGasLabel = nullptr;
static QLabel *dopingInjectionLabel = nullptr;
static QLabel *dopingPurgeLabel = nullptr;

static QLabel *flowArgonLabel = nullptr;
static QLabel *flowDopingGasLabel = nullptr;
static QLabel *flowInjectionLabel = nullptr;
static QLabel *flowPurgeLabel = nullptr;

// ============================================================================
// Страница Вакуум (Vacuum Page)
// ============================================================================

// Vacuum Pump Status
static QLabel *pumpPressureStatusLabel = nullptr;

// Vacuum Page Parameters
static QLabel *vacuumPumpPressureLabel = nullptr;
static QLabel *vacuumChamberPressureLabel = nullptr;
static QLabel *vacuumPumpingTimeLabel = nullptr;
static QLabel *vacuumGasPressureLabel = nullptr;
static QLabel *vacuumSwitchLabel = nullptr;
static QLabel *vacuumPumpAlarmLabel = nullptr;

// ============================================================================
// Общие функции (все страницы)
// ============================================================================

// Status Bar
static QLabel *statusBarDot = nullptr;
static QLabel *statusBarText = nullptr;

// Регистрация виджетов
// Главная страница
void registerXYOffsetX(QLabel *label) { xyOffsetXLabel = label; }  // Страница: Главная
void registerXYOffsetY(QLabel *label) { xyOffsetYLabel = label; }  // Страница: Главная

void registerCoilOffset(QLabel *label) { coilOffsetLabel = label; }  // Страница: Главная
void registerCoilOscillations(QLabel *label) { coilOscillationsLabel = label; }  // Страница: Главная

void registerUpperSpindleXOffset(QLabel *label) { upperSpindleXOffsetLabel = label; }  // Страница: Главная
void registerUpperSpindleSpeed(QLabel *label) { upperSpindleSpeedLabel = label; }  // Страница: Главная
void registerUpperSpindlePosition(QLabel *label) { upperSpindlePositionLabel = label; }  // Страница: Главная

void registerLowerSpindleXOffset(QLabel *label) { lowerSpindleXOffsetLabel = label; }  // Страница: Главная
void registerLowerSpindleSpeed(QLabel *label) { lowerSpindleSpeedLabel = label; }  // Страница: Главная
void registerLowerSpindlePosition(QLabel *label) { lowerSpindlePositionLabel = label; }  // Страница: Главная

void registerGridAmp(QLabel *label) { gridAmpLabel = label; }  // Страница: Главная
void registerPValue(QLabel *label) { pValueLabel = label; }  // Страница: Главная
void registerIValue(QLabel *label) { iValueLabel = label; }  // Страница: Главная
void registerUValue(QLabel *label) { uValueLabel = label; }  // Страница: Главная

void registerGeneratorPercent(QLabel *label) { generatorPercentLabel = label; }  // Страница: Главная
void registerStopwatchPercent(QLabel *label) { stopwatchPercentLabel = label; }  // Страница: Главная

// Страница Легирования
void registerDopingArgon(QLabel *label) { dopingArgonLabel = label; }  // Страница: Легирование
void registerDopingGas(QLabel *label) { dopingGasLabel = label; }  // Страница: Легирование
void registerDopingInjection(QLabel *label) { dopingInjectionLabel = label; }  // Страница: Легирование
void registerDopingPurge(QLabel *label) { dopingPurgeLabel = label; }  // Страница: Легирование

void registerFlowArgon(QLabel *label) { flowArgonLabel = label; }  // Страница: Легирование
void registerFlowDopingGas(QLabel *label) { flowDopingGasLabel = label; }  // Страница: Легирование
void registerFlowInjection(QLabel *label) { flowInjectionLabel = label; }  // Страница: Легирование
void registerFlowPurge(QLabel *label) { flowPurgeLabel = label; }  // Страница: Легирование

// Страница Финальные
void registerFinalGrowthRate(QLabel *label) { finalGrowthRateLabel = label; }  // Страница: Финальные
void registerFinalDiameter(QLabel *label) { finalDiameterLabel = label; }  // Страница: Финальные
void registerFinalLowSpeed(QLabel *label) { finalLowSpeedLabel = label; }  // Страница: Финальные
void registerFinalPolysiliconDiameter(QLabel *label) { finalPolysiliconDiameterLabel = label; }  // Страница: Финальные
void registerFinalHighSpeed(QLabel *label) { finalHighSpeedLabel = label; }  // Страница: Финальные

// Страница Вакуум
void registerVacuumPumpPressure(QLabel *label) { vacuumPumpPressureLabel = label; }  // Страница: Вакуум
void registerVacuumChamberPressure(QLabel *label) { vacuumChamberPressureLabel = label; }  // Страница: Вакуум
void registerVacuumPumpingTime(QLabel *label) { vacuumPumpingTimeLabel = label; }  // Страница: Вакуум
void registerVacuumGasPressure(QLabel *label) { vacuumGasPressureLabel = label; }  // Страница: Вакуум
void registerVacuumSwitch(QLabel *label) { vacuumSwitchLabel = label; }  // Страница: Вакуум
void registerVacuumPumpAlarm(QLabel *label) { vacuumPumpAlarmLabel = label; }  // Страница: Вакуум

void registerPumpPressureStatus(QLabel *statusLabel) { pumpPressureStatusLabel = statusLabel; }  // Страница: Вакуум

// Общие функции
void registerStatusBar(QLabel *statusDot, QLabel *statusText)  // Страница: Все
{
    statusBarDot = statusDot;
    statusBarText = statusText;
}

// Вспомогательная функция для форматирования значения
static QString formatValue(double value, const QString &unit = "")
{
    QString formatted;
    if (unit.isEmpty()) {
        formatted = QString::number(value, 'f', 1);
    } else {
        formatted = QString::number(value, 'f', 1) + " " + unit;
    }
    return formatted;
}

// Вспомогательная функция для форматирования значения с двумя знаками после запятой
static QString formatValue2(double value, const QString &unit = "")
{
    QString formatted;
    if (unit.isEmpty()) {
        formatted = QString::number(value, 'f', 2);
    } else {
        formatted = QString::number(value, 'f', 2) + " " + unit;
    }
    return formatted;
}

// Функции обновления значений
// Главная страница
void updateXYOffsetX(double value)  // Страница: Главная
{
    if (xyOffsetXLabel) {
        xyOffsetXLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: XY Offset X =" << value;
    }
}

void updateXYOffsetY(double value)  // Страница: Главная
{
    if (xyOffsetYLabel) {
        xyOffsetYLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: XY Offset Y =" << value;
    }
}

void updateCoilOffset(double value)  // Страница: Главная
{
    if (coilOffsetLabel) {
        coilOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Coil Offset =" << value;
    }
}

void updateCoilOscillations(double value)  // Страница: Главная
{
    if (coilOscillationsLabel) {
        coilOscillationsLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Coil Oscillations =" << value;
    }
}

void updateUpperSpindleXOffset(double value)  // Страница: Главная
{
    if (upperSpindleXOffsetLabel) {
        upperSpindleXOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Upper Spindle X Offset =" << value;
    }
}

void updateUpperSpindleSpeed(double value)  // Страница: Главная
{
    if (upperSpindleSpeedLabel) {
        upperSpindleSpeedLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Upper Spindle Speed =" << value;
    }
}

void updateUpperSpindlePosition(double value)  // Страница: Главная
{
    if (upperSpindlePositionLabel) {
        upperSpindlePositionLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Upper Spindle Position =" << value;
    }
}

void updateLowerSpindleXOffset(double value)  // Страница: Главная
{
    if (lowerSpindleXOffsetLabel) {
        lowerSpindleXOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Lower Spindle X Offset =" << value;
    }
}

void updateLowerSpindleSpeed(double value)  // Страница: Главная
{
    if (lowerSpindleSpeedLabel) {
        lowerSpindleSpeedLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Lower Spindle Speed =" << value;
    }
}

void updateLowerSpindlePosition(double value)  // Страница: Главная
{
    if (lowerSpindlePositionLabel) {
        lowerSpindlePositionLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Lower Spindle Position =" << value;
    }
}

void updateGridAmp(double value)  // Страница: Главная
{
    if (gridAmpLabel) {
        gridAmpLabel->setText(formatValue2(value, "AMP"));
        qDebug() << "Values: Grid AMP =" << value;
    }
}

void updatePValue(double value)  // Страница: Главная
{
    if (pValueLabel) {
        pValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: P Value =" << value;
    }
}

void updateIValue(double value)  // Страница: Главная
{
    if (iValueLabel) {
        iValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: I Value =" << value;
    }
}

void updateUValue(double value)  // Страница: Главная
{
    if (uValueLabel) {
        uValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: U Value =" << value;
    }
}

void updateGeneratorPercent(double value)  // Страница: Главная
{
    if (generatorPercentLabel) {
        generatorPercentLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Generator Percent =" << value;
    }
}

void updateStopwatchPercent(double value)  // Страница: Главная
{
    if (stopwatchPercentLabel) {
        stopwatchPercentLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Stopwatch Percent =" << value;
    }
}

// Страница Легирования
void updateDopingArgon(double value, const QString &unit)  // Страница: Легирование
{
    if (dopingArgonLabel) {
        dopingArgonLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Argon =" << value << unit;
    }
}

void updateDopingGas(double value, const QString &unit)  // Страница: Легирование
{
    if (dopingGasLabel) {
        dopingGasLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Gas =" << value << unit;
    }
}

void updateDopingInjection(double value, const QString &unit)  // Страница: Легирование
{
    if (dopingInjectionLabel) {
        dopingInjectionLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Injection =" << value << unit;
    }
}

void updateDopingPurge(double value, const QString &unit)  // Страница: Легирование
{
    if (dopingPurgeLabel) {
        dopingPurgeLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Purge =" << value << unit;
    }
}

void updateFlowArgon(double value, const QString &unit)  // Страница: Легирование
{
    if (flowArgonLabel) {
        flowArgonLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Argon =" << value << unit;
    }
}

void updateFlowDopingGas(double value, const QString &unit)  // Страница: Легирование
{
    if (flowDopingGasLabel) {
        flowDopingGasLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Doping Gas =" << value << unit;
    }
}

void updateFlowInjection(double value, const QString &unit)  // Страница: Легирование
{
    if (flowInjectionLabel) {
        flowInjectionLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Injection =" << value << unit;
    }
}

void updateFlowPurge(double value, const QString &unit)  // Страница: Легирование
{
    if (flowPurgeLabel) {
        flowPurgeLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Purge =" << value << unit;
    }
}

// Страница Финальные
void updateFinalGrowthRate(double value)  // Страница: Финальные
{
    if (finalGrowthRateLabel) {
        finalGrowthRateLabel->setText(formatValue(value));
        qDebug() << "Values: Final Growth Rate =" << value;
    }
}

void updateFinalDiameter(double value)  // Страница: Финальные
{
    if (finalDiameterLabel) {
        finalDiameterLabel->setText(formatValue(value));
        qDebug() << "Values: Final Diameter =" << value;
    }
}

void updateFinalLowSpeed(double value)  // Страница: Финальные
{
    if (finalLowSpeedLabel) {
        finalLowSpeedLabel->setText(formatValue(value));
        qDebug() << "Values: Final Low Speed =" << value;
    }
}

void updateFinalPolysiliconDiameter(double value)  // Страница: Финальные
{
    if (finalPolysiliconDiameterLabel) {
        finalPolysiliconDiameterLabel->setText(formatValue(value));
        qDebug() << "Values: Final Polysilicon Diameter =" << value;
    }
}

void updateFinalHighSpeed(double value)  // Страница: Финальные
{
    if (finalHighSpeedLabel) {
        finalHighSpeedLabel->setText(formatValue(value));
        qDebug() << "Values: Final High Speed =" << value;
    }
}

// Страница Вакуум
void updateVacuumPumpPressure(double value)  // Страница: Вакуум
{
    if (vacuumPumpPressureLabel) {
        vacuumPumpPressureLabel->setText(formatValue2(value, "мбар"));
        qDebug() << "Values: Vacuum Pump Pressure =" << value << "мбар";
    }
}

void updateVacuumChamberPressure(double value)  // Страница: Вакуум
{
    if (vacuumChamberPressureLabel) {
        vacuumChamberPressureLabel->setText(formatValue2(value, "мбар"));
        qDebug() << "Values: Vacuum Chamber Pressure =" << value << "мбар";
    }
}

void updateVacuumPumpingTime(double value)  // Страница: Вакуум
{
    if (vacuumPumpingTimeLabel) {
        vacuumPumpingTimeLabel->setText(formatValue2(value, "сек"));
        qDebug() << "Values: Vacuum Pumping Time =" << value << "сек";
    }
}

void updateVacuumGasPressure(double value)  // Страница: Вакуум
{
    if (vacuumGasPressureLabel) {
        vacuumGasPressureLabel->setText(formatValue2(value, "бар"));
        qDebug() << "Values: Vacuum Gas Pressure =" << value << "бар";
    }
}

void updateVacuumSwitch(double value)  // Страница: Вакуум
{
    if (vacuumSwitchLabel) {
        vacuumSwitchLabel->setText(formatValue2(value, "мбар"));
        qDebug() << "Values: Vacuum Switch =" << value << "мбар";
    }
}

void updateVacuumPumpAlarm(double value)  // Страница: Вакуум
{
    if (vacuumPumpAlarmLabel) {
        vacuumPumpAlarmLabel->setText(formatValue2(value, "сек"));
        qDebug() << "Values: Vacuum Pump Alarm =" << value << "сек";
    }
}

void updatePumpPressureStatus(PumpPressureStatus status)  // Страница: Вакуум
{
    QString text;
    QString color;
    
    switch (status) {
        case PumpPressureStatus::Low:
            text = QString::fromUtf8("Низкое");
            color = "#3498db"; // синий
            break;
        case PumpPressureStatus::LowMedium:
            text = QString::fromUtf8("Низкое-среднее");
            color = "#1abc9c"; // бирюзовый
            break;
        case PumpPressureStatus::Medium:
            text = QString::fromUtf8("Среднее");
            color = "#f1c40f"; // желтый
            break;
        case PumpPressureStatus::MediumHigh:
            text = QString::fromUtf8("Среднее-высокое");
            color = "#e67e22"; // оранжевый
            break;
        case PumpPressureStatus::High:
            text = QString::fromUtf8("Высокое");
            color = "#e74c3c"; // красный
            break;
    }
    
    if (pumpPressureStatusLabel) {
        pumpPressureStatusLabel->setText(text);
        pumpPressureStatusLabel->setStyleSheet(
            QString("QLabel {"
                    "  font-size: 13px;"
                    "  font-weight: bold;"
                    "  color: %1;"
                    "  background: transparent;"
                    "  padding: 0px;"
                    "}").arg(color)
        );
    }
    
    qDebug() << "Values: Pump Pressure Status =" << static_cast<int>(status) << text;
}

// Общие функции
void updateConnectionStatus(ConnectionStatus status)  // Страница: Все
{
    QString text;
    QString color;
    
    switch (status) {
        case ConnectionStatus::Connecting:
            text = QString::fromUtf8("Идет подключение");
            color = "#f1c40f"; // желтый
            break;
        case ConnectionStatus::Disconnected:
            text = QString::fromUtf8("Нет связи с контроллером");
            color = "#e74c3c"; // красный
            break;
        case ConnectionStatus::Connected:
            text = QString::fromUtf8("Контроллер подключен");
            color = "#27ae60"; // зеленый
            break;
    }
    
    if (statusBarDot) {
        statusBarDot->setStyleSheet(
            QString("QLabel {"
                    "  color: %1;"
                    "  font-size: 18px;"
                    "  background: transparent;"
                    "  padding: 0px;"
                    "}").arg(color)
        );
    }
    
    if (statusBarText) {
        statusBarText->setText(text);
        statusBarText->setStyleSheet(
            QString("QLabel {"
                    "  font-size: 15px;"
                    "  color: %1;"
                    "  background: transparent;"
                    "  padding: 0px;"
                    "}").arg(color)
        );
    }
    
    qDebug() << "Values: Connection Status =" << static_cast<int>(status) << text;
}
} // namespace Values
