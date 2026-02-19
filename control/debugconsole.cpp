#include "control/debugconsole.h"
#include "activity.h"
#include "errorlogio.h"
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
        *m_outputStream << "> " << Qt::flush;
        
        // Ждем ввода данных (блокирующий вызов)
        line = m_inputStream->readLine();
        
        // Проверяем, не был ли поток остановлен
        if (!m_running)
            break;
        
        line = line.trimmed();
        
        if (line.isEmpty())
            continue;
        
        if (line.toLower() == "quit" || line.toLower() == "exit") {
            writeLine(QString::fromUtf8("Exiting debug console..."));
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

void DebugConsole::writeLine(const QString &text)
{
    if (m_outputStream) {
        *m_outputStream << text << Qt::endl;
        m_outputStream->flush();
    }
    ErrorLogIO::appendToConsoleLog(text);
}

void DebugConsole::printWelcome()
{
    writeLine(QString());
    writeLine(QString::fromUtf8("========================================"));
    writeLine(QString::fromUtf8("  Control Thread Debug Console"));
    writeLine(QString::fromUtf8("========================================"));
    writeLine(QString::fromUtf8("Type 'help' for available commands"));
    writeLine(QString::fromUtf8("Type 'quit' or 'exit' to exit"));
    writeLine(QString());
}

void DebugConsole::printHelp()
{
    writeLine(QString());
    writeLine(QString::fromUtf8("Available commands:"));
    writeLine(QString::fromUtf8("  help                          - Show this help"));
    writeLine(QString::fromUtf8("  quit/exit                     - Exit console"));
    writeLine(QString());
    writeLine(QString::fromUtf8("Activity commands:"));
    writeLine(QString::fromUtf8("  [Главная страница]"));
    writeLine(QString::fromUtf8("  activity.xy.up                - XY Up"));
    writeLine(QString::fromUtf8("  activity.xy.down              - XY Down"));
    writeLine(QString::fromUtf8("  activity.xy.left              - XY Left"));
    writeLine(QString::fromUtf8("  activity.xy.right              - XY Right"));
    writeLine(QString::fromUtf8("  activity.coil.up              - Coil Up"));
    writeLine(QString::fromUtf8("  activity.coil.down          - Coil Down"));
    writeLine(QString::fromUtf8("  activity.coil.start           - Start Oscillations"));
    writeLine(QString::fromUtf8("  activity.upper.start          - Upper Spindle Start"));
    writeLine(QString::fromUtf8("  activity.upper.stop           - Upper Spindle Stop"));
    writeLine(QString::fromUtf8("  activity.lower.start          - Lower Spindle Start"));
    writeLine(QString::fromUtf8("  activity.lower.stop           - Lower Spindle Stop"));
    writeLine(QString::fromUtf8("  activity.heating.on            - Heating On"));
    writeLine(QString::fromUtf8("  activity.heating.off          - Heating Off"));
    writeLine(QString::fromUtf8("  activity.generator.on         - Generator On"));
    writeLine(QString::fromUtf8("  activity.generator.off         - Generator Off"));
    writeLine(QString::fromUtf8("  activity.generator.reset      - Generator Reset"));
    writeLine(QString::fromUtf8("  [Страница Настроек]"));
    writeLine(QString::fromUtf8("  activity.settings.xy.up                - Settings XY Up"));
    writeLine(QString::fromUtf8("  activity.settings.xy.down              - Settings XY Down"));
    writeLine(QString::fromUtf8("  activity.settings.xy.left              - Settings XY Left"));
    writeLine(QString::fromUtf8("  activity.settings.xy.right             - Settings XY Right"));
    writeLine(QString::fromUtf8("  activity.settings.coil.up              - Settings Coil Up"));
    writeLine(QString::fromUtf8("  activity.settings.coil.down            - Settings Coil Down"));
    writeLine(QString::fromUtf8("  activity.settings.coil.neutral          - Settings Coil Neutral"));
    writeLine(QString::fromUtf8("  activity.settings.coil.acceleration    - Settings Coil Acceleration"));
    writeLine(QString::fromUtf8("  activity.settings.coil.service         - Settings Coil Service"));
    writeLine(QString::fromUtf8("  activity.settings.upper.start          - Settings Upper Spindle Start"));
    writeLine(QString::fromUtf8("  activity.settings.upper.stop           - Settings Upper Spindle Stop"));
    writeLine(QString::fromUtf8("  activity.settings.lower.start          - Settings Lower Spindle Start"));
    writeLine(QString::fromUtf8("  activity.settings.lower.stop           - Settings Lower Spindle Stop"));
    writeLine(QString::fromUtf8("  activity.settings.heating.on           - Settings Heating On"));
    writeLine(QString::fromUtf8("  activity.settings.heating.off          - Settings Heating Off"));
    writeLine(QString::fromUtf8("  activity.settings.generator.on         - Settings Generator On"));
    writeLine(QString::fromUtf8("  activity.settings.generator.off        - Settings Generator Off"));
    writeLine(QString::fromUtf8("  activity.settings.generator.reset      - Settings Generator Reset"));
    writeLine(QString::fromUtf8("  activity.settings.reflector.up         - Settings Reflector Up"));
    writeLine(QString::fromUtf8("  activity.settings.reflector.down       - Settings Reflector Down"));
    writeLine(QString::fromUtf8("  activity.settings.lighting.all         - Settings Lighting All"));
    writeLine(QString::fromUtf8("  activity.settings.lighting.half        - Settings Lighting 50%"));
    writeLine(QString::fromUtf8("  activity.settings.nitrogen.open        - Settings Nitrogen Valve Open"));
    writeLine(QString::fromUtf8("  activity.settings.autodope             - Settings Autodope"));
    writeLine(QString::fromUtf8("  [Страница Легирования]"));
    writeLine(QString::fromUtf8("  activity.argon.start          - Argon Start"));
    writeLine(QString::fromUtf8("  activity.argon.stop            - Argon Stop"));
    writeLine(QString::fromUtf8("  activity.injection.start       - Injection Start"));
    writeLine(QString::fromUtf8("  activity.injection.stop       - Injection Stop"));
    writeLine(QString::fromUtf8(""));
    writeLine(QString::fromUtf8("Values commands:"));
    writeLine(QString::fromUtf8("  [Главная страница]"));
    writeLine(QString::fromUtf8("  values.set.xy.x <value>       - Set XY Offset X (MM)"));
    writeLine(QString::fromUtf8("  values.set.xy.y <value>       - Set XY Offset Y (MM)"));
    writeLine(QString::fromUtf8("  values.set.coil.offset <value> - Set Coil Offset (MM)"));
    writeLine(QString::fromUtf8("  values.set.coil.osc <value>   - Set Coil Oscillations (MM/min)"));
    writeLine(QString::fromUtf8("  values.set.upper.x <value>     - Set Upper Spindle X Offset (MM)"));
    writeLine(QString::fromUtf8("  values.set.upper.speed <value> - Set Upper Spindle Speed (MM/min)"));
    writeLine(QString::fromUtf8("  values.set.upper.pos <value>   - Set Upper Spindle Position (MM)"));
    writeLine(QString::fromUtf8("  values.set.lower.x <value>     - Set Lower Spindle X Offset (MM)"));
    writeLine(QString::fromUtf8("  values.set.lower.speed <value> - Set Lower Spindle Speed (MM/min)"));
    writeLine(QString::fromUtf8("  values.set.lower.pos <value>   - Set Lower Spindle Position (MM)"));
    writeLine(QString::fromUtf8("  values.set.grid.amp <value>    - Set Grid AMP"));
    writeLine(QString::fromUtf8("  values.set.p <value>           - Set P Value (%)"));
    writeLine(QString::fromUtf8("  values.set.i <value>           - Set I Value (%)"));
    writeLine(QString::fromUtf8("  values.set.u <value>           - Set U Value (%)"));
    writeLine(QString::fromUtf8("  values.set.generator <value>    - Set Generator Percent (%)"));
    writeLine(QString::fromUtf8("  [Страница Настроек]"));
    writeLine(QString::fromUtf8("  values.set.settings.xy.x <value>        - Set Settings XY Offset X (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.xy.y <value>        - Set Settings XY Offset Y (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.coil.offset <value> - Set Settings Coil Offset (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.coil.osc <value>    - Set Settings Coil Oscillations (MM/min)"));
    writeLine(QString::fromUtf8("  values.set.settings.upper.x <value>     - Set Settings Upper Spindle X Offset (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.upper.speed <value> - Set Settings Upper Spindle Speed (MM/min)"));
    writeLine(QString::fromUtf8("  values.set.settings.upper.pos <value>   - Set Settings Upper Spindle Position (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.lower.x <value>     - Set Settings Lower Spindle X Offset (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.lower.speed <value> - Set Settings Lower Spindle Speed (MM/min)"));
    writeLine(QString::fromUtf8("  values.set.settings.lower.pos <value>   - Set Settings Lower Spindle Position (MM)"));
    writeLine(QString::fromUtf8("  values.set.settings.grid.amp <value>    - Set Settings Grid AMP"));
    writeLine(QString::fromUtf8("  values.set.settings.p <value>           - Set Settings P Value (%)"));
    writeLine(QString::fromUtf8("  values.set.settings.i <value>           - Set Settings I Value (%)"));
    writeLine(QString::fromUtf8("  values.set.settings.u <value>           - Set Settings U Value (%)"));
    writeLine(QString::fromUtf8("  values.set.settings.generator <value>   - Set Settings Generator Percent (%)"));
    writeLine(QString::fromUtf8("  values.set.settings.generator.status <0|1> - Set Generator Status (0=Error, 1=OK)"));
    writeLine(QString::fromUtf8("  [Страница Вакуум]"));
    writeLine(QString::fromUtf8("  values.set.vacuum.pump <value>  - Set Vacuum Pump Pressure (mbar)"));
    writeLine(QString::fromUtf8("  values.set.vacuum.chamber <value> - Set Vacuum Chamber Pressure (mbar)"));
    writeLine(QString::fromUtf8("  values.set.vacuum.time <value> - Set Vacuum Pumping Time (sec)"));
    writeLine(QString::fromUtf8("  values.set.vacuum.gas <value>   - Set Vacuum Gas Pressure (bar)"));
    writeLine(QString::fromUtf8("  [Страница Контроль газа]"));
    writeLine(QString::fromUtf8("  values.set.gas.pressure.status <0|1|2> - Gas Pressure Status (0=Normal, 1=Low, 2=High)"));
    writeLine(QString::fromUtf8(""));
    writeLine(QString::fromUtf8("Main Page Control commands:"));
    writeLine(QString::fromUtf8("  [Страница Главная]"));
    writeLine(QString::fromUtf8("  main.heating.on                 - Heating Selector On"));
    writeLine(QString::fromUtf8("  main.heating.off                - Heating Selector Off"));
    writeLine(QString::fromUtf8("  main.generator.on               - Generator Selector On"));
    writeLine(QString::fromUtf8("  main.generator.off              - Generator Selector Off"));
    writeLine(QString::fromUtf8("  main.upper.start               - Upper Spindle Start"));
    writeLine(QString::fromUtf8("  main.upper.stop                - Upper Spindle Stop"));
    writeLine(QString::fromUtf8("  main.lower.start               - Lower Spindle Start"));
    writeLine(QString::fromUtf8("  main.lower.stop                - Lower Spindle Stop"));
    writeLine(QString::fromUtf8(""));
    writeLine(QString::fromUtf8("Settings Page Control commands:"));
    writeLine(QString::fromUtf8("  [Страница Настроек]"));
    writeLine(QString::fromUtf8("  settings.heating.on                 - Settings Heating Selector On"));
    writeLine(QString::fromUtf8("  settings.heating.off                - Settings Heating Selector Off"));
    writeLine(QString::fromUtf8("  settings.generator.on               - Settings Generator Selector On"));
    writeLine(QString::fromUtf8("  settings.generator.off              - Settings Generator Selector Off"));
    writeLine(QString::fromUtf8("  settings.generator.status.ok        - Set Generator Status: OK"));
    writeLine(QString::fromUtf8("  settings.generator.status.error     - Set Generator Status: Error"));
    writeLine(QString::fromUtf8("  settings.upper.start                - Settings Upper Spindle Start"));
    writeLine(QString::fromUtf8("  settings.upper.stop                 - Settings Upper Spindle Stop"));
    writeLine(QString::fromUtf8("  settings.lower.start                - Settings Lower Spindle Start"));
    writeLine(QString::fromUtf8("  settings.lower.stop                 - Settings Lower Spindle Stop"));
    writeLine(QString::fromUtf8(""));
    writeLine(QString::fromUtf8("Vacuum Control commands:"));
    writeLine(QString::fromUtf8("  [Страница Вакуум]"));
    writeLine(QString::fromUtf8("  vacuum.pump.on                  - Vacuum Pump On"));
    writeLine(QString::fromUtf8("  vacuum.pump.off                  - Vacuum Pump Off"));
    writeLine(QString::fromUtf8("  vacuum.valve.on                  - Vacuum Valve On"));
    writeLine(QString::fromUtf8("  vacuum.valve.off                 - Vacuum Valve Off"));
    writeLine(QString::fromUtf8("  vacuum.autopump.on              - Auto Pump Down On"));
    writeLine(QString::fromUtf8("  vacuum.autopump.off              - Auto Pump Down Off"));
    writeLine(QString::fromUtf8("  vacuum.upperdoor.open            - Upper Door Open"));
    writeLine(QString::fromUtf8("  vacuum.upperdoor.close           - Upper Door Close"));
    writeLine(QString::fromUtf8("  vacuum.lowerdoor.open            - Lower Door Open"));
    writeLine(QString::fromUtf8("  vacuum.lowerdoor.close           - Lower Door Close"));
    writeLine(QString::fromUtf8("  vacuum.maindoor.open             - Main Door Status: Open"));
    writeLine(QString::fromUtf8("  vacuum.maindoor.close            - Main Door Status: Close"));
    writeLine(QString::fromUtf8("  vacuum.lighting.0 <0|1>         - Lighting Button 0 (top) On/Off"));
    writeLine(QString::fromUtf8("  vacuum.lighting.1 <0|1>         - Lighting Button 1 (left) On/Off"));
    writeLine(QString::fromUtf8("  vacuum.lighting.2 <0|1>          - Lighting Button 2 (right) On/Off"));
    writeLine(QString::fromUtf8("  vacuum.lighting.3 <0|1>          - Lighting Button 3 (bottom) On/Off"));
    writeLine(QString::fromUtf8("  [Страница Контроль газа]"));
    writeLine(QString::fromUtf8("  gas.pressure.status.normal  - Gas Pressure: Normal"));
    writeLine(QString::fromUtf8("  gas.pressure.status.low    - Gas Pressure: Low"));
    writeLine(QString::fromUtf8("  gas.pressure.status.high   - Gas Pressure: High"));
    writeLine(QString::fromUtf8("\n  Журнал и архив ошибок:"));
    writeLine(QString::fromUtf8("  log.add <message> [color] [0|1]  - Add to Error Log (color e.g. #e74c3c, resettable 1/0)"));
    writeLine(QString::fromUtf8("  archive.add <message>           - Add to Error Archive"));
    writeLine(QString::fromUtf8(""));
    writeLine(QString::fromUtf8("Status commands:"));
    writeLine(QString::fromUtf8("  [Все страницы]"));
    writeLine(QString::fromUtf8("  status.connection.connecting   - Set connection status: Connecting"));
    writeLine(QString::fromUtf8("  status.connection.disconnected  - Set connection status: Disconnected"));
    writeLine(QString::fromUtf8("  status.connection.connected      - Set connection status: Connected"));
    writeLine(QString::fromUtf8("  [Страница Вакуум]"));
    writeLine(QString::fromUtf8("  status.pump.low                 - Set pump pressure: Low"));
    writeLine(QString::fromUtf8("  status.pump.lowmedium           - Set pump pressure: Low-Medium"));
    writeLine(QString::fromUtf8("  status.pump.medium               - Set pump pressure: Medium"));
    writeLine(QString::fromUtf8("  status.pump.mediumhigh          - Set pump pressure: Medium-High"));
    writeLine(QString::fromUtf8("  status.pump.high                 - Set pump pressure: High"));
    writeLine(QString::fromUtf8(""));
}

void DebugConsole::processCommand(const QString &command)
{
    if (!m_outputStream)
        return;
    
    if (!parseAndExecute(command)) {
        writeLine(QString::fromUtf8("Unknown command: ") + command);
        writeLine(QString::fromUtf8("Type 'help' for available commands"));
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
        writeLine(QString::fromUtf8("Executed: XY Up"));
        return true;
    }
    if (cmd == "activity.xy.down") {
        Activity::handleXYDown();
        writeLine(QString::fromUtf8("Executed: XY Down"));
        return true;
    }
    if (cmd == "activity.xy.left") {
        Activity::handleXYLeft();
        writeLine(QString::fromUtf8("Executed: XY Left"));
        return true;
    }
    if (cmd == "activity.xy.right") {
        Activity::handleXYRight();
        writeLine(QString::fromUtf8("Executed: XY Right"));
        return true;
    }
    if (cmd == "activity.coil.up") {
        Activity::handleCoilUp();
        writeLine(QString::fromUtf8("Executed: Coil Up"));
        return true;
    }
    if (cmd == "activity.coil.down") {
        Activity::handleCoilDown();
        writeLine(QString::fromUtf8("Executed: Coil Down"));
        return true;
    }
    if (cmd == "activity.coil.start") {
        Activity::handleStartOscillations();
        writeLine(QString::fromUtf8("Executed: Start Oscillations"));
        return true;
    }
    if (cmd == "activity.upper.start") {
        Activity::handleUpperSpindleStartStop(true);
        Values::updateUpperSpindleStartStop(true);
        writeLine(QString::fromUtf8("Executed: Upper Spindle Start"));
        return true;
    }
    if (cmd == "activity.upper.stop") {
        Activity::handleUpperSpindleStartStop(false);
        Values::updateUpperSpindleStartStop(false);
        writeLine(QString::fromUtf8("Executed: Upper Spindle Stop"));
        return true;
    }
    if (cmd == "activity.lower.start") {
        Activity::handleLowerSpindleStartStop(true);
        Values::updateLowerSpindleStartStop(true);
        writeLine(QString::fromUtf8("Executed: Lower Spindle Start"));
        return true;
    }
    if (cmd == "activity.lower.stop") {
        Activity::handleLowerSpindleStartStop(false);
        Values::updateLowerSpindleStartStop(false);
        writeLine(QString::fromUtf8("Executed: Lower Spindle Stop"));
        return true;
    }
    if (cmd == "activity.heating.on") {
        Activity::handleHeatingStateChanged(1);
        Values::updateHeatingSelector(true);
        writeLine(QString::fromUtf8("Executed: Heating On"));
        return true;
    }
    if (cmd == "activity.heating.off") {
        Activity::handleHeatingStateChanged(0);
        Values::updateHeatingSelector(false);
        writeLine(QString::fromUtf8("Executed: Heating Off"));
        return true;
    }
    if (cmd == "activity.generator.on") {
        Activity::handleGeneratorStateChanged(1);
        Values::updateGeneratorSelector(true);
        writeLine(QString::fromUtf8("Executed: Generator On"));
        return true;
    }
    if (cmd == "activity.generator.off") {
        Activity::handleGeneratorStateChanged(0);
        Values::updateGeneratorSelector(false);
        writeLine(QString::fromUtf8("Executed: Generator Off"));
        return true;
    }
    if (cmd == "activity.generator.reset") {
        Activity::handleGeneratorReset();
        writeLine(QString::fromUtf8("Executed: Generator Reset"));
        return true;
    }
    // Activity commands - Settings Page
    if (cmd == "activity.settings.xy.up") {
        Activity::handleSettingsXYUp();
        writeLine(QString::fromUtf8("Executed: Settings XY Up"));
        return true;
    }
    if (cmd == "activity.settings.xy.down") {
        Activity::handleSettingsXYDown();
        writeLine(QString::fromUtf8("Executed: Settings XY Down"));
        return true;
    }
    if (cmd == "activity.settings.xy.left") {
        Activity::handleSettingsXYLeft();
        writeLine(QString::fromUtf8("Executed: Settings XY Left"));
        return true;
    }
    if (cmd == "activity.settings.xy.right") {
        Activity::handleSettingsXYRight();
        writeLine(QString::fromUtf8("Executed: Settings XY Right"));
        return true;
    }
    if (cmd == "activity.settings.coil.up") {
        Activity::handleSettingsCoilUp();
        writeLine(QString::fromUtf8("Executed: Settings Coil Up"));
        return true;
    }
    if (cmd == "activity.settings.coil.down") {
        Activity::handleSettingsCoilDown();
        writeLine(QString::fromUtf8("Executed: Settings Coil Down"));
        return true;
    }
    if (cmd == "activity.settings.coil.neutral") {
        Activity::handleSettingsCoilNeutral();
        writeLine(QString::fromUtf8("Executed: Settings Coil Neutral"));
        return true;
    }
    if (cmd == "activity.settings.coil.acceleration") {
        Activity::handleSettingsCoilAcceleration();
        writeLine(QString::fromUtf8("Executed: Settings Coil Acceleration"));
        return true;
    }
    if (cmd == "activity.settings.coil.service") {
        Activity::handleSettingsCoilService();
        writeLine(QString::fromUtf8("Executed: Settings Coil Service"));
        return true;
    }
    if (cmd == "activity.settings.upper.start") {
        Activity::handleSettingsUpperSpindleStartStop(true);
        Values::updateSettingsUpperSpindleStartStop(true);
        writeLine(QString::fromUtf8("Executed: Settings Upper Spindle Start"));
        return true;
    }
    if (cmd == "activity.settings.upper.stop") {
        Activity::handleSettingsUpperSpindleStartStop(false);
        Values::updateSettingsUpperSpindleStartStop(false);
        writeLine(QString::fromUtf8("Executed: Settings Upper Spindle Stop"));
        return true;
    }
    if (cmd == "activity.settings.lower.start") {
        Activity::handleSettingsLowerSpindleStartStop(true);
        Values::updateSettingsLowerSpindleStartStop(true);
        writeLine(QString::fromUtf8("Executed: Settings Lower Spindle Start"));
        return true;
    }
    if (cmd == "activity.settings.lower.stop") {
        Activity::handleSettingsLowerSpindleStartStop(false);
        Values::updateSettingsLowerSpindleStartStop(false);
        writeLine(QString::fromUtf8("Executed: Settings Lower Spindle Stop"));
        return true;
    }
    if (cmd == "activity.settings.heating.on") {
        Activity::handleSettingsHeatingStateChanged(1);
        Values::updateSettingsHeatingSelector(true);
        writeLine(QString::fromUtf8("Executed: Settings Heating On"));
        return true;
    }
    if (cmd == "activity.settings.heating.off") {
        Activity::handleSettingsHeatingStateChanged(0);
        Values::updateSettingsHeatingSelector(false);
        writeLine(QString::fromUtf8("Executed: Settings Heating Off"));
        return true;
    }
    if (cmd == "activity.settings.generator.on") {
        Activity::handleSettingsGeneratorStateChanged(1);
        Values::updateSettingsGeneratorSelector(true);
        writeLine(QString::fromUtf8("Executed: Settings Generator On"));
        return true;
    }
    if (cmd == "activity.settings.generator.off") {
        Activity::handleSettingsGeneratorStateChanged(0);
        Values::updateSettingsGeneratorSelector(false);
        writeLine(QString::fromUtf8("Executed: Settings Generator Off"));
        return true;
    }
    if (cmd == "activity.settings.generator.reset") {
        Activity::handleSettingsGeneratorReset();
        writeLine(QString::fromUtf8("Executed: Settings Generator Reset"));
        return true;
    }
    if (cmd == "activity.settings.reflector.up") {
        Activity::handleSettingsReflectorUp();
        writeLine(QString::fromUtf8("Executed: Settings Reflector Up"));
        return true;
    }
    if (cmd == "activity.settings.reflector.down") {
        Activity::handleSettingsReflectorDown();
        writeLine(QString::fromUtf8("Executed: Settings Reflector Down"));
        return true;
    }
    if (cmd == "activity.settings.lighting.all") {
        Activity::handleSettingsLightingMode(0);
        writeLine(QString::fromUtf8("Executed: Settings Lighting Mode - All"));
        return true;
    }
    if (cmd == "activity.settings.lighting.half") {
        Activity::handleSettingsLightingMode(1);
        writeLine(QString::fromUtf8("Executed: Settings Lighting Mode - 50%"));
        return true;
    }
    if (cmd == "activity.settings.nitrogen.open") {
        Activity::handleSettingsNitrogenValveOpen();
        writeLine(QString::fromUtf8("Executed: Settings Nitrogen Valve Open"));
        return true;
    }
    if (cmd == "activity.settings.autodope") {
        Activity::handleSettingsAutodope();
        writeLine(QString::fromUtf8("Executed: Settings Autodope"));
        return true;
    }
    if (cmd == "activity.argon.start") {
        Activity::handleArgonStartStop(true);
        writeLine(QString::fromUtf8("Executed: Argon Start"));
        return true;
    }
    if (cmd == "activity.argon.stop") {
        Activity::handleArgonStartStop(false);
        writeLine(QString::fromUtf8("Executed: Argon Stop"));
        return true;
    }
    if (cmd == "activity.injection.start") {
        Activity::handleInjectionStartStop(true);
        writeLine(QString::fromUtf8("Executed: Injection Start"));
        return true;
    }
    if (cmd == "activity.injection.stop") {
        Activity::handleInjectionStartStop(false);
        writeLine(QString::fromUtf8("Executed: Injection Stop"));
        return true;
    }
    
    // Values commands
    if (cmd.startsWith("values.set.")) {
        if (parts.size() < 2) {
            writeLine(QString::fromUtf8("Error: Value required. Usage: ") + cmd + QString::fromUtf8(" <value>"));
            return true;
        }
        
        bool ok;
        double value = parts[1].toDouble(&ok);
        if (!ok) {
            writeLine(QString::fromUtf8("Error: Invalid value: ") + parts[1]);
            return true;
        }
        
        if (cmd == "values.set.xy.x") {
            Values::updateXYOffsetX(value);
            writeLine(QString::fromUtf8("Set XY Offset X = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.xy.y") {
            Values::updateXYOffsetY(value);
            writeLine(QString::fromUtf8("Set XY Offset Y = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.coil.offset") {
            Values::updateCoilOffset(value);
            writeLine(QString::fromUtf8("Set Coil Offset = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.coil.osc") {
            Values::updateCoilOscillations(value);
            writeLine(QString::fromUtf8("Set Coil Oscillations = ") + value + QString::fromUtf8(" MM/min"));
            return true;
        }
        if (cmd == "values.set.upper.x") {
            Values::updateUpperSpindleXOffset(value);
            writeLine(QString::fromUtf8("Set Upper Spindle X Offset = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.upper.speed") {
            Values::updateUpperSpindleSpeed(value);
            writeLine(QString::fromUtf8("Set Upper Spindle Speed = ") + value + QString::fromUtf8(" MM/min"));
            return true;
        }
        if (cmd == "values.set.upper.pos") {
            Values::updateUpperSpindlePosition(value);
            writeLine(QString::fromUtf8("Set Upper Spindle Position = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.lower.x") {
            Values::updateLowerSpindleXOffset(value);
            writeLine(QString::fromUtf8("Set Lower Spindle X Offset = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.lower.speed") {
            Values::updateLowerSpindleSpeed(value);
            writeLine(QString::fromUtf8("Set Lower Spindle Speed = ") + value + QString::fromUtf8(" MM/min"));
            return true;
        }
        if (cmd == "values.set.lower.pos") {
            Values::updateLowerSpindlePosition(value);
            writeLine(QString::fromUtf8("Set Lower Spindle Position = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.grid.amp") {
            Values::updateGridAmp(value);
            writeLine(QString::fromUtf8("Set Grid AMP = ") + value + QString::fromUtf8(" AMP"));
            return true;
        }
        if (cmd == "values.set.p") {
            Values::updatePValue(value);
            writeLine(QString::fromUtf8("Set P Value = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.i") {
            Values::updateIValue(value);
            writeLine(QString::fromUtf8("Set I Value = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.u") {
            Values::updateUValue(value);
            writeLine(QString::fromUtf8("Set U Value = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.generator") {
            Values::updateGeneratorPercent(value);
            writeLine(QString::fromUtf8("Set Generator Percent = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        // Settings Page values
        if (cmd == "values.set.settings.xy.x") {
            Values::updateSettingsXYOffsetX(value);
            writeLine(QString::fromUtf8("Set Settings XY Offset X = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.xy.y") {
            Values::updateSettingsXYOffsetY(value);
            writeLine(QString::fromUtf8("Set Settings XY Offset Y = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.coil.offset") {
            Values::updateSettingsCoilOffset(value);
            writeLine(QString::fromUtf8("Set Settings Coil Offset = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.coil.osc") {
            Values::updateSettingsCoilOscillations(value);
            writeLine(QString::fromUtf8("Set Settings Coil Oscillations = ") + value + QString::fromUtf8(" MM/min"));
            return true;
        }
        if (cmd == "values.set.settings.upper.x") {
            Values::updateSettingsUpperSpindleXOffset(value);
            writeLine(QString::fromUtf8("Set Settings Upper Spindle X Offset = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.upper.speed") {
            Values::updateSettingsUpperSpindleSpeed(value);
            writeLine(QString::fromUtf8("Set Settings Upper Spindle Speed = ") + value + QString::fromUtf8(" MM/min"));
            return true;
        }
        if (cmd == "values.set.settings.upper.pos") {
            Values::updateSettingsUpperSpindlePosition(value);
            writeLine(QString::fromUtf8("Set Settings Upper Spindle Position = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.upper.rpm") {
            Values::updateSettingsUpperSpindleRpm(value);
            writeLine(QString::fromUtf8("Set Settings Upper Spindle RPM = ") + value + QString::fromUtf8(" RPM"));
            return true;
        }
        if (cmd == "values.set.settings.upper.alarm") {
            Values::updateSettingsUpperSpindleAlarm(value);
            writeLine(QString::fromUtf8("Set Settings Upper Spindle Alarm = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.lower.x") {
            Values::updateSettingsLowerSpindleXOffset(value);
            writeLine(QString::fromUtf8("Set Settings Lower Spindle X Offset = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.lower.speed") {
            Values::updateSettingsLowerSpindleSpeed(value);
            writeLine(QString::fromUtf8("Set Settings Lower Spindle Speed = ") + value + QString::fromUtf8(" MM/min"));
            return true;
        }
        if (cmd == "values.set.settings.lower.pos") {
            Values::updateSettingsLowerSpindlePosition(value);
            writeLine(QString::fromUtf8("Set Settings Lower Spindle Position = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.lower.alarm") {
            Values::updateSettingsLowerSpindleAlarm(value);
            writeLine(QString::fromUtf8("Set Settings Lower Spindle Alarm = ") + value + QString::fromUtf8(" MM"));
            return true;
        }
        if (cmd == "values.set.settings.grid.amp") {
            Values::updateSettingsGridAmp(value);
            writeLine(QString::fromUtf8("Set Settings Grid AMP = ") + value + QString::fromUtf8(" AMP"));
            return true;
        }
        if (cmd == "values.set.settings.p") {
            Values::updateSettingsPValue(value);
            writeLine(QString::fromUtf8("Set Settings P Value = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.settings.i") {
            Values::updateSettingsIValue(value);
            writeLine(QString::fromUtf8("Set Settings I Value = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.settings.u") {
            Values::updateSettingsUValue(value);
            writeLine(QString::fromUtf8("Set Settings U Value = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.settings.generator") {
            Values::updateSettingsGeneratorPercent(value);
            writeLine(QString::fromUtf8("Set Settings Generator Percent = ") + value + QString::fromUtf8(" %"));
            return true;
        }
        if (cmd == "values.set.settings.generator.status") {
            int intVal = static_cast<int>(value);
            if (intVal != 0 && intVal != 1) {
                writeLine(QString::fromUtf8("Error: Invalid value. Use 0 (Error) or 1 (OK)"));
                return true;
            }
            Values::updateSettingsGeneratorStatus(intVal == 1);
            writeLine(QString::fromUtf8("Set Settings Generator Status = ") + (intVal == 1 ? QString::fromUtf8("OK") : QString::fromUtf8("Error")));
            return true;
        }
        if (cmd == "values.set.vacuum.pump") {
            Values::updateVacuumPumpPressure(value);
            writeLine(QString::fromUtf8("Set Vacuum Pump Pressure = ") + value + QString::fromUtf8(" mbar"));
            return true;
        }
        if (cmd == "values.set.vacuum.chamber") {
            Values::updateVacuumChamberPressure(value);
            writeLine(QString::fromUtf8("Set Vacuum Chamber Pressure = ") + value + QString::fromUtf8(" mbar"));
            return true;
        }
        if (cmd == "values.set.vacuum.time") {
            Values::updateVacuumPumpingTime(value);
            writeLine(QString::fromUtf8("Set Vacuum Pumping Time = ") + value + QString::fromUtf8(" sec"));
            return true;
        }
        if (cmd == "values.set.vacuum.gas") {
            Values::updateVacuumGasPressure(value);
            writeLine(QString::fromUtf8("Set Vacuum Gas Pressure = ") + value + QString::fromUtf8(" bar"));
            return true;
        }
        if (cmd == "values.set.gas.pressure.status") {
            int intVal = static_cast<int>(value);
            if (intVal < 0 || intVal > 2) {
                writeLine(QString::fromUtf8("Error: Invalid value. Use 0 (Normal), 1 (Low), or 2 (High)"));
                return true;
            }
            Values::updateGasPanelGasPressureStatus(intVal);
            const char* names[] = {"Normal", "Low", "High"};
            writeLine(QString::fromUtf8("Set Gas Pressure Status = ") + names[intVal]);
            return true;
        }
    }
    
    // Status commands
    if (cmd == "status.connection.connecting") {
        Values::updateConnectionStatus(Values::ConnectionStatus::Connecting);
        writeLine(QString::fromUtf8("Set connection status: Connecting"));
        return true;
    }
    if (cmd == "status.connection.disconnected") {
        Values::updateConnectionStatus(Values::ConnectionStatus::Disconnected);
        writeLine(QString::fromUtf8("Set connection status: Disconnected"));
        return true;
    }
    if (cmd == "status.connection.connected") {
        Values::updateConnectionStatus(Values::ConnectionStatus::Connected);
        writeLine(QString::fromUtf8("Set connection status: Connected"));
        return true;
    }
    if (cmd == "status.pump.low") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::Low);
        writeLine(QString::fromUtf8("Set pump pressure: Low"));
        return true;
    }
    if (cmd == "status.pump.lowmedium") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::LowMedium);
        writeLine(QString::fromUtf8("Set pump pressure: Low-Medium"));
        return true;
    }
    if (cmd == "status.pump.medium") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::Medium);
        writeLine(QString::fromUtf8("Set pump pressure: Medium"));
        return true;
    }
    if (cmd == "status.pump.mediumhigh") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::MediumHigh);
        writeLine(QString::fromUtf8("Set pump pressure: Medium-High"));
        return true;
    }
    if (cmd == "status.pump.high") {
        Values::updatePumpPressureStatus(Values::PumpPressureStatus::High);
        writeLine(QString::fromUtf8("Set pump pressure: High"));
        return true;
    }
    
    // Main Page Control commands
    if (cmd == "main.heating.on") {
        Values::updateHeatingSelector(true);
        writeLine(QString::fromUtf8("Main Page: Heating Selector: On"));
        return true;
    }
    if (cmd == "main.heating.off") {
        Values::updateHeatingSelector(false);
        writeLine(QString::fromUtf8("Main Page: Heating Selector: Off"));
        return true;
    }
    if (cmd == "main.generator.on") {
        Values::updateGeneratorSelector(true);
        writeLine(QString::fromUtf8("Main Page: Generator Selector: On"));
        return true;
    }
    if (cmd == "main.generator.off") {
        Values::updateGeneratorSelector(false);
        writeLine(QString::fromUtf8("Main Page: Generator Selector: Off"));
        return true;
    }
    if (cmd == "main.upper.start") {
        Values::updateUpperSpindleStartStop(true);
        Activity::handleUpperSpindleStartStop(true);
        writeLine(QString::fromUtf8("Main Page: Upper Spindle: Start"));
        return true;
    }
    if (cmd == "main.upper.stop") {
        Values::updateUpperSpindleStartStop(false);
        Activity::handleUpperSpindleStartStop(false);
        writeLine(QString::fromUtf8("Main Page: Upper Spindle: Stop"));
        return true;
    }
    if (cmd == "main.lower.start") {
        Values::updateLowerSpindleStartStop(true);
        Activity::handleLowerSpindleStartStop(true);
        writeLine(QString::fromUtf8("Main Page: Lower Spindle: Start"));
        return true;
    }
    if (cmd == "main.lower.stop") {
        Values::updateLowerSpindleStartStop(false);
        Activity::handleLowerSpindleStartStop(false);
        writeLine(QString::fromUtf8("Main Page: Lower Spindle: Stop"));
        return true;
    }
    // Settings Page Control commands
    if (cmd == "settings.heating.on") {
        Values::updateSettingsHeatingSelector(true);
        writeLine(QString::fromUtf8("Settings Page: Heating Selector: On"));
        return true;
    }
    if (cmd == "settings.heating.off") {
        Values::updateSettingsHeatingSelector(false);
        writeLine(QString::fromUtf8("Settings Page: Heating Selector: Off"));
        return true;
    }
    if (cmd == "settings.generator.on") {
        Values::updateSettingsGeneratorSelector(true);
        writeLine(QString::fromUtf8("Settings Page: Generator Selector: On"));
        return true;
    }
    if (cmd == "settings.generator.off") {
        Values::updateSettingsGeneratorSelector(false);
        writeLine(QString::fromUtf8("Settings Page: Generator Selector: Off"));
        return true;
    }
    if (cmd == "settings.generator.status.ok") {
        Values::updateSettingsGeneratorStatus(true);
        writeLine(QString::fromUtf8("Settings Page: Generator Status: OK"));
        return true;
    }
    if (cmd == "settings.generator.status.error") {
        Values::updateSettingsGeneratorStatus(false);
        writeLine(QString::fromUtf8("Settings Page: Generator Status: Error"));
        return true;
    }
    if (cmd == "settings.upper.start") {
        Values::updateSettingsUpperSpindleStartStop(true);
        Activity::handleSettingsUpperSpindleStartStop(true);
        writeLine(QString::fromUtf8("Settings Page: Upper Spindle: Start"));
        return true;
    }
    if (cmd == "settings.upper.stop") {
        Values::updateSettingsUpperSpindleStartStop(false);
        Activity::handleSettingsUpperSpindleStartStop(false);
        writeLine(QString::fromUtf8("Settings Page: Upper Spindle: Stop"));
        return true;
    }
    if (cmd == "settings.lower.start") {
        Values::updateSettingsLowerSpindleStartStop(true);
        Activity::handleSettingsLowerSpindleStartStop(true);
        writeLine(QString::fromUtf8("Settings Page: Lower Spindle: Start"));
        return true;
    }
    if (cmd == "settings.lower.stop") {
        Values::updateSettingsLowerSpindleStartStop(false);
        Activity::handleSettingsLowerSpindleStartStop(false);
        writeLine(QString::fromUtf8("Settings Page: Lower Spindle: Stop"));
        return true;
    }
    if (cmd == "gas.pressure.status.normal") {
        Values::updateGasPanelGasPressureStatus(0);
        writeLine(QString::fromUtf8("Gas Panel: Pressure Status: Normal"));
        return true;
    }
    if (cmd == "gas.pressure.status.low") {
        Values::updateGasPanelGasPressureStatus(1);
        writeLine(QString::fromUtf8("Gas Panel: Pressure Status: Low"));
        return true;
    }
    if (cmd == "gas.pressure.status.high") {
        Values::updateGasPanelGasPressureStatus(2);
        writeLine(QString::fromUtf8("Gas Panel: Pressure Status: High"));
        return true;
    }

    // Error Log and Archive commands
    if (cmd == "log.add") {
        if (parts.size() < 2) {
            writeLine(QString::fromUtf8("Error: Message required. Usage: log.add <message> [color] [0|1]"));
            return true;
        }
        QStringList msgParts = parts.mid(1);
        QString color;
        bool resettable = true;
        for (int i = msgParts.size() - 1; i >= 0; --i) {
            if (msgParts[i].startsWith(QString::fromUtf8("#"))) {
                color = msgParts[i];
                msgParts.removeAt(i);
                break;
            }
        }
        if (!msgParts.isEmpty() && (msgParts.last() == QString::fromUtf8("0") || msgParts.last() == QString::fromUtf8("1"))) {
            resettable = (msgParts.last() == QString::fromUtf8("1"));
            msgParts.removeLast();
        }
        QString message = msgParts.join(QString::fromUtf8(" ")).trimmed();
        if (message.isEmpty()) {
            writeLine(QString::fromUtf8("Error: Message cannot be empty"));
            return true;
        }
        ErrorLogIO::appendToErrorsLog(message, color, resettable);
        writeLine(QString::fromUtf8("Added to Error Log: ") + message);
        return true;
    }
    if (cmd == "archive.add") {
        if (parts.size() < 2) {
            writeLine(QString::fromUtf8("Error: Message required. Usage: archive.add <message>"));
            return true;
        }
        QString message = parts.mid(1).join(QString::fromUtf8(" ")).trimmed();
        if (message.isEmpty()) {
            writeLine(QString::fromUtf8("Error: Message cannot be empty"));
            return true;
        }
        ErrorLogIO::appendToArchive(message);
        writeLine(QString::fromUtf8("Added to Archive: ") + message);
        return true;
    }

    // Vacuum Control commands
    if (cmd == "vacuum.pump.on") {
        Values::updateVacuumPumpSelector(true);
        writeLine(QString::fromUtf8("Vacuum Pump: On"));
        return true;
    }
    if (cmd == "vacuum.pump.off") {
        Values::updateVacuumPumpSelector(false);
        writeLine(QString::fromUtf8("Vacuum Pump: Off"));
        return true;
    }
    if (cmd == "vacuum.valve.on") {
        Values::updateVacuumValveSelector(true);
        writeLine(QString::fromUtf8("Vacuum Valve: On"));
        return true;
    }
    if (cmd == "vacuum.valve.off") {
        Values::updateVacuumValveSelector(false);
        writeLine(QString::fromUtf8("Vacuum Valve: Off"));
        return true;
    }
    if (cmd == "vacuum.autopump.on") {
        Values::updateAutoPumpDownSelector(true);
        writeLine(QString::fromUtf8("Auto Pump Down: On"));
        return true;
    }
    if (cmd == "vacuum.autopump.off") {
        Values::updateAutoPumpDownSelector(false);
        writeLine(QString::fromUtf8("Auto Pump Down: Off"));
        return true;
    }
    if (cmd == "vacuum.upperdoor.open") {
        Values::updateUpperDoorSelector(true);
        writeLine(QString::fromUtf8("Upper Door: Open"));
        return true;
    }
    if (cmd == "vacuum.upperdoor.close") {
        Values::updateUpperDoorSelector(false);
        writeLine(QString::fromUtf8("Upper Door: Close"));
        return true;
    }
    if (cmd == "vacuum.lowerdoor.open") {
        Values::updateLowerDoorSelector(true);
        writeLine(QString::fromUtf8("Lower Door: Open"));
        return true;
    }
    if (cmd == "vacuum.lowerdoor.close") {
        Values::updateLowerDoorSelector(false);
        writeLine(QString::fromUtf8("Lower Door: Close"));
        return true;
    }
    if (cmd == "vacuum.maindoor.open") {
        Values::updateMainDoorStatus(true);
        writeLine(QString::fromUtf8("Main Door Status: Open"));
        return true;
    }
    if (cmd == "vacuum.maindoor.close") {
        Values::updateMainDoorStatus(false);
        writeLine(QString::fromUtf8("Main Door Status: Close"));
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
                        writeLine(QString::fromUtf8("Lighting Button ") + QString::number(index) + QString::fromUtf8(": ") + (state == 1 ? QString::fromUtf8("On") : QString::fromUtf8("Off")));
                        return true;
                    } else {
                        writeLine(QString::fromUtf8("Error: Invalid state. Use 0 (Off) or 1 (On)"));
                        return true;
                    }
                } else {
                    writeLine(QString::fromUtf8("Error: Invalid button index. Use 0-3"));
                    return true;
                }
            }
        } else {
            writeLine(QString::fromUtf8("Error: State required. Usage: ") + cmd + QString::fromUtf8(" <0|1>"));
            return true;
        }
    }
    
    return false;
}
