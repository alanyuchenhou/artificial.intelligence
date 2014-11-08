// Agent.h

// HW2 Solution by Larry Holder

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "WorldState.h"
#include <list>

class SearchState
{
public:
	SearchState (Location& loc, Orientation orient, Action act, SearchState* par);
	bool operator== (const SearchState& searchState);
	Location location;
	Orientation orientation;
	Action action; // action used to get from parent state to this state
	SearchState* parent;
};

class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	Action Process (Percept& percept);
	void GameOver (int score);

	// HW2 (assumes 4x4 world)
	void UpdateState (Percept& percept);
	void ExecuteAction (Action action);
	bool CanReachSafeUnvisitedLocation();
	bool PlanSafeRoute (Location& goalLocation);
	void AddRouteToPlan (SearchState* state);
	SearchState* ChildState (SearchState* state, Action action);
	void ClearStates (list<SearchState*> stateList);

	list<Action> plan;
	WorldState worldState;
	bool safeLocations[5][5]; // only use [1][1] to [4][4]
	bool visitedLocations[5][5];
};

#endif // AGENT_H
