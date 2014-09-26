#ifndef AGENT_H
#define AGENT_H
#define DIMENSION 4
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include "Action.h"
#include "Location.h"
#include "Orientation.h"
#include "Percept.h"
#include "WorldState.h"

enum environmentFeature {YES, NO, UNKNOWN};
class roomEnvironment
{
 public:
  bool visited;
  environmentFeature wumpus;
  environmentFeature pit;
  environmentFeature safe;
  environmentFeature stench;
  environmentFeature breeze;
};


class SearchState
{
 public:
  SearchState (Location& loc, Orientation& ori, int dep, SearchState* par, Action act)
    {
      location.X = loc.X;
      location.Y = loc.Y;
      orientation = ori;
      depth = dep;
      parent = par;
      action = act;
    }

  Location location;
  Orientation orientation;
  int depth;
  SearchState* parent;
  Action action;
};

class Agent
{
 public:
  Agent ();
  ~Agent ();
  void Initialize ();
  Action Process (Percept& percept);
  void GameOver (int score);

  void UpdateState (Percept& percept);
  void LookForWumpus ();
  void LookForPits ();
  Action ChooseAction (Percept& percept);
  bool FindSafeUnvisitedLocation (Location& location);
  bool FindUnknownUnvisitedLocation (Location& location);
  bool FindSafeLocationFacingWumpus (Location& location, Orientation& orientation);
  Action FirstAction (SearchState* state);
  Action Navigate (Location& startLocation, Orientation& startOrientation, Location& goalLocation, Orientation& goalOrientation);
  SearchState* IterativeDeepeningSearch (SearchState* initialState, SearchState* goalState);
  SearchState* DepthLimitedSearch (SearchState* initialState, SearchState* goalState, int depthLimit);
  bool GoalTest (SearchState* state, SearchState* goalState);
  SearchState* GetChildState (SearchState* state, Action action);
  int CityBlockDistance (Location& location1, Location& location2);

  Action lastAction;
  WorldState agentState;
  roomEnvironment agentWorld[DIMENSION+1][DIMENSION+1];
  int worldSize;
};

#endif
