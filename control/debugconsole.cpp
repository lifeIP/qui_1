#include "control/debugconsole.h"
#include "activity.h"
#include "values.h"

#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QThread>
#include <QIODevice>
#include <iostream>
#include <QStringList>

DebugConsole::DebugConsole(QObject *parent)
    : QObject(parent)
    , m_inputStream(nullptr)
    , m_outputStream(nullptr)
    , m_running(false)
{
}

DebugConsole::~DebugConsole()
{
    stop();
}

void DebugConsole::start()
{
    if (m_running)
        return;
    
    m_running = true;
    m_inputStream = new QTextStream(stdin, QIODevice::ReadOnly);
    m_outputStream = new QTextStream(stdout, QIODevice::WriteOnly);
    
    printWelcome();
    printHelp();
    
    // Запускаем обработку команд в отдельном цикле
    QString line;
    while (m_running) {
        *m_outputStream << "> " << flush;
        
        // Ждем ввода данных (блокирующий вызов)
        line = m_inputStream->readLine();
        
        // Проверяем, не был ли поток остановлен
        if (!m_running)
            break;
        
        line = line.trimmed();
        
        if (line.isEmpty())
            continue;
        
        if (line.toLower() == "quit" || line.toLower() == "exit") {
            *m_outputStream << "Exiting debug console..." << endl;
            m_running = false;
            break;
        }
        
        if (line.toLower() == "help") {
            printHelp();
            continue;
        }
        
        processCommand(line);
    }
}

void DebugConsole::stop()
{
    m_running = false;
}

void DebugConsole::printWelcome()
{
    if (!m_outputStream)
        return;
    
    *m_outputStream << "\n";
    *m_outputStream << "========================================\n";
    *m_outputStream << "  Control Thread Debug Console\n";
    *m_outputStream << "========================================\n";
    *m_outputStream << "Type 'help' for available commands\n";
    *m_outputStream << "Type 'quit' or 'exit' to exit\n";
    *m_outputStream << "\n";
}

void DebugConsole::printHelp()
{
    if (!m_outputStream)
        return;
    
    *m_outputStream << "\n";
    *m_outputStream << "Available commands:\n";
    *m_outputStream << "  help                          - Show this help\n";
    *m_outputStream << "  quit/exit                     - Exit console\n";
    *m_outputStream << "\n";
    *m_outputStream << "Activity commands:\n";
    *m_outputStream << "  activity.xy.up                - XY Up\n";
    *m_outputStream << "  activity.xy.down              - XY Down\n";
    *m_outputStream << "  activity.xy.left              - XY Left\n";
    *m_outputStream << "  activity.xy.right              - XY Right\n";
    *m_outputStream << "  activity.coil.up              - Coil Up\n";
    *m_outputStream << "  activity.coil.down          - Coil Down\n";
    *m_outputStream << "  activity.coil.start           - Start Oscillations\n";
    *m_outputStream << "  activity.upper.start          - Upper Spindle Start\n";
    *m_outputStream << "  activity.upper.stop           - Upper Spindle Stop\n";
    *m_outputStream << "  activity.lower.start          - Lower Spindle Start\n";
    *m_outputStream << "  activity.lower.stop           - Lower Spindle Stop\n";
    *m_outputStream << "  activity.heating.on            - Heating On\n";
    *m_outputStream << "  activity.heating.off          - Heating Off\n";
    *m_outputStream << "  activity.generator.on         - Generator On\n";
    *m_outputStream << "  activity.generator.off         - Generator Off\n";
    *m_outputStream << "  activity.generator.reset      - Generator Reset\n";
    *m_outputStream << "  activity.argon.start          - Argon Start\n";
    *m_outputStream << "  activity.argon.stop            - Argon Stop\n";
    *m_outputStream << "  activity.injection.start       - Injection Start\n";
    *m_outputStream << "  activity.injection.stop       - Injection Stop\n";
    *m_outputStream << "\n";
    *m_outputStream << "Values commands:\n";
    *m_outputStream << "  values.set.xy.x <value>       - Set XY Offset X (MM)\n";
    *m_outputStream << "  values.set.xy.y <value>       - Set XY Offset Y (MM)\n";
    *m_outputStream << "  values.set.coil.offset <value> - Set Coil Offset (MM)\n";
    *m_outputStream << "  values.set.coil.osc <value>   - Set Coil Oscillations (MM/min)\n";
    *m_outputStream << "  values.set.upper.x <value>     - Set Upper Spindle X Offset (MM)\n";
    *m_outputStream << "  values.set.upper.speed <value> - Set Upper Spindle Speed (MM/min)\n";
    *m_outputStream << "  values.set.upper.pos <value>   - Set Upper Spindle Position (MM)\n";
    *m_outputStream << "  values.set.lower.x <value>     - Set Lower Spindle X Offset (MM)\n";
    *m_outputStream << "  values.set.lower.speed <value> - Set Lower Spindle Speed (MM/min)\n";
    *m_outputStream << "  values.set.lower.pos <value>   - Set Lower Spindle Position (MM)\n";
    *m_outputStream << "  values.set.grid.amp <value>    - Set Grid AMP\n";
    *m_outputStream << "  values.set.p <value>           - Set P Value (%)\n";
    *m_outputStream << "  values.set.i <value>           - Set I Value (%)\n";
    *m_outputStream << "  values.set.u <value>           - Set U Value (%)\n";
    *m_outputStream << "  values.set.generator <value>    - Set Generator Percent (%)\n";
    *m_outputStream << "  values.set.vacuum.pump <value>  - Set Vacuum Pump Pressure (mbar)\n";
    *m_outputStream << "  values.set.vacuum.chamber <value> - Set Vacuum Chamber Pressure (mbar)\n";
    *m_outputStream << "  values.set.vacuum.time <value> - Set Vacuum Pumping Time (sec)\n";
    *m_outputStream << "  values.set.vacuum.gas <value>   - Set Vacuum Gas Pressure (bar)\n";
    *m_outputStream << "\n";
    *m_outputStream << "Status commands:\n";
    *m_outputStream << "  status.connection.connecting   - Set connection status: Connecting\n";
    *m_outputStream << "  status.connection.disconnected  - Set connection status: Disconnected\n";
    *m_outputStream << "  status.connection.connected      - Set connection status: Connected\n";
    *m_outputStream << "  status.pump.low                 - Set pump pressure: Low\n";
    *m_outputStream << "  status.pump.lowmedium           - Set pump pressure: Low-Medium\n";
    *m_outputStream << "  status.pump.medium               - Set pump pressure: Medium\n";
    *m_outputStream << "  status.pump.mediumhigh          - Set pump pressure: Medium-High\n";
    *m_outputStream << "  status.pump.high                 - Set pump pressure: High\n";
    *m_outputStream << "\n";
}

void DebugConsole::processCommand(const QString &command)
{
    if (!m_outputStream)
        return;
    
    if (!parseAndExecute(command)) {
        *m_outputStream << "Unknown command: " << command << endl;
        *m_outputStream << "Type 'help' for available commands" << endl;
    }
}

bool DebugConsole::parseAndExecute(const QString &command)
{
    QStringList parts = command.split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return false;
    
    QString cmd = parts[0].toLower();
    
    // Activity commands
    if (cmd == "activity.xy.up") {
        Activity::handleXYUp();
        *m_outputStream << "Executed: XY Up" << endl;
        return true;
    }
    if (cmd == "activity.xy.down") {
        Activity::handleXYDown();
        *m_outputStream << "Executed: XY Down" << endl;
        return true;
    }
    if (cmd == "activity.xy.left") {
        Activity::handleXYLeft();
        *m_outputStream << "Executed: XY Left" << endl;
        return true;
    }
    if (cmd == "activity.xy.right") {
        Activity::handleXYRight();
        *m_outputStream << "Executed: XY Right" << endl;
        return true;
    }
    if (cmd == "activity.coil.up") {
        Activity::handleCoilUp();
        *m_outputStream << "Executed: Coil Up" << endl;
        return true;
    }
    if (cmd == "activity.coil.down") {
        Activity::handleCoilDown();
        *m_outputStream << "Executed: Coil Down" << endl;
        return true;
    }
    if (cmd == "activity.coil.start") {
        Activity::handleStartOscillations();
        *m_outputStream << "Executed: Start Oscillations" << endl;
        return true;
    }
    if (cmd == "activity.upper.start") {
        Activity::handleUpperSpindleStartStop(true);
        *m_outputStream << "Executed: Upper Spindle Start" << endl;
        return true;
    }
    if (cmd == "activity.upper.stop") {
        Activity::handleUpperSpindleStartStop(false);
        *m_outputStream << "Executed: Upper Spindle Stop" << endl;
        return true;
    }
    if (cmd == "activity.lower.start") {
        Activity::handleLowerSpindleStartStop(true);
        *m_outputStream << "Executed: Lower Spindle Start" << endl;
        return true;
    }
    if (cmd == "activity.lower.stop") {
        Activity::handleLowerSpindleStartStop(false);
        *m_outputStream << "Executed: Lower Spindle Stop" << endl;
        return true;
    }
    if (cmd == "activity.heating.on") {
        Activity::handleHeatingStateChanged(1);
        *m_outputStream << "Executed: Heating On" << endl;
        return true;
    }
    if (cmd == "activity.heating.off") {
        Activity::handleHeatingStateChanged(0);
        *m_outputStream << "Executed: Heating Off" << endl;
        return true;
    }
    if (cmd == "activity.generator.on") {
        Activity::handleGeneratorStateChanged(1);
        *m_outputStream << "Executed: Generator On" << endl;
        return true;
    }
    if (cmd == "activity.generator.off") {
        Activity::handleGeneratorStateChanged(0);
        *m_outputStream << "Executed: Generator Off" << endl;
        return true;
    }
    if (cmd == "activity.generator.reset") {
        Activity::handleGeneratorReset();
        *m_outputStream << "Executed: Generator Reset" << endl;
        return true;
    }
    if (cmd == "activity.argon.start") {
        Activity::handleArgonStartStop(true);
        *m_outputStream << "Executed: Argon Start" << endl;
        return true;
    }
    if (cmd == "activity.argon.stop") {
        Activity::handleArgonStartStop(false);
        *m_outputStream << "Executed: Argon Stop" << endl;
        return true;
    }
    if (cmd == "activity.injection.start") {
        Activity::handleInjectionStartStop(true);
        *m_outputStream << "Executed: Injection Start" << endl;
        return true;
    }
    if (cmd == "activity.injection.stop") {
        Activity::handleInjectionStartStop(false);
        *m_outputStream << "Executed: Injection Stop" << endl;
        return true;
    }
    
    // Values commands
    if (cmd.startsWith("values.set.")) {
        if (parts.size() < 2) {
            *m_outputStream << "Error: Value required. Usage: " << cmd << " <value>" << endl;
            return true;
        }
        
        bool ok;
        double value = parts[1].toDouble(&ok);
        if (!ok) {
            *m_outputStream << "Error: Invalid value: " << parts[1] << endl;
            return true;
        }
        
        if (cmd == "values.set.xy.x") {
            Values::updateXYOffsetX(value);
            *m_outputStream << "Set XY Offset X = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.xy.y") {
            Values::updateXYOffsetY(value);
            *m_outputStream << "Set XY Offset Y = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.coil.offset") {
            Values::updateCoilOffset(value);
            *m_outputStream << "Set Coil Offset = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.coil.osc") {
            Values::updateCoilOscillations(value);
            *m_outputStream << "Set Coil Oscillations = " << value << " MM/min" << endl;
            return true;
        }
        if (cmd == "values.set.upper.x") {
            Values::updateUpperSpindleXOffset(value);
            *m_outputStream << "Set Upper Spindle X Offset = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.upper.speed") {
            Values::updateUpperSpindleSpeed(value);
            *m_outputStream << "Set Upper Spindle Speed = " << value << " MM/min" << endl;
            return true;
        }
        if (cmd == "values.set.upper.pos") {
            Values::updateUpperSpindlePosition(value);
            *m_outputStream << "Set Upper Spindle Position = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.lower.x") {
            Values::updateLowerSpindleXOffset(value);
            *m_outputStream << "Set Lower Spindle X Offset = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.lower.speed") {
            Values::updateLowerSpindleSpeed(value);
            *m_outputStream << "Set Lower Spindle Speed = " << value << " MM/min" << endl;
            return true;
        }
        if (cmd == "values.set.lower.pos") {
            Values::updateLowerSpindlePosition(value);
            *m_outputStream << "Set Lower Spindle Position = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.grid.amp") {
            Values::updateGridAmp(value);
            *m_outputStream << "Set Grid AMP = " << value << " AMP" << endl;
            return true;
        }
        if (cmd == "values.set.p") {
            Values::updatePValue(value);
            *m_outputStream << "Set P Value = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.i") {
            Values::updateIValue(value);
            *m_outputStream << "Set I Value = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.u") {
            Values::updateUValue(value);
            *m_outputStream << "Set U Value = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.generator") {
            Values::updateGeneratorPercent(value);
            *m_outputStream << "Set Generator Percent = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.vacuum.pump") {
            Values::updateVacuumPumpPressure(value);
            *m_outputStream << "Set Vacuum Pump Pressure = " << value << " mbar" << endl;
            return true;
        }
        if (cmd == "values.set.vacuum.chamber") {
            Values::updateVacuumChamberPressure(value);
            *m_outputStream << "Set Vacuum Chamber Pressure = " << value << " mbar" << endl;
            return true;
        }
        if (cmd == "values.set.vacuum.time") {
            Values::updateVacuumPumpingTime(value);
            *m_outputStream << "Set Vacuum Pumping Time = " << value << " sec" << endl;
            return true;
        }
        if (cmd == "values.set.vacuum.gas") {
            Values::updateVacuumGasPressure(value);
            *m_outputStream << "Set Vacuum Gas Pressure = " << value << " bar" << endl;
            return true;
        }
    }
    
    // Status commands
    if (cmd == "status.connection.connecting") {
        Values::updateConnectionStatus(Values::ConnectionStatus::Connecting);
        *m_outputStream << "Set connection status: Connecting" << endl;
        return true;
    }
    if (cmd == "status.connection.disconnected") {
        Values::updateConnectionStatus(Values::ConnectionStatus::Disconnected);
        *m_outputStream << "Set connection status: Disconnected" << endl;
        return true;
    }
    if (cmd == "status.connection.connected") {
        Values::updateConnectionStatus(Values::ConnectionStatus::Connected);
        *m_outputStream << "Set connection status: Connected" << endl;
        return true;
    }
    if (cmd == "status.pump.low") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::Low);
        *m_outputStream << "Set pump pressure: Low" << endl;
        return true;
    }
    if (cmd == "status.pump.lowmedium") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::LowMedium);
        *m_outputStream << "Set pump pressure: Low-Medium" << endl;
        return true;
    }
    if (cmd == "status.pump.medium") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::Medium);
        *m_outputStream << "Set pump pressure: Medium" << endl;
        return true;
    }
    if (cmd == "status.pump.mediumhigh") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::MediumHigh);
        *m_outputStream << "Set pump pressure: Medium-High" << endl;
        return true;
    }
    if (cmd == "status.pump.high") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::High);
        *m_outputStream << "Set pump pressure: High" << endl;
        return true;
    }
    
    return false;
}
