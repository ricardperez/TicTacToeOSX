//
//  constants.h
//  TicTacToeSolver
//
//  Created by Ricard Pérez del Campo on 23/05/13.
//  Copyright (c) 2013 Ricard Pérez del Campo. All rights reserved.
//

#ifndef TicTacToeSolver_constants_h
#define TicTacToeSolver_constants_h


#define DEBUG 1

typedef enum {NOUGHT, CROSS} Player;
const int BOARD_SIZE = 3;


typedef enum {UP=0, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT} Direction;
typedef enum { EASY, NORMAL, HARD } AILevel;

typedef struct
{
	int row;
	int column;
} Position;

inline Position MakePosition(int row, int column)
{
	Position pos;
	pos.row = row;
	pos.column = column;
	return pos;
}

/**
 * In order to make it possible to create a map where keys are Position values.
 */
inline bool operator<(const Position & first, const Position & second)
{
	return ((first.row < second.row) || ((first.row == second.row) && (first.column < second.column)));
}

inline Direction InversedDirection(Direction dir)
{
	Direction inv;
	switch (dir)
	{
		case UP:
			inv = DOWN;
			break;
		case DOWN:
			inv = UP;
			break;
		case LEFT:
			inv = RIGHT;
			break;
		case RIGHT:
			inv = LEFT;
			break;
		case UP_LEFT:
			inv = DOWN_RIGHT;
			break;
		case DOWN_RIGHT:
			inv = UP_LEFT;
			break;
		case UP_RIGHT:
			inv = DOWN_LEFT;
			break;
		case DOWN_LEFT:
			inv = UP_RIGHT;
			break;
		default: //??
			inv = UP;
			break;
	}
	return inv;
}

inline const std::string DirectionName(Direction dir)
{
	std::string name;
	switch (dir)
	{
		case UP:
			name = "Up";
			break;
		case DOWN:
			name = "Down";
			break;
		case LEFT:
			name = "Left";
			break;
		case RIGHT:
			name = "Right";
			break;
		case UP_LEFT:
			name = "Up-Left";
			break;
		case DOWN_RIGHT:
			name = "Down-Right";
			break;
		case UP_RIGHT:
			name = "Up-Right";
			break;
		case DOWN_LEFT:
			name = "Down-Left";
			break;
		default: //??
			break;
	}
	return name;
}

inline Position MovePosition(Position pos, Direction dir)
{
	int row = pos.row;
	int column = pos.column;
	
	switch (dir)
	{
		case UP:
			--row;
			break;
		case DOWN:
			++row;
			break;
		case LEFT:
			--column;
			break;
		case RIGHT:
			++column;
			break;
		case UP_LEFT:
			--row;
			--column;
			break;
		case DOWN_RIGHT:
			++row;
			++column;
			break;
		case UP_RIGHT:
			--row;
			++column;
			break;
		case DOWN_LEFT:
			++row;
			--column;
			break;
		default: //??
			break;
	}
	
	return MakePosition(row,column);
}


#endif
