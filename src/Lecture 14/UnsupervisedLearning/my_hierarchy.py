from matplotlib import pyplot as plt
import numpy as np
from scipy.cluster.hierarchy import dendrogram
from sklearn.cluster import AgglomerativeClustering

def go(
    ):

    # X = np.array([[1, 1], [1, 0], [0, 0], [5, 6], [6, 6], [5, 0], [6, 0]])
    # X = np.array([[1, 1], [2, 2], [3, 3], [4, 4], [5, 5], [6, 6], [7, 7], [8, 8]])
    X = np.vstack((np.random.multivariate_normal([20, 30], [[10, 0], [0, 10]], 100),
                   np.random.multivariate_normal([30, 30], [[10, 0], [0, 10]], 100),
                   np.random.multivariate_normal([10, 30], [[10, 0], [0, 10]], 100)))

    # setting distance_threshold=0 ensures we compute the full tree.
    model = AgglomerativeClustering(distance_threshold=0, n_clusters=None)

    model = model.fit(X)
    plt.title('Hierarchical Clustering Dendrogram')
    # plot the top three levels of the dendrogram
    _plot_dendrogram(model, truncate_mode='level', p=3)
    plt.xlabel("Number of points in node (or index of point if no parenthesis).")
    _plot_points(X)
    plt.show()



def _plot_dendrogram(model, **kwargs):
    # Create linkage matrix and then plot the dendrogram

    # create the counts of samples under each node
    counts = np.zeros(model.children_.shape[0])
    n_samples = len(model.labels_)
    for i, merge in enumerate(model.children_):
        current_count = 0
        for child_idx in merge:
            if child_idx < n_samples:
                current_count += 1  # leaf node
            else:
                current_count += counts[child_idx - n_samples]
        counts[i] = current_count

    linkage_matrix = np.column_stack([model.children_, model.distances_,
                                      counts]).astype(float)

    # Plot the corresponding dendrogram
    dendrogram(linkage_matrix, **kwargs)


def _plot_points(
    points
    ):
    plt.figure()
    plt.axis()
    plt.title("Объекты")
    plt.plot(points[:, 0], points[:, 1], "ro")
