#include "decodexml.h"

DecodeXml::DecodeXml(Engine *eng, TileGLWidget *tileSimWidg) //: QWidget(0)
{
	engine = eng;	
	tileSimWidget = tileSimWidg;
	data = new QDomDocument;
}
DecodeXml::~DecodeXml()
{
	//delete data;
}

bool DecodeXml::parseXML(const QString &fileName) throw(QString)
{
	QFile *file = new QFile(fileName);
	if(!data->setContent(file))
		throw(tr("I can't setContent for file"));
	QDomElement docElem = data->documentElement();
	//checking if root name is proper
	if(docElem.tagName() != "engine")
		throw(tr("Invalid root name"));
	QDomNodeList nodeList = docElem.elementsByTagName("tile");
	//ckeck all nodes named tile
	for(int i =0; i < nodeList.count(); i++)
	{
		QDomElement el = nodeList.at(i).toElement();
		//checking if attributes are valid
		if(!el.hasAttribute(QString("xPos"))||!el.hasAttribute(QString("yPos"))||!el.hasAttribute(QString("tileNo")))
			throw(tr("Attributes of tile are invalid"));
		engine->addTile(el.attribute(QString("xPos")).toUInt(0,10),el.attribute(QString("yPos")).toUInt(0,10),el.attribute(QString("tileNo")).toUInt(0,10));
		tileSimWidget->addTile(el.attribute(QString("xPos")).toUInt(0,10),el.attribute(QString("yPos")).toUInt(0,10),el.attribute(QString("tileNo")).toUInt(0,10));
	}
	//Parse textfile map inside xml file if it exists.
	nodeList = docElem.elementsByTagName("tilemap");
    	for(int i = 0; i < nodeList.count(); i++)
	{
        	QDomElement el = nodeList.at(i).toElement();
		QString textmap = el.text();
        	if(!parseTileMap(textmap,el.attribute(QString("xPos"),"0").toUInt(0,10),el.attribute(QString("yPos"),"0").toUInt(0,10)))
			throw tr("TileMap inside <tilemap></tilemap> is invalid.\n");
	}
	nodeList = docElem.elementsByTagName("train");
	//check all nodes named train
	QString errorName;
	for(int i = 0 ; i < nodeList.count(); i++)
	{
		QDomElement el = nodeList.at(i).toElement();
		QDomNode pEntries = el.firstChild();
		while(!pEntries.isNull())
		{
			QDomElement peData = pEntries.toElement();
			QString tagNam = peData.tagName();
			//checkiong name of train's child
			if(tagNam.toStdString()!="station")
				throw(tr("Wrong train's child name. 'station' expected."));
			if(!peData.hasAttribute(QString("x"))||!peData.hasAttribute(QString("y")))
				throw(tr("Attributes of a station are invalid"));
			//adding station destination of train route to temp vector
			tempvec.push_back(
				Engine::Stop(
					Engine::Pos(
						peData.attribute(QString("x")).toUInt(0,10),
						peData.attribute(QString("y")).toUInt(0,10)
					),
					peData.hasAttribute(QString("time")) ? peData.attribute(QString("time")).toInt(0,10) : 0 
				)
			);
			pEntries = pEntries.nextSibling();
		}
        if(!el.hasAttribute(QString("length"))||!el.hasAttribute(QString("maxspeed")))
			throw(tr("Attributes of a train are invalid"));
		errorName.clear();
		try
		{
			engine->addTrain(el.attribute(QString("length")).toUInt(0,10),
				el.attribute(QString("maxspeed")).toUInt(0,10), tempvec,
				el.hasAttribute(QString("loop")) ? el.attribute(QString("loop")).toUInt(0,10) : 0,
				el.hasAttribute(QString("name")) ? el.attribute(QString("name")).toStdString() : "NoName" );
		}
		catch(Engine::NoStation)
		{
			errorName = tr("Unable to find the specified station.");
		}
		catch(Engine::NoRoute)
		{
			errorName = tr("There is no connections between given stations.");
		}
		catch(Engine::reserved)
		{
			errorName = tr("Some trains are placed too close to each other");
		}
		catch(...)
		{
			errorName = tr("Unexcepted error occured");
		}
		tempvec.clear();
		if(errorName.size())
			throw errorName;
	}
	//iteration feature
	/*	nodeList=docElem.elementsByTagName("iteration");
	//ckeck only one node named iteration
	QDomElement el=nodeList.at(0).toElement();
	if(!el.hasAttribute(QString("state"))){throw(QString("Node iteration has not an attribute named state."));}
	return  el.attribute(QString("state")).toUInt(0,10);*/
	return 1;
}

int DecodeXml::getIteration()
{
	QDomNodeList nodeList = data->elementsByTagName("iteration");
    if(nodeList.count()!=0) {
        QDomElement ele=nodeList.at(nodeList.count()-1).toElement();
        return ele.attribute(QString("num")).toUInt(0,10);
    }
    return 0;
}

void DecodeXml::setIteration(int value)
{
	QDomNodeList nodeList = data->elementsByTagName("iteration");
	if(nodeList.count() != 0) {
		QDomElement ele = nodeList.at(0).toElement();
        ele.setAttribute(QString("num"),value);
        return;
    }
	if(nodeList.count() == 0)
    {
		QDomElement ele = data->createElement(QString("iteration"));
        ele.setAttribute(QString("num"),value);
		QDomElement root = data->documentElement();
        root.appendChild(ele);
        return;
    }
}

bool DecodeXml::saveToFile(QString &path)
{

    QFile outFile(path);
	if(!outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
	  return false;
    QTextStream stream(&outFile);
	stream << data->toString();
    outFile.close();
	return true;
}
