#define MAX_TOKENS 200
#define MAX_NUM_LENGTH 200

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

void printError(char *);
int getTokens(char *, Token *);
ExpressionNode *parseExpression(Token *, int);
ExpressionNode *parseFactor(Token *, int);
ExpressionNode *parseValue(Token *, int);
double evaluate(ExpressionNode *);
double twoPassEvaluate(char *);
