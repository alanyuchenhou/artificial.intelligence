// Agent.cc

// HW3 Solution, based on HW2 Solution, by Larry Holder

#include <iostream>
#include <algorithm>
#include "Agent.h"

using namespace std;


// ----- SearchState class -----

SearchState::SearchState (Location& loc, Orientation orient, Action act, SearchState* par)
{
	location = loc;
	orientation = orient;
	action = act;
	parent = par;
}

bool SearchState::operator== (const SearchState& searchState)
{
	if ((location == searchState.location) && (orientation == searchState.orientation))
	{
		return true;
	}
	return false;
}


// ----- Agent class -----

Agent::Agent ()
{
	// HW3:
	// Processing for new world
	// Reset stench locations
	for (int x = 1; x <= 4; x++)
	{
		for (int y = 1; y <= 4; y++)
		{
			stenchLocations[x][y] = false;
		}
	}
	wumpusLocationKnown = false;
}

Agent::~Agent ()
{
	// nothing for now
}

void Agent::Initialize ()
{
	worldState.worldSize = 4; // Given in HW2 problem definition
	worldState.agentLocation = Location(1,1);
	worldState.agentOrientation = RIGHT;
	worldState.agentHasArrow = true;
	worldState.agentHasGold = false;
	worldState.wumpusAlive = true;
	// Initialize safe and visited locations
	for (int x = 1; x <= 4; x++)
	{
		for (int y = 1; y <= 4; y++)
		{
			safeLocations[x][y] = false;
			visitedLocations[x][y] = false;
		}
	}
	safeLocations[1][1] = true;
	visitedLocations[1][1] = true;
	plan.clear();
}

Action Agent::Process (Percept& percept)
{
	Action action;
	Orientation orient;
	int x, y;

	UpdateState (percept);
	orient = worldState.agentOrientation;
	x = worldState.agentLocation.X;
	y = worldState.agentLocation.Y;
	Location home = Location(1,1);

	// Agent should always GRAB the gold and then leave cave
	if (percept.Glitter)
	{
		plan.clear();
		plan.push_back(GRAB);
		PlanSafeRoute(home, orient, false); // route home always exists (HW3 adds orientation)
		plan.push_back(CLIMB);
		cout << "Grabbing gold and heading home\n";
	} else if (plan.empty()) {
		if (CanReachSafeUnvisitedLocation()) {
			// Agent should visit all safe locations reachable via safe locations
			// If CanReachSafeUnvisitedLocation returns true, then plan added as side effect
			// So, nothing else to do
		} else if (CanKillWumpus()) {
			// If CanKillWumpus returns true, then plan added as side effect
			// So, nothing else to do
		} else {
			// Give up (safe route to home should always exist)
			PlanSafeRoute(home, orient, false); // HW3 adds orientation
			plan.push_back(CLIMB);
			cout << "Giving up. Leaving cave.\n";
		}
	}
	action = plan.front();
	plan.pop_front();
	ExecuteAction (action);
	return action;
}

void Agent::GameOver (int score)
{
	// nothing for now
	cout << "Agent::GameOver\n";
}

void Agent::UpdateState (Percept& percept)
{
	int x, y;
	// Update wumpus health
	if (percept.Scream)
	{
		worldState.wumpusAlive = false;
		// HW3: revisit locations surrounding (now dead) wumpus
		x = worldState.wumpusLocation.X;
		y = worldState.wumpusLocation.Y;
		if (x > 1) visitedLocations[x-1][y] = false;
		if (x < 4) visitedLocations[x+1][y] = false;
		if (y > 1) visitedLocations[x][y-1] = false;
		if (y < 4) visitedLocations[x][y+1] = false;
		// But current agent location still visited (in case reset above)
		visitedLocations[worldState.agentLocation.X][worldState.agentLocation.Y] = true;
	}
	// Update safe locations
	x = worldState.agentLocation.X;
	y = worldState.agentLocation.Y;
	safeLocations[x][y] = true; // current location must be safe
	if (((! percept.Stench) || (! worldState.wumpusAlive)) &&
		(! percept.Breeze))
	{
		// Adjacent locations are safe
		if (x > 1)
			safeLocations[x-1][y] = true;
		if (x < 4)
			safeLocations[x+1][y] = true;
		if (y > 1)
			safeLocations[x][y-1] = true;
		if (y < 4)
			safeLocations[x][y+1] = true;
	}

	// HW3:
	// Update stench information
	if (percept.Stench)
	{
		stenchLocations[x][y] = true;
	}
	// Check if we now know where wumpus is
	if (! wumpusLocationKnown)
	{
		wumpusLocationKnown = InferWumpusLocation();
		if (wumpusLocationKnown)
		{
			cout << "Found the wumpus at (" << worldState.wumpusLocation.X << "," << worldState.wumpusLocation.Y << ")!\n";
		}
	}
}

void Agent::ExecuteAction (Action action)
{
	Orientation orient = worldState.agentOrientation;
	if (action == FORWARD)
	{
		// Assume not facing wall
		if (orient == UP)
			worldState.agentLocation.Y++;
		if (orient == DOWN)
			worldState.agentLocation.Y--;
		if (orient == LEFT)
			worldState.agentLocation.X--;
		if (orient == RIGHT)
			worldState.agentLocation.X++;
		visitedLocations[worldState.agentLocation.X][worldState.agentLocation.Y] = true;
	} else if (action == TURNLEFT) {
		if (orient == UP)
			worldState.agentOrientation = LEFT;
		if (orient == DOWN)
			worldState.agentOrientation = RIGHT;
		if (orient == LEFT)
			worldState.agentOrientation = DOWN;
		if (orient == RIGHT)
			worldState.agentOrientation = UP;
	} else if (action == TURNRIGHT) {
		if (orient == UP)
			worldState.agentOrientation = RIGHT;
		if (orient == DOWN)
			worldState.agentOrientation = LEFT;
		if (orient == LEFT)
			worldState.agentOrientation = UP;
		if (orient == RIGHT)
			worldState.agentOrientation = DOWN;
	} else if (action == GRAB) {
		// Assume gold is here
		worldState.agentHasGold = true;
	} else if (action == SHOOT) {
		worldState.agentHasArrow = false;
	} else if (action == CLIMB) {
		// Nothing to do here; either leave game if in [1,1] else no effect
	}
}

bool Agent::CanReachSafeUnvisitedLocation()
{
	int x, y;
	Location goalLocation;
	Orientation goalOrientation = worldState.agentOrientation; // HW3: including orientation in search

	for (x = 1 ; x <= 4; x++)
	{
		for (y = 1; y <= 4; y++)
		{
			if ((safeLocations[x][y] && (! visitedLocations[x][y])))
			{
				goalLocation.X = x;
				goalLocation.Y = y;
				if (PlanSafeRoute(goalLocation, goalOrientation, false)) // HW3: adds orientation
				{
					cout << "Navigating to safe location (" << x << "," << y << ")\n";
					return true;
				}
			}
		}
	}
	return false;
}

bool Agent::PlanSafeRoute (Location& goalLocation, Orientation goalOrientation, bool testOrientation)
// Attempts to find route from agent's current location to the given goalLocation
// that passes through only known safe locations. If testOrientation=true, then
// the goal test also constains orientation to be goalOrientation. If route found,
// then returns true and actions to get there are put on plan; otherwise, returns
// false and no change to plan. This is breadth-first search.
{
	list<SearchState*> frontier;
	list<SearchState*> explored;
	SearchState* initialState = new SearchState(worldState.agentLocation, worldState.agentOrientation, FORWARD, NULL);
	SearchState* currentState;
	SearchState* nextState;

	if ((initialState->location == goalLocation) && // goal test
		(!testOrientation || (initialState->orientation == goalOrientation))) // HW3 adds orientation to goal test
	{
		delete initialState; // HW3 (should have been in HW2)
		return true; // nothing added to plan
	}
	frontier.push_back (initialState);
	while (! frontier.empty())
	{
		currentState = frontier.front();
		frontier.pop_front();
		explored.push_back (currentState);
		// Try actions: FORWARD=0, TURNLEFT=1, TURNRIGHT=2
		for (int action = 0; action < 3; action++)
		{
			if ((nextState = ChildState (currentState, (Action) action)) != NULL)
			{
				if ((nextState->location == goalLocation) &&
					(!testOrientation || (nextState->orientation == goalOrientation))) // HW3 adds orientation to goal test
				{
					AddRouteToPlan(nextState);
					ClearStates (frontier);
					ClearStates (explored);
					return true;
				} else {
					if ((find (explored.begin(), explored.end(), nextState) == explored.end()) &&
						(find (frontier.begin(), frontier.end(), nextState) == frontier.end())) {
						frontier.push_back(nextState);
					}
				}
			}
		}
	}
	ClearStates (explored);
	return false;
}

void Agent::AddRouteToPlan (SearchState* state)
{
	list<Action> route;
	list<Action>::iterator itr;

	while (state->parent != NULL)
	{
		route.push_front(state->action);
		state = state->parent;
	}
	for (itr = route.begin(); itr != route.end(); itr++)
	{
		plan.push_back(*itr);
	}
}

SearchState* Agent::ChildState (SearchState* state, Action action)
{
	int x, y;
	SearchState* childState = NULL;
	Location newLocation;

	if (action == TURNLEFT)
	{
		childState = new SearchState (state->location, state->orientation, TURNLEFT, state);
		switch (state->orientation)
		{
			case UP: childState->orientation = LEFT; break;
			case DOWN: childState->orientation = RIGHT; break;
			case LEFT: childState->orientation = DOWN; break;
			case RIGHT: childState->orientation = UP; break;
		}
	}
	if (action == TURNRIGHT)
	{
		childState = new SearchState (state->location, state->orientation, TURNRIGHT, state);
		switch (state->orientation)
		{
			case UP: childState->orientation = RIGHT; break;
			case DOWN: childState->orientation = LEFT; break;
			case LEFT: childState->orientation = UP; break;
			case RIGHT: childState->orientation = DOWN; break;
		}
	}
	if (action == FORWARD)
	{
		x = state->location.X;
		y = state->location.Y;
		switch (state->orientation)
		{
			case UP: y++; break;
			case DOWN: y--; break;
			case LEFT: x--; break;
			case RIGHT: x++; break;
		}
		if (safeLocations[x][y])
		{
			newLocation.X = x;
			newLocation.Y = y;
			childState = new SearchState (newLocation, state->orientation, FORWARD, state);
		}
	}
	return childState;
}

void Agent::ClearStates (list<SearchState*> stateList)
{
	SearchState* tmpState;
	list<SearchState*>::iterator itr;
	for (itr = stateList.begin(); itr != stateList.end(); itr++)
	{
		tmpState = *itr;
		delete tmpState;
	}
}

// ----- HW3 -----

bool Agent::CanKillWumpus ()
{
	int x, y, wumpusX, wumpusY;
	Location goalLocation;
	Orientation goalOrientation;

	if (wumpusLocationKnown && worldState.wumpusAlive && worldState.agentHasArrow)
	{
		// Look for safe location facing wumpus agent can safely get to
		wumpusX = worldState.wumpusLocation.X;
		wumpusY = worldState.wumpusLocation.Y;
		for (x = 1; x <= 4; x++)
		{
			for (y = 1; y <= 4; y++)
			{
				if ((((x == wumpusX) && (y != wumpusY)) || ((x != wumpusX) && (y == wumpusY))) &&
						safeLocations[x][y])
				{
					// Found safe location facing wumpus
					goalLocation.X = x;
					goalLocation.Y = y;
					// Find correct orientation
					if ((x == wumpusX) && (y < wumpusY))
						goalOrientation = UP;
					if ((x == wumpusX) && (y > wumpusY))
						goalOrientation = DOWN;
					if ((x < wumpusX) && (y == wumpusY))
						goalOrientation = RIGHT;
					if ((x > wumpusX) && (y == wumpusY))
						goalOrientation = LEFT;
					// Try to get there safely
					if (PlanSafeRoute(goalLocation, goalOrientation, true))
					{
						cout << "Moving to shoot wumpus from (" << x << "," << y << ")\n";
						plan.push_back(SHOOT);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Agent::InferWumpusLocation ()
{
	int x, y, numStenches;

	for (x = 1; x <= 4; x++)
	{
		for (y = 1; y <= 4; y++)
		{
			// Check if we can infer wumpus at (x,y)
			// First compute number of surrounding stenches
			numStenches = 0;
			if ((x > 1) && stenchLocations[x-1][y])
				numStenches++;
			if ((x < 4) && stenchLocations[x+1][y])
				numStenches++;
			if ((y > 1) && stenchLocations[x][y-1])
				numStenches++;
			if ((y < 4) && stenchLocations[x][y+1])
				numStenches++;
			// If 3 or 4 surrounding stenches, then found it
			if (numStenches > 2)
			{
				worldState.wumpusLocation.X = x;
				worldState.wumpusLocation.Y = y;
				return true;
			}
			// If 2 stenches in line, then found it (2 combinations)
			if (((x > 1) && (x < 4) && stenchLocations[x-1][y] && stenchLocations[x+1][y]) ||
					((y > 1) && (y < 4) && stenchLocations[x][y-1] && stenchLocations[x][y+1]))
			{
				worldState.wumpusLocation.X = x;
				worldState.wumpusLocation.Y = y;
				return true;
			}
			// If 2 diagonal stenches adjacent to non-stench, then found it (4 combinations)
			if (((x < 4) && (y < 4) && stenchLocations[x][y+1] && stenchLocations[x+1][y] && NoStench(x+1,y+1)) ||
				((x < 4) && (y > 1) && stenchLocations[x][y-1] && stenchLocations[x+1][y] && NoStench(x+1,y-1)) ||
				((x > 1) && (y > 1) && stenchLocations[x][y-1] && stenchLocations[x-1][y] && NoStench(x-1,y-1)) ||
				((x > 1) && (y < 4) && stenchLocations[x][y+1] && stenchLocations[x-1][y] && NoStench(x-1,y+1)))
			{
				worldState.wumpusLocation.X = x;
				worldState.wumpusLocation.Y = y;
				return true;
			}
			// If 1 stench with 3 adjacent non-stenches (or walls), then found it (4 combinations)
			if (((y > 1) && stenchLocations[x][y-1] && NoStench(x-1,y-1) && NoStench(x,y-2) && NoStench(x+1,y-1)) ||
				((y < 4) && stenchLocations[x][y+1] && NoStench(x-1,y+1) && NoStench(x,y+2) && NoStench(x+1,y+1)) ||
				((x > 1) && stenchLocations[x-1][y] && NoStench(x-1,y+1) && NoStench(x-2,y) && NoStench(x-1,y-1)) ||
				((x < 4) && stenchLocations[x+1][y] && NoStench(x+1,y+1) && NoStench(x+2,y) && NoStench(x+1,y-1)))
			{
				worldState.wumpusLocation.X = x;
				worldState.wumpusLocation.Y = y;
				return true;
			}
		}
	}
	return false;
}

bool Agent::NoStench (int x, int y)
// Returns true if (x,y) is off-world, or visited and no stench
{
	if ((x < 1) || (x > 4) || (y < 1) || (y > 4))
		return true;
	if (visitedLocations[x][y] && !stenchLocations[x][y])
		return true;
	return false;
}
