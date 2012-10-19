enum NodeType {
	MAX, MIN
};

class Tree {
public:
	Tree();
	Tree(TreeNode* h) {head = new TreeNode(h->get_board_copy(), h->get_node_type(), h->get_move()); }
	~Tree() { delete head; }

	const TreeNode* get_head() { return head; }
	void createTree(const Board& h, int depth);
	void extendTree();
	void pruneTree(TreeNode* child);
private:
	TreeNode* head;
};

class TreeNode {
public:
	TreeNode();
	TreeNode(TreeNode* h);
	TreeNode(Board, NodeType);
	TreeNode(Board, NodeType, Move);
	~TreeNode();

	int 		number_of_children() 	{ return children.size(); }
	const TreeNode* get_child(int i) 	{ return children[i]; }
	const& Board 	get_board() 		{ return board; }
	Board 		get_board_copy() 	{ return board; }
	NodeType 	get_node_type() 	{ return node_type; }
	Move 		get_move() 		{ return move; }

	void removeChild(int i) { children.erase(children.begin()+i); }
	void createChildren(int depth);
	void extend();

private:
	std::vector<TreeNode*> children;
	Board board;
	Move move;
	int value;
	Nodetype node_type;
};

