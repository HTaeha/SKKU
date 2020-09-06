from socket import *
import time
import os
from threading import Thread
from random import *

IP = ''
Port = 10080
ADDR = (IP, Port)
num = 0

def padzero(num, digits):
    num = str(num)
    if digits - len(num) < 0:
        raise OverflowError
    for i in range(digits - len(num)):
        num = '0' + num
    return num

if __name__ == "__main__":
    pkt_loss_prob = float(input("packet loss probability: "))

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

    file, s_ADDR = r_sock.recvfrom(1024)

    file = file.decode()
    file_split = file.split('.')
    file_name = file_split[0]
    log_file_name = file_name + "_receiving_log.txt"
    f_log = open(log_file_name, 'w')
    f_log.close()

    start_time = time.time()
    
    pkt_len, s_ADDR = r_sock.recvfrom(1024)
    pkt_len = int(pkt_len.decode())
    row = pkt_len
    col = 2
    pkt_arr = [[-1 for j in range(col)] for i in range(row)]
    while True:
        data, s_ADDR = r_sock.recvfrom(1500)
        r_ack_num = int(data[:8].decode())
        pkt_data = data[8:]
        
        if r_ack_num == 99999999:
            with open(file, 'wb') as f:
                for i in range(len(pkt_arr)):
                    f.write(pkt_arr[i][0])
            f_log = open(log_file_name, 'a')
            tm = time.time() - start_time
            f_log.write("\nFile transfer is finished.\n")
            f_log.write("Throughput: %.2f pkts / sec\n" % (pkt_len/tm))
            f_log.close()
            break
        
        
        pkt_arr[r_ack_num][1] = r_ack_num
        
        
        f_log = open(log_file_name, 'a')
        tm = time.time() - start_time
        f_log.write("%.3f pkt: %d | received\n" % (tm, r_ack_num))
        f_log.close()

        rand_num = random()

        if rand_num <= pkt_loss_prob:
            f_log = open(log_file_name, 'a')
            tm = time.time() - start_time
            f_log.write("%.3f pkt: %d | dropped\n" % (tm, r_ack_num))
            print("%d dropped" % r_ack_num)
            f_log.close()
            pkt_arr[r_ack_num][1] = -1
            continue
        else:
            pkt_arr[r_ack_num][0] = pkt_data
            '''if pkt_arr[0][1] == -1:
                num = -1
                r_sock.sendto(str(num).encode(), s_ADDR)
                '''
            #else:
            for i in range(1, len(pkt_arr)):
                num = -1
                if pkt_arr[i][1] == -1:
                    num = pkt_arr[i-1][1]
                    break
                if num == -1:
                    num = len(pkt_arr) - 1
            if pkt_arr[0][1] == -1:
                r_sock.sendto(str(-1).encode(), s_ADDR)
            else:
                r_sock.sendto(str(padzero(num, 8)).encode(), s_ADDR)
            f_log = open(log_file_name, 'a')
            tm = time.time() - start_time
            f_log.write("%.3f ACK: %d | sent\n" % (tm, num))
            f_log.close()


    r_sock.close()
