//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include "pca.h"
#include "eigen.h"

void test_get_first_eigenvalues() {
    Eigen::Matrix<double, 5, 1> v;
    v << 5, 4, 3, 2, 1;
    Eigen::Matrix<double, 5, 5> A = v.array().matrix().asDiagonal();

    std::pair<Vector, Matrix> result = get_first_eigenvalues(A, A.cols(), 100000, 1e-15);

    std::cout << "\n eigenvalues: \n" << result.first.array().round() << std::endl;
    std::cout << "\n eigenvectors: \n" << result.second.array().round() << std::endl;
}

int main(int argc, char **argv) {
    std::cout << "Hola mundo!" << std::endl;
    test_get_first_eigenvalues();
    Matrix X(2,2);
    X(0,0) =2;
    X(0,1) =4;
    X(1,0) =4;
    X(1,1) =10;

    PCA pca(2,100000, 1e-15);

    Matrix u = pca.transform(X);
    std::cout << "\n-- matrix :" <<  "-----\n" << u << "\n-------\n"
              << std::endl;
    return 0;
}

