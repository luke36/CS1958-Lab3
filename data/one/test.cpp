#include "vector.hpp"
#include <iostream>
using namespace sjtu;

int main() {
  vector<int> v;
  for (int i = 0; i < 10; i++)
    v.push_back(i);
  for (auto i = v.cbegin(); i != v.cend(); i++)
    std::cout << *i << std::endl;
}