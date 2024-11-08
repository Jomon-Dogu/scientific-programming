import numpy as np
import matplotlib.pyplot as plt

# CSV-Daten laden
u = np.loadtxt("velocity_u.csv", delimiter=",")
v = np.loadtxt("velocity_v.csv", delimiter=",")
p = np.loadtxt("pressure.csv", delimiter=",")

# Geschwindigkeitsfeld plotten und speichern
plt.figure()
plt.quiver(u, v)
plt.title("Geschwindigkeitsfeld")
plt.xlabel("x")
plt.ylabel("y")
plt.savefig("geschwindigkeitsfeld.png")  # Speichert das Bild als PNG-Datei
plt.close()  # Schließt die Figure, um Speicher freizugeben

# Druckfeld plotten und speichern
plt.figure()
plt.imshow(p, cmap="jet", origin="lower")
plt.colorbar()
plt.title("Druckfeld")
plt.xlabel("x")
plt.ylabel("y")
plt.savefig("druckfeld.png")  # Speichert das Bild als PNG-Datei
plt.close()  # Schließt die Figure, um Speicher freizugeben
