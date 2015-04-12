#include "informationwidget.h"
#define MAXWIDTH 200
#define TILESIZE 50

InformationWidget::InformationWidget(Engine * eng, QWidget *parent) : QWidget(parent), engine(eng)
{
	createWidgets();
	setMaximumWidth(MAXWIDTH);
	init();
	update();
	//activeChanged(trainsList->count()-1);
}

InformationWidget::~InformationWidget()
{

}

void InformationWidget::createWidgets()
{
	layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignTop);
	setLayout(layout);
	trainLabel = new QLabel(tr("Train list:"));
	trainsList = new QComboBox();
	trainInfoLabel = new QLabel(tr("Simple information"));
	layout->addWidget(trainLabel);
	layout->addWidget(trainsList);
	layout->addWidget(trainInfoLabel);
	connect(trainsList, SIGNAL(activated(int)), this, SLOT(activeChanged(int)));
}

void InformationWidget::init()
{
	trainsList->clear(); //clear old list, i'm not sure if trains can disapeard
	const Engine::TrainState *t;
	//int num = 0;
	foreach (t, engine->trainStates) {
		trainsList->addItem(QString(t->name.c_str()));
	}
	trainsList->addItem("None");
	trainInfoLabel->setText("");
	lastActive = trainsList->count() - 1;
	trainsList->setCurrentIndex(trainsList->count() - 1);

}

void InformationWidget::update()
{
	activeChanged(trainsList->currentIndex());
}

void InformationWidget::activeChanged(int current)
{
	QString information;
	information = tr("Time:") + " " + QString::number(engine->getIterations()/30.0,'f',1) + "\n\n";
	if(current == trainsList->count() - 1){
		if(lastActive != current)
			emit activeChangedSig(NULL);
		lastActive = current;
		trainInfoLabel->setText(information);
		return;
	}

	information += tr("Name:") + "\t"+ QString(engine->trainStates[current]->name.c_str()) + "\n";
	information += tr("Speed:") + '\t' + QString::number(engine->trainStates[current]->speed) + "\n";
	information += tr("Route length:") + "\t" + QString::number(engine->trainStates[current]->routeLength) + "\n";
	information += tr("Stations:") + "\n";
	for(size_t i = 0; i < engine->trainStates[current]->stations.size(); ++i)
	{
		information += " " + tr("at (") + QString::number(engine->trainStates[current]->stations[i].x / TILESIZE) + "," + QString::number(engine->trainStates[current]->stations[i].y / TILESIZE) + ") " + QString::number(engine->trainStates[current]->stations[i].time/30.0,'f',2) + "s \n";
	}
	if(engine->trainStates[current]->time)
	{
		information += "\n\t" + tr("Train reached \n the destination station") + "\n";
		information += "\n" + tr("Average speed") + " " + QString::number(engine->trainStates[current]->avgSpeed,'f',2) + "\n";
		information +=  tr("Travel time") + " " + QString::number(engine->trainStates[current]->time/30.0,'f',2) + "\n";
		information +=  tr("Idle time") + " " + QString::number(engine->trainStates[current]->idleTime/30.0,'f',2);
	}
	trainInfoLabel->setText(information);
	if(lastActive == current)
		return;

	lastActive = current;
	emit activeChangedSig(engine->trainStates[current]);
}
