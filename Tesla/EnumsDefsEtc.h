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
};


enum MouseModes
{
    NeutralMM,
    EraserMM,
    WireMM,
    CornerWireMM,
    ForkMM,
    BatteryMM,
    BulbMM
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
