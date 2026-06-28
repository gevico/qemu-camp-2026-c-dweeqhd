#include <stdio.h>

static int add(int a, int b) { return a + b; }
static int subtract(int a, int b) { return a - b; }
static int multiply(int a, int b) { return a * b; }
static int divide_int(int a, int b) { return a / b; }

int main(void) {
    int a, b;
    char op;
    int (*operations[])(int, int) = {add, subtract, multiply, divide_int};
    char operators[] = "+-*/";

    if (scanf("%d %d %c", &a, &b, &op) != 3) {
        return 1;
    }

    int index = -1;
    for (int i = 0; i < 4; i++) {
        if (op == operators[i]) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        printf("error: invalid operator\n");
        return 0;
    }
    if (op == '/' && b == 0) {
        printf("error: division by zero\n");
        return 0;
    }

    printf("%d %c %d = %d\n", a, op, b, operations[index](a, b));
    return 0;
}
