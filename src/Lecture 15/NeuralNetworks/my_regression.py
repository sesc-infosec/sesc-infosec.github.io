from matplotlib import pyplot as plt
import numpy as np
from tensorflow import keras


def go():
    X_train = np.linspace(0, 20, 100)
    y_train = 3 * np.sin(X_train) + np.random.normal(0, 0.3, 100)
    X_test = np.linspace(20, 30, 50)
    y_test = 3 * np.sin(X_test) + np.random.normal(0, 0.3, 50)

    model = keras.Sequential()
    model.add(keras.layers.Dense(20, input_shape=(1,), activation="sigmoid"))
    model.add(keras.layers.Dense(20, activation="sigmoid"))
    model.add(keras.layers.Dense(1, activation="linear"))

    model.compile(optimizer='adam', loss="mean_squared_error")

    for _ in range(10):
        model.fit(X_train, y_train, epochs=10000, verbose=0)

        y1_train = model.predict(X_train)
        y1_test = model.predict(X_test)

        plt.figure()
        plt.plot(X_train, y_train, "g-")
        plt.plot(X_test, y_test, "g-")
        plt.plot(X_train, y1_train, "b-")
        plt.plot(X_test, y1_test, "r-")
        plt.show()
