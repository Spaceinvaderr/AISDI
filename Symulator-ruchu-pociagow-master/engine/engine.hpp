/* vim: set tabstop=4 shiftwidth=4 expandtab : */
#ifndef ENGINE_H
#define ENGINE_H
#define SEMPOS 0

#include<vector>
#include<list>
#include<assert.h>
#include <cstddef>
#include <iostream>
#include <utility>
using namespace std;

class Engine{
    public:

        //Exceptions:

        // Wrong position. There is no such Juction
        class NoJunction {};

        // Invalid station(s)
        class NoStation {};

        // This segment is already reserved by another train
        class reserved {};

        // impossible to find route for a train
        class NoRoute {};

        class InvalidMap {};

        struct Pos{
            unsigned x;
            unsigned y;

                        Pos(unsigned valX=0, unsigned valY=0);
            bool operator==(const Pos& pos) const;
        };

        struct Stop : Pos{
            Stop(Pos sPos, int sTime);
            int time;
        };

        class TrainState {
            public:
                vector< Pos > positions;
                vector< pair<Pos, int> > cars;
                vector< Stop > stations;
                unsigned routeLength;
                unsigned routeLengthForSpeedCalculation;
                unsigned speed;
                string name;
                unsigned long time;
                float avgSpeed;
                unsigned long idleTime;
                bool getLoop();
                float opacity;
            protected:
                bool loop;
        };

        void addTile(unsigned xPos, unsigned yPos, unsigned tileNo);

        class SemaforState {
            public:
                bool ifGreen();
                int getAngle();
                Pos & getPos();
            protected:
                bool green;
                Pos position;
                int angle;
                SemaforState(Pos p);
        };

        void addTrain(unsigned length,unsigned maxSpeed, vector<Stop>& targets, bool loop=false, string name="")
                      throw(reserved,NoStation);

        void addSegment();
        void addJunction(unsigned xPos, unsigned yPos);
        void addJunction(Pos pos);
        void update(unsigned times=1);
        void init();
        unsigned getDistance(unsigned startX, unsigned startY, unsigned endX, unsigned endY) throw(NoStation, NoRoute);
        void reset();
        void clear();
        void resetTileIterator();
        unsigned long getIterations();
        int getNoOfTilesAt(Pos pos);
        Engine();
    private:
        class Junction;
        class Track;
        class Train;
        class Segment;
        class Semafor;
        class InSemafor;
        class OutSemafor;
    public:
        struct TrainStop {
            Junction* junction;
            const int defTime;
            int curTime;
            TrainStop(Junction* j,int time=0);
            bool operator!=(const TrainStop& stop);
            void reset();
            void wait();
            bool ifWait();
        };

        vector<const TrainState*> trainStates;
        vector<SemaforState*> semaforStates;
        vector<Pos> deadlocks;
        list<TrainStop>* findShortest(TrainStop pointA, TrainStop pointB, unsigned* distance);
        ~Engine();
        struct TileDesc {
            Pos pos;
            unsigned tileNo;
            Segment* segment;
            bool occupied;
            unsigned short edges;
            TileDesc(unsigned posX, unsigned posY, unsigned number, Segment* s, unsigned short edg);
        };
        TileDesc* getNextTurn(Pos p);
    private:
        vector<Junction*> junctions;
        vector<Junction*> crossings;
        vector<Train*> trains;
        list<Segment*> segments;
        vector<Semafor*> semafors;
        Segment* addGetSegment();   //adds a new segment and returns it
        vector<TileDesc*>::iterator currentTile;
        void joinSegments(Segment* s1, Segment* s2);
        unsigned long iterations;
        void initJunctions();
        void initTrains();

        Junction* getJunction(Pos pos);
        Segment* getSegmentFor(Junction* junc1, Junction* junc2, Segment* prefered=NULL); //returns the suggested segment for the track between junc1 and junc2
       
        void initSemafors();

        unsigned distanceBetween(TrainStop pointA, TrainStop pointB);

        struct Route{
            Track* track;
            const Junction* destination;
            Route(Track* t, Junction* j);
        };

        struct Station {
            Junction* stopJunction; //junction at which the train stops
            unsigned tileX; //
            unsigned tileY; //coordinates in TILESIZE
            Station(Junction* junc, unsigned x, unsigned y);
        };
        

        vector<TileDesc*> tiles;
        vector<Station*> stations;
        TileDesc* getTileAt(Pos pos);

        void addTrack(unsigned xStart, unsigned yStart, unsigned xEnd,
                      unsigned yEnd, unsigned length, TileDesc* tile) throw(NoJunction);
        void addTrack(Pos start, Pos end, unsigned length, TileDesc* tile) throw(NoJunction);

        class Junction {
            private:

                Pos position;
                vector< Route* > outTracks;
                vector< Track* > inTracks;
                bool ifCrossing;
                unsigned number; //przepraszam, musiałem, upraszcza to wyznaczanie trasy
            public:
                Junction(unsigned x, unsigned y);
                ~Junction();
                Pos getPos() const;
                unsigned getNoOfOutTracks();
                const Track* getNextTrack( const Junction& dest) const;
                void addOutTrack(Track* track);
                void addInTrack(Track* track);
                const Junction* getNextCrossing() const;
                void setCrossing();
                bool getCrossing();
                vector<Route*>::iterator outTracksBegin();
                vector<Route*>::iterator outTracksEnd();
                const Track* getTrackTo(const Junction* dest);
                unsigned getDistanceTo(const Junction* dest) throw(NoJunction);
                void init(Engine* caller) throw(reserved, NoRoute);
                unsigned getNumber() const;
                void setNumber(int num);
                void addSemafors(vector<Semafor*>* semafors, Engine* engine);
                friend Engine::Segment* Engine::getSegmentFor(Engine::Junction* junc1, Engine::Junction* junc2, Engine::Segment* prefered=NULL);
        };

        class Track {
                private:
                        //! Pointer to start connector
                        Junction* start;

                        //! Pointer to stop connector
                        Junction* end;

                        //! Track length
                        unsigned length;

                        Segment* segment;

                        InSemafor* inSemafor;
                        OutSemafor* outSemafor;
                        TileDesc* tile;

                public:
                        Track(Junction* sStart, Junction* sEnd, unsigned l,Segment* seg, TileDesc* t);
                        Junction* getStart() const;
                        Junction* getEnd() const;
                        unsigned getLength() const;
                        Pos getPartPos(unsigned position) const;
                        Segment& getSegment() const;

                        void addInSemafor(InSemafor* s);
                        void addOutSemafor(OutSemafor* s);
                        Semafor* getInSemafor() const;
                        Semafor* getOutSemafor() const;
                        int getAngle() const;
                        float getRadian() const;
                        void setSegment(Segment* seg);
                        TileDesc* getTile() const;
        };

        class Train : public TrainState{
            private:
                Engine* engine; //pointer to the engine for using its methods
                unsigned length;
                unsigned maxSpeed;
                unsigned position;
                list< const Track* > tracks;
                list< TrainStop >* destinations;
                list< TrainStop >* fullDestinations;
                bool move(unsigned);
                Train* waitingFor;
                unsigned deadlock;
                TileDesc* deadlockAtTile;
                void perfectAngle(Pos p, int* val, const Track* t);
                Junction *prevDestination1;
                Junction *prevDestination2;
            public:
                Train(Engine* caller, unsigned l, unsigned mSpeed,
                    list<TrainStop>* targets, bool loop, string tName, vector<Stop>& tStations);
                ~Train();
                void findRoute() throw(NoRoute);
                void update();
                void init() throw(reserved);
                void setWaitingFor(Train* train);
                Train* getWaitingFor();
                void incDeadlock();
                unsigned getDeadlock();
                void reset();
        };
        class Segment {
            private:
                Train* train;
                unsigned reserved;
                vector<Track*> tracks;

            public:
                Segment();
                bool ifReserved() const;
                bool canReserve(Train* t) const;
                bool reserve(Train* t);
                void release(Train* t);
                void addTrack(Track* t);
                vector<Track*>::iterator tracksBegin();
                vector<Track*>::iterator tracksEnd();
                void reset();
        };
        class Semafor : public SemaforState {
            protected:
                Segment* segment;
            public:
                Semafor(Track* t,unsigned p);
                virtual void pre() = 0;
                void set();
                void reset();
                virtual void post() = 0;
                virtual ~Semafor();
        };

        class InSemafor : public Semafor {
            public:
                InSemafor(Track* t,unsigned p);
                void pre();
                void post();
                ~InSemafor();
        };

        class OutSemafor : public Semafor {
            public:
                OutSemafor(Track* t,unsigned p);
                void pre();
                void post();
                ~OutSemafor();
        };
};


#endif //ENGINE_H
