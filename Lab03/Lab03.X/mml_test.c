// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>

// User libraries:
#include "MatrixMath.h"

// Module-level variables:
float zero_matrix[3][3] = {
    {},
    {},
    {}
};
float x = 10;
float matrix_1[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float matrix_1a[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float matrix_1b[3][3] = {
    {11, 12, 13},
    {14, 15, 16},
    {17, 18, 19}
};
float matrix_1c[3][3] = {
    {1, 4, 7},
    {2, 3, 8},
    {3, 5, 9}
};
float matrix_1d[3][3] = {
    {1, 4, 7},
    {2, 5, 8},
    {3, 6, 9}
};
float matrix_2a[3][3] = {
    {19, 18, 17},
    {16, 15, 14},
    {13, 12, 11}
};
float matrix_2[3][3] = {
    {9, 8, 7},
    {6, 5, 4},
    {3, 2, 1}
};
float matrix_3[3][3] = {
    {51, 32, 45},
    {23, 75, 56},
    {1, 23, 7}
};

float matrix_4[3][3] = {
    {10, 10, 10},
    {10, 10, 10},
    {10, 10, 10}
};
float matrix_5[3][3] = {
    {30, 24, 18},
    {84, 69, 54},
    {138, 114, 90}
};
float matrix_6[3][3] = {
    {100, 251, 1178},
    {325, 641, 502},
    {550, 1031, 826}
};
float matrix_7[3][3] = {
    {10, 20, 30},
    {40, 50, 60},
    {70, 80, 90}
};
float matrix_8[3][3] = {
    {90, 80, 70},
    {60, 50, 40},
    {30, 20, 10}
};
float matrix_9[3][3] = {
    {1, 2, 3},
    {4, 3, 6},
    {7, 8, 9}
};
float matrix_9a[3][3] = {
    {-7 / 8, 1 / 4, 1 / 8},
    {1 / 4, -1 / 2, 1 / 4},
    {11 / 24, 1 / 4, -5 / 24}
};
float matrix_9b[3][3] = {
    {1, 2, 3},
    {4, 3, 6},
    {7, 8, 9}
};
float matrix_9c[3][3] = {
    {-13 / 18, 1 / 3, 1 / 18},
    {-1 / 18, -2 / 3, 7 / 18},
    {11 / 18, 1 / 3, -5 / 18}
};

float result[3][3];
float mat;
int c = 0;
int d = 0;

void main(void)
{
    BOARD_Init();

    printf("Beginning Vince's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);
    //Matrix Equal check
    if (MatrixEquals(matrix_1, matrix_1a) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    if (MatrixEquals(matrix_5, matrix_1) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixEqual()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;

    //Matrix Multiplication check
    MatrixMultiply(matrix_1, matrix_2, zero_matrix);
    if (MatrixEquals(result, matrix_5) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixMultiply(matrix_1, matrix_3, zero_matrix);
    if (MatrixEquals(result, matrix_6) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixMultiply()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //Matrix ScalarMultiply
    MatrixScalarMultiply(x, matrix_1, zero_matrix);
    if (MatrixEquals(result, matrix_7) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixScalarMultiply(x, matrix_2, zero_matrix);
    if (MatrixEquals(result, matrix_1b) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixScalarMultiply()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //    Matrix Determinant
    MatrixDeterminant(matrix_1);
    if (result == 0) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixDeterminant(matrix_3);
    if (result == -21843) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixDeterminant()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //Matrix Add
    MatrixAdd(matrix_2, matrix_1, zero_matrix);
    if (MatrixEquals(result, matrix_4) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixAdd(matrix_1, matrix_4, zero_matrix);
    if (MatrixEquals(result, matrix_8) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixAdd()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //Matrix Add
    MatrixScalarAdd(x, matrix_1, zero_matrix);
    if (MatrixEquals(result, matrix_1b) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixScalarAdd(x, matrix_2, zero_matrix);
    if (MatrixEquals(result, matrix_2a) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixScalarAdd()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //    Matrix Inverse
    MatrixInverse(matrix_9, zero_matrix);
    if (MatrixEquals(result, matrix_9a) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixInverse(matrix_9b, zero_matrix);
    if (MatrixEquals(result, matrix_9c) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixInverse()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //    Matrix Transpose
    MatrixTranspose(matrix_9b, zero_matrix);
    if (MatrixEquals(result, matrix_1c) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixTranspose(matrix_1, zero_matrix);
    if (MatrixEquals(result, matrix_1d) == 1) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixTranspose()\n", c);
    if (c == 2) {
        d += 1;
    }

    c = 0;
    //    Matrix trace
    float x = 0;
    int i = 133;
    int j = 15;
    MatrixTrace(matrix_1);
    if (result == j) {
        c += 1;
    } else {
        c += 0;
    }
    MatrixTrace(matrix_3);
    if (result == i) {
        c += 1;
    } else {
        c += 0;
    }
    printf("PASSED " "%d" "/ 2: MatrixTrace()\n", c);
    if (c == 2) {
        d += 1;
    }
    c = 0;
    printf("%d " "out of 9 functions passed ", d);
    float f = 0;
    f = 100 * (d / 9.0);
    printf("%.1f%%\n", f);
    printf("Output of MatrixPrint():\n");
    MatrixPrint(matrix_1);
    printf("Expected Output of MatrixPrint():\n");
    printf("1.00    2.00    3.00\n4.00    5.00    6.00\n7.00    8.00    9.00");
    while (1);

}

