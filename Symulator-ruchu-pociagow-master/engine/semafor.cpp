/* vim: set tabstop=4 shiftwidth=4 expandtab : */
#include"engine.hpp"
#include<cmath>
using namespace std;
#define SEMWIDTH 15

Engine::Semafor::Semafor(Track* t, unsigned p) : SemaforState(Pos(0,0)) {
    assert(t != NULL);
    segment = &(t->getSegment());
    Pos n = t->getPartPos(p);
    position.x = (unsigned)((float)n.x + (sin(t->getRadian()+(M_PI/2))*SEMWIDTH));
    position.y = (unsigned)((float)n.y + (cos(t->getRadian()+(M_PI/2))*SEMWIDTH));
    angle = t->getAngle();
};

void Engine::Semafor::set(){
    green = true; 
};

void Engine::Semafor::reset(){
    green = false;
};

Engine::InSemafor::InSemafor(Track* t, unsigned p) : Semafor(t,p){};

void Engine::InSemafor::pre(){
    green = false;
};

void Engine::InSemafor::post(){
    if(!segment->ifReserved())
        green = true;
};

Engine::OutSemafor::OutSemafor(Track* t,unsigned p) : Semafor(t,p){};

void Engine::OutSemafor::pre(){
    green = true;
};

void Engine::OutSemafor::post(){
};

Engine::SemaforState::SemaforState(Engine::Pos p) : position(p) {};

bool Engine::SemaforState::ifGreen(){
    return green;
};

int Engine::SemaforState::getAngle(){
    return angle;
};

Engine::Pos & Engine::SemaforState::getPos(){
    return position;
};
