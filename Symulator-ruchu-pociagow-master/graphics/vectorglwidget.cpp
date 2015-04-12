#include "vectorglwidget.h"
#include <math.h>
#include <iostream>
#include "../engine/engine.hpp"

VectorGLWidget::VectorGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), beginX(0), beginY(0), semafors(NULL)
{
    //setMinimumSize(800, 600);
    makeCurrent();
	loadTextures();
}

VectorGLWidget::~VectorGLWidget()
{

}

void VectorGLWidget::initializeGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	glOrtho (beginX, beginX+width() , beginY, beginY+height(), 0, 1);
    glMatrixMode (GL_MODELVIEW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void VectorGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QPair<int,int> a;
    foreach (a, stationsSim) {
        drawStation(a);
    }

    QPair< QPair<int,int>, QPair<int,int> > b;
    foreach (b, tracksSim ) {
        drawTrack(b);
    }

	 //QPair< QPair<int,int>, bool > d;
	 //foreach (d, semaforsSim) {
	 //	drawSemafor(d.first,d.second);
	 //}


	 Engine::SemaforState* d;
	 if(semafors!=NULL)
	 foreach (d, *semafors) {
	 	drawSemafor(*d);
	 }

    float temp = 0.0f; //temporary version
    QVector< QPair<int,int> > c;
    foreach (c, trainsSim) {
        drawTrain(c,2,temp);
        temp += 0.2f;
    }

    QVector< QPair < QPair<int,int> , int > > e;
    foreach (e, carsSim) {
        drawCar(e);
    }
	//drawTest();
}


void VectorGLWidget::resizeGL(int w, int h)
{
	initializeGL();
    glViewport(0, 0, w, h);
}

void VectorGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePosition = event->pos();
}

void VectorGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastMousePosition.x();
    int dy = event->y() - lastMousePosition.y();
    if (event->buttons() & Qt::LeftButton) {
        beginX -= dx;
        beginY += dy;
    }
	initializeGL(); 
    lastMousePosition = event->pos();
    updateGL();
}

void VectorGLWidget::animate()
{
    updateGL();
}

void VectorGLWidget::drawStation(QPair<int,int> position, int size)
{    
    glBegin(GL_QUADS);//start drawing a line loop
            glColor3f(1.0f,0.0f,0.0f);
            glVertex2f(position.first-size,position.second-size);//left of window
            glVertex2f(position.first+size,position.second-size);//left of window
            glVertex2f(position.first+size,position.second+size);//left of window
            glVertex2f(position.first-size,position.second+size);//left of window
    glEnd();
}

void VectorGLWidget::drawTrack(QPair< QPair<int,int>, QPair<int,int> > positions, int size)
{
    glLineWidth((float)size);
    glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(positions.first.first, positions.first.second);
        glVertex2f(positions.second.first, positions.second.second);
    glEnd();
}

void VectorGLWidget::drawTrain(QVector< QPair<int,int> > positions, int size, float colorMod)
{
    glLineWidth((float)size);
    glBegin(GL_LINE_STRIP);
        glColor3f(0.3f, colorMod, 1.0f - colorMod );
        QPair<int,int> temp;
        foreach(temp, positions){
            glVertex2f(temp.first, temp.second);
        }

    glEnd();
}

void VectorGLWidget::drawCar(QVector< QPair < QPair<int,int>, int > > positions)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, textures[0]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f,1.0f,1.0f);
	
	float w = 15;///width();
	float h = 15;///height();
    QPair< QPair<int,int>,int> temp;
    foreach(temp, positions){
		glTranslatef(temp.first.first, temp.first.second, 0);
		glRotatef(temp.second, 0.0f, 0.0f, 1.0f );
    	glBegin(GL_QUADS);//start drawing a line loop
           	//glColor3f(0.0f,1.0f,1.0f);
            glVertex2f(-w,-h); glTexCoord2f(0,1);
            glVertex2f(-w, h); glTexCoord2f(1,1);
            glVertex2f( w, h); glTexCoord2f(1,0);
            glVertex2f( w,-h); glTexCoord2f(0,0);
    	glEnd();
		glLoadIdentity();
    }
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void VectorGLWidget::drawSemafor(Engine::SemaforState & s)
{    
	int size = 10;
	glEnable(GL_TEXTURE_2D);
	if(s.ifGreen())
		glBindTexture( GL_TEXTURE_2D, textures[1]);
	else
		glBindTexture( GL_TEXTURE_2D, textures[2]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(s.getPos().x,s.getPos().y,0);
	glRotatef(s.getAngle() + 90, 0.0f, 0.0f, 1.0f );
    glBegin(GL_QUADS);//start drawing a line loop
            glVertex2f(-size,-size); glTexCoord2f(0,1);
            glVertex2f(+size,-size); glTexCoord2f(1,1);
            glVertex2f(+size,+size); glTexCoord2f(1,0);
            glVertex2f(-size,+size); glTexCoord2f(0,0);
    glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void VectorGLWidget::drawTest(){

		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, textures[0]);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor3f(1.0f,1.0f,1.0f);

		glTranslatef(135, 135, 0);
		glRotatef(45, 0.0f, 0.0f, 1.0f );
		//glTranslatef(-125,-125, 0);
		float s = 15;
    	glBegin(GL_QUADS);//start drawing a line loop
            glVertex2f(-s,-s); glTexCoord2f(0,1);
            glVertex2f(-s, s); glTexCoord2f(1, 1);
            glVertex2f( s, s); glTexCoord2f( 1, 0);
            glVertex2f( s,-s); glTexCoord2f( 0,0);
    	glEnd();
		glDisable(GL_BLEND);
		glLoadIdentity();
}

bool VectorGLWidget::loadImage(QString filename,GLuint& texture){
	QImage t;
	QImage b;
    
	if ( !b.load( filename ) ){
	//	b = QImage( 16, 16, 32 );
	//	b.fill( Qt::green.rgb() );
		return false;
 	}
   
 	t = QGLWidget::convertToGLFormat( b );
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(),
			0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	
	return true;
}

void VectorGLWidget::loadTextures(){
    //loadImage("images/1-pociag-niebieski.png",textures[0]);
    textures[0] = bindTexture(QImage(":images/1-pociag-niebieski.png"));
	textures[1] = bindTexture(QImage("images/green light.png"));
 	textures[2] = bindTexture(QImage("images/red light.png"));
}

void VectorGLWidget::addStation(int x, int y)
{
    stationsSim.push_back(qMakePair(x,y));
}

void VectorGLWidget::addTrack(int beginX, int beginY, int endX, int endY)
{
    tracksSim.push_back(qMakePair(qMakePair(beginX,beginY),qMakePair(endX,endY)));
}

void VectorGLWidget::setTrains(QVector< QVector< QPair<int,int> > > positions)
{
    trainsSim = positions;
}

void VectorGLWidget::setCars(QVector< QVector< QPair < QPair<int,int>, int > > > positions)
{
    carsSim = positions;
}

void VectorGLWidget::setSemafors(QVector < QPair< QPair<int,int>, bool > > positions)
{
    semaforsSim = positions;
}

void VectorGLWidget::setSemafors(vector<Engine::SemaforState*>* s){
	semafors = s;
};

