#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QWidget>
#include <QHBoxLayout>
#include <QtGui>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QDockWidget>
#include <QTime>
#include <QKeySequence>
#include <QAction>
#include <QDomDocument>
#include <QDesktopWidget>
#include "vectorglwidget.h"
#include "tileglwidget.h"
#include "informationwidget.h"
#include "../engine/engine.hpp"
#include "decodexml.h"


class TileGLWidget;
class DecodeXml;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);//: QMainWindow(parent);
	~MainWindow();
private slots:
	void openSim(QString fileName = "");
	void saveSim();
	void saveAsSim();
	void play();
	void pause();
	void about();
	void setSpeed();
	void backToBegin();
	void changeNumSpeed();
	void updateEngine();
	void setStatusBar(QString);
	//void showSetSpeedMessage();
private:
	//functions responsible for creating proper window with proper functionality
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createMainWidget();

	//Opening, saving functions
	bool decodeFile(const QString &fileName);
	void showSorryBox();
	//Toolbars
	QToolBar *fileToolBar;
	QToolBar *simulationToolBar;
	QLineEdit *speedLine;
	QSlider *speedSlider;
	//Menus: File, Simulation, Help
	QMenu *fileMenu;
	QMenu *simulationMenu;
	QMenu *helpMenu;
	//Buttons MenuBar Area
	QAction *openSimAct;
	QAction *saveSimAct;
	QAction *saveAsSimAct;
	QAction *exitAct;
	QAction *playAct;
	QAction *pauseAct;
	QAction *backToBeginAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	//QAction *setSpeedAct;
	QAction *aboutAct;
	QAction *aboutQtAct;
	//QPushButton *getDistance;
	//Useful area for simulation
	//QDockWidget *simulationDock;
	//VectorGLWidget *simulationWidget;
	QDockWidget *tileSimDock;
	TileGLWidget *tileSimWidget;
	QDockWidget *infoTrainsDock;
	InformationWidget *infoTrainWidget;
	//Playing simulation
	QTimer *timer;
	unsigned int iterationNum;
	Engine *engine;
	//Engine *copyOfEngine;
	bool isOpened;
	//Reading and writing simulation
	QString openedFileName; //we can use static function QFile::copy();
    //Structure of .xml file
	DecodeXml *simulationData;

};

#endif // MAINWINDOW_H
