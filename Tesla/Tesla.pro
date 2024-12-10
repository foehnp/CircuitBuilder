QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ControlPane/ControlPane.cpp \
        DrawArea/BJTDI.cpp \
        DrawArea/BatteryDI.cpp \
        DrawArea/BulbDI.cpp \
        DrawArea/ComponentDI.cpp \
        DrawArea/CornerWireDI.cpp \
        DrawArea/DiodeDI.cpp \
        DrawArea/DrawAreaItem.cpp \
        DrawArea/EmptyComponentDI.cpp \
        DrawArea/ForkDI.cpp \
        DrawArea/GroundDI.cpp \
        DrawArea/ParamDialog.cpp \
        DrawArea/ResistorDI.cpp \
        DrawArea/WireDI.cpp \
        MainView.cpp \
        Menu/Menu.cpp \
        Menu/MenuAction.cpp \
        Menu/OpenAction.cpp \
        Menu/SaveAction.cpp \
        Menu/SaveAsAction.cpp \
        Scale/ScaleGraphicsItem.cpp \
        Scale/ScaleWidget.cpp \
        Scale/ScaleCollection.cpp \
        Solver/NLSolver.cpp \
        Solver/Solver.cpp \
        Toolbox/BJTTI.cpp \
        Toolbox/BatteryTI.cpp \
        Toolbox/BulbTI.cpp \
        Toolbox/CornerWireTI.cpp \
        Toolbox/DiodeTI.cpp \
        Toolbox/EmptyComponentTI.cpp \
        Toolbox/EraserTI.cpp \
        Toolbox/ForkTI.cpp \
        Toolbox/GroundTI.cpp \
        Toolbox/ToolboxArea.cpp \
        Toolbox/WireTI.cpp \
        Toolbox/ComponentTI.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ControlPane/ControlPane.h \
    DrawArea/BJTDI.h \
    DrawArea/BatteryDI.h \
    DrawArea/BulbDI.h \
    DrawArea/ComponentDI.h \
    DrawArea/CornerWireDI.h \
    DrawArea/DiodeDI.h \
    DrawArea/DrawAreaItem.h \
    DrawArea/EmptyComponentDI.h \
    DrawArea/ForkDI.h \
    DrawArea/GroundDI.h \
    DrawArea/ParamDialog.h \
    DrawArea/ResistorDI.h \
    DrawArea/WireDI.h \
    EnumsDefsEtc.h \
    MainView.h \
    Menu/Menu.h \
    Menu/MenuAction.h \
    Menu/OpenAction.h \
    Menu/SaveAction.h \
    Menu/SaveAsAction.h \
    Scale/ScaleGraphicsItem.h \
    Scale/ScaleWidget.h \
    Scale/ScaleCollection.h \
    Solver/NLSolver.h \
    Solver/Solver.h \
    Toolbox/BJTTI.h \
    Toolbox/BatteryTI.h \
    Toolbox/BulbTI.h \
    Toolbox/CornerWireTI.h \
    Toolbox/DiodeTI.h \
    Toolbox/EmptyComponentTI.h \
    Toolbox/ComponentTI.h \
    Toolbox/EraserTI.h \
    Toolbox/ForkTI.h \
    Toolbox/GroundTI.h \
    Toolbox/ToolboxArea.h \
    Toolbox/WireTI.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    imagesRF.qrc

DISTFILES += \
    ControlPane/sdfsdf.txt \
    Images/eraser.png

INCLUDEPATH += \
    D:\eigen-3.4-rc1
