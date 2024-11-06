#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

// Funktion, die die modifizierten Lorenz-Differentialgleichungen zurückgibt
void lorenz_derivatives(double X, double Y, double Z, double a, double b, double c, double &X_dot, double &Y_dot, double &Z_dot) {
    X_dot = (1.0 / 2.0) * ((-a * pow(X, 3) + (2 * a + b - Z) * pow(X, 2) * Y + (a - 2) * X * pow(Y, 2) - (b - Z) * pow(Y, 3)) / (pow(X, 2) + pow(Y, 2)));
    Y_dot = (1.0 / 2.0) * (((b - Z) * pow(X, 3) + (a - 2) * pow(X, 2) * Y + (-2 * a - b + Z) * X * pow(Y, 2) - a * pow(Y, 3)) / (pow(X, 2) + pow(Y, 2)));
    Z_dot = 2 * pow(X, 3) * Y - 2 * X * pow(Y, 3) - c * Z;
}

// Runge-Kutta 4. Ordnung zur Integration des Systems
void runge_kutta(double &X, double &Y, double &Z, double dt, double a, double b, double c) {
    double X_dot, Y_dot, Z_dot;
    
    // Berechnung der Korrekturen für X, Y und Z
    double k1_X, k2_X, k3_X, k4_X;
    double k1_Y, k2_Y, k3_Y, k4_Y;
    double k1_Z, k2_Z, k3_Z, k4_Z;

    // Berechne die k1 Werte
    lorenz_derivatives(X, Y, Z, a, b, c, X_dot, Y_dot, Z_dot);
    k1_X = dt * X_dot;
    k1_Y = dt * Y_dot;
    k1_Z = dt * Z_dot;

    // Berechne k2 Werte
    lorenz_derivatives(X + k1_X / 2.0, Y + k1_Y / 2.0, Z + k1_Z / 2.0, a, b, c, X_dot, Y_dot, Z_dot);
    k2_X = dt * X_dot;
    k2_Y = dt * Y_dot;
    k2_Z = dt * Z_dot;

    // Berechne k3 Werte
    lorenz_derivatives(X + k2_X / 2.0, Y + k2_Y / 2.0, Z + k2_Z / 2.0, a, b, c, X_dot, Y_dot, Z_dot);
    k3_X = dt * X_dot;
    k3_Y = dt * Y_dot;
    k3_Z = dt * Z_dot;

    // Berechne k4 Werte
    lorenz_derivatives(X + k3_X, Y + k3_Y, Z + k3_Z, a, b, c, X_dot, Y_dot, Z_dot);
    k4_X = dt * X_dot;
    k4_Y = dt * Y_dot;
    k4_Z = dt * Z_dot;

    // Aktualisiere X, Y, Z nach der Runge-Kutta 4. Ordnung
    X += (k1_X + 2 * k2_X + 2 * k3_X + k4_X) / 6.0;
    Y += (k1_Y + 2 * k2_Y + 2 * k3_Y + k4_Y) / 6.0;
    Z += (k1_Z + 2 * k2_Z + 2 * k3_Z + k4_Z) / 6.0;
}

int main() {
    // Parameter
    double dt = 0.01; // Zeit-Schrittweite
    double a = 10.0;
    double b = 28.0;
    double c = 8.0 / 3.0;

    // Zeitarray
    int num_steps = 10000;
    vector<double> time(num_steps);

    // Initialwerte
    double X = 0.0;
    double Y = 1.0;
    double Z = 1.05;

    // Ergebnisse in Vektoren speichern
    vector<double> X_values(num_steps);
    vector<double> Y_values(num_steps);
    vector<double> Z_values(num_steps);

    // Setze Startwerte
    X_values[0] = X;
    Y_values[0] = Y;
    Z_values[0] = Z;

    // Berechnung der Trajektorien
    for (int i = 1; i < num_steps; ++i) {
        // Runge-Kutta zur Berechnung der neuen Werte
        runge_kutta(X, Y, Z, dt, a, b, c);
        
        // Speichere die berechneten Werte
        X_values[i] = X;
        Y_values[i] = Y;
        Z_values[i] = Z;
    }

    // Ausgabe der Ergebnisse in eine CSV-Datei
    ofstream outfile("/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_data_2.csv");
    outfile << "X,Y,Z\n";
    for (int i = 0; i < num_steps; ++i) {
        outfile << X_values[i] << "," << Y_values[i] << "," << Z_values[i] << "\n";
    }
    outfile.close();

    cout << "Daten wurden in 'lorenz_data_2.csv' gespeichert." << endl;

    return 0;
}
