#include <iostream>
#include"engine.hpp"
using namespace std;

Engine::Junction::Junction(unsigned x, unsigned y)
{
    position.x = x;
    position.y = y;
    ifCrossing = false;
}

Engine::Junction::~Junction()
{
    for(vector<Route*>::iterator it = outTracks.begin(); it != outTracks.end(); ++it)
    {
     delete((*it)->track);
     delete *it;
    }
}

Engine::Pos Engine::Junction::getPos() const
{
    return position;
}

unsigned Engine::Junction::getNoOfOutTracks()
{
    return outTracks.size();
}

const Engine::Track* Engine::Junction::getNextTrack( const Junction& dest) const {
    if(ifCrossing==false){
        if(outTracks.size() == 1) return outTracks[0]->track;
        assert(outTracks.size()==0);
        return NULL;
        }
    for(vector< Route* >::iterator it = ((Junction *)this)->outTracks.begin();
        it != ((Junction *)this)->outTracks.end();++it)
        if((*it)->destination == &dest)
            return  (*it)->track;
    return NULL;
};

void Engine::Junction::addOutTrack(Track* track){
//    assert(&track != NULL);
    if(outTracks.size()>0)
        ifCrossing=true;
    outTracks.push_back(new Route(track, NULL));
        assert(track->getEnd() != NULL);
    assert(track->getStart() == this);
};

void Engine::Junction::addInTrack(Track* track){
        assert(track!=NULL);
        inTracks.push_back(track);
};

const Engine::Junction* Engine::Junction::getNextCrossing() const {
    if(ifCrossing == true)
        return this;
    assert(outTracks.size() <= 1);
    if(outTracks.size()==1)
	{
		if(outTracks[0]->track->getEnd()->outTracks.size() == 1 &&
			outTracks[0]->track->getEnd()->outTracks[0]->track->getEnd() == this) return NULL;
        return outTracks[0]->track->getEnd()->getNextCrossing();
	}
    return NULL;
}

//const Engine::Junction* Engine::Junction::getNextCrossing() const;
void Engine::Junction::setCrossing()
{
    ifCrossing = true;
}

bool Engine::Junction::getCrossing(){
   return ifCrossing;
};

vector<Engine::Route*>::iterator Engine::Junction::outTracksBegin()
{
    return outTracks.begin();
}
vector<Engine::Route*>::iterator Engine::Junction::outTracksEnd()
{
    return outTracks.end();
}

const Engine::Track* Engine::Junction::getTrackTo(const Engine::Junction* dest)
{
    for(vector<Route*>::iterator it = outTracks.begin(); it != outTracks.end(); ++it)
    {
        if((*it)->destination == dest) return (*it)->track;
    }
    return NULL;
}

unsigned Engine::Junction::getDistanceTo(const Engine::Junction* dest) throw(NoJunction)
{
    for(vector<Route*>::iterator it = outTracks.begin(); it != outTracks.end(); ++it)
    {
        if((*it)->destination == dest)
                {
                        int distance = 0;
                        const Track* curTrack = (*it)->track;
                        //const Track* nextTrack = curTrack->getEnd()->getNextTrack(*dest); unused
                        while(curTrack->getEnd() != dest)
                        {
                                distance += curTrack->getLength();
                                curTrack = curTrack->getEnd()->getNextTrack(*dest);
                        }
                        distance += curTrack->getLength();
                        return distance;
                }
    }
    throw NoJunction();
}

void Engine::Junction::init(Engine* caller) throw(Engine::reserved, Engine::NoRoute) {
    //assert(ifCrossing=false);
    if(outTracks.size() == 0 || ifCrossing == false) return;
    for(vector< Route* >::iterator it = outTracks.begin(); //iterating through outTracks
        it!= outTracks.end();++it)
    {
        assert((*it)->track->getStart() != NULL);
        assert((*it)->track->getEnd() != NULL);
        assert((*it)->track->getStart() != (*it)->track->getEnd());
        assert(this == (*it)->track->getStart());

        (*it)->destination = (*it)->track->getEnd()->getNextCrossing();
        //assert(this != (*it)->destination);
    }


    if(outTracks.size() + inTracks.size() <= 2) return;
    Segment* crossSegment = NULL;
    for(vector<Route*>::iterator it = outTracks.begin(); it!=outTracks.end(); ++it)
    {
        if(crossSegment == NULL)
        {
            crossSegment = &((*it)->track->getSegment());
            continue;
        }
        Track* tr = (*it)->track;
        Segment* trackSegment = &(tr->getSegment());
        if(trackSegment != crossSegment)
        {
            caller->joinSegments(crossSegment, trackSegment);
        }
        /*for(vector<Route*>::iterator iit = tr->getEnd()->outTracks.begin();
            iit != tr->getEnd()->outTracks.end(); ++iit )
        {
            if(&((*iit)->track->getSegment()) != crossSegment )
                caller->joinSegments(crossSegment, &((*iit)->track->getSegment()));
        }
        for(vector<Track*>::iterator iit = tr->getEnd()->inTracks.begin();
            iit != tr->getEnd()->inTracks.end(); ++iit )
        {
            if(&((*iit)->getSegment()) != crossSegment )
                caller->joinSegments(crossSegment, &((*iit)->getSegment()));
        }*/
    }
    for(vector<Track*>::iterator it = inTracks.begin(); it!=inTracks.end(); ++it)
    {
        assert(crossSegment != NULL);
        Segment* trackSegment = &((*it)->getSegment());
        if(trackSegment != crossSegment)
        {
            caller->joinSegments(crossSegment, trackSegment);
        }
        //assert((*it)->track->getStart() != NULL);
        /*for(vector<Route*>::iterator iit = (*it)->getStart()->outTracks.begin();
            iit != (*it)->getStart()->outTracks.end(); ++iit )
        {
            if(&((*iit)->track->getSegment()) != crossSegment )
                caller->joinSegments(crossSegment, &((*iit)->track->getSegment()));
        }
        for(vector<Track*>::iterator iit = (*it)->getStart()->inTracks.begin();
            iit != (*it)->getStart()->inTracks.end(); ++iit )
        {
            if(&((*iit)->getSegment()) != crossSegment )
                caller->joinSegments(crossSegment, &((*iit)->getSegment()));
        }*/
    }
};

unsigned Engine::Junction::getNumber() const {
   return number;
}

void Engine::Junction::setNumber(int num) {
        number = num;
}

//####################
//a może tak?
//####################
void Engine::Junction::addSemafors(vector<Engine::Semafor*>* semafors, Engine* engine){
        //Ading inSemafors
        bool add;
        //if(!ifCrossing) return;
        /*for(vector<Route*>::iterator out = outTracks.begin(); out < outTracks.end();
                        ++out){
                add = true;
                for(vector<Track*>::iterator in = inTracks.begin(); in < inTracks.end();
                                ++in){
                        if(&((*in)->getSegment()) == &((*out)->track->getSegment())){
                                add=false;
                                break;
                        }
                }
//if(outTracks.size()+inTracks.size() <= 2) add = false;
                if(add){
                        Track* track = (*out)->track;
                        unsigned pos = SEMPOS;
                        while(pos > track->getLength()){
                                pos -= track->getLength();
                                if(track->getEnd()->ifCrossing){
                                        add = false;
                                        break;
                                }
                                if(track->getEnd()->outTracks.size()<1){
                                        add = false;
                                        break;
                                }
                                assert(track->getEnd()->outTracks.size()==1);
                                track = track->getEnd()->outTracks[0]->track;
                        }
                        if(add){
                                InSemafor* semafor = new InSemafor(track,pos);
                                track->addInSemafor(semafor);
                                semafors->push_back(semafor);
                        }

                }
        }*/
        for(vector<Track*>::iterator in = inTracks.begin(); in < inTracks.end();
                        ++in){
                add = false;
                for(vector<Route*>::iterator out = outTracks.begin(); out < outTracks.end();
                                ++out){
                        if(&((*in)->getSegment()) != &((*out)->track->getSegment())){
                                add=true;
                                if(engine->getNoOfTilesAt((*out)->track->getEnd()->getPos()) > 1) break;
                                if((*out)->track->getEnd()->getNoOfOutTracks() == 1)
                                {
                                        Track* nextTrack = (*((*out)->track->getEnd()->outTracksBegin()))->track;
                                        if(&(nextTrack->getSegment()) != &((*out)->track->getSegment())) add=false;
                                }
                                else if((*out)->track->getEnd()->getNoOfOutTracks() == 0) add=false;
                                break;
                        }
                }
                if(add){
                        Track* track = (*in);
                        unsigned pos = SEMPOS;
                        while(pos > track->getLength()){
                                pos -= track->getLength();
                                if(track->getStart()->inTracks.size()!=1){
                                        add = false;
                                        break;
                                }
                                track = track->getStart()->inTracks[0];
                        }
                        if(add){
                                OutSemafor* semafor = new OutSemafor(track,track->getLength()-pos);
                                track->addOutSemafor(semafor);
                                semafors->push_back(semafor);
                        }

                }
        }
}

/*
void Engine::Junction::addSemafors(vector<Engine::Semafor*>* semafors){
        //Ading inSemafors
        bool add;
        //if(!ifCrossing) return;
        for(vector<Route*>::iterator out = outTracks.begin(); out < outTracks.end();
                        ++out){
                add = true;
                                if((*out)->track->getEnd()->getNoOfOutTracks() == 1)
                                {
                                        Track* nextTrack = (*((*out)->track->getEnd()->outTracksBegin()))->track;
                                        if(&(nextTrack->getSegment()) != &((*out)->track->getSegment())) add=false;
                                }
                                else if((*out)->track->getEnd()->getNoOfOutTracks() == 0) add=false;

                for(vector<Track*>::iterator in = inTracks.begin(); in < inTracks.end();
                                ++in){
                        if(&((*in)->getSegment()) == &((*out)->track->getSegment())){
                                add=false;
                                break;
                        }
                }
//if(outTracks.size()+inTracks.size() <= 2) add = false;
                if(add){
                        Track* track = (*out)->track;
                        unsigned pos = SEMPOS;
                        while(pos > track->getLength()){
                                pos -= track->getLength();
                                if(track->getEnd()->ifCrossing){
                                        add = false;
                                        break;
                                }
                                if(track->getEnd()->outTracks.size()<1){
                                        add = false;
                                        break;
                                }
                                assert(track->getEnd()->outTracks.size()==1);
                                track = track->getEnd()->outTracks[0]->track;
                        }
                        if(add){
                                InSemafor* semafor = new InSemafor(track,pos);
                                track->addInSemafor(semafor);
                                semafors->push_back(semafor);
                        }

                }
        }
        
        for(vector<Track*>::iterator in = inTracks.begin(); in < inTracks.end();
                        ++in){
                add = false;
                for(vector<Route*>::iterator out = outTracks.begin(); out < outTracks.end();
                                ++out){
                        if(&((*in)->getSegment()) != &((*out)->track->getSegment())){
                                add=true;
                                break;
                        }
                }
//if(outTracks.size()+inTracks.size() <= 2) add = false;
                if(add){
                        Track* track = (*in);
                        unsigned pos = SEMPOS;
                        while(pos > track->getLength()){
                                pos -= track->getLength();
                                if(track->getStart()->inTracks.size()!=1){
                                        add = false;
                                        break;
                                }
                                track = track->getStart()->inTracks[0];
                        }
                        if(add){
                                OutSemafor* semafor = new OutSemafor(track,track->getLength()-pos);
                                track->addOutSemafor(semafor);
                                semafors->push_back(semafor);
                        }

                }
        }
        //
}*/
