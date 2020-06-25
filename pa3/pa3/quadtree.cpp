
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
#include <cmath>
#include <vector>
#include <numeric>
#include <stdio.h>

using namespace std;


// Node constructor, given.
quadtree::Node::Node(pair<int,int> ul, int d,RGBAPixel a, double v)
	:upLeft(ul),dim(d),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

// quadtree destructor, given.
quadtree::~quadtree() {
	clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree & other) {
	copy(other);
}
// quadtree assignment operator, given.
quadtree & quadtree::operator=(const quadtree & rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}


quadtree::quadtree(PNG & imIn) {
	unsigned int width = imIn.width();
	unsigned int height = imIn.height();
	int dim = width >= height ? (int) log2(width) : (int) log2(height);
	imIn.resize(pow(2,dim), pow(2,dim));
	stats s(imIn);
	pair<int,int> ul = make_pair(0,0);
	edge = pow(2,dim);
	root = buildTree(s, ul, dim);
}


quadtree::Node * quadtree::buildTree(stats & s, pair<int,int> & ul, int dim) {
	if (dim == 0) {
		return new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));
	}

	Node *node = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

	dim--;

	// Make ul coordinates of subsquares
	pair<int, int> topRight = make_pair(ul.first + pow(2, dim), ul.second);
	pair<int, int> lowerLeft = make_pair(ul.first, ul.second + pow(2,dim));
	pair<int, int> lowerRight = make_pair(ul.first + pow(2, dim), ul.second + pow(2, dim));

	node->NW = buildTree(s, ul, dim);
	node->NE = buildTree(s, topRight, dim);
	node->SW = buildTree(s, lowerLeft, dim);
	node->SE = buildTree(s, lowerRight, dim);
	
	return node;
}


void quadtree::renderRecurse(PNG &png, Node *subRoot)
{
	if (subRoot->SW == NULL)
	{
		int i = subRoot->upLeft.first;
		int j = subRoot->upLeft.second;
		for (int x = i; x < (i + pow(2, subRoot->dim)); x++)
		{
			for (int y = j; y < (j + pow(2, subRoot->dim)); y++)
			{
				*(png.getPixel(x, y)) = subRoot->avg;
			}
		}
		return;
	}

	renderRecurse(png, subRoot->NE);
	renderRecurse(png, subRoot->NW);
	renderRecurse(png, subRoot->SW);
	renderRecurse(png, subRoot->SE);
}

PNG quadtree::render() {
	PNG png((unsigned int) edge, (unsigned int) edge);
	renderRecurse(png, root);
	return png;
}

int quadtree::idealPrune(int leaves) {
	int tol = 0;
	int L = 0;
	int R = root->var;

	while (L <= R) {
		int M = L + (R-L) / 2;
		if (this->pruneSize(M) <= leaves) {
			tol = M;
			R = M - 1;
		} else if (this->pruneSize(M) > leaves) {
			L = M + 1;
		}
	}
	return tol;
}

int quadtree::pruneSizeRecurse(Node *&subRoot, int tol)
{
	if (subRoot == NULL)
	{
		return 0;
	}
	if (prunable(subRoot, tol))
	{
		return 1;
	}
	return pruneSizeRecurse(subRoot->NE, tol) +
		   pruneSizeRecurse(subRoot->NW, tol) +
		   pruneSizeRecurse(subRoot->SE, tol) +
		   pruneSizeRecurse(subRoot->SW, tol);
}

int quadtree::pruneSize(int tol)
{
	int prunesize = pruneSizeRecurse(root, tol);
	return prunesize;
}

void quadtree::pruneRecurse(Node* & subRoot, int tol) {
	if (prunable(subRoot, tol)) {
		clearRecurse(subRoot->NE);
		clearRecurse(subRoot->NW);
		clearRecurse(subRoot->SE);
		clearRecurse(subRoot->SW);
		return;
	}
	pruneRecurse(subRoot->NE, tol);
	pruneRecurse(subRoot->NW, tol);
	pruneRecurse(subRoot->SE, tol);
	pruneRecurse(subRoot->SW, tol);
}

void quadtree::prune(int tol)
{
	pruneRecurse(root, tol);
}

void quadtree::clearRecurse(Node *&subRoot)
{
	if (subRoot != NULL) {
		clearRecurse(subRoot->NE);
		clearRecurse(subRoot->NW);
		clearRecurse(subRoot->SE);
		clearRecurse(subRoot->SW);
	}
	delete subRoot;
	subRoot = NULL;
}

void quadtree::clear() {
	clearRecurse(root);
	delete root;
	root = NULL;
	edge = 0;
}

quadtree::Node *quadtree::copyRecurse(const Node *other)
{
	if (other == NULL) {
		return NULL;
	}

	Node* node = new Node(other->upLeft, other->dim, other->avg, other->var);
	node->NW = copyRecurse(other->NW);
	node->SE = copyRecurse(other->SE);
	node->NE = copyRecurse(other->NE);
	node->SW = copyRecurse(other->SW);
	return node;
}

void quadtree::copy(const quadtree &other)
{
	root = copyRecurse(other.root);
	edge = other.edge;
}
