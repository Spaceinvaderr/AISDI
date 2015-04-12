#ifndef DECODEXML_H
#define DECODEXML_H

#include <QtXml/QXmlInputSource>
#include <vector>
#include <iostream>
#include "../engine/engine.hpp"
#include "vectorglwidget.h"
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomElement>
#include <QtGui>
#include <QObject>
#include "tileglwidget.h"

using namespace std;

class TileGLWidget;

class DecodeXml : public QObject
{
	Q_OBJECT
private:
	//temp vector to keep station of current train
	vector<Engine::Stop> tempvec;
	//pointer to actually engine. on which parser working on
	Engine *engine;
	//widget to tile  visualization  
	TileGLWidget *tileSimWidget;
	QDomDocument *data;
	int parseTileMap(QString &text, int XPos = 0, int YPos = 0);//, int shift = 0); shift has no purpose currently.

public:
	//comment
    void setIteration(int value);
    int getIteration();
	bool saveToFile(QString &path);
	bool parseXML(const QString &fileName)throw(QString);
	DecodeXml(Engine *eng, TileGLWidget *tileSimWidg);
	~DecodeXml();
};

#endif // DECODEXML_H
