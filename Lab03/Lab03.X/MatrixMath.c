/*
 * File:   MatrixMath.c
 * Author: Vince
 *
 * Created on January 26, 2020, 11:58 PM
 */


#include "MatrixMath.h"
#include "stdbool.h"
#include "BOARD.h"
#include <stdlib.h>

//void main(void)
//{

void MatrixPrint(float mat[3][3])
{
    int c;
    int d;
    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            printf("%.2f\t", mat[c][d]);
        }
        printf("\n");
    }
}



/******************************************************************************
 * Matrix - Matrix Operations
 *****************************************************************************/

/**
 * MatrixEquals checks if the two matrix arguments are equal (to within FP_DELTA).
 * @param: mat1, pointer to a 3x3 float array
 * @param: mat2, pointer to a 3x3 float array
 * @return: TRUE if and only if every element of mat1 is within FP_DELTA of the corresponding element of mat2,  otherwise return FALSE
 * Neither mat1 nor mat2 is modified by this function.
 */
int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    int c;
    int d;
    float x = 0;

    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            x = mat1[c][d] - mat2[c][d];
            abs(x);
        }
    }
    if (FP_DELTA > x) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * MatrixAdd performs an element-wise matrix addition operation on two 3x3 matrices and 
 * "returns" the result by modifying the third argument.
 * @param: mat1, pointer to a summand 3x3 matrix
 * @param: mat2, pointer to a summand 3x3 matrix
 * @param: result, pointer to a 3x3 matrix that is modified to contain the sum of mat1 and mat2.
 * @return:  None
 * mat1 and mat2 are not modified by this function.  result is modified by this function.
 */
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int c;
    int d;

    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            result[c][d] = mat1[c][d] + mat2[c][d];
        }
    }
    return result[3][3];
}

/**
 * MatrixMultiply performs a matrix-matrix multiplication operation on two 3x3
 * matrices and "returns" the result by modifying the third argument.
 * @param: mat1, pointer to left factor 3x3 matrix
 * @param: mat2, pointer to right factor 3x3 matrix
 * @param: result, pointer to matrix that is modified to contain the matrix product of mat1 and mat2.
 * @return: none
 * mat1 and mat2 are not modified by this function.  result is modified by this function.
 */
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int c;
    int d;
    int e;
    int result1;

    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            for (e = 0; e < 3; e++) {
                result1 = result1 + (mat1[c][e] * mat2[e][d]);
            }
            result[c][d] = result1;
            result1 = 0;
        }

    }
    return result[3][3];
}

/******************************************************************************
 * Matrix - Scalar Operations
 *****************************************************************************/

/**
 * MatrixScalarAdd performs the addition of a matrix and a scalar.  Each element of the matrix is increased by x.
 * The result is "returned"by modifying the third argument.
 * @param: x, a scalar float
 * @param: mat, pointer to a 3x3 matrix
 * @param: result, pointer to matrix that is modified to contain mat + x.
 * @return: none
 * x and mat are not modified by this function.  result is modified by this function.
 */
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    int c;
    int d;

    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            result[c][d] = x + mat[c][d];
        }
    }
    return result[3][3];
}

/**
 * MatrixScalarAdd performs the multiplication of a matrix and a scalar.
 * Each element of the matrix is multiplied x.
 * The result is "returned"by modifying the third argument.
 * @param: x, a scalar float
 * @param: mat, pointer to a 3x3 matrix
 * @param: result, pointer to matrix that is modified to contain mat + x.
 * @return: none
 * x and mat are not modified by this function.  result is modified by this function.
 */
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    int c;
    int d;

    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            result[c][d] = x * mat[c][d];
        }
    }
    return result[3][3];
}

/******************************************************************************
 * Unary Matrix Operations
 *****************************************************************************/

/**
 * MatrixTrace calculates the trace of a 3x3 matrix.
 * @param: mat, a pointer to a 3x3 matrix
 * @return: the trace of mat
 */
float MatrixTrace(float mat[3][3])
{
    int c;
    float result = 0;
    for (c = 0; c < 3; c++) {
       result = result + mat[c][c];
    }
    return result;
}

/**
 * MatrixTranspose calculates the transpose of a matrix and "returns" the
 * result through the second argument.
 * @param: mat, pointer to a 3x3 matrix
 * @param: result, pointer to matrix that is modified to transpose of mat
 * mat is not modified by this function.  result is modified by this function.
 */
void MatrixTranspose(float mat[3][3], float result[3][3])
{
    int c;
    int d;

    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            result[d][c] = mat[c][d];
        }
    }
    return result[3][3];
}
/**
 * MatrixSubmatrix finds a submatrix of a 3x3 matrix that is 
 * formed by removing the i-th row and the j-th column.  The 
 * submatrix is "returned" by modifying the final argument.
 * 
 * @param: i, a row of the matrix, INDEXING FROM 0
 * @param: j, a column of the matrix, INDEXING FROM 0
 * @param: mat, a pointer to a 3x3 matrix
 * @param: result, a pointer to a 2x2 matrix
 * @return: none
 * 
 * mat is not modified by this function.  Result is modified by this function.
 */
//    void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]){
//        int c;
//        int d;
//        
//        for (c = 0; c < 3; c++){
//            for (d = 0; d <3; d++){
//                result [3][3] = mat[0][d] + mat [c][0];
//            }
//        }
//        return result[3][3];
//    }

/**
 * MatrixDeterminant calculates the determinant of a 3x3 matrix 
 * and returns the value as a float.
 * @param: mat, a pointer to a 3x3 matrix
 * @return: the determinant of mat
 * mat is not modified by this function.
 * */
float MatrixDeterminant(float mat[3][3])
{
    int c;
    float result;
    for (c = 0; c < 3; c++) {
        result = result + (mat[0][c]*(mat[1][(c + 1) % 3] * mat[2][c + 2 % 3] - mat[1][(c + 2) % 3]));
    }
    return result;
}

/* MatrixInverse calculates the inverse of a matrix and
 * "returns" the result by modifying the second argument.
 * @param: mat, a pointer to a 3x3 matrix
 * @param: result, a pointer to a 3x3 matrix that is modified to contain the inverse of mat
 * @return: none
 * mat is not modified by this function.  result is modified by this function.
 */
void MatrixInverse(float mat[3][3], float result[3][3])
{
    int c;
    int d;
    for (c = 0; c < 3; c++) {
        for (d = 0; d < 3; d++) {
            result[3][3] = mat[c][d] / d;
        }
    }
    return result[3][3];
}

