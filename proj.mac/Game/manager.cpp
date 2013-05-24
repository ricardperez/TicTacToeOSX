//
//  manager.cpp
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include "manager.h"
#include <unistd.h>

Manager::Manager() : ai(HARD, this->board, CROSS)
{
	this->board.registerAI(this->ai);
}

void Manager::startGame()
{
	this->board.registerAI(this->ai);
	int turn = 0;
	Player human = (this->ai.getPlayer() == CROSS ? NOUGHT : CROSS);
	
	std::cout << "You are playing as " << (human == CROSS ? "crosses (X)" : "noughts (O)") << std::endl;
	std::cout << this->board << std::endl << std::endl;
	
	bool completed = false;
	bool finished = false;
	Player winner;
	while (!completed && !finished)
	{
		Player player = ((turn % 2) ? CROSS : NOUGHT);
		
		if (player == human)
		{
			std::cout << "Your turn." << std::endl;
			std::cout << "Enter position ({row} {column}) where {row} and {column} are two numbers from 0 to " << BOARD_SIZE << "." << std::endl;
			int row, column;
			std::cin >> row;
			std::cin >> column;
			
			Position position = MakePosition(row, column);
			while (!this->board.positionIsValid(position) || this->board.positionIsOccupied(position))
			{
				std::cout << "Please, enter a valid position." << std::endl;
				std::cin >> row;
				std::cin >> column;
				position = MakePosition(row, column);
			}
			
			this->board.setPlayerAtPosition(player, position);
		} else
		{
			std::cout << "Computer turn" << std::endl;
			sleep(1);
			Position pos = this->ai.nextMovement();
			std::cout << "Moved at (" << pos.row << ", " << pos.column << ")" << std::endl;
		}
		
		++turn;
		std::cout << this->board << std::endl << std::endl;
		
		completed = this->board.isCompleted();
		finished = this->board.isFinished(winner);
	}
	
	if (finished)
	{
		if (winner == human)
		{
			std::cout << "   ------ YOU WON" << std::endl;
		} else
		{
			std::cout << "   ------ YOU LOST" << std::endl;
		}
	} else
	{
		std::cout << "   ------ DRAW" << std::endl;
	}
}