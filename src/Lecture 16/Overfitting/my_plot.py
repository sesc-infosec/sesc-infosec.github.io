from matplotlib import pyplot as plt
import numpy as np
from tensorflow import keras


def go(
    ):
    loss_train, loss_cv = _fit()
    _draw(loss_train, loss_cv)


def _draw(
    loss_train,
    loss_cv
    ):
    X = np.arange(1, len(loss_train) + 1, 1)

    plt.figure()
    plt.title("Train/validation losses")
    plt.plot(X, loss_train, "g", label="train loss")
    plt.plot(X, loss_cv, "r", label="validation loss")
    plt.legend()
    plt.show()


def _fit(
    ):
    model = keras.Sequential()
    model.add(keras.layers.Dense(20, input_shape=(1,), activation="sigmoid"))
    model.add(keras.layers.Dense(20, activation="sigmoid"))
    model.add(keras.layers.Dense(1, activation="linear"))
    model.compile(optimizer='adam', loss="mean_squared_error")

    np.random.seed(104)
    # X_train = np.linspace(0, 20, 100)
    # y_train = 3 * np.sin(X_train) + np.random.normal(0, 0.3, 100)
    X_train = np.linspace(0, 20, 10)
    y_train = 3 * np.sin(X_train) + np.random.normal(0, 0.3, 10)
    loss_train = []
    X_cv = np.linspace(0, 20, 30)
    y_cv = 3 * np.sin(X_cv) + np.random.normal(0, 0.3, 30)
    loss_cv = []

    # steps_number = 400
    steps_number = 100
    step_size = 100

    for i in range(steps_number):
        model.fit(X_train, y_train, epochs=step_size, verbose=0)
        loss_train.append(model.evaluate(X_train, y_train, verbose=0))
        loss_cv.append(model.evaluate(X_cv, y_cv, verbose=0))
        print(i)

    return loss_train, loss_cv
