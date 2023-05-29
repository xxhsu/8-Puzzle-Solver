#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
   string puzzle;
   string line;

   ifstream infile;
   infile.open(".puzzle");

   if (infile.is_open()) {
      cout << "Loading puzzle..." << endl;
      while (getline(infile, line))
      {
         /* code */
      }
      infile.close();
   } else {
      cout << "Puzzle file not found" << endl;
   }
}