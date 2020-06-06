//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include "pca.h"
#include "eigen.h"
#include "CSVToEigen.h"
#include "knn.h"

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



    CSVToEigen<MatrixXd> converter = CSVToEigen<MatrixXd>();

    MatrixXd X_train = converter.load_csv("../notebooks/X_train.csv");
    MatrixXd X_val = converter.load_csv("../notebooks/X_val.csv");
    MatrixXd y_train = converter.load_csv("../notebooks/y_train.csv");
    MatrixXd y_val = converter.load_csv("../notebooks/y_val.csv");

    KNNClassifier knn = KNNClassifier(100);
    knn.fit(X_train, y_train);

    MatrixXd prediction = knn.predict(X_val);

    return 0;
}

