#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void printPuzzle(char puzzle[3][3]) {
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         cout << puzzle[i][j];
      }
      cout << endl;
   }
}

int main() {
   char puzzle[3][3];

   ifstream infile;
   infile.open(".puzzle");

   if (infile.is_open()) {
      string line;
      cout << "Loading puzzle..." << endl;

      for (int i = 0; i < 3; i++) {
         getline(infile, line);
         for (int j = 0; j < 3; j++) {
            puzzle[i][j] = line[j];
         }
      }
      infile.close();
      printPuzzle(puzzle);
   } else {
      cout << "Puzzle file not found" << endl;
   }
}