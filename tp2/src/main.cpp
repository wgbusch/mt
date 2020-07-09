//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include <sys/time.h>
#include "pca.h"
#include "eigen.h"
#include "CSVEigenConverter.h"
#include "knn.h"
#include "Matrix_printer.h"

// neighbors for finding the mode in KNN
unsigned int N_NEIGHBORS = 5;
// components for PCA
unsigned int N_COMPONENTS = 20;
// number of iterations to find eigenvalues and eigenvectors in power iteration
unsigned int N_ITERATIONS = 2000;
// epsilon for power iteration
double EPSILON = 1e-10;
// percentage of train cases used to train the model.
unsigned int PERCENTAGE_OF_TRAIN_CASES = 1;
const char *methods[2] = {"KNN", "KNN+PCA"};
const char *modes[3] = {"TRAIN", "PREDICT", "TRAIN+PREDICT"};

std::string method;
std::string train_set;
std::string test_set;
std::string classif;
std::string mode;
std::string base_change_matrix_path;
std::string X_matrix_path;
std::string Y_matrix_path;

void print_info(double time);

void save_to_file(std::string, Vector matrix);

timeval start_time();

double end_time(timeval start);

void get_arguments(int argc, char **pString);

void test_get_first_eigenvalues() {
    Eigen::Matrix<double, 5, 1> v;
    v << 5, 4, 3, 2, 1;
    Eigen::Matrix<double, 5, 5> A = v.array().matrix().asDiagonal();

    std::pair<Vector, Matrix> result = get_first_eigenvalues(A, A.cols(), 100000, 1e-15);

    std::cout << "\n eigenvalues: \n" << result.first.array().round() << std::endl;
    std::cout << "\n eigenvectors: \n" << result.second.array().round() << std::endl;
}

void test() {
    CSVEigenConverter<MatrixXd> converter = CSVEigenConverter<MatrixXd>();

    MatrixXd X_train = converter.load_csv("../notebooks/06_06_18_22_31/X_train.csv");
    MatrixXd X_val = converter.load_csv("../notebooks/06_06_18_22_31/X_val.csv");
    MatrixXd y_train = converter.load_csv("../notebooks/06_06_18_22_31/y_train.csv");
    MatrixXd y_val = converter.load_csv("../notebooks/06_06_18_22_31/y_val.csv");

    PCA pca = PCA(20, 1000, 1e-10);

    pca.fit(X_train);
    MatrixXd X_train_pca = pca.transform(X_train);
    MatrixXd X_val_pca = pca.transform(X_val);

    KNNClassifier knn = KNNClassifier(1000);
    knn.fit(X_train_pca, y_train);

    MatrixXd prediction = knn.predict(X_val_pca);
}

int main(int argc, char **argv) {

    timeval start = start_time();

    get_arguments(argc,argv);

    if (mode == modes[0]) {
        CSVEigenConverter<MatrixXd> converter = CSVEigenConverter<MatrixXd>();
        Matrix temp = converter.load_csv(train_set, true);
        Matrix X = temp.block(0, 1, temp.rows(), temp.cols() - 1);
        Matrix Y = temp.block(0, 0, temp.rows(), 1);

        if (method == methods[1]) {
            PCA pca = PCA(N_COMPONENTS, N_ITERATIONS, EPSILON);
            pca.fit(X);
            X = pca.transform(X);
            converter.writeToCSVfile(base_change_matrix_path,
                                     pca.get_base_change_matrix());
        }

        converter.writeToCSVfile(X_matrix_path, X);
        converter.writeToCSVfile(Y_matrix_path, Y);

    } else if (mode == modes[1]) {
        CSVEigenConverter<MatrixXd> converter = CSVEigenConverter<MatrixXd>();
        Matrix X = converter.load_csv(X_matrix_path, true);
        Matrix Y = converter.load_csv(Y_matrix_path, true);
        Matrix X_test_temp = converter.load_csv(test_set, true);
        Matrix X_test = X_test_temp.block(0, 1, X_test_temp.rows(), X_test_temp.cols() - 1);
        Matrix base_change_matrix = converter.load_csv(base_change_matrix_path, true);

        if (method == methods[1]) {
            X_test = X_test * base_change_matrix;
        }

        KNNClassifier knn = KNNClassifier(N_NEIGHBORS);
        knn.fit(X, Y);
        Vector y_pred = knn.predict(X_test);
        save_to_file(classif, y_pred);

    } else if (mode == modes[2]) {
        CSVEigenConverter<MatrixXd> converter = CSVEigenConverter<MatrixXd>();
        Matrix temp = converter.load_csv(train_set, true);
        KNNClassifier knn = KNNClassifier(N_NEIGHBORS);
        Matrix X = temp.block(0, 1, temp.rows(), temp.cols() - 1);
        Matrix Y = temp.block(0, 0, temp.rows(), 1);
        Matrix X_test = converter.load_csv(test_set, true);

        if (method == methods[1]) {
            PCA pca = PCA(N_COMPONENTS, N_ITERATIONS, EPSILON);
            pca.fit(X);
            X = pca.transform(X);
            X_test = pca.transform(X_test);
        }

        knn.fit(X, Y);
        Vector y_pred = knn.predict(X_test);
        save_to_file(classif, y_pred);
    }

    double time = end_time(start);
    print_info(time);

    return 0;
}

void get_arguments(int argc, char **argv) {
    if (strcmp(argv[1], "0") == 0) {
        method = methods[0];
    } else if (strcmp(argv[1], "1") == 0) {
        method = methods[1];
    }
    train_set = argv[2];
    test_set = argv[3];
    classif = argv[4];

    // Mode for the application: 0 for only training,
    // 1 for only predict, 2 for train+predict.
    if(argc >5){
        mode = argv[5];
        if (strcmp(argv[5], "0") == 0) {
            mode = modes[0];
        } else if (strcmp(argv[5], "1") == 0) {
            mode = modes[1];
        } else if (strcmp(argv[5], "2") == 0) {
            mode = modes[2];
        }

        // Paths where to save the matrix that are created in training (in train mode - 0),
        // or where to load them from (in predict mode - 1).
        // Can be whatever for mode 2 (train+predict at once - 2)
        base_change_matrix_path = argv[6];
        X_matrix_path = argv[7];
        Y_matrix_path = argv[8];
    } else{
        mode = modes[2];
    }
}

double end_time(timeval start) {

    timeval end;
    gettimeofday(&end, NULL);
    long elapsed_seconds = end.tv_sec - start.tv_sec; /* diff between seconds counter */
    long elapsed_useconds = end.tv_usec - start.tv_usec; /* diff between microseconds counter */
    return ((elapsed_seconds) * 1000 + elapsed_useconds / 1000.0) + 0.5;
}

timeval start_time() {

    timeval start;
    gettimeofday(&start, NULL);
    return start;
}

void save_to_file(std::string name, Vector content) {
    //preparo archivo salida para escritura
    std::ofstream archivoDeSalida;
    archivoDeSalida.setf(std::ios::fixed, std::ios::floatfield); // tipo salida
    archivoDeSalida.precision(15); // cant decimales
    archivoDeSalida.open(name);
    for (unsigned int i = 0; i < content.rows(); i++) {
        archivoDeSalida << (unsigned int) content[i] << std::endl;
    }
    archivoDeSalida.close();

}

void print_info(double time) {
    unsigned int percentage = (unsigned int) PERCENTAGE_OF_TRAIN_CASES * 100;
    std::cout << "------------------------°------------------------" << std::endl;
    std::cout << "Finished in: " << (unsigned int) time / 1000 << " seconds." << std::endl;
    std::cout << "Method used in trainig: " << method << std::endl;
    std::cout << "Number of neighbors used in KNN: " << N_NEIGHBORS << std::endl;
    std::cout << "Number of components used in PCA: " << N_COMPONENTS << std::endl;
    std::cout << "Number of iterations used in power iteration: " << N_ITERATIONS << std::endl;
    std::cout << "Epsilon used in power iteration: " << EPSILON << std::endl;
    std::cout << "Percentage of train data used to fit model : " << percentage << "%" << std::endl;
    std::cout << "------------------------°------------------------" << std::endl;
}
