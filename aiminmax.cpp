#include "ai.h"

Move AIMinMax::getMove(const Board& b) {
	// if the tree exists
	if(move_tree.get_head()!=NULL) {
		const TreeNode* head = move_tree.get_head();
		bool found_child = false;
		for(int i = 0; i < head->number_of_children(); i++) {
			const TreeNode* child = head.get_child(i);
			if(child.get_board()==b) {
				move_tree.pruneTree(child);
				found_child = true;
				break;
			}
		}
		// if no child was found
		if(!found_child) {
			delete tree;
			move_tree.createTree(b);
		}
		// if a child was found
		else {
			move_tree.extendTree();
		}
		move_tree.calculateTree();
	}
	// if the tree does not exist
	else {
		move_tree.createTree(b);
	}
}

void Tree::pruneTree(TreeNode* child) {
	// already checked to see if a child exists in the gen move function
	for(int i = 0; i<children.size(); i++) {
		if(head->get_child()==child) head->removeChild(i);
	}
	delete head;
	head = child;
}

TreeNode::TreeNode(TreeNode* h) {
	board = h->get_board_copy();
	move = h->get_move();
	node_type = h->get_node_type();
	for(int i = 0; i< h->number_of_children(); i++) {
		TreeNode* child = new Treenode(h->getChild(i));
		children.push_back(child);
	}
}

Tree::TreeNode(Board b, NodeType n) {
	board = b;
	node_type = n;
}

Tree::TreeNode(Board b, NodeType n, move m) {
	TreeNode(b,n);
	move = m;
}

TreeNode::~TreeNode() {
	for(int i = 0; i<children.size(); i++) {
		delete children[i];
	}
}

void Tree::createTree(const Board& h, int depth) {
	head = new TreeNode(board, MAX);
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
/*
void Tree::calculateTree() {
	
}
*/
