#include <iostream>
#include <fstream>
#include <utility>

using namespace std;
using direction = enum {Up, Down, Right, Left};

class Solver {
   using Puzzle = unsigned short int[3][3];
   using Coordinate = pair<unsigned short int, unsigned short int>;

   public:
      Puzzle board;
      Coordinate zeroPos;
      bool validity = false;

      int loadPuzzleFromFile(string path) {
         ifstream infile;
         infile.open(path);
         if (infile.is_open()) {
            string line;        
            int puzzleWidth;
            for (int i = 0; i < 3; i++) {
               getline(infile, line);
               if (i == 0) {
                  puzzleWidth = line.size();       
               } else if (puzzleWidth != line.size()) {
                  cerr << "Puzzle file invalid" << endl;
                  return 0;
               }
               for (int j = 0; j < 3; j++) {
                  board[i][j] = line[j]-'0';
               }
            }
            infile.close();
            validateBoard();
         } else {
            cerr << "Puzzle file not found" << endl;
         }
         return 0;
      }

      void crashIfBoardIsInvalid() {
         if (!validity) {
            cout << "Error: Invalid puzzle" << endl;
            exit(0);
         }
      }

      void printPuzzle(Puzzle puzzle) const {
         cout << "Board:" << endl;
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               cout << puzzle[i][j] << " ";
            }
            cout << endl;
         }
      }

      inline void move(direction direction) {
         locateZero();

         int x = zeroPos.first;
         int y = zeroPos.second;

         switch (direction) {
         case Up:
            if (x < 2) {
               board[x][y] = board[x+1][y];
               board[x+1][y] = 0;
            }
            break;
         case Right:
            if (y > 0) {
               board[x][y] = board[x][y-1];
               board[x][y-1] = 0;
            }
            break;
         case Down:
            if (x > 0) {
               board[x][y] = board[x-1][y];
               board[x-1][y] = 0;
            }
            break;
         case Left:
            if (y < 2) {
               board[x][y] = board[x][y+1];
               board[x][y+1] = 0;
            }
            break;
         default:
            break;
         }
      }

      inline void locateZero() {
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (board[i][j] == 0) {
                  zeroPos.first = i;
                  zeroPos.second = j;
               }
            }
         }
      }
   private:
      int validateBoard() {
         int pieceNumberCount[9] = {0};
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (board[i][j] < 0 && board[i][j] > 8) {
                  return 0;
               }
               pieceNumberCount[board[i][j]]++;
               if (pieceNumberCount[board[i][j]] > 1) {
                  return 0;
               }
            }
         }
         validity = true;
         return 0;
      }
};

int main() {
   Solver solver;
   
   solver.loadPuzzleFromFile(".puzzle");
   solver.crashIfBoardIsInvalid();

   solver.move(Up);
   solver.move(Down);
   solver.move(Down);
   solver.move(Left);

   solver.printPuzzle(solver.board);
}