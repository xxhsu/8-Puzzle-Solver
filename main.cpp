#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <unordered_map>

using namespace std;
using Direction = enum {UP, DOWN, RIGHT, LEFT};

class Solver {
	using Puzzle = vector<vector <unsigned short int>>;
	using PuzzleSet = vector<Puzzle>;
	using Coordinate = pair<unsigned short int, unsigned short int>;
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

	public:
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

		inline Coordinate locateBlank(const Puzzle &puzzle) const {
			Coordinate blankPos;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (puzzle[i][j] == 0) {
						blankPos.first = i;
						blankPos.second = j;
			    	}
				}
			}
			return blankPos;
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
			Coordinate blankPos = locateBlank(puzzle);
			
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

		int aStarSearch() {
			if (!validate(start)) {
				cerr << "Invalid puzzle" << endl;
				return -1;
			}

			PuzzlePriorityQueue frontier;
			frontier.put(0, start);

			unordered_map<double, Puzzle> puzzleIds;
			puzzleIds[0] = start;
			
			unordered_map<double, double> cameFrom;
			cameFrom[0] = 0;

			unordered_map<unsigned short, unsigned short> costFromStart;
			costFromStart[0] = 0;

			while (!frontier.empty()) {
				Puzzle current = frontier.get();

				if (current == goal) {
					break;
				}
				
				puzzleIds.find(7);

				for (Puzzle next : neighbors(current)) {
					
				}
			}
			return 0;
		}
};

int main() {
	Solver solver;
	
	solver.loadPuzzleFromFile(".puzzle");
	solver.aStarSearch();
}