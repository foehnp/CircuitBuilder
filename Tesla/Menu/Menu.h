#pragma once

#include <QGraphicsItem>
#include <EnumsDefsEtc.h>

class MainView;
class MenuAction;

class Menu : public QGraphicsItem
{
public:
    Menu(MainView* mainView, double squareBreadth);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void pushBackAction(MenuAction* menuAction);

    void setIsAvailableGeneral(bool isOn);
    bool isAvailableGeneral() const;

    QString getFilePath() const
    {
        return m_filePath;
    }
    void setFilePath(const QString& filePath)
    {
        m_filePath = filePath;
    }

private:
    MainView* m_mainView = nullptr;
    double m_squareBreadth;
    std::vector<MenuAction*> m_toolboxItemList;

    bool m_isAvailableGeneral;

    RunMode m_state = Drawing;

    QString m_filePath;
};
