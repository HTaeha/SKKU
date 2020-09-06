from socket import *
from threading import Thread
import sys
import time

count_pkt = 0       ## 보낸 패킷의 개수
count_ack = 0       ## 받은 ACK의 개수
PPS = 0             ## packets per sec

# ------------------------------------------------------ #
## receiver에게 패킷을 보내는 쓰레드 함수이다.
def sending_pkt(raddr, ):
    global count_pkt, PPS
    current_time = time.time()
    i=1
    msg = '#'*1000
    while True:
        SPP = 1/PPS
        ## SSP는 하나의 패킷 전송시 걸리는 시간이다.
        ## 즉, SSP 시간만큼 지날때마다 패킷을 하나씩 전송한다.
        if time.time() - current_time > SPP*i:
            ## 패킷 대신 1000 byte 길이의 msg를 보낸다.
            sock.sendto(msg.encode(), raddr)
            ## 패킷을 하나 전송할때마다, 보낸 패킷 수를 증가시킨다.
            count_pkt += 1
            i += 1
# ------------------------------------------------------ #

# ------------------------------------------------------ #
## receiver에게 ACK를 받는 쓰레드 함수이다.
def receiving_ack():
    global count_ack

    while True:
        data, addr = sock.recvfrom(1000)
        ## ACK를 하나 받을때마다, 받은 ACK 수를 증가시킨다.
        count_ack += 1


# ------------------------------------------------------ #

# ------------------------------------------------------ #
## sender의 화면에 출력하는 쓰레드 함수이다.
def print_info():
    global count_pkt, count_ack, PPS
    i = 1
    current_time = time.time()
    while True:
        ## 2초마다 출력한다.
        now = time.time() - current_time
        if now > 2*i:
            ## sending rate는 1초당 보낸 패킷 수 이므로, 2초동안 보낸 패킷 수(count_pkt)를 2로 나눈 값이다.
            print('%.3f sending rate: %.1f'%(now, count_pkt/2))
            ## goodput은 1초당 받은 ACK 수 이므로, 2초동안 받은 ACK 수(count_ack)를 2로 나눈 값이다.
            print('%.3f goodput: %.1f'%(now, count_ack/2))
            ## gooput ratio는 받은 ACK 수를 보낸 패킷 수로 나눈 값이므로, goodput을 sending rate로 나눈 값이다.
            print('%.3f goodput ratio: %.2f'%(now, round(count_ack/count_pkt, 2)))
            print('')
            ## 출력을 해준 뒤, 보낸 패킷 수와 받은 ACK 수를 초기화시킨다.
            count_pkt = 0
            count_ack = 0
            i += 1

# ------------------------------------------------------ #

# ------------------------------------------------------ #
if __name__ == '__main__':

    sock = socket(AF_INET, SOCK_DGRAM)
    sock.bind(('127.0.0.1', 0))

    receiver_IP = '127.0.0.1'
    ## receiver의 주소
    raddr = (receiver_IP, 10080)
    PPS = int(input('sending rate : '))

    thread_pkt = Thread(target = sending_pkt, args = (raddr, ))
    thread_ack = Thread(target = receiving_ack)
    thread_print = Thread(target = print_info)

    thread_pkt.start()
    thread_ack.start()
    thread_print.start()

    thread_pkt.join()
    thread_ack.join()
    thread_print.join()


# ------------------------------------------------------ #
