#ifndef TILEGLWIDGET_H
#define TILEGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QVector>
#include <QPair>
#include <GL/glu.h>
#include "mainwindow.h"
#include "../engine/engine.hpp"
#define MAXTEX 41
#define OTHTEX 5
#define SCALEVAL 8
#define MAXX 1000
#define MAXY 1000

class MainWindow;

class TileGLWidget : public QGLWidget
{
	Q_OBJECT
public:
	TileGLWidget(MainWindow *window, Engine *engine, QWidget *parent = 0);
	~TileGLWidget();
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void addTile(int x, int y, int type);
	void clearAll();

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
public slots:
	void animate();
	//void measureMode();
	void zoomIn();
	void zoomOut();
	void higlightedChange(const Engine::TrainState *);
signals:
	void informEmit(QString);
private:
	void drawSkull(Engine::Pos &position);
	void drawBorder(int x, int y);
	void drawTile(int x, int y, int type);
	void drawCar(const Engine::TrainState &t);
	//void drawSemafor(int x, int y, bool green);
	void drawSemafor(Engine::SemaforState & s);
	MainWindow *mainWindow;
	Engine *engine;
	const Engine::TrainState * highlighted;
	unsigned int lastNumOfDeadlocks;
	QVector< int> tiles[MAXX][MAXY];
	QVector< QPair< QPair<int,int>, int> > farTiles;
	//zooming
	int zoomOpt;
	//mouse events variables
	int beginX;
	int beginY;
	QPoint lastMousePosition;
	QPoint lastMeasurePosition;
	float zoomBeginValue;
	QPair<int,int> lastBegin;
	//textures
	GLuint tileTextures[MAXTEX];
	GLuint oldTexture;
    const QString tileTexturesSource[MAXTEX] = {":tiles/0.png", ":tiles/1.png", ":tiles/2.png", ":tiles/3.png", ":tiles/4.png", ":tiles/5.png", ":tiles/6.png", ":tiles/7.png", ":tiles/8.png", ":tiles/9.png", ":tiles/10.png", ":tiles/11.png", ":tiles/12.png", ":tiles/13.png", ":tiles/14.png", ":tiles/15.png", ":tiles/16.png", ":tiles/17.png", ":tiles/18.png", ":tiles/19.png", ":tiles/20.png", ":tiles/21.png", ":tiles/22.png", ":tiles/23.png", ":tiles/24.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/37.png", ":tiles/38.png", ":tiles/39.png", ":tiles/40.png"};
	GLuint otherTextures[OTHTEX];
	const QString otherTexturesSource[OTHTEX] = {":images/bluecar.png", ":images/green light.png", ":images/red light.png", ":images/skullblack.png", ":images/redcar.png"};
	const float zoomValues[SCALEVAL] = {0.25f, 0.5f, 0.75f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f};
	int mode; //0 - simple scrool, 1 - first point measure, 2 - point measure

};
#endif // TILEGLWIDGET_H

