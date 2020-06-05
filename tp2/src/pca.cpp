#include <iostream>
#include "pca.h"
#include "eigen.h"

using namespace std;

PCA::PCA(unsigned int n_components, unsigned num_iter, double epsilon) {
    this->n_components = n_components;
    this->num_iter = num_iter;
    this->epsilon = epsilon;
}

void PCA::fit(Matrix X) {
    Matrix covariance_matrix = calculate_covariance(X);
    Matrix eigenvectors_matrix = get_first_eigenvalues(covariance_matrix, n_components,
                                                       num_iter, epsilon).second;
    this->base_change_matrix = eigenvectors_matrix;
}


MatrixXd PCA::transform(Matrix X) {
    return X * base_change_matrix;
}

Matrix PCA::calculate_covariance(Matrix matrix) {
    Matrix matrix_with_zero_mean(matrix);
    unsigned n = matrix.rows();
    Vector mean;
    Vector sum(matrix_with_zero_mean.cols());
    for (unsigned int i = 0; i < matrix.rows(); i++) {
        sum += matrix.row(i);
    }
    mean = sum / n;

    for (unsigned int i = 0; i < matrix.cols(); i++) {
        matrix_with_zero_mean.col(i) -= Vector::Constant(matrix_with_zero_mean.rows(), mean(i));
    }
    return (matrix_with_zero_mean.transpose() * matrix_with_zero_mean) / (n - 1);
}
