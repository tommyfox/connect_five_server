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

#ifndef _GAME_H_
#define _GAME_H_

#include <stdexcept>
#include <cstdlib>
#include <stack>
#include <iostream>

namespace FIAR {

enum COLOR {
  EMPTY = 0,
  WHITE,
  BLACK
};


class Game {
  class Board {
    public:
      Board() throw (std::bad_alloc);
      ~Board() { delete proto_board; }

      bool checkBounds(const int&, const int&) const;
      COLOR&
        operator() (const int&, const int&) throw (std::out_of_range);
      friend std::ostream& operator<<(std::ostream& os, Game& bo);
    private:
      COLOR* proto_board;
  };

  class Move {
    public:
      Move(const int& row, const int& col, const COLOR& s) : space(s) {
        loc.first = row;
        loc.second = col;
      }

      const int&    getRow() const { return loc.first; }
      const int&    getCol() const { return loc.second; }
      const COLOR&  getColor() const { return space; }

    private:
      std::pair<int, int> loc;
      COLOR               space;
  };

  public:
    Game() : status(EMPTY) {}

    bool          exec(const int&, const int&, const COLOR&);
    bool          undo();
    const COLOR&  getStatus() const { return status; }

    friend std::ostream& operator<<(std::ostream& os, Game& b);
  private:
    void calcStatus(const int&, const int&) const;

    Board             board;
    COLOR             status;
    std::stack<Move>  moves;
};

inline Game::Board::Board() throw (std::bad_alloc) {
  try {
    proto_board = new COLOR[255];
    for(unsigned i = 0; i < 255; ++i) {
      proto_board[i] = EMPTY;
    }
  }
  catch (std::bad_alloc&) {
    delete[] proto_board;
    throw;
  }
}

inline bool Game::Board::checkBounds(const int& r, const int& c) const {
  if((r > 14) || (r < 0) || (c > 14) || (c < 0)) return false;
    else return true;
}

inline COLOR&
Game::Board::operator() (const int& r, const int& c) throw (std::out_of_range) {
  if(!(this->checkBounds(r, c)))
    throw std::out_of_range("operator() subscript out of bounds.");
  return proto_board[(r*14) + c];
}

inline bool
Game::exec(const int& r, const int& c, const COLOR& s) {
  if(   (!board.checkBounds(r, c))
    ||  (board(r, c) != EMPTY)
    ||  (status != EMPTY)
    ||  (s == EMPTY)) { return false; }

  board(r, c) = s;
  this->calcStatus(r, c);
  moves.push(Move(r,c,s));

  return true;
}

inline bool
Game::undo() {
  if(!moves.empty()) {
      board(moves.top().getRow(), moves.top().getCol()) = EMPTY;
      moves.pop();
      return true;
  }
  else {
    return false;
  }
}

std::ostream& operator<<(std::ostream& os, Game& bo) {
	COLOR* pieces = bo.board.proto_board;
	os << "\r;  1 2 3 4 5 6 7 8 9 a b c d e f";
	os << "\r\n";
	for(int row = 0; row<15; row++) {
		os << "\r";
		if(row<9) os << ";" << row+1 << " ";
		else if(row == 9) os << ";a ";
		else if (row == 10) os << ";b ";
		else if (row == 11) os << ";c ";
		else if (row == 12) os << ";d ";
		else if (row == 13) os << ";e ";
		else if (row == 14) os << ";f ";
		for(int column = 0; column<15; column++) {
			if(*pieces==EMPTY) os << "+ ";
			else if(*pieces==WHITE) os << "O ";
			else os << "@ ";
			pieces++;
		}
		os << "\r\n";
	}
	return os;
}
// End namespace FIAR
}

#endif
