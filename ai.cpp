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

#include <cstdlib>
#include <ctime>

#include "ai.h"

using namespace FIAR;

Move
AIRand::genMove(const Board& b) {
  srand(time(0));
  int r = rand() % 15;
  int c = rand() % 15;

  bool valid = b.checkBounds(r, c);
  while(valid != true || b(r, c) != EMPTY) {
    r = rand() % 15;
    c = rand() % 15;
    valid = b.checkBounds(r, c);
  }

  Move move(r, c, EMPTY);
  return move;
}
