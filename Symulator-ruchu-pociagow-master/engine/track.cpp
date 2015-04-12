/* vim: set tabstop=4 shiftwidth=4 expandtab : */
#include"engine.hpp"
#include<cmath>
using namespace std;

Engine::Track::Track(Engine::Junction* sStart, Engine::Junction* sEnd,
        unsigned l,Engine::Segment* seg, TileDesc* t)
        : start(sStart), end(sEnd), length(l), segment(seg),
        inSemafor(NULL), outSemafor(NULL), tile(t) {
    seg->addTrack(this);
    assert(end != NULL);
    assert(segment!=NULL);
};

Engine::Junction* Engine::Track::getStart() const {
    return start;
};

Engine::Junction* Engine::Track::getEnd() const {
    return end;
};

unsigned Engine::Track::getLength() const {
    return length;
};

Engine::Pos Engine::Track::getPartPos(unsigned position) const {
    assert(position<=length);
    unsigned x = ((int)end->getPos().x - (int)start->getPos().x)*
        (int)position/(int)length + (int)start->getPos().x;
    unsigned y = ((int)end->getPos().y - (int)start->getPos().y)*
        (int)position/(int)length + (int)start->getPos().y;
    return Pos(x,y);
};
Engine::Segment& Engine::Track::getSegment() const {
    return *segment;
};

Engine::Semafor* Engine::Track::getInSemafor() const {
    return inSemafor;
}

Engine::Semafor* Engine::Track::getOutSemafor() const {
    return outSemafor;
}

void Engine::Track::addInSemafor(Engine::InSemafor* s){
    assert(s!=NULL);
    inSemafor = s;
};
void Engine::Track::addOutSemafor(Engine::OutSemafor* s){
    assert(s!=NULL);
    outSemafor = s;
};
float Engine::Track::getRadian() const {
    Pos s=start->getPos();
    Pos e=end->getPos();
    return atan2((int)(e.x)-(int)(s.x),(int)(e.y)-(int)(s.y));
};
int Engine::Track::getAngle() const {
    Pos s=start->getPos();
    Pos e=end->getPos();
    return -(int)(atan2((int)(e.x)-(int)(s.x),(int)(e.y)-(int)(s.y))*180/M_PI);
};
void Engine::Track::setSegment(Engine::Segment* seg) {
    segment = seg;
}
Engine::TileDesc* Engine::Track::getTile() const
{
    return tile;
}
