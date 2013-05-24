//
//  ai.cpp
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include "ai.h"
#include <cstdlib>
#include "board.h"
#include <cfloat>
#import <cmath>

AI::AI(AILevel aiLevel, Board &board, Player player)
{
	this->aiLevel = aiLevel;
	this->player = player;
	this->board = &board;
	
	this->boxesWeights = new std::vector<std::map<Direction, int> > *[BOARD_SIZE];
	
	for (int i=0; i<BOARD_SIZE; i++)
	{
		for (int p=0; p<2; p++)
		{
			std::map<Direction, int> playerWeights;
			
			this->boxesWeights[i] = new std::vector<std::map<Direction, int> >[BOARD_SIZE];
			
			for (int j=0; j<BOARD_SIZE; j++)
			{
				std::map<Direction, int> aiPlayerWeights;
				this->boxesWeights[i][j].push_back(aiPlayerWeights);
				
				std::map<Direction, int> humanPlayerWeights;
				this->boxesWeights[i][j].push_back(humanPlayerWeights);
			}
		}
	}
	
	this->recalculateWeights();
}

AI::~AI()
{
	for (int i=0; i<BOARD_SIZE; i++)
	{
		delete [] this->boxesWeights[i];
	}
	delete [] this->boxesWeights;
}

const AI & AI::operator= (const AI &ai)
{
	if (this != &ai)
	{
		this->aiLevel = ai.aiLevel;
		this->player = ai.player;
		this->board = ai.board;
		
		this->boxesWeights = new std::vector<std::map<Direction, int> > *[BOARD_SIZE];
		for (int i=0; i<BOARD_SIZE; i++)
		{
			this->boxesWeights[i] = new std::vector<std::map<Direction, int> >[BOARD_SIZE];
			for (int j=0; j<BOARD_SIZE; j++)
			{
				std::map<Direction, int> aiPlayerWeights;
				aiPlayerWeights.insert(ai.boxesWeights[i][j][0].begin(), ai.boxesWeights[i][j][0].end());
				this->boxesWeights[i][j].push_back(aiPlayerWeights);
				
				std::map<Direction, int> humanPlayerWeights;
				humanPlayerWeights.insert(ai.boxesWeights[i][j][1].begin(), ai.boxesWeights[i][j][1].end());
				this->boxesWeights[i][j].push_back(humanPlayerWeights);
			}
		}
	}
	return *this;
}


const std::vector<std::map<Direction, int> > & AI::weightsAtPosition(const Position &position) const
{
	return this->boxesWeights[position.row][position.column];
}

std::map<Direction, int> & AI::weightsAtPositionForPlayer(const Position &position, const Player &player) const
{
	return this->boxesWeights[position.row][position.column][(player == this->player ? 0 : 1)];
}


void AI::applyWeightForBoxesFromPositionInDirectionAndPlayer(std::map<Position, int> &weightsByPosition, const Direction &direction, const Player &player)
{
	for (std::map<Position, int>::iterator it=weightsByPosition.begin(); it!=weightsByPosition.end(); ++it)
	{
		Position nextPosition = it->first;
		std::map<Direction, int> & weights = this->weightsAtPositionForPlayer(nextPosition, player);
		weights[direction] = it->second;
	}
}

bool AI::getAttackInfoToBoxAtPositionForPlayer(const Position &position, const Player &player, int &currBestWeight, float &currBestMedianWeight) const
{
	std::map<Direction, int> & weights = this->weightsAtPositionForPlayer(position, player);
	
	currBestWeight = INT_MAX;
	currBestMedianWeight = FLT_MAX;
	
	int nSolutions = 0;
	int accumulatedWeight = 0;
	for (std::map<Direction, int>::iterator it=weights.begin(); it!=weights.end(); ++it)
	{
		int weight = it->second;
		
		if (weight > 0) //negative if already occupied; zero if no win is possible using this one
		{
			if (player == this->player)
			{
			
//				Let's add some noise in order to make the AI fail
				switch (this->aiLevel)
				{
					case EASY:
						weight += (rand()%3);
						break;
					case NORMAL:
						weight += (rand()%2);
						break;
					case HARD:
					default:
						break;
				}
			}
			
			if (weight < currBestWeight)
			{
				currBestWeight = weight;
			}
			
			++nSolutions;
			accumulatedWeight += weight;
		}
	}
	if (nSolutions > 0)
	{
		float medianWeight = ((float)accumulatedWeight / nSolutions);
		if (medianWeight < currBestMedianWeight)
		{
			currBestMedianWeight = medianWeight;
		}
	}
	
	return (nSolutions > 0);
}

Position AI::nextMovement()
{
	int aiBestWeight = INT_MAX;
	Position aiBestWeightPosition = MakePosition(0, 0);
	float aiBestMedianWeight = FLT_MAX;
	Position aiBestMedianWeightPosition = MakePosition(0, 0);
	
	int humanBestWeight = INT_MAX;
	Position humanBestWeightPosition;
	float humanBestMedianWeight = FLT_MAX;
	Position humanBestMedianWeightPosition;
	
	Player humanPlayer = (this->player == CROSS ? NOUGHT : CROSS);
	
	for (int i=0; i<BOARD_SIZE; i++)
	{
		for (int j=0; j<BOARD_SIZE; j++)
		{
			Position position = MakePosition(i, j);
			
//			We have to decide if to attack or to defend.
//			Let's see what is the best box to check for the AI and what is its
//			weight but also which will be the best one for the user.
//			We will make one of those 2 movements: the one with the lowest
//			weight.
			
			//ATTACK
			int currAIBestWeight;
			float currAIBestMedianWeight;
			bool aiAnySolution = this->getAttackInfoToBoxAtPositionForPlayer(position, this->player, currAIBestWeight, currAIBestMedianWeight);
			if (aiAnySolution)
			{
				bool change = ((currAIBestWeight < aiBestWeight) || ((currAIBestWeight == aiBestWeight) && rand()%2));
				//let's add some random behaviour to make it harder to predict.
				if (change)
				{
					aiBestWeight = currAIBestWeight;
					aiBestWeightPosition = MakePosition(i, j);
				}
				change = ((currAIBestMedianWeight < aiBestMedianWeight) || ((fabsf(currAIBestMedianWeight - aiBestMedianWeight) < 0.01f) && rand()%2));
				if (change)
				{
					aiBestMedianWeight = currAIBestMedianWeight;
					aiBestMedianWeightPosition = MakePosition(i, j);
				}
			}
			
			//DEFFEND
			int currHumanBestWeight;
			float currHumanBestMedianWeight;
			bool humanAnySolution = this->getAttackInfoToBoxAtPositionForPlayer(position, humanPlayer, currHumanBestWeight, currHumanBestMedianWeight);
			if (humanAnySolution)
			{
				bool change = ((currHumanBestWeight < humanBestWeight) || ((currHumanBestWeight == humanBestWeight) && rand()%2));
				if (change)
				{
					humanBestWeight = currHumanBestWeight;
					humanBestWeightPosition = MakePosition(i, j);
				}
				change = ((currHumanBestMedianWeight < humanBestMedianWeight) || ((fabsf(currHumanBestMedianWeight-humanBestMedianWeight) < 0.0001f) && rand()%2));
				if (change)
				{
					humanBestMedianWeight = currHumanBestMedianWeight;
					humanBestMedianWeightPosition = MakePosition(i, j);
				}
			}
		}
	}
	
	bool shouldTakeMiddlePosition = (!this->board->positionIsOccupied(MakePosition(BOARD_SIZE/2, BOARD_SIZE/2)));
	if (shouldTakeMiddlePosition)
	{
		switch (this->aiLevel)
		{
			case EASY:
				break;
				shouldTakeMiddlePosition = (rand()%3 == 0);
			case NORMAL:
				shouldTakeMiddlePosition = (rand()%2 == 0);
				break;
			default:
				break;
		}
	}
	
	Position pos;
	if (aiBestWeight == 1)
	{
		if (DEBUG)
		{
			std::cout << "  1st --> Attacking at best weight" << std::endl;
		}
		pos = aiBestWeightPosition;
	} else if (humanBestWeight == 1)
	{
		if (DEBUG)
		{
			std::cout << "  2nd --> Defending at best weight" << std::endl;
		}
		pos = humanBestWeightPosition;
	} else if (!this->board->positionIsOccupied(MakePosition(BOARD_SIZE/2, BOARD_SIZE/2)))
	{
		if (DEBUG)
		{
			std::cout << "  3rd --> Taking the middle position" << std::endl;
		}
		pos = MakePosition(BOARD_SIZE/2, BOARD_SIZE/2);
	}
	else if (humanBestMedianWeight < aiBestMedianWeight)
	{
		if (DEBUG)
		{
			std::cout << "  4th --> Defending at best median" << std::endl;
		}
		pos = humanBestMedianWeightPosition;
	} else
	{
		if (DEBUG)
		{
			std::cout << "  5th --> Attacking at best median" << std::endl;
		}
		pos = aiBestMedianWeightPosition;
	}
	this->board->setPlayerAtPosition(this->player, pos);
	return pos;
}

void AI::playerWasSetToPosition(Player player, Position position)
{
	this->recalculateWeights();
}

void AI::recalculateWeights()
{
	Player humanPlayer = (this->player == CROSS ? NOUGHT : CROSS);
	
	for (int i=0; i<BOARD_SIZE; i++)
	{
		for (int j=0; j<BOARD_SIZE; j++)
		{
			Position position = MakePosition(i, j);
			int nDirectionsToCheck;
			Direction *directionsToCheck = this->board->getDirectionsToCheckForPosition(position, nDirectionsToCheck);
						
			int nextDirectionIndex = 0;
			while (nextDirectionIndex < nDirectionsToCheck)
			{
				Direction nextDirection = directionsToCheck[nextDirectionIndex];
				
				std::map<Position, int> aiWeightsInCurrDirection = this->board->weightForBoxesFromPositionInDirectionForPlayer(position, nextDirection, this->player);
				this->applyWeightForBoxesFromPositionInDirectionAndPlayer(aiWeightsInCurrDirection, nextDirection, this->player);
				
				std::map<Position, int> humanWeightInCurrDirection = this->board->weightForBoxesFromPositionInDirectionForPlayer(position, nextDirection, humanPlayer);
				this->applyWeightForBoxesFromPositionInDirectionAndPlayer(humanWeightInCurrDirection, nextDirection, humanPlayer);
				
				++nextDirectionIndex;
			}
			
			delete [] directionsToCheck;
			
		}
	}
	
	if (DEBUG > 1)
	{
		this->debug();
	}
}


void AI::debug()
{
	Player human = (this->player == CROSS ? NOUGHT : CROSS);
	
	std::cout << std::endl << ":::::::DEBUG::::::" << std::endl;
	for (int i=0; i<BOARD_SIZE; i++)
	{
		for (int j=0; j<BOARD_SIZE; j++)
		{
			Position position = MakePosition(i, j);
			std::cout << "Position (" << i << "," << j << "):" << std::endl;
			
			std::cout << "\tAI:" << std::endl;
			std::map<Direction, int> & aiWeights = this->weightsAtPositionForPlayer(position, this->player);
			for (std::map<Direction, int>::iterator it=aiWeights.begin(); it!=aiWeights.end(); ++it)
			{
				std::cout << "\t\t" << DirectionName(it->first) << ": " << it->second << std::endl;
			}
			
			std::cout << "\tHuman:" << std::endl;
			std::map<Direction, int> & humanWeights = this->weightsAtPositionForPlayer(position, human);
			for (std::map<Direction, int>::iterator it=humanWeights.begin(); it!=humanWeights.end(); ++it)
			{
				std::cout << "\t\t" << DirectionName(it->first) << ": " << it->second << std::endl;
			}
		}
	}
	std::cout << std::endl;
}
