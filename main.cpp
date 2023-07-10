#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>
#include <unordered_map>

using namespace std;

class Solver {
	using Puzzle = vector<vector <unsigned short>>;
	using PuzzleSize = enum {THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10};
	using PuzzlePieces = set<unsigned short, greater<unsigned short>>;
	using PuzzleSet = vector<Puzzle>;
	using Coordinate = struct {
		unsigned short x, y;
	};

	// Priority queue stores puzzle
	using PuzzlePriorityQueue = struct {
		using PQElements = pair<int, Puzzle>;
		priority_queue<PQElements, vector<PQElements>, greater<PQElements>> elems;

		inline bool empty() const {
			return elems.empty();
		}
		inline void put(int priority, Puzzle puzzle) {
			elems.emplace(priority, puzzle);
		}
		inline Puzzle get() {
			Puzzle puzzle = elems.top().second;
			elems.pop();
			return puzzle;
		}
	};

	// Struct stores puzzle solving infomation
	using PuzzleBook = struct {
		unordered_map<double, Puzzle> ids; // Puzzle id database. KEY: puzzle id, VALUE: puzzle
		unordered_map<double, double> trace; // Moved trace. KEY: puzzle id, VALUE: last step puzzle id
		unordered_map<unsigned short, unsigned short> costFromStart; // Cost from start. KEY: puzzle id, VALUE: cost count
	};

	private:
		PuzzleBook puzzleBook;
		PuzzlePieces puzzlePieces;

		void setPuzzlePieces() {
			unsigned short puzzleSize = getPuzzleSize();
			for (int i = 0; i < puzzleSize*puzzleSize; i++) {
				puzzlePieces.insert(i);
			}
			unsigned short blankPiece = *puzzlePieces.begin();
			puzzlePieces.erase(puzzlePieces.begin());
			puzzlePieces.insert(blankPiece);
		}

		void setGoal() {
			Puzzle tempPuzzle;
			vector<unsigned short> col;
			unsigned short puzzleSize = getPuzzleSize();
			unsigned short piece;

			for (int i = 0; i < puzzleSize*puzzleSize; i++) {
				//piece = puzzlePieces.find(i); // todo
				//col.push_back(piece);
			}

			for (int i = 0; i < puzzleSize; i++) {
				for (int j = 0; j < puzzleSize; j++) {
					int d = i*j;
					//unsigned short a = puzzlePieces.find(d);
					//col.push_back(a);
				}
				tempPuzzle.push_back(col);
				col.clear();
			}
			goal = tempPuzzle;
		}

		inline unsigned short getPuzzleSize() const {
			return static_cast<unsigned short>(puzzleSize);
		}

		bool validatePuzzle(const Puzzle &puzzle) const {
			if (puzzle.size() < 3) {
				return false;
			}
			unsigned short puzzleSize = getPuzzleSize();
			unsigned short pieceNumberCount[puzzleSize*puzzleSize] = {0};
			for (int i = 0; i < puzzleSize; i++) {
			   for (int j = 0; j < puzzleSize; j++) {
			    	if (puzzle[i][j] < 0 && puzzle[i][j] > (puzzleSize*puzzleSize - 1)) {
			    		return false;
			    	}
			    	pieceNumberCount[puzzle[i][j]]++;
			    	if (pieceNumberCount[puzzle[i][j]] > 1) {
						cout << puzzle[i][j] << ": " << pieceNumberCount[puzzle[i][j]] << endl;
			    		return false;
			    	}
			   }
			}
			return true;
		}

		bool validatePiece(unsigned short puzzlePiece) {
			if (puzzlePieces.find(puzzlePiece) == puzzlePieces.end()) {
				return false;
			} else {
				return true;
			}
		}

	public:
		PuzzleSize puzzleSize = FOUR;
		Puzzle start;
		Puzzle goal {
			{1, 2, 3},
			{4, 5, 6},
			{7, 8, 0}
		};

		Solver() {
			setPuzzlePieces();
			//setGoal();
		}

		inline Coordinate locatePiece(
			const Puzzle &puzzle,
			unsigned short int puzzlePiece) {
			Coordinate piecePos;
			unsigned short size = getPuzzleSize();
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (puzzle[i][j] == puzzlePiece) {
						piecePos.x = i;
						piecePos.y = j;
					}
				}
			}
			return piecePos;
		}

		inline PuzzleSet neighbors(const Puzzle &puzzle) {
			PuzzleSet puzzleSet;
			Puzzle tempPuzzle = puzzle;
			Coordinate blankPos = locatePiece(puzzle, 0);
			
			unsigned short puzzleSize = getPuzzleSize();
			unsigned short x = blankPos.x;
			unsigned short y = blankPos.y;

			if (x < puzzleSize - 1) {
			   tempPuzzle[x][y] = tempPuzzle[x+1][y];
			   tempPuzzle[x+1][y] = 0;
			   puzzleSet.push_back(tempPuzzle);
			   tempPuzzle = puzzle;
			}
			if (y > 0) {
			   tempPuzzle[x][y] = tempPuzzle[x][y-1];
			   tempPuzzle[x][y-1] = 0;
			   puzzleSet.push_back(tempPuzzle);
			   tempPuzzle = puzzle;
			}
			if (x > 0) {
			   tempPuzzle[x][y] = tempPuzzle[x-1][y];
			   tempPuzzle[x-1][y] = 0;
			   puzzleSet.push_back(tempPuzzle);
			   tempPuzzle = puzzle;
			}
			if (y < puzzleSize - 1) {
			   tempPuzzle[x][y] = tempPuzzle[x][y+1];
			   tempPuzzle[x][y+1] = 0;
			   puzzleSet.push_back(tempPuzzle);
			   tempPuzzle = puzzle;
			}
			return puzzleSet;
		}

		inline unsigned short heuristic(const Puzzle &puzzle) {
			unsigned short total = 0;
			unsigned short puzzlePiece;
			unsigned short puzzleSize = getPuzzleSize();
			for (int i = 0; i < puzzleSize*puzzleSize; i++) {
				puzzlePiece = i;
				Coordinate currentPos = locatePiece(puzzle, puzzlePiece);
				Coordinate goalPos = locatePiece(goal, puzzlePiece);
				total += manhattanDistance(currentPos, goalPos);
			}
			return total;
		}

		inline unsigned short manhattanDistance(
			const Coordinate &from,
			const Coordinate &to) const {
			return abs(from.x - to.x) + abs(from.y - to.y);
		}
		
		inline double findPuzzleIdInBook(const Puzzle &puzzle) {
			double id = -1;
			for (int i = 0; i < puzzleBook.ids.size(); i++) {
				if (puzzleBook.ids[i] == puzzle) {
					id = i;
					break;
				}
			}
			return id;
		}

		void printResult() {
			double currentId = findPuzzleIdInBook(goal);

			if (currentId == -1) {
				cout << "Puzzle unsolved" << endl;
				return;
			}

			vector<string> stepTexts;
			
			do {
				Puzzle currentPuzzle = puzzleBook.ids[currentId];
				Puzzle lastPuzzle = puzzleBook.ids[puzzleBook.trace[currentId]];
				Coordinate fromBlankPos = locatePiece(lastPuzzle, 0);
				Coordinate toBlankPos = locatePiece(currentPuzzle, 0);
				string directionText = stepText(fromBlankPos, toBlankPos);

				stepTexts.push_back(directionText);
				currentId = puzzleBook.trace[currentId];
			} while (puzzleBook.trace[currentId] != currentId);

			reverse(stepTexts.begin(), stepTexts.end());

			cout << "Number of possible steps: " << puzzleBook.ids.size() - 1 << endl;
			cout << "Number of actual steps: " << stepTexts.size() << endl;
			cout << "Steps: ";

			for (auto text : stepTexts) {
				cout << text << " ";
			}
			cout << endl;
		}

		string stepText(const Coordinate &from, const Coordinate &to) const {
			if (to.x == from.x) {
				if (to.y - from.y == 1) {
					return "L";
				}
				if (to.y - from.y == -1) {
					return "R";
				}
			}
			if (to.y == from.y) {
				if (to.x - from.x == 1) {
					return "U";
				}
				if (to.x - from.x == -1) {
					return "D";
				}
			}
			return "NULL";
		}

		void aStarSearch() {
			if (!validatePuzzle(start)) {
				cerr << "Invalid puzzle" << endl;
				return;
			}

			PuzzlePriorityQueue frontier;
			unsigned short priority = 0;
			unsigned short currentCost;
			unsigned short newCost;
			double pid = 0;

			frontier.put(priority, start);
			puzzleBook.ids[pid] = start;
			puzzleBook.trace[pid] = pid;
			puzzleBook.costFromStart[pid] = 0;

			while (!frontier.empty()) {
				Puzzle current = frontier.get();

				if (current == goal) {
					break;
				}

				double currentId = findPuzzleIdInBook(current);

				for (Puzzle next : neighbors(current)) {
					double nextId = findPuzzleIdInBook(next);
					if (nextId == -1) {
						pid++;
						puzzleBook.ids[pid] = next;
						nextId = pid;
					}
					
					currentCost = puzzleBook.costFromStart[currentId];
					newCost = currentCost + 1;

					if (puzzleBook.costFromStart.find(nextId) == puzzleBook.costFromStart.end() // id not found
						|| newCost < puzzleBook.costFromStart[nextId]) {
						puzzleBook.costFromStart[nextId] = newCost;
						priority = newCost + heuristic(next);
						frontier.put(priority, next);
						puzzleBook.trace[nextId] = currentId;
					}
				}
			}
		}
};

int main() {
	auto clockStart = chrono::high_resolution_clock::now();

	Solver solver;

	solver.start = {
		{3, 0, 1, 13},
		{11, 12, 7, 4},
		{8, 5, 2, 14},
		{9, 6, 10, 15}
	};
	solver.goal = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 0}
	};
	// {5, 4, 8},
	// {6, 2, 0},
	// {3, 7, 1}
	solver.aStarSearch();

	auto clockEnd = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = clockEnd - clockStart;
	cout << "Time spent: " << elapsed.count() << " sec" << endl;

	solver.printResult();
}
