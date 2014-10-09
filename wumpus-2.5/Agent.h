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

enum feature {YES, NO, UNKNOWN};
class room
{
 public:
  bool visited;
  feature wumpus;
  feature pit;
  feature safe;
  feature stench;
  feature breeze;
};


class stage
{
 public:
  stage (Location& l1, Orientation& o1, int d1, stage* p1, Action a1)
    {
      depth = d1;
      parent = p1;
      action = a1;
      location.X = l1.X;
      location.Y = l1.Y;
      orientation = o1;
    }
  int depth;
  stage* parent;
  Action action;
  Location location;
  Orientation orientation;
};

class Agent
{
 public:
  Agent ();
  ~Agent ();
  void Initialize ();
  void GameOver (int score);
  void updateCoordinate (Percept&percept);
  void updateKnowledgeBase (Percept& percept);
  void locateWumpus ();
  void updatePitMap ();
  Action getNextAction (Percept& percept);
  Action getFirstMove (stage* state);
  Action getMove (Location& startLocation, Orientation& startOrientation, Location& goalLocation, Orientation& goalOrientation);
  stage* iterativeDeepeningSearch (stage* initialState, stage* goalState);
  stage* depthLimitedSearch (stage* initialState, stage* goalState, int depthLimit);
  stage* getChildState (stage* state, Action action);
  int getDistance (Location& l1, Location& l2);
  bool getSafeLocation (Location& location);
  bool getRiskyLocation (Location& location);
  bool getShootingPosition (Location& location, Orientation& orientation);
  bool goalTest (stage* state, stage* goalState);
  Action Process (Percept& percept);
 private:
  Action previousAction;
  Location agentLocation;
  Orientation agentOrientation;
  bool agentHasArrow;
  bool agentHasGold;
  Location wumpusLocation;
  bool wumpusAlive;
  bool wumpusLocated;
  room site[DIMENSION+2][DIMENSION+2];
};

#endif
