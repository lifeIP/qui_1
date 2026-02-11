#pragma once

// Глобальные функции для обработки действий пользователя
// Все действия с кнопками и элементами управления обрабатываются здесь

namespace Activity {

// XY Control
void handleXYUp();
void handleXYDown();
void handleXYLeft();
void handleXYRight();

// XY Control (Settings Page)
void handleSettingsXYUp();
void handleSettingsXYDown();
void handleSettingsXYLeft();
void handleSettingsXYRight();

// Coil Control
void handleCoilUp();
void handleCoilDown();
void handleStartOscillations();

// Coil Control (Settings Page)
void handleSettingsCoilUp();
void handleSettingsCoilDown();
void handleSettingsStartOscillations();
void handleSettingsCoilNeutral();
void handleSettingsCoilAcceleration();
void handleSettingsCoilService();

// Upper Spindle Control
void handleUpperSpindleLeft();
void handleUpperSpindleRight();
void handleUpperSpindleSpeed();
void handleUpperSpindleUp();
void handleUpperSpindleDown();
void handleUpperSpindleUpUp();
void handleUpperSpindleDownDown();
void handleUpperSpindleStartStop(bool isStart);

// Upper Spindle Control (Settings Page)
void handleSettingsUpperSpindleLeft();
void handleSettingsUpperSpindleRight();
void handleSettingsUpperSpindleSpeed();
void handleSettingsUpperSpindleUp();
void handleSettingsUpperSpindleDown();
void handleSettingsUpperSpindleUpUp();
void handleSettingsUpperSpindleDownDown();
void handleSettingsUpperSpindleStartStop(bool isStart);

// Lower Spindle Control
void handleLowerSpindleLeft();
void handleLowerSpindleRight();
void handleLowerSpindleSpeed();
void handleLowerSpindleUp();
void handleLowerSpindleDown();
void handleLowerSpindleUpUp();
void handleLowerSpindleDownDown();
void handleLowerSpindleStartStop(bool isStart);
void handleLowerSpindleHold();
void handleLowerSpindleOscillate();
void handleLowerSpindleAutot();

// Lower Spindle Control (Settings Page)
void handleSettingsLowerSpindleLeft();
void handleSettingsLowerSpindleRight();
void handleSettingsLowerSpindleSpeed();
void handleSettingsLowerSpindleUp();
void handleSettingsLowerSpindleDown();
void handleSettingsLowerSpindleUpUp();
void handleSettingsLowerSpindleDownDown();
void handleSettingsLowerSpindleStartStop(bool isStart);
void handleSettingsLowerSpindleHold();
void handleSettingsLowerSpindleOscillate();
void handleSettingsLowerSpindleAutot();

// Heating
void handleHeatingStateChanged(int state); // 0 = Выкл, 1 = Вкл

// Heating (Settings Page)
void handleSettingsHeatingStateChanged(int state); // 0 = Выкл, 1 = Вкл

// Generator
void handleGeneratorReset();
void handleGeneratorStateChanged(int state); // 0 = Выкл, 1 = Вкл

// Generator (Settings Page)
void handleSettingsGeneratorReset();
void handleSettingsGeneratorStateChanged(int state); // 0 = Выкл, 1 = Вкл

// Reflector
void handleReflectorUp();
void handleReflectorDown();

// Reflector (Settings Page)
void handleSettingsReflectorUp();
void handleSettingsReflectorDown();

// Lighting
void handleLightingMode(int mode); // 0 = Всё, 1 = 50%

// Lighting (Settings Page)
void handleSettingsLightingMode(int mode); // 0 = Всё, 1 = 50%

// Bottom Controls
void handleNitrogenValveOpen();
void handleAutodope();

// Bottom Controls (Settings Page)
void handleSettingsNitrogenValveOpen();
void handleSettingsAutodope();

// Doping Page
void handleArgonStartStop(bool isStart);
void handleInjectionStartStop(bool isStart);
void handlePhosphorusSelect();
void handleDiboraneSelect();
void handleAutodopeSettings();

// Status Bar
void handleStatusBarReset();

// Final Page
void handleApplyToAutot();

// Vacuum Page
void handleVacuumPumpStateChanged(int state);  // 0 = Выкл, 1 = Вкл
void handleVacuumValveStateChanged(int state);  // 0 = Выкл, 1 = Вкл
void handleAutoPumpDownStateChanged(int state);  // 0 = Выкл, 1 = Вкл
void handleUpperDoorStateChanged(int state);  // 0 = Закр, 1 = Откр
void handleLowerDoorStateChanged(int state);  // 0 = Закр, 1 = Откр
void handleLightingButtonToggled(int buttonIndex, bool isOn);  // 0-3: верх, лево, право, низ

} // namespace Activity
