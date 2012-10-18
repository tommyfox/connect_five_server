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
#include "ai.h"

using namespace FIAR;

Game::Game(const COLOR& c, const AI_TYPE& ait) : srv_color(c), end_status(EMPTY) {
  try {
    switch (ait) {
      case EASY:
      break;

      case MED:
      break;

      case HARD:
      break;
      
      default:
        ai = new AIRand();
      break;
    }
    
  }
  catch (std::bad_alloc&) {
    delete ai;
    throw;
  }
}

bool
Game::exec(const int& r, const int& c, const COLOR& s) {
  if(   (!board.checkBounds(r, c))
    ||  (board(r, c) != EMPTY)
    ||  (end_status != EMPTY)
    ||  (s == EMPTY)) { return false; }
  else {
    board(r, c) = s;
    this->calcStatus(r, c);
    moves.push(Move(r,c,s));

    Move ai_move = ai->genMove(board);
    board(ai_move.getRow(), ai_move.getCol()) = srv_color;
    moves.push(ai_move);

    return true;
  }
}

bool
Game::undo() {
  if(moves.size() >= 2) {
    for(int i = 0; i < 2; ++i) {
      board(moves.top().getRow(), moves.top().getCol()) = EMPTY;
      moves.pop();
    }
      return true;
  }
  else {
    return false;
  }
}

int Game::calcStatus(const int& r, const int& c) {
	if(!board.checkBounds(r,c)) return 0;

	COLOR center = board(r,c);

	int row_count=0;
	int column_count=0;
	int positive_count=0;
	int negative_count=0;
	// row
	for(int row = r-4; row<=r+4; row++) {
		if(board.checkBounds(row,c) && board(row,c)==center) {
			row_count++;
		}
		else if(board.checkBounds(row,c)) {
			row_count = 0;
		}
		if(row_count>=5) return row_count;
	}
	// column
	for(int column = c-4; column<=c+4; column++) {
		if(board.checkBounds(r,column) && board(r,column)==center) {
			column_count++;
		}
		else if(board.checkBounds(r,column)) {
			column_count = 0;
		}
		if(column_count>=5) return column_count;
	}
	// negative slope
	for(int row = -4; row<=4; row++) {
		if(board.checkBounds(r+row,c+row) && board(r+row,c+row)==center) {
			negative_count++;
		}
		else if(board.checkBounds(r+row,c+row)) {
			negative_count = 0;
		}
		if(negative_count>=5) return negative_count;
	}
	// positive slope
	for(int row = -4; row<=4; row++) {
		if(board.checkBounds(r-row,c+row) && board(r-row,c+row)==center) {
			positive_count++;
		}
		else if(board.checkBounds(r-row,c+row)) {
			positive_count = 0;
		}
		if(positive_count>=5) return positive_count;
	}
	return std::max(std::max(row_count,column_count),std::max(positive_count,negative_count));
}

std::ostream& FIAR::operator<<(std::ostream& os, Game& g) {
	os << "\r;  1 2 3 4 5 6 7 8 9 a b c d e f";
	os << "\r\n";
	for(int row = 0; row<15; row++) {
		if (row<9) os << ";" << row+1 << " ";
		else if (row == 9) os << ";a ";
		else if (row == 10) os << ";b ";
		else if (row == 11) os << ";c ";
		else if (row == 12) os << ";d ";
		else if (row == 13) os << ";e ";
		else if (row == 14) os << ";f ";
		for(int column = 0; column<15; column++) {
			COLOR pieces = g.board(row,column);
			if(pieces==EMPTY) os << "+ ";
			else if(pieces==WHITE) os << "O ";
			else os << "@ ";
		}
		os << "\r\n";
	}
	return os;
}
