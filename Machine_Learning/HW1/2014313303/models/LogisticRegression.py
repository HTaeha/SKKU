import numpy as np

class LogisticRegression:
    def __init__(self, num_features):
        self.num_features = num_features
        self.W = np.zeros((self.num_features, 1))

    def train(self, x, y, epochs, batch_size, lr, optim):
        final_loss = None   # loss of final epoch

        # Train should be done for 'epochs' times with minibatch size of 'batch size'
        # The function 'train' should return the loss of final epoch
        # Loss of an epoch is calculated as an average of minibatch losses

        # ========================= EDIT HERE ========================


        W = self.W
        epsilon = 0.00001
        for i in range(epochs):
            minibatch_loss = 0.
            for j in range(len(x)//batch_size+1):
                updated_x = x[j*batch_size:(j+1)*batch_size]
                updated_y = y[j*batch_size:(j+1)*batch_size]

                h = self._sigmoid(np.dot(updated_x, W).T)- updated_y
                grad = np.multiply(updated_x, h.T)
                grad = grad.sum(axis=0)
                for l in range(len(W)):
                    W[l] = optim.update(W[l], grad[l], lr)
                loss_h = self._sigmoid(np.dot(updated_x, W).T)
                loss = np.multiply(np.log(loss_h+epsilon), updated_y) + np.multiply((1-updated_y), np.log(1-loss_h+epsilon))
                minibatch_loss += -loss.sum(axis=1)
        final_loss = minibatch_loss/(len(x)//batch_size+1)
        self.W = W

        # ============================================================
        return final_loss

    def eval(self, x):
        threshold = 0.5
        pred = None

        # Evaluation Function
        # Given the input 'x', the function should return prediction for 'x'
        # The model predicts the label as 1 if the probability is greater or equal to 'threshold'
        # Otherwise, it predicts as 0

        # ========================= EDIT HERE ========================
        res = np.dot(x, self.W)
        pred = res
        for i in range(len(x)):
            if res[i] >= threshold:
                pred[i] = 1
            else:
                pred[i] = 0

        # ============================================================

        return pred

    def _sigmoid(self, x):
        sigmoid = None

        # Sigmoid Function
        # The function returns the sigmoid of 'x'

        # ========================= EDIT HERE ========================
        sigmoid = 1/(1+np.exp(-1*x))
        # ============================================================
        return sigmoid
