#include "../s21_matrix.h"

int sum_and_sub(int operation, matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = OK;
  if (result == NULL || !is_matrix_correct(A) || !is_matrix_correct(B)) {
    error = INCORRECT_MATRIX;
  }
  if (!error) {
    if (!is_eq_sizes(A, B)) {
      error = CALCULATION_ERROR;
    }
  }
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
    if (!error) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j] * operation;
        }
      }
    }
  }
  if (error) s21_remove_matrix(result);
  return error;
}

double calculate_matrix_element(int pos_i, int pos_j, matrix_t *A,
                                matrix_t *B) {
  double value = 0;
  for (int k = 0; k < A->columns; k++) {
    value += A->matrix[pos_i][k] * B->matrix[k][pos_j];
  }
  return value;
}

double calculate_minor(int pos_i, int pos_j, matrix_t *A, double *result) {
  matrix_t B = {0};
  int error = s21_create_matrix(A->rows - 1, A->columns - 1, &B);
  if (!error) {
    for (int i = 0, counter_i = 0; i < A->rows; i++) {
      if (i != pos_i) {
        for (int j = 0, counter_j = 0; j < A->columns; j++) {
          if (j != pos_j) {
            B.matrix[counter_i][counter_j] = A->matrix[i][j];
            counter_j++;
          }
        }
        counter_i++;
      }
    }
    error = s21_determinant(&B, result);
  }
  s21_remove_matrix(&B);
  return error;
}

int is_matrix_correct(matrix_t *A) {
  int error = SUCCESS;
  if (A == NULL || A->matrix == NULL || A->columns < 1 || A->rows < 1) {
    error = FAILURE;
  } else {
    for (int i = 0; i < A->rows && error; i++) {
      error = (A->matrix[i] == NULL) ? FAILURE : SUCCESS;
    }
  }
  return error;
}

int is_eq_sizes(matrix_t *A, matrix_t *B) {
  return (A->columns == B->columns && A->rows == B->rows) ? SUCCESS : FAILURE;
}

int is_opposite_sizes(matrix_t *A, matrix_t *B) {
  return (A->columns == B->rows && A->rows == B->columns) ? SUCCESS : FAILURE;
}

int is_square_matrix(int rows, int columns) {
  return (rows == columns) ? SUCCESS : FAILURE;
}