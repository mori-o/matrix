#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#define SUCCESS 1
#define FAILURE 0

#include "math.h"
#include "stdlib.h"

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

enum ERROR_CODES { OK = 0, INCORRECT_MATRIX = 1, CALCULATION_ERROR = 2 };

// s21_matrix
int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// s21_helpers
int sum_and_sub(int operation, matrix_t *A, matrix_t *B, matrix_t *result);
double calculate_matrix_element(int pos_i, int pos_j, matrix_t *A, matrix_t *B);
double calculate_minor(int pos_i, int pos_j, matrix_t *A, double *result);
int is_opposite_sizes(matrix_t *A, matrix_t *B);
int is_square_matrix(int rows, int columns);
int is_eq_sizes(matrix_t *A, matrix_t *B);
int is_matrix_correct(matrix_t *A);

#endif