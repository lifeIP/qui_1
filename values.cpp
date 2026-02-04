#include "values.h"

#include <QDebug>
#include <QLabel>

namespace Values {

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

static QLabel *dopingArgonLabel = nullptr;
static QLabel *dopingGasLabel = nullptr;
static QLabel *dopingInjectionLabel = nullptr;
static QLabel *dopingPurgeLabel = nullptr;

static QLabel *flowArgonLabel = nullptr;
static QLabel *flowDopingGasLabel = nullptr;
static QLabel *flowInjectionLabel = nullptr;
static QLabel *flowPurgeLabel = nullptr;

static QLabel *finalGrowthRateLabel = nullptr;
static QLabel *finalDiameterLabel = nullptr;
static QLabel *finalLowSpeedLabel = nullptr;
static QLabel *finalPolysiliconDiameterLabel = nullptr;
static QLabel *finalHighSpeedLabel = nullptr;

// Регистрация виджетов
void registerXYOffsetX(QLabel *label) { xyOffsetXLabel = label; }
void registerXYOffsetY(QLabel *label) { xyOffsetYLabel = label; }

void registerCoilOffset(QLabel *label) { coilOffsetLabel = label; }
void registerCoilOscillations(QLabel *label) { coilOscillationsLabel = label; }

void registerUpperSpindleXOffset(QLabel *label) { upperSpindleXOffsetLabel = label; }
void registerUpperSpindleSpeed(QLabel *label) { upperSpindleSpeedLabel = label; }
void registerUpperSpindlePosition(QLabel *label) { upperSpindlePositionLabel = label; }

void registerLowerSpindleXOffset(QLabel *label) { lowerSpindleXOffsetLabel = label; }
void registerLowerSpindleSpeed(QLabel *label) { lowerSpindleSpeedLabel = label; }
void registerLowerSpindlePosition(QLabel *label) { lowerSpindlePositionLabel = label; }

void registerGridAmp(QLabel *label) { gridAmpLabel = label; }
void registerPValue(QLabel *label) { pValueLabel = label; }
void registerIValue(QLabel *label) { iValueLabel = label; }
void registerUValue(QLabel *label) { uValueLabel = label; }

void registerGeneratorPercent(QLabel *label) { generatorPercentLabel = label; }
void registerStopwatchPercent(QLabel *label) { stopwatchPercentLabel = label; }

void registerDopingArgon(QLabel *label) { dopingArgonLabel = label; }
void registerDopingGas(QLabel *label) { dopingGasLabel = label; }
void registerDopingInjection(QLabel *label) { dopingInjectionLabel = label; }
void registerDopingPurge(QLabel *label) { dopingPurgeLabel = label; }

void registerFlowArgon(QLabel *label) { flowArgonLabel = label; }
void registerFlowDopingGas(QLabel *label) { flowDopingGasLabel = label; }
void registerFlowInjection(QLabel *label) { flowInjectionLabel = label; }
void registerFlowPurge(QLabel *label) { flowPurgeLabel = label; }

void registerFinalGrowthRate(QLabel *label) { finalGrowthRateLabel = label; }
void registerFinalDiameter(QLabel *label) { finalDiameterLabel = label; }
void registerFinalLowSpeed(QLabel *label) { finalLowSpeedLabel = label; }
void registerFinalPolysiliconDiameter(QLabel *label) { finalPolysiliconDiameterLabel = label; }
void registerFinalHighSpeed(QLabel *label) { finalHighSpeedLabel = label; }

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
void updateXYOffsetX(double value)
{
    if (xyOffsetXLabel) {
        xyOffsetXLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: XY Offset X =" << value;
    }
}

void updateXYOffsetY(double value)
{
    if (xyOffsetYLabel) {
        xyOffsetYLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: XY Offset Y =" << value;
    }
}

void updateCoilOffset(double value)
{
    if (coilOffsetLabel) {
        coilOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Coil Offset =" << value;
    }
}

void updateCoilOscillations(double value)
{
    if (coilOscillationsLabel) {
        coilOscillationsLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Coil Oscillations =" << value;
    }
}

void updateUpperSpindleXOffset(double value)
{
    if (upperSpindleXOffsetLabel) {
        upperSpindleXOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Upper Spindle X Offset =" << value;
    }
}

void updateUpperSpindleSpeed(double value)
{
    if (upperSpindleSpeedLabel) {
        upperSpindleSpeedLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Upper Spindle Speed =" << value;
    }
}

void updateUpperSpindlePosition(double value)
{
    if (upperSpindlePositionLabel) {
        upperSpindlePositionLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Upper Spindle Position =" << value;
    }
}

void updateLowerSpindleXOffset(double value)
{
    if (lowerSpindleXOffsetLabel) {
        lowerSpindleXOffsetLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Lower Spindle X Offset =" << value;
    }
}

void updateLowerSpindleSpeed(double value)
{
    if (lowerSpindleSpeedLabel) {
        lowerSpindleSpeedLabel->setText(formatValue(value, "MM/мин"));
        qDebug() << "Values: Lower Spindle Speed =" << value;
    }
}

void updateLowerSpindlePosition(double value)
{
    if (lowerSpindlePositionLabel) {
        lowerSpindlePositionLabel->setText(formatValue(value, "MM"));
        qDebug() << "Values: Lower Spindle Position =" << value;
    }
}

void updateGridAmp(double value)
{
    if (gridAmpLabel) {
        gridAmpLabel->setText(formatValue2(value, "AMP"));
        qDebug() << "Values: Grid AMP =" << value;
    }
}

void updatePValue(double value)
{
    if (pValueLabel) {
        pValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: P Value =" << value;
    }
}

void updateIValue(double value)
{
    if (iValueLabel) {
        iValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: I Value =" << value;
    }
}

void updateUValue(double value)
{
    if (uValueLabel) {
        uValueLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: U Value =" << value;
    }
}

void updateGeneratorPercent(double value)
{
    if (generatorPercentLabel) {
        generatorPercentLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Generator Percent =" << value;
    }
}

void updateStopwatchPercent(double value)
{
    if (stopwatchPercentLabel) {
        stopwatchPercentLabel->setText(formatValue(value, "%"));
        qDebug() << "Values: Stopwatch Percent =" << value;
    }
}

void updateDopingArgon(double value, const QString &unit)
{
    if (dopingArgonLabel) {
        dopingArgonLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Argon =" << value << unit;
    }
}

void updateDopingGas(double value, const QString &unit)
{
    if (dopingGasLabel) {
        dopingGasLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Gas =" << value << unit;
    }
}

void updateDopingInjection(double value, const QString &unit)
{
    if (dopingInjectionLabel) {
        dopingInjectionLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Injection =" << value << unit;
    }
}

void updateDopingPurge(double value, const QString &unit)
{
    if (dopingPurgeLabel) {
        dopingPurgeLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Doping Purge =" << value << unit;
    }
}

void updateFlowArgon(double value, const QString &unit)
{
    if (flowArgonLabel) {
        flowArgonLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Argon =" << value << unit;
    }
}

void updateFlowDopingGas(double value, const QString &unit)
{
    if (flowDopingGasLabel) {
        flowDopingGasLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Doping Gas =" << value << unit;
    }
}

void updateFlowInjection(double value, const QString &unit)
{
    if (flowInjectionLabel) {
        flowInjectionLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Injection =" << value << unit;
    }
}

void updateFlowPurge(double value, const QString &unit)
{
    if (flowPurgeLabel) {
        flowPurgeLabel->setText(formatValue(value, unit));
        qDebug() << "Values: Flow Purge =" << value << unit;
    }
}

// Final Page
void updateFinalGrowthRate(double value)
{
    if (finalGrowthRateLabel) {
        finalGrowthRateLabel->setText(formatValue(value));
        qDebug() << "Values: Final Growth Rate =" << value;
    }
}

void updateFinalDiameter(double value)
{
    if (finalDiameterLabel) {
        finalDiameterLabel->setText(formatValue(value));
        qDebug() << "Values: Final Diameter =" << value;
    }
}

void updateFinalLowSpeed(double value)
{
    if (finalLowSpeedLabel) {
        finalLowSpeedLabel->setText(formatValue(value));
        qDebug() << "Values: Final Low Speed =" << value;
    }
}

void updateFinalPolysiliconDiameter(double value)
{
    if (finalPolysiliconDiameterLabel) {
        finalPolysiliconDiameterLabel->setText(formatValue(value));
        qDebug() << "Values: Final Polysilicon Diameter =" << value;
    }
}

void updateFinalHighSpeed(double value)
{
    if (finalHighSpeedLabel) {
        finalHighSpeedLabel->setText(formatValue(value));
        qDebug() << "Values: Final High Speed =" << value;
    }
}

} // namespace Values
