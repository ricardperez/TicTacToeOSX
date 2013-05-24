//
//  board.h
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __TicTacToeSolver__board__
#define __TicTacToeSolver__board__

#include <iostream>
#include "constants.h"
#import <map>

class AI;
class Box;

/**
 * The representation of the board.
 * It will contain a 2-dimensional array of boxes.
 * It's interface will let make movements by setting a position to a given user,
 * ask if a position is occupied, ask if the game is finished or completed and
 * register an AI in order to be notified when changes happen.
 * It also implements the << operator in order to be displayed in a nice way.
 */
class Board
{
	
private:
	
	Box **boxes;
	AI *ai;
	
	Box & boxAtPosition(Position pos) const;
public:
	
	Board();
	Board(const Board &board);
	~Board();
	
	Board & operator =(const Board &board);
	
	/**
	 * Will return true is the row and column are both in these limits:
	 *  [0, BOARD_SIZE)
	 */
	bool positionIsValid(Position pos) const;
	
	friend std::ostream& operator<< (std::ostream &out, Board &board);
	
	/**
	 * Returns true only if all the boxes have a player assigned.
	 */
	bool isCompleted() const;
	
	/**
	 * Returns true if any player has won. If true, then it assigns the winner
	 * player to winner.
	 */
	bool isFinished(Player &winner) const;
	
	/**
	 * Checks if any player has already taken that position.
	 */
	bool positionIsOccupied(Position position) const;
	
	/**
	 * Occupies a position by a player. The position must not be occupied yet.
	 */
	void setPlayerAtPosition(Player player, Position position);
	
	/**
	 * Registers an ai instance in order to be notified when a movement is done.
	 * The given ai instance should have been instantiated giving this
	 * particular board at constructing.
	 */
	void registerAI(AI &ai);
	
	/**
	 * If the box is already occupied will return -1 if its player is p or -2 if
	 * it's the other one.
	 * If it's not occupied, then it will return an integer that corresponds to 
	 * the required boxes to occupy in the given direction in order to win this 
	 * game by that user. If no solution is possible, it will return 0.
	 *
	 * NOTE: all the boxes in the same direction will return the same value if
	 * they are not occupied, so it may be wasteful to invoke this function for 
	 * all those boxes.
	 * 
	 * The return value will contain the weight for each box accessible from 
	 * position and in direction direction (and reversed).
	 */
	std::map<Position, int> weightForBoxesFromPositionInDirectionForPlayer(Position position, Direction direction, Player player);
	
	/**
	 * Will return an array of directions to be iterated from position in order
	 * to get all the boxes to look for the solution.
	 * It will save redundant checks. If we have already checked first column
	 * while inspecting box {0,0}, it will not be necessary to check it again
	 * when inspecting box {1,0}.
	 * The result will be as follows:
	 *   HORIZONTAL, VERTICAL, DOWN-RIGHT for box {0,0}
	 *   VERTICAL, DOWN-LEFT for box {0, n}
	 *   VERTICAL for boxes {0, x} where x > 0 and x < n
	 *   HORIZONTAL for boxes {y, 0} where y > 0
	 *   none for other cases
	 * The nPositions variable will be set to the correct number of elements.
	 *
	 * The caller will be responsible to deallocate this array.
	 */
	Direction * getDirectionsToCheckForPosition(const Position &position, int &nPositions) const;
};

#endif /* defined(__TicTacToeSolver__board__) */
