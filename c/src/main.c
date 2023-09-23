#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TOKENS 200
#define MAX_NUM_LENGTH 200

void printError(char *message) { printf("ERROR: %s\n", message); }

typedef enum {
  TokenType_number,       // 0
  TokenType_plus,         // 1
  TokenType_minus,        // 2
  TokenType_star,         // 3
  TokenType_slash,        // 4
  TokenType_openBracket,  // 5
  TokenType_closeBracket, // 6
} TokenType;

typedef struct {
  TokenType type;
  union {
    char c;
    double f;
  } value;
} Token;

typedef enum {
  ExpressionNodeType_leaf,
  ExpressionNodeType_binary,
} ExpressionNodeType;

typedef enum {
  BinaryOperatorNodeType_addition,
  BinaryOperatorNodeType_subtraction,
  BinaryOperatorNodeType_multiplication,
  BinaryOperatorNodeType_division,
} BinaryOperatorNodeType;

struct ExpressionNode;
typedef struct {
  BinaryOperatorNodeType type;
  struct ExpressionNode *left;
  struct ExpressionNode *right;
} BinaryOperatorNode;

typedef struct {
  double value;
} LeafNode;

typedef struct ExpressionNode {
  ExpressionNodeType type;
  union {
    BinaryOperatorNode b;
    LeafNode l;
  } node;
} ExpressionNode;

ExpressionNode *parseExpression(Token *, int);
// return NULL on error
ExpressionNode *parseValue(Token *tokens, int tokenCount) {
  printf("parseValue:\n");
  printf("\ttokenCount: %d\n", tokenCount);
  printf("\tfirstToken: type=%d, value=%f\n", tokens->type, tokens->value.f);
  Token debugLastToken = *(tokens + tokenCount - 1);
  printf("\tlastToken: type=%d, value=%f\n", debugLastToken.type,
         debugLastToken.value.f);
  if (tokenCount == 0) {
    printError("parseValue tokenCount zero");
    return NULL;
  }
  if (tokenCount == 1) {
    printf("returned value=%f\n", tokens->value.f);
    Token token = *tokens;
    if (token.type == TokenType_number) {
      ExpressionNode *result = malloc(sizeof(ExpressionNode));
      result->type = ExpressionNodeType_leaf;
      result->node.l = (LeafNode){.value = token.value.f};
      return result;
    }
    printError("parseValue single token not a number");
    return NULL;
  }

  Token firstToken = *tokens;
  if (firstToken.type != TokenType_openBracket) {
    printError("parseValue multiple tokens not starting with open bracket");
    return NULL;
  }
  Token lastToken = *(tokens + tokenCount - 1);
  if (lastToken.type != TokenType_closeBracket) {
    printError("parseValue multiple tokens not ending with close bracket");
    return NULL;
  }

  return parseExpression(tokens + 1, tokenCount - 2);
}

// return NULL on error
ExpressionNode *parseFactor(Token *tokens, int tokenCount) {
  printf("parseFactor:\n");
  printf("\tfirstToken: type=%d, value=%f\n", tokens->type, tokens->value.f);
  Token debugLastToken = *(tokens + tokenCount - 1);
  printf("\tlastToken: type=%d, value=%f\n", debugLastToken.type,
         debugLastToken.value.f);
  // iterate back through tokens, looking for * or /
  for (int i = 0; i < tokenCount; i++) {
    Token token = *(tokens + tokenCount - 1 - i);
    // skip over contents of brackets
    int unaccountedCloseBracketCount = 0;
    do {
      if (token.type == TokenType_closeBracket) {
        unaccountedCloseBracketCount++;
      } else if (token.type == TokenType_openBracket) {
        unaccountedCloseBracketCount--;
      }
      if (unaccountedCloseBracketCount > 0) {
        i++;
        token = *(tokens + tokenCount - 1 - i);
      }
    } while (unaccountedCloseBracketCount > 0);
    if (token.type == TokenType_star) {
      ExpressionNode *result = malloc(sizeof(ExpressionNode));
      result->type = ExpressionNodeType_binary;
      result->node.b =
          (BinaryOperatorNode){.type = BinaryOperatorNodeType_multiplication,
                               .left = parseFactor(tokens, tokenCount - i - 1),
                               .right = parseValue(tokens + tokenCount - i, i)};
      return result;
    }
    if (token.type == TokenType_slash) {
      ExpressionNode *result = malloc(sizeof(ExpressionNode));
      result->type = ExpressionNodeType_binary;
      result->node.b =
          (BinaryOperatorNode){.type = BinaryOperatorNodeType_division,
                               .left = parseFactor(tokens, tokenCount - i - 1),
                               .right = parseValue(tokens + tokenCount - i, i)};
      return result;
    }
  }
  return parseValue(tokens, tokenCount);
}

// return NULL on error
ExpressionNode *parseExpression(Token *tokens, int tokenCount) {
  printf("parseExpression:\n");
  printf("\tfirstToken: type=%d, value=%f\n", tokens->type, tokens->value.f);
  Token debugLastToken = *(tokens + tokenCount - 1);
  printf("\tlastToken: type=%d, value=%f\n", debugLastToken.type,
         debugLastToken.value.f);
  // iterate back through tokens, looking for + or -
  for (int i = 0; i < tokenCount; i++) {
    Token token = *(tokens + tokenCount - 1 - i);
    // skip over contents of brackets
    int unaccountedCloseBracketCount = 0;
    do {
      if (token.type == TokenType_closeBracket) {
        unaccountedCloseBracketCount++;
      } else if (token.type == TokenType_openBracket) {
        unaccountedCloseBracketCount--;
      }
      if (unaccountedCloseBracketCount > 0) {
        i++;
        token = *(tokens + tokenCount - 1 - i);
      }
    } while (unaccountedCloseBracketCount > 0);
    if (token.type == TokenType_plus) {
      ExpressionNode *result = malloc(sizeof(ExpressionNode));
      result->type = ExpressionNodeType_binary;
      result->node.b = (BinaryOperatorNode){
          .type = BinaryOperatorNodeType_addition,
          .left = parseExpression(tokens, tokenCount - i - 1),
          .right = parseFactor(tokens + tokenCount - i, i)};
      return result;
    }
    if (token.type == TokenType_minus) {
      ExpressionNode *result = malloc(sizeof(ExpressionNode));
      result->type = ExpressionNodeType_binary;
      result->node.b = (BinaryOperatorNode){
          .type = BinaryOperatorNodeType_subtraction,
          .left = parseExpression(tokens, tokenCount - i - 1),
          .right = parseFactor(tokens + tokenCount - i, i)};
      return result;
    }
  }
  return parseFactor(tokens, tokenCount);
}

double evaluate(ExpressionNode *node) {
  switch (node->type) {
  case ExpressionNodeType_leaf:
    return node->node.l.value;
  case ExpressionNodeType_binary: {
    double left = evaluate(node->node.b.left);
    double right = evaluate(node->node.b.right);
    switch (node->node.b.type) {
    case BinaryOperatorNodeType_addition:
      return left + right;
    case BinaryOperatorNodeType_subtraction:
      return left - right;
    case BinaryOperatorNodeType_multiplication:
      return left * right;
    case BinaryOperatorNodeType_division:
      return left / right;
    }
  }
  }
}

// return number of tokens on success, return -1 on error
int getTokens(char *input, Token *output) {
  int tokenCount = 0;
  while (*input) {
    while (isspace(*input)) {
      input++;
    }
    Token token;
    if (*input == '+') {
      token = (Token){.type = TokenType_plus, .value = '+'};
      input++;
    } else if (*input == '-') {
      token = (Token){.type = TokenType_minus, .value = '-'};
      input++;
    } else if (*input == '*') {
      token = (Token){.type = TokenType_star, .value = '*'};
      input++;
    } else if (*input == '/') {
      token = (Token){.type = TokenType_slash, .value = '/'};
      input++;
    } else if (*input == '(') {
      token = (Token){.type = TokenType_openBracket, .value = '('};
      input++;
    } else if (*input == ')') {
      token = (Token){.type = TokenType_closeBracket, .value = ')'};
      input++;
    } else if (isdigit(*input) || *input == '.') {
      char numberBuffer[MAX_NUM_LENGTH];
      int numCount = 0;
      numberBuffer[numCount++] = *input++;
      while (isdigit(*input) || *input == '.') {
        numberBuffer[numCount++] = *input++;
      }
      numberBuffer[numCount] = '\0';
      token = (Token){.type = TokenType_number, .value.f = atof(numberBuffer)};
    } else {
      printError("Illegal character");
      return -1;
    }
    output[tokenCount++] = token;
  }
  return tokenCount;
}

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

  // debug printing
  printf("Start of tokens:\n");
  for (int i = 0; i < tokenCount; i++) {
    printf("%d. type=%d, value.f=%f\n", i, (tokens + i)->type,
           (tokens + i)->value.f);
  }
  printf("End of tokens\n");

  // Parsing
  ExpressionNode *rootNode = parseExpression(tokens, tokenCount);

  // Evaluating
  double result = evaluate(rootNode);
  printf("Result = %f", result);
  return 0;

error:
  return 1;
}
