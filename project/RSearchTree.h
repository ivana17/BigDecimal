#include "BigDecimal.h"
#include <vector>
#ifndef _RSEARCHTREE_H_
#define _RSEARCHTREE_H_

class RSearchTree{
private:
	TreeNode* root;
	void destroyTree();
	void moveTree(RSearchTree&);
	void copyTree(const RSearchTree&);
public:
	RSearchTree(vector<BigDecimal*>& values);
	RSearchTree(const RSearchTree&);
	RSearchTree(RSearchTree&&);
	RSearchTree& operator= (const RSearchTree&);
	RSearchTree& operator= (RSearchTree&&);
	vector<TreeNode*> intervalNodes(int start, int end);
	BigDecimal intervalSum(int start, int end);
	void printTree();
	~RSearchTree();
};
#endif _RSEARCHTREE_H_