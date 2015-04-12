#include <QFile>
#include <QString>
#include "decodexml.h"


struct SRPTile{
	QString line1, line2, line3;
	int size;
	int numTiles;
	void GetSize();
	bool ReadFromStream(QTextStream &input);
	qint64 GetTileNumber(int index);
};

void inline SRPTile::GetSize(){
	size = line1.size();
	int temp = line2.size();
	if(temp<size)
		size = temp;
	temp = line3.size();
	if(temp<size)
		size = temp;
	size++;
	int rest = size%4;
	size -= rest;
	numTiles = size/4;
	size--;
}

bool SRPTile::ReadFromStream(QTextStream &input){
	if (input.atEnd()) return false; // incomplete row
	line1 = input.readLine();
	if (input.atEnd()) return false; // incomplete row
	line2 = input.readLine();
	if (input.atEnd()) return false; // incomplete row
	line3 = input.readLine();
	line1.toLower();
	line2.toLower();
	line3.toLower();
	GetSize();
	return true;
}

qint64 SRPTile::GetTileNumber(int index){
	if(index >= numTiles)
		return 0;
	int i = index*4;
	qint64 result = 0;
	char a1, a2, a3;
	a1 = line1.at(i).toLatin1();
	a2 = line1.at(i+1).toLatin1();
	a3 = line1.at(i+2).toLatin1();
	char b1, b2, b3;
	b1 = line2.at(i).toLatin1();
	b2 = line2.at(i+1).toLatin1();
	b3 = line2.at(i+2).toLatin1();
	char c1, c2, c3;
	c1 = line3.at(i).toLatin1();
	c2 = line3.at(i+1).toLatin1();
	c3 = line3.at(i+2).toLatin1();

	//decorations
	if(b2=='1'){
		//tree
		return (qint64)1 << (37 - 1);
	}
	if(b2=='2'){
		//rock
		return (qint64)1 << (38 - 1);
	}

	if(a1=='x')
		a1 = ' ';

    bool StraightRails = b2=='b';
	if(!StraightRails){
		switch(b2){
			case 'u':
                result |= 1 << 1;
				StraightRails = true;
				break;
			case 'd':
                result |= 1 << 2;
				StraightRails = true;
				break;
			case 'l':
                result |= 1 << 3;
				StraightRails = true;
				break;
			case 'r':
                result |= 1 << 4;
				StraightRails = true;
				break;
			default:
				break;
		}
	}

    //one-directional
	//"straights"
	if(a2=='u' && StraightRails && c2=='u'){
		//up
        result |= 1 << 1;
	}
	if(a2=='d' && StraightRails && c2=='d'){
		//down
        result |= 1 << 2;
	}
	if(b1=='l' && StraightRails && b3=='l'){
		//left
        result |= 1 << 3;
	}
	if(b1=='r' && StraightRails && b3=='r'){
		//right
        result |= 1 << 4;
	}

	//turnings
	if(c3=='u' || c3=='r'){
		//turn up-right
		result |= 1 << 5;
	}
	if(c1=='u' || c1=='l'){
		//turn up-left
		result |= 1 << 6;
	}
	if(a3=='d' || a3=='r'){
		//turn down-right
		result |= 1 << 7;
	}
	if(a1=='d' || a1=='l'){
		//turn down-left
		result |= 1 << 8;
	}
	if(a3=='l' || a3=='u'){
		//turn left-up
		result |= 1 << 9;
	}
	if(c3=='l' || c3=='d'){
		//turn left-down
		result |= 1 << 10;
	}
	if(a1=='r' || a1=='u'){
		//turn right-up
		result |= 1 << 11;
	}
	if(c1=='r' || c1=='d'){
		//turn right-down
		result |= 1 << 12;
	}

	//stations
	if(a2=='u' && b2=='s' && c2=='u'){
		//up
        result |= 1 << 13;
	}
	if(a2=='d' && b2=='s' && c2=='d'){
		//down
        result |= 1 << 14;
	}
	if(b1=='l' && b2=='s' && b3=='l'){
		//left
        result |= 1 << 15;
	}
	if(b1=='r' && b2=='s' && b3=='r'){
		//right
        result |= 1 << 16;
	}

	//"both"-directional
	//stations
	if(a2=='b' && b2=='s' && c2=='b'){
		//up/down
        result |= 1 << 17;
	}
	if(b1=='b' && b2=='s' && b3=='b'){
		//left/right
        result |= 1 << 18;
	}

	//"straights"
	if(a2=='b' && StraightRails && c2=='b'){
		//up/down
        result |= 1 << 19;
	}
	if(b1=='b' && StraightRails && b3=='b'){
		//left/right
        result |= 1 << 20;
	}

	//turnings
	if(c3=='b'){
		//bottom and right
		result |= 1 << 21;
	}
	if(c1=='b'){
		//bottom and left
		result |= 1 << 22;
	}
	if(a3=='b'){
		//top and right
		result |= 1 << 23;
	}
	if(a1=='b'){
		//top and left
		result |= 1 << 24;
	}

    return result>>1;
}

int DecodeXml::parseTileMap(QString &text,int XPos, int YPos) {
	if(text==0)
        return 0; // wrong input data

	QTextStream input(&text);
    input.readLine(); // nothing better for skipping a line??

	vector<SRPTile*> TileLines;
	SRPTile* FirstRow = new SRPTile();
	TileLines.push_back(FirstRow);
	if(!FirstRow->ReadFromStream(input))
        return 0;// incomplete data
	if(FirstRow->size < 3)
		return 0;// the only tile is not complete

	//skip the line
	if(!input.atEnd())
		input.readLine();
    //data not ended?
	if (!input.atEnd()){
        //ok, so load the whole data
		SRPTile* NextRow = new SRPTile();
		bool DeleteLastRow = true;
		while(NextRow->ReadFromStream(input)){
			TileLines.push_back(NextRow);
			if (input.atEnd()){
				DeleteLastRow = false;
				break;
			}
			input.readLine(); // nothing better for skipping line??
			NextRow = new SRPTile();
		}
		if(DeleteLastRow){
			delete NextRow;
		}
    }

	int MapHeight = TileLines.size();

	//Find the MapWidth
	int MapWidth = 0;
	for(int i = 0; i < MapHeight; i++){
		int numTiles = TileLines[i]->numTiles;
		if(numTiles > MapWidth)
			MapWidth = numTiles;
	}
    //Paint our art.
	for(int y = 0; y < MapHeight; y++){
		SRPTile* Row = TileLines[y];
		for(int x = 0; x < MapWidth; x++){
			qint64 tile = Row->GetTileNumber(x);
			qint64 flag = 1;
			for(int i = 1; i <= 39; i++){
				if(flag&tile){
					engine->addTile(x+XPos,MapHeight-1-y+YPos,i);
					tileSimWidget->addTile(x+XPos,MapHeight-1-y+YPos,i);
				}
                flag <<= (qint64)1;
			}
		}
	}

    //do the clean up.
    while(TileLines.size()){
        delete TileLines.back();
        TileLines.pop_back();
    }

	return 1;
}
