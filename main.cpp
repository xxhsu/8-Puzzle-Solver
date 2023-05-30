#include <iostream>
#include <fstream>

using namespace std;

class Puzzle {
   public:
      int board[3][3];
      int zeroPos[2];
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
            exit(0);
         }
      }

      void printBoard() {
         cout << "Puzzle Board:" << endl;
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               cout << board[i][j];
            }
            cout << endl;
         }
      }

      void move(int direction) {
         locateZero();
         switch (direction) {
         case 1:
            if (zeroPos[0] < 2) {
               board[zeroPos[0]][zeroPos[1]] = board[zeroPos[0]+1][zeroPos[1]];
               board[zeroPos[0]+1][zeroPos[1]] = 0;
            }
            break;
         case 2:
            if (zeroPos[1] > 0) {
               board[zeroPos[0]][zeroPos[1]] = board[zeroPos[0]][zeroPos[1]-1];
               board[zeroPos[0]][zeroPos[1]-1] = 0;
            }
            break;
         case 3:
            if (zeroPos[0] > 0) {
               board[zeroPos[0]][zeroPos[1]] = board[zeroPos[0]-1][zeroPos[1]];
               board[zeroPos[0]-1][zeroPos[1]] = 0;
            }
            break;
         case 4:
            if (zeroPos[1] < 2) {
               board[zeroPos[0]][zeroPos[1]] = board[zeroPos[0]][zeroPos[1]+1];
               board[zeroPos[0]][zeroPos[1]+1] = 0;
            }
            break;
         default:
            break;
         }
      }

      void locateZero() {
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (board[i][j] == 0) {
                  zeroPos[0] = i;
                  zeroPos[1] = j;
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
   Puzzle puzzle;
   
   puzzle.loadPuzzleFromFile(".puzzle");
   puzzle.crashIfBoardIsInvalid();

   puzzle.printBoard();
}