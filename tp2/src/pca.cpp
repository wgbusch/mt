#include <iostream>
#include "pca.h"
#include "eigen.h"
#include "Matrix_printer.h"

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
    MatrixXd centered = matrix.rowwise() - matrix.colwise().mean();
    return (centered.transpose() * centered) / double(matrix.rows() - 1);
}
