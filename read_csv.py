import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
from matplotlib.animation import PillowWriter
# Lade die Daten aus der CSV-Datei
data = pd.read_csv("/home/wolff/git/scientific-programming/bin/spendulum_data.csv")

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
ani.save('pendulum_animation.gif', writer=PillowWriter(fps=30))