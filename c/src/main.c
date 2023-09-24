#include "calc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printError("Expected a single string argument");
    return 1;
  }
  char *input = argv[1];
  printf("%f\n", twoPassEvaluate(input));
  return 0;
}
