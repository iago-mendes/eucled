#include <iostream>

#include "../algorithms/embedding.h"
#include "../classes/dyad.h"
#include "../classes/grid.h"
#include "../classes/metric.h"
#include "../functions/commutator.h"

int main() {
  std::string test_case;
  std::cin >> test_case;

  std::cout << "Test case: " << test_case << std::endl;
  
  if (test_case == "XPeanut") {
    double s0;
    std::cin >> s0;
    std::cout << "s0 = " << s0 << std::endl;
  }
}
