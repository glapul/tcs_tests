#include "WeirdNumber.h"
#include <iostream>
#define print(x) std::cout << #x << " = " << x << std::endl;

void fn2(WeirdNumber num) {
  std::cout << "fn2: " << num << "\n";
}

int main()
{
  WeirdNumber x(2);
  WeirdNumber y(1,-2);
  print(x);
  print(y);
  print(x++ + y + x);
  print(x);
  print(x++ + y + x);
  print(x);
  x++;
  x++;
  WeirdNumber z = x + y;
  print(x);
  print(y);
  print(z);
  fn2(x++);
  std::cout << x << "\n";
  std::cout << WeirdNumber(0,1) + WeirdNumber(1) - WeirdNumber(1,-1) * WeirdNumber(1,1) << "\n";
  std::cout << WeirdNumber() << " "
            << WeirdNumber(1, 0) << " " 
            << WeirdNumber(-1, 0) << " " 
            << WeirdNumber(0, 1) << " "
            << WeirdNumber(0, -1) << " "
            << WeirdNumber(1, 1) << " "
            << WeirdNumber(1, -1) << " "
            << WeirdNumber(0, -5) << " "
            << WeirdNumber(0, 100) << "\n";
  return 0;
}