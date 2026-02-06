#pragma once

#include <QLabel>
#include <QString>

// Глобальная система управления значениями в интерфейсе
// Все изменяемые значения обрабатываются через функции этого модуля

namespace Values {

// Регистрация виджетов для обновления значений
void registerXYOffsetX(QLabel *label);
void registerXYOffsetY(QLabel *label);

void registerCoilOffset(QLabel *label);
void registerCoilOscillations(QLabel *label);

void registerUpperSpindleXOffset(QLabel *label);
void registerUpperSpindleSpeed(QLabel *label);
void registerUpperSpindlePosition(QLabel *label);

void registerLowerSpindleXOffset(QLabel *label);
void registerLowerSpindleSpeed(QLabel *label);
void registerLowerSpindlePosition(QLabel *label);

void registerGridAmp(QLabel *label);
void registerPValue(QLabel *label);
void registerIValue(QLabel *label);
void registerUValue(QLabel *label);

void registerGeneratorPercent(QLabel *label);
void registerStopwatchPercent(QLabel *label);

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

void updateCoilOffset(double value);  // в MM
void updateCoilOscillations(double value);  // в MM/мин

void updateUpperSpindleXOffset(double value);  // в MM
void updateUpperSpindleSpeed(double value);  // в MM/мин
void updateUpperSpindlePosition(double value);  // в MM

void updateLowerSpindleXOffset(double value);  // в MM
void updateLowerSpindleSpeed(double value);  // в MM/мин
void updateLowerSpindlePosition(double value);  // в MM

void updateGridAmp(double value);  // в AMP
void updatePValue(double value);  // в %
void updateIValue(double value);  // в %
void updateUValue(double value);  // в %

void updateGeneratorPercent(double value);  // в %
void updateStopwatchPercent(double value);  // в %

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

} // namespace Values
