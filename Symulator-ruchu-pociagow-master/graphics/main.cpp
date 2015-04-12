#include <QApplication>
#include <QObject>
#include "mainwindow.h"
//#include <QPlastiqueStyle>


int main(int argc, char *argv[])
{
	//QApplication::setStyle(new QPlastiqueStyle);
	//One instance only
	QApplication a(argc, argv);
	MainWindow *w;
//	QSharedMemory sharedMemory("UNIQUEAISDIPROGRAM");
//	if(!sharedMemory.create(1)){ //app is working currently so exit
//		QMessageBox::warning(0,QObject::tr("Cannot open the application"), QObject::tr("Previous instance is still running"));
//	return 0;
//	}
	w = new MainWindow(argc,argv);
	w->show();
	return a.exec();
}

