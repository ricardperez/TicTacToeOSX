//
//  ai.h
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __TicTacToeSolver__ai__
#define __TicTacToeSolver__ai__

#include <iostream>
#include <map>
#include <vector>

#include "constants.h"

class Board;

/**
 * This is the responsible to handle the computer AI.
 * It's configurable by 3 modes: easy, normal or hard. The used logic is the 
 * same, but the easier it is, the more noise it adds to the calculations so the
 * decision made may not be the best one.
 * 
 * To use this class, it's required to call the registerAI() function from the 
 * board instance (given by parameter at constructing).
 */
class AI
{
	
private:
	/**
	 * One from the enumeration {EASY, NORMAL, HARD}
	 */
	AILevel aiLevel;
	
	/**
	 * The board used to play. It will be required that someone calls the 
	 * registerAI() to it in order to make it notify this when a movement is
	 * done.
	 */
	Board *board;
	
	/**
	 * The player the AI plays as (CROSS or NOUGHT).
	 */
	Player player;
	
	/**
	 * There is one instance of this structure per each board box.
	 * The outer vector has 2 elements. The first one will contain info for the
	 * AI user, while the second will have info for the other user.
	 * Each of those maps will let us know the most potential box for the next
	 * movement. There will be an integer value related with a direction that
	 * will mean the number of positions that would be required to win the game
	 * if that box was used in the current turn.
	 * If a position is already taken, will have a -1 value if it's a AI
	 * position, or -2 if it's a human's. If the position is not a potential
	 * winning position (because a neighbor is already taken by the "enemy", the
	 * value will be 0.
	 */
	std::vector<std::map<Direction, int> > **boxesWeights;
	
	/**
	 * This function will return the boxesWeights element related to the given
	 * position.
	 */
	const std::vector<std::map<Direction, int> > & weightsAtPosition(const Position &position) const;
	
	/**
	 * This function will return the map<Direction, int> from the boxesWeights
	 * related to the given position and player. The AI player has to access the
	 * first element from the vector, while the human one has to get the second
	 * one.
	 */
	std::map<Direction, int> & weightsAtPositionForPlayer(const Position &position, const Player &player) const;
	
	/**
	 * This function is called after instantiating the class and every time a
	 * movement is done.
	 * It updates the boxesWeights attribute in order to make the next
	 * nextMovement() call to be perfect.
	 */
	void recalculateWeights();
	
	/**
	 * Will update those instances from the boxesWeights that correspond with
	 * the given positions at weightsByPosition parameter, setting the value for
	 * the given direction to the appropiate integer value of the map.
	 */
	void applyWeightForBoxesFromPositionInDirectionAndPlayer(std::map<Position, int> &weightsByPosition, const Direction &direction, const Player &player);
	
	/**
	 * This function is used to get information about making an attack to a
	 * given position.
	 * Will return true if that position is ok to be attacked (that is that it's
	 * not occupied and it may be part of a solution) or false otherwise.
	 * If true is returned, currBestWeight value will be assigned to the minimum
	 * number of positions that would be needed to be "conquerred" in order to
	 * win using the 'position' one. It's about a minimum, because there may be
	 * multiple solutions using that position because of taking different
	 * directions.
	 * If true is returned, currBestMedianWeight is set to the median of all the
	 * possible weights.
	 */
	bool getAttackInfoToBoxAtPositionForPlayer(const Position &position, const Player &player, int &currBestWeight, float &currBestMedianWeight) const;
	
	/**
	 * Will display to the std::cout some useful information about the weights
	 * at this point of execution.
	 * Will show the information for both AI user and human user.
	 *
	 * NOTE: should only be called if the macro DEBUG is set to >1 because a lot
	 * of text may be displayed.
	 */
	void debug();
	
public:
	
	/**
	 * Constructor with params.
	 * aiLevel is one of {EASY, NORMAL, HARD}. HARD will not add noise to the
	 * movements, so the best solution will always be taken. Others will.
	 * board is the board were the game will be played.
	 * player is the player to be controlled by the AI.
	 * It's required that after instantiating an AI object, the registerAI() 
	 * method is called in the same board instance given here.
	 */
	AI(AILevel aiLevel, Board &board, Player player);
	
	/**
	 * Will destroy the dynamic 2-dimensional array boxesWeights
	 */
	~AI();
	
	/**
	 * This function will make the AI movement.
	 * It will return the position occupied by that movement.
	 * Will invoke setPlayerAtPosition() from the related board with the current
	 * player and the found position.
	 *
	 * The way to found the best position follows these rules:
	 *  - If AI player is only one turn to win, that position will be chosen.
	 *  - If human player is only one turn to win, that position will be chosen.
	 *  - If the middle position is free, it will be chosen.
	 *  - If the minimum median weight's position for human player is less than
	 * the ai's, then that position will be chosen.
	 *  - Otherwise, the minimum median weight's position for the AI will be
	 * chosen.
	 *
	 * The median weight of a position is the median of the different weights of
	 * a position.
	 * A weight is the required positions to fill in a given direction and
	 * position in order to win the game.
	 */
	Position nextMovement();
	
	/**
	 * This function should be called from the board when a movement is done in
	 * order to be able to recalculate the weights to make the "perfect" move in
	 * the next turn.
	 */
	void playerWasSetToPosition(Player player, Position position);
	
	/**
	 * inline function to get the player set by the constructor.
	 */
	const Player & getPlayer() const {
		return this->player;
	};
	
	/**
	 * Assignment operator. Creates a new dynamic array if necessary 
	 * (after releasing the current one).
	 */
	const AI & operator= (const AI &ai);
};

#endif /* defined(__TicTacToeSolver__ai__) */
