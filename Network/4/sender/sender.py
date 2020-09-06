from socket import *
import time
import os
from threading import Thread

pkt_size = 1400
pkt_divided = []
pkt_state = []  #0 ACK을 아직 못 받음.
                #1 ACK을 받아서 전송 완료된 것을 확인.
start_window = 0
duplicated = 0
num = 0
final = False

s_sock = socket(AF_INET, SOCK_DGRAM)

def padzero(num, digits):
    num = str(num)
    if digits - len(num) < 0:
        raise OverflowError
    for i in range(digits - len(num)):
        num = '0' + num
    return num

def receiving_ACK(s_pkt_num, log_file_name, window_size, r_ADDR, pkt_len, start_time):
    global num, final, duplicated
    
    s_pkt_num = 0
    before_ack = 0
    if pkt_len < window_size:
        for i in range(pkt_len):
            sending_pkt(s_pkt_num, log_file_name, r_ADDR, pkt_len, start_time)
            s_pkt_num += 1
    else:
        for i in range(window_size):
            sending_pkt(s_pkt_num, log_file_name, r_ADDR, pkt_len, start_time)
            s_pkt_num += 1
    while True:
        msg, r_ADDR = s_sock.recvfrom(1024)
        r_ACK_num = int(msg.decode())

        print("ack: %d, pkt : %d"%(r_ACK_num, s_pkt_num))
        f_log = open(log_file_name, 'a')
        tm = time.time() - start_time
        f_log.write("%.3f ACK: %d | received\n" % (tm, r_ACK_num))
        f_log.close()

        pkt_state[r_ACK_num] = 1

        if r_ACK_num + 1 == pkt_len:
            final = True
            sending_pkt(s_pkt_num, log_file_name, r_ADDR, pkt_len, start_time)
            return
        
        if s_pkt_num == pkt_len:
            continue
        
        if duplicated >= 3:
            '''if r_ACK_num == 0:
                print("dropped : ", 0)
                f_log = open(log_file_name, 'a')
                tm = time.time() - start_time
                f_log.write("%.3f pkt: %d | 3 duplicated ACKs\n" % (tm, 0))
                f_log.close()
                sending_pkt(0, log_file_name, r_ADDR, pkt_len)
            else:'''
            for i in range(pkt_len):
                if pkt_state[i] == 0:
                    print("dropped : ", i)
                    f_log = open(log_file_name, 'a')
                    tm = time.time() - start_time
                    f_log.write("%.3f pkt: %d | 3 duplicated ACKs\n" % (tm, i-1))
                    f_log.close()
                    sending_pkt(i, log_file_name, r_ADDR, pkt_len, start_time)
                    break
            duplicated = 0
            continue

        sending_pkt(s_pkt_num, log_file_name, r_ADDR, pkt_len, start_time)
        s_pkt_num += 1

        if before_ack == r_ACK_num:
            duplicated += 1
        
        before_ack = r_ACK_num
        
def sending_pkt(s_pkt_num, log_file_name, r_ADDR, pkt_len, start_time):
    if final == True:
        s_pkt_num = 99999999
        s_sock.sendto(str(s_pkt_num).encode() + b'finished', r_ADDR)
        f_log = open(log_file_name, 'a')
        tm = time.time() - start_time
        f_log.write("\nFile transfer is finished.\n")
        f_log.write("Throughput: %.2f pkts / sec\n" % (pkt_len/tm))
        f_log.close()
        return
        
    pkt_data = str(padzero(s_pkt_num, 8)).encode() + pkt_divided[s_pkt_num]
    s_sock.sendto(pkt_data, r_ADDR)
            
    f_log = open(log_file_name, 'a')
    tm = time.time() - start_time
    f_log.write('%.3f pkt: %d | sent\n' % (tm, s_pkt_num))
    f_log.close()

def start_sender(IP, window_size, timeout):
    global pkt_state
    
    r_ADDR = (IP, 10080)
    
    file = input("file_name: ")
    file_split = file.split('.')
    file_name = file_split[0]
    log_file_name = file_name + "_sending_log.txt"
    f = open(log_file_name, 'w')
    f.close()        

    start_time = time.time()
    t = Thread(target = start_sender, args = (IP, window_size, timeout))
    #t.start()
    #s_sock.bind(('', 10080))
    s_sock.sendto(file.encode(), r_ADDR)
    
    with open(file, 'rb') as f:
        while True:
            buffer = f.read(pkt_size)
            if not buffer: break
            pkt_divided.append(buffer)
            pkt_state.append(0)

    pkt_len = len(pkt_divided)
    s_sock.sendto(str(pkt_len).encode(), r_ADDR)
    
    print("pkt_len : ", len(pkt_divided))

    s_pkt_num = 0
    threads = []
    t_send_pkt = Thread(target = sending_pkt, args = (s_pkt_num, log_file_name, r_ADDR, pkt_len, start_time))
    #threads.append(t_send_pkt)
    t_receive_ACK = Thread(target = receiving_ACK, args = (s_pkt_num, log_file_name, window_size, r_ADDR, pkt_len, start_time))
    threads.append(t_receive_ACK)
    #t_send_pkt.start()
    t_receive_ACK.start()
    for thread in threads:
        thread.join()
        
if __name__ == '__main__':
    

    IP = input("Receiver IP address: ")
    window_size = int(input("window size: "))
    timeout = float(input("timeout (sec): "))

    
    start_sender(IP, window_size, timeout)

s_sock.close()
