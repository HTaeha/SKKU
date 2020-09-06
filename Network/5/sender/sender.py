from socket import *
import time
import os
from threading import Thread
import threading

pkt_size = 1400
stop_sign = False
window_size = 0
pkt_count = 0
sending_time = []  #보낸 pkt의 시간 저장
s_pkt_num = 0
sum_RTT = 0
len_RTT = 0
timeout = 0.2
start_time = 0
sending_rate_len = 0

def padzero(num, digits):
    num = str(num)
    if digits - len(num) < 0:
        raise OverflowError
    for i in range(digits - len(num)):
        num = '0' + num
    return num
def timer():
    global timeout, stop_sign

    while True:
        if stop_sign == True:
            return

        for i in range(len(sending_time)):
            if sending_time != -1 and time.time() - sending_time[i] > timeout:
                AIMD()
                break
def AIMD():
    global window_size

    lock.acquire()
    window_size // 2
    lock.release()
def receiving_ACK():
    global window_size, sum_RTT, len_RTT, pkt_count, stop_sign

    while True:
        if stop_sign == True:
            return
        msg, r_ADDR = s_sock.recvfrom(1024)
        
        lock.acquire()
        current_ACK = int(msg.decode())

        print("current_ACK : %d"%(current_ACK))
#avg_RTT 계산.
        try:
            sum_RTT += time.time() - sending_time[current_ACK]        
        except IndexError:
            print("IndexError \ts_pkt_num : %d, current_ACK : %d"%(s_pkt_num, current_ACK))
        
        len_RTT += 1  #len_RTT는 들어온 ACK의 갯수.

        sending_time[current_ACK] = -1
        window_size += 1

        if pkt_count > 0:
            pkt_count -= 1
        lock.release()

def sending_pkt(r_ADDR, start_time):
    global stop_sign, pkt_count, s_pkt_num, window_size, sending_rate_len

    msg = 'Hello World!!'

    while True:
        if stop_sign == True:
            return
        if pkt_count == window_size:
            continue
        elif 0 <= pkt_count and pkt_count < window_size:
            lock.acquire()
            pkt_data = str(padzero(s_pkt_num, 8)).encode() + msg.encode()
            s_sock.sendto(pkt_data, r_ADDR)
            sending_time.append(time.time())
            print("pkt_num: %d, sending_time : %.4f, window_size : %d"%(s_pkt_num, sending_time[s_pkt_num]-start_time, window_size))
            pkt_count += 1
            s_pkt_num += 1
            sending_rate_len += 1

            lock.release()

def store_information():
    global stop_sign, sending_rate_len, len_RTT, sum_RTT, start_time
    time_left = 2
    next_time = 2

    log_file_name = str(s_sock.getsockname()[1]) + '_log.txt'
    print(log_file_name)
    f = open(log_file_name, 'w')
    f.close()
    f = open(log_file_name, 'a')

    while True:
        if stop_sign == True:
            print("stopstostopeji")
            return
        current_time = time.time() - start_time
#f.write(str(int(current_time)))
#      f.write("\n")
        lock.acquire()
        if current_time >= next_time:
            if len_RTT != 0:
                avg_RTT = sum_RTT/len_RTT
            else:
                avg_RTT = 0
            
            f.write("%.3f | avg_RTT : %.4f\n"%(current_time, avg_RTT))
            f.write("%.3f | Sending rate : %.4f\n"%(current_time, (sending_rate_len)/2))
            f.write("%.3f | Goodput : %.4f\n"%(current_time, (len_RTT)/2))

            sending_rate_len = 0
            len_RTT = 0
            sum_RTT = 0

            next_time += time_left
        lock.release()
    f.close()
def stop_sign():
    stop = input()
    if stop == 'stop':
        stop_sign = True
def start_sender(IP):
    global start_time
    r_ADDR = (IP, 10080)
    
    start_time = time.time()
#t = Thread(target = start_sender, args = (IP))
    #t.start()
    
    threads = []
    t_send_pkt = Thread(target = sending_pkt, args = (r_ADDR, start_time))
    threads.append(t_send_pkt)
    t_receive_ACK = Thread(target = receiving_ACK)
    threads.append(t_receive_ACK)
    t_store_info = Thread(target = store_information)
    threads.append(t_store_info)
    t_stop_sign = Thread(target = stop_sign)
    threads.append(t_stop_sign)
    t_timer = Thread(target = timer)
    threads.append(t_timer)

    for thread in threads:
        thread.start()

    for thread in threads:
        thread.join()
        
if __name__ == '__main__':
    s_sock = socket(AF_INET, SOCK_DGRAM)
    lock = threading.Lock()

    IP = input("Receiver IP address: ")
    start, window_size = input().split()
    window_size = int(window_size)	
    
    start_sender(IP)

    s_sock.close()
