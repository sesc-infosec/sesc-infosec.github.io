from matplotlib import pyplot as plt
import numpy as np

def go(
    ):
    plt.figure()
    plt.title("Функция ошибок", size=20)
    plt.xlabel("M(w)", size=16)
    plt.ylabel("L(M)", size=16)
    plt.axis([-5, 5, -0.5, 5])
    x = np.arange(-5, 5, 0.1)
    plt.plot([-5, 0, 0, 5], [1, 1, 0, 0], "r")
    plt.plot([-4, 1, 5], [5, 0, 0], "y", label="опрорные вектора")
    plt.plot(x, np.log2(1 + np.exp(-x)), "g", label="логистическая регрессия")
    plt.plot(x, 2 / (1 + np.exp(x)), "b", label="нейросети")
    plt.legend()
    plt.show()
