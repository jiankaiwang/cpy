#include <iostream>
#include <cstdlib>
#include "infer.hpp"

using namespace std;

int main(int argc, char* args[]) {
  int result = add_mul(40, 50);
  cout << "add_mul(40, 50): " << result << ", valid: " << (result == 2090) << endl;
  return 0;
}
