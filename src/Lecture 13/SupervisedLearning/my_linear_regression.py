import matplotlib.pyplot as plt
import numpy as np
from sklearn import linear_model

def go(
    ):
    X = np.array([2, 3, 4, 5, 6]).reshape((-1, 1))
    y = np.array([5, 7, 6, 8, 7])
    reg = linear_model.LinearRegression()

    for _ in range(5):
        reg.fit(X, y)
        w = np.insert(reg.coef_, 0, reg.intercept_)
        _draw(X, w, y)
        X = np.hstack((X, (X[:, 0] * X[:, -1]).reshape(-1, 1)))


def _draw(
    X,
    w,
    y
    ):
    plt.figure()
    plt.title("n = " + str(X.shape[1]))
    Xs = np.arange(X[:, 0].min() - 1, X[:, 0].max() + 1, 0.1)
    plt.plot(Xs, _f(Xs, w), "g-")
    plt.plot(X[:, 0], y, "ro")
    plt.show()


def _f(
    X,
    W
    ):
    Y = np.zeros(X.size)
    for i in range(X.size):
        power = 0
        for w in W:
            Y[i] += pow(X[i], power) * w
            power += 1

    return Y
