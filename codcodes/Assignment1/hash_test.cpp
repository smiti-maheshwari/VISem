// set_union example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

int main () {
  std::vector<std::vector<int>> src = ...;
  std::set<int> all;

  for(int i = 0; i < src.size(); i++) {
    all.insert(src[i].begin(), src[i].end());
  }

std::vector<int> result(all.begin(), all.end());

  return 0;
}