/*************************************\
 *  Project: Project 2 - Five-in-a-Row
 *
 *  Coder: Chris Polansky
 *  Contact: chris.polansky@tamu.edu
 *
 *  Editor: VIM - Vi IMproved 7.2
 *  Compiler: g++-4.6 4.6.3
 *
 *  License: Proprietary
 *
\*************************************/

#include <algorithm>
#include "game.h"

using namespace FIAR;

enum direction {
	ALL, UP, DOWN, LEFT, RIGHT, UPRIGHT, UPLEFT, DOWNRIGHT, DOWNLEFT;
}

int Game::calcStatus(const int& r, const int& c, direction dir) const {
	if(!checkBounds(r,c)) return 0;

	COLOR center 		= board(r,c);
	COLOR top 			= board(r-1,c);
	COLOR bottom 		= board(r+1,c);
	COLOR left 			= board(r,c-1);
	COLOR right 		= board(r,c+1);
	COLOR top_left	= board(r-1,c-1);
	COLOR top_right	= board(r-1,c+1);
	COLOR bot_left	= board(r+1,c-1);
	COLOR bot_right = board(r+1,c+1);

	// positve and negative are the diagonals
	int up_down, left_right, positive, negative;
	up_down = 0; left_right = 0; positive = 0; negative = 0;
	if( (dir==ALL||dir==UP) && top==center) up_down += calcStatus(r-1,c, UP);
  if( (dir==ALL||dir==DOWN) && bottom==center) up_down += calcStatus(r+1,c, DOWN);
	if( (dir==ALL||dir==LEFT) && left==center) left_right += calcStatus(r,c-1, LEFT);
	if( (dir==ALL||dir==RIGHT) && right==center) left_right += calcStatus(r,c+1, RIGHT);
	if( (dir==ALL||dir==UPRIGHT) && top_right==center) positive+= calcStatus(r-1,c+1, UPRIGHT);
	if( (dir==ALL||dir==UPLEFT) && top_left==center) negative+= calcStatus(r-1,c-1,UPLEFT);
	if( (dir==ALL||dir==DOWNLEFT) && bot_left==center) positive+= calcStatus(r+1,c-1,DOWNLEFT);
	if( (dir==ALL||dir==DOWNRIGHT) && bot_right==center) negative+= calcStatus(r+1,c+1,DOWNRIGHT);

	int cardinals max(up_down,left_right);
	int diagonals max(positive,negative);
	return max(cardinals, diagonals);

}

std::ostream& FIAR::operator<<(std::ostream& os, Game& bo) {
  //COLOR* pieces = bo.board.proto_board;
	os << "\r;  1 2 3 4 5 6 7 8 9 a b c d e f";
	os << "\r\n";
	for(int row = 0; row<15; row++) {
		//os << "\r";
		if (row<9) os << ";" << row+1 << " ";
		else if (row == 9) os << ";a ";
		else if (row == 10) os << ";b ";
		else if (row == 11) os << ";c ";
		else if (row == 12) os << ";d ";
		else if (row == 13) os << ";e ";
		else if (row == 14) os << ";f ";
		for(int column = 0; column<15; column++) {
			COLOR pieces = bo.getBoard()(row,column);
			if(pieces==EMPTY) os << "+ ";
			else if(pieces==WHITE) os << "O ";
			else os << "@ ";
		}
		os << "\r\n";
	}
	return os;
}
