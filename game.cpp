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

Game::Game(const COLOR& c, const AI_TYPE& ait) : srv_color(c), end_status(NOBODY) {
  try {
    switch (ait) {
      case(MINMAX):
        std::cerr << "got to the minxmax case\n";
        ai = new AIMinMax();
        break;
      default:
	std::cerr << "got to the default case\n";
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
    ||  (end_status != NOBODY)
    ||  (s == EMPTY)) { return false; }
  else {
    board(r, c) = s;
    moves.push(Move(r,c,s));
    this->calcStatus(r, c);

    if(end_status == NOBODY) {
      Move ai_move = ai->genMove(board);
      board(ai_move.getRow(), ai_move.getCol()) = srv_color;
      moves.push(ai_move);
      this->calcStatus(r, c);
    }

    return true;
  }
}

bool
Game::undo() {
  if(moves.size() >= 2) {
    for(int i = 0; i < 2; ++i) {
      board(moves.top().getRow(), moves.top().getCol()) = EMPTY;
      moves.pop();
      if(end_status == TIE) break;
    }
      return true;
  }
  else {
    return false;
  }
}

void Game::calcStatus(const int& r, const int& c) {
  if(moves.size() == 255) {
    end_status = TIE;
  }

  COLOR color = board(r, c);

  // Up and Down, Left and Right
  for(int i = 0; i < 15; ++i) {
    for(int j = 0; j < 11; ++j) {
      int ud_count = 0;
      int lr_count = 0;
      for(int k = 0; k < 5; ++k) {
        if(board(j + k, i) == color) {
          ++ud_count;
        }
        if(board(i, j + k) == color) {
          ++lr_count;
        }
      }
      if(ud_count == 5 || lr_count == 5) {
        if(color == WHITE) end_status = WHITE_WIN;
          else end_status = BLACK_WIN;
        return;
      }
    }
  }

  // Diagonals
  for(int i = 4; i < 15; ++i) {
    for(int j = 0; j < (i - 3); ++j) {
      int diags[4] = {0};
      for(int k = 0; k < 5; ++k) {
        if(board(i-k-j, j+k) == color) ++diags[0];
        if(board(14 - (i-k-j), 14 - (j+k)) == color) ++diags[1];
        if(board(j+k, 14-i+k) == color) ++diags[2];
        if(board(14 - (j+k), 14 - (14-i+k)) == color) ++diags[3];
      }
      for(int k = 0; k < 4; ++k) {
        if(diags[k] == 5) {
          if(color == WHITE) end_status = WHITE_WIN;
            else end_status = BLACK_WIN;
          return;
        }
      }
    }
  }
}

bool  Board::operator==(const Board& a) const {
	for(int i = 0; i<15; i++) {
		for(int j = 0; j<15; j++) {
			int index = i*15+j;
			COLOR * temp = proto_board;
			temp = temp + index;
			if(*temp!=a(i,j)) return false;
		}
	}
	return true;
}

std::ostream& FIAR::operator<<(std::ostream& os, Game& g) {
  os << "1 2 3 4 5 6 7 8 9 a b c d e f\n";
  for(int i = 0; i < 15; ++i) {
    for(int j = 0; j < 15; ++j) {
      COLOR p = g.board(i,j);
      if(p == EMPTY) {
        os << "+";
      }
      else if(p == WHITE) {
        os << "O";
      }
      else {
        os << "#";
      }
      os << " ";
    }
    os << "\n";
  }
  return os;
}
