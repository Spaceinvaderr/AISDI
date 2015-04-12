/* vim: set tabstop=4 shiftwidth=4 expandtab : */
#include"engine.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#define BUMPER 6
#define CARLEN 36
using namespace std;

Engine::Train::Train(Engine* caller, unsigned l, unsigned mSpeed, list<TrainStop>* targets, bool lp, string tName, vector<Stop>& tStations) :
    engine(caller), maxSpeed(mSpeed)
{
    if(maxSpeed > 42) maxSpeed = 42;
    else if(maxSpeed < 1) maxSpeed = 1; 
    loop = lp;
    length = l*CARLEN+BUMPER;
    //length = l - l%CARLEN + BUMPER;
    if(length < 42) length = 42;
    destinations = targets;
    fullDestinations = new list<TrainStop>();
    waitingFor = NULL;
    deadlock = 0;
    routeLength = 0;
    avgSpeed = 0.0;
    time = 0;
    idleTime = 0;
    name = tName;
    stations = tStations;
    opacity = 1.0f;
    prevDestination1 = destinations->begin()->junction;
    prevDestination2 = prevDestination1;
    routeLengthForSpeedCalculation = routeLength;
}

Engine::Train::~Train()
{
    delete destinations;
}

void Engine::Train::findRoute() throw(NoRoute)
{
    //list destinations stores the values of the target junctions
    list<TrainStop>* newDestinations = new list<TrainStop>();
    list<TrainStop>::iterator firstPoint = destinations->begin();
    list<TrainStop>::iterator secondPoint = destinations->begin();
    ++secondPoint;
    newDestinations->push_back(*firstPoint);
    while(secondPoint != destinations->end())
    {
        unsigned partLength = 0;
        list<TrainStop>* partResult = engine->findShortest(*firstPoint, 
        *secondPoint, &partLength);
        if(partResult == NULL) throw NoRoute();
        ++firstPoint;
        ++secondPoint;
        partResult->pop_front();
        while(partResult->size() > 0)
        {
            newDestinations->push_back(partResult->front());
            partResult->pop_front();
        }
        routeLength += partLength;
        delete partResult;
    }
    if(loop && destinations->back() != destinations->front())
    {
        unsigned partLength = 0;
        list<TrainStop>* partResult = engine->findShortest(destinations->back(), destinations->front(), &partLength);
        if(partResult == NULL) throw NoRoute();
        partResult->pop_front();
        while(partResult->size() > 0)
        {
            newDestinations->push_back(partResult->front());
            partResult->pop_front();
        }
        routeLength += partLength;
        delete partResult;
    }
    delete destinations;
    destinations = newDestinations;

    for(list<TrainStop>::iterator it = destinations->begin(); it!=destinations->end(); ++it)
    {
        fullDestinations->push_back(*it);
    }
    routeLength -= length;
    routeLengthForSpeedCalculation = routeLength;
}

bool Engine::Train::move(unsigned distance){
    if(deadlock) {
        speed = 0;
        return false;
    }
    unsigned pos = position + distance;
    const Track * t;
    list<const Track*> newTracks;
    list<TrainStop>::iterator dst = destinations->begin();
    newTracks.push_front(tracks.front());
    while(newTracks.front()->getLength() < pos){
        pos = pos - newTracks.front()->getLength();
        if((*dst).junction == newTracks.front()->getEnd()){
            if((*dst).ifWait()){
                if(speed == 0) (*dst).wait();
                return false;
            }
            ++dst;
        }
        if(dst == destinations->end()) {
            if(speed == 0) {         
                time = engine->getIterations();
                avgSpeed = ((float) routeLengthForSpeedCalculation)/((float) time);
            }
            return false;
        } 
        t = newTracks.front()->getEnd()->getNextTrack(*((*dst).junction));
        if(t==NULL) return false;
        
        if(!(t->getSegment().canReserve(this))) return false;
        if(engine->getNoOfTilesAt(t->getTile()->pos) > 1)
        {
            for(list<const Track*>::iterator it = tracks.begin(); it != tracks.end(); ++it)
            {
                if(t->getTile()==(*it)->getTile()) continue;
                TileDesc* tTile;
                TileDesc* itTile;
                if((tTile = t->getTile())->pos == (itTile = (*it)->getTile())->pos &&
                    itTile->occupied && ((itTile->edges | tTile->edges) == 0x1111 && (itTile->edges == 0x1100 || itTile->edges == 0x0011)))
                {
                    deadlockAtTile=itTile;
                    deadlock++;
                    return false;
                }
            }
        }
        t->getTile()->occupied = true;
        newTracks.push_front(t);
    }
    position = pos;
    newTracks.pop_back();
    while(newTracks.size()){
        t = newTracks.back();
        bool tst = t->getSegment().reserve(this);
        assert(tst);
        tracks.push_front(t);
        newTracks.pop_back();
    }
    bool TurnAround = false;
    if(loop)
    {
        while(destinations->begin() != dst)
        {
            Junction* j = (destinations->begin()++)->junction;
            if(j->getPos().x == prevDestination2->getPos().x && j->getPos().y == prevDestination2->getPos().y){
                TurnAround = true;
            }
            prevDestination2 = prevDestination1;
            prevDestination1 = j;

            destinations->push_back(destinations->front());
            destinations->pop_front();
        }
    }
    else
    {
        while(destinations->begin() != dst)
        {
            Junction* j = (destinations->begin()++)->junction;
            if(j->getPos().x == prevDestination2->getPos().x && j->getPos().y == prevDestination2->getPos().y){
                TurnAround = true;
            }
            prevDestination2 = prevDestination1;
            prevDestination1 = j;

            destinations->pop_front();
        }
    }
    //if(TurnAround){
        //routeLengthForSpeedCalculation -= length-CARLEN-BUMPER;
        // return move(length-CARLEN-BUMPER);
    //}
    return true;
}

void Engine::Train::update(){
    if(time) //we are at the final station
    {
        if(opacity == 0.6f) return;
        if(opacity <= 0.62f)
        {
            opacity = 0.6f;
            //releasing tracks
            for(list< const Track* >::iterator it = tracks.begin(); it!=tracks.end(); ++it)
            {
                (*it)->getSegment().release(this);
                (*it)->getTile()->occupied = false;
            }
        }
        else opacity -= 0.03f;
        return;
    }
    if(cars.size()>2 && (((cars[0].second+180)%360 == (cars[1].second)%360) || ((cars[0].second)%360 == (cars[1].second+180)%360))){
        routeLengthForSpeedCalculation -= length-CARLEN-BUMPER;
        move(length-CARLEN-BUMPER);
     }
    if(!deadlock && move(speed)){
        if(speed < maxSpeed && move(speed +1))
            ++speed;
    }
    else
        if(speed > 0){
            if(deadlock && !deadlockAtTile->occupied) { deadlockAtTile = NULL; deadlock=0; }
            else
            {
                --speed;
                if(deadlock && speed==0) engine->deadlocks.push_back(positions.front());
            }
        }


    assert(tracks.size()>0);

    unsigned logLength; // logical length - train length + breaking distance 
    logLength = length + ((speed-1)*speed)/2;

    //Updating positions
    Semafor* s;
    list< const Track* >::iterator it = tracks.begin();

    // Sending signals to semafors
    s = (*it)->getOutSemafor();
    if(s!=NULL) s->reset();
    s = (*it)->getInSemafor();
    if(s!=NULL) s->set();

    //if(deadlock) return;
    // Iterating through reserved tracks in front of train
    unsigned len = position;
    unsigned carFront = (((speed-1)*speed)/2)+BUMPER;
    while(it != tracks.end() && len < carFront ) {
        ++it;
            s = (*it)->getInSemafor();
            if(s!=NULL) s->set();
        len += (*it)->getLength();
    }

    // Iterating through tracks occupied by train
    positions.clear();
    cars.clear();
    positions.push_back((*it)->getPartPos(len - carFront ));

    unsigned carsLen = CARLEN/2 + carFront;

    // Sending signal to semafor
    s = (*it)->getInSemafor();
    if(s!=NULL) s->reset();

    while(carsLen <= len && len < logLength ){
        pair<Pos,int> p;
        p.first = (*it)->getPartPos(len - carsLen);
        p.second = (*it)->getAngle();
        perfectAngle(p.first, &(p.second), (*it));
        cars.push_back(p);
        carsLen+=CARLEN;
    } 

    ++it;
    while(it != tracks.end() && len < logLength) {
        positions.push_back((*it)->getEnd()->getPos());
        len += (*it)->getLength();
        
        while(carsLen <= len && len < logLength ){
            pair<Pos,int> p;
            p.first = (*it)->getPartPos(len - carsLen);
            p.second = (*it)->getAngle();
            perfectAngle(p.first, &(p.second), (*it));
            cars.push_back(p);
            carsLen+=CARLEN;
        } 
        ++it;
    }

    //Releasing back tracks
    for(list< const Track* >::iterator itt = it ;itt!=tracks.end();++itt)
    {
        (*itt)->getSegment().release(this);
        (*itt)->getTile()->occupied = false;
    }
    
    tracks.erase(it, tracks.end());
    //Updating end postion
    positions.push_back(tracks.back()->getPartPos(len-logLength));
    while(carsLen <= logLength){
        pair<Pos,int> p;
        p.first = tracks.back()->getPartPos(len - carsLen);
        p.second = tracks.back()->getAngle();
        perfectAngle(p.first, &(p.second), (*it));
        cars.push_back(p);
        carsLen+=CARLEN;
    }
    if(speed == 0) idleTime++;
};

void Engine::Train::init() throw(reserved){
    assert(destinations->size()>=2);
    speed=0;
    position=0;
    TrainStop j = destinations->front();
    destinations->pop_front();
    const Track* t = j.junction->getNextTrack(*(destinations->front().junction));
    if(!(t->getSegment().reserve(this))) throw reserved();
    tracks.push_front(t);
    if(t==NULL) throw reserved();
    assert(length>0);
    if(length<maxSpeed) speed--;
    if(!move(length)) throw reserved();
    positions.push_back(tracks.front()->getPartPos(position));
    for(list<const Track*>::iterator it = tracks.begin();it != tracks.end();++it)
        positions.push_back((*it)->getStart()->getPos());
}

void Engine::Train::setWaitingFor(Engine::Train* train)
{
    waitingFor = train;
}
Engine::Train* Engine::Train::getWaitingFor()
{
    return waitingFor;
}

void Engine::Train::incDeadlock()
{
    deadlock++;
}

unsigned Engine::Train::getDeadlock()
{
    return deadlock;
}

void Engine::Train::reset()
{
    destinations->clear();
    for(list<TrainStop>::iterator it = fullDestinations->begin(); it != fullDestinations->end(); ++it)
    {
        destinations->push_back(*it);
    }
    tracks.clear();
    positions.clear();
    cars.clear();
    waitingFor = NULL;
    deadlock = 0;
    time = 0;
    avgSpeed = 0.0;
    idleTime = 0;
    speed = 0;
    opacity = 1.0f;
    deadlockAtTile = NULL;
    routeLengthForSpeedCalculation = routeLength;
}

void Engine::Train::perfectAngle(Pos p, int* val, const Track* t)
{
    TileDesc* trainTile = t->getTile();
    TileDesc* tile;
    while((tile = engine->getNextTurn(p)) != NULL)
    {
        if(trainTile == tile)
        {
            unsigned xSquare, ySquare;
            double distLeft, fullDist;
            fullDist = 35;
            switch(tile->tileNo)
            {
            case 5:
                xSquare = tile->pos.x+50-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y+25-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val <= -(90 - distLeft/fullDist * 90) && *val >-20) break;
                *val = -(90 - distLeft/fullDist * 90);
                break;
            case 6:
                xSquare = tile->pos.x-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y+25-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val >= 90 - distLeft/fullDist * 90 && *val < 20) break;
                *val = 90 - distLeft/fullDist * 90;
                break;
            case 7:
                xSquare = tile->pos.x+50-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y+25-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val >= -(90 + distLeft/fullDist * 90) && *val<-160) break;
                *val = -(90 + distLeft/fullDist * 90);
                break;
            case 8:
                xSquare = tile->pos.x-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y+25-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val <= 90 + distLeft/fullDist * 90 && *val>160) break;
                *val = 90 + distLeft/fullDist * 90;
                break;
            case 9:
                xSquare = tile->pos.x+25-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y+50-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val <= distLeft/fullDist * 90 && *val>70) break;
                *val = distLeft/fullDist * 90;
                break;
            case 10:
                xSquare = tile->pos.x+25-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val >= 180-distLeft/fullDist * 90 && *val < 110) break;
                *val = 180-distLeft/fullDist * 90;
                break;
            case 11:
                xSquare = tile->pos.x+25-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y+50-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val >= -(distLeft/fullDist * 90) && *val<-70) break;
                *val = -(distLeft/fullDist * 90);
                break;
            case 12:
                xSquare = tile->pos.x+25-p.x;
                xSquare *= xSquare;
                ySquare = tile->pos.y-p.y;
                ySquare *= ySquare;
                distLeft = sqrt(xSquare+ySquare);
                if(*val <= distLeft/fullDist * 90 - 180 && *val>-110) break;
                *val = distLeft/fullDist * 90 - 180;
                break;
            case 21:
                if(*val<=0)
                {
                    xSquare = tile->pos.x+50-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y+25-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val <= -(90 - distLeft/fullDist * 90) && *val >-20) break;
                    *val = -(90 - distLeft/fullDist * 90);
                }
                else
                {
                    xSquare = tile->pos.x+25-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val >= 180-distLeft/fullDist * 90 && *val < 110) break;
                    *val = 180-distLeft/fullDist * 90;
                }
                break;
            case 22:
                if(*val>=0)
                {
                    xSquare = tile->pos.x-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y+25-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val >= 90 - distLeft/fullDist * 90 && *val < 20) break;
                    *val = 90 - distLeft/fullDist * 90;
                }
                else
                {
                    xSquare = tile->pos.x+25-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val <= distLeft/fullDist * 90 - 180 && *val>-110) break;
                    *val = distLeft/fullDist * 90 - 180;
                }
                break;
            case 23:
                if(*val<0)
                {
                    xSquare = tile->pos.x+50-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y+25-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val >= -(90 + distLeft/fullDist * 90) && *val<-160) break;
                    *val = -(90 + distLeft/fullDist * 90);
                }
                else
                {
                    xSquare = tile->pos.x+25-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y+50-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val <= distLeft/fullDist * 90 && *val>70) break;
                    *val = distLeft/fullDist * 90;
                }
                break;
            case 24:
                if(*val>0)
                {
                    xSquare = tile->pos.x-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y+25-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val <= 90 + distLeft/fullDist * 90 && *val>160) break;
                    *val = 90 + distLeft/fullDist * 90;
                }
                else
                {
                    xSquare = tile->pos.x+25-p.x;
                    xSquare *= xSquare;
                    ySquare = tile->pos.y+50-p.y;
                    ySquare *= ySquare;
                    distLeft = sqrt(xSquare+ySquare);
                    if(*val >= -(distLeft/fullDist * 90) && *val<-70) break;
                    *val = -(distLeft/fullDist * 90);
                }
                break;
            default:
                break;
            }
        }
    }
    engine->resetTileIterator();
}
