#include <iostream>
#include <fstream>
#include <utility>

using namespace std;
using direction = enum {UP, DOWN, RIGHT, LEFT};

class Solver {
   using Puzzle = unsigned short int[3][3];
   using Coordinate = pair<unsigned short int, unsigned short int>;

   public:
      Puzzle board;
      Coordinate blankPos;
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
            locateBlank();
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
         cout << "Puzzle Board:" << endl;
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               cout << puzzle[i][j] << " ";
            }
            cout << endl;
         }
      }

      inline void move(const direction& direction) {
         int x = blankPos.first;
         int y = blankPos.second;

         switch (direction) {
         case UP:
            if (x < 2) {
               board[x][y] = board[x+1][y];
               board[x+1][y] = 0;
            }
            break;
         case RIGHT:
            if (y > 0) {
               board[x][y] = board[x][y-1];
               board[x][y-1] = 0;
            }
            break;
         case DOWN:
            if (x > 0) {
               board[x][y] = board[x-1][y];
               board[x-1][y] = 0;
            }
            break;
         case LEFT:
            if (y < 2) {
               board[x][y] = board[x][y+1];
               board[x][y+1] = 0;
            }
            break;
         default:
            break;
         }
         
         locateBlank();
      }

      inline void locateBlank() {
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (board[i][j] == 0) {
                  blankPos.first = i;
                  blankPos.second = j;
               }
            }
         }
      }

      void printBlankPos() const {
         cout << "(" << blankPos.first << ", " << blankPos.second << ")" << endl;
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

   //solver.move(UP);
   solver.move(DOWN);
   solver.move(DOWN);
   solver.move(LEFT);

   solver.printBlankPos();
   solver.printPuzzle(solver.board);
}