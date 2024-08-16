#pragma once

#include "MenuAction.h"

class SaveAsAction : public MenuAction
{
public:
    SaveAsAction(double squareBreadth, Menu* parentMenu, MainView* mainView);

protected:
    QString getIconPath() const override
    {
        return ":/Images/save_icon_transparent.png";
    }

    void executeAction() override;

    bool isAvailable() const override;

private:
    QGraphicsPixmapItem* m_iconPixmapItem;
};

