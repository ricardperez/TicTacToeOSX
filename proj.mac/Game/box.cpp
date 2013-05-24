//
//  box.cpp
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#include "box.h"

Box::Box()
{
	this->occupied = false;
}

Box::Box(const Box &box)
{
	this->occupied = box.occupied;
	this->p = box.p;
}

const Box & Box::operator= (Box &box)
{
	if (&box != this)
	{
		this->occupied = box.occupied;
		this->p = box.p;
	}
	
	return *this;
}

bool Box::isOccupied() const
{
	return this->occupied;
}

void Box::setPlayer(Player p)
{
	this->p = p;
	this->occupied = true;
}

const Player & Box::getPlayer() const
{
	return this->p;
}

char Box::playerSymbol() const
{
	if (this->occupied)
	{
		return (this->p == CROSS ? 'X' : 'O');
	} else
	{
		return '-';
	}
}