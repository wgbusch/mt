//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include "pca.h"
#include "eigen.h"
#include "CSVToEigen.h"
#include "knn.h"
#include "Matrix_printer.h"

// neighbors for finding the mode in KNN
unsigned int N_NEIGHBORS = 20;
// components for PCA
unsigned int N_COMPONENTS=20;
// number of iterations to find eigenvalues and eigenvectors in power iteration
unsigned int N_ITERATIONS=2000;
// epsilon for power iteration
double EPSILON=1e-10;

void test_get_first_eigenvalues() {
    Eigen::Matrix<double, 5, 1> v;
    v << 5, 4, 3, 2, 1;
    Eigen::Matrix<double, 5, 5> A = v.array().matrix().asDiagonal();

    std::pair<Vector, Matrix> result = get_first_eigenvalues(A, A.cols(), 100000, 1e-15);

    std::cout << "\n eigenvalues: \n" << result.first.array().round() << std::endl;
    std::cout << "\n eigenvectors: \n" << result.second.array().round() << std::endl;
}

void test(){

    CSVToEigen<MatrixXd> converter = CSVToEigen<MatrixXd>();

    MatrixXd X_train = converter.load_csv("../notebooks/06_06_18_22_31/X_train.csv");
    MatrixXd X_val = converter.load_csv("../notebooks/06_06_18_22_31/X_val.csv");
    MatrixXd y_train = converter.load_csv("../notebooks/06_06_18_22_31/y_train.csv");
    MatrixXd y_val = converter.load_csv("../notebooks/06_06_18_22_31/y_val.csv");

    PCA pca = PCA(20,1000,1e-10);

    pca.fit(X_train);
    MatrixXd X_train_pca = pca.transform(X_train);
    MatrixXd X_val_pca = pca.transform(X_val);

    KNNClassifier knn = KNNClassifier(1000);
    knn.fit(X_train_pca, y_train);

    MatrixXd prediction = knn.predict(X_val_pca);
}

enum class methods{
    KNN,
    KNNwithPCA
};

int main(int argc, char **argv) {
    std::cout << "Hola mundo!" << std::endl;

    methods method;
    if(strcmp(argv[1], "0")== 0){
        method = methods(0);
    }
    else if(strcmp(argv[1], "1")== 0){
        method = methods(1);
    }

    std::string train_set = argv[2];

    std::string test_set = argv[3];

    std::string classif = argv[4];

    CSVToEigen<MatrixXd> converter = CSVToEigen<MatrixXd>();

    //preparo archivo salida para escritura
    std::ofstream archivoDeSalida;
    archivoDeSalida.setf(std::ios::fixed, std::ios::floatfield); // tipo salida
    archivoDeSalida.precision(15); // cant decimales
    archivoDeSalida.open(classif);


    if(method== methods::KNN){
            KNNClassifier knn = KNNClassifier(N_NEIGHBORS);

            //Matrix temp = converter.load_csv(train_set,true);
            //Matrix X = temp [1:2];
            //Matrix Y = temp.row(0);

           // knn.fit(X,Y);

            Matrix X_test = converter.load_csv(test_set,true);

            Vector y_pred = knn.predict(X_test);

            for (unsigned int i =0; i < y_pred.rows(); i++){
                archivoDeSalida << y_pred[i] << std::endl;
            }
            archivoDeSalida.close();
    } else if(method == methods::KNNwithPCA){

    }


    return 0;
}