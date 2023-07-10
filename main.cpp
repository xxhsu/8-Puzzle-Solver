#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unordered_map>

using namespace std;

class Solver {
	using Puzzle = vector<vector <unsigned short>>;
	using PuzzlePiece = enum {ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8};
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

	public:
		Puzzle start;
		Puzzle goal {
			{1, 2, 3},
			{4, 5, 6},
			{7, 8, 0}
		};

		void loadPuzzleFromFile(string path) {
			ifstream infile;
			infile.open(path);
			if (!infile.is_open()) {
				cerr << "Puzzle file not found" << endl;
				return;
			}
			
			string fileText;
			string line;
			while(getline(infile, line)) {
				fileText += line;
			}
			infile.close();

			string pieces;
			for (char c : fileText) {
				if (isdigit(c)) {
					pieces += c;
				}
			}

			if (pieces.size() != 9) {
				cerr << "Invalid puzzle file" << endl;
				return;
			}

			Puzzle tempPuzzle;
			vector<unsigned short> col;
			for (int i = 0; i < 9; i++) {
			 	col.push_back(pieces[i]-'0');
				if (i % 3 == 2) {
					tempPuzzle.push_back(col);
					col.clear();
				}
			}

			if (!validate(tempPuzzle)) {
				cerr << "Invalid puzzle file" << endl;
				return;
			}

			start = tempPuzzle;
		}

		void aStarSearch() {
			if (!validate(start)) {
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
				Coordinate fromBlankPos = locatePiece(lastPuzzle, ZERO);
				Coordinate toBlankPos = locatePiece(currentPuzzle, ZERO);
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

	private:
		PuzzleBook puzzleBook;

		bool validate(const Puzzle &puzzle) const {
			if (puzzle.size() < 3) {
				return false;
			}
			int pieceNumberCount[9] = {0};
			for (int i = 0; i < 3; i++) {
			   for (int j = 0; j < 3; j++) {
			      if (puzzle[i][j] < 0 && puzzle[i][j] > 8) {
			         return false;
			      }
			      pieceNumberCount[puzzle[i][j]]++;
			      if (pieceNumberCount[puzzle[i][j]] > 1) {
			         return false;
			      }
			   }
			}
			return true;
		}

		inline Coordinate locatePiece(
			const Puzzle &puzzle,
			const PuzzlePiece &puzzlePiece) const {
			Coordinate piecePos;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (puzzle[i][j] == puzzlePiece) {
						piecePos.x = i;
						piecePos.y = j;
					}
				}
			}
			return piecePos;
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

		inline unsigned short manhattanDistance(
			const Coordinate &from,
			const Coordinate &to) const {
			return abs(from.x - to.x) + abs(from.y - to.y);
		}

		inline unsigned short heuristic(const Puzzle &puzzle) const {
			unsigned short total = 0;
			for (int i = ZERO; i < EIGHT; i++) {
    			PuzzlePiece pieceNum = static_cast<PuzzlePiece>(i);
				Coordinate currentPos = locatePiece(puzzle, pieceNum);
				Coordinate goalPos = locatePiece(goal, pieceNum);
				total += manhattanDistance(currentPos, goalPos);
			}
			return total;
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

		inline PuzzleSet neighbors(const Puzzle &puzzle) {
			PuzzleSet puzzleSet;
			Puzzle tempPuzzle = puzzle;
			Coordinate blankPos = locatePiece(puzzle, ZERO);
			
			unsigned short x = blankPos.x;
			unsigned short y = blankPos.y;

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
};

int main() {
	auto start = chrono::high_resolution_clock::now();

	Solver solver;
	solver.loadPuzzleFromFile(".puzzle");
	solver.aStarSearch();

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = end - start;
	cout << "Time spent: " << elapsed.count() << " sec" << endl;

	solver.printResult();
}
