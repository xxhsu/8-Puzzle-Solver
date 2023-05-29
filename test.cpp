#include <iostream>
#include <fstream>

using namespace std;

int main() {
   string puzzle;

   ifstream infile;
   infile.open("puzzle.txt");

   cout << "Loading puzzle..." << endl;
   infile >> puzzle;
   cout << puzzle << endl;

   infile.close();

   return 0;
}