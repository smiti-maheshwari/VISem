// basic file operations
#include <iostream>
#include <fstream>
using namespace std;
void readfile(const std::string &filepath,std::string &buffer);

void write_to_file(string filename, string result) {
  ofstream myfile;
  myfile.open(filename.c_str());
  myfile << result;
  myfile.close();
}


string read_from_file(string filename) {
  ifstream myfile;
  string result;

  myfile.open(filename.c_str());
  myfile >> result;
  myfile.close();
}

int main() {
  string filename = "follow_pos.txt";

  //cout << "yes";
  string result;

  readfile(filename, result);

  cout << result;
}


void readfile(const std::string &filepath,std::string &buffer){
    std::ifstream fin(filepath.c_str());
    getline(fin, buffer, char(-1));
    fin.close();
}