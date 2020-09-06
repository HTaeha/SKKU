import numpy as np

class SoftmaxClassifier:
    def __init__(self, num_features, num_label):
        self.num_features = num_features
        self.num_label = num_label
        self.W = np.zeros((self.num_features, self.num_label))

    def train(self, x, y, epochs, batch_size, lr, optimizer):
        """
        N : # of training data
        D : # of features
        C : # of classes

        [INPUT]
        x : (N, D), input data (first column is bias for all data)
        y : (N, )
        epochs: (int) # of training epoch to execute
        batch_size : (int) # of minibatch size
        lr : (float), learning rate
        optimizer : (Python class) Optimizer

        [OUTPUT]
        final_loss : (float) loss of last training epoch

        [Functionality]
        Given training data, hyper-parameters and optimizer, execute training procedure.
        Training should be done in minibatch (not the whole data at a time)
        Procedure for one epoch is as follow:
        - For each minibatch
            - Compute probability of each class for data
            - Compute softmax loss
            - Compute gradient of weight
            - Update weight using optimizer
        * loss of one epoch = Mean of minibatch losses
        (minibatch losses = [0.5, 1.0, 1.0, 0.5] --> epoch loss = 0.75)

        """
        print('========== TRAINING START ==========')
        final_loss = None   # loss of final epoch
        num_data, num_feat = x.shape
        losses = []
        for epoch in range(1, epochs + 1):
            batch_losses = []   # list for storing minibatch losses

        # ========================= EDIT HERE ========================
            for j in range(len(x)//batch_size+1):
                if j*batch_size >= len(x):
                    break
                updated_x = x[j * batch_size:(j + 1) * batch_size]
                updated_y = y[j * batch_size:(j + 1) * batch_size].reshape(-1,1)

                prob = np.zeros((len(updated_x), self.num_label))

                prob = np.dot(updated_x, self.W)
                prob = self._softmax(prob)
                
                batch_losses.append(self.softmax_loss(prob, updated_y))

                grad_weight = self.compute_grad(updated_x, self.W, prob, updated_y)
        
                self.W = optimizer.update(self.W, grad_weight, lr)
               
        # ============================================================
            epoch_loss = sum(batch_losses) / len(batch_losses)  # epoch loss
            # print loss every 10 epoch
            if epoch % 10 == 0:
                print('Epoch %d : Loss = %.4f' % (epoch, epoch_loss))
            # store losses
            losses.append(epoch_loss)
        final_loss = losses[-1]

        return final_loss

    def eval(self, x):
        """

        [INPUT]
        x : (N, D), input data

        [OUTPUT]
        pred : (N, ), predicted label for N test data

        [Functionality]
        Given N test data, compute probability and make predictions for each data.
        """
        pred = None
        # ========================= EDIT HERE ========================
                
        pred = np.zeros((len(x), 1))
        prob = np.dot(x, self.W)
        
        for i in range(len(x)):
            pred[i][0] = np.where(prob[i,:] == np.max(prob[i,:]))[0][0]

        # ============================================================
        return pred

    def softmax_loss(self, prob, label):
        """
        N : # of minibatch data
        C : # of classes

        [INPUT]
        prob : (N, C), probability distribution over classes for N data
        label : (N, ), label for each data

        [OUTPUT]
        softmax_loss : scalar, softmax loss for N input

        [Functionality]
        Given probability and correct label, compute softmax loss for N minibatch data
        """
        softmax_loss = 0.0
        # ========================= EDIT HERE ========================
        epsilon = 0.00000001
        for n in range(len(label)):
            softmax_loss += -(np.log(prob[n][label[n]] + epsilon))

        softmax_loss /= len(label)

        # ============================================================
        return softmax_loss

    def compute_grad(self, x, weight, prob, label):
        """
        N : # of minibatch data
        D : # of features
        C : # of classes

        [INPUT]
        x : (N, D), input data
        weight : (D, C), Weight matrix of classifier
        prob : (N, C), probability distribution over classes for N data
        label : (N, ), label for each data. (0 <= c < C for c in label)

        [OUTPUT]
        gradient of weight: (D, C), Gradient of weight to be applied (dL/dW)

        [Functionality]
        Given input (x), weight, probability and label, compute gradient of weight.
        """
        grad_weight = np.zeros_like(weight, dtype=np.float32) # (D, C)
        # ========================= EDIT HERE ========================
        for n in range(len(label)):
            prob[n][label[n]] -= 1
        grad_weight = np.dot(x.T, prob)/len(label)

        # ============================================================
        return grad_weight


    def _softmax(self, x):
        """
        [INPUT]
        x : (N, C), score before softmax

        [OUTPUT]
        softmax : (same shape with x), softmax distribution over axis-1

        [Functionality]
        Given an input x, apply softmax function over axis-1 (classes).
        """
        softmax = None
        # ========================= EDIT HERE ========================
        softmax = np.zeros((x.shape))
        epsilon = 0.00000001
        for n in range(len(x)):
            sum_prob = 0
            for c in range(len(x[0])):
                softmax[n][c] = np.exp(x[n][c] - np.max(x[n,:]))
                sum_prob += softmax[n][c]
            softmax[n,:] /= (sum_prob + epsilon)
        
        # ============================================================
        return softmax
