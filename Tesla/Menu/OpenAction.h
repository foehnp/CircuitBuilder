#pragma once

#include "MenuAction.h"

class OpenAction : public MenuAction
{
public:
    OpenAction(double squareBreadth, Menu* parentMenu, MainView* mainView);

protected:
    QString getIconPath() const override
    {
        return ":/Images/open_file_icon_transparent.png";
    }

    void executeAction() override;

    bool isAvailable() const override;

private:
    QGraphicsPixmapItem* m_iconPixmapItem;
};
