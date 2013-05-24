//
//  board.cpp
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include "board.h"
#include "constants.h"
#include "ai.h"
#include "box.h"

Board::Board()
{
	this->ai = 0;
	this->boxes = new Box*[BOARD_SIZE];
	for (int i=0; i<BOARD_SIZE; i++)
	{
		this->boxes[i] = new Box[BOARD_SIZE];
	}
}

Board::Board(const Board &board)
{
	this->ai = board.ai;
	this->boxes = new Box*[BOARD_SIZE];
	for (int i=0; i<BOARD_SIZE; i++)
	{
		for (int j=0; j<BOARD_SIZE; j++)
		{
			this->boxes[i][j] = board.boxes[i][j];
		}
	}
}

Board::~Board()
{
	for (int i=0; i<BOARD_SIZE; i++)
	{
		delete [] this->boxes[i];
	}
	delete [] this->boxes;
}

Board & Board::operator =(const Board &board)
{
	if (this != &board)
	{
		for (int i=0; i<BOARD_SIZE; i++)
		{
			delete [] this->boxes[i];
		}
		delete [] this->boxes;
		
		this->boxes = new Box*[BOARD_SIZE];
		for (int i=0; i<BOARD_SIZE; i++)
		{
			for (int j=0; j<BOARD_SIZE; j++)
			{
				this->boxes[i][j] = board.boxes[i][j];
			}
		}
	}
	return *this;
}

bool Board::positionIsValid(Position pos) const
{
	return (pos.row >= 0 && pos.column >= 0 && pos.row < BOARD_SIZE && pos.column < BOARD_SIZE);
}

Box & Board::boxAtPosition(Position pos) const
{
	if (!this->positionIsValid(pos))
	{
		printf("error????");
		return this->boxes[0][0];
	}
	
	Box &box = this->boxes[pos.row][pos.column];	
	return box;
}

bool Board::positionIsOccupied(Position position) const
{
	return this->boxAtPosition(position).isOccupied();
}


std::ostream& operator<< (std::ostream &out, Board &board)
{
	out << "  ";
	for (int i=0; i<BOARD_SIZE; i++)
	{
		out << i;
		if (i < (BOARD_SIZE - 1))
		{
			out << ' ';
		}
	}
	out << std::endl;
	for (int i=0; i<BOARD_SIZE; i++)
	{
		out << i << ' ';
		for (int j=0; j<BOARD_SIZE; j++)
		{
			out << board.boxes[i][j].playerSymbol();
			if (j < (BOARD_SIZE - 1))
			{
				out << ' ';
			}
		}
		if (i < (BOARD_SIZE - 1))
		{
			out << std::endl;
		}
	}
	
	return out;
}

bool Board::isCompleted() const
{
	bool allAssigned = true;
	int i = 0;
	while (allAssigned && i<BOARD_SIZE)
	{
		int j=0;
		while (allAssigned && j<BOARD_SIZE)
		{
			allAssigned = this->boxes[i][j].isOccupied();
			++j;
		}
		++i;
	}
	
	return allAssigned;
}

bool Board::isFinished(Player &winner) const
{
	bool finished = false;
	int row = 0;
	while (row < BOARD_SIZE && !finished)
	{
		int column = 0;
		while (column < BOARD_SIZE && !finished)
		{
			Position pos = MakePosition(row, column);
			
			Box &box = this->boxAtPosition(pos);
			if (box.isOccupied())
			{
				const Player &player = box.getPlayer();
				
				int nDirections;
				Direction *directions = this->getDirectionsToCheckForPosition(pos, nDirections);
				int i = 0;
				while (i < nDirections && !finished)
				{
					Direction nextDirection = directions[i];
					
					bool samePlayer = true;
					Position nextPos = MovePosition(pos, nextDirection);
					while (this->positionIsValid(nextPos) && samePlayer)
					{
						Box &box = this->boxAtPosition(nextPos);
						samePlayer = (box.isOccupied() && box.getPlayer() == player);
						nextPos = MovePosition(nextPos, nextDirection);
					}
					
					finished = (samePlayer);
					if (finished)
					{
						winner = player;
					}
					++i;
				}
				
				delete [] directions;
			}
			
			++column;
		}
		++row;
	}
	
	return finished;
}


void Board::setPlayerAtPosition(Player player, Position position)
{
	Box &box = this->boxAtPosition(position);
	
	box.setPlayer(player);
	if (this->ai != NULL)
	{
		this->ai->playerWasSetToPosition(player, position);
	}
}

void Board::registerAI(AI &ai)
{
	this->ai = &ai;
}

std::map<Position, int> Board::weightForBoxesFromPositionInDirectionForPlayer(Position position, Direction direction, Player player)
{
	int weight = 0;
	bool canComplete = true;
	Position nextPosition = position;
	while (canComplete && this->positionIsValid(nextPosition))
	{
		Box &box = this->boxAtPosition(nextPosition);
		if (!box.isOccupied())
		{
			++weight;
		} else
		{
			canComplete = (box.getPlayer() == player);
		}
		nextPosition = MovePosition(nextPosition, direction);
	}
	
	Direction invDirection = InversedDirection(direction);
	Position prevPosition = MovePosition(position, invDirection);
	while (canComplete && this->positionIsValid(prevPosition))
	{
		Box &box = this->boxAtPosition(prevPosition);
		if (!box.isOccupied())
		{
			++weight;
		} else
		{
			canComplete = (box.getPlayer() == player);
		}
		prevPosition = MovePosition(prevPosition, invDirection);
	}
	
	
	std::map<Position, int> weightsByPosition;
	nextPosition = position;
	while (this->positionIsValid(nextPosition))
	{
		Box &box = this->boxAtPosition(nextPosition);
		if (canComplete)
		{
			 if (box.isOccupied() /*&& box.getPlayer() == player*/)
			 {
				 weightsByPosition[nextPosition] = -1;
			 } else
			 {
				 weightsByPosition[nextPosition] = weight;
			 }
		} else
		{
			if (box.isOccupied())
			{
				weightsByPosition[nextPosition] = -2;
			} else
			{
				weightsByPosition[nextPosition] = 0;
			}
		}
		
		nextPosition = MovePosition(nextPosition, direction);
	}
	
	prevPosition = MovePosition(position, invDirection);
	while (this->positionIsValid(prevPosition))
	{
		Box &box = this->boxAtPosition(prevPosition);
		if (canComplete)
		{
			if (box.isOccupied() /*&& box.getPlayer() == player*/)
			{
				weightsByPosition[prevPosition] = -1;
			} else
			{
				weightsByPosition[prevPosition] = weight;
			}
		} else
		{
			if (box.isOccupied())
			{
				weightsByPosition[prevPosition] = -2;
			} else
			{
				weightsByPosition[prevPosition] = 0;
			}
		}
		prevPosition = MovePosition(prevPosition, invDirection);
	}
	
	return weightsByPosition;
}


Direction * Board::getDirectionsToCheckForPosition(const Position &position, int &nPositions) const
{
	Direction *directions;
	if (position.row == 0 && position.column == 0)
	{
//		directions = new Direction[3]{DOWN, RIGHT, DOWN_RIGHT}; //not working with g++???
		directions = new Direction[3];
		directions[0] = DOWN;
		directions[1] = RIGHT;
		directions[2] = DOWN_RIGHT;
		nPositions = 3;
	} else if (position.row == 0 && position.column == (BOARD_SIZE-1))
	{
//		directions = new Direction[2]{DOWN, DOWN_LEFT};
		directions = new Direction[2];
		directions[0] = DOWN;
		directions[1] = DOWN_LEFT;
		nPositions = 2;
	} else if (position.row == 0)
	{
//		directions = new Direction[1]{DOWN};
		directions = new Direction[1];
		directions[0] = DOWN;
		nPositions = 1;
	} else if (position.column == 0)
	{
//		directions = new Direction[1]{RIGHT};
		directions = new Direction[1];
		directions[0] = RIGHT;
		nPositions = 1;
	} else
	{
		directions = 0;
		nPositions = 0;
	}
	return directions;
}
