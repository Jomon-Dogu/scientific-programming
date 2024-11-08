#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

// Parameter
const int N = 100;            // Grid resolution
const double dt = 0.01;       // Time step
const double Re = 1000.0;     // Reynolds number
const double dx = 1.0 / N;    // Grid spacing

// Velocity and pressure fields
std::vector<std::vector<double>> u(N, std::vector<double>(N, 0.0));
std::vector<std::vector<double>> v(N, std::vector<double>(N, 0.0));
std::vector<std::vector<double>> p(N, std::vector<double>(N, 0.0));

// Funktionsdeklarationen
void apply_boundary_conditions();
void save_results();
double laplacian(const std::vector<std::vector<double>> &field, int i, int j);

// Laplace-Funktion für Diffusionsterm
double laplacian(const std::vector<std::vector<double>> &field, int i, int j) {
    return (field[i+1][j] - 2*field[i][j] + field[i-1][j]) / (dx * dx) +
           (field[i][j+1] - 2*field[i][j] + field[i][j-1]) / (dx * dx);
}

// Simulationsschritt
void step() {
    apply_boundary_conditions();  // Randbedingungen am Anfang

    std::vector<std::vector<double>> u_new = u;
    std::vector<std::vector<double>> v_new = v;

    // Hauptberechnung
    for (int i = 1; i < N-1; i++) {
        for (int j = 1; j < N-1; j++) {
            // Advektion und Diffusion
            double adv_u = -u[i][j] * (u[i+1][j] - u[i-1][j]) / (2 * dx)
                           - v[i][j] * (u[i][j+1] - u[i][j-1]) / (2 * dx);
            double adv_v = -u[i][j] * (v[i+1][j] - v[i-1][j]) / (2 * dx)
                           - v[i][j] * (v[i][j+1] - v[i][j-1]) / (2 * dx);

            double diff_u = laplacian(u, i, j) / Re;
            double diff_v = laplacian(v, i, j) / Re;

            u_new[i][j] = u[i][j] + dt * (adv_u + diff_u);
            v_new[i][j] = v[i][j] + dt * (adv_v + diff_v);
        }
    }

    // Druckkorrektur
    for (int iter = 0; iter < 50; iter++) {
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                double div = (u_new[i+1][j] - u_new[i-1][j]) / (2 * dx) +
                             (v_new[i][j+1] - v_new[i][j-1]) / (2 * dx);
                if (div != 0) {
                    p[i][j] = 0.25 * (p[i+1][j] + p[i-1][j] + p[i][j+1] + p[i][j-1]
                                    - dx * dx * div / dt);
                }
            }
        }
    }

    // Geschwindigkeitskorrektur
    for (int i = 1; i < N-1; i++) {
        for (int j = 1; j < N-1; j++) {
            u[i][j] = u_new[i][j] - dt * (p[i+1][j] - p[i-1][j]) / (2 * dx);
            v[i][j] = v_new[i][j] - dt * (p[i][j+1] - p[i][j-1]) / (2 * dx);
        }
    }

    apply_boundary_conditions();  // Randbedingungen am Ende
}

// Randbedingungen definieren
void apply_boundary_conditions() {
    // Beispiel: Linker Rand (Einlassbedingung u = 1.0)
    for (int j = 0; j < N; j++) {
        u[0][j] = 1.0;  // Einlass
        v[0][j] = 0.0;
    }

    // Andere Ränder: No-Slip-Bedingung
    for (int i = 0; i < N; i++) {
        u[i][0] = 0.0;        // Unterer Rand
        u[i][N-1] = 0.0;      // Oberer Rand
        v[i][0] = 0.0;
        v[i][N-1] = 0.0;
    }

    // Rechter Rand: No-Slip-Bedingung
    for (int j = 0; j < N; j++) {
        u[N-1][j] = 0.0;
        v[N-1][j] = 0.0;
    }
}

// Funktion zur Speicherung der Ergebnisse in CSV-Dateien
void save_results() {
    // Speichern der u-Geschwindigkeit
    std::ofstream u_file("velocity_u.csv");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            u_file << u[i][j];
            if (j < N - 1) u_file << ",";
        }
        u_file << "\n";
    }
    u_file.close();

    // Speichern der v-Geschwindigkeit
    std::ofstream v_file("velocity_v.csv");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            v_file << v[i][j];
            if (j < N - 1) v_file << ",";
        }
        v_file << "\n";
    }
    v_file.close();

    // Speichern des Druckfeldes
    std::ofstream p_file("pressure.csv");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            p_file << p[i][j];
            if (j < N - 1) p_file << ",";
        }
        p_file << "\n";
    }
    p_file.close();
}

// main-Funktion
int main() {
    for (int t = 0; t < 1000; t++) {
        step();
        if (t % 100 == 0) {
            std::cout << "Simulation step: " << t << std::endl;
        }
    }
    std::cout << "Simulation completed." << std::endl;

    // Ergebnisse speichern
    save_results();
    return 0;
}
