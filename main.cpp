#include <iostream>
#include <string>
#include <chrono>
#include "solver.h"

using namespace std;

int main()
{
	auto start = chrono::high_resolution_clock::now();

	Solver solver;
	
	solver.loadPuzzleFromFile(".puzzle");
	solver.aStarSearch();

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;
	cout << "Time spent: " << elapsed.count() << " sec" << endl;

	solver.printResult();
}
