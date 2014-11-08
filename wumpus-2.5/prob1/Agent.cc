// Agent.cc
//
// Homework 1 Solution

#include <iostream>
#include "Agent.h"

using namespace std;

Agent::Agent ()
{

}

Agent::~Agent ()
{

}

void Agent::Initialize ()
{
	location = Location(1,1);
	orientation = RIGHT;
	hasGold = false;
	safe = false;
}

Action Agent::Process (Percept& percept)
{
	Action action;

	if (hasGold)
	{
		// Return to (1,1) along same path (assumed safe) and climb
		if (location == Location(1,1))
			action = CLIMB;
		else if ((location == Location(4,4)) && (orientation != DOWN))
			action = TURNLEFT;
		else if ((location == Location(4,1)) && (orientation != LEFT))
			action = TURNRIGHT;
		else action = FORWARD;
	} else {
		// Head to (4,4), avoiding wumpus, and grab gold
		if (location == Location(4,4))
			action = GRAB;
		else if ((location == Location(4,1)) && (orientation != UP))
			action = TURNLEFT;
		else if (percept.Stench && (! safe))
			action = SHOOT;
		else action = FORWARD;
	}
	UpdateState (action);
	return action;
}

void Agent::GameOver (int score)
{

}

void Agent::UpdateState (Action action)
{
	if (action == GRAB)
		hasGold = true;
	else if (action == TURNLEFT)
		ExecuteTurnLeft();
	else if (action == TURNRIGHT)
		ExecuteTurnRight();
	else if (action == FORWARD)
		ExecuteForward();
	else if (action == SHOOT)
		safe = true;
	// No updates needed for CLIMB
}

void Agent::ExecuteForward ()
{
	if (orientation == UP)
		location.Y = location.Y + 1;
	else if (orientation == DOWN)
		location.Y = location.Y - 1;
	else if (orientation == LEFT)
		location.X = location.X - 1;
	else location.X = location.X + 1;
}

void Agent::ExecuteTurnLeft ()
{
	if (orientation == UP)
		orientation = LEFT;
	else if (orientation == DOWN)
		orientation = RIGHT;
	else if (orientation == LEFT)
		orientation = DOWN;
	else orientation = UP;
}

void Agent::ExecuteTurnRight ()
{
	if (orientation == UP)
		orientation = RIGHT;
	else if (orientation == DOWN)
		orientation = LEFT;
	else if (orientation == LEFT)
		orientation = UP;
	else orientation = DOWN;
}
