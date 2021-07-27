

/*
Ingmar Fjolla
Professor Xiaojie Zhang
CSCI 135
Project 3 
the purpose of this project is 
to maximize the performance of the robots
from the base program we were given

*/



#include <cstdlib>
#include <iostream>
#include <cmath>
#include "bot.h"

using namespace std;

const int MAX_ROBOT_NUM = 50;

int NUM;          // to remember number or robots
int ROWS, COLS;   // map dimensions

bool robotIsBroken[MAX_ROBOT_NUM];

/* Initialization procedure, called when the game starts: */
void onStart(int num, int rows, int cols, double mpr,
             Area &area, ostream &log)
{
	NUM = num;   // save the number of robots and the map dimensions
	ROWS = rows;
	COLS = cols;
	
	for(int i = 0; i < MAX_ROBOT_NUM; i++)
	{
		robotIsBroken[i] = false;
	}

	log << "Start!" << endl;
}

bool robotIsAdjacent(int bestRow, int bestCol, Area area) //function to try and make robots not bump eachother
{
	bool adjacentRobotFound = false;
	int adjacentRobotId = 0;
	
	while(adjacentRobotFound == false && adjacentRobotId < NUM)
	{
		if(bestCol == area.locate(adjacentRobotId).c && bestRow == area.locate(adjacentRobotId).r)
		{
			adjacentRobotFound = true;
		}
		
		adjacentRobotId++;
	}
	
	return adjacentRobotFound;//call this function later for the switch case
}

/* Deciding robot's next move */
Action onRobotAction(int id, Loc loc, Area &area, ostream &log) {
	int row = loc.r; // current row and column
	int col = loc.c;

	if (area.inspect(row, col) == DEBRIS)
		return COLLECT;
	else
	{
		int bestCol = -1; // -1 so that the first col and row found are going to be the best
		int bestRow = -1;
	
		for(int scanrow = 0; scanrow < ROWS; scanrow++)
		{
			for(int scancol = 0; scancol < COLS; scancol++)
			{
				if(area.inspect(scanrow,scancol) == DEBRIS)
				{
					if((bestCol < 0 && bestRow < 0) || sqrt(pow(scancol-col,2) + pow(scanrow-row,2)) < sqrt(pow(bestCol-col,2) + pow(bestRow-row,2)))
					{
						bestCol = scancol;
						bestRow = scanrow;
					}			
				}
			}
		}
		
		bool moveToBroken = false;
		int brokenRobotCol;
		int brokenRobotRow;
		int larger;
		if(ROWS > COLS)
		{
			larger = ROWS;
		}
		else
		{
			larger = COLS;
		}
			
		int brokenRobotId = 0;
		for(int i = 0; i < NUM; i++)//iterating throughout each robot 
		{
			brokenRobotCol = area.locate(i).c; //locates broken robots
			brokenRobotRow = area.locate(i).r; //same as above
			
			if(i != id && robotIsBroken[i] && (sqrt(pow(brokenRobotCol-col,2) + pow(brokenRobotRow-row,2)) < (sqrt(pow(bestCol-col,2) + pow(bestRow-row,2))) + larger/2))
			{
				bestCol = brokenRobotCol;
				bestRow = brokenRobotRow;
				moveToBroken = true;
				brokenRobotId = i;
			}
		}
		
		if(moveToBroken == true)
		{
			if(bestCol == col)
			{
				if(bestRow == row + 1)
				{
					robotIsBroken[brokenRobotId] = false;
					return REPAIR_DOWN;
				}
				if(bestRow == row - 1)
				{
					robotIsBroken[brokenRobotId] = false;
					return REPAIR_UP;
				}
			}
			if(bestRow == row)
			{
				if(bestCol == col + 1)
				{
					robotIsBroken[brokenRobotId] = false;
					return REPAIR_RIGHT;
					
				}
				if(bestCol == col - 1)
				{
					robotIsBroken[brokenRobotId] = false;
					return REPAIR_LEFT;
				}
			}
		}
		
		if(bestRow > row) //compares the row of a bot vs row of debris
		{		
			if(robotIsAdjacent(bestRow, bestCol, area) == true)
			{
				switch(rand() % 2) 
				{
					case 0:
						return RIGHT;
					default:
						return LEFT;
				}
			}
			
			return DOWN;
		}
		if(bestRow < row) //compares the row of a bot vs row of debris
		{
			
			if(robotIsAdjacent(bestRow, bestCol, area) == true)
			{
				switch(rand() % 2) 
				{
					case 0:
						return RIGHT;
					default:
						return LEFT;
				}
			}
			return UP;
		}
		if(bestCol < col)//compares the row of a bot vs row of debris
		{
			if(robotIsAdjacent(bestRow, bestCol, area) == true)
			{
				switch(rand() % 2) 
				{
					case 0:
						return UP;
					default:
						return DOWN;
				}
			}
			return LEFT;
		}
		if(bestCol > col)//compares the row of a bot vs row of debris
		{
			
			if(robotIsAdjacent(bestRow, bestCol, area) == true)
			{
				switch(rand() % 2) 
				{
					case 0:
						return UP;
					default:
						return DOWN;
				}
			}
			return RIGHT;
		}
		
		switch(rand() % 4) { //if nothing else above, it just does this (given)
		case 0:
			return LEFT;
		case 1:
			return RIGHT;
		case 2:
			return UP;
		default:
			return DOWN;
		}
	}
}


void onRobotMalfunction(int id, Loc loc, Area &area, ostream &log) {
	log << "Robot " << id << " is damaged." << endl;
	robotIsBroken[id] = true;
}

void onClockTick(int time, ostream &log) {
	if (time % 100 == 0) log << time << " ";
}



