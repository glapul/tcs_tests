#include "WeirdNumber.h"
#include <iostream>

int main()
{
  WeirdNumber x(2);
  WeirdNumber y(1,-2);
  std::cout << "x = " << x << "\ny = " << y << "\n";
  std::cout << x++ + y + x << "\n";
  std::cout << x << "\n";
  std::cout << WeirdNumber(0,1) + WeirdNumber(1) - WeirdNumber(1,-1)*WeirdNumber(1,1) << "\n";
  return 0;
}