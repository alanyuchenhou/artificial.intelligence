// Agent.h
//
// Homework 1 Solution

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"

class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	Action Process (Percept& percept);
	void GameOver (int score);

	void UpdateState(Action action);
	void ExecuteForward();
	void ExecuteTurnLeft();
	void ExecuteTurnRight();
	Location location;
	Orientation orientation;
	bool hasGold;
	bool safe;
};

#endif // AGENT_H
