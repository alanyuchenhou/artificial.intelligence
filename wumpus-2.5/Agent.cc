// I received help from my friends Xiyu Xie and Prabal Poudel.
// The help I received from them includes both algorithm and data structure design.
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
  agentLocation = Location(1,1);
  agentOrientation = RIGHT;
  agentHasGold = false;
  wumpusAlive = true;
  wumpusLocated = NO;
  wumpusLocation = Location(0,0);
  previousAction = SHOOT;
  for (int x = 1; x <= DIMENSION; x++)
    {
      for (int y = 1; y <= DIMENSION; y++)
    	{
	  site[x][y].visited = false;
	  site[x][y].safe = UNKNOWN;
	  site[x][y].stench = UNKNOWN;
	  site[x][y].breeze = UNKNOWN;
	  site[x][y].wumpus = UNKNOWN;
	  site[x][y].pit = UNKNOWN;
    	}
    }
  site[1][1].visited = true;
  site[1][1].safe = YES;
  site[1][1].wumpus = NO;
  site[1][1].pit = NO;
}

Action Agent::Process (Percept& percept)
{
  cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n";
  updateKnowledgeBase (percept);
  previousAction = getNextAction (percept);
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> \n";
  return previousAction;
}

void Agent::GameOver (int score)
{
  score = 0;
}
int Agent::getDistance (Location& l1, Location& l2)
{
  return (abs (l1.X - l2.X) + abs (l1.Y - l2.Y));
}
void Agent::updateKnowledgeBase (Percept& percept)
{
  updateCoordinate(percept);
  int x, y;
  x = agentLocation.X;
  y = agentLocation.Y;
  if (previousAction == GRAB)
    agentHasGold = true;
  site[x][y].pit = NO;
  site[x][y].wumpus = NO;
  site[x][y].visited = true;
  site[x][y].safe = YES;
  if (percept.Scream)
    wumpusAlive = false;
  if (percept.Stench)
    site[x][y].stench = YES;
  else site[x][y].stench = NO;
  if (percept.Breeze)
    site[x][y].breeze = YES;
  else site[x][y].breeze = NO;
  for (int xi = x-1; xi <= x+1; xi++) {
    for (int yi = y-1; yi <= y+1; yi++) {
      if ( ((xi == x)&&(yi != y)) || ((yi == y)&&(xi != x)) ) {
	if (! percept.Breeze)
	  site[xi][yi].pit = NO;
	if (! percept.Stench)
	  site[xi][yi].wumpus = NO;
	if ( ((! wumpusAlive)||(site[xi][yi].wumpus == NO)) && (site[xi][yi].pit == NO) ) {
	  site[xi][yi].safe = YES;
	}
      }
    }
  }
  updatePitMap();
  if (wumpusAlive && !wumpusLocated) {
    locateWumpus();
  }
}

void Agent::updatePitMap () {
  int x, y;
  int suspectPitCount = 4;
  Location suspectPitLocation;

  for (x = 1; x <= DIMENSION; x++) {
    for (y = 1; y <= DIMENSION; y++) {
      if (site[x][y].breeze == YES) {
	if ((x == 1) || (site[x-1][y].pit == NO)) {
	  suspectPitCount--;
	}
	else {
	  suspectPitLocation.X = x-1;
	  suspectPitLocation.Y = y;
	}
	if ((x == DIMENSION) || (site[x+1][y].pit == NO)) {
	  suspectPitCount--;
	}
	else {
	  suspectPitLocation.X = x+1;
	  suspectPitLocation.Y = y;
	}
	if ((y == 1) || (site[x][y-1].pit == NO)) {
	  suspectPitCount--;
	}
	else {
	  suspectPitLocation.X = x;
	  suspectPitLocation.Y = y-1;
	}
	if ((y == DIMENSION) || (site[x][y+1].pit == NO)) {
	  suspectPitCount--;
	}
	else {
	  suspectPitLocation.X = x;
	  suspectPitLocation.Y = y+1;
	}
	if ((suspectPitCount == 1) && (site[suspectPitLocation.X][suspectPitLocation.Y].pit == UNKNOWN))
	  {
	    cout << "pit located at (" << suspectPitLocation.X << "," << suspectPitLocation.Y << ")\n";
	    site[suspectPitLocation.X][suspectPitLocation.Y].pit = YES;
	    site[suspectPitLocation.X][suspectPitLocation.Y].safe = NO;
	  }
      }
    }
  }
}
void Agent::locateWumpus () {
  int x, y;
  int suspectwumpusCount = 4;
  Location suspectWumpusLocation;
  for (x = 1; x <= DIMENSION; x++)
    {
      for (y = 1; y <= DIMENSION; y++)
	{
	  if (site[x][y].stench == YES)
	    {
	      if ((y == DIMENSION) || (site[x][y+1].wumpus == NO))
		{
		  suspectwumpusCount--;
		}
	      else {
		suspectWumpusLocation.X = x;
		suspectWumpusLocation.Y = y+1;
	      }
	      if ((y == 1) || (site[x][y-1].wumpus == NO))
		{
		  suspectwumpusCount--;
		}
	      else {
		suspectWumpusLocation.X = x;
		suspectWumpusLocation.Y = y-1;
	      }
	      if ((x == 1) || (site[x-1][y].wumpus == NO))
		{
		  suspectwumpusCount--;
		}
	      else {
		suspectWumpusLocation.X = x-1;
		suspectWumpusLocation.Y = y;
	      }
	      if ((x == DIMENSION) || (site[x+1][y].wumpus == NO))
		{
		  suspectwumpusCount--;
		}
	      else {
		suspectWumpusLocation.X = x+1;
		suspectWumpusLocation.Y = y;
	      }
	      if (suspectwumpusCount == 1)
		{
		  wumpusLocated = true;
		  cout << "wumpus at (" << suspectWumpusLocation.X << "," << suspectWumpusLocation.Y << ")\n";
		  Agent::wumpusLocation = suspectWumpusLocation;
		  for (int x = 1; x <= DIMENSION; x++) {
		    for (int y = 1; y <= DIMENSION; y++) {
		      site[x][y].wumpus = NO;
		    }
		  }
		  site[suspectWumpusLocation.X][suspectWumpusLocation.Y].wumpus = YES;
		  site[suspectWumpusLocation.X][suspectWumpusLocation.Y].safe = NO;
		  return;
		}
	    }
	}
    }
}

Action Agent::getNextAction (Percept& percept)
{
  Action action;
  Location goalLocation;
  Orientation goalOrientation;
  Location location11 = Location(1,1);

  if (percept.Glitter) {
    action = GRAB;
  }
  else if (agentHasGold) {
    if (agentLocation == location11) {
      action = CLIMB;
    }
    else {
      action = getMove (agentLocation, agentOrientation, location11, agentOrientation);
    }
  }
  else if (getSafeLocation (goalLocation)) {
    action = getMove (agentLocation, agentOrientation, goalLocation, goalOrientation);
  }
  else if (wumpusAlive && getShootingPosition (goalLocation, goalOrientation)) {
    if ((agentLocation == goalLocation) && (agentOrientation == goalOrientation)) {
      action = SHOOT;
    }
    else {
      action = getMove (agentLocation, agentOrientation, goalLocation, goalOrientation);
    }
  }
  else if (getRiskyLocation (goalLocation)) {
    site[goalLocation.X][goalLocation.Y].safe = YES;
    action = getMove (agentLocation, agentOrientation, goalLocation, goalOrientation);
    site[goalLocation.X][goalLocation.Y].safe = UNKNOWN;
  }
  else if (agentLocation == location11) {
    action = CLIMB;
  }
  else {
    action = getMove (agentLocation, agentOrientation, location11, goalOrientation);
  }
  return action;
}

bool Agent::getSafeLocation (Location& location)
{
  int x, y;
  bool found = false;

  for (x = 1; x <= DIMENSION; x++)
    {
      for (y = 1; y <= DIMENSION; y++)
	{
	  if ((site[x][y].safe == YES) && (! site[x][y].visited))
	    {
	      found = true;
	      location.X = x;
	      location.Y = y;
	    }
	}
    }
  return found;
}

bool Agent::getRiskyLocation (Location& location)
{
  int x, y;
  bool found = false;

  for (x = 1; x <= DIMENSION; x++) {
    for (y = 1; y <= DIMENSION; y++) {
      if (site[x][y].safe == YES) {
	if ((y < DIMENSION) && (site[x][y+1].safe == UNKNOWN)) {
	  found = true;
	  location.X = x;
	  location.Y = y+1;
	}
	if ((y > 1) && (site[x][y-1].safe == UNKNOWN)) {
	  found = true;
	  location.X = x;
	  location.Y = y-1;
	}
	if ((x > 1) && (site[x-1][y].safe == UNKNOWN)) {
	  found = true;
	  location.X = x-1;
	  location.Y = y;
	}
	if ((x < DIMENSION) && (site[x+1][y].safe == UNKNOWN)) {
	  found = true;
	  location.X = x+1;
	  location.Y = y;
	}
      }
    }
  }
  return found;
}

bool Agent::getShootingPosition (Location& location, Orientation& orientation) {
  int x, y;
  bool found = false;

  if (! (wumpusLocation == Location(0,0))) {
    for (x = 1; x <= DIMENSION; x++) {
      for (y = 1; y <= DIMENSION; y++) {
	if ((! found) && (site[x][y].safe == YES) &&
	    ((x == wumpusLocation.X) || (y == wumpusLocation.Y))) {
	  found = true;
	  location.X = x;
	  location.Y = y;
	}
      }
    }
    if (found) {
      if (location.X == wumpusLocation.X) {
	if (location.Y < wumpusLocation.Y) {
	  orientation = UP;
	}
	else {
	  orientation = DOWN;
	}
      }
      else {
	if (location.X < wumpusLocation.X) {
	  orientation = RIGHT;
	}
	else {
	  orientation = LEFT;
	}
      }
    }
  }
  return found;
}

Action Agent::getFirstMove (stage* state) {
  Action action = CLIMB;
  while (state->parent != NULL) {
    action = state->action;
    state = state->parent;
  }
  return action;
}

Action Agent::getMove (Location& startLocation, Orientation& startOrientation,
		       Location& goalLocation, Orientation& goalOrientation) {
  cout << "moving: (" << startLocation.X << "," << startLocation.Y << ")->("
       << goalLocation.X << "," << goalLocation.Y << ")\n";
  stage* initialState = new stage (startLocation, startOrientation, 0, NULL, CLIMB);
  stage* goalState = new stage (goalLocation, goalOrientation, 0, NULL, CLIMB);
  stage* finalState = iterativeDeepeningSearch (initialState, goalState);
  stage* tmpState;
  Action action = getFirstMove (finalState);
  while (finalState != NULL) {
    tmpState = finalState;
    finalState = finalState->parent;
    delete tmpState;
  }
  delete goalState;
  return action;
}

stage* Agent::iterativeDeepeningSearch (stage* initialState, stage* goalState)
{
  int depthLimit = getDistance (initialState->location, goalState->location);
  stage* finalState = NULL;

  while (finalState == NULL) {
    finalState = depthLimitedSearch (initialState, goalState, depthLimit);
    depthLimit++;
  }
  return finalState;
}

stage* Agent::depthLimitedSearch (stage* state, stage* goalState, int depthLimit)
{
  stage* childState;
  stage* resultState;

  if (goalTest (state, goalState)) {
    return state;
  }
  if (state->depth < depthLimit) {
    childState = getChildState (state, FORWARD);
    if (childState != NULL) {
      resultState = depthLimitedSearch (childState, goalState, depthLimit);
      if (resultState != NULL)
	return resultState;
      else
	delete childState;
    }
    childState = getChildState (state, TURNLEFT);
    resultState = depthLimitedSearch (childState, goalState, depthLimit);
    if (resultState != NULL)
      return resultState;
    else
      delete childState;
    childState = getChildState (state, TURNRIGHT);
    resultState = depthLimitedSearch (childState, goalState, depthLimit);
    if (resultState != NULL)
      return resultState;
    else
      delete childState;
  }
  return NULL;
}

bool Agent::goalTest (stage* state, stage* goalState)
{
  if ((state->location.X == goalState->location.X) &&
      (state->location.Y == goalState->location.Y) &&
      (state->orientation == goalState->orientation))
    return true;
  else
    return false;
}

stage* Agent::getChildState (stage* state, Action action) {
  int x, y;
  stage* childState = NULL;
  Location newLocation;

  if (action == TURNLEFT) {
    childState = new stage (state->location, state->orientation,
			    state->depth + 1, state, TURNLEFT);
    switch (state->orientation) {
    case UP: childState->orientation = LEFT; break;
    case DOWN: childState->orientation = RIGHT; break;
    case LEFT: childState->orientation = DOWN; break;
    case RIGHT: childState->orientation = UP; break;
    }
  }

  if (action == TURNRIGHT) {
    childState = new stage (state->location, state->orientation,
			    state->depth + 1, state, TURNRIGHT);
    switch (state->orientation) {
    case UP: childState->orientation = RIGHT; break;
    case DOWN: childState->orientation = LEFT; break;
    case LEFT: childState->orientation = UP; break;
    case RIGHT: childState->orientation = DOWN; break;
    }
  }

  if (action == FORWARD) {
    x = state->location.X;
    y = state->location.Y;
    switch (state->orientation) {
    case UP: y++; break;
    case DOWN: y--; break;
    case LEFT: x--; break;
    case RIGHT: x++; break;
    }
    if ((x >= 1) && (x <= DIMENSION) && (y >= 1) &&
	(y <= DIMENSION) && (site[x][y].safe == YES))	{
      newLocation.X = x;
      newLocation.Y = y;
      childState = new stage (newLocation, state->orientation,
			      state->depth + 1, state, FORWARD);
    }
  }
  return childState;
}
void Agent::updateCoordinate (Percept & percept) {
  if ((previousAction == FORWARD) && (! percept.Bump)) {
    switch (agentOrientation) {
    case UP: agentLocation.Y++; break;
    case DOWN: agentLocation.Y--; break;
    case LEFT: agentLocation.X--; break;
    case RIGHT: agentLocation.X++; break;
    }
  }
  if (previousAction == TURNLEFT) {
    switch (agentOrientation) {
    case UP: agentOrientation = LEFT; break;
    case DOWN: agentOrientation = RIGHT; break;
    case LEFT: agentOrientation = DOWN; break;
    case RIGHT: agentOrientation = UP; break;
    }
  }
  if (previousAction == TURNRIGHT) {
    switch (agentOrientation) {
    case UP: agentOrientation = RIGHT; break;
    case DOWN: agentOrientation = LEFT; break;
    case LEFT: agentOrientation = UP; break;
    case RIGHT: agentOrientation = DOWN; break;
    }
  }
}
