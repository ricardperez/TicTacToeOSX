//
//  box.h
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef __TicTacToeSolver__box__
#define __TicTacToeSolver__box__

#include <iostream>
#include "constants.h"

/**
 * This is the class to represent a position from the board. It will let us know
 * if it's occupied and by whom.
 */
class Box
{
private:
	bool occupied;
	Player p;
	
public:
	
	Box();
	Box(const Box &box);
	const Box & operator= (Box &box);
	
	bool isOccupied() const;
	void setPlayer(Player p);
	const Player & getPlayer() const;
	char playerSymbol() const;
	
};

#endif /* defined(__TicTacToeSolver__box__) */
