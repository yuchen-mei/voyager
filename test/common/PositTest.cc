#include "src/PositTypes.h"
#include "systemc.h"

int sc_main(int argc, char* argv[]) {
  float f = 0.1;
  Posit<8, 1, 8, 16> p(f);

  std::cout << p << std::endl;
}