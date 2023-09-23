#include "calc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printError("Expected a single string argument");
    goto error;
  }
  char *input = argv[1];

  Token tokens[MAX_TOKENS];
  int tokenCount = getTokens(input, tokens);
  if (tokenCount == -1)
    goto error;

  ExpressionNode *rootNode = parseExpression(tokens, tokenCount);

  printf("%f\n", evaluate(rootNode));
  return 0;

error:
  return 1;
}
