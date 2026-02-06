#pragma once

#include <QLabel>
#include <QString>

// Forward declarations
class selector;
class doorselector;

// Глобальная система управления значениями в интерфейсе
// Все изменяемые значения обрабатываются через функции этого модуля

namespace Values {

// Регистрация виджетов для обновления значений
void registerXYOffsetX(QLabel *label);
void registerXYOffsetY(QLabel *label);

// Регистрация виджетов для страницы настроек (Settings Page)
void registerSettingsXYOffsetX(QLabel *label);
void registerSettingsXYOffsetY(QLabel *label);

void registerCoilOffset(QLabel *label);
void registerCoilOscillations(QLabel *label);

void registerSettingsCoilOffset(QLabel *label);
void registerSettingsCoilOscillations(QLabel *label);

void registerUpperSpindleXOffset(QLabel *label);
void registerUpperSpindleSpeed(QLabel *label);
void registerUpperSpindlePosition(QLabel *label);

void registerSettingsUpperSpindleXOffset(QLabel *label);
void registerSettingsUpperSpindleSpeed(QLabel *label);
void registerSettingsUpperSpindlePosition(QLabel *label);

void registerLowerSpindleXOffset(QLabel *label);
void registerLowerSpindleSpeed(QLabel *label);
void registerLowerSpindlePosition(QLabel *label);

void registerSettingsLowerSpindleXOffset(QLabel *label);
void registerSettingsLowerSpindleSpeed(QLabel *label);
void registerSettingsLowerSpindlePosition(QLabel *label);

void registerGridAmp(QLabel *label);
void registerPValue(QLabel *label);
void registerIValue(QLabel *label);
void registerUValue(QLabel *label);

void registerSettingsGridAmp(QLabel *label);
void registerSettingsPValue(QLabel *label);
void registerSettingsIValue(QLabel *label);
void registerSettingsUValue(QLabel *label);

void registerGeneratorPercent(QLabel *label);
void registerStopwatchPercent(QLabel *label);

void registerSettingsGeneratorPercent(QLabel *label);

void registerDopingArgon(QLabel *label);
void registerDopingGas(QLabel *label);
void registerDopingInjection(QLabel *label);
void registerDopingPurge(QLabel *label);

void registerFlowArgon(QLabel *label);
void registerFlowDopingGas(QLabel *label);
void registerFlowInjection(QLabel *label);
void registerFlowPurge(QLabel *label);

// Final Page
void registerFinalGrowthRate(QLabel *label);
void registerFinalDiameter(QLabel *label);
void registerFinalLowSpeed(QLabel *label);
void registerFinalPolysiliconDiameter(QLabel *label);
void registerFinalHighSpeed(QLabel *label);

// Vacuum Page
void registerVacuumPumpPressure(QLabel *label);
void registerVacuumChamberPressure(QLabel *label);
void registerVacuumPumpingTime(QLabel *label);
void registerVacuumGasPressure(QLabel *label);
void registerVacuumSwitch(QLabel *label);
void registerVacuumPumpAlarm(QLabel *label);

// Status Bar
enum class ConnectionStatus {
    Connecting,    // идет подключение
    Disconnected, // нет связи с контроллером
    Connected      // контроллер подключен
};
void registerStatusBar(QLabel *statusDot, QLabel *statusText);
void updateConnectionStatus(ConnectionStatus status);

// Vacuum Pump Status
enum class PumpPressureStatus {
    Low,              // низкое - синий
    LowMedium,        // низкое-среднее - бирюзовый
    Medium,           // среднее - желтый
    MediumHigh,       // среднее-высокое - оранжевый
    High              // высокое - красный
};
void registerPumpPressureStatus(QLabel *statusLabel);
void updatePumpPressureStatus(PumpPressureStatus status);

// Функции обновления значений
void updateXYOffsetX(double value);  // в MM
void updateXYOffsetY(double value);  // в MM

// Settings Page
void updateSettingsXYOffsetX(double value);  // в MM
void updateSettingsXYOffsetY(double value);  // в MM

void updateCoilOffset(double value);  // в MM
void updateCoilOscillations(double value);  // в MM/мин

void updateSettingsCoilOffset(double value);  // в MM
void updateSettingsCoilOscillations(double value);  // в MM/мин

void updateUpperSpindleXOffset(double value);  // в MM
void updateUpperSpindleSpeed(double value);  // в MM/мин
void updateUpperSpindlePosition(double value);  // в MM

void updateSettingsUpperSpindleXOffset(double value);  // в MM
void updateSettingsUpperSpindleSpeed(double value);  // в MM/мин
void updateSettingsUpperSpindlePosition(double value);  // в MM

void updateLowerSpindleXOffset(double value);  // в MM
void updateLowerSpindleSpeed(double value);  // в MM/мин
void updateLowerSpindlePosition(double value);  // в MM

void updateSettingsLowerSpindleXOffset(double value);  // в MM
void updateSettingsLowerSpindleSpeed(double value);  // в MM/мин
void updateSettingsLowerSpindlePosition(double value);  // в MM

void updateGridAmp(double value);  // в AMP
void updatePValue(double value);  // в %
void updateIValue(double value);  // в %
void updateUValue(double value);  // в %

void updateSettingsGridAmp(double value);  // в AMP
void updateSettingsPValue(double value);  // в %
void updateSettingsIValue(double value);  // в %
void updateSettingsUValue(double value);  // в %

void updateGeneratorPercent(double value);  // в %
void updateStopwatchPercent(double value);  // в %

void updateSettingsGeneratorPercent(double value);  // в %

void updateDopingArgon(double value, const QString &unit = "л/мин");
void updateDopingGas(double value, const QString &unit = "мл/мин");
void updateDopingInjection(double value, const QString &unit = "л/мин");
void updateDopingPurge(double value, const QString &unit = "бар");

void updateFlowArgon(double value, const QString &unit = "л/мин");
void updateFlowDopingGas(double value, const QString &unit = "мл/мин");
void updateFlowInjection(double value, const QString &unit = "мл/мин");
void updateFlowPurge(double value, const QString &unit = "бар");

// Final Page
void updateFinalGrowthRate(double value);  // в мм
void updateFinalDiameter(double value);  // в мм
void updateFinalLowSpeed(double value);  // в мм
void updateFinalPolysiliconDiameter(double value);  // в мм
void updateFinalHighSpeed(double value);  // в мм

// Vacuum Page
void updateVacuumPumpPressure(double value);  // в мбар
void updateVacuumChamberPressure(double value);  // в мбар
void updateVacuumPumpingTime(double value);  // в сек
void updateVacuumGasPressure(double value);  // в бар
void updateVacuumSwitch(double value);  // в мбар
void updateVacuumPumpAlarm(double value);  // в сек

// Main Page Controls
void registerHeatingSelector(class selector *widget);
void registerGeneratorSelector(class selector *widget);
void registerUpperSpindleStartStopButton(class QWidget *widget);  // TextButtonWidget для СТАРТ/СТОП верхнего шпинделя
void registerLowerSpindleStartStopButton(class QWidget *widget);  // TextButtonWidget для СТАРТ/СТОП нижнего шпинделя

// Settings Page Controls
void registerSettingsHeatingSelector(class selector *widget);
void registerSettingsGeneratorSelector(class selector *widget);
void registerSettingsUpperSpindleStartStopButton(class QWidget *widget);  // TextButtonWidget для СТАРТ/СТОП верхнего шпинделя (настройки)
void registerSettingsLowerSpindleStartStopButton(class QWidget *widget);  // TextButtonWidget для СТАРТ/СТОП нижнего шпинделя (настройки)

void updateHeatingSelector(bool state);  // true = Вкл, false = Выкл
void updateGeneratorSelector(bool state);  // true = Вкл, false = Выкл
void updateUpperSpindleStartStop(bool isStart);  // true = СТАРТ, false = СТОП
void updateLowerSpindleStartStop(bool isStart);  // true = СТАРТ, false = СТОП

void updateSettingsHeatingSelector(bool state);  // true = Вкл, false = Выкл (настройки)
void updateSettingsGeneratorSelector(bool state);  // true = Вкл, false = Выкл (настройки)
void updateSettingsUpperSpindleStartStop(bool isStart);  // true = СТАРТ, false = СТОП (настройки)
void updateSettingsLowerSpindleStartStop(bool isStart);  // true = СТАРТ, false = СТОП (настройки)

bool getHeatingSelectorState();  // Получить текущее состояние селектора нагрева
bool getGeneratorSelectorState();  // Получить текущее состояние селектора генератора
bool getUpperSpindleStartStopState();  // Получить текущее состояние кнопки СТАРТ/СТОП верхнего шпинделя
bool getLowerSpindleStartStopState();  // Получить текущее состояние кнопки СТАРТ/СТОП нижнего шпинделя

// Settings Page Controls state getters (при необходимости использования)
bool getSettingsHeatingSelectorState();
bool getSettingsGeneratorSelectorState();
bool getSettingsUpperSpindleStartStopState();
bool getSettingsLowerSpindleStartStopState();

// Vacuum Page Controls
void registerVacuumPumpSelector(class selector *widget);
void registerVacuumValveSelector(class selector *widget);
void registerAutoPumpDownSelector(class selector *widget);
void registerUpperDoorSelector(class doorselector *widget);
void registerLowerDoorSelector(class doorselector *widget);
void registerMainDoorStatus(class QWidget *widget);  // TextButtonWidget для статуса главной двери
void registerLightingButton(int index, class QWidget *widget);  // IconButtonWidget для освещения

void updateVacuumPumpSelector(bool state);
void updateVacuumValveSelector(bool state);
void updateAutoPumpDownSelector(bool state);
void updateUpperDoorSelector(bool state);  // true = Откр, false = Закр
void updateLowerDoorSelector(bool state);  // true = Откр, false = Закр
void updateMainDoorStatus(bool isOpen);  // true = открыта, false = закрыта
void updateLightingButton(int index, bool isOn);  // 0-3: верх, лево, право, низ
bool getLightingButtonState(int index);  // Получить текущее состояние кнопки освещения

// Получить текущее состояние селекторов и doorselector
bool getVacuumPumpSelectorState();
bool getVacuumValveSelectorState();
bool getAutoPumpDownSelectorState();
bool getUpperDoorSelectorState();  // true = Откр, false = Закр
bool getLowerDoorSelectorState();  // true = Откр, false = Закр
bool getMainDoorStatusState();  // true = открыта, false = закрыта

} // namespace Values
