import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Lade die CSV-Datei mit den Lorenz-Daten
data = pd.read_csv('/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_data.csv')

# 3D-Plot erstellen
fig = plt.figure(figsize=(20,20))
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
plt.savefig('/home/jomon/git-repository/scientific-programming/Lorenz-Attraktor/lorenz_plot.png')
print("Der Plot wurde als 'lorenz_plot.png' gespeichert.")

# Plot anzeigen
#plt.show()
