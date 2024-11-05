#pragma once

#include <QColor>

#include <string>
#include <map>

const QColor ITEM_SELECTED_COLOR(0, 0, 255, 63);

enum ComponentName
{
    EmptyComponent,
    Wire,
    CornerWire,
    Fork,
    Battery,
    Bulb,
    Diode,
    BJT,
    Resistor
};


enum MouseModes
{
    RunningMM,
    NeutralMM,
    EraserMM,
    WireMM,
    CornerWireMM,
    ForkMM,
    BatteryMM,
    BulbMM,
    DiodeMM,
    BJTMM,
    ResistorMM
};

enum RunMode
{
    Drawing,
    Running,
};

enum PhysicalQuantity
{
    Potential,
    Current,
};

static std::map<std::string, ComponentName> persistenceNameToComponentNameMap = {
    {"EmptyComponent", EmptyComponent},
    {"Wire" , Wire},
    {"CornerWire" , CornerWire},
    {"Fork" , Fork},
    {"Battery" , Battery},
    {"Bulb" , Bulb},
    {"Diode" , Diode},
    {"BJT" , BJT},
    {"Resistor" , Resistor},
};

static ComponentName persistenceNameToComponentName(const std::string& persistenceName)
{
    return persistenceNameToComponentNameMap[persistenceName];
}

static std::string componentNameToPersistenceName(const ComponentName& componentName)
{
    for (const auto& [per, com] : persistenceNameToComponentNameMap)
    {
        if (com == componentName)
        {
            return per;
        }
    }
    return "empty";
}
