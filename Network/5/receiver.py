from socket import *
from threading import Thread
import sys
import time

count_pkt = 0                   ## 받은 패킷 수
count_ack = 0                   ## 보낸 ACK 수
avg_queue_occupancy = 0         ## 큐에 들어있는 평균 패킷 수
pkt_queue = []                  ## 받은 패킷을 넣어두는 리스트

# ------------------------------------------------------ #
## sender로부터 패킷을 받는 쓰레드 함수이다.
def network_emulator(BLR, qsize):
    global count_pkt
    i = 0
    while True:
        data, saddr = sock.recvfrom(1000)
        ## 패킷을 하나 받을때마다, 받은 패킷 수를 증가시킨다
        count_pkt += 1
        ## 큐가 차있지 않으면, 패킷을 보낸 sender의 주소를 큐에 넣는다
        if len(pkt_queue) < qsize:
            pkt_queue.append(saddr)


# ------------------------------------------------------ #

# ------------------------------------------------------ #
## sender에게 ACK를 보내는 쓰레드 함수이다.
def ack_generator(BLR, qsize):
    global count_ack, user_list, avg_queue_occupancy
    i = 1
    ## BLR은 bottleneck link rate이고, SPP는 bottleneck에 하나의 패킷이 지날때 걸리는 시간이다
    SPP = 1/BLR
    current_time = time.time()
    msg = '#'*1000
    while True:
        ## SSP 시간만큼 지날때마다 ACK를 하나씩 전송한다.
        if time.time() - current_time > SPP*i:
            ## 큐에 저장된 주소를 꺼내, 그 주소를 가지는 sender에게 ACK를 보낸다.
            if len(pkt_queue) > 0:
                data = pkt_queue.pop(0)
                ## ACK를 하나 보낼때마다, 보낸 ACK수를 증가시킨다.
                count_ack += 1
                sock.sendto(msg.encode(), data)
            i += 1


# ------------------------------------------------------ #

# ------------------------------------------------------ #
## receiver의 화면에 출력하는 쓰레드 함수이다
def print_info(BLR, ):
    global count_pkt, count_ack, avg_queue_occupancy
    i = 1
    sum = 0
    current_time = time.time()
    while True:
        now = time.time() - current_time
        ## 0.1초가 지날때마다, 큐에 들어있는 패킷수를 sum에 저장한다.
        if now > 0.1*i:
            sum += len(pkt_queue)
            if i % 20 == 0:
                ## 2초가 지날때마다, sum값을 20으로 나눠 평균 큐 길이(average queue occupancy)값을 구한다
                avg_queue_occupancy = sum/20
                ## incoming rate는 1초당 받은 패킷 수 이므로, 2초동안 받은 패킷 수(count_pkt)값을 2로 나눈 값이다.
                print('%.4f incoming rate: %.1f'%(now, count_pkt/2))
                ## forwarding rate는 1초당 NE에서 AG로 이동한 패킷 수를 나타낸다.
                ## AG는 NE로부터 패킷을 받으면 바로 ACK를 sender에게 보내므로,
                ## forwarding rate는 보낸 ACK 수(count_ack)값을 2로 나눈 값이다.
                print('%.4f forwarding rate: %.1f'%(now, count_ack/2))
                print('%.4f avg queue occupancy: %.2f'%(now, round(avg_queue_occupancy, 2)))
                print('')
                ## 출력을 해준 뒤, 받은 패킷 수와 보낸 ACM 수, 큐의 평균 길이 합을 초기화시킨다.
                count_pkt = 0
                count_ack = 0
                sum = 0

            i += 1

# ------------------------------------------------------ #

# ------------------------------------------------------ #
if __name__ == '__main__':

    sock = socket(AF_INET, SOCK_DGRAM)
    sock.bind(('127.0.0.1', 10080))

    bottleneck_link_rate = int(input('bottleneck link rate: '))
    queue_size = int(input('bottleneck queue size: '))

    thread_NE = Thread(target = network_emulator, args = (bottleneck_link_rate, queue_size))
    thread_AG = Thread(target = ack_generator, args = (bottleneck_link_rate, queue_size))
    thread_print = Thread(target = print_info, args = (bottleneck_link_rate, ))

    thread_NE.start()
    thread_AG.start()
    thread_print.start()

    thread_NE.join()
    thread_AG.join()
    thread_print.join()


# ------------------------------------------------------ #
