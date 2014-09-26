// Agent.cc

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
  entranceCoordinate.X = 1;
  entranceCoordinate.Y = 1;
  agentCoordinate.X = 1;
  agentCoordinate.Y = 1;
  destinationCoordinate.X = 4;
  destinationCoordinate.Y = 4;
  orientation = 0;
  wumpusDown = false;
  armed = true;
  goal = "secure gold";
}

Action Agent::Process (Percept& percept)
{
  // char c;
  string decision;
  Action action;
  bool validAction = false;

  if (percept.Scream == true)
    wumpusDown = true;
  while (! validAction)
    {
      validAction = true;
      if (percept.Scream == true)
	wumpusDown = true;
      if (agentCoordinate == entranceCoordinate && goal == "evacuate") {
	decision = "climb";
      }
      else if (destinationCoordinate.X > agentCoordinate.X) { // moving right
	switch (orientation) {
	case 0: decision = "forward"; break;
	case 90: decision = "right"; break;
	case 180: decision = "left"; break;
	case 270: decision = "left"; break;
	default: assert(false);
	}
      }
      else if (destinationCoordinate.X < agentCoordinate.X) { // moving left
	switch (orientation) {
	case 0: decision = "left"; break;
	case 90: decision = "left"; break;
	case 180: decision = "forward"; break;
	case 270: decision = "right"; break;
	default: assert(false);
	}
      }
      else if (destinationCoordinate.Y > agentCoordinate.Y) { // moving up
	switch (orientation) {
	case 0: decision = "left"; break;
	case 90: decision = "forward"; break;
	case 180: decision = "right"; break;
	case 270: decision = "left"; break;
	default: assert(false);
	}
      }
      else if (destinationCoordinate.Y < agentCoordinate.Y) { // moving down
	switch (orientation) {
	case 0: decision = "right"; break;
	case 90: decision = "left"; break;
	case 180: decision = "left"; break;
	case 270: decision = "forward"; break;
	default: assert(false);
	}
      }
      else {			// at destination
	if (percept.Glitter == true && goal == "secure gold") {
	  decision = "grab";
	  goal = "evacuate";
	  destinationCoordinate = route.back();
	  route.pop_back();
	}
      }
      if (percept.Stench == true && wumpusDown == false && armed == true) {
	cout << "wumpus detected!" << endl;
	if (			// facing a wall
	    (agentCoordinate.X == 4 && orientation == 0) ||
	    (agentCoordinate.Y == 4 && orientation == 90) ||
	    (agentCoordinate.X == 1 && orientation == 180) ||
	    (agentCoordinate.Y == 1 && orientation == 270)) {
	}			// hold your fire
	else {
	  decision = "shoot";
	}
      }
      cout << "#################################################################" << endl;
      cout << "agent: goal = " << goal
	   << "; destination = " << destinationCoordinate.X << "," << destinationCoordinate.Y
	   << "; position = " << agentCoordinate.X << "," << agentCoordinate.Y
	   << "; orientation = " << orientation
	   << endl;
      cout << "agent: routeSize = " << route.size() << "; route = " << endl;
      for (vector<Location>::iterator i = route.begin(); i != route.end(); i++) {
	previousCoordinate.X = (*i).X;
	previousCoordinate.Y = (*i).Y;
	cout << previousCoordinate.X << "," << previousCoordinate.Y << "; ";
      }
      cout << endl;
      cout << "agent: decision = " << decision << endl;
      cout << "Action?" << endl;;
      // cin >> c;
      // if (c == 'f') {
      if (decision == "forward") {
	action = FORWARD;
	if (goal == "secure gold") {
	  route.push_back(agentCoordinate);
	}
	else if (!route.empty()) {
	  destinationCoordinate.X = (route.back()).X;
	  destinationCoordinate.Y = (route.back()).Y;
	  route.pop_back();
	}
	switch (orientation) {
	case 0: agentCoordinate.X++; break; // moving east
	case 90: agentCoordinate.Y++; break; // moving north
	case 180: agentCoordinate.X--; break; // moving west
	case 270: agentCoordinate.Y--; break; // moving south
	default: assert(false);
	}
      // } else if (c == 'l') {
      } else if (decision == "left") {
	action = TURNLEFT;
	orientation = (orientation+90)%360;
      // } else if (c == 'r') {
      } else if (decision == "right") {
	action = TURNRIGHT;
	orientation = (orientation+360-90)%360;
      // } else if (c == 'g') {
      } else if (decision == "grab") {
	action = GRAB;
      // } else if (c == 's') {
      } else if (decision == "shoot") {
	action = SHOOT;
	armed = false;
      // } else if (c == 'c') {
      } else if (decision == "climb") {
	action = CLIMB;
      } else {
	cout << "Huh?" << endl;
	validAction = false;
      }
    }
  return action;
}

void Agent::GameOver (int score)
{
  cout << "my score is: " << score << endl;
}

