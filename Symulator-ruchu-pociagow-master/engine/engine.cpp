/* vim: set tabstop=4 shiftwidth=4 expandtab : */
#include"engine.hpp"
using namespace std;

Engine::~Engine(){
    for(vector<Train*>::iterator it = trains.begin(); it != trains.end(); ++it)
        delete (*it);
    for(list<Segment*>::iterator it = segments.begin(); it != segments.end(); ++it)
        delete (*it);
    for(vector<Semafor*>::iterator it = semafors.begin(); it != semafors.end(); ++it)
        delete (*it);
    for(vector<Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
        delete (*it);
    for(vector<TileDesc*>::iterator it = tiles.begin(); it != tiles.end(); ++it)
        delete (*it);
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
        delete (*it);
}

Engine::Engine() : iterations(0) { }

bool Engine::TrainStop::operator!=(const Engine::TrainStop& stop){
    return junction != stop.junction;
};

void Engine::initJunctions()
{
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
        if((*it)->getNoOfOutTracks() == 0) (*it)->setCrossing();

    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
    {
        (*it)->init(this);
        if((*it)->getCrossing())
            crossings.push_back(*it);
    }

    for(vector<Train*>::iterator it = trains.begin(); it != trains.end(); ++it)
        (*it)->findRoute();
}

void Engine::initTrains()
{
    for(vector<Train*>::iterator it = trains.begin(); it != trains.end(); ++it){
        (*it)->init();
    }
}

void Engine::initSemafors(){
    for(vector<Junction*>::iterator it = junctions.begin();
            it != junctions.end(); ++it){
        (*it)->addSemafors(&semafors, this);
    }
}

Engine::Route::Route(Engine::Track* t, Engine::Junction* j)
        : track(t), destination(j) { };

void Engine::addJunction(unsigned xPos, unsigned yPos)
{
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
    {
        if((*it)->getPos() == Pos(xPos, yPos)) return;
    }
    junctions.push_back(new Junction(xPos, yPos));
    //stations.push_back(new Station(junctions.back(), xPos, yPos)); //this is to be REMOVED in the working tile version
}

void Engine::addJunction(Engine::Pos pos)
{
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
    {
        if((*it)->getPos() == pos) return;
    }
    junctions.push_back(new Junction(pos.x, pos.y));
}

Engine::Pos::Pos(unsigned valX, unsigned valY) : x(valX), y(valY) { };

bool Engine::Pos::operator==(const Engine::Pos& pos) const {
    return pos.x == x && pos.y == y;
}

Engine::Stop::Stop(Pos sPos, int sTime) : Pos(sPos), time(sTime) {
    if(time == 0) time = 1;
};

Engine::TrainStop::TrainStop(Engine::Junction* j,int time) : junction(j), defTime(time), curTime(time) {};

void Engine::TrainStop::reset(){
    curTime=defTime;
}

void Engine::TrainStop::wait(){
    if(curTime>0){
        --curTime;
    }
}

bool Engine::TrainStop::ifWait(){
    if(curTime==0){
        reset();
        return false;
    }
    return true;
}
Engine::Segment* Engine::addGetSegment(){
    Segment* segment = new Segment();
    segments.push_back(segment);
    return segment;
}

void Engine::addSegment(){
    segments.push_back(new Segment());
};

void Engine::update(unsigned times){
    for(unsigned i = 0;i<times;++i){
        semaforStates.clear();
        for( vector<Semafor*>::iterator sem = semafors.begin(); sem!=semafors.end();++sem)
            (*sem)->pre();
       for( vector<Train*>::iterator it = trains.begin();it != trains.end();++it)
           (*it)->update();
        for( vector<Semafor*>::iterator sem = semafors.begin(); sem!=semafors.end();++sem){
            (*sem)->post();
            semaforStates.push_back(*sem);
        }
    }
    //checking for deadlocks
    for(unsigned i=0; i<trains.size(); i++)
    {
        if(trains[i]->getDeadlock() > 5) continue;
        Train* waitingFor = trains[i]->getWaitingFor();
        for(unsigned count = 0; count<trains.size(); count++)
        {
            if(waitingFor == NULL) break;
            if(waitingFor == trains[i])
            {
                if(trains[i]->getDeadlock() >= 5)
                {
                    /*cout<<"Deadlock! Train at ("<<trains[i]->positions.front().x<<","<<trains[i]->positions.front().y<<") is waiting for a train at ("
                        <<trains[i]->getWaitingFor()->positions.front().x<<","<<trains[i]->getWaitingFor()->positions.front().y<<")"<<endl;*/
                    deadlocks.push_back(trains[i]->positions.front());
                }
                trains[i]->incDeadlock();
                break;
            }
            waitingFor = waitingFor->getWaitingFor();
        }
    }
    iterations += times;
};
void Engine::init(){
    currentTile = tiles.begin();
    initJunctions();
    initTrains();
    for( vector<Train*>::iterator it = trains.begin();it != trains.end();++it)
        trainStates.push_back((*it));
    initSemafors();
}

void Engine::addTrack(unsigned xStart, unsigned yStart, unsigned xEnd,
                      unsigned yEnd, unsigned length, TileDesc* tile) throw(NoJunction)
{
    assert(segments.size() > 0);
    Junction *start, *end;
    start = end = NULL;
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
    {
        Pos curPos = (*it)->getPos();
        if(curPos.x == xStart && curPos.y == yStart && start == NULL)
        {
            start = *it;
            if(end != NULL) break;
        }
        if(curPos.x == xEnd && curPos.y == yEnd && end == NULL)
        {
            end = *it;
            if(start != NULL) break;
        }
    }
    if(start == NULL || end == NULL) { cout<<"No Junction at ("<<xStart<<","<<xEnd<<")"<<endl; throw NoJunction(); }
    assert(end != NULL);
    assert(segments.size()>0);
    Track* track = new Track(start, end, length,segments.back(), tile);
    start->addOutTrack(track);
    end->addInTrack(track);
    assert(start != end);
}

void Engine::addTrack(Engine::Pos startPos, Engine::Pos endPos, unsigned length, TileDesc* tile) throw(NoJunction)
{
    assert(segments.size() > 0);
    Junction *start, *end;
    start = end = NULL;
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
    {
        Pos curPos = (*it)->getPos();
        if(curPos == startPos && start == NULL)
        {
            start = *it;
            if(end != NULL) break;
        }
        if(curPos == endPos && end == NULL)
        {
            end = *it;
            if(start != NULL) break;
        }
    }
    if(start == NULL || end == NULL) throw NoJunction();
    assert(end != NULL);
    assert(segments.size()>0);
    Track* track = new Track(start, end, length, segments.back(), tile);
    start->addOutTrack(track);
    end->addInTrack(track);
    assert(start != end);
}

Engine::TileDesc* Engine::getTileAt(Engine::Pos pos)
{
    pos.x -= pos.x%50;
    pos.y -= pos.y%50;
    for(vector<TileDesc*>::iterator it = tiles.begin(); it != tiles.end(); ++it)
    {
        if((*it)->pos == pos) return (*it);
    }
    return NULL;
}

Engine::Junction* Engine::getJunction(Engine::Pos pos)
{
        for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
        {
                if((*it)->getPos() == pos) return (*it);
        }
        return NULL;
}

void Engine::addTrain(unsigned length,unsigned maxSpeed, vector<Engine::Stop>& targets, bool loop, string name)
    throw(reserved,NoStation)
{
    if(targets.size() < 2) throw NoStation();
    list<TrainStop>* targetJunctions = new list<TrainStop>();
    //get the absolute coordinates of the stations
    for(vector<Stop>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        bool stationExists = false;
        for(vector<Station*>::iterator st = stations.begin(); st != stations.end(); ++st)
        {
            if((*st)->tileX == (*it).x && (*st)->tileY == (*it).y) //got it
            {
                Pos newPos = (*st)->stopJunction->getPos();
                (*it).x = newPos.x;
                (*it).y = newPos.y;
                stationExists = true;
                break;
            }
        }
        if(!stationExists) throw NoStation();
    }
    //need to find the Junction objects given Pos objects
    for(vector<Stop>::iterator it = targets.begin(); it!=targets.end(); ++it)
    {
        Junction* junc = NULL;
        for(vector<Junction*>::iterator iit = junctions.begin(); iit != junctions.end(); ++iit)
        {
            if((*it) == (*iit)->getPos()) junc = *iit;
        }
        if(junc == NULL) throw NoStation();

        junc->setCrossing();
        targetJunctions->push_back(TrainStop(junc,(*it).time));
    }
    trains.push_back(new Train(this, length, maxSpeed, targetJunctions, loop, name, targets));
}

Engine::Segment* Engine::getSegmentFor(Engine::Junction* junc1, Engine::Junction* junc2, Engine::Segment* prefered)
{
    Segment* segments[2];
    segments[0] = NULL;
    segments[1] = NULL;
    if(junc1 != NULL)
    {
        //find the first segment
        for(vector<Route*>::iterator it = junc1->outTracks.begin(); it != junc1->outTracks.end(); ++it)
        {
            bool segmentFound = false;
            for(vector<Route*>::iterator iit = (*it)->track->getEnd()->outTracks.begin(); iit != (*it)->track->getEnd()->outTracks.end(); ++iit)
            {
                if((*iit)->track->getEnd() == junc1)
                {
                    segments[0] = &((*iit)->track->getSegment());
                    segmentFound = true;
                    break;
                }
            }
            if(segmentFound) break;
        }
    }
    if(junc2 != NULL)
    {
        //find the second segment
        for(vector<Route*>::iterator it = junc2->outTracks.begin(); it != junc2->outTracks.end(); ++it)
        {
            bool segmentFound = false;
            for(vector<Route*>::iterator iit = (*it)->track->getEnd()->outTracks.begin(); iit != (*it)->track->getEnd()->outTracks.end(); ++iit)
            {
                if((*iit)->track->getEnd() == junc2)
                {
                    segments[1] = &((*iit)->track->getSegment());
                    segmentFound = true;
                    break;
                }
            }
            if(segmentFound) break;
        }
    }
    if(segments[0] == NULL && segments[1] == NULL)
    {
        return addGetSegment();
    }
    if(segments[0] == NULL) return segments[1];
    if(segments[1] == NULL) return segments[0];
    if(segments[0] != segments[1])
    {
        if(prefered == segments[1])
        {
            joinSegments(segments[1], segments[0]);
            return segments[1];
        }
        joinSegments(segments[0], segments[1]);
        return segments[0];
    }
    if(segments[0] == segments[1]) return segments[0];
    return NULL;
}

void Engine::joinSegments(Engine::Segment* s1, Engine::Segment* s2)
{
    if(s1 == s2) return;
    //s2 will be deleted
    for(vector<Track*>::iterator it = s2->tracksBegin(); it != s2->tracksEnd(); ++it)
    {
        (*it)->setSegment(s1);
        s1->addTrack(*it);
    }
    for(vector<TileDesc*>::iterator it = tiles.begin(); it != tiles.end(); ++it)
    { 
        if((*it)->segment == s2) (*it)->segment = s1;
    }
    for(list<Segment*>::iterator it = segments.begin(); it != segments.end(); ++it)
    {
        if((*it) == s2)
        {
            segments.erase(it);
            break;
        }
    }
    delete s2;
}

unsigned Engine::getDistance(unsigned startX, unsigned startY, unsigned endX, unsigned endY) throw(NoStation, NoRoute)
{
    Junction* start = NULL;
    Junction* end = NULL;
    for(vector<Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
    {
        if((*it)->tileX == startX && (*it)->tileY == startY)
        {
            start = (*it)->stopJunction;
            if(end != NULL) break;
        }
        if((*it)->tileX == endX && (*it)->tileY == endY)
        {
            end = (*it)->stopJunction;
            if(start != NULL) break;
        }
    }
    if(start == NULL || end == NULL) throw NoStation();
    unsigned result = 2147483647;
    delete findShortest(start, end, &result);
    if(result == 2147483647) throw NoRoute();
    return result;
}

list<Engine::TrainStop>* Engine::findShortest(Engine::TrainStop pointA, Engine::TrainStop pointB, unsigned* distance)
{
    //if(pointA == NULL || pointB == NULL) return NULL;
        int* distances = new int[crossings.size()];                        //for storing distances from pointA to each junction
        Junction** previous = new Junction*[crossings.size()];        //the previous junction on the way to pointB of each junction
        int* visited = new int[crossings.size()];                //specifies if the junction determined by array index was already visited
        for(unsigned i=0; i<crossings.size(); i++) visited[i] = 0; //reset the visited array

        list<Junction*>* unvisited = new list<Junction*>();                //list of all unvisited crossings
        //setting the starting values for distances

    for(unsigned i=0; i<crossings.size(); i++)
    {
        previous[i] = NULL;
    }

        for(unsigned i=0; i<crossings.size(); i++)
        {
                if(crossings.at(i) == pointA.junction) distances[i] = 0;        //0 for pointA
                else distances[i] = 2147483647;                                                //Infinity (let's say it is) for all the others
                unvisited->push_back(crossings.at(i));                                //adding each to the unvisited list
                crossings[i]->setNumber(i);
        }
        Junction* current = pointA.junction;                //currently processed junction
        while(!unvisited->empty())
        {
                vector<const Junction*> neighbors;        //neighbors of the current junction
        //fill the neighbors vector
        for(vector<Route*>::iterator it = current->outTracksBegin(); it != current->outTracksEnd(); ++it)
        {
            if((*it)->destination == NULL) continue;
            if((*it)->destination == current) continue;
            neighbors.push_back((*it)->destination);
        }
                for(unsigned i=0; i<neighbors.size(); i++)
                {
                        if(visited[neighbors.at(i)->getNumber()] != 0) continue;        //if neighbor already visited, skip it
                        int newDistance = current->getDistanceTo(neighbors.at(i)) + distances[current->getNumber()]; //calculate the newDistance
            //assert(neighbors[i] != current);
                        if(newDistance < distances[neighbors.at(i)->getNumber()])        //if the newDistance is less than previously
                        {
                                distances[neighbors.at(i)->getNumber()] = newDistance; //update the point's distance
                                previous[neighbors.at(i)->getNumber()] = current;                //set the current junction as the previous for the neighbor
                assert(current != NULL);
                        }
                        if(neighbors.at(i) == pointB.junction)        //if we've found a way to pointB
                        {
                                list<TrainStop>* result = new list<TrainStop>();        //create a result list
                                //filling the result list. We start from the last junction, so it's necessary to put all the crossings at the front
                                result->push_front(pointB);        //push the last junction
                                result->push_front(TrainStop(current,0));//push the second last junction
                                while(current != pointA.junction)        //move back, adding every junction to the list until you get to pointA
                                {
                    if(current == NULL)
                    {
                                    delete[] distances;
                                    delete[] previous;
                                    delete[] visited;
                        delete result;
                                    delete unvisited;
                        return NULL;
                    }
                                        current = previous[current->getNumber()];
                    if(current == pointA.junction)
                        result->push_front(pointA);
                    else
                                            result->push_front(current);
                                }
                                delete[] distances;
                                delete[] previous;
                                delete[] visited;
                                delete unvisited;
                (*distance) = newDistance;
                                return result;
                        }
                }
                unvisited->remove(current);
                visited[current->getNumber()] = 1; //set current as visited
                if(unvisited->empty())
                {
                        delete[] distances;
                        delete[] previous;
                        delete[] visited;
                        delete unvisited;
                        return NULL;        //this may happen if there's no connection between pointA and pointB
                }
                current = unvisited->front();        //for a while, set the first junction in the list as current
                //but here, the junction with smallest distance will be set as current
                for(list<Junction*>::iterator it=unvisited->begin(); it!=unvisited->end(); it++)
                {
                        if(distances[(*it)->getNumber()] < distances[current->getNumber()]) current = *it;
                }
        }
    return NULL;
}

Engine::Station::Station(Junction* junc, unsigned x, unsigned y) : stopJunction(junc), tileX(x), tileY(y)
{
    junc->setCrossing();
}

void Engine::reset()
{
    deadlocks.clear();
    for(vector<Train*>::iterator it = trains.begin(); it != trains.end(); ++it)
    {
        (*it)->reset();
    }
    for(list<Segment*>::iterator it = segments.begin(); it != segments.end(); ++it)
    {
        (*it)->reset();
    }
    for(vector<Train*>::iterator it = trains.begin(); it != trains.end(); ++it)
    {
        (*it)->init();
    }
    iterations = 0;
}

void Engine::clear()
{
    deadlocks.clear();
    trainStates.clear();
    semaforStates.clear();
    for(vector<Junction*>::iterator it = junctions.begin(); it != junctions.end(); ++it)
    {
        delete (*it);
    }
    junctions.clear();
    crossings.clear();
    for(vector<Train*>::iterator it = trains.begin(); it != trains.end(); ++it)
    {
        delete (*it);
    }
    trains.clear();
    for(list<Segment*>::iterator it = segments.begin(); it != segments.end(); ++it)
    {
        delete (*it);
    }
    segments.clear();
    for(vector<Semafor*>::iterator it = semafors.begin(); it != semafors.end(); ++it)
    {
        delete (*it);
    }
    semafors.clear();
    for(vector<TileDesc*>::iterator it = tiles.begin(); it != tiles.end(); ++it)
    {
        delete (*it);
    }
    tiles.clear();
    for(vector<Station*>::iterator it = stations.begin(); it != stations.end(); ++it)
    {
        delete (*it);
    }
    stations.clear();
    iterations = 0;
}

Engine::TileDesc* Engine::getNextTurn(Engine::Pos p)
{
    p.x -= p.x%50;
    p.y -= p.y%50;
    for(; currentTile != tiles.end(); ++currentTile)
    {
        if((*currentTile)->pos == p &&
            (( (*currentTile)->tileNo >= 5 && (*currentTile)->tileNo <= 12) ||
             ( (*currentTile)->tileNo >= 21 && (*currentTile)->tileNo <= 24)) )
        {
            vector<TileDesc*>::iterator temp = currentTile;
            ++currentTile;
            return (*temp);
        }
    }
    return NULL;
}

void Engine::resetTileIterator()
{
    currentTile = tiles.begin();
}

unsigned long Engine::getIterations()
{
    return iterations;
}


int Engine::getNoOfTilesAt(Engine::Pos pos)
{
    int result = 0;
    pos.x = pos.x - pos.x%50;
    pos.y = pos.y - pos.y%50;
    for(vector<TileDesc*>::iterator it = tiles.begin(); it != tiles.end(); ++it)
    {
        if((*it)->pos == pos) result++;
    }
    return result;
}

Engine::Semafor::~Semafor() { }
Engine::InSemafor::~InSemafor() { }
Engine::OutSemafor::~OutSemafor() { }
bool Engine::TrainState::getLoop()
{
    return loop;
}

Engine::TileDesc::TileDesc(unsigned posX, unsigned posY, unsigned number,
   Engine::Segment* s, unsigned short edg) : tileNo(number), segment(s), edges(edg)
{
    pos.x = posX;
    pos.y = posY;
}
