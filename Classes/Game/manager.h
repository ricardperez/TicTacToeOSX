//
//  manager.h
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __TicTacToeSolver__manager__
#define __TicTacToeSolver__manager__

#include <iostream>

#include "board.h"
#include "ai.h"

/**
 * This class instantiates the board and the AI and makes some I/O work.
 * It takes the user input to select the position where he wants to make the
 * following movement and informs him about the AI movements.
 * It also displays a message when the game ends.
 */
class Manager
{
private:
	Board board;
	AI ai;
public:
	Manager();
	
	/**
	 * When called, the game starts with the created board and AI.
	 * It should only be called once.
	 */
	void startGame();
	
};

#endif /* defined(__TicTacToeSolver__manager__) */
