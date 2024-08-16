#pragma once

#include "MenuAction.h"

class SaveAsAction : public MenuAction
{
public:
    SaveAsAction(double squareBreadth, Menu* parentMenu, MainView* mainView);

protected:
    QString getIconPath() const override
    {
        return ":/Images/save_as_icon.png";
    }

    void executeAction() override;
};

