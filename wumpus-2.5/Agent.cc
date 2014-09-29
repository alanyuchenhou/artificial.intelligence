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
  previousAction = CLIMB;
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
void Agent::updateCoordinate (Percept&percept)
{
  if ((previousAction == FORWARD) && (! percept.Bump))
    {
      if (agentOrientation == RIGHT)
	agentLocation.X++;
      else if (agentOrientation == LEFT)
	agentLocation.X--;
      else if (agentOrientation == UP)
	agentLocation.Y++;
      else if (agentOrientation == DOWN)
	agentLocation.Y--;
    }
  if (previousAction == TURNLEFT)
    {
      if (agentOrientation == RIGHT)
	agentOrientation = UP;
      else if (agentOrientation == LEFT)
	agentOrientation = DOWN;
      else if (agentOrientation == UP)
	agentOrientation = LEFT;
      else if (agentOrientation == DOWN)
	agentOrientation = RIGHT;
    }
  if (previousAction == TURNRIGHT)
    {
      if (agentOrientation == RIGHT)
	agentOrientation = DOWN;
      else if (agentOrientation == LEFT)
	agentOrientation = UP;
      else if (agentOrientation == UP)
	agentOrientation = RIGHT;
      else if (agentOrientation == DOWN)
	agentOrientation = LEFT;
    }
}
void Agent::updateKnowledgeBase (Percept& percept)
{
  updateCoordinate(percept);
  int x, y;
  x = agentLocation.X;
  y = agentLocation.Y;
  if (previousAction == GRAB)
    agentHasGold = true;
  site[x][y].visited = true;
  site[x][y].safe = YES;
  site[x][y].pit = NO;
  site[x][y].wumpus = NO;
  if (percept.Scream) {
    wumpusAlive = false;
  }
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
  locatePits();
  if (wumpusAlive && !wumpusLocated) {
    locateWumpus();
  }
}

void Agent::locatePits ()
{
  int x, y;
  int pitSiteCount;
  Location pitSite;

  for (x = 1; x <= DIMENSION; x++)
    {
      for (y = 1; y <= DIMENSION; y++)
	{
	  if (site[x][y].breeze == YES)
	    {
	      pitSiteCount = 4;
	      if ((y == DIMENSION) || (site[x][y+1].pit == NO))
		{
		  pitSiteCount--;
		}
	      else {
		pitSite.X = x;
		pitSite.Y = y+1;
	      }
	      if ((y == 1) || (site[x][y-1].pit == NO))
		{
		  pitSiteCount--;
		}
	      else {
		pitSite.X = x;
		pitSite.Y = y-1;
	      }
	      if ((x == 1) || (site[x-1][y].pit == NO))
		{
		  pitSiteCount--;
		}
	      else {
		pitSite.X = x-1;
		pitSite.Y = y;
	      }
	      if ((x == DIMENSION) || (site[x+1][y].pit == NO))
		{
		  pitSiteCount--;
		}
	      else {
		pitSite.X = x+1;
		pitSite.Y = y;
	      }
	      if ((pitSiteCount == 1) && (site[pitSite.X][pitSite.Y].pit == UNKNOWN))
		{
		  cout << "pit at (" << pitSite.X << "," << pitSite.Y << ")\n";
		  site[pitSite.X][pitSite.Y].pit = YES;
		  site[pitSite.X][pitSite.Y].safe = NO;
		}
	    }
	}
    }
}
void Agent::locateWumpus ()
{
  int x, y;
  int wumpusSiteCount;
  Location wumpusSite;
  for (x = 1; x <= DIMENSION; x++)
    {
      for (y = 1; y <= DIMENSION; y++)
	{
	  if (site[x][y].stench == YES)
	    {
	      wumpusSiteCount = 4;
	      if ((y == DIMENSION) || (site[x][y+1].wumpus == NO))
		{
		  wumpusSiteCount--;
		}
	      else {
		wumpusSite.X = x;
		wumpusSite.Y = y+1;
	      }
	      if ((y == 1) || (site[x][y-1].wumpus == NO))
		{
		  wumpusSiteCount--;
		}
	      else {
		wumpusSite.X = x;
		wumpusSite.Y = y-1;
	      }
	      if ((x == 1) || (site[x-1][y].wumpus == NO))
		{
		  wumpusSiteCount--;
		}
	      else {
		wumpusSite.X = x-1;
		wumpusSite.Y = y;
	      }
	      if ((x == DIMENSION) || (site[x+1][y].wumpus == NO))
		{
		  wumpusSiteCount--;
		}
	      else {
		wumpusSite.X = x+1;
		wumpusSite.Y = y;
	      }
	      if (wumpusSiteCount == 1)
		{
		  wumpusLocated = true;
		  cout << "wumpus at (" << wumpusSite.X << "," << wumpusSite.Y << ")\n";
		  Agent::wumpusLocation = wumpusSite;
		  for (int x = 1; x <= DIMENSION; x++) {
		    for (int y = 1; y <= DIMENSION; y++) {
		      site[x][y].wumpus = NO;
		    }
		  }
		  site[wumpusSite.X][wumpusSite.Y].wumpus = YES;
		  site[wumpusSite.X][wumpusSite.Y].safe = NO;
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

  // If agent perceives gold, then grab it
  if (percept.Glitter)
    {
      // If agent sees gold, then GRAB
      action = GRAB;
    } else if (agentHasGold) {
    // If agent has gold, then navigate to (1,1) and climb
    if (agentLocation == location11)
      {
	action = CLIMB;
      } else {
      action = getMove (agentLocation, agentOrientation, location11, agentOrientation);
    }
  } else if (getSafeLocation (goalLocation)) {
    // If no gold, then navigate to safe, unvisited location (if exists)
    action = getMove (agentLocation, agentOrientation, goalLocation, agentOrientation);
  } else if (wumpusAlive && getShootingPosition (goalLocation, goalOrientation)) {
    // If can shoot live wumpus, then navigate to safe location facing wumpus and SHOOT
    if ((agentLocation == goalLocation) && (agentOrientation == goalOrientation))
      {
	action = SHOOT;
      } else {
      action = getMove (agentLocation, agentOrientation, goalLocation, goalOrientation);
    }
  } else if (getRiskyLocation (goalLocation)) {
    // If none of the above works out, then take a risk and navigate to an unknown, unvisited location (if exists)
    site[goalLocation.X][goalLocation.Y].safe = YES; //temporarily, so search can get there.
    action = getMove (agentLocation, agentOrientation, goalLocation, agentOrientation);
    site[goalLocation.X][goalLocation.Y].safe = UNKNOWN;
  } else if (agentLocation == location11) { // nothing above worked, so just give up
    action = CLIMB;
  } else {
    action = getMove (agentLocation, agentOrientation,
		       location11, agentOrientation);
  }
  return action;
}

// Returns true if safe, unvisited location exists, and stores it in given location.
// Otherwise, returns false;
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

// See if there is a safe=UNKNOWN location next to a safe=YES location.
// If so, set location and return true; otherwise, returns false.
bool Agent::getRiskyLocation (Location& location)
{
  int x, y;
  bool found = false;

  for (x = 1; x <= DIMENSION; x++)
    {
      for (y = 1; y <= DIMENSION; y++)
	{
	  if (site[x][y].safe == YES)
	    {
	      // Check UP
	      if ((y < DIMENSION) && (site[x][y+1].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x;
		  location.Y = y+1;
		}
	      // Check DOWN
	      if ((y > 1) && (site[x][y-1].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x;
		  location.Y = y-1;
		}
	      // Check LEFT
	      if ((x > 1) && (site[x-1][y].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x-1;
		  location.Y = y;
		}
	      // Check RIGHT
	      if ((x < DIMENSION) && (site[x+1][y].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x+1;
		  location.Y = y;
		}
	    }
	}
    }
  return found;
}

// Returns true if safe location facing wumpus exists, and stores it in given location and orientation.
// If wumpus location unknown or safe facing location not available, returns false;
bool Agent::getShootingPosition (Location& location, Orientation& orientation)
{
  int x, y;
  bool found = false;

  if (! (wumpusLocation == Location(0,0)))
    {
      for (x = 1; x <= DIMENSION; x++)
	{
	  for (y = 1; y <= DIMENSION; y++)
	    {
	      if ((! found) && (site[x][y].safe == YES) &&
		  ((x == wumpusLocation.X) || (y == wumpusLocation.Y)))
		{
		  found = true;
		  location.X = x;
		  location.Y = y;
		}
	    }
	}
      if (found)
	{
	  // Determine orientation
	  if (location.X == wumpusLocation.X)
	    {
	      if (location.Y < wumpusLocation.Y)
		{
		  orientation = UP;
		} else {
		orientation = DOWN;
	      }
	    } else {
	    if (location.X < wumpusLocation.X)
	      {
		orientation = RIGHT;
	      } else {
	      orientation = LEFT;
	    }
	  }
	}
    }
  return found;
}

// Traverse up parent states until depth=1 state, and return its action.
Action Agent::getFirstMove (stage* state)
{
  Action action = CLIMB; // dummy action
  while (state->parent != NULL)
    {
      // if (state->parent->parent == NULL)
      // 	{
      // 	  action = state->action;
      // 	}
      action = state->action;
      state = state->parent;
    }
  return action;
}

Action Agent::getMove (Location& startLocation, Orientation& startOrientation,
			Location& goalLocation, Orientation& goalOrientation)
{
  cout << "moving: (" << startLocation.X << "," << startLocation.Y << ")->("
       << goalLocation.X << "," << goalLocation.Y << ")\n";

  stage* initialState = new stage (startLocation, startOrientation, 0, NULL, CLIMB);
  stage* goalState = new stage (goalLocation, goalOrientation, 0, NULL, CLIMB);
  stage* finalState = iterativeDeepeningSearch (initialState, goalState);
  stage* tmpState;
  Action action = getFirstMove (finalState);
  // Delete states (while-loop deletes initialState as well)
  while (finalState != NULL)
    {
      tmpState = finalState;
      finalState = finalState->parent;
      delete tmpState;
    }
  delete goalState;
  return action;
}

stage* Agent::iterativeDeepeningSearch (stage* initialState, stage* goalState)
{
  // Solution depth must be at least the city block distance between the initial and goal locations
  int depthLimit = getDistance (initialState->location, goalState->location);
  stage* finalState = NULL;

  while (finalState == NULL)
    {
      finalState = depthLimitedSearch (initialState, goalState, depthLimit);
      depthLimit++;
    }
  return finalState;
}

stage* Agent::depthLimitedSearch (stage* state, stage* goalState, int depthLimit)
{
  stage* childState;
  stage* resultState;

  if (goalTest (state, goalState))
    {
      return state;
    }
  if (state->depth < depthLimit)
    {
      childState = getChildState (state, FORWARD);
      if (childState != NULL)
	{
	  resultState = depthLimitedSearch (childState, goalState, depthLimit);
	  if (resultState != NULL) return resultState;
	  else delete childState;
	}
      childState = getChildState (state, TURNLEFT);
      resultState = depthLimitedSearch (childState, goalState, depthLimit);
      if (resultState != NULL) return resultState;
      else delete childState;
      childState = getChildState (state, TURNRIGHT);
      resultState = depthLimitedSearch (childState, goalState, depthLimit);
      if (resultState != NULL) return resultState;
      else delete childState;
    }
  return NULL;
}

// True if state location and orientation same as goal location and orientation.
bool Agent::goalTest (stage* state, stage* goalState)
{
  if ((state->location.X == goalState->location.X) &&
      (state->location.Y == goalState->location.Y) &&
      (state->orientation == goalState->orientation))
    return true;
  else return false;
}

// Returns new state after applying action to given state. For FORWARD, only
// works if adjacent location exists and is safe. If not, return NULL.
stage* Agent::getChildState (stage* state, Action action)
{
  int x, y;
  stage* childState = NULL;
  Location newLocation;

  if (action == TURNLEFT)
    {
      childState = new stage (state->location, state->orientation, state->depth + 1, state, TURNLEFT);
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
      childState = new stage (state->location, state->orientation, state->depth + 1, state, TURNRIGHT);
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
      if ((x >= 1) && (x <= DIMENSION) && (y >= 1) && (y <= DIMENSION) && (site[x][y].safe == YES))
	{
	  newLocation.X = x;
	  newLocation.Y = y;
	  childState = new stage (newLocation, state->orientation, state->depth + 1, state, FORWARD);
	}
    }
  return childState;
}

