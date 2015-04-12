#include "tileglwidget.h"
#include <math.h>
#include <iostream>
#define TILESIZE 50

TileGLWidget::TileGLWidget (MainWindow *window, Engine *eng, QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), mainWindow(window), engine(eng),  highlighted(NULL), lastNumOfDeadlocks(0), zoomOpt(3), beginX(0), beginY(0), mode(1)
{
    oldTexture = -1;
}

TileGLWidget ::~TileGLWidget ()
{

}

void TileGLWidget ::initializeGL()
{
//	std::cout << "initializeGL" << std::endl;
    static int init = 0;
    glEnable (GL_BLEND);
    glDisable (GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (beginX, beginX+zoomValues[zoomOpt]*width() , beginY, beginY+zoomValues[zoomOpt]*height(), 0, 1);
    glMatrixMode (GL_MODELVIEW);
    if(init == 0){
        for(int i = 0; i < MAXTEX; ++i)
            tileTextures[i] = bindTexture(QImage(tileTexturesSource[i]));
        for(int i = 0; i < OTHTEX; ++i)
            otherTextures[i] = bindTexture(QImage(otherTexturesSource[i]));
    }
    init++;
}


void TileGLWidget ::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    //glClear(GL_COLOR_BUFFER_BITT);
    int it;
    //draw border
    glEnable(GL_TEXTURE_2D);
    for(int i = beginX/50; i <= (beginX + zoomValues[zoomOpt]*width())/50; ++i)
        for(int j = beginY/50; j <= (beginY + zoomValues[zoomOpt]*height())/50; ++j){
            drawTile(i,j,0);

        }
    glDisable(GL_TEXTURE_2D);
    //draw active stations border
    if(highlighted != NULL)
    {
        for(size_t i = 0; i < highlighted->stations.size(); ++i)
        {
            drawBorder(highlighted->stations[i].x / TILESIZE , highlighted->stations[i].y / TILESIZE);
        }
    }
    glEnable(GL_TEXTURE_2D);
    //draw tiles
    for(int i = beginX/50; i <= (beginX + zoomValues[zoomOpt]*width())/50 && i < MAXX; ++i)
        for(int j = beginY/50; j <= (beginY + zoomValues[zoomOpt]*height())/50 && j < MAXX; ++j){
            foreach(it, tiles[i][j])
                drawTile(i,j,it);
        }
    if(beginX + zoomValues[zoomOpt]*width()/50 >= MAXX || (beginY + zoomValues[zoomOpt]*height())/50 >= MAXX){
        QPair< QPair<int,int>, int>  it;
        foreach (it, farTiles) {
            if(it.first.first >=  beginX/50 &&  it.first.first <= (beginX + zoomValues[zoomOpt]*width())/50 && it.first.second >= beginY/50 && it.first.second <=  (beginY + zoomValues[zoomOpt]*height())/50)
                drawTile(it.first.first, it.first.second, it.second);

        }
    }
    //draw semafors and trains
    //when there isn't any opened simulation, *engine is null
    if(engine != NULL){
        Engine::SemaforState* d;
        foreach (d, engine->semaforStates) {
            drawSemafor(*d);
        }
        const Engine::TrainState *t;
        foreach (t, engine->trainStates) {
            drawCar(*t);
        }
    }
    //draw deadlocks
    if(lastNumOfDeadlocks != engine->deadlocks.size())
    {
        if(engine->deadlocks.size() == 0)
        {
            lastNumOfDeadlocks = 0;
        }
        else
        {
            lastNumOfDeadlocks = engine->deadlocks.size();
            //QMessageBox::warning(this, tr("Deadlock!"), tr("There is a deadlock at") + " (" + QString::number(engine->deadlocks[engine->deadlocks.size() - 1].x/TILESIZE) +  "," + QString::number(engine->deadlocks[engine->deadlocks.size() - 1].y/TILESIZE) + ')');
            emit informEmit(tr("There is a deadlock at") + " (" + QString::number(engine->deadlocks[engine->deadlocks.size() - 1].x/TILESIZE) +  "," + QString::number(engine->deadlocks[engine->deadlocks.size() - 1].y/TILESIZE) + ')');
        }
    }
    for(size_t i = 0; i < engine->deadlocks.size(); ++i)
    {
        drawSkull(engine->deadlocks[i]);
    }
    glDisable(GL_TEXTURE_2D);
}


void TileGLWidget::resizeGL(int w, int h)
{
    initializeGL();
    glViewport(0, 0, w, h);
}

void TileGLWidget ::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        lastMousePosition = event->pos();
    }
    if(event->button() == Qt::RightButton && engine != NULL){
        if(mode == 1){
            QMenu menu(this);
            menu.addAction(tr("Get distance from here"));
            menu.exec(event->globalPos());
            lastMeasurePosition= event->pos();
            lastBegin = qMakePair(beginX, beginY);
            zoomBeginValue = zoomValues[zoomOpt];
            mode++;
            return;
        }
        if(mode == 2){
            QMenu menu(this);
            menu.addAction(tr("Get distance to here"));
            menu.exec(event->globalPos());
            int result = 0;
            try{
                //std::cout << "Coor" << " x" << (lastMeasurePosition.x()+lastBegin.first)*zoomBeginValue/TILESIZE << " y " << (height() - lastMeasurePosition.y()+lastBegin.second)*zoomBeginValue/TILESIZE << " x "<< (event->pos().x()+beginX)*zoomValues[zoomOpt]/TILESIZE << "y" << (height() - event->pos().y()+beginY)*zoomValues[zoomOpt]/TILESIZE << std::endl;
                //std::cout << "LastMousePosition x" << lastMeasurePosition.x() << "LastMousePosition y " << lastMeasurePosition.y() << "Last begin x" <<  lastBegin.first << " Last begin y" << lastBegin.second <<  "MousePosition x" << event->pos().x() << "Mouse Postion y" << event->pos().y() << "begin x" << beginX << "begin y" <<  beginY << std::endl;
                result = engine->getDistance((lastMeasurePosition.x()*zoomBeginValue+lastBegin.first)/TILESIZE, ((height() - lastMeasurePosition.y())*zoomBeginValue+lastBegin.second)/TILESIZE, (event->pos().x()*zoomValues[zoomOpt]+beginX)/TILESIZE, ((height() - event->pos().y())*zoomValues[zoomOpt]+beginY)/TILESIZE);

                QString informationMsg = tr("The distance equals to ") + QString::number(result);
                QMessageBox::information(this, tr("Distance found"), informationMsg);
            }catch(Engine::NoRoute e)
            {
                QMessageBox::information(this, tr("No route"), tr("There is no route between these stations"));
            }catch(Engine::NoStation e)
            {
                QMessageBox::information(this, tr("No station"), tr("Path must begin and finish at stations"));
            }
            lastMousePosition = event->pos();
            mode = 1;
            return;
        }
    }
}

void TileGLWidget ::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        //QMessageBox::information(this, tr("Working"), tr("working"));
        int dx = event->x() - lastMousePosition.x();
        int dy = event->y() - lastMousePosition.y();
        if (event->buttons() & Qt::LeftButton) {
            beginX -= dx;
            beginY += dy;
        }
        //map is in first wedge
        if(beginX < 0)
            beginX = 0;
        if(beginY < 0)
            beginY = 0;
        lastMousePosition = event->pos();
        initializeGL();
        updateGL();
    }
}

void TileGLWidget ::animate()
{
    updateGL();
}


void TileGLWidget::drawTile(int x, int y, int type)
{
    if(oldTexture != tileTextures[type]){
        glBindTexture(GL_TEXTURE_2D, tileTextures[type]);
        oldTexture = tileTextures[type];
    }
    // make a rectangle
    glTranslatef(x*TILESIZE, y*TILESIZE, 0);
    glBegin(GL_QUADS);
        // top left
        glTexCoord2i(0, 0);
        glVertex3f(0,0,0);
        // top right
        glTexCoord2i(1, 0);
        glVertex3f(TILESIZE, 0, 0);
        // bottom right
        glTexCoord2i(1, 1);
        glVertex3f(TILESIZE, TILESIZE, 0);
        // bottom left
        glTexCoord2i(0, 1);
        glVertex3f(0, TILESIZE, 0);
    glEnd();
    glLoadIdentity();
}

void TileGLWidget::drawBorder(int x, int y)
{
    glLineWidth(1.0f/zoomValues[zoomOpt]);
    glTranslatef(x*TILESIZE, y*TILESIZE, 0);
    glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(TILESIZE, 0, 0);
        glVertex3f(TILESIZE, TILESIZE, 0);
        glVertex3f(0, TILESIZE, 0);
    glEnd();
    glLoadIdentity();
}

void TileGLWidget::addTile(int x, int y, int type)
{
    if(type > MAXTEX)
        type = MAXTEX - 1;
    if(x < MAXX && y < MAXY)
        tiles[x][y].push_back(type);
    else
        farTiles.push_back(qMakePair(qMakePair(x,y), type));
}


void TileGLWidget::drawCar(const Engine::TrainState &t)
{
    //glColor4f(1.0, 0.0, 0.0, 0.0f);
    if(&t != highlighted){
        if(oldTexture != otherTextures[0]){
            glBindTexture(GL_TEXTURE_2D, otherTextures[0]);
            oldTexture = otherTextures[0];
        }
    }
    else{
        if(oldTexture != otherTextures[4]){
            glBindTexture(GL_TEXTURE_2D, otherTextures[4]);
            oldTexture = otherTextures[4];
        }
    }
    float w = 15;///width();
    float h = 15;///height();
    pair< Engine::Pos, int > temp;
    glColor4f(1.0f,1.0f,1.0f, t.opacity);
    glLoadIdentity();
    foreach(temp, t.cars){
        glTranslatef(temp.first.x, temp.first.y, 0);
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
    glLoadIdentity();
    glColor4f(1.0,1.0,1.0,1.0);
    //glClear(GL_COLOR_BUFFER_BIT)
    //glClearColor(0.0f,0.0f,0.0f,1.0f);
}

void TileGLWidget::drawSemafor(Engine::SemaforState & s)
{
    int size = 10;
    if(s.ifGreen())
        glBindTexture( GL_TEXTURE_2D, otherTextures[1]);
    else
        glBindTexture( GL_TEXTURE_2D, otherTextures[2]);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f,1.0f,1.0f);
    glTranslatef(s.getPos().x,s.getPos().y,0);
    glRotatef(s.getAngle() + 90, 0.0f, 0.0f, 1.0f );
    glBegin(GL_QUADS);//start drawing a line loop
            glVertex2f(-size,-size); glTexCoord2f(0,1);
            glVertex2f(+size,-size); glTexCoord2f(1,1);
            glVertex2f(+size,+size); glTexCoord2f(1,0);
            glVertex2f(-size,+size); glTexCoord2f(0,0);
    glEnd();
    //glDisable(GL_BLEND);
    glLoadIdentity();
}

void TileGLWidget::drawSkull(Engine::Pos &position)
{
    int size = TILESIZE / 2;
    glEnable(GL_TEXTURE_2D);
    if(oldTexture != otherTextures[3]){
        glBindTexture( GL_TEXTURE_2D, otherTextures[3]);
        oldTexture = otherTextures[3];
    }
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f,1.0f,1.0f);
    glTranslatef(position.x,position.y,0);
    glBegin(GL_QUADS);//start drawing a line loop
        glVertex2f(+size,-size); glTexCoord2f(0,1);
        glVertex2f(+size,+size); glTexCoord2f(1,1);
        glVertex2f(-size,+size); glTexCoord2f(1,0);
        glVertex2f(-size,-size); glTexCoord2f(0,0);
    glEnd();
    glLoadIdentity();
}

void TileGLWidget::clearAll()
{
    for(int i = 0; i < MAXX; ++i)
        for(int j = 0; j < MAXY; ++j)
            tiles[i][j].clear();
    farTiles.clear();
    highlighted = NULL;
    lastNumOfDeadlocks = 0;
}

void TileGLWidget::zoomIn()
{
    if(zoomOpt < SCALEVAL - 1)
    {
        zoomOpt++;
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        glOrtho (beginX, beginX+zoomValues[zoomOpt]*width() , beginY, beginY+zoomValues[zoomOpt]*height(), 0, 1);
        glMatrixMode (GL_MODELVIEW);initializeGL();
        updateGL();
    }
}

void TileGLWidget::zoomOut()
{
    if(zoomOpt > 0)
    {
        zoomOpt--;
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        glOrtho (beginX, beginX+zoomValues[zoomOpt]*width() , beginY, beginY+zoomValues[zoomOpt]*height(), 0, 1);
        glMatrixMode (GL_MODELVIEW);initializeGL();
        updateGL();
    }
}


void TileGLWidget::higlightedChange(const Engine::TrainState * t)
{
    highlighted = t;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (beginX, beginX+zoomValues[zoomOpt]*width() , beginY, beginY+zoomValues[zoomOpt]*height(), 0, 1);
    glMatrixMode (GL_MODELVIEW);initializeGL();
    updateGL();
}
