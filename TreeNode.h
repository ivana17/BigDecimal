#ifndef _TREENODE_H_
#define _TREENODE_H_

class TreeNode{
private:
	int start;
	int end;
	BigDecimal value;
	TreeNode* left;
	TreeNode* right;
	BigDecimal getVal();
	bool intersect(int, int);
public:
	TreeNode(int start, int end, const BigDecimal* val);
	TreeNode(const TreeNode&);
	TreeNode* getLeft();
	TreeNode* getRight();
	void setLeft(TreeNode*);
	void setRight(TreeNode*);
	int getStart();
	int getEnd();
	bool covers(int start, int end);
	friend ostream& operator<<(ostream&, const TreeNode&);
	friend class RSearchTree;
	~TreeNode();
};

#endif _TREENODE_H_
