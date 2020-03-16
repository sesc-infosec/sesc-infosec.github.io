from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
from tensorflow import keras


def go(
    ):
    _init()

    FuncAnimation(_fig, _animate, frames=20000, blit=True, repeat=False)

    plt.show()


_X_train = np.linspace(0, 20, 100)
_y_train = 3 * np.sin(_X_train) + np.random.normal(0, 0.3, 100)
_loss_train = []
_X_cv = np.linspace(0, 20, 30)
_y_cv = 3 * np.sin(_X_cv) + np.random.normal(0, 0.3, 30)
_loss_cv = []

_model = keras.Sequential()

_fig, _ = plt.subplots()


def _animate(
    i
    ):
    loss_train, loss_cv = _step(5)
    _loss_train.append(loss_train)
    _loss_cv.append(loss_cv)
    X = np.arange(1, len(_loss_train) + 1, 1)
    p1, = plt.plot(X, _loss_train, "g")
    p2, = plt.plot(X, _loss_cv, "r")
    return p1, p2,


def _init(
    ):
    _model.add(keras.layers.Dense(20, input_shape=(1,), activation="sigmoid"))
    _model.add(keras.layers.Dense(20, activation="sigmoid"))
    _model.add(keras.layers.Dense(1, activation="linear"))
    _model.compile(optimizer='adam', loss="mean_squared_error")

    plt.axis([0, 20000, 0, 5])


def _step(
    n_epochs
    ):
    _model.fit(_X_train, _y_train, epochs=n_epochs, verbose=0)
    loss_train = _model.evaluate(_X_train, _y_train, verbose=0)
    loss_cv = _model.evaluate(_X_cv, _y_cv, verbose=0)
    return loss_train, loss_cv
