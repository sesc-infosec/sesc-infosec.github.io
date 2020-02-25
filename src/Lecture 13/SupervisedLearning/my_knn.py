from matplotlib import pyplot as plt
import numpy as np
from sklearn import neighbors

def go(
    ):
    X1 = np.random.multivariate_normal([20, 10], [[10, 0], [0, 10]], 100)
    y1 = np.full_like(np.arange(100), 1)
    X2 = np.random.multivariate_normal([30, 30], [[10, 0], [0, 10]], 100)
    y2 = np.full_like(np.arange(100), 2)
    X3 = np.random.multivariate_normal([10, 30], [[10, 0], [0, 10]], 100)
    y3 = np.full_like(np.arange(100), 3)

    X = np.vstack((X1, X2, X3))
    y = np.hstack((y1, y2, y3))

    clf = neighbors.KNeighborsClassifier()
    clf.fit(X, y)

    plt.figure()
    plt.title("kNN")
    for y in range(40):
        for x in range(40):
            cols = ["ro", "go", "bo"]
            k = clf.predict([[x, y]])[0]
            plt.plot([x], [y], cols[k - 1])
    plt.plot(X1[:, 0], X1[:, 1], "ro")
    plt.plot(X2[:, 0], X2[:, 1], "go")
    plt.plot(X3[:, 0], X3[:, 1], "bo")
    plt.axis([0, 40, 0, 40])
    plt.show()
