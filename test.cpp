#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Puzzle {
   public:
      char puzzle[3][3];
      bool puzzleValidity = false;
      
      Puzzle() {
         loadPuzzleFromFile();
         validatePuzzle();
      }

      void validatePuzzle() {
         puzzleValidity = true;
         for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
               if (!puzzle[i][j]) {
                  puzzleValidity = false;
                  break;
               }
            }
         }
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
         infile.open(".uzzle");

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
            exit(0);
         }
      }
};

int main() {
   Puzzle puzzle;
   puzzle.printPuzzle();
}