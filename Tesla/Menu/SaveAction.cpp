#include "SaveAction.h"

#include "Menu.h"

#include <MainView.h>

#include <QFileDialog>

SaveAction::SaveAction(double squareBreadth, Menu* parentMenu, MainView* mainView) :
    MenuAction(squareBreadth, parentMenu, mainView)
{
    QPixmap pM(getIconPath());
    pM.scaledToHeight(squareBreadth);
    QGraphicsPixmapItem* m_iconPixmapItem = new QGraphicsPixmapItem(pM);
    QRectF rect = m_iconPixmapItem->boundingRect();
    m_iconPixmapItem->setScale(squareBreadth / rect.height());
    m_iconPixmapItem->setPos(0., 0.);
    m_iconPixmapItem->setParentItem(this);
}

void SaveAction::executeAction()
{
    QString filePath = m_parentMenu->getFilePath();
    if (filePath != "")
    {
        m_mainView->saveToFile(filePath);
    }
    else
    {
        QString dir("C:/Users/foehn/OneDrive/Documents/dump/");
        QString newFilePath = QFileDialog::getSaveFileName(m_mainView, QFileDialog::tr("Save File"),
                                   dir,
                                   QFileDialog::tr("Circuit files (*.crc)"));
        m_mainView->saveToFile(newFilePath);
        m_parentMenu->setFilePath(newFilePath);
    }
}

bool SaveAction::isAvailable() const
{
    return m_parentMenu->isAvailableGeneral() && (m_parentMenu->getFilePath() != "");
}
