#include "dz1.h"

TreeNode::TreeNode(int start, int end, const BigDecimal* val) :start(start), end(end), value(*val), left(nullptr), right(nullptr){}

TreeNode::TreeNode(const TreeNode& originalNode) : value(originalNode.value){
	start = originalNode.start;
	end = originalNode.end;
	left = nullptr;
	right = nullptr;
}

TreeNode* TreeNode::getLeft(){
	return this->left;
}

TreeNode* TreeNode::getRight(){
	return this->right;
}

void TreeNode::setLeft(TreeNode* subtreeRoot){
	this->left = subtreeRoot;
}

void TreeNode::setRight(TreeNode* subtreeRoot){
	this->right = subtreeRoot;
}

int TreeNode::getStart(){
	return this->start;
}

int TreeNode::getEnd(){
	return this->end;
}

BigDecimal TreeNode::getVal(){
	return BigDecimal(this->value);
}

bool TreeNode::covers(int Start, int End){
	if ((this->start >= Start) && (this->end <= End)) return true;
	else return false;
}

ostream& operator<<(ostream& out, const TreeNode& node){
	out << node.value;
	out << "[" << node.start << ":" << node.end << "]";
	return out;
}

TreeNode::~TreeNode(){
	left = nullptr;
	right = nullptr;
}

bool TreeNode::intersect(int s, int e){
	if ((start > e) || (end < s)) return false;
	else if ((start <= s) && (end <= e)) return true;
	else if ((start >= s) && (end >= e)) return true;
	else if ((start <= s) && (end >= e)) return true;
	else return false;
}