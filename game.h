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

// End namespace FIAR
}

#endif
