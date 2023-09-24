#include "calc.h"
#include <stdio.h>
#include <string.h>

static int failureCount = 0;

static int testCount = 0;

void test(char *name, char *input, double expectedOutput) {
  printf("%s - ", name);
  double result = twoPassEvaluate(input);
  if (result == expectedOutput) {
    printf("SUCCESS\n");
  } else {
    printf("FAILURE: expected %f, received %f\n", expectedOutput, result);
    failureCount++;
  }
  testCount++;
}

void summariseResults() {
  printf("SUMMARY - %d / %d tests passed\n", testCount - failureCount, testCount);
}

int main(void) {
  test("single digit", "1", 1);
  test("addition", "1+2", 3);
  test("subtraction", "1-2", -1);
  test("multiplication", "2*3", 6);
  test("division", "4/2", 2);
  test("floating point", "0.5*3", 1.5);
  test("whitespace", "   1   +   1   ", 2);
  test("order of operations", "1-2*3+4/2-1", -4);
  test("ignore outer brackets", "(1+2+3)", 6);
  test("order of brackets", "(1-2)*(3+4)/(1-2)", 7);
  test("nested brackets", "((1+2) * (3-4*(5+6)+(7-8)))", -126);

  summariseResults();
}
