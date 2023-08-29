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
};


enum MouseModes
{
    NeutralMM,
    EraserMM,
    WireMM,
    CornerWireMM,
    ForkMM,
    BatteryMM,
    BulbMM,
    DiodeMM,
    BJTMM,
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
