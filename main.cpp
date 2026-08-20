#include "kwp/Vec2.hpp"

#include <iostream>
#include <ostream>

int main() {
  constexpr kwp::Vec2 vecRed{1, 2};
  std::cout << vecRed.x << std::endl;
  std::cout << vecRed.y << std::endl;

  return 0;
}
