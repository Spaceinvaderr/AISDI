#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "mappainter.h"
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
#include <QToolBox>
#include <QCursor>
#include <QInputDialog>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void newFile();
    void saveFile();
    void openFile();
    void exit();
    void ChooseTile0();
    void ChooseTile1();
    void ChooseTile2();
    void ChooseTile3();
    void ChooseTile4();
    void ChooseTile5();
    void ChooseTile6();
    void ChooseTile7();
    void ChooseTile8();
    void ChooseTile9();
    void ChooseTile10();
    void ChooseTile11();
    void ChooseTile12();
    void ChooseTile13();
    void ChooseTile14();
    void ChooseTile15();
    void ChooseTile16();
    void ChooseTile17();
    void ChooseTile18();
    void ChooseTile19();
    void ChooseTile20();
    void ChooseTile21();
    void ChooseTile22();
    void ChooseTile23();
    void ChooseTile24();
    void ChooseTile37();
    void ChooseTile38();
    void changeMode();
    void createTrain();


private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createMainWidget();

    int which;
    MapPainter *mapwidget;
    QDockWidget *contentsWindow;
    QVBoxLayout *trainWindow;
    QVBoxLayout *rightWidget;
    QDomDocument *Doc;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QMenu *fileMenu;

    class TileToolBar:public QToolBar
    {
    public:
        TileToolBar(const QString & title/*, QWidget * parent = 0*/): QToolBar(title) {}
    private:
    };
    QToolBar *TileNo0ToolBar;
    QToolBar *TileNo1ToolBar;
    QAction *No0;
    QAction *No1;
    QAction *No2;
    QAction *No3;
    QAction *No4;
    QAction *No5;
    QAction *No6;
    QAction *No7;
    QAction *No8;
    QAction *No9;
    QAction *No10;
    QAction *No11;
    QAction *No12;
    QAction *No13;
    QAction *No14;
    QAction *No15;
    QAction *No16;
    QAction *No17;
    QAction *No18;
    QAction *No19;
    QAction *No20;
    QAction *No21;
    QAction *No22;
    QAction *No23;
    QAction *No24;
    QAction *No37;
    QAction *No38;
    QAction *cursor;
    QAction *createtrain;
    QCursor *cur;

};



#endif // MAINWINDOW_H
