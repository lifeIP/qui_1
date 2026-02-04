#pragma once

// Глобальные функции для обработки действий пользователя
// Все действия с кнопками и элементами управления обрабатываются здесь

namespace Activity {

// XY Control
void handleXYUp();
void handleXYDown();
void handleXYLeft();
void handleXYRight();

// Coil Control
void handleCoilUp();
void handleCoilDown();
void handleStartOscillations();

// Upper Spindle Control
void handleUpperSpindleLeft();
void handleUpperSpindleRight();
void handleUpperSpindleSpeed();
void handleUpperSpindleUp();
void handleUpperSpindleDown();
void handleUpperSpindleUpUp();
void handleUpperSpindleDownDown();
void handleUpperSpindleStartStop(bool isStart);

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

// Heating
void handleHeatingStateChanged(int state); // 0 = Выкл, 1 = Вкл

// Generator
void handleGeneratorReset();
void handleGeneratorStateChanged(int state); // 0 = Выкл, 1 = Вкл

// Reflector
void handleReflectorUp();
void handleReflectorDown();

// Lighting
void handleLightingMode(int mode); // 0 = Всё, 1 = 50%

// Bottom Controls
void handleNitrogenValveOpen();
void handleAutodope();

// Doping Page
void handleArgonStop();
void handleInjectionStart();
void handlePhosphorusSelect();
void handleDiboraneSelect();

// Status Bar
void handleStatusBarReset();

// Final Page
void handleApplyToAutot();

} // namespace Activity
