#include "mainwindow.h"


MainWindow::MainWindow(int argc, char** argv,QWidget * parent) :QMainWindow(parent), iterationNum(0)
{
	QDesktopWidget desktop;
	//Set minimum and maximum window size
	setMinimumSize(QSize(500,400));
	setMaximumSize(QSize(desktop.geometry().width(), desktop.geometry().height()));
	//Set defualt size
	resize(QSize(desktop.geometry().width() - 100, desktop.geometry().height() - 100));
	//Creating engine
	engine = new Engine;
	//Set icons size
	setIconSize(QSize(30,30));
	timer = new QTimer(this);
	timer->stop();
	createMainWidget();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	//Create parsing utility
	simulationData = new DecodeXml(engine, tileSimWidget);
	//If timer starts, animation starts
	connect(timer, SIGNAL(timeout()), this, SLOT(updateEngine()));
	isOpened = false;
	if(argc > 1)
	{
		openSim(argv[1]);
	}
}


MainWindow::~MainWindow()
{
	delete engine;
}


void MainWindow::openSim(QString fileName)
{
	if(isOpened == true)
	{
		if(QMessageBox::question(this, tr("Are you sure?"), tr("There is simulation opened, are you sure of leaving it?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
		{
			return;
		}
	}
	if(!fileName.size())
		fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString("../xml/"), tr("XML files (*.xml)"));
	if(!fileName.size())
		return;
	engine->clear();
	tileSimWidget->clearAll();
	iterationNum = 0;
	openedFileName = fileName;
	if(decodeFile(fileName) == true){
		isOpened = true;
	}else{
		isOpened = false;
		engine->clear();
		tileSimWidget->clearAll();
		return;
	}
	try{
		engine->init();
		infoTrainWidget->init();
	}
	catch(Engine::reserved)
	{
		isOpened = false;
		engine->clear();
		infoTrainWidget->init();
		tileSimWidget->clearAll();
		QMessageBox::warning(this, tr("Decoding error occured"), tr("One segment can be reserved by exactly one train."));
	}
	catch(Engine::NoRoute)
	{
		isOpened = false;
		engine->clear();
		infoTrainWidget->init();
		tileSimWidget->clearAll();
		QMessageBox::warning(this, tr("Decoding error occured"), tr("There is no connection between 2 given stations in one of the train route."));
	}
	//if file was saved on specific iteration, go to this iteration
	iterationNum = simulationData->getIteration();
	engine->update(iterationNum);
	//After open simulation show what we have got
	updateEngine();
}

void MainWindow::saveSim()
{
	simulationData->setIteration(iterationNum);
	if(simulationData->saveToFile(openedFileName) == false)
		QMessageBox::warning(this, tr("Saving failed"), tr("Saving failed"));
}

void MainWindow::saveAsSim()
{
	if(isOpened == false){
		QMessageBox::information(this, tr("Nothing to save"), tr("There is no simulation to save."));
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString("../"), tr("XML files (*.xml)"));
	if(fileName.isEmpty())
		return;
	simulationData->setIteration(iterationNum);
	if(simulationData->saveToFile(fileName) == false){
		QMessageBox::warning(this, tr("Saving failed"), tr("Saving failed"));
	}
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("Train simulator app"),
			tr("Last modified: 4.12.2013"));
}

void MainWindow::play()
{
	if(isOpened == false){
		QMessageBox::warning(this, tr("No simulation set"), tr("No simulation read"));
		return;
	}
	timer->start(1000/((int)(0.6*(speedSlider->value())) + 1));
}

void MainWindow::pause()
{
	timer->stop();
}


void MainWindow::setSpeed()
{
	showSorryBox();
}

void MainWindow::backToBegin()
{
	iterationNum = 0;
	engine->reset();
	updateEngine();
}


void MainWindow::createActions()
{
	//Open simulation action
	openSimAct = new QAction(QIcon(":/icons/openFileButton.png"), tr("&Open simulation"), this);
	openSimAct->setIconVisibleInMenu(true);
	openSimAct->setShortcuts(QKeySequence::Open);
	openSimAct->setStatusTip(tr("Open a simulation file"));
	connect(openSimAct, SIGNAL(triggered()), this, SLOT(openSim()));
	//Save simulation action
	saveSimAct = new QAction(QIcon(":/icons/newFileButton.png"), tr("&Save simulation"), this);
	saveSimAct->setShortcuts(QKeySequence::Save);
	saveSimAct->setStatusTip(tr("Save the simulation to disk"));
	connect(saveSimAct, SIGNAL(triggered()), this, SLOT(saveSim()));
	//Save As simulation action
	saveAsSimAct = new QAction(tr("Save simulation &As..."), this);
	saveAsSimAct->setShortcuts(QKeySequence::SaveAs);
	saveAsSimAct->setStatusTip(tr("Save the simulation under a new name"));
	connect(saveAsSimAct, SIGNAL(triggered()), this, SLOT(saveAsSim()));
	//Exit action
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
	//Play action
	playAct = new QAction(QIcon(":/icons/playButton.png"),tr("&Play"), this);
	playAct->setShortcuts(QKeySequence::MoveToNextChar);
	playAct->setStatusTip(tr("Play the simulation"));
	connect(playAct, SIGNAL(triggered()), this, SLOT(play()));
	//Pause action
	pauseAct = new QAction(QIcon(":/icons/pauseButton.png"),tr("Pa&use"), this);
	pauseAct->setShortcuts(QKeySequence::MoveToNextLine);
	pauseAct->setStatusTip(tr("Pause the simulation"));
	connect(pauseAct, SIGNAL(triggered()), this, SLOT(pause()));
	//Back to begin action
	backToBeginAct = new QAction(QIcon(":/icons/backtostartButton.png"), tr("&Back to begin"), this);
	backToBeginAct->setShortcuts(QKeySequence::MoveToPreviousChar);
	connect(backToBeginAct, SIGNAL(triggered()), this, SLOT(backToBegin()));
	//Zoom in
	zoomInAct = new QAction(tr("-"), this);
	connect(zoomInAct, SIGNAL(triggered()), tileSimWidget, SLOT(zoomIn()));
	//Zoom out
	zoomOutAct = new QAction(tr("+"), this);
	connect(zoomOutAct, SIGNAL(triggered()), tileSimWidget, SLOT(zoomOut()));
	//Speed action
	//setSpeedAct = new QAction(tr("Se&t speed"), this);
	//connect(setSpeedAct, SIGNAL(triggered()), this, SLOT(showSetSpeedMessage()));
	//setSpeedAct->setShortcut(QKeySequence::MoveToPreviousLine);
	//pauseAct->setShortcuts(QKeySequence::);
	//setSpeedAct->setStatusTip(tr("Set speed of the simulation"));
	//connect(setSpeedAct, SIGNAL(triggered()), this, SLOT(setSpeed()));


}

void MainWindow::createMenus()
{
	//File menu
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openSimAct);
	fileMenu->addAction(saveSimAct);
	fileMenu->addAction(saveAsSimAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	//Simulation menu
	simulationMenu = menuBar()->addMenu(tr("&Simulation"));
	simulationMenu->addAction(playAct);
	simulationMenu->addAction(pauseAct);
	//simulationMenu->addAction(setSpeedAct);
	simulationMenu->addAction(backToBeginAct);
	menuBar()->addSeparator();
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openSimAct);
	fileToolBar->addAction(saveSimAct);
	simulationToolBar = addToolBar(tr("Simulation"));
	simulationToolBar->addAction(playAct);
	simulationToolBar->addAction(pauseAct);
	simulationToolBar->addAction(backToBeginAct);
	simulationToolBar->addAction(zoomInAct);
	simulationToolBar->addAction(zoomOutAct);
	//Pair: slider and line with number, changing slider changes num
	speedLine = new QLineEdit;
	speedLine->setMaximumWidth(40);
	speedSlider = new QSlider( Qt::Horizontal, this);
	speedSlider->setValue(49);
	changeNumSpeed();
	speedSlider->setMinimumWidth(200);
	connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(changeNumSpeed()));
	speedLine->setReadOnly(true);
	simulationToolBar->addWidget(speedSlider);
	simulationToolBar->addWidget(speedLine);
	//Free space after simulation toolbar
	addToolBar(tr("Empty"));

}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createMainWidget()
{
	tileSimDock = new QDockWidget(tr("Tile simulation view"), this);
	tileSimWidget = new TileGLWidget(this, engine);
	tileSimDock->setWidget(tileSimWidget);
	tileSimDock->setAllowedAreas( Qt::LeftDockWidgetArea);
	tileSimDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

	infoTrainsDock = new QDockWidget(tr("Information about trains"), this);
	infoTrainWidget = new InformationWidget(engine);
	infoTrainsDock->setWidget(infoTrainWidget);
	infoTrainsDock->setAllowedAreas(Qt::RightDockWidgetArea);
	//connection between activated train in informationwidget and train in tileglwidget
	connect(infoTrainWidget, SIGNAL(activeChangedSig(const Engine::TrainState*)), tileSimWidget, SLOT(higlightedChange(const Engine::TrainState *)));
	connect(tileSimWidget, SIGNAL(informEmit(QString)), this, SLOT(setStatusBar(QString)));
	//addDockWidget(Qt::LeftDockWidgetArea,simulationDock);
	addDockWidget(Qt::LeftDockWidgetArea,tileSimDock);
	addDockWidget(Qt::RightDockWidgetArea , infoTrainsDock);
}

void MainWindow::changeNumSpeed()
{
	QString temp;
	temp.setNum((speedSlider->value() + 1)*0.02f);
	if(timer->timerId() != -1){
		timer->start(1000/((int)(0.6*(speedSlider->value())) + 1));
	}
	speedLine->setText(temp);

}

//Edwin, you should code this function
bool MainWindow::decodeFile(const QString &fileName)
{
	try{
		simulationData->parseXML(fileName);
		return true;
	}catch(QString error)
	{
		QMessageBox::warning(this, tr("Decoding error"), error);
	}
	catch(...)
	{
		QMessageBox::warning(this, tr("Decoding error"), tr("Decoding error occured"));
	}
	return false;
}


//Info function
void MainWindow::showSorryBox()
{
	QMessageBox * box = new QMessageBox(QMessageBox::Critical, tr("Sorry"), tr("This function is not implemented yet"), QMessageBox::Ok, this, Qt::Dialog);
	box->exec();
}


void MainWindow::updateEngine()
{
	engine->update(1);
	iterationNum++;
	tileSimWidget->animate();
	infoTrainWidget->update();
}

void MainWindow::setStatusBar(QString value)
{
	statusBar()->showMessage(value);
}

