#include <iostream>
#include <cstdlib>
#include "infer.hpp"

using namespace std;

int main(int argc, char* args[]) {
  cout << "addNum(10, 20) + mulNum(10, 20): " << add_mul(10, 20) << endl;
  return 0;
}
