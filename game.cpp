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

#include "game.h"

using namespace FIAR;

void Game::calcStatus(const int& r, const int& c) const {

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
