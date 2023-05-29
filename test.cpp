#include <iostream>
#include <fstream>

using namespace std;

int main() {
   string puzzle;

   ifstream infile;
   infile.open(".puzzle");

   cout << "Loading puzzle..." << endl;
   infile >> puzzle;
   cout << puzzle;

   infile.close();

   return 0;
}