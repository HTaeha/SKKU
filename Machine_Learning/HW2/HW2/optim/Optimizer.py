import numpy as np

class SGD:
    def __init__(self, gamma, epsilon):
        self.gamma = gamma
        self.epsilon = epsilon

    def update(self, w, grad, lr):
        updated_weight = None
        updated_weight = w - lr*grad
        return updated_weight

class Momentum:
    def __init__(self, gamma, epsilon):
        self.gamma = gamma
        self.epsilon = epsilon
        self.v = 0

    def update(self, w, grad, lr):
        updated_weight = None
        self.v = self.gamma*self.v + lr*grad
        updated_weight = w - self.v
        return updated_weight


class RMSProp:
    def __init__(self, gamma, epsilon):
        self.gamma = gamma
        self.epsilon = epsilon
        self.G = 0

    def update(self, w, grad, lr):
        updated_weight = None
        self.G = self.gamma*self.G + (1-self.gamma)*(grad**2)
        updated_weight = w - lr*grad/((self.G+self.epsilon)**0.5)
        return updated_weight
