import sys
import random

import numpy as np
import keras
from sklearn import svm
from sklearn.model_selection import KFold
from sklearn.metrics import confusion_matrix
from sklearn.metrics import classification_report
from sklearn.preprocessing import LabelEncoder
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline
from keras.models import Model, Sequential
from keras.layers import Dense, Dropout, Input, BatchNormalization, LSTM
from keras.layers import Conv1D, MaxPooling1D
from sklearn.neural_network import BernoulliRBM
from keras.optimizers import Adam

'''
Canonical Models in machine learning.
Data :
        The credit approval data set from the UCI data repository
        http://archive.ics.uci.edu/ml/datasets/Credit+Approval
        Number of instances : 690
        Number of attributes : 15
        Attribute Information:
            A1:	b, a.
            A2:	continuous.
            A3:	continuous.
            A4:	u, y, l, t.
            A5:	g, p, gg.
            A6:	c, d, cc, i, j, k, m, r, q, w, x, e, aa, ff.
            A7:	v, h, bb, j, n, z, dd, ff, o.
            A8:	continuous.
            A9:	t, f.
            A10:	t, f.
            A11:	continuous.
            A12:	t, f.
            A13:	g, p, s.
            A14:	continuous.
            A15:	continuous.
            A16: +,-         (class attribute)
        Class Distribution
            +: 307 (44.5%)
            -: 383 (55.5%)
Model : 
        Decision Tree
        Support Vector Machine(SVM)
        Multi Layer Perceptron(MLP)
Evaluation : 
        Use 10-fold evaluation method.
        Accuracy
        Precision
        Recall
        F1-score
'''

# MLP batch size
batch_size = 32
# 1 : DecisionTree
# 2 : SVM
# 3 : MLP
model_select = int(sys.argv[1])

def evaluation_method(true, pred):
    #confusion_matrix
    # TN    FP
    # FN    TP
    cm = confusion_matrix(true, pred)
    TN = cm[0][0]
    FP = cm[0][1]
    FN = cm[1][0]
    TP = cm[1][1]
    accuracy = (TN + TP) / (TN + FP + FN + TP)
    precision = TP / (TP + FP)
    recall = TP / (TP + FN)
    f1_score = (2*precision*recall) / (precision + recall)
    print("accuracy : {}".format(accuracy))
    print("precision : {}".format(precision))
    print("recall : {}".format(recall))
    print("F1-score : {}".format(f1_score))
    print("=========================================\n")
    acc_list.append(accuracy)

if __name__ == "__main__":
    #Open data.
    filename = "data/crx.data"
    with open(filename, 'r') as data_file:           
        data = data_file.readlines()
    attribute = []
    label = []

    #Data shuffle.
    random.shuffle(data)

    #Label encoder.
    encoder = LabelEncoder()

    #Make input data.
    for item in data:
        attr_list = item.split(',')
        #Remove missing value data.
        if '?' in attr_list:
            continue
        #attr_list[-1][0] is label data. Other data is '\n', so delete it.
        attr_list[-1] = attr_list[0-1][0]
        #attr_list[-1] is label data. Other data is attribute.
        attribute.append(attr_list[:-1])
        # - is label 0, + is label 1.
        if attr_list[-1] == '-':
            label.append(0)
        else:
            label.append(1)
    X_data = np.asarray(attribute)
    Y_data = np.asarray(label).reshape((-1,1))

    # The number of attributes.
    attr_num = len(X_data[0])
    # Encode categorical data to number.
    for i in range(attr_num):
        if not (i == 1 or i == 2 or i == 7 or i == 10 or i == 13 or i == 14):
            encoder.fit(X_data[:,i])
            X_data[:,i] = encoder.transform(X_data[:,i])

    #10-fold
    kf = KFold(n_splits = 10)
    n = 1
    acc_list = []
    #Divide the entire data into 10 parts. 
    for train_index, test_index in kf.split(X_data):
        print("{}th cross validation".format(n))
        n += 1
        X_train, X_test = X_data[train_index], X_data[test_index]
        Y_train, Y_test = Y_data[train_index], Y_data[test_index]

        adam = Adam(lr=0.001, beta_1=0.9, beta_2=0.999, epsilon=1e-08)
        #1. CNN
        if model_select == 1:
            print("CNN")
            X_train = X_train.reshape(len(X_train), len(X_train[0]), 1)
            X_test = X_test.reshape(len(X_test), len(X_test[0]), 1)
            Y_train = Y_train.reshape(len(Y_train), len(Y_train[0]), 1)

            inputs = Input(shape=(attr_num,1), dtype='float32')
            conv = Conv1D(128, 3, padding='valid', activation='relu', strides=1)(inputs)
            pool = MaxPooling1D(pool_size=2)(conv)
            after_pool = BatchNormalization()(pool)
            conv2 = Conv1D(128, 3, padding='valid', activation='relu', strides=1)(after_pool)
            pool2 = MaxPooling1D(pool_size=3)(conv2)
            after_pool2 = BatchNormalization()(pool2)
            d = Dense(64, activation='relu')(after_pool2)
            after_d = BatchNormalization()(d)
            d2 = Dense(32, activation='relu')(after_d)
            after_d2 = BatchNormalization()(d2)
            output = Dense(1, activation='sigmoid')(after_d2)
            model = Model(inputs=inputs, outputs=output)

            model.compile(loss='binary_crossentropy', optimizer=adam, metrics=['accuracy'])
            model.summary() 
            model.fit(X_train, Y_train, batch_size = batch_size, epochs = 30, verbose = 1)

            predict = model.predict(X_test)
            predictY = np.around(predict)

            predictY = predictY.reshape(-1,1)
            evaluation_method(Y_test, predictY)

        #2. RBM
        elif model_select == 2:
            print("RBM")
            rbm = BernoulliRBM(n_components = 200, n_iter = 40,
                            learning_rate = 0.01,  verbose = True)
            logistic = LogisticRegression(C = 1.0)
         
            classifier = Pipeline([("rbm", rbm), ("logistic", logistic)])
            classifier.fit(X_train, Y_train)

            predictY = classifier.predict(X_test)
            evaluation_method(Y_test, predictY)

        #3. RNN
        elif model_select == 3:
            print("RNN")
            X_train = X_train.reshape(len(X_train), len(X_train[0]), 1)
            X_test = X_test.reshape(len(X_test), len(X_test[0]), 1)

            inputs = Input(shape=(attr_num, 1), dtype='float32')
            forward = LSTM(128)(inputs)
            after_forward = BatchNormalization()(forward)
            backward = LSTM(128, go_backwards=True)(inputs)
            after_backward = BatchNormalization()(backward)
            merge = keras.layers.concatenate([after_forward, after_backward], axis=-1)
            after_merge = BatchNormalization()(merge)

            d = Dense(64, activation='relu')(after_merge)
            after_d = BatchNormalization()(d)
            d2 = Dense(32, activation='relu')(after_d)
            after_d2 = BatchNormalization()(d2)

            output = Dense(1, activation='sigmoid')(after_d2)
            model = Model(inputs=inputs, outputs=output)
            model.compile(loss='binary_crossentropy', optimizer=adam, metrics=['accuracy'])
            
            model.summary() 
            model.fit(X_train, Y_train, batch_size = batch_size, epochs = 50, verbose = 1)

            predict = model.predict(X_test)
            predictY = np.around(predict)
            evaluation_method(Y_test, predictY)

    # Print 10-fold accuracy.
    print(acc_list)
    # Print average accuracy.
    print(sum(acc_list)/len(acc_list))
