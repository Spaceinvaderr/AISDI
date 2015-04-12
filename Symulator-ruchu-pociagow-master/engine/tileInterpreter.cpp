#define TILESIZE 50
#define RIGHT 0
#define BOTTOM 1
#define LEFT 2
#define TOP 3
#include <iostream>
#include "engine.hpp"
using namespace std;

/* Explanation of unsigned short Engine::TileDesc::edges :
	0xabcd
	a-up, b-down, c-left, d-right
	a,b,c,d are either 0 or 1
*/

void Engine::addTile(unsigned xPos, unsigned yPos, unsigned tileNo)
{
	Segment* segment = NULL;
	xPos *= TILESIZE;
	yPos *= TILESIZE;

	Pos edges[4];
	edges[RIGHT].x = xPos+TILESIZE;
	edges[RIGHT].y = yPos+TILESIZE/2;
	edges[BOTTOM].x = xPos+TILESIZE/2;
	edges[BOTTOM].y = yPos;
	edges[LEFT].x = xPos;
	edges[LEFT].y = yPos+TILESIZE/2;
	edges[TOP].x = xPos+TILESIZE/2;
	edges[TOP].y = yPos+TILESIZE;
	
	TileDesc* tile = getTileAt(Pos(xPos, yPos));
	if(tile != NULL)
	{
		segment = tile->segment;
	}
	switch(tileNo)
	{
	case 1:
		//straight up
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1100);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[BOTTOM]);
		addJunction(edges[TOP]);
		segments.push_back(segment);
		addTrack(edges[BOTTOM], edges[TOP], TILESIZE, tile);
		break;
	case 2:
		//straight down
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1100);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[TOP]);
		addJunction(edges[BOTTOM]);
		segments.push_back(segment);
		addTrack(edges[TOP], edges[BOTTOM], TILESIZE, tile);
		break;
	case 3:
		//straight left
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0011);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[RIGHT]);
		addJunction(edges[LEFT]);
		segments.push_back(segment);
		addTrack(edges[RIGHT], edges[LEFT], TILESIZE, tile);
		break;
	case 4:
		//straight right
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0011);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[LEFT]);
		addJunction(edges[RIGHT]);
		segments.push_back(segment);
		addTrack(edges[LEFT], edges[RIGHT], TILESIZE, tile);
		break;
	case 5:
		//turn up->right
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0101);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[BOTTOM]);
		addJunction(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100);
		addJunction(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100);
		addJunction(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100);
		addJunction(edges[RIGHT]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE/2, yPos, xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, xPos+TILESIZE, yPos+TILESIZE/2, TILESIZE/4, tile);
		break;
	case 6:
		//turn up->left
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0110);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[BOTTOM]);
		addJunction(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100);
		addJunction(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100);
		addJunction(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100);
		addJunction(edges[LEFT]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE/2, yPos, xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, xPos, yPos+TILESIZE/2, TILESIZE/4, tile);
		break;
	case 7:
		//turn down->right
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1001);
	        if (segment == NULL) segment = addGetSegment();
	        addJunction(edges[TOP]);
	        addJunction(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100);
	        addJunction(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100);
	        addJunction(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100);
	        addJunction(edges[RIGHT]);
	        segments.push_back(segment);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE, xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, xPos+TILESIZE, yPos+TILESIZE/2, TILESIZE/4, tile);
	        break;
	case 8:
		//turn down->left
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1010);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[TOP]);
		addJunction(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100);
		addJunction(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100);
		addJunction(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100);
		addJunction(edges[LEFT]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE/2, yPos+TILESIZE, xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, xPos, yPos+TILESIZE/2, TILESIZE/4, tile);
		break;
	case 9:
		//turn left->up
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1001);
	        if (segment == NULL) segment = addGetSegment();
	        addJunction(edges[RIGHT]);
	        addJunction(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100);
	        addJunction(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100);
	        addJunction(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100);
	        addJunction(edges[TOP]);
	        segments.push_back(segment);
	        addTrack(xPos+TILESIZE, yPos+TILESIZE/2, xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, xPos+TILESIZE/2, yPos+TILESIZE, TILESIZE/4, tile);
	        break;
	case 10:
		//turn left->down
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0101);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[RIGHT]);
		addJunction(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100);
		addJunction(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100);
		addJunction(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100);
		addJunction(edges[BOTTOM]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE, yPos+TILESIZE/2, xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
		addTrack(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, xPos+TILESIZE/2, yPos, TILESIZE/4, tile);
		break;
	case 11:
		//turn right->up
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1010);
	        if (segment == NULL) segment = addGetSegment();
	        addJunction(edges[LEFT]);
	        addJunction(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100);
	        addJunction(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100);
	        addJunction(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100);
	        addJunction(edges[TOP]);
	        segments.push_back(segment);
	        addTrack(xPos, yPos+TILESIZE/2, xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, xPos+TILESIZE/2, yPos+TILESIZE, TILESIZE/4, tile);
	        break;
	case 12:
		//turn right->down
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0110);
	        if (segment == NULL) segment = addGetSegment();
	        addJunction(edges[LEFT]);
	        addJunction(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100);
	        addJunction(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100);
	        addJunction(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100);
	        addJunction(edges[BOTTOM]);
	        segments.push_back(segment);
	        addTrack(xPos, yPos+TILESIZE/2, xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, xPos+TILESIZE/2, yPos, TILESIZE/4, tile);
	        break;
	case 13:
		//station - straight up
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1100);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[BOTTOM]);
		addJunction(xPos+TILESIZE/2, yPos+TILESIZE/2);
		addJunction(edges[TOP]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE/2, yPos, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
		addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos+TILESIZE, TILESIZE/2, tile);
		stations.push_back(new Station(getJunction(Pos(xPos+TILESIZE/2, yPos+TILESIZE/2)), xPos/TILESIZE, yPos/TILESIZE));
		break;
	case 14:
		//station - straight down
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1100);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[TOP]);
		addJunction(xPos+TILESIZE/2, yPos+TILESIZE/2);
		addJunction(edges[BOTTOM]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE/2, yPos+TILESIZE, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
		addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos, TILESIZE/2, tile);
		stations.push_back(new Station(getJunction(Pos(xPos+TILESIZE/2, yPos+TILESIZE/2)), xPos/TILESIZE, yPos/TILESIZE));
		break;
	case 15:
		//station - straight left
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0011);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[RIGHT]);
		addJunction(xPos+TILESIZE/2, yPos+TILESIZE/2);
		addJunction(edges[LEFT]);
		segments.push_back(segment);
		addTrack(xPos+TILESIZE, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
		addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos, yPos+TILESIZE/2, TILESIZE/2, tile);
		stations.push_back(new Station(getJunction(Pos(xPos+TILESIZE/2, yPos+TILESIZE/2)), xPos/TILESIZE, yPos/TILESIZE));
		break;
	case 16:
		//station - straight right
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0011);
		if (segment == NULL) segment = addGetSegment();
		addJunction(edges[LEFT]);
		addJunction(xPos+TILESIZE/2, yPos+TILESIZE/2);
		addJunction(edges[RIGHT]);
		segments.push_back(segment);
		addTrack(xPos, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
		addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos+TILESIZE, yPos+TILESIZE/2, TILESIZE/2, tile);
		stations.push_back(new Station(getJunction(Pos(xPos+TILESIZE/2, yPos+TILESIZE/2)), xPos/TILESIZE, yPos/TILESIZE));
		break;
	case 17:
		//station - up/down
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1100);
	        addJunction(edges[TOP]);
	        addJunction(xPos+TILESIZE/2, yPos+TILESIZE/2);
	        addJunction(edges[BOTTOM]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[TOP]), getJunction(edges[BOTTOM]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[TOP]), getJunction(edges[BOTTOM]), segment));
	        }
	        segments.push_back(segment);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos, TILESIZE/2, tile);
	
	        addTrack(xPos+TILESIZE/2, yPos, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos+TILESIZE, TILESIZE/2, tile);
	
	        stations.push_back(new Station(getJunction(Pos(xPos+TILESIZE/2, yPos+TILESIZE/2)), xPos/TILESIZE, yPos/TILESIZE));
		break;
	case 18:
		//station - left/right
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0011);
	        addJunction(edges[LEFT]);
	        addJunction(xPos+TILESIZE/2, yPos+TILESIZE/2);
	        addJunction(edges[RIGHT]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[LEFT]), getJunction(edges[RIGHT]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[LEFT]), getJunction(edges[RIGHT]), segment));
	        }
	        segments.push_back(segment);
	        addTrack(xPos, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos+TILESIZE, yPos+TILESIZE/2, TILESIZE/2, tile);
	
	        addTrack(xPos+TILESIZE, yPos+TILESIZE/2, xPos+TILESIZE/2, yPos+TILESIZE/2, TILESIZE/2, tile);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE/2, xPos, yPos+TILESIZE/2, TILESIZE/2, tile);
	
	        stations.push_back(new Station(getJunction(Pos(xPos+TILESIZE/2, yPos+TILESIZE/2)), xPos/TILESIZE, yPos/TILESIZE));
	        break;
	case 19:
		//straight up/down
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1100);
		addJunction(edges[TOP]);
		addJunction(edges[BOTTOM]);
		if(segment == NULL)
		{
			segment = getSegmentFor(getJunction(edges[TOP]), getJunction(edges[BOTTOM]));
		}
		else
		{
			joinSegments(segment, getSegmentFor(getJunction(edges[TOP]), getJunction(edges[BOTTOM]), segment));
		}
		segments.push_back(segment);
		addTrack(edges[TOP], edges[BOTTOM], TILESIZE, tile);
		addTrack(edges[BOTTOM], edges[TOP], TILESIZE, tile);
		break;
	case 20:
		//straight left/right
		tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0011);
		addJunction(edges[LEFT]);
	        addJunction(edges[RIGHT]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[LEFT]), getJunction(edges[RIGHT]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[LEFT]), getJunction(edges[RIGHT]), segment));
	        }
		segments.push_back(segment);
		addTrack(edges[LEFT], edges[RIGHT], TILESIZE, tile);
		addTrack(edges[RIGHT], edges[LEFT], TILESIZE, tile);
		break;
	case 21:
	        //turn up->right / left->down
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0101);
	        addJunction(edges[BOTTOM]);
	        addJunction(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100);
	        addJunction(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100);
	        addJunction(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100);
	        addJunction(edges[RIGHT]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[BOTTOM]), getJunction(edges[RIGHT]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[BOTTOM]), getJunction(edges[RIGHT]), segment));
	        }
	        segments.push_back(segment);
	        addTrack(xPos+TILESIZE/2, yPos, xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, xPos+TILESIZE, yPos+TILESIZE/2, TILESIZE/4, tile);
	
	        addTrack(xPos+TILESIZE, yPos+TILESIZE/2, xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+79*TILESIZE/100, yPos+46*TILESIZE/100, xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+64*TILESIZE/100, yPos+36*TILESIZE/100, xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+53*TILESIZE/100, yPos+20*TILESIZE/100, xPos+TILESIZE/2, yPos, TILESIZE/4, tile);
		break;
	case 22:
		//turn up->left / right->down
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x0110);
	        addJunction(edges[BOTTOM]);
	        addJunction(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100);
	        addJunction(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100);
	        addJunction(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100);
	        addJunction(edges[LEFT]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[BOTTOM]), getJunction(edges[LEFT]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[BOTTOM]), getJunction(edges[LEFT]), segment));
	        }
	        segments.push_back(segment);
	        addTrack(xPos+TILESIZE/2, yPos, xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, xPos, yPos+TILESIZE/2, TILESIZE/4, tile);
	
	        addTrack(xPos, yPos+TILESIZE/2, xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+21*TILESIZE/100, yPos+46*TILESIZE/100, xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+36*TILESIZE/100, yPos+36*TILESIZE/100, xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+47*TILESIZE/100, yPos+20*TILESIZE/100, xPos+TILESIZE/2, yPos, TILESIZE/4, tile);
		break;
	case 23:
		//turn down->right / left->up
	        //ok?
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1001);
	        if (segment == NULL) segment = addGetSegment();
	        addJunction(edges[TOP]);
	        addJunction(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100);
	        addJunction(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100);
	        addJunction(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100);
	        addJunction(edges[RIGHT]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[TOP]), getJunction(edges[RIGHT]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[TOP]), getJunction(edges[RIGHT]), segment));
	        }
	        segments.push_back(segment);
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE, xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, xPos+TILESIZE, yPos+TILESIZE/2, TILESIZE/4, tile);
	
	        addTrack( xPos+TILESIZE, yPos+TILESIZE/2, xPos+79*TILESIZE/100, yPos+54*TILESIZE/100,TILESIZE/4, tile);
	        addTrack(xPos+79*TILESIZE/100, yPos+54*TILESIZE/100, xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+64*TILESIZE/100, yPos+64*TILESIZE/100, xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+53*TILESIZE/100, yPos+80*TILESIZE/100, xPos+TILESIZE/2, yPos+TILESIZE, TILESIZE/4, tile);
	        break;
	case 24:
		//turn down->left / right->up
			tile = new TileDesc(xPos, yPos, tileNo, segment, 0x1010);
	        if (segment == NULL) segment = addGetSegment();
	        addJunction(edges[TOP]);
	        addJunction(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100);
	        addJunction(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100);
	        addJunction(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100);
	        addJunction(edges[LEFT]);
	        if(segment == NULL)
	        {
	            segment = getSegmentFor(getJunction(edges[TOP]), getJunction(edges[LEFT]));
	        }
	        else
	        {
	            joinSegments(segment, getSegmentFor(getJunction(edges[TOP]), getJunction(edges[LEFT]), segment));
	        }
	        segments.push_back(segment);
	
	        addTrack(xPos+TILESIZE/2, yPos+TILESIZE, xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, xPos, yPos+TILESIZE/2, TILESIZE/4, tile);
	
	        addTrack(xPos, yPos+TILESIZE/2, xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+21*TILESIZE/100, yPos+54*TILESIZE/100, xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+36*TILESIZE/100, yPos+64*TILESIZE/100, xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, TILESIZE/4, tile);
	        addTrack(xPos+47*TILESIZE/100, yPos+80*TILESIZE/100, xPos+TILESIZE/2, yPos+TILESIZE, TILESIZE/4, tile);
	        break;
	default:
		if(segment != NULL) throw InvalidMap();
		return;
	}
	segments.pop_back();
	tile->segment = segment;
	tiles.push_back(tile);
}
