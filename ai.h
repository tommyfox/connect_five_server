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

#ifndef _AI_H_
#define _AI_H_

#include "game.h"

namespace FIAR {

class node_data {
public:
	node_data() :move(1,1,EMPTY) { }
	~node_data() { }
	Board board;
	Move move;
	double value;
};

class AIRand : public AI {
  public:
    virtual Move genMove(const Board&);
};

class AIMinMax : public AI {
  public:
    virtual Move genMove(const Board&);
};

// End namespace FIAR
}

#endif
