from matplotlib import pyplot as plt
import numpy as np
from sklearn import linear_model

def go(
    ):
    X1 = np.random.multivariate_normal([10, 10], [[10, 0], [0, 10]], 100)
    y1 = np.full_like(np.arange(100), 1)
    X2 = np.random.multivariate_normal([30, 30], [[10, 0], [0, 10]], 100)
    y2 = np.full_like(np.arange(100), 2)

    X = np.vstack((X1, X2))
    y = np.hstack((y1, y2))
    clf = linear_model.LogisticRegression()
    clf.fit(X, y)

    w = clf.coef_[0]
    a = -w[0] / w[1]
    b = -clf.intercept_[0] / w[1]

    plt.figure()
    plt.title("Logistic regression")
    xx = np.array([0, 40])
    plt.plot(xx, a * xx + b, "b-")
    plt.plot(X1[:, 0], X1[:, 1], "ro")
    plt.plot(X2[:, 0], X2[:, 1], "go")
    plt.axis([0, 40, 0, 40])
    plt.show()
