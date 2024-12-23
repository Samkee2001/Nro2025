#include <cmath>
#include <iostream>

// Definicija konstante pi
const double moj_pi = 3.141592653589793;

// Funkcija za izracun ArcTan z uporabo Taylorjevega razvoja
double arcTan(double x, int koraki) {
    double vsota = 0.0;
    for (int n = 0; n < koraki; ++n) {
        vsota += pow(-1, n) * pow(x, 2 * n + 1) / (2 * n + 1);
    }
    return vsota;
}

// Funkcija
double integrand(double x) {
    return exp(3 * x) * arcTan(x / 2, 10);
}

// Trapezno pravilo za numericni izracun integrala
double trapez(double zacetek, double konec, int deli) {
    double korak = (konec - zacetek) / deli;
    double rezultat = 0.5 * (integrand(zacetek) + integrand(konec));

    for (int i = 1; i < deli; ++i) {
        double trenutna_tocka = zacetek + i * korak;
        rezultat += integrand(trenutna_tocka);
    }

    return korak * rezultat;
}

int main() {
    double a = 0.0, b = moj_pi / 4; // Meje integracije
    int razdeli = 750; // Stevilo razdelkov

    double vrednost = trapez(a, b, razdeli);
    std::cout << "Rezultat integrala je: " << vrednost << std::endl;

    return 0;
}