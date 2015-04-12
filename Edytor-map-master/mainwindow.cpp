
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //set minimum window size
    setMinimumSize(QSize(1028, 668));
    //set icons size
    setIconSize(QSize(30,30));
    createActions();
    createMenus();
    createToolBars();
    createMainWidget();
    mapwidget->setStartPoint(0,1000*50-mapwidget->height());
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMainWidget()
{
    contentsWindow = new QDockWidget(tr("Table of Contents"), this);
    contentsWindow->setAllowedAreas(Qt::LeftDockWidgetArea
                                     | Qt::RightDockWidgetArea);
    contentsWindow->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, contentsWindow);
    mapwidget=new MapPainter(this, Doc);
    contentsWindow->setWidget(mapwidget);

}

void MainWindow::createActions()
{
    newAct=new QAction(tr("&New"), this);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    openAct=new QAction(tr("&Open"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    saveAct=new QAction(tr("&Save"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));
    exitAct=new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
    No0=new QAction(QIcon(":/tiles/0_x.png"), tr("&No0"), this);
    No0->setIconVisibleInMenu(true);
    connect(No0, SIGNAL(triggered()), this, SLOT(ChooseTile0()));
    No1=new QAction(QIcon(":/tiles/1.png"), tr("&No1"), this);
    No1->setIconVisibleInMenu(true);
    connect(No1, SIGNAL(triggered()), this, SLOT(ChooseTile1()));
    No2=new QAction(QIcon(":/tiles/2.png"), tr("&No2"), this);
    No2->setIconVisibleInMenu(true);
    connect(No2, SIGNAL(triggered()), this, SLOT(ChooseTile2()));
    No3=new QAction(QIcon(":/tiles/3.png"), tr("&No3"), this);
    No3->setIconVisibleInMenu(true);
    connect(No3, SIGNAL(triggered()), this, SLOT(ChooseTile3()));
    No4=new QAction(QIcon(":/tiles/4.png"), tr("&No4"), this);
    No4->setIconVisibleInMenu(true);
    connect(No4, SIGNAL(triggered()), this, SLOT(ChooseTile4()));
    No5=new QAction(QIcon(":/tiles/5.png"), tr("&No5"), this);
    No5->setIconVisibleInMenu(true);
    connect(No5, SIGNAL(triggered()), this, SLOT(ChooseTile5()));
    No6=new QAction(QIcon(":/tiles/6.png"), tr("&No6"), this);
    No6->setIconVisibleInMenu(true);
    connect(No6, SIGNAL(triggered()), this, SLOT(ChooseTile6()));
    No7=new QAction(QIcon(":/tiles/7.png"), tr("&No7"), this);
    No7->setIconVisibleInMenu(true);
    connect(No7, SIGNAL(triggered()), this, SLOT(ChooseTile7()));
    No8=new QAction(QIcon(":/tiles/8.png"), tr("&No8"), this);
    No8->setIconVisibleInMenu(true);
    connect(No8, SIGNAL(triggered()), this, SLOT(ChooseTile8()));
    No9=new QAction(QIcon(":/tiles/9.png"), tr("&No9"), this);
    No9->setIconVisibleInMenu(true);
    connect(No9, SIGNAL(triggered()), this, SLOT(ChooseTile9()));
    No10=new QAction(QIcon(":/tiles/10.png"), tr("&No10"), this);
    No10->setIconVisibleInMenu(true);
    connect(No10, SIGNAL(triggered()), this, SLOT(ChooseTile10()));
    No11=new QAction(QIcon(":/tiles/11.png"), tr("&No11"), this);
    No11->setIconVisibleInMenu(true);
    connect(No11, SIGNAL(triggered()), this, SLOT(ChooseTile11()));
    cursor=new QAction(QIcon(":/tiles/cursor.png"),QString("Cursor"),this);
    connect(cursor,SIGNAL(triggered()),this,SLOT(changeMode()));
    No12=new QAction(QIcon(":/tiles/12.png"), tr("&No12"), this);
    No12->setIconVisibleInMenu(true);
    connect(No12, SIGNAL(triggered()), this, SLOT(ChooseTile12()));
    No13=new QAction(QIcon(":/tiles/13.png"), tr("&No13"), this);
    No13->setIconVisibleInMenu(true);
    connect(No13, SIGNAL(triggered()), this, SLOT(ChooseTile13()));
    No14=new QAction(QIcon(":/tiles/14.png"), tr("&No14"), this);
    No14->setIconVisibleInMenu(true);
    connect(No14, SIGNAL(triggered()), this, SLOT(ChooseTile14()));
    No15=new QAction(QIcon(":/tiles/15.png"), tr("&No15"), this);
    No15->setIconVisibleInMenu(true);
    connect(No15, SIGNAL(triggered()), this, SLOT(ChooseTile15()));
    No16=new QAction(QIcon(":/tiles/16.png"), tr("&No16"), this);
    No16->setIconVisibleInMenu(true);
    connect(No16, SIGNAL(triggered()), this, SLOT(ChooseTile16()));
    No17=new QAction(QIcon(":/tiles/17.png"), tr("&No17"), this);
    No17->setIconVisibleInMenu(true);
    connect(No17, SIGNAL(triggered()), this, SLOT(ChooseTile17()));
    No18=new QAction(QIcon(":/tiles/18.png"), tr("&No18"), this);
    No18->setIconVisibleInMenu(true);
    connect(No18, SIGNAL(triggered()), this, SLOT(ChooseTile18()));
    No19=new QAction(QIcon(":/tiles/19.png"), tr("&No18"), this);
    No19->setIconVisibleInMenu(true);
    connect(No19, SIGNAL(triggered()), this, SLOT(ChooseTile19()));
    No20=new QAction(QIcon(":/tiles/20.png"), tr("&No20"), this);
    No20->setIconVisibleInMenu(true);
    connect(No20, SIGNAL(triggered()), this, SLOT(ChooseTile20()));
    No21=new QAction(QIcon(":/tiles/21.png"), tr("&No21"), this);
    No21->setIconVisibleInMenu(true);
    connect(No21, SIGNAL(triggered()), this, SLOT(ChooseTile21()));
    No22=new QAction(QIcon(":/tiles/22.png"), tr("&No22"), this);
    No22->setIconVisibleInMenu(true);
    connect(No22, SIGNAL(triggered()), this, SLOT(ChooseTile22()));
    No23=new QAction(QIcon(":/tiles/23.png"), tr("&No23"), this);
    No23->setIconVisibleInMenu(true);
    connect(No23, SIGNAL(triggered()), this, SLOT(ChooseTile23()));
    No24=new QAction(QIcon(":/tiles/24.png"), tr("&No24"), this);
    No24->setIconVisibleInMenu(true);
    connect(No24, SIGNAL(triggered()), this, SLOT(ChooseTile24()));
    No37=new QAction(QIcon(":/tiles/37.png"), tr("&No37"), this);
    No37->setIconVisibleInMenu(true);
    connect(No37, SIGNAL(triggered()), this, SLOT(ChooseTile37()));
    No38=new QAction(QIcon(":/tiles/38.png"), tr("&No38"), this);
    No38->setIconVisibleInMenu(true);
    connect(No38, SIGNAL(triggered()), this, SLOT(ChooseTile38()));
    createtrain=new QAction(tr("Trains"),this);
    createtrain->setIconVisibleInMenu(true);
    connect(createtrain,SIGNAL(triggered()),this,SLOT(createTrain()));
}

void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);



}

void MainWindow::createToolBars()
{
    TileNo0ToolBar=addToolBar(tr("&Tiles"));
    TileNo1ToolBar=addToolBar(tr("&Tiles"));
    TileNo0ToolBar->addAction(No0);
    TileNo0ToolBar->addAction(No1);
    TileNo0ToolBar->addAction(No2);
    TileNo0ToolBar->addAction(No3);
    TileNo0ToolBar->addAction(No4);
    TileNo0ToolBar->addAction(No5);
    TileNo0ToolBar->addAction(No6);
    TileNo0ToolBar->addAction(No7);
    TileNo0ToolBar->addAction(No8);
    TileNo0ToolBar->addAction(No9);
    TileNo0ToolBar->addAction(No10);
    TileNo0ToolBar->addAction(No11);
    TileNo0ToolBar->addAction(No12);
    TileNo0ToolBar->addAction(No13);
    TileNo0ToolBar->addAction(No14);
    TileNo0ToolBar->addAction(No15);
    TileNo0ToolBar->addAction(No16);
    TileNo1ToolBar->addAction(No17);
    TileNo1ToolBar->addAction(No18);
    TileNo1ToolBar->addAction(No19);
    TileNo1ToolBar->addAction(No20);
    TileNo1ToolBar->addAction(No21);
    TileNo1ToolBar->addAction(No22);
    TileNo1ToolBar->addAction(No23);
    TileNo1ToolBar->addAction(No24);
    TileNo0ToolBar->addAction(No37);
    TileNo0ToolBar->addAction(No38);
    TileNo0ToolBar->addAction(cursor);
    TileNo0ToolBar->addAction(createtrain);
}

void MainWindow::newFile()
{
    mapwidget->clearMap();
}
void MainWindow::saveFile()
{
    Doc=new QDomDocument();
    QDomElement root = Doc->createElement("engine");
    Doc->appendChild(root);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString("../"), tr("XML files (*.xml)"));
    QString type(".xml");
    if(!fileName.endsWith(".xml")) fileName.append(type);
    QFile outFile(fileName);
    if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
      qDebug( "Failed to open file for writing." );
      return;
    }
    root=Doc->documentElement();
    QDomElement tag;
    int it;
    for(int i = 0; i<MAXX;i=i+1) {
        for(int j=0;j<MAXY;j=j+1) {
            foreach(it, mapwidget->tiles[i][j]) {
                if(it!=0){
                    tag=Doc->createElement("tile");
                    tag.setAttribute(QString("xPos"),double(i));
                    tag.setAttribute(QString("yPos"),double(1000-j));
                    tag.setAttribute(QString("tileNo"),double(it));
                }
            root.appendChild(tag);
            }
        }
    }
    for(int i = 0; i<mapwidget->getAmountOfTrains();i=i+1) {
        QVector<Train> trainsToSave=mapwidget->getTrains();
        tag=Doc->createElement("train");
        tag.setAttribute(QString("maxspeed"),double(trainsToSave[i].getMaxspeed()));
        tag.setAttribute(QString("length"),double(trainsToSave[i].getLenght()));
        tag.setAttribute(QString("loop"),double(trainsToSave[i].getLoop()));
        tag.setAttribute(QString("name"),trainsToSave[i].getName());
        QVector<Train::Station> stationsToSave=trainsToSave[i].getStations();
        int j=0;
        for(Train::Station station=stationsToSave.at(0);j<stationsToSave.size();++j,station=stationsToSave.at(j)) {
            QDomElement child=Doc->createElement("station");
            child.setAttribute(QString("x"),double(station.x));
            child.setAttribute(QString("y"),double(1000-station.y));
            child.setAttribute(QString("time"),station.delay);
            tag.appendChild(child);
        }
        root.appendChild(tag);
    }
    QTextStream stream(&outFile);
    stream<<Doc->toString();
    outFile.close();
    return;
}

void MainWindow::openFile()
{
    mapwidget->clearMap();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString("../"), tr("XML files (*.xml)"));
    if(fileName==0) return;
    QFile *file = new QFile(fileName);
    Doc= new QDomDocument(fileName);
    if(!Doc->setContent(file)){ return; }// throw(QString("I can't setContent for file")); }
    QDomElement docElem = Doc->documentElement();
    //checking if root name is proper
    if(docElem.tagName() != "engine"){ throw(QString("Invalid root name")); }
    QDomNodeList nodeList = docElem.elementsByTagName("tile");
    //ckeck all nodes named tile
    for(int i =0; i < nodeList.count(); i++)
    {
        QDomElement el = nodeList.at(i).toElement();
        //checking if attributes are valid
        if(!el.hasAttribute(QString("xPos"))||!el.hasAttribute(QString("yPos"))||!el.hasAttribute(QString("tileNo"))){throw(QString("Attributes of tile are invalid"));}
        int x=el.attribute(QString("xPos")).toUInt(0,10);
        int y=el.attribute(QString("yPos")).toUInt(0,10);
        mapwidget->addTile(x,1000-y,el.attribute(QString("tileNo")).toUInt(0,10));
    }
    nodeList = docElem.elementsByTagName("train");
    //check all nodes named train

    for(int i = 0 ; i < nodeList.count(); i++)
    {
        QDomElement el = nodeList.at(i).toElement();
        QDomNode pEntries = el.firstChild();
        QDomElement peData = pEntries.toElement();
        QString tagNam = peData.tagName();
        //if(!el.hasAttribute(QString("length"))||!el.hasAttribute(QString("maxspeed"))){throw(QString("Attributes of train are invalid"));}
        mapwidget->addTrain(el.attribute(QString("maxspeed")).toUInt(0,10),el.attribute(QString("length")).toUInt(0,10),el.attribute(QString("loop")).toUInt(0,10),QString("name"));
        std::cout << el.attribute(QString("maxspeed")).toUInt(0,10) << " " << el.attribute(QString("length")).toUInt(0,10) << std::endl;
        while(!pEntries.isNull())
        {

            QDomElement peData = pEntries.toElement();
            QString tagNam = peData.tagName();
            //checkiong name of train's child
           // if(tagNam.toStdString()!="station"){throw(QString("Wrong name for child of train. It should be 'station;"));}
            //if(!peData.hasAttribute(QString("x"))||!peData.hasAttribute(QString("y"))){throw(QString("Attributes of station are invalid")); }
            //adding station destination of train route to temp vector
            mapwidget->addStationToTrain(peData.attribute(QString("x")).toUInt(0,10),1000-peData.attribute(QString("y")).toUInt(0,10),peData.attribute(QString("time")).toUInt(0,10), i);
            pEntries = pEntries.nextSibling();
        }

     }
    mapwidget->update();
}

void MainWindow::exit()
{

}

void MainWindow::ChooseTile0()
{
    mapwidget->tileToDraw(0);
    mapwidget->mode=0;
    QPixmap tile(":/cursor/0.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);

}
void MainWindow::ChooseTile1()
{

    mapwidget->tileToDraw(1);
    mapwidget->mode=0;
    QPixmap tile(":/cursor/1.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);

}
void MainWindow::ChooseTile2()
{
    mapwidget->tileToDraw(2);
    mapwidget->mode=0;
    QPixmap tile(":/cursor/2.png");
     cur=new QCursor(tile);
     mapwidget->setCursor(*cur);
}
void MainWindow::ChooseTile3()
{
    mapwidget->tileToDraw(3);
    QPixmap tile(":/cursor/3.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile4()
{
    mapwidget->tileToDraw(4);
    QPixmap tile(":/cursor/4.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile5()
{
    mapwidget->tileToDraw(5);
    mapwidget->mode=0;
    QPixmap tile(":/cursor/5.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
}
void MainWindow::ChooseTile6()
{
    mapwidget->tileToDraw(6);
    QPixmap tile(":/cursor/6.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile7()
{
    mapwidget->tileToDraw(7);
    QPixmap tile(":/cursor/7.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile8()
{
    mapwidget->tileToDraw(8);
    QPixmap tile(":/cursor/8.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile9()
{
    mapwidget->tileToDraw(9);
     QPixmap tile(":/cursor/9.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile10()
{
    mapwidget->tileToDraw(10);
    mapwidget->mode=0;
    QPixmap tile(":/cursor/10.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
}
void MainWindow::ChooseTile11()
{
    mapwidget->tileToDraw(11);
    QPixmap tile(":/cursor/11.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile12()
{
    mapwidget->tileToDraw(12);
    QPixmap tile(":/cursor/12.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile13()
{
    mapwidget->tileToDraw(13);
    QPixmap tile(":/cursor/13.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile14()
{
    mapwidget->tileToDraw(14);
    QPixmap tile(":/cursor/14.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile15()
{
    mapwidget->tileToDraw(15);
    QPixmap tile(":/cursor/15.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile16()
{
    mapwidget->tileToDraw(16);
    QPixmap tile(":/cursor/16.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}

void MainWindow::ChooseTile17()
{
    mapwidget->tileToDraw(17);
    QPixmap tile(":/cursor/17.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile18()
{
    mapwidget->tileToDraw(18);
    QPixmap tile(":/cursor/18.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;

}
void MainWindow::ChooseTile19()
{
    mapwidget->tileToDraw(19);
    mapwidget->mode=0;
     QPixmap tile(":/cursor/19.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
}
void MainWindow::ChooseTile20()
{
    mapwidget->tileToDraw(20);
    QPixmap tile(":/cursor/20.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile21()
{
    mapwidget->tileToDraw(21);
    QPixmap tile(":/cursor/21.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile22()
{
    mapwidget->tileToDraw(22);
    QPixmap tile(":/cursor/22.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile23()
{
    mapwidget->tileToDraw(23);
    QPixmap tile(":/cursor/23.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile24()
{
    mapwidget->tileToDraw(24);
    QPixmap tile(":/cursor/24.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile37()
{
    mapwidget->tileToDraw(37);
    QPixmap tile(":/cursor/37.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}
void MainWindow::ChooseTile38()
{
    mapwidget->tileToDraw(38);
    QPixmap tile(":/cursor/38.png");
    cur=new QCursor(tile);
    mapwidget->setCursor(*cur);
    mapwidget->mode=0;
}


void MainWindow::changeMode()
{
    mapwidget->mode=1;
    cur=new QCursor(Qt::SizeAllCursor);
    mapwidget->setCursor(*cur);
}
void MainWindow::createTrain()
{
    mapwidget->mode=2;
    cur=new QCursor(Qt::CrossCursor);
    mapwidget->setCursor(*cur);

}
