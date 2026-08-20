#include <doctest/doctest.h>

int factorial(int n) {
  if (n <= 1) {
    return 1;
  }
  return n * factorial(n - 1);
}

TEST_SUITE("factorial") {
  TEST_CASE("Factorial") { CHECK(factorial(1) == 1); }
}
