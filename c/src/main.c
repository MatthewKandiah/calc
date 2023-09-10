#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_COUNT 200
#define MAX_NUMBER_CHAR_COUNT 200

typedef enum {
  invalid,
  plus,
  minus,
  star,
  slash,
  number,
} TokenType;

TokenType charToTokenType(char input) {
  switch (input) {
  case '+':
    return plus;
  case '-':
    return minus;
  case '*':
    return star;
  case '/':
    return slash;
  }
  printf("ERROR: invalid input %c in charToTokenType\n", input);
  return -1;
}

typedef struct {
  TokenType type;
  double value;
} Token;

typedef struct {
  Token *tokens;
  int count;
} TokenList;

// return next token and increment input pointer one-past-end of corresponding
// string characters "consumes" string characters for generated token
Token getNextToken(char **input) {
  Token result;
  char numberBuffer[MAX_NUMBER_CHAR_COUNT];
  int numberCharCount;
  switch (**input) {
  case '+':
  case '-':
  case '*':
  case '/':
    result = (Token){.type = charToTokenType(**input), .value = 0};
    (*input)++;
    return result;

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    memset(numberBuffer, 0, sizeof(numberBuffer));
    numberCharCount = 0;
    while (**input == '0' || **input == '1' || **input == '2' ||
           **input == '3' || **input == '4' || **input == '5' ||
           **input == '6' || **input == '7' || **input == '8' ||
           **input == '9' || **input == '.') {
      numberBuffer[numberCharCount] = **input;
      numberCharCount++;
      (*input)++;
    }
    numberBuffer[numberCharCount] = 0;
    result = (Token) { .type = number, .value = strtod(numberBuffer, 0) };
	return result;
  }

  // increment and return invalid, avoids spamming console and segfaulting on
  // bad inputs
  (*input)++;
  return (Token){.type = invalid, .value = -1};
}

TokenList getTokens(char *input) {
  Token tokenBuffer[MAX_TOKEN_COUNT];
  int tokenCount = 0;
  while (*input) {
    Token nextToken = getNextToken(&input);
    tokenBuffer[tokenCount] = nextToken;
    tokenCount++;
    if (tokenCount >= MAX_TOKEN_COUNT) {
      printf("WARNING: Token pointer buffer full\n");
    }
  }
  return (TokenList){.tokens = tokenBuffer, .count = tokenCount};
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("USAGE: expected single string argument\n\t$calc <string>\n");
    return 1;
  }

  char *inputString = argv[1];
  TokenList tokenList = getTokens(inputString);
  for (int i = 0; i < tokenList.count; i++) {
    printf("%d. %u %f\n", i, tokenList.tokens[i].type,
           tokenList.tokens[i].value);
  }
  return 0;
}
