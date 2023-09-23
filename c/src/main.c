#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TOKENS 200
#define MAX_NUM_LENGTH 200

void printError(char *message) { printf("ERROR: %s\n", message); }

typedef enum {
  number,
  plus,
  minus,
  star,
  slash,
  openBracket,
  closeBracket,
  end,
} TokenType;

typedef struct {
  TokenType type;
  union {
    char c;
    float f;
  } value;
} Token;

void getTokens(char *input, Token *output) {
  int tokenCount = 0;
  while (*input) {
    while (isspace(*input)) {
      input++;
    }
    Token token;
    if (*input == '+') {
      token = (Token){.type = plus, .value = '+'};
      input++;
    } else if (*input == '-') {
      token = (Token){.type = minus, .value = '-'};
      input++;
    } else if (*input == '*') {
      token = (Token){.type = star, .value = '*'};
      input++;
    } else if (*input == '/') {
      token = (Token){.type = slash, .value = '/'};
      input++;
    } else if (*input == '(') {
      token = (Token){.type = openBracket, .value = '('};
      input++;
    } else if (*input == ')') {
      token = (Token){.type = closeBracket, .value = ')'};
      input++;
    } else if (isdigit(*input) || *input == '.') {
      char numberBuffer[MAX_NUM_LENGTH];
      int numCount = 0;
      numberBuffer[numCount++] = *input++;
      while (isdigit(*input) || *input == '.') {
        numberBuffer[numCount++] = *input++;
      }
      numberBuffer[numCount] = '\0';
      token = (Token){.type = number, .value.f = atof(numberBuffer)};
    } else {
      printError("Illegal character");
      exit(1);
    }
    output[tokenCount++] = token;
  }
  output[tokenCount++] = (Token){.type = end, .value = '\0'};
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printError("Expected a single string argument");
    goto error;
  }
  char *input = argv[1];

  Token tokens[MAX_TOKENS];
  getTokens(input, tokens);

  Token* tp = tokens;
  while(tp->type != end) {
    printf("%d\n", tp->type);
    tp++;
  }
  // Parsing

  // Evaluating
  return 0;

error:
  return 1;
}
