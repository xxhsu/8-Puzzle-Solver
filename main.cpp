#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using Direction = enum {UP, DOWN, RIGHT, LEFT};

class Solver {
	using Puzzle = vector<vector <unsigned short int>>;
	using PuzzleSet = vector<Puzzle>;
	using PuzzlePiece = enum {ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8};
	using Coordinate = pair<unsigned short int, unsigned short int>;

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
		unordered_map<unsigned short, unsigned short> costSoFar; // Cost from start. KEY: puzzle id, VALUE: cost count

		inline double findId(const Puzzle &puzzle) {
			double id = -1;
			for (int i = 0; i < ids.size(); i++) {
				if (ids[i] == puzzle) {
					id = i;
					break;
				}
			}
			return id;
		}
	};

	public:
		PuzzleBook puzzleBook;
		Puzzle start;
		Puzzle goal {
			{1, 2, 3},
			{4, 5, 6},
			{7, 8, 0}
		};

		int loadPuzzleFromFile(string path) {
			ifstream infile;
			infile.open(path);
			if (infile.is_open()) {
				string line;
				unsigned short int puzzleWidth;
				Puzzle tempPuzzle;
				vector<unsigned short int> col;
				for (int i = 0; i < 3; i++) {
				 	getline(infile, line);
				 	if (i == 0) {
				 		puzzleWidth = line.size();       
				 	} else if (puzzleWidth != line.size()) {
				 		cerr << "Puzzle file invalid" << endl;
				 		return 0;
				 	}
				 	for (int j = 0; j < 3; j++) {
				 		col.push_back(line[j]-'0');
				 	}
					tempPuzzle.push_back(col);
					col.clear();
				}
				start = tempPuzzle;
				infile.close();
				if (!validate(start)) {
					cerr << "Invalid puzzle" << endl;
				};
			} else {
			   cerr << "Puzzle file not found" << endl;
			}
			return 0;
		}

		int validate(const Puzzle &puzzle) const {
			if (!puzzle.size()) {
				return 0;
			}
			int pieceNumberCount[9] = {0};
			for (int i = 0; i < 3; i++) {
			   for (int j = 0; j < 3; j++) {
			      if (puzzle[i][j] < 0 && puzzle[i][j] > 8) {
			         return 0;
			      }
			      pieceNumberCount[puzzle[i][j]]++;
			      if (pieceNumberCount[puzzle[i][j]] > 1) {
			         return 0;
			      }
			   }
			}
			return 1;
		}

		inline Coordinate locatePiece(
			const Puzzle &puzzle,
			const PuzzlePiece &puzzlePiece) const {
			Coordinate piecePos;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (puzzle[i][j] == puzzlePiece) {
						piecePos.first = i;
						piecePos.second = j;
			    	}
				}
			}
			return piecePos;
		}

		void printPuzzle(Puzzle puzzle) const {
			cout << "Puzzle Board:" << endl;
			for (int i = 0; i < 3; i++) {
			   for (int j = 0; j < 3; j++) {
			      cout << puzzle[i][j] << " ";
			   }
			   cout << endl;
			}
		}

		inline PuzzleSet neighbors(const Puzzle &puzzle) {
			PuzzleSet puzzleSet;
			Puzzle tempPuzzle = puzzle;
			Coordinate blankPos = locatePiece(puzzle, ZERO);
			
			int x = blankPos.first;
			int y = blankPos.second;

			if (x < 2) {
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
			if (y < 2) {
			   tempPuzzle[x][y] = tempPuzzle[x][y+1];
			   tempPuzzle[x][y+1] = 0;
			   puzzleSet.push_back(tempPuzzle);
			   tempPuzzle = puzzle;
			}
			return puzzleSet;
		}

		inline unsigned short int heuristic(const Puzzle &puzzle) const {
			unsigned short int total = 0;
			for (int i = ZERO; i < EIGHT; i++) {
    			PuzzlePiece pieceNum = static_cast<PuzzlePiece>(i);
				Coordinate currentPos = locatePiece(puzzle, pieceNum);
				Coordinate goalPos = locatePiece(goal, pieceNum);
				total += manhattanDistance(currentPos, goalPos);
			}
			return 0;
		}

		inline unsigned short int manhattanDistance(
			const Coordinate &from,
			const Coordinate &to) const {
			return abs(from.first - to.first) + abs(from.second - to.second);
		}

		void printSteps() {
			double currentId = puzzleBook.findId(goal);
			vector<string> stepTexts;

			do {
				Puzzle currentPuzzle = puzzleBook.ids[currentId];
				Puzzle lastPuzzle = puzzleBook.ids[puzzleBook.trace[currentId]];
				Coordinate fromBlankPos = locatePiece(lastPuzzle, ZERO);
				Coordinate toBlankPos = locatePiece(currentPuzzle, ZERO);
				string directionText = stepText(fromBlankPos, toBlankPos);
				stepTexts.push_back(directionText);
				currentId = puzzleBook.trace[currentId];
			} while (puzzleBook.trace[currentId] != currentId);

    		reverse(stepTexts.begin(), stepTexts.end());

			for (auto i : stepTexts) {
				cout << i << " ";
			}
			cout << endl;
		}

		string stepText(const Coordinate &from, const Coordinate &to) const {
			if (to.first == from.first) {
				if (to.second - from.second == 1) {
					return "LEFT";
				}
				if (to.second - from.second == -1) {
					return "RIGHT";
				}
			}
			if (to.second == from.second) {
				if (to.first - from.first == 1) {
					return "UP";
				}
				if (to.first - from.first == -1) {
					return "DOWN";
				}
			}
			return "NULL";
		}

		int aStarSearch() {
			if (!validate(start)) {
				cerr << "Invalid puzzle" << endl;
				return -1;
			}

			PuzzlePriorityQueue frontier;
			double pid = 0;

			frontier.put(0, start);
			puzzleBook.ids[pid] = start;
			puzzleBook.trace[pid] = pid;
			puzzleBook.costSoFar[pid] = 0;

			while (!frontier.empty()) {
				Puzzle current = frontier.get();

				if (current == goal) {
					break;
				}

				double currentId = puzzleBook.findId(current);

				for (Puzzle next : neighbors(current)) {
					pid++;
					puzzleBook.ids[pid] = next;
					double nextId = puzzleBook.findId(next);
					unsigned short int currentCost = puzzleBook.costSoFar[currentId];
					unsigned short int newCost = currentCost + 1;

					if (puzzleBook.costSoFar.find(nextId) == puzzleBook.costSoFar.end()
						|| newCost < puzzleBook.costSoFar[nextId]) {
						puzzleBook.costSoFar[nextId] = newCost;
						unsigned short int priority = newCost + heuristic(next);
						frontier.put(priority, next);
						puzzleBook.trace[nextId] = currentId;
					}
				}
			}
			return 0;
		}
};

int main() {
    auto start = chrono::high_resolution_clock::now();

	Solver solver;
	
	solver.loadPuzzleFromFile(".puzzle");
	solver.aStarSearch();
	solver.printSteps();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
	cout << "Time taken by program: " << elapsed.count() << " seconds" << endl;
}