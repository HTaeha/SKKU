import numpy as np

class LinearRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, epochs, batch_size, lr, optim):
        final_loss = None   # loss of final epoch

        # Training should be done for 'epochs' times with minibatch size of 'batch_size'
        # The function 'train' should return the loss of final epoch
        # Loss of an epoch is calculated as an average of minibatch losses

        # ========================= EDIT HERE ========================
        W = self.W
        for i in range(epochs):
            minibatch_loss = 0
            for j in range(len(x)//batch_size+1):
                if j*batch_size >= len(x):
                    break
                updated_x = x[j * batch_size:(j + 1) * batch_size]
                updated_y = y[j * batch_size:(j + 1) * batch_size].reshape(-1,1)
                
                grad = np.dot(updated_x, W) - updated_y
                grad = np.multiply(updated_x, grad)
                grad = grad.mean(axis=0).reshape(-1,1)

                W = optim.update(W, grad, lr)
                loss = np.square(np.dot(updated_x, W) - updated_y)
                minibatch_loss += loss.mean(axis=0)
        final_loss = minibatch_loss/(len(x)//batch_size+1)
        self.W = W

        # ============================================================
        return final_loss

    def eval(self, x):
        pred = None

        # Evaluation Function
        # Given the input 'x', the function should return prediction for 'x'

        # ========================= EDIT HERE ========================
        pred = np.dot(x, self.W)


        # ============================================================
        return pred
