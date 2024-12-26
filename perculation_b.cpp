#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <cstdlib>
#include <cassert>
#include <cairo.h>  // Für PNG-Ausgabe
#include <fstream>   // Für Dateiausgabe

// Funktion zum Implementieren des Hoshen-Kopelman-Algorithmus
std::vector<std::vector<int>> hoshen_kopelman(const std::vector<std::vector<int>>& lattice) {
    int rows = lattice.size();
    int cols = lattice[0].size();
    std::vector<std::vector<int>> labels(rows, std::vector<int>(cols, -1));
    int label_counter = 0;
    std::unordered_map<int, int> equivalences; // Zuordnung von Labels (Cluster)

    // Durchlaufen der Lattice-Array und Zuweisen von Labels
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (lattice[i][j] == 1) { // Nur besetzte Zellen verarbeiten
                int left_label = (j > 0) ? labels[i][j - 1] : -1;
                int top_label = (i > 0) ? labels[i - 1][j] : -1;

                if (left_label == -1 && top_label == -1) {
                    label_counter += 10; // Neue Cluster-ID
                    labels[i][j] = label_counter;
                } else if (left_label != -1 && top_label == -1) {
                    labels[i][j] = left_label;
                } else if (top_label != -1 && left_label == -1) {
                    labels[i][j] = top_label;
                } else if (left_label != -1 && top_label != -1) {
                    labels[i][j] = std::min(left_label, top_label);
                    if (left_label != top_label) {
                        equivalences[std::max(left_label, top_label)] = std::min(left_label, top_label);
                    }
                }
            }
        }
    }

    // Funktion zum Finden der Root-Cluster-ID (repräsentiert durch Rekursion)
    auto find_root = [&equivalences](int label) {
        while (equivalences.find(label) != equivalences.end()) {
            label = equivalences[label];
        }
        return label;
    };

    // Auflösen der Äquivalenzen und Umbenennung der Labels
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (labels[i][j] > 0) {
                labels[i][j] = find_root(labels[i][j]);
            }
        }
    }

    return labels;
}

// Hilfsfunktion zum Erzeugen eines zufälligen Gitters
std::vector<std::vector<int>> generate_lattice(int size, double probability) {
    std::vector<std::vector<int>> lattice(size, std::vector<int>(size));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(probability);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            lattice[i][j] = dis(gen);
        }
    }

    return lattice;
}

// Funktion zum Erzeugen und Speichern des PNGs mit den Metadaten als Legende
void plot_clusters(const std::vector<std::vector<int>>& labels, double execution_time, int lattice_size, double probability) {
    int width = labels.size();
    int height = labels[0].size();

    // Cairo-Setup für das Bild (größere Bildgröße für mehr Platz)
    int img_width = width * 2;  // Bildbreite verdoppeln für mehr Platz
    int img_height = height * 2; // Bildhöhe verdoppeln für mehr Platz

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, img_width, img_height);
    cairo_t *cr = cairo_create(surface);

    // Hintergrundfarbe: schwarz (für nicht besetzte Zellen)
    cairo_set_source_rgb(cr, 0, 0, 0); // Schwarz
    cairo_paint(cr);

    // Clusterfarben generieren
    std::unordered_map<int, cairo_pattern_t*> color_map;
    int cluster_counter = 1;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (labels[i][j] != -1) { // Besetzte Zellen
                if (color_map.find(labels[i][j]) == color_map.end()) {
                    // Generiere eine zufällige Farbe für jedes Cluster
                    double r = (rand() % 100) / 100.0;
                    double g = (rand() % 100) / 100.0;
                    double b = (rand() % 100) / 100.0;
                    color_map[labels[i][j]] = cairo_pattern_create_rgb(r, g, b);
                }
                cairo_set_source(cr, color_map[labels[i][j]]);
                cairo_rectangle(cr, j * 2, i * 2, 2, 2); // Skalieren, um Cluster besser sichtbar zu machen
                cairo_fill(cr);
            }
        }
    }

    // Metadaten (Ausführungszeit, Gittergröße, Wahrscheinlichkeit)
    std::string metadata = "Execution Time: " + std::to_string(execution_time) + " s\n"
                         + "Lattice Size: " + std::to_string(lattice_size) + " x " + std::to_string(lattice_size) + "\n"
                         + "Probability: " + std::to_string(probability);

    // Text auf das Bild setzen (als Legende)
    cairo_set_source_rgb(cr, 1, 1, 1); // Weiß
    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

    // Dynamische Schriftgröße, abhängig von der Bildgröße
    double font_size = (img_width > 4000) ? 120 : 72; // Größere Schriftgröße bei größeren Bildern
    cairo_set_font_size(cr, font_size);

    // Text einfügen, weiter unten verschoben
    cairo_move_to(cr, 100, img_height - 150);  // Position der Legende weiter nach unten verschoben
    cairo_show_text(cr, metadata.c_str());

    // PNG speichern
    cairo_surface_write_to_png(surface, "cluster_labeling_high_contrast.png");

    // Aufräumen
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    std::cout << "Cluster labeling plot saved as 'cluster_labeling_high_contrast.png'.\n";
}

// Funktion zum Schreiben der Matrix in eine Datei
void write_matrix_to_file(const std::vector<std::vector<int>>& matrix, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei zum Schreiben der Matrix.\n";
        return;
    }

    for (const auto& row : matrix) {
        for (int value : row) {
            outfile << value << " ";
        }
        outfile << "\n";
    }

    outfile.close();
    std::cout << "Matrix wurde in die Datei '" << filename << "' geschrieben.\n";
}

int main() {
    // Startzeit messen
    clock_t start_time = clock();

    // Parameter für das Gitter
    int L = 6000;
    double p = 0.4;

    // Zufälliges Gitter generieren
    auto random_lattice = generate_lattice(L, p);

    // Hoshen-Kopelman-Algorithmus anwenden
    auto labeled_clusters = hoshen_kopelman(random_lattice);

    // Endzeit messen
    clock_t end_time = clock();
    double execution_time = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Plot mit den Metadaten erstellen und speichern
    plot_clusters(labeled_clusters, execution_time, L, p);

    // Matrix in eine Textdatei schreiben
    write_matrix_to_file(labeled_clusters, "cluster_matrix.txt");

    // Ausführungszeit ausgeben
    std::cout << "Total execution time: " << execution_time << " seconds\n";

    return 0;
}
