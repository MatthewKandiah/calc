#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 200
#define MAX_INT_LENGTH 200

typedef enum {
  number,
  plus,
  minus,
  star,
  slash,
  end,
} TokenType;

typedef struct {
  TokenType type;
  float value;
} Token;

typedef struct ExpressionNode{
  float value;
  struct ExpressionNode *left;
  struct ExpressionNode *right;
}ExpressionNode;

ExpressionNode *parse_first_token(Token **currentToken) {
  if ((*currentToken)->type != number) {
    printf("ERROR: Expected first token to be a number\n");
    exit(1);
  }

  ExpressionNode *node_ptr = malloc(sizeof(ExpressionNode));
  node_ptr->value = (*currentToken)->value;
  node_ptr->left = 0;
  node_ptr->right = 0;
  (*currentToken)++;
  return node_ptr;
}

int token_type_to_precedence(TokenType type) {
  switch (type) {
    case plus:
      return 10;
    case minus:
      return 11;
    case star:
      return 20;
    case slash:
      return 21;
    case number:
      printf("ERROR: getting precedence for number token\n");
      exit(1);
    case end:
      return 0;
  }
  printf("ERROR: failed to get precedence\n");
  exit(1);
}

ExpressionNode *parse_binary_expression(Token **current_token, ExpressionNode *left) {
  Token first_token = **current_token;
  if (first_token.type == end) {
    return left;
  }
  if (first_token.type == number) {
    printf("ERROR: Expected operator, received number %f", first_token.value);
    exit(1);
  }
  int current_operator_precedence = token_type_to_precedence(first_token.type);
  (*current_token)++;

  Token second_token = **current_token;
  if (second_token.type != number) {
    printf("ERROR: Expected number after operator\n");
    exit(1);
  }
  ExpressionNode *right = malloc(sizeof(ExpressionNode));
  right->value = (*current_token)->value;
  right->left = 0;
  right->right = 0;
  (*current_token)++;

  Token peek_token = **current_token;
  int next_op_precedence = token_type_to_precedence(peek_token.type);
  ExpressionNode *binaryNode = malloc(sizeof(ExpressionNode));
  binaryNode->value = first_token.type;
  binaryNode->left = left;
  if (current_operator_precedence >= next_op_precedence) {
    binaryNode->right = right;
    return parse_binary_expression(current_token, binaryNode);
  } else {
    binaryNode->right = parse_binary_expression(current_token, right);
    return binaryNode;
  }
}

float evaluate(ExpressionNode *node) {
  if (node->left == 0 && node->right == 0){
    return node->value;
  }
  if (node->left == 0 || node->right == 0) {
    printf("ERROR: poorly formed node with only one child\n");
    exit(1);
  }
  if (node->value == plus) {
    return evaluate(node->left) + evaluate(node->right);
  }
  if (node->value == minus) {
    return evaluate(node->left) - evaluate(node->right);
  }
  if (node->value == star) {
    return evaluate(node->left) * evaluate(node->right);
  }
  if (node->value == slash) {
    return evaluate(node->left) / evaluate(node->right);
  }
  printf("ERROR: failed to evaluate node\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("USAGE: expected a single string argument\n");
    return 1;
  }

  char *input = argv[1];

  // tokenizing
  Token token_buffer[MAX_TOKENS];
  int token_count = 0;
  char number_character_buffer[MAX_INT_LENGTH];
  while (*input) {
    if (*input == ' ') {
      input++;
      continue;
    }
    int number_character_count = 0;
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
    } else if (*input == '*') {
      token_buffer[token_count] = (Token){.type = star, .value = 0};
      token_count++;
      input++;
      continue;
    } else if (*input == '/') {
      token_buffer[token_count] = (Token){.type = slash, .value = 0};
      token_count++;
      input++;
      continue;
    } else {
      while (*input == '0' || *input == '1' || *input == '2' || *input == '3' ||
             *input == '4' || *input == '5' || *input == '6' || *input == '7' ||
             *input == '8' || *input == '9' || *input == '.') {
        number_character_buffer[number_character_count] = *input;
        input++;
        number_character_count++;
      }
      if (number_character_count == 0) {
        printf("ERROR: Illegal character %c\n", *input);
        return -1;
      }
      number_character_buffer[number_character_count] = '\0';
      token_buffer[token_count] =
          (Token){.type = number, .value = atof(number_character_buffer)};
      token_count++;
      continue;
    }
  }
  token_buffer[token_count] = (Token){.type = end, .value = 0};
  token_count++;

  Token *current_token_ptr = &token_buffer[0];
  ExpressionNode *first_node = parse_first_token(&current_token_ptr);
  ExpressionNode *root_node = parse_binary_expression(&current_token_ptr, first_node);

  // evaluating
  float answer = evaluate(root_node);
  printf("%f\n", answer);
}
