#ifndef MAPPAINTER_H
#define MAPPAINTER_H
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <iostream>
#include <QMouseEvent>
#include <QDomDocument>
#include <QDomElement>
#include <QInputDialog>
#include <QMessageBox>
#define MAXTRAINS 1000
#define MAXX 1000
#define MAXY 1000
#define MAXTEX 41

class Train;
class MainWindow;
class MapPainter:public QWidget
{
    Q_OBJECT
public:
    MapPainter(QWidget *parent, QDomDocument *Doc);
    void tileToDraw(int no);
    void setStartPoint(int x,int y);
    void clearMap();
    QVector<Train> getTrains(){return trains;}
    unsigned getAmountOfTrains(){return countTrains;}
protected:
    void paintEvent(QPaintEvent *event);
    void paintEvent(QPaintEvent *event, int x, int y);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void addTile(int x, int y, int no);
    void addTrain(const unsigned int maxspeed,const unsigned int length,const bool loop, const QString name);
    void addStationToTrain(const unsigned int x,const unsigned int y,const unsigned int delay, const unsigned int noTrain);
    int beginX;
    int beginY;
    QPoint lastMousePosition;
    QVector<int> tiles[MAXX][MAXY];
    QVector<Train> trains;
    int countTrains;
    QPixmap tileTextures[MAXTEX];
    const QString tileTexturesSource[MAXTEX] = {":tiles/0_x.png", ":tiles/1.png", ":tiles/2.png", ":tiles/3.png", ":tiles/4.png", ":tiles/5.png", ":tiles/6.png", ":tiles/7.png", ":tiles/8.png", ":tiles/9.png", ":tiles/10.png", ":tiles/11.png", ":tiles/12.png", ":tiles/13.png", ":tiles/14.png", ":tiles/15.png", ":tiles/16.png", ":tiles/17.png", ":tiles/18.png", ":tiles/19.png", ":tiles/20.png", ":tiles/21.png", ":tiles/22.png", ":tiles/23.png", ":tiles/24.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/null.png", ":tiles/37.png", ":tiles/38.png", ":tiles/39.png", ":tiles/40.png"};
    int status;
    QDomDocument *XmlContainer;
    int mode;
    friend class MainWindow;

};

class Train
{
public:
    Train() {}
    Train(const unsigned int inputLength, const unsigned int inputMaxspeed, const bool inputLoop, const QString inputName): length(inputLength), maxspeed(inputMaxspeed), loop(inputLoop), name(inputName)  {}
    ~Train() {vecStations.clear();}
    void addDestinationStation(unsigned int inputX, unsigned int inputY, unsigned inputDelay);
    struct Station {
        unsigned x;
        unsigned y;
        unsigned delay;
    };
    void changeProperties(const unsigned int inputLength, const unsigned int inputMaxspeed, const bool inputLoop, const QString inputName);
    QVector<Station> getStations(){return vecStations;}
    unsigned getLenght(){return length;}
    unsigned getMaxspeed(){return maxspeed;}
    unsigned getLoop(){return loop;}
    QString getName(){return name;}
private:
    QString name;
    unsigned length;
    unsigned maxspeed;
    unsigned loop;
    QVector<Station> vecStations;
};



#endif // MAPPAINTER_H
