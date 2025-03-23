#include "../s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = OK;
  if (rows > 0 && columns > 0 && result != NULL) {
    result->matrix = (double **)malloc(rows * sizeof(double *));
    if (result->matrix == NULL) {
      free(result->matrix);
      error = INCORRECT_MATRIX;
    }
    for (int i = 0; i < rows && !error; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
      if (result->matrix[i] == NULL) {
        for (int j = 0; j < i; j++) {
          free(result->matrix[j]);
        }
        free(result->matrix);
        error = INCORRECT_MATRIX;
      }
    }
    if (!error) {
      result->rows = rows;
      result->columns = columns;
    }
  } else {
    error = INCORRECT_MATRIX;
  }
  return error;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    if (A->matrix != NULL) {
      for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
      }
      free(A->matrix);
    }
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int error = is_eq_sizes(A, B);
  error = (error && is_matrix_correct(A) && is_matrix_correct(B)) ? SUCCESS
                                                                  : FAILURE;
  for (int i = 0; i < A->rows && error; i++) {
    for (int j = 0; j < A->columns && error; j++) {
      error = (A->matrix[i][j] != B->matrix[i][j]) ? FAILURE : SUCCESS;
    }
  }
  return error;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return sum_and_sub(1, A, B, result);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return sum_and_sub(-1, A, B, result);
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int is_correct = (result != NULL && is_matrix_correct(A)) ? SUCCESS : FAILURE;
  int error = is_correct ? OK : INCORRECT_MATRIX;
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
    if (!error) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
        }
      }
    }
  }
  if (error) s21_remove_matrix(result);
  return error;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = OK;
  if (result == NULL || !is_matrix_correct(A) || !is_matrix_correct(B)) {
    error = INCORRECT_MATRIX;
  }
  if (A->columns != B->rows) {
    error = CALCULATION_ERROR;
  }
  if (!error) {
    error = s21_create_matrix(A->rows, B->columns, result);
    if (!error) {
      for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->columns; j++) {
          result->matrix[i][j] = calculate_matrix_element(i, j, A, B);
        }
      }
    }
  }
  if (error) s21_remove_matrix(result);
  return error;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int error = (result != NULL && is_matrix_correct(A) ? OK : INCORRECT_MATRIX);
  if (!error) {
    error = s21_create_matrix(A->columns, A->rows, result);
    if (!error) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  }
  if (error) s21_remove_matrix(result);
  return error;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int error = OK;
  if (result == NULL || !is_matrix_correct(A)) {
    error = INCORRECT_MATRIX;
  }
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
    if (!error && result->rows == 1 && result->columns == 1) {
      result->matrix[0][0] = A->matrix[0][0];
    } else if (!error) {
      // minors of matrix
      for (int i = 0; i < A->rows && !error; i++) {
        for (int j = 0; j < A->columns && !error; j++) {
          error = calculate_minor(i, j, A, &result->matrix[i][j]);
        }
      }
    }

    // matrix of algebraic complement
    if (!error) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] *= pow(-1, i + j);
        }
      }
    }
  }
  if (error) s21_remove_matrix(result);
  return error;
}

int s21_determinant(matrix_t *A, double *result) {
  int error = OK;
  if (result == NULL || !is_matrix_correct(A)) {
    error = INCORRECT_MATRIX;
  }
  if (!error && !is_square_matrix(A->rows, A->columns)) {
    error = CALCULATION_ERROR;
  }
  if (!error) {
    if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else if (A->rows == 2) {
      *result = (A->matrix[0][0] * A->matrix[1][1] -
                 A->matrix[0][1] * A->matrix[1][0]);
    } else {
      *result = 0;
      for (int j = 0; j < A->columns && !error; j++) {
        double value;
        error = calculate_minor(0, j, A, &value);
        if (!error) {
          *result += pow(-1, j) * A->matrix[0][j] * value;
        }
      }
    }
  }
  return error;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int error = OK;
  if (result == NULL || !is_matrix_correct(A)) {
    error = INCORRECT_MATRIX;
  }
  if (!error && !is_square_matrix(A->rows, A->columns)) {
    error = CALCULATION_ERROR;
  }
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
    double determinant = 0.0;
    if (!error) {
      error = s21_determinant(A, &determinant);
    }
    if (!error && !determinant) {
      error = CALCULATION_ERROR;
    }
    if (!error) {
      if (!error && result->rows == 1 && result->columns == 1) {
        if (A->matrix[0][0] != 0) {
          result->matrix[0][0] = 1.0 / A->matrix[0][0];
        } else {
          error = CALCULATION_ERROR;
        }
      } else if (!error) {
        matrix_t complemented = {0};
        matrix_t transposed = {0};
        error = s21_calc_complements(A, &complemented);
        if (!error) error = s21_transpose(&complemented, &transposed);
        s21_remove_matrix(&complemented);
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = transposed.matrix[i][j] / determinant;
          }
        }
        s21_remove_matrix(&transposed);
      }
    }
  }
  if (error) s21_remove_matrix(result);
  return error;
}