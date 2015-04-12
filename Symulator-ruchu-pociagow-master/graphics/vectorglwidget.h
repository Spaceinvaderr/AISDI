#ifndef VECTORGLWIDGET_H
#define VECTORGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QVector>
#include <QPair>
#include <GL/glu.h>
#include "../engine/engine.hpp"

class VectorGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    VectorGLWidget(QWidget *parent = 0);
    ~VectorGLWidget();
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void addStation(int,int);
    void addTrack(int,int,int,int);
    void setTrains(QVector< QVector< QPair<int,int> > > );
    void setCars(QVector< QVector< QPair < QPair<int,int>, int > > > );
	void setSemafors(QVector< QPair< QPair<int,int>, bool > >);
	void setSemafors(vector<Engine::SemaforState*>* s);
protected: 
    float X(int);
    float Y(int);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
public slots:
    void animate();
private:
    int beginX;
    int beginY;
    GLuint textures[13];
    QPoint lastMousePosition;
    void drawStation(QPair<int,int>, int = 2);
    void drawTrack(QPair< QPair<int,int>, QPair<int,int> >, int = 2);
    void drawTrain(QVector< QPair<int,int> >, int = 2, float color = 1.0); //it will change
	void drawCar(QVector< QPair<QPair<int,int>,int> >);
    void drawSemafor(Engine::SemaforState & s);
    void drawTest();
	bool loadImage(QString filename,GLuint& texture);
	void loadTextures();
    QVector< QPair<int,int> > stationsSim;
    QVector< QPair< QPair<int,int>, QPair<int,int> > > tracksSim;
    QVector< QVector< QPair<int,int> > > trainsSim;
    QVector< QVector< QPair< QPair<int,int>, int > > > carsSim;
    QVector< QPair< QPair<int,int>, bool > > semaforsSim;
	vector<Engine::SemaforState*>* semafors;

};

#endif // VECTORGLWIDGET_H
