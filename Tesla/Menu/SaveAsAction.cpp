#include "SaveAsAction.h"

#include <MainView.h>

#include <QFileDialog>

SaveAsAction::SaveAsAction(double squareBreadth, Menu* parentMenu, MainView* mainView) :
    MenuAction(squareBreadth, parentMenu, mainView)
{

}

void SaveAsAction::executeAction()
{
    QString dir("C:/Users/foehn/OneDrive/Documents/dump/");
    QString fileName = QFileDialog::getSaveFileName(m_mainView, QFileDialog::tr("Save File"),
                               dir,
                               QFileDialog::tr("Circuit files (*.crc)"));
    m_mainView->saveToFile(fileName);
}
