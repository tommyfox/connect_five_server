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
#include <vector>
#include <stack>
#include <iostream>


namespace FIAR {

enum COLOR {
  EMPTY = 0,
  WHITE,
  BLACK
};

class Board {
  public:
    Board() throw (std::bad_alloc);
    ~Board() { }

    bool          checkBounds(const int&, const int&) const;
    COLOR         get_color(const int&, const int&) const;
    COLOR&        operator()(const int&, const int&) throw (std::out_of_range);
    const COLOR&  operator()(const int&, const int&) const throw (std::out_of_range);
    void setColor(int i, int j, COLOR c) {proto_board[15*i+j] = c;}

    bool    	  operator==(const Board& a) const;
    void          operator=(const Board& a);

  private:
    std::vector<COLOR> proto_board;
};

class Move {
  public:
    Move(const int& row, const int& col, const COLOR& s) :
      loc(row, col), space(s) {}

    const int&    getRow() const { return loc.first; }
    const int&    getCol() const { return loc.second; }
    const COLOR&  getColor() const { return space; }

    void setRow(int r) { loc.first = r; }
    void setCol(int c) { loc.second = c; }
    void setColor(COLOR c) {space = c;}

  private:
    std::pair<int, int> loc;
    COLOR               space;
};

enum END_STATUS {
  NOBODY = 0,
  WHITE_WIN,
  BLACK_WIN,
  TIE
};

enum AI_TYPE {
  RAND = 0,
  MINMAX
};

class AI {
  public:
    virtual Move genMove(const Board&) = 0;
};

class Game {
  public:
    Game(const COLOR&, const AI_TYPE&);
    ~Game() { delete ai; }

    bool              exec(const int&, const int&, const COLOR&);
    bool              undo();
    const END_STATUS& getStatus() const { return end_status; }

  private:
    void calcStatus(const int&, const int&);

    Board             board;
    COLOR             srv_color;
    END_STATUS        end_status;
    AI*               ai;
    std::stack<Move>  moves;

    friend std::ostream& operator<<(std::ostream&, Game&);
};

inline Board::Board() throw (std::bad_alloc) {
  try {
    for(unsigned i = 0; i < 225; ++i) {
      proto_board.push_back(EMPTY);
    }
  }
  catch (...) {
//    delete[] proto_board;
    throw;
  }
}

inline bool
Board::checkBounds(const int& r, const int& c) const {
  if((r > 14) || (r < 0) || (c > 14) || (c < 0)) return false;
    else return true;
}

inline COLOR
Board::get_color(const int& r, const int& c) const {
  if(!(this->checkBounds(r,c))) throw std::out_of_range("operator() subscript out of bounds");
  return proto_board[(r*15)+c];
}

inline COLOR&
Board::operator() (const int& r, const int& c) throw (std::out_of_range) {
  if(!(this->checkBounds(r, c)))
    throw std::out_of_range("operator() subscript out of bounds.");
  return proto_board[(r*15) + c];
}

inline const COLOR&
Board::operator() (const int& r, const int& c) const throw (std::out_of_range) {
  if(!(this->checkBounds(r, c)))
    throw std::out_of_range("const operator() subscript out of bounds.");
  return proto_board[(r*15) + c];
}

std::ostream& operator<<(std::ostream&, const Game&);

// End namespace FIAR
}

#endif
