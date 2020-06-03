#include <algorithm>
//#include <chrono>
#include <iostream>
#include "knn.h"
#include "pca.h"

using namespace std;

KNNClassifier::KNNClassifier(unsigned int n_neighbors) {
    this->n_neighbors = n_neighbors;
    this->Pca = PCA(this->n_components, this->num_iter, this->epsilon);
}

void KNNClassifier::fit(Matrix X, Matrix y) {
    Pca.fit(X);
    this->y_train = y;
}

Vector KNNClassifier::predict(Matrix X) {
    // Creamos vector columna a devolver
    auto ret = Vector(X.rows());
    MatrixXd X_pca = this->Pca.transform(X);

    for (unsigned k = 0; k < X_pca.rows(); ++k) {
        ret(k) = calculate_KNN(X_pca.row(k));
    }

    return ret;
}

unsigned int KNNClassifier::calculate_KNN(Vector type) {
    
}
