/* Copyright (C) 2018
* Course: CO2003
* Author: Rang Nguyen
* Ho Chi Minh City University of Technology
*/

#include "TreeSet.h"

TreeSet::TreeSet()
{
	root = NULL;
	count = 0;
}
TreeSet::~TreeSet()
{
	clear();
}

void TreeSet::clearRec(AVLNode* root) {
	if (root != NULL) {
		clearRec(root->left);
		clearRec(root->right);
		delete root;
	}
}
void TreeSet::clear() {
	clearRec(root);
	root = NULL;
	count = 0;
}
int max(int a, int b)
{
	return (a > b) ? a : b;
}
AVLNode* newNode(int val)
{
	AVLNode* Node = new AVLNode(val);
	Node->balance = 1;
	Node->left = NULL;
	Node->right = NULL;
	return Node;
}
int height(AVLNode* node)
{
	if (node == NULL)
	{
		return 0;
	}
	return node->balance;
}
AVLNode* rightRotate(AVLNode* node)
{
	AVLNode* leftTree = node->left;
	AVLNode* rightTree = leftTree->right;

	leftTree->right = node;
	node->left = rightTree;

	node->balance = max(height(node->left), height(node->right)) + 1;
	leftTree->balance = max(height(leftTree->left), height(leftTree->right)) + 1;
	return leftTree;
}
AVLNode* leftRotate(AVLNode* node)
{
	AVLNode* rightTree = node->right;
	AVLNode* leftTree = rightTree->left;
	rightTree->left = node;
	node->right = leftTree;
	node->balance = max(height(node->left), height(node->right)) + 1;
	rightTree->balance = max(height(rightTree->left), height(rightTree->right)) + 1;
	return rightTree;
}
int getBalance(AVLNode* node)
{
	if (node == NULL)
	{
		return 0;
	}
	return height(node->left) - height(node->right);
}
AVLNode* insert(AVLNode* &root, int val, int &check)
{
	if (root == NULL)
	{
		check = 1;
		AVLNode* pNode = newNode(val);
		root = pNode;
		return root;
	}
	if (val < root->key)
	{
		root->left = insert(root->left, val, check);
	}
	else if (val > root->key)
	{
		root->right = insert(root->right, val, check);
	}
	else
	{
		check = 0;
		return root;
	}
	root->balance = 1 + max(height(root->left), height(root->right));
	int balance = getBalance(root);
	if (balance > 1 && val < root->left->key)
	{
		return rightRotate(root);
	}
	if (balance<-1 && val>root->right->key)
	{
		return leftRotate(root);
	}
	if (balance > 1 && val > root->left->key)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	if (balance < -1 && val < root->right->key)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}
AVLNode* minValueNode(AVLNode* root)
{
	AVLNode* current = root;
	while (current->left != NULL)
	{
		current = current->left;
	}
	return current;
}
AVLNode* deleteNode(AVLNode* &root, int val, int &check)
{
	if (root == NULL)
	{
		return root;
	}
	if (val < root->key)
	{
		root->left = deleteNode(root->left, val, check);

	}
	else if (val > root->key)
	{
		root->right = deleteNode(root->right, val, check);

	}
	else
	{
		check = 1;
		if ((root->left == NULL) || (root->right == NULL))
		{
			AVLNode* temp = root->left ? root->left : root->right;
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else
			{
				*root = *temp;
			}
			free(temp);
		}
		else
		{
			AVLNode* temp = minValueNode(root->right);
			root->key = temp->key;
			root->right = deleteNode(root->right, temp->key, check);
		}
	}
	if (root == NULL)
	{
		return root;
	}
	root->balance = 1 + max(height(root->left), height(root->right));
	int balance = getBalance(root);

	if (balance > 1 && getBalance(root->left) >= 0)
	{
		return rightRotate(root);
	}
	if (balance > 1 && getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	if (balance < -1 && getBalance(root->right) <= 0)
	{
		return leftRotate(root);
	}
	if (balance < -1 && getBalance(root->right)>0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}
	return root;
}
int TreeSet::add(int val) {
	int check = 0;
	root = insert(root, val, check);
	if (check == 1)
	{
		count += 1;
	}
	return check;
}
bool checkContains(AVLNode* root, int val)
{
	if (root != NULL)
	{
		if (root->key == val)
		{
			return true;
		}
		else if (val < root->key)
		{
			checkContains(root->left, val);
		}
		else
		{
			checkContains(root->right, val);
		}
	}
	else
	{
		return false;
	}
}
bool TreeSet::contains(int val) {
	return checkContains(root, val);
}
void copySet(AVLNode* &root, AVLNode* set)
{

	if (set != NULL)
	{
		int check = 0;
		insert(root, set->key, check);
		copySet(root->left, set->left);

		copySet(root->right, set->right);
	}
}
void TreeSet::copy(const TreeSet& set) {
	this->clear();
	this->count = set.count;
	copySet(this->root, set.root);
}
int TreeSet::first() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	AVLNode* pTemp = root;
	while (pTemp->left != NULL)
	{
		pTemp = pTemp->left;
	}
	return pTemp->key;
}
int TreeSet::last() {
	if (root == NULL) {
		throw "NoSuchElementException";
	}
	AVLNode* pTemp = root;
	while (pTemp->right != NULL)
	{
		pTemp = pTemp->right;
	}
	return pTemp->key;
}
void findHigher(AVLNode* root, int val, int &minGreatVal)
{
	if (root == NULL)
	{
		return;
	}
	AVLNode* pTemp = root;
	AVLNode* pPre = NULL;
	while (pTemp != NULL)
	{
		if (pTemp->key > val)
		{
			pPre = pTemp;
			pTemp = pTemp->left;
		}
		else
		{
			pTemp = pTemp->right;
		}
	}
	if (pPre != NULL)
	{
		minGreatVal = pPre->key;
	}
}
int TreeSet::higher(int val) {
	int minGreatVal = -1;
	findHigher(root, val, minGreatVal);
	return minGreatVal;

}
void findLower(AVLNode* root, int val, int &maxLessVal)
{
	if (root == NULL)
	{
		return;
	}
	AVLNode* pTemp = root;
	AVLNode* pPre = NULL;
	while (pTemp != NULL)
	{
		if (pTemp->key < val)
		{
			pPre = pTemp;
			pTemp = pTemp->right;
		}
		else
		{
			pTemp = pTemp->left;
		}
	}
	if (pPre != NULL)
	{
		maxLessVal = pPre->key;
	}
}
int TreeSet::lower(int val)
{
	int maxLessVal = -1;
	findLower(root, val, maxLessVal);
	return maxLessVal;
}
int TreeSet::remove(int val) {
	int check = 0;
	root = deleteNode(root, val, check);
	if (check == 1)
	{
		count -= 1;
	}
	return check;
}
void findSubset(AVLNode* root, AVLNode* &newRoot, int fromVal, int toVal, int &count)
{
	if (root == NULL)
	{
		return;
	}
	else if (root->key >= fromVal && root->key < toVal)
	{
		//AVLNode* pNode = new AVLNode(root->key);
		bool taller = false;
		int check = 0;
		newRoot = insert(newRoot, root->key, check);
		count += 1;
	}
	findSubset(root->left, newRoot, fromVal, toVal, count);
	findSubset(root->right, newRoot, fromVal, toVal, count);
}
TreeSet* TreeSet::subSet(int fromVal, int toVal) {
	TreeSet* newSet = new TreeSet();
	int count = 0;
	findSubset(root, newSet->root, fromVal, toVal, count);
	newSet->count = count;
	return newSet;
}
int TreeSet::size() {
	return count;
}

