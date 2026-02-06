#include "activity.h"

#include <QDebug>

namespace Activity {

// XY Control
void handleXYUp()
{
    qDebug() << "Activity: XY Up";
    // TODO: реализовать обработку движения XY вверх
}

void handleXYDown()
{
    qDebug() << "Activity: XY Down";
    // TODO: реализовать обработку движения XY вниз
}

void handleXYLeft()
{
    qDebug() << "Activity: XY Left";
    // TODO: реализовать обработку движения XY влево
}

void handleXYRight()
{
    qDebug() << "Activity: XY Right";
    // TODO: реализовать обработку движения XY вправо
}

// Coil Control
void handleCoilUp()
{
    qDebug() << "Activity: Coil Up";
    // TODO: реализовать обработку движения катушки вверх
}

void handleCoilDown()
{
    qDebug() << "Activity: Coil Down";
    // TODO: реализовать обработку движения катушки вниз
}

void handleStartOscillations()
{
    qDebug() << "Activity: Start Oscillations";
    // TODO: реализовать запуск колебаний
}

// Upper Spindle Control
void handleUpperSpindleLeft()
{
    qDebug() << "Activity: Upper Spindle Left";
    // TODO: реализовать обработку движения верхнего шпинделя влево
}

void handleUpperSpindleRight()
{
    qDebug() << "Activity: Upper Spindle Right";
    // TODO: реализовать обработку движения верхнего шпинделя вправо
}

void handleUpperSpindleSpeed()
{
    qDebug() << "Activity: Upper Spindle Speed";
    // TODO: реализовать обработку изменения скорости верхнего шпинделя
}

void handleUpperSpindleUp()
{
    qDebug() << "Activity: Upper Spindle Up";
    // TODO: реализовать обработку движения верхнего шпинделя вверх
}

void handleUpperSpindleDown()
{
    qDebug() << "Activity: Upper Spindle Down";
    // TODO: реализовать обработку движения верхнего шпинделя вниз
}

void handleUpperSpindleUpUp()
{
    qDebug() << "Activity: Upper Spindle UpUp";
    // TODO: реализовать обработку быстрого движения верхнего шпинделя вверх
}

void handleUpperSpindleDownDown()
{
    qDebug() << "Activity: Upper Spindle DownDown";
    // TODO: реализовать обработку быстрого движения верхнего шпинделя вниз
}

void handleUpperSpindleStartStop(bool isStart)
{
    if (isStart) {
        qDebug() << "Activity: Upper Spindle Start";
        // TODO: реализовать запуск верхнего шпинделя
    } else {
        qDebug() << "Activity: Upper Spindle Stop";
        // TODO: реализовать остановку верхнего шпинделя
    }
}

// Lower Spindle Control
void handleLowerSpindleLeft()
{
    qDebug() << "Activity: Lower Spindle Left";
    // TODO: реализовать обработку движения нижнего шпинделя влево
}

void handleLowerSpindleRight()
{
    qDebug() << "Activity: Lower Spindle Right";
    // TODO: реализовать обработку движения нижнего шпинделя вправо
}

void handleLowerSpindleSpeed()
{
    qDebug() << "Activity: Lower Spindle Speed";
    // TODO: реализовать обработку изменения скорости нижнего шпинделя
}

void handleLowerSpindleUp()
{
    qDebug() << "Activity: Lower Spindle Up";
    // TODO: реализовать обработку движения нижнего шпинделя вверх
}

void handleLowerSpindleDown()
{
    qDebug() << "Activity: Lower Spindle Down";
    // TODO: реализовать обработку движения нижнего шпинделя вниз
}

void handleLowerSpindleUpUp()
{
    qDebug() << "Activity: Lower Spindle UpUp";
    // TODO: реализовать обработку быстрого движения нижнего шпинделя вверх
}

void handleLowerSpindleDownDown()
{
    qDebug() << "Activity: Lower Spindle DownDown";
    // TODO: реализовать обработку быстрого движения нижнего шпинделя вниз
}

void handleLowerSpindleStartStop(bool isStart)
{
    if (isStart) {
        qDebug() << "Activity: Lower Spindle Start";
        // TODO: реализовать запуск нижнего шпинделя
    } else {
        qDebug() << "Activity: Lower Spindle Stop";
        // TODO: реализовать остановку нижнего шпинделя
    }
}

void handleLowerSpindleHold()
{
    qDebug() << "Activity: Lower Spindle Hold";
    // TODO: реализовать обработку удерживания нижнего шпинделя
}

void handleLowerSpindleOscillate()
{
    qDebug() << "Activity: Lower Spindle Oscillate";
    // TODO: реализовать обработку осцилляции нижнего шпинделя
}

void handleLowerSpindleAutot()
{
    qDebug() << "Activity: Lower Spindle Autot";
    // TODO: реализовать обработку автотяги нижнего шпинделя
}

// Heating
void handleHeatingStateChanged(int state)
{
    if (state == 0) {
        qDebug() << "Activity: Heating Off";
        // TODO: реализовать выключение подогрева
    } else {
        qDebug() << "Activity: Heating On";
        // TODO: реализовать включение подогрева
    }
}

// Generator
void handleGeneratorReset()
{
    qDebug() << "Activity: Generator Reset";
    // TODO: реализовать сброс генератора
}

void handleGeneratorStateChanged(int state)
{
    if (state == 0) {
        qDebug() << "Activity: Generator Off";
        // TODO: реализовать выключение генератора
    } else {
        qDebug() << "Activity: Generator On";
        // TODO: реализовать включение генератора
    }
}

// Reflector
void handleReflectorUp()
{
    qDebug() << "Activity: Reflector Up";
    // TODO: реализовать обработку движения рефлектора вверх
}

void handleReflectorDown()
{
    qDebug() << "Activity: Reflector Down";
    // TODO: реализовать обработку движения рефлектора вниз
}

// Lighting
void handleLightingMode(int mode)
{
    if (mode == 0) {
        qDebug() << "Activity: Lighting Mode - All";
        // TODO: реализовать режим освещения "Всё"
    } else {
        qDebug() << "Activity: Lighting Mode - 50%";
        // TODO: реализовать режим освещения "50%"
    }
}

// Bottom Controls
void handleNitrogenValveOpen()
{
    qDebug() << "Activity: Nitrogen Valve Open";
    // TODO: реализовать открытие азотного крана
}

void handleAutodope()
{
    qDebug() << "Activity: Autodope";
    // TODO: реализовать автолегирование
}

void handleAutodopeSettings()
{
    qDebug() << "Activity: Autodope Settings";
    // TODO: открыть страницу настроек автолегирования на контроллере/ПЛК
}

// Doping Page
void handleArgonStartStop(bool isStart)
{
    if (isStart) {
        qDebug() << "Activity: Argon Start";
        // TODO: реализовать запуск подачи аргона
    } else {
        qDebug() << "Activity: Argon Stop";
        // TODO: реализовать остановку подачи аргона
    }
}

void handleInjectionStartStop(bool isStart)
{
    if (isStart) {
        qDebug() << "Activity: Injection Start";
        // TODO: реализовать запуск впрыска
    } else {
        qDebug() << "Activity: Injection Stop";
        // TODO: реализовать остановку впрыска
    }
}

void handlePhosphorusSelect()
{
    qDebug() << "Activity: Phosphorus Select";
    // TODO: реализовать выбор фосфора
}

void handleDiboraneSelect()
{
    qDebug() << "Activity: Diborane Select";
    // TODO: реализовать выбор диборана
}

// Status Bar
void handleStatusBarReset()
{
    qDebug() << "Activity: Status Bar Reset";
    // TODO: реализовать сброс статус-бара
}

// Final Page
void handleApplyToAutot()
{
    qDebug() << "Activity: Apply to Autot";
    // TODO: реализовать применение финальных параметров к автотяге
}

} // namespace Activity
