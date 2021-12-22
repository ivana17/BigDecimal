#include "dz1.h"

RSearchTree::RSearchTree(vector<BigDecimal*>& values){
	stack<int>* rightBounds = new stack<int>;
	stack<TreeNode*>* nodeStack = new stack<TreeNode*>;
	int iter = 0;
	int n = values.size();
	int mid = n - 1;
	do {
		rightBounds->push(mid);
		mid = mid / 2;
	} while (mid != 0);
	bool newSeg = false;

	while ((iter < n) || (!nodeStack->empty())){

		TreeNode* left = nullptr;
		if ((!nodeStack->empty()) && (!newSeg)){
			left = nodeStack->top();
			nodeStack->pop();
		}
		else{
			left = new TreeNode(iter, iter, values.at(iter));
			++iter;
		}

		if (!(rightBounds->empty()) && (iter == rightBounds->top()))
			newSeg = true;

		TreeNode* right = nullptr;
		if ((!nodeStack->empty()) && (!newSeg)){
			right = nodeStack->top();
			nodeStack->pop();
		}
		else{
			right = new TreeNode(iter, iter, values.at(iter));
			++iter;
		}

		if (right->end < left->start){
			TreeNode* hNode = right;
			right = left;
			left = hNode;
			hNode = nullptr;
		}

		TreeNode* node = new TreeNode(left->start, right->end, &(left->getVal()).add(&right->getVal()));
		node->left = left;
		node->right = right;

		if ((iter == n) && (nodeStack->empty())){
			root = node;
			node = nullptr;
			left = nullptr;
			right = nullptr;
			break;
		}
		else
			nodeStack->push(node);

		if (!(rightBounds->empty())&& (iter > rightBounds->top())) rightBounds->pop();
		if (!(rightBounds->empty()) && (iter != rightBounds->top())){
			newSeg = true;
			mid = rightBounds->top() - iter; 
			while (mid > 1){
				mid = mid / 2;
				rightBounds->push(iter + mid);
			}
		}
		else
			newSeg = false; 
	}
	while (!rightBounds->empty()) rightBounds->pop();

	delete rightBounds;
	delete nodeStack;

	//assert((iter == n) && (nodeStack.empty()));
}

RSearchTree::RSearchTree(RSearchTree&& originalTree){
	this->moveTree(originalTree);
}

void RSearchTree::moveTree(RSearchTree& originalTree){
	this->root = originalTree.root;
	originalTree.root = nullptr;
}

void RSearchTree::copyTree(const RSearchTree& originalTree){
	if (originalTree.root == nullptr) return;
	TreeNode* node = originalTree.root;
	queue<TreeNode*>* nodeQueue = new queue<TreeNode*>;
	queue<TreeNode*>* newNodeQueue = new queue<TreeNode*>;
	TreeNode* newNode = this->root = new TreeNode(node->start, node->end, &node->value);
	nodeQueue->push(node);
	newNodeQueue->push(newNode);

	while (!nodeQueue->empty() && !newNodeQueue->empty()){
		node = nodeQueue->front();
		nodeQueue->pop();
		newNode = newNodeQueue->front();
		newNodeQueue->pop();

		if (node->left){
			newNode->left = new TreeNode(node->left->start, node->left->end, &(node->left)->value);
			nodeQueue->push(node->left);
			newNodeQueue->push(newNode->left);
		}
		if (node->right){
			newNode->right = new TreeNode(node->right->start, node->right->end, &(node->right)->value);
			nodeQueue->push(node->right);
			newNodeQueue->push(newNode->right);
		}
	}
	node = nullptr;
	newNode = nullptr;
	delete nodeQueue;
	delete newNodeQueue;
}

RSearchTree& RSearchTree::operator= (RSearchTree&& originalTree){
	this->destroyTree();
	this->moveTree(originalTree);
	return *this;
}

vector<TreeNode*> RSearchTree::intervalNodes(int Start, int End){
	queue<TreeNode*> nodeQueue;
	vector<TreeNode*> nodeVector;
	if (!root) return nodeVector;
	TreeNode* node = root;
	nodeQueue.push(node);
	while (!nodeQueue.empty()){
		TreeNode* newNode = nodeQueue.front();
		nodeQueue.pop();
		if (newNode->covers(Start, End)) {
			TreeNode* coverNode = newNode;
			nodeVector.push_back(coverNode);
		}
		else if (newNode->intersect(Start, End)){
			nodeQueue.push(newNode->left);
			nodeQueue.push(newNode->right);
		}
	}

	return nodeVector;
}

BigDecimal RSearchTree::intervalSum(int Start, int End){
	vector<TreeNode*> nodeVector = intervalNodes(Start, End);
	if (nodeVector.empty()) return BigDecimal("0\0");
	vector<TreeNode*>::iterator iter;
	iter = nodeVector.begin();
	BigDecimal sum = "0\0";
	while (iter != nodeVector.end()){
		TreeNode* tNode = *iter;
		sum = sum.add(&(tNode->getVal()));
		++iter;
	}
	return sum;
}

void RSearchTree::printTree(){
	if (!this->root) return;
	TreeNode* node = this->root;
	queue<TreeNode*>* nodeQueue = new queue<TreeNode*>;
	nodeQueue->push(node);
	int i = 0;
	while (!nodeQueue->empty()){
		node = nodeQueue->front();
		nodeQueue->pop();
		if (i % 10 == 9) cout << endl;
		cout << *node << " ";
		if (node->left)
			nodeQueue->push(node->left);
		if (node->right)
			nodeQueue->push(node->right);
		i++;
	}
}

RSearchTree::RSearchTree(const RSearchTree& originalTree){
	this->copyTree(originalTree);
}

RSearchTree& RSearchTree::operator= (const RSearchTree& originalTree){
	this->destroyTree();
	this->copyTree(originalTree);
	return *this;
}

void RSearchTree::destroyTree(){
	TreeNode* node = this->root;
	if (node == nullptr)
		return;
	TreeNode* lastVisitedNode = nullptr;
	stack<TreeNode*> unvisitedNodes;
	while (!unvisitedNodes.empty() || node != nullptr){
		if (node != nullptr){
			unvisitedNodes.push(node);
			node = node->left;
		}
		else{
			TreeNode* topNode = unvisitedNodes.top();
			if (topNode->right != nullptr && lastVisitedNode != topNode->right){
				node = topNode->right;
			}
			else{
				delete topNode;
				lastVisitedNode = topNode;
				unvisitedNodes.pop();
			}
		}
	}
	bool noUnvisitiedNodes = unvisitedNodes.empty();
}

RSearchTree::~RSearchTree(){
	destroyTree();
	this->root = nullptr;
}