import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import time  # Importiere das Modul für die Zeitmessung

def hoshen_kopelman(lattice):
    """
    Implement the Hoshen-Kopelman algorithm to label clusters in a given lattice.

    Parameters:
        lattice (numpy.ndarray): A 2D array of 0s and 1s representing the lattice.

    Returns:
        numpy.ndarray: A 2D array where each occupied site has a cluster label, 
                       or -1 if the site is unoccupied.
    """
    # Initialize the cluster labeling matrix
    labels = -np.ones_like(lattice, dtype=int)  # Start with all unoccupied (-1)
    label_counter = 0  # Cluster label counter
    equivalences = {}  # To keep track of equivalent labels

    rows, cols = lattice.shape

    for i in range(rows):
        for j in range(cols):
            if lattice[i, j] == 1:  # Only process occupied sites
                # Get the neighbors' labels (left and top)
                left_label = labels[i, j - 1] if j > 0 else -1
                top_label = labels[i - 1, j] if i > 0 else -1

                if left_label == -1 and top_label == -1:
                    # New cluster, increase by 10 instead of 1
                    label_counter += 10
                    labels[i, j] = label_counter
                elif left_label != -1 and top_label == -1:
                    # Use left label
                    labels[i, j] = left_label
                elif top_label != -1 and left_label == -1:
                    # Use top label
                    labels[i, j] = top_label
                elif left_label != -1 and top_label != -1:
                    # Merge clusters
                    labels[i, j] = min(left_label, top_label)
                    if left_label != top_label:
                        equivalences[max(left_label, top_label)] = min(left_label, top_label)

    # Resolve equivalences
    # A recursive function to find the root label of a cluster
    def find_root(label):
        while label in equivalences:
            label = equivalences[label]
        return label

    # Now, resolve all labels based on equivalences
    for i in range(rows):
        for j in range(cols):
            if labels[i, j] > 0:
                labels[i, j] = find_root(labels[i, j])

    return labels

def generate_high_contrast_colormap(num_clusters):
    """
    Generate a colormap with high contrast for neighboring clusters.
    This function assigns distinct colors to each cluster without a gradient.

    Parameters:
        num_clusters (int): The number of clusters to be assigned unique colors.

    Returns:
        ListedColormap: A colormap with distinct colors for each cluster.
    """
    # Create distinct hues for each cluster to maximize contrast
    hues = np.linspace(0, 1, num_clusters, endpoint=False)  # Spread the hues in the full range
    colors = np.array([plt.cm.hsv(hue) for hue in hues])  # Generate RGB colors

    return ListedColormap(colors)

def plot_clusters(labels, execution_time, lattice_size, probability):
    """
    Plot the cluster-labeled lattice with distinct colors for each cluster and include metadata.

    Parameters:
        labels (numpy.ndarray): A 2D array of cluster labels.
        execution_time (float): The total execution time of the script.
        lattice_size (int): The size of the lattice (L).
        probability (float): The probability of occupied sites.

    Returns:
        None: Displays the plot.
    """
    unique_labels = np.unique(labels)
    num_clusters = len(unique_labels)

    # Generate a colormap with high contrast for the clusters
    cmap = generate_high_contrast_colormap(num_clusters)

    # Create a custom colormap where -1 (unoccupied) is black
    colors = np.array(cmap.colors)
    colors = np.vstack([np.array([0, 0, 0, 1]), colors])  # Add black for -1
    new_cmap = ListedColormap(colors)

    # Create the plot
    plt.figure(figsize=(8, 8))
    plt.imshow(labels, cmap=new_cmap, origin='upper')
    plt.title("Cluster Labeling with High Contrast")
    plt.colorbar(label="Cluster Labels")

    # Add metadata as text to the plot
    metadata_text = (f"Execution Time: {execution_time:.4f} s\n"
                     f"Lattice Size: {lattice_size} x {lattice_size}\n"
                     f"Probability: {probability}")
    plt.text(0.02, 0.98, metadata_text, ha='left', va='top', transform=plt.gca().transAxes,
             fontsize=12, color='white', bbox=dict(facecolor='black', alpha=0.7))

    # Save the plot
    plt.savefig("cluster_labeling_high_contrast_py.png")
    print("Cluster labeling plot saved as 'cluster_labeling_high_contrast_py.png'.")

# Example usage
if __name__ == "__main__":
    # Start the timer
    start_time = time.time()

    # Define lattice size and probability
    L = 6000
    p = 0.4
    np.set_printoptions(threshold=np.inf, linewidth=np.inf)

    # Generate a random lattice
    random_lattice = np.random.choice([0, 1], size=(L, L), p=[1-p, p])

    # Apply Hoshen-Kopelman algorithm
    labeled_clusters = hoshen_kopelman(random_lattice)

    # Print the original and labeled lattice
    print("Random Lattice:")
    print(random_lattice)
    print("Cluster-Labeled Lattice:")
    print(labeled_clusters)

    # End the timer and calculate execution time
    end_time = time.time()
    execution_time = end_time - start_time

    # Plot the clusters with metadata
    plot_clusters(labeled_clusters, execution_time, L, p)

    # Print total execution time
    print(f"Total execution time: {execution_time:.4f} seconds")
