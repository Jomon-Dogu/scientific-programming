#include <iostream>
#include <fstream>
#include <vector>

int main() {
    double a = 10.0, b = 28.0, c = 8.0 / 3.0;
    double dt = 0.01;
    int num_steps = 10000;

    // Zeitvektor
    std::vector<double> time(num_steps);
    for (int i = 0; i < num_steps; ++i) {
        time[i] = i * dt;
    }

    // Zustandsvektoren
    std::vector<double> X(num_steps, 0.0);
    std::vector<double> Y(num_steps, 1.0);
    std::vector<double> Z(num_steps, 1.05);

    // Runge-Kutta-Integration
    for (int i = 0; i < num_steps - 1; ++i) {
        // Runge-Kutta für X
        double k1_x = dt * (a * (Y[i] - X[i]));
        double k2_x = dt * (a * (Y[i] - X[i] + k1_x / 2));
        double k3_x = dt * (a * (Y[i] - X[i] + k2_x / 2));
        double k4_x = dt * (a * (Y[i] - X[i] + k3_x));

        X[i + 1] = X[i] + (k1_x / 6 + k2_x / 3 + k3_x / 3 + k4_x / 6);

        // Runge-Kutta für Y
        double k1_y = dt * (X[i] * (b - Z[i]) - Y[i]);
        double k2_y = dt * (X[i] * (b - Z[i]) - Y[i] + k1_y / 2);
        double k3_y = dt * (X[i] * (b - Z[i]) - Y[i] + k2_y / 2);
        double k4_y = dt * (X[i] * (b - Z[i]) - Y[i] + k3_y);

        Y[i + 1] = Y[i] + (k1_y / 6 + k2_y / 3 + k3_y / 3 + k4_y / 6);

        // Runge-Kutta für Z
        double k1_z = dt * (X[i] * Y[i] - c * Z[i]);
        double k2_z = dt * (X[i] * Y[i] - c * Z[i] + k1_z / 2);
        double k3_z = dt * (X[i] * Y[i] - c * Z[i] + k2_z / 2);
        double k4_z = dt * (X[i] * Y[i] - c * Z[i] + k3_z);

        Z[i + 1] = Z[i] + (k1_z / 6 + k2_z / 3 + k3_z / 3 + k4_z / 6);
    }

    // Speichern der Daten in eine CSV-Datei
    std::ofstream file("/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_data.csv");
    file << "time,X,Y,Z\n";
    for (int i = 0; i < num_steps; ++i) {
        file << time[i] << "," << X[i] << "," << Y[i] << "," << Z[i] << "\n";
    }
    file.close();

    std::cout << "Daten wurden in 'lorenz_data.csv' gespeichert." << std::endl;
    return 0;
}
