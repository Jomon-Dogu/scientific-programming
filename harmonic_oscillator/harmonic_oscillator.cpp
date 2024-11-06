#include <iostream>
#include <fstream>
#include <cmath>

class Pendulum {
private:
    double g;      // Erdbeschleunigung in m/s^2
    double L;      // Länge des Pendels in Metern
    double dt;     // Zeitschritt in Sekunden
    double theta;  // Aktueller Winkel in Radiant
    double omega;  // Aktuelle Winkelgeschwindigkeit in rad/s

public:
    // Konstruktor
    Pendulum(double length, double gravity, double time_step)
        : L(length), g(gravity), dt(time_step), theta(M_PI / 4), omega(0.0) {}

    // Funktion zur Berechnung der nächsten Position des Pendels
    void step() {
        double alpha = -(g / L) * sin(theta);  // Winkelbeschleunigung
        omega += alpha * dt;                   // Aktualisierung der Winkelgeschwindigkeit
        theta += omega * dt;                   // Aktualisierung des Winkels
    }

    // Funktion zur Berechnung der Position des Pendels
    void getPosition(double& x, double& y) const {
        x = L * sin(theta);
        y = -L * cos(theta);
    }

    // Funktion zur Abfrage des aktuellen Winkels
    double getAngle() const {
        return theta;
    }
};

int main() {
    const double g = 9.81;   // Erdbeschleunigung in m/s^2
    const double L = 1.0;    // Länge des Pendels in Metern
    const double dt = 0.02;  // Zeitschritt in Sekunden
    const int steps = 500;    // Anzahl der Zeitschritte

    // Erstelle ein Pendelobjekt
    Pendulum pendulum(L, g, dt);

    // Datei zur Ausgabe der Ergebnisse
    std::ofstream outfile("/home/jomon/git-repository/scientific-programming/harmonic_oscillator/spendulum_data.csv");
    outfile << "time,angle,x,y\n";

    double t = 0.0;  // Startzeit
    // Simulation
    for (int i = 0; i < steps; ++i) {
        double x, y;
        pendulum.getPosition(x, y); // Berechnung der Position

        // Schreiben der Daten in die Datei
        outfile << t << "," << pendulum.getAngle() << "," << x << "," << y << "\n";

        pendulum.step(); // Update der Pendelposition
        t += dt;        // Aktualisierung der Zeit
    }

    outfile.close();
    std::cout << "Simulation abgeschlossen! Daten gespeichert in 'spendulum_data.csv'.\n";
    return 0;
}
