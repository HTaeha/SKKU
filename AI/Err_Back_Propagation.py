from random import *
from math import exp

train_len = 11
x_len = 1
t_len = 1
hidden_node_num = 11

train_x = [[0 for i in range(x_len)] for n in range(train_len)]
train_t = [[0 for i in range(x_len)] for n in range(train_len)]
weight_kj = [[0 for j in range(hidden_node_num)] for k in range(t_len)]
weight_ji = [[0 for i in range(x_len)] for j in range(hidden_node_num)]
hidden = [0 for j in range(hidden_node_num)]
output = [0 for k in range(t_len)]
d_kj = [[0 for j in range(hidden_node_num)] for k in range(t_len)]
d_ji = [[0 for i in range(x_len)] for j in range(hidden_node_num)]

test_len = 101
test_x = [[0 for i in range(x_len)] for n in range(test_len)]
test_t = [[0 for i in range(x_len)] for n in range(test_len)]
learning_rate = 0.7
epoch = 100000

#sigmoid function
def SIGMOID(n):
	return 1/(1 + exp(-(n)))

#initialize t, weight, test data
def init():
	for j in range(hidden_node_num):
		for i in range(x_len):
			weight_ji[j][i] = uniform(-0.1, 0.1)
	for k in range(t_len):
		for j in range(hidden_node_num):
			weight_kj[k][j] = uniform(-0.1, 0.1)

	train_num = 0.0
	for n in range(train_len):
		for i in range(x_len):
			train_x[n][i] = train_num
			train_t[n][i] = train_x[n][i]*(1-train_x[n][i])
			train_num += 0.1

	test_num = 0.0
	for n in range(test_len):
		for i in range(x_len):
			test_x[n][i] = test_num
			test_t[n][i] = test_x[n][i]*(1-test_x[n][i])
			test_num += 0.01

def forward(x):
	net1 = 0

	for j in range(hidden_node_num):
		for i in range(x_len):
			net1 = weight_ji[j][i]*x[i]
		hidden[j] = (SIGMOID(net1))
		
	for k in range(t_len):
		for j in range(hidden_node_num):
			net2 = weight_kj[k][j]*hidden[k]
		output[k] = (SIGMOID(net2))

def backward(x, t):
	for k in range(t_len):
		for j in range(hidden_node_num):
			d_num = (-1)*(t[k]-output[k])*output[k]*(1-output[k])*hidden[j]
			d_kj[k][j] = d_num
	for j in range(hidden_node_num):
		for i in range(x_len):
			for k in range(t_len):
				d_num = weight_kj[k][j]*(t[k]-output[k])*output[k]*(1-output[k])
			d_num *= (-1)*x[i]*hidden[j]*(1-hidden[j])
			d_ji[j][i] = d_num

def update_weight():
	d_kj_sum = []
	d_ji_sum = []
	for k in range(t_len):
		for j in range(hidden_node_num):
			weight_kj[k][j] -= learning_rate*d_kj[k][j]
	for j in range(hidden_node_num):
		for i in range(x_len):
			weight_ji[j][i] -= learning_rate*d_ji[j][i]

if __name__ == "__main__":
	init()

	for i in range(epoch):
		for n in range(train_len):
			forward(train_x[n])
			backward(train_x[n], train_t[n])
			update_weight()

	test_output = []
	for n in range(test_len):
		forward(test_x[n])
		test_output.append(output[0])
