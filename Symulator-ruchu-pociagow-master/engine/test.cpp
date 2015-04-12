#include<iostream>
#include"engine.hpp"

Engine engine;

int main(){
	engine.addJunction(30,30);
	engine.addJunction(30,40);
	engine.addJunction(300,200);
	engine.addJunction(70,70);

	engine.addSegment();
	engine.addTrack(30 ,30 ,30 ,40 ,50);
	engine.addTrack(30 ,40 ,300,200,50);
	engine.addTrack(300,200,70 ,70 ,50);
	engine.addTrack(70 ,70 ,30 ,30 ,50);

	vector<Engine::Pos> targets;
	targets.push_back(Engine::Pos(300,200));
	targets.push_back(Engine::Pos(30 ,30 ));
	engine.addTrain(20,10,targets);

	engine.init();
	for(int i = 0; i<30; ++i){
		engine.update(1);
		cout << "State [" << i << "] = ";
		for(vector< Engine::Pos>::iterator it = engine.trainStates[0]->positions.begin(); it < engine.trainStates[0]->positions.end();++it)
			cout << "[" << (*it).x << ";" << (*it).y << "]";
		cout << endl;
	}

	

	return 0;
};
