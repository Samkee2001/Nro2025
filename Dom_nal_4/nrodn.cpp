#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <omp.h>
#include <ctime>


using namespace std;

int main() {

    // inicializiramo matriko A
    vector<vector<double>> A;

    // inicializiramo vektor b;
    vector<double> b;

    // podamo ime datoteke
    std::string filename = "./datoteka_A_b.txt";

    // preberemo datoteko
    std::ifstream infile;
    infile.open(filename);

    // preberemo prvo vrstico, v kateri imamo podano velikost matrike A 
    std::string string_first_line;
    std::getline(infile, string_first_line);

    std::replace(string_first_line.begin(), string_first_line.end(), '=', ' ');

    std::istringstream iss(string_first_line);
    std::string nepomemben_del1;
    std::string nepomemben_del2;
    int n;

    iss >> nepomemben_del1;
    iss >> nepomemben_del2;
    iss >> n;

    std::cout << "Velikost matrike A: " << n << "x" << n << std::endl;;

    for (int iiA = 0; iiA < n; iiA++) {
        std::string line;
        std::getline(infile, line);
        std::replace(line.begin(), line.end(), ';', ' ');

        std::istringstream iss_column(line);
        vector<double> row;

        for (int column = 0; column < n; column++) {
            double element_a = 0;
            iss_column >> element_a;
            row.push_back(element_a);
        }
        A.push_back(row);
    }

    std::string empty_line;
    std::getline(infile, empty_line);

    std::string string_line_b;
    std::getline(infile, string_line_b);

    std::replace(string_line_b.begin(), string_line_b.end(), '>', ' ');
    std::istringstream iss_b(string_line_b);
    int n_b;

    iss_b >> nepomemben_del1;
    iss_b >> nepomemben_del2;
    iss_b >> n_b;

    std::cout << "Velikost vektorja b: " << n_b << std::endl;;

    for (int iib = 0; iib < n_b; iib++) {
        std::string line_b_element;
        std::getline(infile, line_b_element);
        std::istringstream iss_b_element(line_b_element);

        double b_element = 0;
        iss_b_element >> b_element;
        b.push_back(b_element);
    }

    // Dodani del - Serijska implementacija Gauss-Seidel metode
    vector<double> T(n_b, 100);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int iter = 0; iter < 2000; iter++) {
        vector<double> T_new(T);
        for (int i = 0; i < n; i++) {
            double sigma = 0.0;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sigma += A[i][j] * T[j];
                }
            }
            T_new[i] = (b[i] - sigma) / A[i][i];
        }
        T = T_new;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_duration = end_time - start_time;
    std::cout << "Time of Gauss-Seidel (serial): " << time_duration.count() << " seconds" << std::endl;

    double max_T = 0;
    for (int i = 0; i < n_b; i++) {
        if (T[i] > max_T) {
            max_T = T[i];
        }
    }
    std::cout << "Max. temperature (serial): " << max_T << " degree C." << std::endl;

    // Dodani del - Preverjanje možnosti paralelizacije
    // Preverimo, ali ima matrika A vse neničelne diagonalne elemente,
    // To je ključno za pravilno delovanje Gauss-Seidel metode.
    bool can_parallelize = true;
    for (int i = 0; i < n; i++) {
        if (A[i][i] == 0) {
            can_parallelize = false;
            break;
        }
    }

    // Če so diagonalni elementi nič, paralelizacija ni možna
    if (!can_parallelize) {
        std::cout << "Paralelizacija ni možna, ker matrika A vsebuje ničelne diagonalne elemente." << std::endl;
        return 0;
    }

    // Dodani del - Paralelizacija z uporabo OpenMP
    vector<double> T_parallel(n_b, 100);

    start_time = std::chrono::high_resolution_clock::now();

    // Zanka za iteracije metode Gauss-Seidel, paralelizirana z OpenMP.
    for (int iter = 0; iter < 2000; iter++) {
#pragma omp parallel for // OpenMP za paralelno izvajanje
        for (int i = 0; i < n; i++) {
            double sigma = 0.0; // Del vsote, ki vključuje vse elemente razen diagonalnega.
            for (int j = 0; j < n; j++) {
                if (j != i) { // Izključimo prispevek diagonalnega elementa.
                    sigma += A[i][j] * T_parallel[j];
                }
            }
            // Posodobimo trenutni element rešitve na osnovi Gauss-Seidel formule.
            T_parallel[i] = (b[i] - sigma) / A[i][i];
        }
    }

    end_time = std::chrono::high_resolution_clock::now();
    time_duration = end_time - start_time;
    std::cout << "Time of Gauss-Seidel (parallel): " << time_duration.count() << " seconds" << std::endl;

    max_T = 0;
    for (int i = 0; i < n_b; i++) {
        if (T_parallel[i] > max_T) {
            max_T = T_parallel[i];
        }
    }
    std::cout << "Max. temperature (parallel): " << max_T << " degree C." << std::endl;

    return 0;
}

