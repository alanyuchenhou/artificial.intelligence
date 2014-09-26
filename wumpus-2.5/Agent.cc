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
  int x, y;
  agentState.agentLocation = Location(1,1);
  agentState.agentOrientation = RIGHT;
  agentState.agentHasGold = false;
  agentState.wumpusAlive = true;
  agentState.wumpusLocation = Location(0,0); //unknown
  lastAction = SHOOT; // dummy action to start
  for (x = 1; x <= MAX_WORLD_SIZE; x++)
    {
      for (y = 1; y <= MAX_WORLD_SIZE; y++)
    	{
	  agentWorld[x][y].visited = false;
	  agentWorld[x][y].safe = UNKNOWN;
	  agentWorld[x][y].stench = UNKNOWN;
	  agentWorld[x][y].breeze = UNKNOWN;
	  agentWorld[x][y].wumpus = UNKNOWN;
	  agentWorld[x][y].pit = UNKNOWN;
    	}
    }
  agentWorld[1][1].visited = true;
  agentWorld[1][1].safe = YES;
  agentWorld[1][1].wumpus = NO;
  agentWorld[1][1].pit = NO;
  worldSize = MAX_WORLD_SIZE;
}

Action Agent::Process (Percept& percept)
{
  UpdateState (percept);
  lastAction = ChooseAction (percept);

  return lastAction;
}

void Agent::GameOver (int score)
{
  cout << "my score is: " << score << endl;
}

void Agent::UpdateState (Percept& percept)
{
  int x, y;

  x = agentState.agentLocation.X;
  y = agentState.agentLocation.Y;

  // Update agent state based on last action and given percept
  if ((lastAction == FORWARD) && (! percept.Bump))
    {
      if (agentState.agentOrientation == RIGHT)
	x++;
      else if (agentState.agentOrientation == LEFT)
	x--;
      else if (agentState.agentOrientation == UP)
	y++;
      else if (agentState.agentOrientation == DOWN)
	y--;
      agentState.agentLocation = Location(x,y);
    }
  if (lastAction == TURNLEFT)
    {
      if (agentState.agentOrientation == RIGHT)
	agentState.agentOrientation = UP;
      else if (agentState.agentOrientation == LEFT)
	agentState.agentOrientation = DOWN;
      else if (agentState.agentOrientation == UP)
	agentState.agentOrientation = LEFT;
      else if (agentState.agentOrientation == DOWN)
	agentState.agentOrientation = RIGHT;
    }
  if (lastAction == TURNRIGHT)
    {
      if (agentState.agentOrientation == RIGHT)
	agentState.agentOrientation = DOWN;
      else if (agentState.agentOrientation == LEFT)
	agentState.agentOrientation = UP;
      else if (agentState.agentOrientation == UP)
	agentState.agentOrientation = RIGHT;
      else if (agentState.agentOrientation == DOWN)
	agentState.agentOrientation = LEFT;
    }
  // Update pit/wumpus information (hey, I'm alive)
  agentWorld[x][y].pit = NO;
  if (agentState.wumpusAlive)
    agentWorld[x][y].wumpus = NO;

  // Update stench info
  if (percept.Stench)
    agentWorld[x][y].stench = YES;
  else agentWorld[x][y].stench = NO;

  // Update breeze info
  if (percept.Breeze)
    agentWorld[x][y].breeze = YES;
  else agentWorld[x][y].breeze = NO;

  // Update wumpus health
  if (percept.Scream)
    agentState.wumpusAlive = false;

  // Update agent's gold status (we only do a grab if saw glitter)
  if (lastAction == GRAB)
    agentState.agentHasGold = true;

  // Update safe and visited information about the world
  agentWorld[x][y].visited = true;
  agentWorld[x][y].safe = YES;
  if (((! agentState.wumpusAlive) || (! percept.Stench)) && (! percept.Breeze))
    {
      // locations adjacent to (x,y) are safe
      if (x > 1)
	agentWorld[x-1][y].safe = YES;
      if (x < worldSize)
	agentWorld[x+1][y].safe = YES;
      if (y > 1)
	agentWorld[x][y-1].safe = YES;
      if (y < worldSize)
	agentWorld[x][y+1].safe = YES;
    }
  // Check if we can find the elusive wumpus
  if (agentState.wumpusAlive && (agentState.wumpusLocation == Location(0,0)))
    LookForWumpus();

  // Check if we can find a pit
  LookForPits();
}

// Check if we can pinpoint the wumpus's location.
// For each stench, see if only one adjacent location where wumpus != NO.
void Agent::LookForWumpus ()
{
  int x, y;
  int possibleWumpusLocations;
  Location wumpusLocation;

  for (x = 1; x <= worldSize; x++)
    {
      for (y = 1; y <= worldSize; y++)
	{
	  if (agentWorld[x][y].stench == YES)
	    {
	      possibleWumpusLocations = 4;
	      // Check UP from (x,y)
	      if ((y == worldSize) || (agentWorld[x][y+1].wumpus == NO))
		{
		  possibleWumpusLocations--;
		} else {
		wumpusLocation.X = x;
		wumpusLocation.Y = y+1;
	      }
	      // Check DOWN from (x,y)
	      if ((y == 1) || (agentWorld[x][y-1].wumpus == NO))
		{
		  possibleWumpusLocations--;
		} else {
		wumpusLocation.X = x;
		wumpusLocation.Y = y-1;
	      }
	      // Check LEFT from (x,y)
	      if ((x == 1) || (agentWorld[x-1][y].wumpus == NO))
		{
		  possibleWumpusLocations--;
		} else {
		wumpusLocation.X = x-1;
		wumpusLocation.Y = y;
	      }
	      // Check RIGHT from (x,y)
	      if ((x == worldSize) || (agentWorld[x+1][y].wumpus == NO))
		{
		  possibleWumpusLocations--;
		} else {
		wumpusLocation.X = x+1;
		wumpusLocation.Y = y;
	      }
	      if (possibleWumpusLocations == 1)
		{
		  // Found it
		  cout << "Found wumpus at (" << wumpusLocation.X << "," << wumpusLocation.Y << ")\n";
		  agentState.wumpusLocation.X = wumpusLocation.X;
		  agentState.wumpusLocation.Y = wumpusLocation.Y;
		  agentWorld[wumpusLocation.X][wumpusLocation.Y].wumpus = YES;
		  // ***** TODO: Might want to set all other locations to wumpus=NO
		  return; // only one wumpus
		}
	    }
	}
    }
}

// Check if we can pinpoint any pit locations.
// For each breeze, see if only one adjacent location where pit != NO.
void Agent::LookForPits ()
{
  int x, y;
  int possiblePitLocations;
  Location pitLocation;

  for (x = 1; x <= worldSize; x++)
    {
      for (y = 1; y <= worldSize; y++)
	{
	  if (agentWorld[x][y].breeze == YES)
	    {
	      possiblePitLocations = 4;
	      // Check UP from (x,y)
	      if ((y == worldSize) || (agentWorld[x][y+1].pit == NO))
		{
		  possiblePitLocations--;
		} else {
		pitLocation.X = x;
		pitLocation.Y = y+1;
	      }
	      // Check DOWN from (x,y)
	      if ((y == 1) || (agentWorld[x][y-1].pit == NO))
		{
		  possiblePitLocations--;
		} else {
		pitLocation.X = x;
		pitLocation.Y = y-1;
	      }
	      // Check LEFT from (x,y)
	      if ((x == 1) || (agentWorld[x-1][y].pit == NO))
		{
		  possiblePitLocations--;
		} else {
		pitLocation.X = x-1;
		pitLocation.Y = y;
	      }
	      // Check RIGHT from (x,y)
	      if ((x == worldSize) || (agentWorld[x+1][y].pit == NO))
		{
		  possiblePitLocations--;
		} else {
		pitLocation.X = x+1;
		pitLocation.Y = y;
	      }
	      if ((possiblePitLocations == 1) && (agentWorld[pitLocation.X][pitLocation.Y].pit == UNKNOWN))
		{
		  // Found a new pit
		  cout << "Found pit at (" << pitLocation.X << "," << pitLocation.Y << ")\n";
		  agentWorld[pitLocation.X][pitLocation.Y].pit = YES;
		  agentWorld[pitLocation.X][pitLocation.Y].safe = NO;
		}
	    }
	}
    }
}

Action Agent::ChooseAction (Percept& percept)
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
    } else if (agentState.agentHasGold) {
    // If agent has gold, then navigate to (1,1) and climb
    if (agentState.agentLocation == location11)
      {
	action = CLIMB;
      } else {
      action = Navigate (agentState.agentLocation, agentState.agentOrientation, location11, agentState.agentOrientation);
    }
  } else if (FindSafeUnvisitedLocation (goalLocation)) {
    // If no gold, then navigate to safe, unvisited location (if exists)
    action = Navigate (agentState.agentLocation, agentState.agentOrientation, goalLocation, agentState.agentOrientation);
  } else if (agentState.wumpusAlive && FindSafeLocationFacingWumpus (goalLocation, goalOrientation)) {
    // If can shoot live wumpus, then navigate to safe location facing wumpus and SHOOT
    if ((agentState.agentLocation == goalLocation) && (agentState.agentOrientation == goalOrientation))
      {
	action = SHOOT;
      } else {
      action = Navigate (agentState.agentLocation, agentState.agentOrientation, goalLocation, goalOrientation);
    }
  } else if (FindUnknownUnvisitedLocation (goalLocation)) {
    // If none of the above works out, then take a risk and navigate to an unknown, unvisited location (if exists)
    agentWorld[goalLocation.X][goalLocation.Y].safe = YES; //temporarily, so search can get there.
    action = Navigate (agentState.agentLocation, agentState.agentOrientation, goalLocation, agentState.agentOrientation);
    agentWorld[goalLocation.X][goalLocation.Y].safe = UNKNOWN;
  } else if (agentState.agentLocation == location11) { // nothing above worked, so just give up
    action = CLIMB;
  } else {
    action = Navigate (agentState.agentLocation, agentState.agentOrientation,
		       location11, agentState.agentOrientation);
  }
  return action;
}

// Returns true if safe, unvisited location exists, and stores it in given location.
// Otherwise, returns false;
bool Agent::FindSafeUnvisitedLocation (Location& location)
{
  int x, y;
  bool found = false;

  for (x = 1; x <= worldSize; x++)
    {
      for (y = 1; y <= worldSize; y++)
	{
	  if ((agentWorld[x][y].safe == YES) && (! agentWorld[x][y].visited))
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
bool Agent::FindUnknownUnvisitedLocation (Location& location)
{
  int x, y;
  bool found = false;

  for (x = 1; x <= worldSize; x++)
    {
      for (y = 1; y <= worldSize; y++)
	{
	  if (agentWorld[x][y].safe == YES)
	    {
	      // Check UP
	      if ((y < worldSize) && (agentWorld[x][y+1].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x;
		  location.Y = y+1;
		}
	      // Check DOWN
	      if ((y > 1) && (agentWorld[x][y-1].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x;
		  location.Y = y-1;
		}
	      // Check LEFT
	      if ((x > 1) && (agentWorld[x-1][y].safe == UNKNOWN))
		{
		  found = true;
		  location.X = x-1;
		  location.Y = y;
		}
	      // Check RIGHT
	      if ((x < worldSize) && (agentWorld[x+1][y].safe == UNKNOWN))
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
bool Agent::FindSafeLocationFacingWumpus (Location& location, Orientation& orientation)
{
  int x, y;
  bool found = false;

  if (! (agentState.wumpusLocation == Location(0,0)))
    {
      for (x = 1; x <= worldSize; x++)
	{
	  for (y = 1; y <= worldSize; y++)
	    {
	      if ((! found) && (agentWorld[x][y].safe == YES) &&
		  ((x == agentState.wumpusLocation.X) || (y == agentState.wumpusLocation.Y)))
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
	  if (location.X == agentState.wumpusLocation.X)
	    {
	      if (location.Y < agentState.wumpusLocation.Y)
		{
		  orientation = UP;
		} else {
		orientation = DOWN;
	      }
	    } else {
	    if (location.X < agentState.wumpusLocation.X)
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
Action Agent::FirstAction (SearchState* state)
{
  Action action = CLIMB; // dummy action
  while (state->parent != NULL)
    {
      if (state->parent->parent == NULL)
	{
	  action = state->action;
	}
      state = state->parent;
    }
  return action;
}

Action Agent::Navigate (Location& startLocation, Orientation& startOrientation,
			Location& goalLocation, Orientation& goalOrientation)
{
  // DEBUG
  cout << "NAVIGATING from (" << startLocation.X << "," << startLocation.Y << ") to ("
       << goalLocation.X << "," << goalLocation.Y << ")\n";

  SearchState* initialState = new SearchState (startLocation, startOrientation, 0, NULL, CLIMB);
  SearchState* goalState = new SearchState (goalLocation, goalOrientation, 0, NULL, CLIMB);
  SearchState* finalState = IterativeDeepeningSearch (initialState, goalState);
  SearchState* tmpState;
  Action action = FirstAction (finalState);
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

SearchState* Agent::IterativeDeepeningSearch (SearchState* initialState, SearchState* goalState)
{
  // Solution depth must be at least the city block distance between the initial and goal locations
  int depthLimit = CityBlockDistance (initialState->location, goalState->location);
  SearchState* finalState = NULL;

  while (finalState == NULL)
    {
      finalState = DepthLimitedSearch (initialState, goalState, depthLimit);
      depthLimit++;
    }
  return finalState;
}

SearchState* Agent::DepthLimitedSearch (SearchState* state, SearchState* goalState, int depthLimit)
{
  SearchState* childState;
  SearchState* resultState;

  if (GoalTest (state, goalState))
    {
      return state;
    }
  if (state->depth < depthLimit)
    {
      childState = GetChildState (state, FORWARD);
      if (childState != NULL)
	{
	  resultState = DepthLimitedSearch (childState, goalState, depthLimit);
	  if (resultState != NULL) return resultState;
	  else delete childState;
	}
      childState = GetChildState (state, TURNLEFT);
      resultState = DepthLimitedSearch (childState, goalState, depthLimit);
      if (resultState != NULL) return resultState;
      else delete childState;
      childState = GetChildState (state, TURNRIGHT);
      resultState = DepthLimitedSearch (childState, goalState, depthLimit);
      if (resultState != NULL) return resultState;
      else delete childState;
    }
  return NULL;
}

// True if state location and orientation same as goal location and orientation.
bool Agent::GoalTest (SearchState* state, SearchState* goalState)
{
  if ((state->location.X == goalState->location.X) &&
      (state->location.Y == goalState->location.Y) &&
      (state->orientation == goalState->orientation))
    return true;
  else return false;
}

// Returns new state after applying action to given state. For FORWARD, only
// works if adjacent location exists and is safe. If not, return NULL.
SearchState* Agent::GetChildState (SearchState* state, Action action)
{
  int x, y;
  SearchState* childState = NULL;
  Location newLocation;

  if (action == TURNLEFT)
    {
      childState = new SearchState (state->location, state->orientation, state->depth + 1, state, TURNLEFT);
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
      childState = new SearchState (state->location, state->orientation, state->depth + 1, state, TURNRIGHT);
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
      if ((x >= 1) && (x <= worldSize) && (y >= 1) && (y <= worldSize) && (agentWorld[x][y].safe == YES))
	{
	  newLocation.X = x;
	  newLocation.Y = y;
	  childState = new SearchState (newLocation, state->orientation, state->depth + 1, state, FORWARD);
	}
    }
  return childState;
}

int Agent::CityBlockDistance (Location& location1, Location& location2)
{
  return (abs (location1.X - location2.X) + abs (location1.Y - location2.Y));
}
