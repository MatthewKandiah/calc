#include "calc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void printError(char *message) { printf("ERROR: %s\n", message); }
// return NULL on error
ExpressionNode *parseValue(Token *tokens, int tokenCount) {
  if (tokenCount == 0) {
    printError("parseValue tokenCount zero");
    return NULL;
  }
  if (tokenCount == 1) {
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
  printError("evaluate failed to evaluate node");
  // Don't think there's an easy way to signal in the return that something went
  // wrong Could pass in a pointer to an error flag if we need to handle this
  // more smartly later? Or pass in a pointer to where we will store the result
  // and return an error flag? I think both work
  return 0;
}

// return number of tokens on success, return -1 on error
int getTokens(char *input, Token *output) {
  int tokenCount = 0;
  while (*input) {
    while (isspace(*input)) {
      input++;
    }
    if (!(*input)) break;
    Token token;
    if (*input == '+') {
      token.type = TokenType_plus;
      input++;
    } else if (*input == '-') {
      token.type = TokenType_minus;
      input++;
    } else if (*input == '*') {
      token.type = TokenType_star;
      input++;
    } else if (*input == '/') {
      token.type = TokenType_slash;
      input++;
    } else if (*input == '(') {
      token.type = TokenType_openBracket;
      input++;
    } else if (*input == ')') {
      token.type = TokenType_closeBracket;
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

double twoPassEvaluate(char *input) {
  Token tokens[MAX_TOKENS];
  int tokenCount = getTokens(input, tokens);
  return evaluate(parseExpression(tokens, tokenCount));
};
