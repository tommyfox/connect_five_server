#include "ai.h"

Move AIMinMax::genMove(const Board& b) {
	// if the tree exists
	if(move_tree.get_head()!=NULL) {
		const TreeNode* head = move_tree.get_head();
		bool found_child = false;
		for(int i = 0; i < head->number_of_children(); i++) {
			TreeNode* child = head->get_child(i);
			if(child->get_board()==b) {
				move_tree.pruneTree(child);
				found_child = true;
				break;
			}
		}
		// if no child was found
		if(!found_child) {
			move_tree.createTree(b,3);
		}
		// if a child was found
		else {
			move_tree.extendTree();
		}
		move_tree.calculateTree();
	}
	// if the tree does not exist
	else {
		move_tree.createTree(b,3);
	}
}

void Tree::pruneTree(TreeNode* child) {
	// already checked to see if a child exists in the gen move function
	for(int i = 0; i< head->number_of_children(); i++) {
		if(head->get_child(i)==child) head->removeChild(i);
	}
	delete head;
	head = child;
}

TreeNode::TreeNode() : move(1,1,FIAR::WHITE) {}


TreeNode::TreeNode(TreeNode* h) : move(h->get_move()) {
	board = h->get_board_copy();
	node_type = h->get_node_type();
	for(int i = 0; i< h->number_of_children(); i++) {
		TreeNode* child = new TreeNode(h->get_child(i));
		children.push_back(child);
	}
}

TreeNode::TreeNode(Board b, NodeType n) : move(1,1,FIAR::WHITE) {
	board = b;
	node_type = n;
}

TreeNode::TreeNode(Board b, NodeType n, Move m) :move(m) {
	TreeNode(b,n);
}

TreeNode::~TreeNode() {
	for(int i = 0; i<children.size(); i++) {
		delete children[i];
	}
}

void Tree::createTree(const Board& h, int depth) {
	delete head;
	head = new TreeNode(h, MAX);
	head->createChildren(depth);
}

void TreeNode::createChildren(int depth) {
	if(depth>0) {
		NodeType n_type;
		if(node_type==MAX) n_type = MIN;
		else n_type = MAX;
		for(int i = 0; i<15; i++) {
			for(int j = 0; j<15; j++) {
				if((	board(i+1,j)
				||	board(i-1,j)
				||	board(i,j+1)
				||	board(i,j-1)
				||	board(i+1,j+1)
				||	board(i+1,j-1)
				||	board(i-1,j+1)
				||	board(i-1,j-1))
				&& 	board(i,j)==EMPTY ) {
					if(node_type==MAX) board(i,j) = WHITE;
					else board(i,j) = BLACK;
					TreeNode* child = new TreeNode(board, n_type, Move(i,j,board(i,j)));
					children.push_back(child);
					board(i,j) = EMPTY;
					child->createChildren(depth-1);
				}
			}
		}
	}
}

void Tree::extendTree() {
	head->extend();
}

void TreeNode::extend() {
	if(children.size()==0) createChildren(2);
	else {
		for(int i = 0; i<children.size(); i++) {
			children[i]->extend();
		}
	}
}

void Tree::calculateTree() {
	
}

