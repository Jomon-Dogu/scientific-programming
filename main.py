import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.animation import PillowWriter
from mpl_toolkits.mplot3d import Axes3D

def create_pendulum_animation(data, gif_path, fps=30):
    """Erstellt eine Animation für das Pendelsystem."""
    # Initialisiere das Diagramm
    fig, ax = plt.subplots()
    ax.set_xlim(-1.5, 1.5)
    ax.set_ylim(-1.5, 0.5)
    ax.set_aspect('equal')
    line, = ax.plot([], [], 'o-', lw=2)  # Pendellinie

    # Funktion zur Initialisierung der Animation
    def init():
        line.set_data([], [])
        return line,

    # Funktion zur Aktualisierung der Animation
    def update(frame):
        x = data['x'][frame]
        y = data['y'][frame]    
        line.set_data([0, x], [0, y])
        return line,

    # Erstellen der Animation
    ani = animation.FuncAnimation(fig, update, frames=len(data), init_func=init, blit=True)

    # Animation als GIF speichern mit PillowWriter
    ani.save(gif_path, writer=PillowWriter(fps=fps))
    print(f"Animation gespeichert unter: {gif_path}")

def create_lorenz_plot(data, png_path):
    """Erstellt einen 3D-Plot für das Lorenz-System und speichert ihn als PNG."""
    # 3D-Plot erstellen
    fig = plt.figure(figsize=(20, 20))
    ax = fig.add_subplot(111, projection='3d')

    # Plot der Lorenz-Daten
    ax.plot(data['X'], data['Y'], data['Z'], label='Lorenz-Attractor', color='b')

    # Achsenbeschriftungen und Titel
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('Lorenz System')

    # Legende
    ax.legend()

    # Plot als PNG speichern
    plt.savefig(png_path)
    print(f"Der Plot wurde als '{png_path}' gespeichert.")

def main():
    # Pendel-Daten (Harmonic Oscillator) einlesen
    pendulum_data = pd.read_csv("/home/jomon/git-repository/scientific-programming/harmonic_oscillator/spendulum_data.csv")
    create_pendulum_animation(pendulum_data, '/home/jomon/git-repository/scientific-programming/harmonic_oscillator/pendulum_animation.gif', fps=30)

    # Lorenz-Daten einlesen (Runge-Kutta)
    lorenz_data = pd.read_csv('/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_data.csv')
    create_lorenz_plot(lorenz_data, '/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_plot.png')

    lorenz_data_2 = pd.read_csv('/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_data_2.csv')
    create_lorenz_plot(lorenz_data_2, '/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_plot_2.png')

if __name__ == "__main__":
    main()
