#include <vector>

enum NodeType {
	MAX, MIN
};

using namespace FIAR;

class TreeNode {
public:
        TreeNode();
        TreeNode(TreeNode* h);
        TreeNode(Board b, NodeType);
        TreeNode(Board b, NodeType n, Move);
        ~TreeNode();

        int             number_of_children() const   { return children.size(); }
        TreeNode* get_child(int i)     const   	     { return children[i]; }
        const Board&    get_board()          const   { return board; }
        Board     	get_board_copy()     const   { return board; }
        NodeType        get_node_type()      const   { return node_type; }
        Move            get_move()           const   { return move; }

        void removeChild(int i) { children.erase(children.begin()+i); }
        void createChildren(int depth);
        void extend();

private:
        std::vector< TreeNode* > children;
        Board board;
        Move move;
        double value;
        NodeType node_type;
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
	void calculateTree();

private:
	TreeNode* head;
};
