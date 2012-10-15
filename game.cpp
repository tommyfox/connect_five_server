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

int Game::calcStatus(const int& r, const int & c, direction dir) const {
	if(!board.checkBounds(r,c)) return 0;

	COLOR center = board(r,c);

	int row_count=0;
	int column_count=0;
	int positive_count=0;
	int negative_count=0;
	for(int row = r-4; row<r+4; row++) {
		if(board.checkBounds(row,c) && board(row,c)==center) {
			row_count++;
		}
		else if(board.checkBounds(row,c)) {
			row_count = 0;
		}
		if(row_count>=5) return row_count;
	}
	for(int column = c-4; column<c+4; column++) {
		if(board.checkBounds(r,column) && board(r,column)==center) {
			column_count++;
		}
		else if(board.checkBounds(r,column)) {
			column_count = 0;
		}
		if(column_count>=5) return column_count;
	}
	for(int row = r-4; row<r+4; row++) {
		if(board.checkBounds(r+row,c+row) && board(r+row,c+row)==center) {
			positive_count++;
		}
		else if(board.checkBounds(r+row,c+row)) {
			positive_count = 0;
		}
		if(positive_count>=5) return positive_count;
	}
	for(int row = r-4; row<r+4; row++) {
		if(board.checkBounds(r+row,-c+row) && board(r+row,-c+row)==center) {
			negative_count++;
		}
		else if(board.checkBounds(r+row,-c+row)) {
			negative_count = 0;
		}
		if(negative_count>=5) return negative_count;
	}

}

/*
int Game::calcStatus(const int& r, const int& c, direction dir) const {
	if(!board.checkBounds(r,c)) return 0;

	bool left_side=false, right_side=false, top_side=false, bottom_side=false;
	if(r<=0) top_side = true;
	if(r>14) bottom_side = true;
	if(c<=0) left_side = true;
	if(c>14) right_side = true;

	COLOR top, bottom, left, right, top_left, top_right, bot_left, bot_right, center;
	center 		= board(r,c);
	if(!top_side)									top 			= board(r-1,c);
	if(!bottom_side) 							bottom 		= board(r+1,c);
	if(!left_side) 								left 			= board(r,c-1);
	if(!right_side) 							right 		= board(r,c+1);
	if(!top_side&&!left_side) 		top_left	= board(r-1,c-1);
	if(!top_side&&!right_side) 		top_right	= board(r-1,c+1);
	if(!bottom_side&&!left_side) 	bot_left	= board(r+1,c-1);
	if(!bottom_side&&!right_side) bot_right = board(r+1,c+1);

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

	int cardinals = std::max(up_down,left_right);
	int diagonals = std::max(positive,negative);
	if(cardinals!=0||diagonals!=0) return std::max(cardinals, diagonals);
	else return 1;
}*/

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
