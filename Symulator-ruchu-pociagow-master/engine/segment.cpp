/* vim: set tabstop=4 shiftwidth=4 expandtab : */
#include"engine.hpp"
using namespace std;

Engine::Segment::Segment() : train(NULL), reserved(0) {
    reserved = 0;
    train = NULL;
};

bool Engine::Segment::ifReserved() const {
    return reserved;
};

bool Engine::Segment::canReserve(Engine::Train* t) const {
    if(reserved && train != t)
    {
        t->setWaitingFor(train);
        return false;
    }
    t->setWaitingFor(NULL);
    return true;
};

bool Engine::Segment::reserve(Engine::Train* t){
    if(reserved && train != t) 
    {
        t->setWaitingFor(train);
        return false;
    }
    train = t;
    ++reserved;
    t->setWaitingFor(NULL);
    return true;
};
void Engine::Segment::release(Engine::Train* t){
    assert(train == t);
    assert(reserved > 0);
    --reserved;
};
void Engine::Segment::addTrack(Track* t) {
    tracks.push_back(t);
}
vector<Engine::Track*>::iterator Engine::Segment::tracksBegin() {
    return tracks.begin();
}
vector<Engine::Track*>::iterator Engine::Segment::tracksEnd() {
    return tracks.end();
}
void Engine::Segment::reset()
{
    train = NULL;
    reserved = 0;
}
