#include "values.h"
#include "widgets/selector.hpp"
#include "widgets/doorselector.hpp"
#include "widgets/textbuttonwidget.h"
#include "widgets/textbuttonwidget.h"
#include "widgets/iconbuttonwidget.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>

namespace Values {

// ============================================================================
// Главная страница (Main Page)
// ============================================================================

// Хранилище указателей на виджеты
static QLabel *xyOffsetXLabel = nullptr;
static QLabel *xyOffsetYLabel = nullptr;

// Settings Page: XY
static QLabel *settingsXYOffsetXLabel = nullptr;
static QLabel *settingsXYOffsetYLabel = nullptr;

static QLabel *coilOffsetLabel = nullptr;
static QLabel *coilOscillationsLabel = nullptr;

// Settings Page: Coil
static QLabel *settingsCoilOffsetLabel = nullptr;
static QLabel *settingsCoilOscillationsLabel = nullptr;

static QLabel *upperSpindleXOffsetLabel = nullptr;
static QLabel *upperSpindleSpeedLabel = nullptr;
static QLabel *upperSpindlePositionLabel = nullptr;

// Settings Page: Upper Spindle
static QLabel *settingsUpperSpindleXOffsetLabel = nullptr;
static QLabel *settingsUpperSpindleSpeedLabel = nullptr;
static QLabel *settingsUpperSpindlePositionLabel = nullptr;
static QLabel *settingsUpperSpindleRpmLabel = nullptr;
static QLabel *settingsUpperSpindleAlarmLabel = nullptr;

static QLabel *lowerSpindleXOffsetLabel = nullptr;
static QLabel *lowerSpindleSpeedLabel = nullptr;
static QLabel *lowerSpindlePositionLabel = nullptr;

// Settings Page: Lower Spindle
static QLabel *settingsLowerSpindleXOffsetLabel = nullptr;
static QLabel *settingsLowerSpindleSpeedLabel = nullptr;
static QLabel *settingsLowerSpindlePositionLabel = nullptr;
static QLabel *settingsLowerSpindleAlarmLabel = nullptr;

static QLabel *gridAmpLabel = nullptr;
static QLabel *pValueLabel = nullptr;
static QLabel *iValueLabel = nullptr;
static QLabel *uValueLabel = nullptr;

// Settings Page: Heating / Grid / PID
static QLabel *settingsGridAmpLabel = nullptr;
static QLabel *settingsPValueLabel = nullptr;
static QLabel *settingsIValueLabel = nullptr;
static QLabel *settingsUValueLabel = nullptr;

static QLabel *generatorPercentLabel = nullptr;
static QLabel *stopwatchPercentLabel = nullptr;

// Settings Page: Generator
static QLabel *settingsGeneratorPercentLabel = nullptr;
static QLabel *settingsGeneratorStatusLabel = nullptr;

// Settings Page: Lighting (4 buttons), Lower Oscillation, Alarm Settings
static QWidget *settingsLightingButtons[4] = {nullptr, nullptr, nullptr, nullptr};
static bool settingsLightingButtonStates[4] = {true, false, false, true};  // top, left, right, bottom
static QLabel *settingsLowerOscillationClockwiseLabel = nullptr;
static QLabel *settingsLowerOscillationCounterClockwiseLabel = nullptr;
static QLabel *settingsLowerOscillationAccelerationLabel = nullptr;
static QLabel *settingsAlarmModeLabel = nullptr;
static QLabel *settingsAlarmDurationLabel = nullptr;

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
// Газовая панель (Gas Panel Page)
// ============================================================================

static QLabel *gasPanelArgonSetpointLabel = nullptr;
static QLabel *gasPanelArgonFlowLabel = nullptr;
static QLabel *gasPanelExhaustSetpointLabel = nullptr;
static QLabel *gasPanelExhaustFlowLabel = nullptr;
static QLabel *gasPanelNitrogenSetpointLabel = nullptr;
static QLabel *gasPanelNitrogenValueLabel = nullptr;
static QLabel *gasPanelNitrogenConcentrationLabel = nullptr;
static QLabel *gasPanelGasPressureSetpointLabel = nullptr;
static QLabel *gasPanelGasPressureLabel = nullptr;

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

// Страница Настройки
void registerSettingsXYOffsetX(QLabel *label) { settingsXYOffsetXLabel = label; }  // Страница: Настройки
void registerSettingsXYOffsetY(QLabel *label) { settingsXYOffsetYLabel = label; }  // Страница: Настройки

void registerCoilOffset(QLabel *label) { coilOffsetLabel = label; }  // Страница: Главная
void registerCoilOscillations(QLabel *label) { coilOscillationsLabel = label; }  // Страница: Главная

void registerSettingsCoilOffset(QLabel *label) { settingsCoilOffsetLabel = label; }  // Страница: Настройки
void registerSettingsCoilOscillations(QLabel *label) { settingsCoilOscillationsLabel = label; }  // Страница: Настройки

void registerUpperSpindleXOffset(QLabel *label) { upperSpindleXOffsetLabel = label; }  // Страница: Главная
void registerUpperSpindleSpeed(QLabel *label) { upperSpindleSpeedLabel = label; }  // Страница: Главная
void registerUpperSpindlePosition(QLabel *label) { upperSpindlePositionLabel = label; }  // Страница: Главная

void registerSettingsUpperSpindleXOffset(QLabel *label) { settingsUpperSpindleXOffsetLabel = label; }  // Страница: Настройки
void registerSettingsUpperSpindleSpeed(QLabel *label) { settingsUpperSpindleSpeedLabel = label; }  // Страница: Настройки
void registerSettingsUpperSpindlePosition(QLabel *label) { settingsUpperSpindlePositionLabel = label; }  // Страница: Настройки
void registerSettingsUpperSpindleRpm(QLabel *label) { settingsUpperSpindleRpmLabel = label; }  // Страница: Настройки
void registerSettingsUpperSpindleAlarm(QLabel *label) { settingsUpperSpindleAlarmLabel = label; }  // Страница: Настройки

void registerLowerSpindleXOffset(QLabel *label) { lowerSpindleXOffsetLabel = label; }  // Страница: Главная
void registerLowerSpindleSpeed(QLabel *label) { lowerSpindleSpeedLabel = label; }  // Страница: Главная
void registerLowerSpindlePosition(QLabel *label) { lowerSpindlePositionLabel = label; }  // Страница: Главная

void registerSettingsLowerSpindleXOffset(QLabel *label) { settingsLowerSpindleXOffsetLabel = label; }  // Страница: Настройки
void registerSettingsLowerSpindleSpeed(QLabel *label) { settingsLowerSpindleSpeedLabel = label; }  // Страница: Настройки
void registerSettingsLowerSpindlePosition(QLabel *label) { settingsLowerSpindlePositionLabel = label; }  // Страница: Настройки
void registerSettingsLowerSpindleAlarm(QLabel *label) { settingsLowerSpindleAlarmLabel = label; }  // Страница: Настройки

void registerGridAmp(QLabel *label) { gridAmpLabel = label; }  // Страница: Главная
void registerPValue(QLabel *label) { pValueLabel = label; }  // Страница: Главная
void registerIValue(QLabel *label) { iValueLabel = label; }  // Страница: Главная
void registerUValue(QLabel *label) { uValueLabel = label; }  // Страница: Главная

void registerSettingsGridAmp(QLabel *label) { settingsGridAmpLabel = label; }  // Страница: Настройки
void registerSettingsPValue(QLabel *label) { settingsPValueLabel = label; }  // Страница: Настройки
void registerSettingsIValue(QLabel *label) { settingsIValueLabel = label; }  // Страница: Настройки
void registerSettingsUValue(QLabel *label) { settingsUValueLabel = label; }  // Страница: Настройки

void registerGeneratorPercent(QLabel *label) { generatorPercentLabel = label; }  // Страница: Главная
void registerStopwatchPercent(QLabel *label) { stopwatchPercentLabel = label; }  // Страница: Главная

void registerSettingsGeneratorPercent(QLabel *label) { settingsGeneratorPercentLabel = label; }  // Страница: Настройки
void registerSettingsGeneratorStatus(QLabel *label) { settingsGeneratorStatusLabel = label; }

void registerSettingsLightingButton(int index, QWidget *widget)  // Страница: Настройки
{
    if (index >= 0 && index < 4) {
        settingsLightingButtons[index] = widget;
    }
}

void registerSettingsLowerOscillationClockwise(QLabel *label) { settingsLowerOscillationClockwiseLabel = label; }
void registerSettingsLowerOscillationCounterClockwise(QLabel *label) { settingsLowerOscillationCounterClockwiseLabel = label; }
void registerSettingsLowerOscillationAcceleration(QLabel *label) { settingsLowerOscillationAccelerationLabel = label; }
void registerSettingsAlarmMode(QLabel *label) { settingsAlarmModeLabel = label; }
void registerSettingsAlarmDuration(QLabel *label) { settingsAlarmDurationLabel = label; }

// Страница Легирования
void registerDopingArgon(QLabel *label) { dopingArgonLabel = label; }  // Страница: Легирование
void registerDopingGas(QLabel *label) { dopingGasLabel = label; }  // Страница: Легирование
void registerDopingInjection(QLabel *label) { dopingInjectionLabel = label; }  // Страница: Легирование
void registerDopingPurge(QLabel *label) { dopingPurgeLabel = label; }  // Страница: Легирование

void registerFlowArgon(QLabel *label) { flowArgonLabel = label; }  // Страница: Легирование
void registerFlowDopingGas(QLabel *label) { flowDopingGasLabel = label; }  // Страница: Легирование
void registerFlowInjection(QLabel *label) { flowInjectionLabel = label; }  // Страница: Легирование
void registerFlowPurge(QLabel *label) { flowPurgeLabel = label; }  // Страница: Легирование

// Газовая панель
void registerGasPanelArgonSetpoint(QLabel *label) { gasPanelArgonSetpointLabel = label; }  // Страница: Газовая панель
void registerGasPanelArgonFlow(QLabel *label) { gasPanelArgonFlowLabel = label; }          // Страница: Газовая панель
void registerGasPanelExhaustSetpoint(QLabel *label) { gasPanelExhaustSetpointLabel = label; }  // Страница: Газовая панель
void registerGasPanelExhaustFlow(QLabel *label) { gasPanelExhaustFlowLabel = label; }          // Страница: Газовая панель
void registerGasPanelNitrogenSetpoint(QLabel *label) { gasPanelNitrogenSetpointLabel = label; }  // Страница: Газовая панель
void registerGasPanelNitrogenValue(QLabel *label) { gasPanelNitrogenValueLabel = label; }        // Страница: Газовая панель
void registerGasPanelNitrogenConcentration(QLabel *label) { gasPanelNitrogenConcentrationLabel = label; }  // Страница: Газовая панель
void registerGasPanelGasPressureSetpoint(QLabel *label) { gasPanelGasPressureSetpointLabel = label; }  // Страница: Газовая панель
void registerGasPanelGasPressure(QLabel *label) { gasPanelGasPressureLabel = label; }                  // Страница: Газовая панель

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

void updateSettingsXYOffsetX(double value)  // Страница: Настройки
{
    if (settingsXYOffsetXLabel) {
        settingsXYOffsetXLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings XY Offset X =" << value;
    }
}

void updateSettingsXYOffsetY(double value)  // Страница: Настройки
{
    if (settingsXYOffsetYLabel) {
        settingsXYOffsetYLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings XY Offset Y =" << value;
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

void updateSettingsCoilOffset(double value)  // Страница: Настройки
{
    if (settingsCoilOffsetLabel) {
        settingsCoilOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Coil Offset =" << value;
    }
}

void updateSettingsCoilOscillations(double value)  // Страница: Настройки
{
    if (settingsCoilOscillationsLabel) {
        settingsCoilOscillationsLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Settings Coil Oscillations =" << value;
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

void updateSettingsUpperSpindleXOffset(double value)  // Страница: Настройки
{
    if (settingsUpperSpindleXOffsetLabel) {
        settingsUpperSpindleXOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Upper Spindle X Offset =" << value;
    }
}

void updateSettingsUpperSpindleSpeed(double value)  // Страница: Настройки
{
    if (settingsUpperSpindleSpeedLabel) {
        settingsUpperSpindleSpeedLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Settings Upper Spindle Speed =" << value;
    }
}

void updateSettingsUpperSpindlePosition(double value)  // Страница: Настройки
{
    if (settingsUpperSpindlePositionLabel) {
        settingsUpperSpindlePositionLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Upper Spindle Position =" << value;
    }
}

void updateSettingsUpperSpindleRpm(double value)  // Страница: Настройки
{
    if (settingsUpperSpindleRpmLabel) {
        settingsUpperSpindleRpmLabel->setText(formatValue(value, "RPM"));
        qDebug() << "Values: Settings Upper Spindle RPM =" << value;
    }
}

void updateSettingsUpperSpindleAlarm(double value)  // Страница: Настройки
{
    if (settingsUpperSpindleAlarmLabel) {
        settingsUpperSpindleAlarmLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Upper Spindle Alarm =" << value;
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

void updateSettingsLowerSpindleXOffset(double value)  // Страница: Настройки
{
    if (settingsLowerSpindleXOffsetLabel) {
        settingsLowerSpindleXOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Lower Spindle X Offset =" << value;
    }
}

void updateSettingsLowerSpindleSpeed(double value)  // Страница: Настройки
{
    if (settingsLowerSpindleSpeedLabel) {
        settingsLowerSpindleSpeedLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Settings Lower Spindle Speed =" << value;
    }
}

void updateSettingsLowerSpindlePosition(double value)  // Страница: Настройки
{
    if (settingsLowerSpindlePositionLabel) {
        settingsLowerSpindlePositionLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Lower Spindle Position =" << value;
    }
}

void updateSettingsLowerSpindleAlarm(double value)  // Страница: Настройки
{
    if (settingsLowerSpindleAlarmLabel) {
        settingsLowerSpindleAlarmLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Settings Lower Spindle Alarm =" << value;
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

void updateSettingsGridAmp(double value)  // Страница: Настройки
{
    if (settingsGridAmpLabel) {
        settingsGridAmpLabel->setText(formatValue2(value, "AMP"));
        qDebug() << "Values: Settings Grid AMP =" << value;
    }
}

void updateSettingsPValue(double value)  // Страница: Настройки
{
    if (settingsPValueLabel) {
        settingsPValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Settings P Value =" << value;
    }
}

void updateSettingsIValue(double value)  // Страница: Настройки
{
    if (settingsIValueLabel) {
        settingsIValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Settings I Value =" << value;
    }
}

void updateSettingsUValue(double value)  // Страница: Настройки
{
    if (settingsUValueLabel) {
        settingsUValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Settings U Value =" << value;
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

void updateSettingsGeneratorPercent(double value)  // Страница: Настройки
{
    if (settingsGeneratorPercentLabel) {
        settingsGeneratorPercentLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Settings Generator Percent =" << value;
    }
}

void updateSettingsGeneratorStatus(bool isOk)  // Страница: Настройки, true = Ок, false = Ошибка
{
    if (settingsGeneratorStatusLabel) {
        if (isOk) {
            settingsGeneratorStatusLabel->setText(QString::fromUtf8("Ок"));
            settingsGeneratorStatusLabel->setStyleSheet("QLabel { color: #27ae60; font-size: 18px; font-weight: bold; }");
        } else {
            settingsGeneratorStatusLabel->setText(QString::fromUtf8("Ошибка"));
            settingsGeneratorStatusLabel->setStyleSheet("QLabel { color: #e74c3c; font-size: 18px; font-weight: bold; }");
        }
    }
}

void updateSettingsLightingButton(int index, bool isOn)  // Страница: Настройки
{
    if (index >= 0 && index < 4 && settingsLightingButtons[index]) {
        IconButtonWidget *btn = qobject_cast<IconButtonWidget*>(settingsLightingButtons[index]);
        if (btn) {
            settingsLightingButtonStates[index] = isOn;
            btn->setBackgroundColor(isOn ? "#f1c40f" : "#bdc3c7");
        }
    }
}

bool getSettingsLightingButtonState(int index)
{
    if (index >= 0 && index < 4) {
        return settingsLightingButtonStates[index];
    }
    return false;
}

void updateSettingsLowerOscillationClockwise(double value)
{
    if (settingsLowerOscillationClockwiseLabel) {
        settingsLowerOscillationClockwiseLabel->setText(QString::number(static_cast<int>(value)) + " °");
    }
}

void updateSettingsLowerOscillationCounterClockwise(double value)
{
    if (settingsLowerOscillationCounterClockwiseLabel) {
        settingsLowerOscillationCounterClockwiseLabel->setText(QString::number(static_cast<int>(value)) + " °");
    }
}

void updateSettingsLowerOscillationAcceleration(double value)
{
    if (settingsLowerOscillationAccelerationLabel) {
        settingsLowerOscillationAccelerationLabel->setText(
            QString::number(value, 'f', 1) + " °/сек²");
    }
}

void updateSettingsAlarmMode(double value)
{
    if (settingsAlarmModeLabel) {
        settingsAlarmModeLabel->setText(formatValue(value, "сек"));
    }
}

void updateSettingsAlarmDuration(double value)
{
    if (settingsAlarmDurationLabel) {
        settingsAlarmDurationLabel->setText(formatValue(value, "сек"));
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

// Газовая панель
void updateGasPanelArgonSetpoint(double value)  // Страница: Газовая панель
{
    if (gasPanelArgonSetpointLabel) {
        gasPanelArgonSetpointLabel->setText(formatValue(value));
        qDebug() << "Values: GasPanel Argon Setpoint =" << value;
    }
}

void updateGasPanelArgonFlow(double value)  // Страница: Газовая панель
{
    if (gasPanelArgonFlowLabel) {
        gasPanelArgonFlowLabel->setText(formatValue(value, QString::fromUtf8("л/мин")));
        qDebug() << "Values: GasPanel Argon Flow =" << value;
    }
}

void updateGasPanelExhaustSetpoint(double value)  // Страница: Газовая панель
{
    if (gasPanelExhaustSetpointLabel) {
        gasPanelExhaustSetpointLabel->setText(formatValue(value));
        qDebug() << "Values: GasPanel Exhaust Setpoint =" << value;
    }
}

void updateGasPanelExhaustFlow(double value)  // Страница: Газовая панель
{
    if (gasPanelExhaustFlowLabel) {
        gasPanelExhaustFlowLabel->setText(formatValue(value, QString::fromUtf8("л/мин")));
        qDebug() << "Values: GasPanel Exhaust Flow =" << value;
    }
}

void updateGasPanelNitrogenSetpoint(double value)  // Страница: Газовая панель
{
    if (gasPanelNitrogenSetpointLabel) {
        gasPanelNitrogenSetpointLabel->setText(formatValue(value));
        qDebug() << "Values: GasPanel Nitrogen Setpoint =" << value;
    }
}

void updateGasPanelNitrogenValue(double value)  // Страница: Газовая панель
{
    if (gasPanelNitrogenValueLabel) {
        gasPanelNitrogenValueLabel->setText(formatValue(value));
        qDebug() << "Values: GasPanel Nitrogen Value =" << value;
    }
}

void updateGasPanelNitrogenConcentration(double value)  // Страница: Газовая панель
{
    if (gasPanelNitrogenConcentrationLabel) {
        gasPanelNitrogenConcentrationLabel->setText(formatValue(value));
        qDebug() << "Values: GasPanel Nitrogen Concentration =" << value;
    }
}

void updateGasPanelGasPressureSetpoint(double value)  // Страница: Газовая панель
{
    if (gasPanelGasPressureSetpointLabel) {
        gasPanelGasPressureSetpointLabel->setText(formatValue(value));
        qDebug() << "Values: GasPanel Gas Pressure Setpoint =" << value;
    }
}

void updateGasPanelGasPressure(double value)  // Страница: Газовая панель
{
    if (gasPanelGasPressureLabel) {
        gasPanelGasPressureLabel->setText(formatValue2(value, QString::fromUtf8("бар")));
        qDebug() << "Values: GasPanel Gas Pressure =" << value;
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

// ============================================================================
// Страница Вакуум (Vacuum Page) - Controls
// ============================================================================

// Main Page Selectors
static selector *heatingSelector = nullptr;
static selector *generatorSelector = nullptr;
static bool heatingSelectorState = false;
static bool generatorSelectorState = false;

// Settings Page Selectors
static selector *settingsHeatingSelector = nullptr;
static selector *settingsGeneratorSelector = nullptr;
static bool settingsHeatingSelectorState = false;
static bool settingsGeneratorSelectorState = false;

// Main Page Start/Stop Buttons (СТАРТ/СТОП)
static QWidget *upperSpindleStartStopButton = nullptr;  // TextButtonWidget (Upper Spindle)
static QWidget *lowerSpindleStartStopButton = nullptr;  // TextButtonWidget (Lower Spindle)
static bool upperSpindleStartStopState = true;  // true = СТАРТ, false = СТОП
static bool lowerSpindleStartStopState = true;  // true = СТАРТ, false = СТОП

// Settings Page Start/Stop Buttons (СТАРТ/СТОП)
static QWidget *settingsUpperSpindleStartStopButton = nullptr;  // TextButtonWidget (Upper Spindle, Settings)
static QWidget *settingsLowerSpindleStartStopButton = nullptr;  // TextButtonWidget (Lower Spindle, Settings)
static bool settingsUpperSpindleStartStopState = true;  // true = СТАРТ, false = СТОП
static bool settingsLowerSpindleStartStopState = true;  // true = СТАРТ, false = СТОП

// Vacuum Page Selectors
static selector *vacuumPumpSelector = nullptr;
static selector *vacuumValveSelector = nullptr;
static selector *autoPumpDownSelector = nullptr;
static doorselector *upperDoorSelector = nullptr;
static doorselector *lowerDoorSelector = nullptr;
static QWidget *mainDoorStatusWidget = nullptr;  // TextButtonWidget
static QWidget *lightingButtons[4] = {nullptr, nullptr, nullptr, nullptr};  // IconButtonWidget
static bool lightingButtonStates[4] = {false, false, false, false};  // Состояния кнопок освещения

// Состояния селекторов и doorselector для синхронизации
static bool vacuumPumpSelectorState = false;
static bool vacuumValveSelectorState = false;
static bool autoPumpDownSelectorState = false;
static bool upperDoorSelectorState = false;  // true = Откр, false = Закр
static bool lowerDoorSelectorState = false;  // true = Откр, false = Закр
static bool mainDoorStatusState = false;  // true = открыта, false = закрыта

// Main Page Selectors
void registerHeatingSelector(selector *widget)  // Страница: Главная
{
    heatingSelector = widget;
}

void registerGeneratorSelector(selector *widget)  // Страница: Главная
{
    generatorSelector = widget;
}

// Settings Page Selectors
void registerSettingsHeatingSelector(selector *widget)  // Страница: Настройки
{
    settingsHeatingSelector = widget;
}

void registerSettingsGeneratorSelector(selector *widget)  // Страница: Настройки
{
    settingsGeneratorSelector = widget;
}

void updateHeatingSelector(bool state)  // Страница: Главная
{
    if (heatingSelector) {
        if (heatingSelectorState != state) {
            heatingSelectorState = state;  // Сохраняем состояние
            heatingSelector->set(state, true);  // animated=true
            qDebug() << "Values: Heating Selector =" << (state ? "On" : "Off");
        }
    }
}

void updateGeneratorSelector(bool state)  // Страница: Главная
{
    if (generatorSelector) {
        if (generatorSelectorState != state) {
            generatorSelectorState = state;  // Сохраняем состояние
            generatorSelector->set(state, true);  // animated=true
            qDebug() << "Values: Generator Selector =" << (state ? "On" : "Off");
        }
    }
}

void updateSettingsHeatingSelector(bool state)  // Страница: Настройки
{
    if (settingsHeatingSelector) {
        if (settingsHeatingSelectorState != state) {
            settingsHeatingSelectorState = state;
            settingsHeatingSelector->set(state, true);
            qDebug() << "Values: Settings Heating Selector =" << (state ? "On" : "Off");
        }
    }
}

void updateSettingsGeneratorSelector(bool state)  // Страница: Настройки
{
    if (settingsGeneratorSelector) {
        if (settingsGeneratorSelectorState != state) {
            settingsGeneratorSelectorState = state;
            settingsGeneratorSelector->set(state, true);
            qDebug() << "Values: Settings Generator Selector =" << (state ? "On" : "Off");
        }
    }
}

bool getHeatingSelectorState()  // Страница: Главная
{
    return heatingSelectorState;
}

bool getGeneratorSelectorState()  // Страница: Главная
{
    return generatorSelectorState;
}

bool getSettingsHeatingSelectorState()  // Страница: Настройки
{
    return settingsHeatingSelectorState;
}

bool getSettingsGeneratorSelectorState()  // Страница: Настройки
{
    return settingsGeneratorSelectorState;
}

// Регистрация и обновление кнопок СТАРТ/СТОП (Главная страница)

void registerUpperSpindleStartStopButton(QWidget *widget)  // Страница: Главная
{
    upperSpindleStartStopButton = widget;
}

void registerLowerSpindleStartStopButton(QWidget *widget)  // Страница: Главная
{
    lowerSpindleStartStopButton = widget;
}

// Регистрация и обновление кнопок СТАРТ/СТОП (Страница настроек)
void registerSettingsUpperSpindleStartStopButton(QWidget *widget)  // Страница: Настройки
{
    settingsUpperSpindleStartStopButton = widget;
}

void registerSettingsLowerSpindleStartStopButton(QWidget *widget)  // Страница: Настройки
{
    settingsLowerSpindleStartStopButton = widget;
}

void updateUpperSpindleStartStop(bool isStart)  // Страница: Главная
{
    if (upperSpindleStartStopButton) {
        if (upperSpindleStartStopState != isStart) {
            upperSpindleStartStopState = isStart;
            auto *btn = qobject_cast<TextButtonWidget*>(upperSpindleStartStopButton);
            if (btn) {
                btn->setStartState(isStart);
                qDebug() << "Values: Upper Spindle Start/Stop =" << (isStart ? "START" : "STOP");
            }
        }
    }
}

void updateLowerSpindleStartStop(bool isStart)  // Страница: Главная
{
    if (lowerSpindleStartStopButton) {
        if (lowerSpindleStartStopState != isStart) {
            lowerSpindleStartStopState = isStart;
            auto *btn = qobject_cast<TextButtonWidget*>(lowerSpindleStartStopButton);
            if (btn) {
                btn->setStartState(isStart);
                qDebug() << "Values: Lower Spindle Start/Stop =" << (isStart ? "START" : "STOP");
            }
        }
    }
}

void updateSettingsUpperSpindleStartStop(bool isStart)  // Страница: Настройки
{
    if (settingsUpperSpindleStartStopButton) {
        if (settingsUpperSpindleStartStopState != isStart) {
            settingsUpperSpindleStartStopState = isStart;
            auto *btn = qobject_cast<TextButtonWidget*>(settingsUpperSpindleStartStopButton);
            if (btn) {
                btn->setStartState(isStart);
                qDebug() << "Values: Settings Upper Spindle Start/Stop =" << (isStart ? "START" : "STOP");
            }
        }
    }
}

void updateSettingsLowerSpindleStartStop(bool isStart)  // Страница: Настройки
{
    if (settingsLowerSpindleStartStopButton) {
        if (settingsLowerSpindleStartStopState != isStart) {
            settingsLowerSpindleStartStopState = isStart;
            auto *btn = qobject_cast<TextButtonWidget*>(settingsLowerSpindleStartStopButton);
            if (btn) {
                btn->setStartState(isStart);
                qDebug() << "Values: Settings Lower Spindle Start/Stop =" << (isStart ? "START" : "STOP");
            }
        }
    }
}

bool getUpperSpindleStartStopState()  // Страница: Главная
{
    return upperSpindleStartStopState;
}

bool getLowerSpindleStartStopState()  // Страница: Главная
{
    return lowerSpindleStartStopState;
}

bool getSettingsUpperSpindleStartStopState()  // Страница: Настройки
{
    return settingsUpperSpindleStartStopState;
}

bool getSettingsLowerSpindleStartStopState()  // Страница: Настройки
{
    return settingsLowerSpindleStartStopState;
}

void registerVacuumPumpSelector(selector *widget)  // Страница: Вакуум
{
    vacuumPumpSelector = widget;
}

void registerVacuumValveSelector(selector *widget)  // Страница: Вакуум
{
    vacuumValveSelector = widget;
}

void registerAutoPumpDownSelector(selector *widget)  // Страница: Вакуум
{
    autoPumpDownSelector = widget;
}

void registerUpperDoorSelector(doorselector *widget)  // Страница: Вакуум
{
    upperDoorSelector = widget;
}

void registerLowerDoorSelector(doorselector *widget)  // Страница: Вакуум
{
    lowerDoorSelector = widget;
}

void registerMainDoorStatus(QWidget *widget)  // Страница: Вакуум
{
    mainDoorStatusWidget = widget;
}

void registerLightingButton(int index, QWidget *widget)  // Страница: Вакуум
{
    if (index >= 0 && index < 4) {
        lightingButtons[index] = widget;
    }
}

void updateVacuumPumpSelector(bool state)  // Страница: Вакуум
{
    if (vacuumPumpSelector) {
        vacuumPumpSelectorState = state;  // Сохраняем состояние
        vacuumPumpSelector->set(state, true);
        qDebug() << "Values: Vacuum Pump Selector =" << (state ? "On" : "Off");
    }
}

void updateVacuumValveSelector(bool state)  // Страница: Вакуум
{
    if (vacuumValveSelector) {
        vacuumValveSelectorState = state;  // Сохраняем состояние
        vacuumValveSelector->set(state, true);
        qDebug() << "Values: Vacuum Valve Selector =" << (state ? "On" : "Off");
    }
}

void updateAutoPumpDownSelector(bool state)  // Страница: Вакуум
{
    if (autoPumpDownSelector) {
        autoPumpDownSelectorState = state;  // Сохраняем состояние
        autoPumpDownSelector->set(state, true);
        qDebug() << "Values: Auto Pump Down Selector =" << (state ? "On" : "Off");
    }
}

void updateUpperDoorSelector(bool state)  // Страница: Вакуум, true = Откр, false = Закр
{
    if (upperDoorSelector) {
        upperDoorSelectorState = state;  // Сохраняем состояние
        upperDoorSelector->set(state, true);
        qDebug() << "Values: Upper Door Selector =" << (state ? "Open" : "Closed");
    }
}

void updateLowerDoorSelector(bool state)  // Страница: Вакуум, true = Откр, false = Закр
{
    if (lowerDoorSelector) {
        lowerDoorSelectorState = state;  // Сохраняем состояние
        lowerDoorSelector->set(state, true);
        qDebug() << "Values: Lower Door Selector =" << (state ? "Open" : "Closed");
    }
}

void updateMainDoorStatus(bool isOpen)  // Страница: Вакуум
{
    if (mainDoorStatusWidget) {
        mainDoorStatusState = isOpen;  // Сохраняем состояние
        TextButtonWidget *btn = qobject_cast<TextButtonWidget*>(mainDoorStatusWidget);
        if (btn) {
            if (isOpen) {
                btn->setText(QString::fromUtf8("Дверь открыта"));
                btn->setBackgroundColor("#d0d3d4");
            } else {
                btn->setText(QString::fromUtf8("Дверь закрыта"));
                btn->setBackgroundColor("#95a5a6");
            }
            qDebug() << "Values: Main Door Status =" << (isOpen ? "Open" : "Closed");
        }
    }
}

void updateLightingButton(int index, bool isOn)  // Страница: Вакуум, 0-3: верх, лево, право, низ
{
    if (index >= 0 && index < 4 && lightingButtons[index]) {
        IconButtonWidget *btn = qobject_cast<IconButtonWidget*>(lightingButtons[index]);
        if (btn) {
            lightingButtonStates[index] = isOn;  // Сохраняем состояние в глобальном массиве
            btn->setIcon("lightbulb");
            btn->setBackgroundColor(isOn ? "#f1c40f" : "#bdc3c7");
            qDebug() << "Values: Lighting Button" << index << "=" << (isOn ? "On" : "Off");
        }
    }
}

bool getLightingButtonState(int index)  // Страница: Вакуум
{
    if (index >= 0 && index < 4) {
        return lightingButtonStates[index];
    }
    return false;
}

bool getVacuumPumpSelectorState()  // Страница: Вакуум
{
    return vacuumPumpSelectorState;
}

bool getVacuumValveSelectorState()  // Страница: Вакуум
{
    return vacuumValveSelectorState;
}

bool getAutoPumpDownSelectorState()  // Страница: Вакуум
{
    return autoPumpDownSelectorState;
}

bool getUpperDoorSelectorState()  // Страница: Вакуум
{
    return upperDoorSelectorState;
}

bool getLowerDoorSelectorState()  // Страница: Вакуум
{
    return lowerDoorSelectorState;
}

bool getMainDoorStatusState()  // Страница: Вакуум
{
    return mainDoorStatusState;
}

} // namespace Values
