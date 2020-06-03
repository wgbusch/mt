#pragma once

#include "types.h"
#include "pca.h"


class KNNClassifier {
public:
    KNNClassifier(unsigned int n_neighbors);

    void fit(Matrix X, Matrix y);

    Vector predict(Matrix X);
private:
    unsigned int n_neighbors;
    unsigned int n_components;
    unsigned num_iter;
    double epsilon;
    MatrixXd y_train;
    PCA Pca;

    unsigned int calculate_KNN(Vector type);
};
