#pragma once

#include <unordered_map>
#include <queue>
#include <vector>
#include <string>

using namespace std;

using Puzzle = vector<vector <unsigned short>>;
using PuzzleSet = vector<Puzzle>;
using PuzzlePiece = enum {ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8};

class Solver
{
	struct Coordinate { unsigned short x, y; };

	// Struct stores puzzle solving infomation
	struct PuzzleBook
	{
		unordered_map<double, Puzzle> ids; // Puzzle id database. KEY: puzzle id, VALUE: puzzle
		unordered_map<double, double> trace; // Moved trace. KEY: puzzle id, VALUE: last step puzzle id
		unordered_map<unsigned short, unsigned short> costFromStart; // Cost from start. KEY: puzzle id, VALUE: cost count
	};

	// Priority queue stores puzzle
	struct PuzzlePriorityQueue
	{
		using PQElements = pair<unsigned short, Puzzle>;
		priority_queue<PQElements, vector<PQElements>, greater<PQElements>> elems;

		inline bool empty() const
		{
			return elems.empty();
		}

		inline void put(unsigned short priority, Puzzle puzzle)
		{
			elems.emplace(priority, puzzle);
		}

		inline Puzzle get()
		{
			Puzzle puzzle = elems.top().second;
			elems.pop();
			return puzzle;
		}
	};

public:
	void loadPuzzleFromFile(const char *path);
	void aStarSearch();
	void printResult();

private:
	PuzzleBook puzzleBook;
	Puzzle start;
	Puzzle goal
	{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 0}
	};

	bool validate(const Puzzle &puzzle) const;
	inline Coordinate locatePiece(const Puzzle &puzzle, const PuzzlePiece &puzzlePiece) const;
	inline double findPuzzleIdInBook(const Puzzle &puzzle);
	inline unsigned short manhattanDistance(const Coordinate &from, const Coordinate &to) const;
	inline unsigned short heuristic(const Puzzle &puzzle) const;
	inline PuzzleSet neighbors(const Puzzle &puzzle);
	string stepText(const Coordinate &from, const Coordinate &to) const;
};
