#include <mappainter.h>
#define TILE_SIZE 50

MapPainter::MapPainter(QWidget *parent, QDomDocument *Doc): QWidget(parent), status(0), XmlContainer(Doc), beginX(0), beginY(0),mode(1),countTrains(0)
{
    for(int i = 0; i < MAXTEX; ++i)
        tileTextures[i] = QPixmap(tileTexturesSource[i]);
    setMinimumSize(QSize(1028,589));
}

void MapPainter::paintEvent(QPaintEvent *event)
{
    QPainter *qp=new QPainter(this);
    for(int i=beginX/TILE_SIZE; i<=(beginX+width())/TILE_SIZE;++i) {
        for(int j=beginY/TILE_SIZE;j<=(beginY+height())/TILE_SIZE;++j) {
            qp->drawPixmap(i*TILE_SIZE-beginX,j*TILE_SIZE-beginY,TILE_SIZE,TILE_SIZE,QPixmap(tileTextures[0]));
            int k;
            if(i<MAXX&&j<MAXY) {
            foreach(k, tiles[i][j])
              qp->drawPixmap(i*TILE_SIZE-beginX,j*TILE_SIZE-beginY,TILE_SIZE,TILE_SIZE,QPixmap(tileTextures[k]));
            }
         }
    }
    delete qp;
}

void MapPainter::mousePressEvent(QMouseEvent *event)
{
    if(mode==1){
        lastMousePosition=event->pos();
        return;
    }
    if(mode==2) {
        int x=(event->x()+beginX)/TILE_SIZE;
        int y=(event->y()+beginY)/TILE_SIZE;
        int k;
        foreach(k, tiles[x][y]) {
             if(k==13||k==14||k==15||k==16||k==17||k==18)  {
                 int train= QInputDialog::getInt(this,QString("Choose train"),QString("Train no(enter -1 to create new one):"));
                 if(train==-1)
                 {
                     int speed= QInputDialog::getInt(this,QString("Speed of train"),QString("Give the max speed of train"));
                     int len= QInputDialog::getInt(this,QString("Lenght of train"),QString("Give the length of train"));
                     bool Loop=QInputDialog::getInt(this,QString("loop"),QString("choose 1 to loop==true or 0 to loop==false"));
                     int Time= QInputDialog::getInt(this,QString("Time of train stop on station"),QString("Give the time of stop this train on station"));
                     QString name=QInputDialog::getText(this,QString("Name of Train"),QString("Give the name of train"));
                     addTrain(speed,len,Loop, name);
                     addStationToTrain(x,y,Time,countTrains-1);
                     return;
                 }
                 if(train>=countTrains || train<-1) {
                     QMessageBox msgBox;
                     msgBox.setText("There aren't train with number you have choosen.");
                     msgBox.exec();
                     return;
                 }
                 else
                 {
                     int Time= QInputDialog::getInt(this,QString("Time of train stop on station"),QString("Give the time of stop this train on station"));
                     addStationToTrain(x,y,Time,train);
                     return;
                 }
             }
        }
        return;
    }
    addTile((event->x()+beginX)/TILE_SIZE, (event->y()+beginY)/TILE_SIZE,status);
    this->update();
    return;
}

void MapPainter::mouseMoveEvent(QMouseEvent *event)
{
    if(mode==1)
    {
        int dx=event->x()-lastMousePosition.x();
        int dy=event->y()-lastMousePosition.y();
        if (event->buttons() & Qt::LeftButton) {
            beginX=beginX-dx;
            beginY=beginY-dy;
        }
        //map is in first wedge
        if(beginX < 0)
            beginX = 0;
        if(beginY < 0)
            beginY = 0;
        if(beginX>MAXX*TILE_SIZE-width())
            beginX=MAXX*TILE_SIZE-width();
        if(beginY>MAXY*TILE_SIZE-height())
            beginY=MAXY*TILE_SIZE-height();
        lastMousePosition = event->pos();
        this->update();
    }
}

void MapPainter::addTile(int x, int y, int no)
{
    if(no>MAXTEX)
        return;
    if(x>=0&&x<MAXX&&y>=0&&y<MAXY) {
        int it;
        foreach(it,tiles[x][y]) {
            if(it==no)
                return;
        }
        tiles[x][y].push_back(no);
        if(no==0)
            tiles[x][y].clear();
    }
    return;
}

void MapPainter::tileToDraw(int no)
{
    status=no;
}
void MapPainter::setStartPoint(int x, int y)
{
    beginX=x;
    beginY=y;
    this->update();
}
void MapPainter::clearMap()
{
     for(int i=0;i<MAXX;++i) {
         trains.clear();
         countTrains=0;
        for(int j=0;j<MAXY;++j) {
            tiles[i][j].clear();
        }
    }
    this->update();
}

void MapPainter::addTrain(const unsigned int maxspeed,const unsigned int length,const bool loop, const QString name)
{
    Train train(length,maxspeed,loop, name);
    trains.push_back(train);
    ++countTrains;
}

void MapPainter::addStationToTrain(const unsigned int x,const unsigned int y,const unsigned int delay, const unsigned int noTrain)
{
    trains[noTrain].addDestinationStation(x,y,delay);
}


void Train::addDestinationStation(unsigned int inputX, unsigned int inputY, unsigned inputDelay)
{
    Station newStation;
    newStation.delay=inputDelay;
    newStation.x=inputX;
    newStation.y=inputY;
    vecStations.push_back(newStation);
}
void Train::changeProperties(const unsigned int inputLength, const unsigned int inputMaxspeed, const bool inputLoop, const QString inputName)
{
    name=inputName;
    length=inputLength;
    maxspeed=inputMaxspeed;
    loop=inputLoop;
}

