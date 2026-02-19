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
    *m_outputStream << QString::fromUtf8("  [Главная страница]\n");
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
    *m_outputStream << QString::fromUtf8("  [Страница Настроек]\n");
    *m_outputStream << "  activity.settings.xy.up                - Settings XY Up\n";
    *m_outputStream << "  activity.settings.xy.down              - Settings XY Down\n";
    *m_outputStream << "  activity.settings.xy.left              - Settings XY Left\n";
    *m_outputStream << "  activity.settings.xy.right             - Settings XY Right\n";
    *m_outputStream << "  activity.settings.coil.up              - Settings Coil Up\n";
    *m_outputStream << "  activity.settings.coil.down            - Settings Coil Down\n";
    *m_outputStream << "  activity.settings.coil.neutral          - Settings Coil Neutral\n";
    *m_outputStream << "  activity.settings.coil.acceleration    - Settings Coil Acceleration\n";
    *m_outputStream << "  activity.settings.coil.service         - Settings Coil Service\n";
    *m_outputStream << "  activity.settings.upper.start          - Settings Upper Spindle Start\n";
    *m_outputStream << "  activity.settings.upper.stop           - Settings Upper Spindle Stop\n";
    *m_outputStream << "  activity.settings.lower.start          - Settings Lower Spindle Start\n";
    *m_outputStream << "  activity.settings.lower.stop           - Settings Lower Spindle Stop\n";
    *m_outputStream << "  activity.settings.heating.on           - Settings Heating On\n";
    *m_outputStream << "  activity.settings.heating.off          - Settings Heating Off\n";
    *m_outputStream << "  activity.settings.generator.on         - Settings Generator On\n";
    *m_outputStream << "  activity.settings.generator.off        - Settings Generator Off\n";
    *m_outputStream << "  activity.settings.generator.reset      - Settings Generator Reset\n";
    *m_outputStream << "  activity.settings.reflector.up         - Settings Reflector Up\n";
    *m_outputStream << "  activity.settings.reflector.down       - Settings Reflector Down\n";
    *m_outputStream << "  activity.settings.lighting.all         - Settings Lighting All\n";
    *m_outputStream << "  activity.settings.lighting.half        - Settings Lighting 50%\n";
    *m_outputStream << "  activity.settings.nitrogen.open        - Settings Nitrogen Valve Open\n";
    *m_outputStream << "  activity.settings.autodope             - Settings Autodope\n";
    *m_outputStream << QString::fromUtf8("  [Страница Легирования]\n");
    *m_outputStream << "  activity.argon.start          - Argon Start\n";
    *m_outputStream << "  activity.argon.stop            - Argon Stop\n";
    *m_outputStream << "  activity.injection.start       - Injection Start\n";
    *m_outputStream << "  activity.injection.stop       - Injection Stop\n";
    *m_outputStream << "\n";
    *m_outputStream << "Values commands:\n";
    *m_outputStream << QString::fromUtf8("  [Главная страница]\n");
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
    *m_outputStream << QString::fromUtf8("  [Страница Настроек]\n");
    *m_outputStream << "  values.set.settings.xy.x <value>        - Set Settings XY Offset X (MM)\n";
    *m_outputStream << "  values.set.settings.xy.y <value>        - Set Settings XY Offset Y (MM)\n";
    *m_outputStream << "  values.set.settings.coil.offset <value> - Set Settings Coil Offset (MM)\n";
    *m_outputStream << "  values.set.settings.coil.osc <value>    - Set Settings Coil Oscillations (MM/min)\n";
    *m_outputStream << "  values.set.settings.upper.x <value>     - Set Settings Upper Spindle X Offset (MM)\n";
    *m_outputStream << "  values.set.settings.upper.speed <value> - Set Settings Upper Spindle Speed (MM/min)\n";
    *m_outputStream << "  values.set.settings.upper.pos <value>   - Set Settings Upper Spindle Position (MM)\n";
    *m_outputStream << "  values.set.settings.lower.x <value>     - Set Settings Lower Spindle X Offset (MM)\n";
    *m_outputStream << "  values.set.settings.lower.speed <value> - Set Settings Lower Spindle Speed (MM/min)\n";
    *m_outputStream << "  values.set.settings.lower.pos <value>   - Set Settings Lower Spindle Position (MM)\n";
    *m_outputStream << "  values.set.settings.grid.amp <value>    - Set Settings Grid AMP\n";
    *m_outputStream << "  values.set.settings.p <value>           - Set Settings P Value (%)\n";
    *m_outputStream << "  values.set.settings.i <value>           - Set Settings I Value (%)\n";
    *m_outputStream << "  values.set.settings.u <value>           - Set Settings U Value (%)\n";
    *m_outputStream << "  values.set.settings.generator <value>   - Set Settings Generator Percent (%)\n";
    *m_outputStream << "  values.set.settings.generator.status <0|1> - Set Generator Status (0=Error, 1=OK)\n";
    *m_outputStream << QString::fromUtf8("  [Страница Вакуум]\n");
    *m_outputStream << "  values.set.vacuum.pump <value>  - Set Vacuum Pump Pressure (mbar)\n";
    *m_outputStream << "  values.set.vacuum.chamber <value> - Set Vacuum Chamber Pressure (mbar)\n";
    *m_outputStream << "  values.set.vacuum.time <value> - Set Vacuum Pumping Time (sec)\n";
    *m_outputStream << "  values.set.vacuum.gas <value>   - Set Vacuum Gas Pressure (bar)\n";
    *m_outputStream << "\n";
    *m_outputStream << "Main Page Control commands:\n";
    *m_outputStream << QString::fromUtf8("  [Страница Главная]\n");
    *m_outputStream << "  main.heating.on                 - Heating Selector On\n";
    *m_outputStream << "  main.heating.off                - Heating Selector Off\n";
    *m_outputStream << "  main.generator.on               - Generator Selector On\n";
    *m_outputStream << "  main.generator.off              - Generator Selector Off\n";
    *m_outputStream << "  main.upper.start               - Upper Spindle Start\n";
    *m_outputStream << "  main.upper.stop                - Upper Spindle Stop\n";
    *m_outputStream << "  main.lower.start               - Lower Spindle Start\n";
    *m_outputStream << "  main.lower.stop                - Lower Spindle Stop\n";
    *m_outputStream << "\n";
    *m_outputStream << "Settings Page Control commands:\n";
    *m_outputStream << QString::fromUtf8("  [Страница Настроек]\n");
    *m_outputStream << "  settings.heating.on                 - Settings Heating Selector On\n";
    *m_outputStream << "  settings.heating.off                - Settings Heating Selector Off\n";
    *m_outputStream << "  settings.generator.on               - Settings Generator Selector On\n";
    *m_outputStream << "  settings.generator.off              - Settings Generator Selector Off\n";
    *m_outputStream << "  settings.generator.status.ok        - Set Generator Status: OK\n";
    *m_outputStream << "  settings.generator.status.error     - Set Generator Status: Error\n";
    *m_outputStream << "  settings.upper.start                - Settings Upper Spindle Start\n";
    *m_outputStream << "  settings.upper.stop                 - Settings Upper Spindle Stop\n";
    *m_outputStream << "  settings.lower.start                - Settings Lower Spindle Start\n";
    *m_outputStream << "  settings.lower.stop                 - Settings Lower Spindle Stop\n";
    *m_outputStream << "\n";
    *m_outputStream << "Vacuum Control commands:\n";
    *m_outputStream << QString::fromUtf8("  [Страница Вакуум]\n");
    *m_outputStream << "  vacuum.pump.on                  - Vacuum Pump On\n";
    *m_outputStream << "  vacuum.pump.off                  - Vacuum Pump Off\n";
    *m_outputStream << "  vacuum.valve.on                  - Vacuum Valve On\n";
    *m_outputStream << "  vacuum.valve.off                 - Vacuum Valve Off\n";
    *m_outputStream << "  vacuum.autopump.on              - Auto Pump Down On\n";
    *m_outputStream << "  vacuum.autopump.off              - Auto Pump Down Off\n";
    *m_outputStream << "  vacuum.upperdoor.open            - Upper Door Open\n";
    *m_outputStream << "  vacuum.upperdoor.close           - Upper Door Close\n";
    *m_outputStream << "  vacuum.lowerdoor.open            - Lower Door Open\n";
    *m_outputStream << "  vacuum.lowerdoor.close           - Lower Door Close\n";
    *m_outputStream << "  vacuum.maindoor.open             - Main Door Status: Open\n";
    *m_outputStream << "  vacuum.maindoor.close            - Main Door Status: Close\n";
    *m_outputStream << "  vacuum.lighting.0 <0|1>         - Lighting Button 0 (top) On/Off\n";
    *m_outputStream << "  vacuum.lighting.1 <0|1>         - Lighting Button 1 (left) On/Off\n";
    *m_outputStream << "  vacuum.lighting.2 <0|1>          - Lighting Button 2 (right) On/Off\n";
    *m_outputStream << "  vacuum.lighting.3 <0|1>          - Lighting Button 3 (bottom) On/Off\n";
    *m_outputStream << "\n";
    *m_outputStream << "Status commands:\n";
    *m_outputStream << QString::fromUtf8("  [Все страницы]\n");
    *m_outputStream << "  status.connection.connecting   - Set connection status: Connecting\n";
    *m_outputStream << "  status.connection.disconnected  - Set connection status: Disconnected\n";
    *m_outputStream << "  status.connection.connected      - Set connection status: Connected\n";
    *m_outputStream << QString::fromUtf8("  [Страница Вакуум]\n");
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
        Values::updateUpperSpindleStartStop(true);
        *m_outputStream << "Executed: Upper Spindle Start" << endl;
        return true;
    }
    if (cmd == "activity.upper.stop") {
        Activity::handleUpperSpindleStartStop(false);
        Values::updateUpperSpindleStartStop(false);
        *m_outputStream << "Executed: Upper Spindle Stop" << endl;
        return true;
    }
    if (cmd == "activity.lower.start") {
        Activity::handleLowerSpindleStartStop(true);
        Values::updateLowerSpindleStartStop(true);
        *m_outputStream << "Executed: Lower Spindle Start" << endl;
        return true;
    }
    if (cmd == "activity.lower.stop") {
        Activity::handleLowerSpindleStartStop(false);
        Values::updateLowerSpindleStartStop(false);
        *m_outputStream << "Executed: Lower Spindle Stop" << endl;
        return true;
    }
    if (cmd == "activity.heating.on") {
        Activity::handleHeatingStateChanged(1);
        Values::updateHeatingSelector(true);
        *m_outputStream << "Executed: Heating On" << endl;
        return true;
    }
    if (cmd == "activity.heating.off") {
        Activity::handleHeatingStateChanged(0);
        Values::updateHeatingSelector(false);
        *m_outputStream << "Executed: Heating Off" << endl;
        return true;
    }
    if (cmd == "activity.generator.on") {
        Activity::handleGeneratorStateChanged(1);
        Values::updateGeneratorSelector(true);
        *m_outputStream << "Executed: Generator On" << endl;
        return true;
    }
    if (cmd == "activity.generator.off") {
        Activity::handleGeneratorStateChanged(0);
        Values::updateGeneratorSelector(false);
        *m_outputStream << "Executed: Generator Off" << endl;
        return true;
    }
    if (cmd == "activity.generator.reset") {
        Activity::handleGeneratorReset();
        *m_outputStream << "Executed: Generator Reset" << endl;
        return true;
    }
    // Activity commands - Settings Page
    if (cmd == "activity.settings.xy.up") {
        Activity::handleSettingsXYUp();
        *m_outputStream << "Executed: Settings XY Up" << endl;
        return true;
    }
    if (cmd == "activity.settings.xy.down") {
        Activity::handleSettingsXYDown();
        *m_outputStream << "Executed: Settings XY Down" << endl;
        return true;
    }
    if (cmd == "activity.settings.xy.left") {
        Activity::handleSettingsXYLeft();
        *m_outputStream << "Executed: Settings XY Left" << endl;
        return true;
    }
    if (cmd == "activity.settings.xy.right") {
        Activity::handleSettingsXYRight();
        *m_outputStream << "Executed: Settings XY Right" << endl;
        return true;
    }
    if (cmd == "activity.settings.coil.up") {
        Activity::handleSettingsCoilUp();
        *m_outputStream << "Executed: Settings Coil Up" << endl;
        return true;
    }
    if (cmd == "activity.settings.coil.down") {
        Activity::handleSettingsCoilDown();
        *m_outputStream << "Executed: Settings Coil Down" << endl;
        return true;
    }
    if (cmd == "activity.settings.coil.neutral") {
        Activity::handleSettingsCoilNeutral();
        *m_outputStream << "Executed: Settings Coil Neutral" << endl;
        return true;
    }
    if (cmd == "activity.settings.coil.acceleration") {
        Activity::handleSettingsCoilAcceleration();
        *m_outputStream << "Executed: Settings Coil Acceleration" << endl;
        return true;
    }
    if (cmd == "activity.settings.coil.service") {
        Activity::handleSettingsCoilService();
        *m_outputStream << "Executed: Settings Coil Service" << endl;
        return true;
    }
    if (cmd == "activity.settings.upper.start") {
        Activity::handleSettingsUpperSpindleStartStop(true);
        Values::updateSettingsUpperSpindleStartStop(true);
        *m_outputStream << "Executed: Settings Upper Spindle Start" << endl;
        return true;
    }
    if (cmd == "activity.settings.upper.stop") {
        Activity::handleSettingsUpperSpindleStartStop(false);
        Values::updateSettingsUpperSpindleStartStop(false);
        *m_outputStream << "Executed: Settings Upper Spindle Stop" << endl;
        return true;
    }
    if (cmd == "activity.settings.lower.start") {
        Activity::handleSettingsLowerSpindleStartStop(true);
        Values::updateSettingsLowerSpindleStartStop(true);
        *m_outputStream << "Executed: Settings Lower Spindle Start" << endl;
        return true;
    }
    if (cmd == "activity.settings.lower.stop") {
        Activity::handleSettingsLowerSpindleStartStop(false);
        Values::updateSettingsLowerSpindleStartStop(false);
        *m_outputStream << "Executed: Settings Lower Spindle Stop" << endl;
        return true;
    }
    if (cmd == "activity.settings.heating.on") {
        Activity::handleSettingsHeatingStateChanged(1);
        Values::updateSettingsHeatingSelector(true);
        *m_outputStream << "Executed: Settings Heating On" << endl;
        return true;
    }
    if (cmd == "activity.settings.heating.off") {
        Activity::handleSettingsHeatingStateChanged(0);
        Values::updateSettingsHeatingSelector(false);
        *m_outputStream << "Executed: Settings Heating Off" << endl;
        return true;
    }
    if (cmd == "activity.settings.generator.on") {
        Activity::handleSettingsGeneratorStateChanged(1);
        Values::updateSettingsGeneratorSelector(true);
        *m_outputStream << "Executed: Settings Generator On" << endl;
        return true;
    }
    if (cmd == "activity.settings.generator.off") {
        Activity::handleSettingsGeneratorStateChanged(0);
        Values::updateSettingsGeneratorSelector(false);
        *m_outputStream << "Executed: Settings Generator Off" << endl;
        return true;
    }
    if (cmd == "activity.settings.generator.reset") {
        Activity::handleSettingsGeneratorReset();
        *m_outputStream << "Executed: Settings Generator Reset" << endl;
        return true;
    }
    if (cmd == "activity.settings.reflector.up") {
        Activity::handleSettingsReflectorUp();
        *m_outputStream << "Executed: Settings Reflector Up" << endl;
        return true;
    }
    if (cmd == "activity.settings.reflector.down") {
        Activity::handleSettingsReflectorDown();
        *m_outputStream << "Executed: Settings Reflector Down" << endl;
        return true;
    }
    if (cmd == "activity.settings.lighting.all") {
        Activity::handleSettingsLightingMode(0);
        *m_outputStream << "Executed: Settings Lighting Mode - All" << endl;
        return true;
    }
    if (cmd == "activity.settings.lighting.half") {
        Activity::handleSettingsLightingMode(1);
        *m_outputStream << "Executed: Settings Lighting Mode - 50%" << endl;
        return true;
    }
    if (cmd == "activity.settings.nitrogen.open") {
        Activity::handleSettingsNitrogenValveOpen();
        *m_outputStream << "Executed: Settings Nitrogen Valve Open" << endl;
        return true;
    }
    if (cmd == "activity.settings.autodope") {
        Activity::handleSettingsAutodope();
        *m_outputStream << "Executed: Settings Autodope" << endl;
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
        // Settings Page values
        if (cmd == "values.set.settings.xy.x") {
            Values::updateSettingsXYOffsetX(value);
            *m_outputStream << "Set Settings XY Offset X = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.xy.y") {
            Values::updateSettingsXYOffsetY(value);
            *m_outputStream << "Set Settings XY Offset Y = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.coil.offset") {
            Values::updateSettingsCoilOffset(value);
            *m_outputStream << "Set Settings Coil Offset = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.coil.osc") {
            Values::updateSettingsCoilOscillations(value);
            *m_outputStream << "Set Settings Coil Oscillations = " << value << " MM/min" << endl;
            return true;
        }
        if (cmd == "values.set.settings.upper.x") {
            Values::updateSettingsUpperSpindleXOffset(value);
            *m_outputStream << "Set Settings Upper Spindle X Offset = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.upper.speed") {
            Values::updateSettingsUpperSpindleSpeed(value);
            *m_outputStream << "Set Settings Upper Spindle Speed = " << value << " MM/min" << endl;
            return true;
        }
        if (cmd == "values.set.settings.upper.pos") {
            Values::updateSettingsUpperSpindlePosition(value);
            *m_outputStream << "Set Settings Upper Spindle Position = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.upper.rpm") {
            Values::updateSettingsUpperSpindleRpm(value);
            *m_outputStream << "Set Settings Upper Spindle RPM = " << value << " RPM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.upper.alarm") {
            Values::updateSettingsUpperSpindleAlarm(value);
            *m_outputStream << "Set Settings Upper Spindle Alarm = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.lower.x") {
            Values::updateSettingsLowerSpindleXOffset(value);
            *m_outputStream << "Set Settings Lower Spindle X Offset = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.lower.speed") {
            Values::updateSettingsLowerSpindleSpeed(value);
            *m_outputStream << "Set Settings Lower Spindle Speed = " << value << " MM/min" << endl;
            return true;
        }
        if (cmd == "values.set.settings.lower.pos") {
            Values::updateSettingsLowerSpindlePosition(value);
            *m_outputStream << "Set Settings Lower Spindle Position = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.lower.alarm") {
            Values::updateSettingsLowerSpindleAlarm(value);
            *m_outputStream << "Set Settings Lower Spindle Alarm = " << value << " MM" << endl;
            return true;
        }
        if (cmd == "values.set.settings.grid.amp") {
            Values::updateSettingsGridAmp(value);
            *m_outputStream << "Set Settings Grid AMP = " << value << " AMP" << endl;
            return true;
        }
        if (cmd == "values.set.settings.p") {
            Values::updateSettingsPValue(value);
            *m_outputStream << "Set Settings P Value = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.settings.i") {
            Values::updateSettingsIValue(value);
            *m_outputStream << "Set Settings I Value = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.settings.u") {
            Values::updateSettingsUValue(value);
            *m_outputStream << "Set Settings U Value = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.settings.generator") {
            Values::updateSettingsGeneratorPercent(value);
            *m_outputStream << "Set Settings Generator Percent = " << value << " %" << endl;
            return true;
        }
        if (cmd == "values.set.settings.generator.status") {
            int intVal = static_cast<int>(value);
            if (intVal != 0 && intVal != 1) {
                *m_outputStream << "Error: Invalid value. Use 0 (Error) or 1 (OK)" << endl;
                return true;
            }
            Values::updateSettingsGeneratorStatus(intVal == 1);
            *m_outputStream << "Set Settings Generator Status = " << (intVal == 1 ? "OK" : "Error") << endl;
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
    
    // Main Page Control commands
    if (cmd == "main.heating.on") {
        Values::updateHeatingSelector(true);
        *m_outputStream << "Main Page: Heating Selector: On" << endl;
        return true;
    }
    if (cmd == "main.heating.off") {
        Values::updateHeatingSelector(false);
        *m_outputStream << "Main Page: Heating Selector: Off" << endl;
        return true;
    }
    if (cmd == "main.generator.on") {
        Values::updateGeneratorSelector(true);
        *m_outputStream << "Main Page: Generator Selector: On" << endl;
        return true;
    }
    if (cmd == "main.generator.off") {
        Values::updateGeneratorSelector(false);
        *m_outputStream << "Main Page: Generator Selector: Off" << endl;
        return true;
    }
    if (cmd == "main.upper.start") {
        Values::updateUpperSpindleStartStop(true);
        Activity::handleUpperSpindleStartStop(true);
        *m_outputStream << "Main Page: Upper Spindle: Start" << endl;
        return true;
    }
    if (cmd == "main.upper.stop") {
        Values::updateUpperSpindleStartStop(false);
        Activity::handleUpperSpindleStartStop(false);
        *m_outputStream << "Main Page: Upper Spindle: Stop" << endl;
        return true;
    }
    if (cmd == "main.lower.start") {
        Values::updateLowerSpindleStartStop(true);
        Activity::handleLowerSpindleStartStop(true);
        *m_outputStream << "Main Page: Lower Spindle: Start" << endl;
        return true;
    }
    if (cmd == "main.lower.stop") {
        Values::updateLowerSpindleStartStop(false);
        Activity::handleLowerSpindleStartStop(false);
        *m_outputStream << "Main Page: Lower Spindle: Stop" << endl;
        return true;
    }
    // Settings Page Control commands
    if (cmd == "settings.heating.on") {
        Values::updateSettingsHeatingSelector(true);
        *m_outputStream << "Settings Page: Heating Selector: On" << endl;
        return true;
    }
    if (cmd == "settings.heating.off") {
        Values::updateSettingsHeatingSelector(false);
        *m_outputStream << "Settings Page: Heating Selector: Off" << endl;
        return true;
    }
    if (cmd == "settings.generator.on") {
        Values::updateSettingsGeneratorSelector(true);
        *m_outputStream << "Settings Page: Generator Selector: On" << endl;
        return true;
    }
    if (cmd == "settings.generator.off") {
        Values::updateSettingsGeneratorSelector(false);
        *m_outputStream << "Settings Page: Generator Selector: Off" << endl;
        return true;
    }
    if (cmd == "settings.generator.status.ok") {
        Values::updateSettingsGeneratorStatus(true);
        *m_outputStream << "Settings Page: Generator Status: OK" << endl;
        return true;
    }
    if (cmd == "settings.generator.status.error") {
        Values::updateSettingsGeneratorStatus(false);
        *m_outputStream << "Settings Page: Generator Status: Error" << endl;
        return true;
    }
    if (cmd == "settings.upper.start") {
        Values::updateSettingsUpperSpindleStartStop(true);
        Activity::handleSettingsUpperSpindleStartStop(true);
        *m_outputStream << "Settings Page: Upper Spindle: Start" << endl;
        return true;
    }
    if (cmd == "settings.upper.stop") {
        Values::updateSettingsUpperSpindleStartStop(false);
        Activity::handleSettingsUpperSpindleStartStop(false);
        *m_outputStream << "Settings Page: Upper Spindle: Stop" << endl;
        return true;
    }
    if (cmd == "settings.lower.start") {
        Values::updateSettingsLowerSpindleStartStop(true);
        Activity::handleSettingsLowerSpindleStartStop(true);
        *m_outputStream << "Settings Page: Lower Spindle: Start" << endl;
        return true;
    }
    if (cmd == "settings.lower.stop") {
        Values::updateSettingsLowerSpindleStartStop(false);
        Activity::handleSettingsLowerSpindleStartStop(false);
        *m_outputStream << "Settings Page: Lower Spindle: Stop" << endl;
        return true;
    }
    
    // Vacuum Control commands
    if (cmd == "vacuum.pump.on") {
        Values::updateVacuumPumpSelector(true);
        *m_outputStream << "Vacuum Pump: On" << endl;
        return true;
    }
    if (cmd == "vacuum.pump.off") {
        Values::updateVacuumPumpSelector(false);
        *m_outputStream << "Vacuum Pump: Off" << endl;
        return true;
    }
    if (cmd == "vacuum.valve.on") {
        Values::updateVacuumValveSelector(true);
        *m_outputStream << "Vacuum Valve: On" << endl;
        return true;
    }
    if (cmd == "vacuum.valve.off") {
        Values::updateVacuumValveSelector(false);
        *m_outputStream << "Vacuum Valve: Off" << endl;
        return true;
    }
    if (cmd == "vacuum.autopump.on") {
        Values::updateAutoPumpDownSelector(true);
        *m_outputStream << "Auto Pump Down: On" << endl;
        return true;
    }
    if (cmd == "vacuum.autopump.off") {
        Values::updateAutoPumpDownSelector(false);
        *m_outputStream << "Auto Pump Down: Off" << endl;
        return true;
    }
    if (cmd == "vacuum.upperdoor.open") {
        Values::updateUpperDoorSelector(true);
        *m_outputStream << "Upper Door: Open" << endl;
        return true;
    }
    if (cmd == "vacuum.upperdoor.close") {
        Values::updateUpperDoorSelector(false);
        *m_outputStream << "Upper Door: Close" << endl;
        return true;
    }
    if (cmd == "vacuum.lowerdoor.open") {
        Values::updateLowerDoorSelector(true);
        *m_outputStream << "Lower Door: Open" << endl;
        return true;
    }
    if (cmd == "vacuum.lowerdoor.close") {
        Values::updateLowerDoorSelector(false);
        *m_outputStream << "Lower Door: Close" << endl;
        return true;
    }
    if (cmd == "vacuum.maindoor.open") {
        Values::updateMainDoorStatus(true);
        *m_outputStream << "Main Door Status: Open" << endl;
        return true;
    }
    if (cmd == "vacuum.maindoor.close") {
        Values::updateMainDoorStatus(false);
        *m_outputStream << "Main Door Status: Close" << endl;
        return true;
    }
    if (cmd.startsWith("vacuum.lighting.")) {
        QStringList parts = command.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            QStringList cmdParts = parts[0].split('.');
            if (cmdParts.size() >= 3) {
                bool ok;
                int index = cmdParts[2].toInt(&ok);
                if (ok && index >= 0 && index < 4) {
                    int state = parts[1].toInt(&ok);
                    if (ok && (state == 0 || state == 1)) {
                        Values::updateLightingButton(index, state == 1);
                        *m_outputStream << "Lighting Button " << index << ": " << (state == 1 ? "On" : "Off") << endl;
                        return true;
                    } else {
                        *m_outputStream << "Error: Invalid state. Use 0 (Off) or 1 (On)" << endl;
                        return true;
                    }
                } else {
                    *m_outputStream << "Error: Invalid button index. Use 0-3" << endl;
                    return true;
                }
            }
        } else {
            *m_outputStream << "Error: State required. Usage: " << cmd << " <0|1>" << endl;
            return true;
        }
    }
    
    return false;
}
