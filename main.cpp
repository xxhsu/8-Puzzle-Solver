#include <iostream>
#include <fstream>

using namespace std;

class Puzzle {
   public:
      int board[3][3];
      bool validity = false;
      
      Puzzle() {
         loadPuzzleFromFile();
         validatePuzzle();
      }

      int validatePuzzle() {
         int digitCount[9] = {0};
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (board[i][j] < 0 && board[i][j] > 8) {
                  return 0;
               }
               digitCount[board[i][j]]++;
               if (digitCount[board[i][j]] > 1) {
                  return 0;
               }
            }
         }
         validity = true;
         return 0;
      }

      void printPuzzle() {
         cout << "Puzzle:" << endl;
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               cout << board[i][j];
            }
            cout << endl;
         }
      }

   private:
      void loadPuzzleFromFile() {
         ifstream infile;
         infile.open(".puzzle");
         if (infile.is_open()) {
            string line;
            for (int i = 0; i < 3; i++) {
               getline(infile, line);
               for (int j = 0; j < 3; j++) {
                  board[i][j] = line[j]-'0';
               }
            }
            infile.close();
         } else {
            cout << "Puzzle file not found" << endl;
         }
      }
};

int main() {
   Puzzle puzzle;

   if (!puzzle.validity) {
      cout << "Invalid puzzle" << endl;
      return 0;
   }
   puzzle.printPuzzle();
}