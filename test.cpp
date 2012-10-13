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

#include <iostream>

int main() {
  try {
    FIAR::Game g;

    std::cerr << g.exec(13, 8, FIAR::BLACK) << std::endl;
    std::cerr << g.exec(13, 8, FIAR::WHITE) << std::endl;

    g.undo();
    g.undo();

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
