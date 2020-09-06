from socket import *
import time
import os
from threading import Thread
from random import *
import collections


IP = ''
Port = 10080
ADDR = (IP, Port)
queue = collections.deque()
incoming_len = 0
forwarding_len = 0
start_time = 0

def padzero(num, digits):
    num = str(num)
    if digits - len(num) < 0:
        raise OverflowError
    for i in range(digits - len(num)):
        num = '0' + num
    return num
def NEM(BLR, queue_size):
    global start_time, incoming_len, forwarding_len
    time_left = 1/BLR
    next_time = 1/BLR

    while True:
        current_time = time.time() - start_time
        if current_time > next_time:
            data, s_ADDR = r_sock.recvfrom(1500)
            pkt_num = int(data[:8].decode())
            print(pkt_num)           
            incoming_len += 1
            if len(queue) == queue_size:
                print("packet drop")
                continue
            else:
                queue.append(pkt_num)
                RM(pkt_num, s_ADDR)

            forwarding_len += 1
            next_time = current_time + time_left

def RM(pkt_num, s_ADDR):
    data = queue.pop()

    r_sock.sendto(str(padzero(data, 8)).encode(), s_ADDR)
# print("pkt_num : %s"&(data))
def store_information():
    global incoming_len, forwarding_len, start_time
    time_left = 0.1  #2
    next_time = 0.1 #2
    queue_sum = 0
    count = 0
    
    f_NEM = open('NEM.log', 'a')
    f_RM = open('RM.log', 'a')

    current = time.time()
    while True:
        current_time = time.time() - start_time

        if current_time > next_time:
            queue_sum += len(queue)
            count += 1

            if count == 20:
                f_NEM.write("%.3f | incoming_rate : %.4f\n"%(current_time, incoming_len/2))
                f_NEM.write("%.3f | forwarding_rate : %.4f\n"%(current_time, forwarding_len/2))
                f_NEM.write("%.3f | avg_queue_utilization : %.4f\n"%(current_time, queue_sum/20))

                incoming_len = 0
                forwarding_len = 0
                queue_sum = 0
            next_time = current_time + time_left

    f_NEM.close()
    f_RM.close()

def start_receiver(BLR, queue_size):
#    time_left = 1
#   next_time = 0
    global start_time
    start_time = time.time()

    f_NEM = open('NEM.log', 'w')
    f_NEM.close()
    f_RM = open('RM.log', 'w')
    f_RM.close()

    threads = []
    t_NEM = Thread(target = NEM, args = (BLR, queue_size))
    threads.append(t_NEM)
    t_store_info = Thread(target = store_information)
    threads.append(t_store_info)

    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()
'''
    while True:
        current_time = time.time() - start_time
        if current_time > next_time:
            t_NEM.start()
            next_time = current_time + time_left
            '''
if __name__ == "__main__":
    BLR = int(input("BLR : "))
    queue_size = int(input("queue_size : "))

    r_sock = socket(AF_INET, SOCK_DGRAM)
    r_sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, SO_RCVBUF)
    r_sock.bind(ADDR)

    RCV_BUF = r_sock.getsockopt(SOL_SOCKET, SO_RCVBUF)
    print("socket recv buffer size: ", RCV_BUF)
    if RCV_BUF < 10000000:
        RCV_BUF = 10000000
        r_sock.setsockopt(SOL_SOCKET, SO_RCVBUF, RCV_BUF)
    RCV_BUF = r_sock.getsockopt(SOL_SOCKET, SO_RCVBUF)
    print("socket recv buffer size updated: ", RCV_BUF)
    print("\n\nreceiver program starts...")

    start_receiver(BLR, queue_size)

    r_sock.close()
