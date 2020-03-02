import matplotlib.pyplot as plt
import numpy as np

from sklearn.cluster import KMeans

def go(
    ):
    X = np.vstack((np.random.multivariate_normal([20, 10], [[10, 0], [0, 10]], 100),
                   np.random.multivariate_normal([30, 30], [[10, 0], [0, 10]], 100),
                   np.random.multivariate_normal([10, 30], [[10, 0], [0, 10]], 100)))

    kmeans = KMeans(n_clusters=4, random_state=0).fit(X)
    Y = kmeans.predict(X)

    plt.figure()
    col = ["ro", "go", "bo", "yo"]
    for i in range(len(X)):
        plt.plot(X[i][0], X[i][1], col[Y[i]])
    plt.show()
