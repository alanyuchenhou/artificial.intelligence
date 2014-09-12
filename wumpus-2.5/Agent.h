// Agent.h

#ifndef AGENT_H
#define AGENT_H
#include "Location.h"
#include "Action.h"
#include "Percept.h"
#include "assert.h"
#include <vector>
using namespace std;
class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	Action Process (Percept& percept);
	void GameOver (int score);
	Location entranceCoordinate;
	Location agentCoordinate;
	Location destinationCoordinate;
	int orientation;
	bool wumpusDown;
	bool armed;
	string goal;
	Location previousCoordinate;
	vector<Location> route;
};

#endif // AGENT_H
