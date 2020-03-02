import numpy as np
from sklearn.decomposition import PCA

def go(
    ):
    np.random.random(100)
    X = np.vstack((7 * np.random.random(100) + 8, 10 * np.random.random(100) + 3))
    X = np.vstack((X, X[0] + X[1] + -0.5 * np.random.random(100)))

    pca = PCA()
    pca.fit(X)
    y = pca.fit_transform(X)
    print(pca.explained_variance_ratio_)
