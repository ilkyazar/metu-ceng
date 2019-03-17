#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define MAX_SHANTING_STACK 100

int IS_DEBUGGING = 0;

enum tokenType {
  VARIABLE = 0,
  NUMBER,
  OPERATOR,
  FUNCTION,
  PARANTHESIS
};

enum functionType {
  UMINUS = 0,
  SIN,
  COS,
  SQRT,
  LN
};

enum associativity {
  LEFT = 0,
  RIGHT
};

struct token {
  /* generic fields */
  char * repr;
  int type;
  /* number/variable related */
  float value;
  /* operator/function related */
  int functionType;
  int precedence;
  int associativity;
};

/* globals */
int isVariableDefined[26];
char formula[200];
int countOfIntervals;
long double countOfExperiments;
int countOfVariables = 0;
struct token * tokens;
int tokensIndex = 0;
struct token * output;
int outputIndex = 0;
float* probabilitiesOfVariables[26];
float minValues[26];
float maxValues[26];

struct token shuntingStack[MAX_SHANTING_STACK];
int shuntingStackLength = 0;

struct token postfixStack[MAX_SHANTING_STACK];
int postfixStackLength = 0;

void push_shuntingStack(struct token token) {
  if(shuntingStackLength > MAX_SHANTING_STACK - 1) {
    if(IS_DEBUGGING) printf("ERROR: Shanting stack overflow\n");
    exit(-1);
  }
  shuntingStack[shuntingStackLength++] = token;
}

struct token pop_shuntingStack() {
  if(!shuntingStackLength) {
    if(IS_DEBUGGING) printf("ERROR: Shanting stack empty\n");
    exit(-1);
  }
  return shuntingStack[--shuntingStackLength];
}

void push_postfixStack(struct token token) {
  if(postfixStackLength > MAX_SHANTING_STACK - 1) {
    if(IS_DEBUGGING) printf("ERROR: Shanting stack overflow\n");
    exit(-1);
  }
  postfixStack[postfixStackLength++] = token;
}

struct token pop_postfixStack() {
  if(!postfixStackLength) {
    if(IS_DEBUGGING) printf("ERROR: Shanting stack empty\n");
    exit(-1);
  }
  return postfixStack[--postfixStackLength];
}

void clearPostFixStack() {
  /* it's not clearing but, if we stack with pop, push and variable of this stack, should be fine */
  postfixStackLength = 0;
}

void printTokens(struct token * tokens, int length) {
  int i;
  for (i = 0;i < length; i++) {
    if(IS_DEBUGGING) printf("%s, ", tokens[i].repr);
  }
  if(IS_DEBUGGING) printf("\n");
}

int variableIndex(char repr) {
  return repr - 65;
}

char charFromIndex(int index) {
  return index + 65;
}

void printVariables(struct token * tokens, int length) {
  int i, j;
  for (i = 0; i < length; i++) {
    if (tokens[i].type == VARIABLE) {
      char repr = tokens[i].repr[0];
      if(IS_DEBUGGING) printf("%c %f %f ", repr, minValues[variableIndex(repr)], maxValues[variableIndex(repr)]);
      for (j = 0; j < countOfIntervals; j++) {
        if(IS_DEBUGGING) printf("%f ", probabilitiesOfVariables[variableIndex(repr)][j]);
      }
      if(IS_DEBUGGING) printf("\n");
    }
  }
  if(IS_DEBUGGING) printf("\n");
}

int howManyDifferentVariables() {
  int i, count = 0;
  for(i = 0; i < 26; i++) {
    if (isVariableDefined[i]) {
      count++;
    }
  }
  return count;
}

int selectInterval(float probabilitiesOfVariables[]) {
  double random = (double) rand() / (double) RAND_MAX;
  double sub = 0;
  int i;

  for(i = 0; i < countOfIntervals; i++) {
    sub += probabilitiesOfVariables[i];
    if (sub >= random) {
      return i;
    }
  }
  return (countOfIntervals - 1);
}

float calculateIntervalDiff(float min, float max) {
  return (max - min) / (float)countOfIntervals;
}

void populateVariablesDatum() {
  int i, j;
  for (i = 0; i < countOfVariables; i++) {
    char variable;
    float min, max;

    scanf("%c %f %f", &variable, &min, &max);

    /* Set basic information for variables */
    minValues[variableIndex(variable)] = min;
    maxValues[variableIndex(variable)] = max;

    /* Set probability distribution of variable */
    probabilitiesOfVariables[variableIndex(variable)] = malloc(sizeof(float) * countOfIntervals);
    for (j = 0; j < countOfIntervals; j++) {
      /* If we are storing j.th probability value for 'C',
         we'd like to store it in array[2][j]
      */
      scanf("%f\n", probabilitiesOfVariables[variableIndex(variable)] + j);
    }
  }
}

void tokenize() {
  int i = 0;
  char current, next;
  tokensIndex = 0;
  while (i < strlen(formula)) {
    current = formula[i];
    next = formula[i + 1];
    if (current >= 'A' && current <= 'Z') {
      /* variable */
      struct token newToken;
      newToken.repr = malloc(sizeof(char));
      newToken.repr[0] = current;
      newToken.type = VARIABLE;
      tokens[tokensIndex++] = newToken;
      isVariableDefined[variableIndex(current)] = 1;
    } else if ((current >= 'a' && current <= 'z') || current == '~') {
      /* function */
      struct token newToken;
      if (current == 's') { /* sin, sqrt */
        if (next == 'i') {
          newToken.repr = "sin";
          newToken.type = FUNCTION;
          i += 2; /* skip sin */
        } else if (next == 'q') {
          newToken.repr = "sqrt";
          newToken.type = FUNCTION;
          i += 3; /* skip sqrt */
        } else {
          if(IS_DEBUGGING) printf("Unknown function specified\n");
          exit(-1);
        }
      } else if (current == 'l') { /* ln */
        newToken.repr = "ln";
        newToken.type = FUNCTION;
        i += 1; /* skip ln */
      } else if (current == 'c') { /* cos */
        newToken.repr = "cos";
        newToken.type = FUNCTION;
        i += 2; /* skip cos */
      } else if (current == '~') { /* cos */
        newToken.repr = "~";
        newToken.type = FUNCTION;
      } else {
        if(IS_DEBUGGING) printf("Unknown function specified\n");
        exit(-1);
      }
      tokens[tokensIndex++] = newToken;
    } else if (current == '(' || current == ')') {
      /* paranthesis */
      struct token newToken;
      newToken.repr = malloc(sizeof(char));
      newToken.repr[0] = current;
      newToken.type = PARANTHESIS;
      tokens[tokensIndex++] = newToken;
    } else if ((current >= '0' && current <= '9') || current == '.') {
      /* number */
      struct token newToken;
      char numberStr[200], nextPossibleNumber = next;
      int index = 0, k;
      numberStr[index] = current;
      /* read all number characters in below while */
      while((nextPossibleNumber >= '0' && nextPossibleNumber <= '9') || nextPossibleNumber == '.') {
        index++;
        nextPossibleNumber = formula[i+index+1];
        numberStr[index] = formula[i+index];
      }
      newToken.repr = malloc(sizeof(float) * (index + 1));
      for (k = 0; k < index + 1; k++) {
        newToken.repr[k] = numberStr[k];
      }
      newToken.type = NUMBER;
      newToken.value = atof(numberStr);
      tokens[tokensIndex++] = newToken;
      i += index; /* skip the amount we processed */
    } else {
      /* operator or irrelevant */
      if (current == ' ') {
        /* just continue */
      } else if (current == '\n' || current == '\0') {
        return;
      } else {
        /* operator */
        struct token newToken;
        newToken.repr = malloc(sizeof(char));
        newToken.repr[0] = current;
        switch (current) {
          case '^':
            newToken.precedence = 4;
            newToken.associativity = RIGHT;
            break;
          case '/':
          case '*':
            newToken.precedence = 3;
            newToken.associativity = LEFT;
            break;
          case '+':
          case '-':
            newToken.precedence = 2;
            newToken.associativity = LEFT;
            break;
        }
        newToken.type = OPERATOR;
        tokens[tokensIndex++] = newToken;
      }
    }
    i++;
  }
}

void shuntingYard() {
  struct token currentToken;
  int nextTokenIndex = 0;
  /* token index was populated by tokenize function */

  while(nextTokenIndex < tokensIndex) {
    currentToken = tokens[nextTokenIndex];
    if(IS_DEBUGGING) printf("Shunting token %s\n", currentToken.repr);
    if (currentToken.type == NUMBER || currentToken.type == VARIABLE) {
      if(IS_DEBUGGING) printf("\tadd it to output\n");
      output[outputIndex++] = currentToken;
    } else if (currentToken.type == FUNCTION) {
      if(IS_DEBUGGING) printf("\tadd it to stack\n");
      push_shuntingStack(currentToken);
    } else if (currentToken.type == OPERATOR) {
      struct token operatorOnStack;
      int checkedOperatorsOnStack = 0;
      while (shuntingStackLength - checkedOperatorsOnStack > 0) {
        if(IS_DEBUGGING) printf("\twhile there is token on stack\n");
        operatorOnStack = shuntingStack[shuntingStackLength - checkedOperatorsOnStack - 1];
        if (operatorOnStack.type != OPERATOR) {
          if(IS_DEBUGGING) printf("\tall operators on stack has been checked, break the loop\n");
          break;
        }

        if ((currentToken.associativity == LEFT && currentToken.precedence <= operatorOnStack.precedence) ||
            (currentToken.associativity == RIGHT && currentToken.precedence < operatorOnStack.precedence)) {
          if(IS_DEBUGGING) printf("\t\tpop from stack and put to output (%s)\n", operatorOnStack.repr);
          output[outputIndex++] = pop_shuntingStack();
          checkedOperatorsOnStack--;    /* poped from shunting stack, so arrange the index */
        }
        checkedOperatorsOnStack++;
      }
      if(IS_DEBUGGING) printf("\toperator checking finished, put current to stack\n");
      push_shuntingStack(currentToken);
    } else if (currentToken.type == PARANTHESIS && currentToken.repr[0] == '(') {
      if(IS_DEBUGGING) printf("\tpush it onto stack\n");
      push_shuntingStack(currentToken);
    } else if (currentToken.type == PARANTHESIS && currentToken.repr[0] == ')') {
      struct token operatorOnStack;
      while (shuntingStackLength > 0) {
        operatorOnStack = shuntingStack[shuntingStackLength - 1];
        if (operatorOnStack.repr[0] != '(') {
          if(IS_DEBUGGING) printf("\tpop to output until left paranthesis (%s)\n", operatorOnStack.repr);
          output[outputIndex++] = pop_shuntingStack();
        } else {
          /* we found left paranthesis here */
          pop_shuntingStack();
          if (shuntingStackLength > 0 && shuntingStack[shuntingStackLength - 1].type == FUNCTION) {
            if(IS_DEBUGGING) printf("\t\tthe token after left paranthesis is funcion, so pop it to output\n");
            output[outputIndex++] = pop_shuntingStack();
          }
          break;
        }
      }
    }
    if(IS_DEBUGGING) printf("shuntingStack: ");
    printTokens(shuntingStack, shuntingStackLength);
    if(IS_DEBUGGING) printf("output: ");
    printTokens(output, outputIndex);
    nextTokenIndex++;
    if(IS_DEBUGGING) printf("\n\n");
  }

  while(shuntingStackLength > 0) {
    output[outputIndex++] = pop_shuntingStack();
    if(IS_DEBUGGING) printf("push remaining operators to stack (%s)\n", output[outputIndex - 1].repr);
  }
  if(IS_DEBUGGING) printf("shuntingStack: ");
  printTokens(shuntingStack, shuntingStackLength);
  if(IS_DEBUGGING) printf("output: ");
  printTokens(output, outputIndex);
  if(IS_DEBUGGING) printf("\n\n");
}

float rangedRandom(float min, float max) {
  return (float)rand()/(float)RAND_MAX * (max - min) + min;
}

float valueOfNumberOrVariable(struct token token) {
  if (token.type == NUMBER) {
    return token.value;
  } else {
    char repr = token.repr[0];
    float min = minValues[variableIndex(repr)], max = maxValues[variableIndex(repr)];
    float * probabilityDistrubution = probabilitiesOfVariables[variableIndex(repr)];
    int intervalIndex = selectInterval(probabilityDistrubution);
    float intervalDiff = calculateIntervalDiff(min, max);
    float randMin = min + intervalIndex * intervalDiff;
    float randMax = randMin + intervalDiff;
    float randValue = rangedRandom(randMin, randMax);
    if(IS_DEBUGGING) printf("random value %f for %c is created\n", randValue, repr);
    return randValue;
  }
}

float postfix() {
  int inputIndex = 0;   /* input is now, queue populated in shunting method */
  float formulaResult;
  clearPostFixStack();

  while(inputIndex < outputIndex) {
    struct token currentInput = output[inputIndex];
    if(IS_DEBUGGING) printf("Postfixing token %s\n", currentInput.repr);
    if (currentInput.type == NUMBER || currentInput.type == VARIABLE) {
      if(IS_DEBUGGING) printf("\tpush to stack\n");
      push_postfixStack(currentInput);
    } else {
      /* either operator or function */
      struct token resultToken;
      float firstValue, secondValue, result;
      if (currentInput.type == OPERATOR) {
        /* 1. and 2. operand positions are converted in stack */
        secondValue = valueOfNumberOrVariable(pop_postfixStack());
        firstValue = valueOfNumberOrVariable(pop_postfixStack());
        switch(currentInput.repr[0]) {
          case '^':
            result = powf(firstValue, secondValue);
            break;
          case '/':
            result = firstValue / secondValue;
            break;
          case '*':
            result = firstValue * secondValue;
            break;
          case '+':
            result = firstValue + secondValue;
            break;
          case '-':
            result = firstValue - secondValue;
            break;
        }
      } else {
        firstValue = valueOfNumberOrVariable(pop_postfixStack());
        switch(currentInput.functionType) {
          case UMINUS:
            result = - firstValue;
            break;
          case SIN:
            result = sin(firstValue);
            break;
          case COS:
            result = cos(firstValue);
            break;
          case SQRT:
            result = sqrt(firstValue);
            break;
          case LN:
            result = log(firstValue);
            break;
        }
      }
      if(IS_DEBUGGING) printf("\nresult calculated for %s as %f\n", currentInput.repr, result);
      resultToken.repr = "POSTIF_CALCULATED_VALUE";
      resultToken.value = result;
      resultToken.type = NUMBER;
      push_postfixStack(resultToken);
    }
    if(IS_DEBUGGING) printf("postfixStack: ");
    printTokens(postfixStack, postfixStackLength);
    inputIndex++;
    if(IS_DEBUGGING) printf("\n\n");
  }
  formulaResult = pop_postfixStack().value;
  return formulaResult;
}

void draw() {
  int experimentMade, i;
  float min = FLT_MAX, max = FLT_MIN;
  float sampleEffect;
  float * probabilityDistrubutions = malloc(sizeof(float) * countOfIntervals);
  float initialDistributionValue = 1.0 / (float)countOfIntervals;
  for (i = 0; i < countOfIntervals; i++) {
    probabilityDistrubutions[i] = 0.0;
  }

  /* LET THE EXPERIMENT BEGIN!!! */
  sampleEffect = 1.0 / countOfExperiments;
  for (experimentMade = 0; experimentMade < countOfExperiments; experimentMade++) {
    float intervalDiff, intervalIndex;
    float sample = postfix();
    if (sample < min) min = sample;
    if (sample > max) max = sample;

    /* Poorly calculate which current interval it would fit */
    intervalDiff = (max - min) / (float)countOfIntervals;
    intervalIndex = fabsf(sample - min) / intervalDiff;
    intervalIndex = (int)intervalIndex;
    for (i = 0; i < countOfIntervals; i++) {
      if (intervalIndex == i) {
        probabilityDistrubutions[i] += sampleEffect;
      }
    }
  }

  /* results */
  printf("%f %f", min, max);
  for (i = 0; i < countOfIntervals; i++) {
    printf(" %f", probabilityDistrubutions[i]);
  }
  printf("\n");
}

int main() {
  srand(time(NULL));
  tokens = malloc(sizeof(struct token) * 200);
  output = malloc(sizeof(struct token) * 200);

  if (fgets(formula, 200, stdin) != NULL) {
    tokenize();
    if(IS_DEBUGGING) printf("\n\nFormula tokenized\n");
    printTokens(tokens, tokensIndex);
    shuntingYard();
  } else {
    if(IS_DEBUGGING) printf("Invalid input\n");
    return -1;
  }

  /* Populate common data to be used accross the program */
  countOfVariables = howManyDifferentVariables();
  scanf("%d %Lf\n", &countOfIntervals, &countOfExperiments);
  if(IS_DEBUGGING) printf("Common %d %d %Lf\n", countOfVariables, countOfIntervals, countOfExperiments);
  populateVariablesDatum();
  printVariables(tokens, tokensIndex);

  if (countOfExperiments == 0) {
    if(IS_DEBUGGING) printf("Invalid count of experiments\n");
    return -1;
  }
  if(IS_DEBUGGING) printf("result is %f\n", postfix());

  draw();

  return 0;
}
