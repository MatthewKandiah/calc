#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 200
#define MAX_INT_LENGTH 200

typedef enum {
  integer,
  plus,
  minus,
} TokenType;

typedef struct {
  TokenType type;
  int value;
} Token;

typedef enum {
  addition,
  subtraction,
}BinaryOperatorNodeType;

union ExpressionNode;

typedef struct {
  BinaryOperatorNodeType type;
  union ExpressionNode *left;
  union ExpressionNode *right;
}BinaryOperatorNode;

typedef struct {
  int value;
}LeafNode;

typedef union {
  BinaryOperatorNode binaryOp;
  LeafNode leaf;
}ExpressionNode;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("USAGE: expected a single string argument\n");
    return 1;
  }

  char *input = argv[1];

  // tokenizing
  Token token_buffer[MAX_TOKENS];
  int token_count = 0;
  char int_character_buffer[MAX_INT_LENGTH];
  while (*input) {
    if (*input == ' ') {
      input++;
      continue;
    }
    int int_character_count = 0;
    if (*input == '+') {
      token_buffer[token_count] = (Token){.type = plus, .value = 0};
      token_count++;
      input++;
      continue;
    } else if (*input == '-') {
      token_buffer[token_count] = (Token){.type = minus, .value = 0};
      token_count++;
      input++;
      continue;
    } else {
      while (*input == '0' || *input == '1' || *input == '2' || *input == '3' ||
             *input == '4' || *input == '5' || *input == '6' || *input == '7' ||
             *input == '8' || *input == '9') {
        int_character_buffer[int_character_count] = *input;
        input++;
        int_character_count++;
      }
      if (int_character_count == 0) {
        printf("ERROR: Illegal character %c\n", *input);
        return -1;
      }
      int_character_buffer[int_character_count] = '\0';
      token_buffer[token_count] =
          (Token){.type = integer, .value = atoi(int_character_buffer)};
      token_count++;
      continue;
    }
  }

  // parsing
  for (int i = 0; i < token_count; i++) {
    Token token = token_buffer[i];
    printf("type: %d, values; %d\n", token.type, token.value);
  }
}
