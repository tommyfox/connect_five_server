#include <boost/property_tree/ptree.hpp>

enum NodeType {
	MAX, MIN
};

Move AIMinMax::getMove(const Board& b) {
	// if the tree exists
	if(move_tree.get_head()!=NULL) {
		const TreeNode* head = move_tree.get_head();
		bool found_child = false;
		for(int i = 0; i < head->number_of_children(); i++) {
			const TreeNode* child = head.get_child(i);
			if(child.get_board()==b) {
				// make this
				pruneTree(child);
				found_child = true;
				break;
			}
		}
		// if no child was found
		if(!found_child) {
			delete tree;
			// make this
			createTree(b);
		}
		// if a child was found
		else {
			// make this
			extendTree();
		}
		recalculateTree();
	}
	// if the tree does not exist
	else {
		createTree(b);
	}
}

class Tree {
public:
	Tree();
	~Tree() { delete head; }

	const TreeNode* get_head() { return head; }
	void createTree(const Board& h, int depth);

private:
	TreeNode* head;
};

class TreeNode {
public:
	TreeNode();
	TreeNode(Board, NodeType);
	TreeNode(Board, NodeType, Move);
	~TreeNode();

	int number_of_children() { return children.size(); }
	const TreeNode* get_child(int i) { return children[i]; }
	const& Board get_board() { return Board; }

	void createChildren(int depth);

private:
	std::vector<TreeNode*> children;
	Board board;
	Move move;
	int value;
	Nodetype node_type;
};

TreeNode::~TreeNode() {
	for(int i = 0; i<children.size(); i++) {
		delete children[i];
	}
}

void Tree::createTree(const Board& h, int depth) {
	head = new TreeNode(board, MAX);
	head.createChildren(depth);
}

void TreeNode::createChildren(int depth) {
	if(depth>0) {
		NodeType node_type WRITE SOEM FUNTION OF THIS;
	
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
					board(i,j) = WHITE;
					head = new TreeNode(board, node_type, Move(i,j,WHITE) );
					head.createChildren(depth-1);
				}
			}
		}
	}
}
