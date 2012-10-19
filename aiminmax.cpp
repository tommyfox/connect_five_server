#include "ai.h"

struct node_data {
	Board board;
	Move move;
	double goodness;
};

enum Player {
	MAX, MIN
};

Move AIMinMax::genMove(const Board& b) {
	tree<node_data> tr;
	
}

node_data alphabeta(tree<node_data>::iterator node, int depth, alpha, beta, Player) {
	if(depth = 0 || node->first_child==NULL) return heuristic(node->data);
	std::vector<node_data> moves_to_check = gen_nodes(*node);
	if(Player==MAX) {
		for(std::vector<node_data>::iterator itr = moves_to_check.begin(); itr!=moves_to_check.end(); itr++) {
			
		}
	}
}

std::vector<node_data> gen_nodes(node_data node) {
	std::vector<node_data> vector_to_return;
	for(int i = 0; i<15; i++) {
		for(int j = 0; j<15; j++) {
			if(board(i,j) == FIAR::EMPTY && (
			   board.checkBounds(i+1,j) 	&& board(i+1,j)   != FIAR::EMPTY
			|| board.checkBounds(i-1,j) 	&& board(i-1,j)   != FIAR::EMPTY
			|| board.checkBounds(i,j+1) 	&& board(i,j+1)   != FIAR::EMPTY
			|| board.checkBounds(i,j-1) 	&& board(i,j-1)   != FIAR::EMPTY
			|| board.checkBounds(i+1,j+1)	&& board(i+1,j+1) != FIAR::EMPTY
			|| board.checkBounds(i+1,j-1) 	&& board(i+1,j-1) != FIAR::EMPTY
			|| board.checkBounds(i-1,j+1)	&& board(i-1,j+1) != FIAR::EMPTY
			|| board.checkBounds(i-1,j-1)	&& board(i-1,j-1) != FIAR::EMPTY ) )
				node_data data_to_return;
				if(node_data.move.getColor()==FIAR::WHITE) {
					node.board(i,j) = FIAR::BLACK;
					data_to_return.move(i,j,FIAR::BLACK);
				}
				else {
					node.board(i,j) = FIAR::WHITE;
					data_to_return.move(i,j,FIAR::WHITE);
				} 
				data_to_return.board = node.board;
				node.board(i,j) = FIAR::EMPTY;
				vector_to_return.push_back(data_to_return);
		}
	}
	return vector_to_return;
}
