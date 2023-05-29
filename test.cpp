#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Puzzle {
   public:
      char puzzle[3][3];
      bool validity = false;
      
      Puzzle() {
         loadPuzzleFromFile();
         validatePuzzle();
      }

      int validatePuzzle() {
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (!puzzle[i][j]) {
                  return 0;
               }
            }
         }
         validity = true;
         return 0;
      }

      void printPuzzle() {
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               cout << puzzle[i][j];
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

            cout << "Puzzle:" << endl;

            for (int i = 0; i < 3; i++) {
               getline(infile, line);
               for (int j = 0; j < 3; j++) {
                  puzzle[i][j] = line[j];
               }
            }

            infile.close();
         } else {
            cerr << "Puzzle file not found" << endl;
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