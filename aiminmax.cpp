#include "ai.h"
#include <vector>
using namespace FIAR;


enum Player {
	MAX, MIN
};

node_data alphabeta(node_data, int, double, double, Player);

Move AIMinMax::genMove(const Board& b) {
	node_data head;
	head.board = b;
	std::cerr << "right before the alphabeta\n";
	node_data best_move = alphabeta(head, 5, -999, 999, MAX);
	return best_move.move;
}

std::vector<node_data> gen_nodes(node_data node);

node_data heuristic(node_data);

node_data alphabeta(node_data node, int depth, double alpha, double beta, Player p) {
	std::cerr << "running alphabeta at depth " << depth << " and position " << node.move.getRow() << ", " << node.move.getCol() << "\n";
	std::vector<node_data> moves_to_check = gen_nodes(node);
	if(depth == 0) return heuristic(node);
	if(p==MAX) {
		std::vector<node_data>::iterator itr;
		for(itr = moves_to_check.begin(); itr!=moves_to_check.end(); itr++) {
			alpha = std::max(alpha, alphabeta((*itr), depth-1, alpha, beta, MIN).value);
			if(beta<=alpha) break;
		}
		if(itr==moves_to_check.end()) itr--;
		(*itr).value = alpha;
		std::cerr << "RETURNED SOMETHING\n\n";
		return *itr;
	}
	else {
		std::vector<node_data>::iterator itr;
		for(itr = moves_to_check.begin(); itr!=moves_to_check.end(); itr++) {
			beta = std::min(beta, alphabeta((*itr), depth-1, alpha, beta, MAX).value);
			if(beta<=alpha) break;
		}
		if(itr==moves_to_check.end()) itr--;
		(*itr).value = alpha;
		std::cerr << "RETURNED SOMETHING\n\n";
		return *itr;
	}
}

std::vector<node_data> gen_nodes(node_data node) {
	std::vector<node_data> vector_to_return;
	Board board = node.board;
	COLOR center;
	for(int i = 0; i<15; i++) {
		for(int j = 0; j<15; j++) {
			//Board board = node.board;
			if((
			   board.checkBounds(i+1,j) 	&& board(i+1,j)   != FIAR::EMPTY
			|| board.checkBounds(i-1,j) 	&& board(i-1,j)   != FIAR::EMPTY
			|| board.checkBounds(i,j+1) 	&& board(i,j+1)   != FIAR::EMPTY
			|| board.checkBounds(i,j-1) 	&& board(i,j-1)   != FIAR::EMPTY
			|| board.checkBounds(i+1,j+1)	&& board(i+1,j+1) != FIAR::EMPTY
			|| board.checkBounds(i+1,j-1) 	&& board(i+1,j-1) != FIAR::EMPTY
			|| board.checkBounds(i-1,j+1)	&& board(i-1,j+1) != FIAR::EMPTY
			|| board.checkBounds(i-1,j-1)	&& board(i-1,j-1) != FIAR::EMPTY ) 
			&& board(i,j) == FIAR::EMPTY) {
				node_data data_to_return;
				center = board(i,j);
				if(node.move.getColor()==FIAR::WHITE) {
					board.setColor(i,j,FIAR::BLACK);
					data_to_return.move.setRow(i);
					data_to_return.move.setCol(j);
					data_to_return.move.setColor(FIAR::BLACK);
				}
				else {
					board.setColor(i,j,FIAR::WHITE);
					data_to_return.move.setRow(i);
					data_to_return.move.setCol(j);
					data_to_return.move.setColor(FIAR::WHITE);
				}
				data_to_return.board = board;
				vector_to_return.push_back(data_to_return);
				board.setColor(i,j,EMPTY);
			}
		}
	}
	return vector_to_return;
}

int enemies_surrounding(int, int , Board);
int friendly_surrounding(int, int, Board);

node_data heuristic(node_data node) {
	/* Thomas's Proposed version: instead of "either-or" construction, all possibilities are considered
	 * and all possibilities add to node value.  All values should be modified according to testing results.
	 * Additionally, code to detect surrending units need be modified to determine space beyond surrounding pieces
	 * On second thought, the "surrounding" functions need to be completely rewritten...

	int friendlies = friendly_surrounding(node.move.getRow(), node.move.getCol(), node.board);
	int enemies = enemies_surrounding(node.move.getRow(), node.move.getCol(), node.board);

	if (friendlies >= 5) node.value += 99; //Node secures win
	if (enemies >= 4) node.value += 50;    //Node would secure enemy win next turn
	if (enemies >= 3) node.value += 25;    //Node need be blocked preemptively
	if (
	
	
	*/
	int friendlies = friendly_surrounding(node.move.getRow(), node.move.getCol(), node.board);
	int enemies = enemies_surrounding(node.move.getRow(), node.move.getCol(), node.board);
	if(friendlies>=5) node.value=999;
	else if(enemies>=5) node.value=998;
	else if(friendlies==4) node.value=100;
	else if(enemies==4) node.value=99;
	else node.value=friendlies+enemies;
	return node;
}

int enemies_surrounding(int r, int c, Board board) {
  COLOR current_move = board(r, c);
  if(current_move==FIAR::WHITE) current_move=FIAR::BLACK;
  else current_move=FIAR::WHITE;
  int i, vertical = 0, horizontal = 0, positive = 0, negative = 0;
  //vertical
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r-i,c)) break;
    if (board(r-i,c) == current_move) ++vertical;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r+i,c)) break;
    if (board(r+i,c) == current_move) ++vertical;
	else break;
  }
  //horizontal
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r,c-i)) break;
    if (board(r,c-i) == current_move) ++horizontal;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r,c+i)) break;
    if (board(r,c+i) == current_move) ++horizontal;
	else break;
  }
  //up-right
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r-i,c+i)) break;
    if (board(r-i,c+i) == current_move) ++positive;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r+i,c-i)) break;
    if (board(r+i,c-i) == current_move) ++positive;
	else break;
  }
  //down-right
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r-i,c-i)) break;
    if (board(r-i,c-i) == current_move) ++negative;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r+i,c+i)) break;
    if (board(r+i,c+i) == current_move) ++negative;
	else break;
  }
  std::max(std::max(vertical, horizontal), std::max(positive, negative));
}

int friendly_surrounding(int r, int c, Board board) {
  COLOR current_move = board(r, c);

  int i, vertical = 0, horizontal = 0, positive = 0, negative = 0;
  //vertical
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r-i,c)) break;
    if (board(r-i,c) == current_move) ++vertical;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r+i,c)) break;
    if (board(r+i,c) == current_move) ++vertical;
	else break;
  }
  //horizontal
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r,c-i)) break;
    if (board(r,c-i) == current_move) ++horizontal;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r,c+i)) break;
    if (board(r,c+i) == current_move) ++horizontal;
	else break;
  }
  //up-right
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r-i,c+i)) break;
    if (board(r-i,c+i) == current_move) ++positive;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r+i,c-i)) break;
    if (board(r+i,c-i) == current_move) ++positive;
	else break;
  }
  //down-right
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r-i,c-i)) break;
    if (board(r-i,c-i) == current_move) ++negative;
	else break;
  }
  for (i = 1; ; ++i) {
    if(!board.checkBounds(r+i,c+i)) break;
    if (board(r+i,c+i) == current_move) ++negative;
	else break;
  }
  return std::max(std::max(vertical, horizontal), std::max(positive, negative));
}
