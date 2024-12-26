import numpy as np
import matplotlib
matplotlib.use('Agg')  # Non-interactive Backend
import matplotlib.pyplot as plt

def generate_random_lattice(L, p):
    """
    Generate a randomly occupied square lattice.

    Parameters:
        L (int): Size of the lattice (L x L).
        p (float): Probability for a site to be occupied (value 1).

    Returns:
        numpy.ndarray: A matrix of size L x L with 0s and 1s.
    """
    return np.random.choice([0, 1], size=(L, L), p=[1-p, p])

def plot_lattice(lattice):
    """
    Plot the given lattice as an image.

    Parameters:
        lattice (numpy.ndarray): The lattice to plot.
    """
    plt.figure(figsize=(6, 6))
    plt.imshow(lattice, cmap='Greys', origin='upper')
    plt.title("Randomly Occupied Lattice")
    plt.colorbar(label="Occupied (1) or Empty (0)")
    plt.savefig("lattice_plot.png")  # Save the plot as an image
    print("Plot saved as 'lattice_plot.png'.")

# Example usage
L = 40  # Size of the lattice
p = 0.5  # Probability of a site being occupied

# Anzeigeoption setzen, um alle Einträge anzuzeigen
np.set_printoptions(threshold=np.inf, linewidth=np.inf)

# Generate and plot the lattice
random_lattice = np.random.choice([0, 1], size=(L, L), p=[1-p, p])



# Matrix in der Konsole ausgeben
print("Generated Matrix:")
print(random_lattice)

# Plot speichern
plot_lattice(random_lattice)
