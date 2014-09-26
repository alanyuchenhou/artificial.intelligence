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
  agentStatus.agentLocation = Location(1,1);
  agentStatus.agentOrientation = RIGHT;
  agentStatus.agentHasGold = false;
  agentStatus.wumpusAlive = true;
  agentStatus.wumpusLocation = Location(0,0); //unknown
  previousAction = SHOOT; // dummy action to start
  for (int x = 1; x <= DIMENSION; x++)
    {
      for (int y = 1; y <= DIMENSION; y++)
    	{
	  worldEnvironment[x][y].visited = false;
	  worldEnvironment[x][y].safe = UNKNOWN;
	  worldEnvironment[x][y].stench = UNKNOWN;
	  worldEnvironment[x][y].breeze = UNKNOWN;
	  worldEnvironment[x][y].wumpus = UNKNOWN;
	  worldEnvironment[x][y].pit = UNKNOWN;
    	}
    }
  worldEnvironment[1][1].visited = true;
  worldEnvironment[1][1].safe = YES;
  worldEnvironment[1][1].wumpus = NO;
  worldEnvironment[1][1].pit = NO;
}

Action Agent::Process (Percept& percept)
{
  updateKnowledgeBase (percept);
  previousAction = getNextAction (percept);

  return previousAction;
}

void Agent::GameOver (int score)
{
  cout << "my score is: " << score << endl;
}

void Agent::updateKnowledgeBase (Percept& percept)
{
  int x, y;

  x = agentStatus.agentLocation.X;
  y = agentStatus.agentLocation.Y;

  // Update agent state based on last action and given percept
  if ((previousAction == FORWARD) && (! percept.Bump))
    {
      if (agentStatus.agentOrientation == RIGHT)
	x++;
      else if (agentStatus.agentOrientation == LEFT)
	x--;
      else if (agentStatus.agentOrientation == UP)
	y++;
      else if (agentStatus.agentOrientation == DOWN)
	y--;
      agentStatus.agentLocation = Location(x,y);
    }
  if (previousAction == TURNLEFT)
    {
      if (agentStatus.agentOrientation == RIGHT)
	agentStatus.agentOrientation = UP;
      else if (agentStatus.agentOrientation == LEFT)
	agentStatus.agentOrientation = DOWN;
      else if (agentStatus.agentOrientation == UP)
	agentStatus.agentOrientation = LEFT;
      else if (agentStatus.agentOrientation == DOWN)
	agentStatus.agentOrientation = RIGHT;
    }
  if (previousAction == TURNRIGHT)
    {
      if (agentStatus.agentOrientation == RIGHT)
	agentStatus.agentOrientation = DOWN;
      else if (agentStatus.agentOrientation == LEFT)
	agentStatus.agentOrientation = UP;
      else if (agentStatus.agentOrientation == UP)
	agentStatus.agentOrientation = RIGHT;
      else if (agentStatus.agentOrientation == DOWN)
	agentStatus.agentOrientation = LEFT;
    }
  // Update pit/wumpus information (hey, I'm alive)
  worldEnvironment[x][y].pit = NO;
  if (agentStatus.wumpusAlive)
    worldEnvironment[x][y].wumpus = NO;

  // Update stench info
  if (percept.Stench)
    worldEnvironment[x][y].stench = YES;
  else worldEnvironment[x][y].stench = NO;

  // Update breeze info
  if (percept.Breeze)
    worldEnvironment[x][y].breeze = YES;
  else worldEnvironment[x][y].breeze = NO;

  // Update wumpus health
  if (percept.Scream)
    agentStatus.wumpusAlive = false;

  // Update agent's gold status (we only do a grab if saw glitter)
  if (previousAction == GRAB)
    agentStatus.agentHasGold = true;

  // Update safe and visited information about the world
  worldEnvironment[x][y].visited = true;
  worldEnvironment[x][y].safe = YES;
  if (((! agentStatus.wumpusAlive) || (! percept.Stench)) && (! percept.Breeze))
    {
      // locations adjacent to (x,y) are safe
      if (x > 1)
	worldEnvironment[x-1][y].safe = YES;
      if (x < DIMENSION)
	worldEnvironment[x+1][y].safe = YES;
      if (y > 1)
	worldEnvironment[x][y-1].safe = YES;
      if (y < DIMENSION)
	worldEnvironment[x][y+1].safe = YES;
    }
  // // Check if we can find the elusive wumpus
  // if (agentStatus.wumpusAlive && (agentStatus.wumpusLocation == Location(0,0)))
  //   LookForWumpus();

  // // Check if we can find a pit
  // LookForPits();
}

// Check if we can pinpoint the wumpus's location.
// For each stench, see if only one adjacent location where wumpus != NO.
// void Agent::LookForWumpus ()
// {
//   int x, y;
//   int possibleWumpusLocations;
//   Location wumpusLocation;

//   for (x = 1; x <= DIMENSION; x++)
//     {
//       for (y = 1; y <= DIMENSION; y++)
// 	{
// 	  if (worldEnvironment[x][y].stench == YES)
// 	    {
// 	      possibleWumpusLocations = 4;
// 	      // Check UP from (x,y)
// 	      if ((y == DIMENSION) || (worldEnvironment[x][y+1].wumpus == NO))
// 		{
// 		  possibleWumpusLocations--;
// 		} else {
// 		wumpusLocation.X = x;
// 		wumpusLocation.Y = y+1;
// 	      }
// 	      // Check DOWN from (x,y)
// 	      if ((y == 1) || (worldEnvironment[x][y-1].wumpus == NO))
// 		{
// 		  possibleWumpusLocations--;
// 		} else {
// 		wumpusLocation.X = x;
// 		wumpusLocation.Y = y-1;
// 	      }
// 	      // Check LEFT from (x,y)
// 	      if ((x == 1) || (worldEnvironment[x-1][y].wumpus == NO))
// 		{
// 		  possibleWumpusLocations--;
// 		} else {
// 		wumpusLocation.X = x-1;
// 		wumpusLocation.Y = y;
// 	      }
// 	      // Check RIGHT from (x,y)
// 	      if ((x == DIMENSION) || (worldEnvironment[x+1][y].wumpus == NO))
// 		{
// 		  possibleWumpusLocations--;
// 		} else {
// 		wumpusLocation.X = x+1;
// 		wumpusLocation.Y = y;
// 	      }
// 	      if (possibleWumpusLocations == 1)
// 		{
// 		  // Found it
// 		  cout << "Found wumpus at (" << wumpusLocation.X << "," << wumpusLocation.Y << ")\n";
// 		  agentStatus.wumpusLocation.X = wumpusLocation.X;
// 		  agentStatus.wumpusLocation.Y = wumpusLocation.Y;
// 		  worldEnvironment[wumpusLocation.X][wumpusLocation.Y].wumpus = YES;
// 		  // ***** TODO: Might want to set all other locations to wumpus=NO
// 		  return; // only one wumpus
// 		}
// 	    }
// 	}
//     }
// }

// Check if we can pinpoint any pit locations.
// For each breeze, see if only one adjacent location where pit != NO.
// void Agent::LookForPits ()
// {
//   int x, y;
//   int possiblePitLocations;
//   Location pitLocation;

//   for (x = 1; x <= DIMENSION; x++)
//     {
//       for (y = 1; y <= DIMENSION; y++)
// 	{
// 	  if (worldEnvironment[x][y].breeze == YES)
// 	    {
// 	      possiblePitLocations = 4;
// 	      // Check UP from (x,y)
// 	      if ((y == DIMENSION) || (worldEnvironment[x][y+1].pit == NO))
// 		{
// 		  possiblePitLocations--;
// 		} else {
// 		pitLocation.X = x;
// 		pitLocation.Y = y+1;
// 	      }
// 	      // Check DOWN from (x,y)
// 	      if ((y == 1) || (worldEnvironment[x][y-1].pit == NO))
// 		{
// 		  possiblePitLocations--;
// 		} else {
// 		pitLocation.X = x;
// 		pitLocation.Y = y-1;
// 	      }
// 	      // Check LEFT from (x,y)
// 	      if ((x == 1) || (worldEnvironment[x-1][y].pit == NO))
// 		{
// 		  possiblePitLocations--;
// 		} else {
// 		pitLocation.X = x-1;
// 		pitLocation.Y = y;
// 	      }
// 	      // Check RIGHT from (x,y)
// 	      if ((x == DIMENSION) || (worldEnvironment[x+1][y].pit == NO))
// 		{
// 		  possiblePitLocations--;
// 		} else {
// 		pitLocation.X = x+1;
// 		pitLocation.Y = y;
// 	      }
// 	      if ((possiblePitLocations == 1) && (worldEnvironment[pitLocation.X][pitLocation.Y].pit == UNKNOWN))
// 		{
// 		  // Found a new pit
// 		  cout << "Found pit at (" << pitLocation.X << "," << pitLocation.Y << ")\n";
// 		  worldEnvironment[pitLocation.X][pitLocation.Y].pit = YES;
// 		  worldEnvironment[pitLocation.X][pitLocation.Y].safe = NO;
// 		}
// 	    }
// 	}
//     }
// }

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
    } else if (agentStatus.agentHasGold) {
    // If agent has gold, then navigate to (1,1) and climb
    if (agentStatus.agentLocation == location11)
      {
	action = CLIMB;
      } else {
      action = getMove (agentStatus.agentLocation, agentStatus.agentOrientation, location11, agentStatus.agentOrientation);
    }
  } else if (getSafeLocation (goalLocation)) {
    // If no gold, then navigate to safe, unvisited location (if exists)
    action = getMove (agentStatus.agentLocation, agentStatus.agentOrientation, goalLocation, agentStatus.agentOrientation);
  } else if (agentStatus.wumpusAlive && getShootingPosition (goalLocation, goalOrientation)) {
    // If can shoot live wumpus, then navigate to safe location facing wumpus and SHOOT
    if ((agentStatus.agentLocation == goalLocation) && (agentStatus.agentOrientation == goalOrientation))
      {
	action = SHOOT;
      } else {
      action = getMove (agentStatus.agentLocation, agentStatus.agentOrientation, goalLocation, goalOrientation);
    }
  } else if (getRiskyLocation (goalLocation)) {
    // If none of the above works out, then take a risk and navigate to an unknown, unvisited location (if exists)
    worldEnvironment[goalLocation.X][goalLocation.Y].safe = YES; //temporarily, so search can get there.
    action = getMove (agentStatus.agentLocation, agentStatus.agentOrientation, goalLocation, agentStatus.agentOrientation);
    worldEnvironment[goalLocation.X][goalLocation.Y].safe = UNKNOWN;
  } else if (agentStatus.agentLocation == location11) { // nothing above worked, so just give up
    action = CLIMB;
  } else {
    action = getMove (agentStatus.agentLocation, agentStatus.agentOrientation,
		       location11, agentStatus.agentOrientation);
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
	  if ((worldEnvironment[x][y].safe == YES) && (! worldEnvironment[x][y].visited))
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
	  if (worldEnvironment[x][y].safe == YES)
	    {
	      // Check UP
	      if ((y < DIMENSION) && (worldEnvironment[x][y+1].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x;
		  location.Y = y+1;
		}
	      // Check DOWN
	      if ((y > 1) && (worldEnvironment[x][y-1].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x;
		  location.Y = y-1;
		}
	      // Check LEFT
	      if ((x > 1) && (worldEnvironment[x-1][y].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x-1;
		  location.Y = y;
		}
	      // Check RIGHT
	      if ((x < DIMENSION) && (worldEnvironment[x+1][y].safe == UNKNOWN))
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

  if (! (agentStatus.wumpusLocation == Location(0,0)))
    {
      for (x = 1; x <= DIMENSION; x++)
	{
	  for (y = 1; y <= DIMENSION; y++)
	    {
	      if ((! found) && (worldEnvironment[x][y].safe == YES) &&
		  ((x == agentStatus.wumpusLocation.X) || (y == agentStatus.wumpusLocation.Y)))
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
	  if (location.X == agentStatus.wumpusLocation.X)
	    {
	      if (location.Y < agentStatus.wumpusLocation.Y)
		{
		  orientation = UP;
		} else {
		orientation = DOWN;
	      }
	    } else {
	    if (location.X < agentStatus.wumpusLocation.X)
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
  // DEBUG
  cout << "NAVIGATING from (" << startLocation.X << "," << startLocation.Y << ") to ("
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
      if ((x >= 1) && (x <= DIMENSION) && (y >= 1) && (y <= DIMENSION) && (worldEnvironment[x][y].safe == YES))
	{
	  newLocation.X = x;
	  newLocation.Y = y;
	  childState = new stage (newLocation, state->orientation, state->depth + 1, state, FORWARD);
	}
    }
  return childState;
}

int Agent::getDistance (Location& l1, Location& l2)
{
  return (abs (l1.X - l2.X) + abs (l1.Y - l2.Y));
}
