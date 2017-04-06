// Using BTree implementation from here: http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// Template class separation workarounds described here: https://www.codeproject.com/Articles/48575/How-to-define-a-template-class-in-a-h-file-and-imp
// For time computation we're using chrono library: http://en.cppreference.com/w/cpp/chrono

#define HUNDRED_MILLION 100000000

#include <iostream>
#include <string>
#include <chrono>
#include <set>
#include <cstdlib> 
#include <memory>

#include "BTree.h"
#include "BTree.cpp" // To fix template class separation

#include "BTreeNode.h"
#include "BTreeNode.cpp" // To fix template class separation

#include "RedBlackTreeNode.h"

#include "RedBlackTree.h"
#include "RedBlackTree.cpp" // To fix template class separation

using namespace std;
using namespace std::chrono;
using namespace BTreeLib;

// Globals:
time_point<high_resolution_clock> start, finish; // (C++11 Style) Chrono start and end time points.
int* integers;
int bestT = 100; // Has been checked previously

// Prototypes:
void generateAndFillRandomIntegersFromRange(int from, int to);
void testAndDetermineBestT();
void checkInsertion();
void checkSearch();
void checkDelection();

// MAIN
int main()
{
	generateAndFillRandomIntegersFromRange(-1000, 1000); // Generate random integers (from range)
	
	//testAndDetermineBestT(); // Testing t parameter (10, 100, 1000) for BTree (1m, 10m, 100m)	
	
	checkInsertion(); // Insersion tests
	checkSearch(); // Search tests
	checkDelection(); // Deletion tests
	
	cout << "All the test have finished." << endl << endl;
	return 0;
}

void generateAndFillRandomIntegersFromRange(int from, int to) 
{
	integers = new int[HUNDRED_MILLION];
	for (size_t i = 0; i < HUNDRED_MILLION; i++)
		integers[i] = from + (rand() % (int)(to - from + 1));
}

void testAndDetermineBestT() 
{
	int TsAndTimes[3][2]; // { t, insert_time }

	cout << "Testing T parameter for BTree (accoding to insertion speed only):" << endl;
	for (unsigned int t = 10, j = 0; t <= 1000; t *= 10, j++)
	{
		TsAndTimes[j][0] = t;
		TsAndTimes[j][1] = 0;

		BTree<int> tmpBTree(t);
		for (unsigned int i = 1; i <= 100; i *= 10)
		{
			int size = 1000000 * i;
			cout << "Size: " << size << endl;

			start = high_resolution_clock::now();
			for (unsigned int j = 0; j < size; j++)
				tmpBTree.insert(integers[j]);
			finish = high_resolution_clock::now();
			int millis = duration_cast<milliseconds>(finish - start).count();
			TsAndTimes[j][1] += millis;
		}
		cout << endl;
	}

	// deleteme
	for (size_t i = 0; i < 3; i++) cout << "Insert BTree -> t: " << TsAndTimes[i][0] << ", Milliseconds: " << TsAndTimes[i][1] << endl;

	// Determining the best t parameter (insert + delete times have to be lowest)
	int minimalIndex = 0;
	for (size_t i = 0; i < 3; i++)
		if (TsAndTimes[i][1] < TsAndTimes[minimalIndex][1])
			minimalIndex = i;
	
	bestT = TsAndTimes[minimalIndex][0];
	cout << "Optimal T is: " << bestT << endl;
}

void checkInsertion()
{
	cout << "Testing insertions:" << endl;
	for (unsigned int size = 1000000; size <= HUNDRED_MILLION; size *= 10)
	{
		// Create nodes for red-black tree
		RedBlackTreeNode<int>** nodes = new RedBlackTreeNode<int>*[size];
		for (unsigned int j = 0; j < size; j++)
			nodes[j] = new RedBlackTreeNode<int>(integers[j]);

		// BTree (insert)
		BTree<int> tmpBTree(bestT);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.insert(integers[j]);
		finish = high_resolution_clock::now();
		cout << "Insert BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (insert)
		RedBlackTree<int> tmpRedBlackTree;
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpRedBlackTree.insert(nodes[j]);
		finish = high_resolution_clock::now();
		cout << "Insert RedBlackTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// There is no garbage collectors here!
		/*for (unsigned int j = 0; j < size; j++)
			delete nodes[j];
		delete[] nodes;*/
	}
}

void checkSearch() 
{
	cout << "Testing search:" << endl;
	for (unsigned int size = 1000000; size <= HUNDRED_MILLION; size *= 10)
	{
		// BTree (search)
		BTree<int> tmpBTree(bestT);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.search(integers[j]); // But this finds the whole node right?
		finish = high_resolution_clock::now();
		cout << "Search BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (search)
		RedBlackTree<int> tmpRedBlackTree;
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpRedBlackTree.search(integers[j]); // Finds by key!
		finish = high_resolution_clock::now();
		cout << "Search RedBlackTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;
	}
}

void checkDelection() 
{
	cout << "Testing deletion:" << endl;
	for (unsigned int size = 1000000; size <= HUNDRED_MILLION; size *= 10)
	{
		// Create nodes for red-black tree
		RedBlackTreeNode<int>** nodes = new RedBlackTreeNode<int>*[size];
		for (unsigned int j = 0; j < size; j++)
			nodes[j] = new RedBlackTreeNode<int>(integers[j]);

		// BTree (delete)
		BTree<int> tmpBTree(bestT);
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpBTree.remove(integers[j]); // But this finds the whole node right?
		finish = high_resolution_clock::now();
		cout << "Search BTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// RedBlackTree (delete)
		RedBlackTree<int> tmpRedBlackTree;
		start = high_resolution_clock::now();
		for (unsigned int j = 0; j < size; j++)
			tmpRedBlackTree.remove(nodes[j]); // Finds by key!
		finish = high_resolution_clock::now();
		cout << "Search RedBlackTree " << size << ", Milliseconds: " << duration_cast<milliseconds>(finish - start).count() << endl;

		// There is no garbage collectors here!
		/*for (unsigned int j = 0; j < size; j++)
			delete nodes[j];
		delete[] nodes;*/
	}
}