#include <QApplication>
#include <QGraphicsView>
#include <DrawArea/DrawAreaItem.h>
#include <DrawArea/WireDI.h>
#include <QMainWindow>
#include <Toolbox/ToolboxArea.h>
#include <Toolbox/EmptyComponentTI.h>
#include <Toolbox/WireTI.h>
#include <Toolbox/CornerWireTI.h>

#include <MainView.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainView mainView;
    mainView.show();

    return a.exec();
}
