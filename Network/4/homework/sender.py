from reliableUDP import UDPsocket
from reliableUDP import padhex
from reliableUDP import get_time_string
import time
import threading
import sched

SERVER_PORT = 10080


class ClientBuffer(list):
    def __init__(self, window_size, file_name, size=1400):
        super().__init__()
        self.window_size = window_size
        self.offset = 0
        with open(file_name, 'rb') as f:
            data = f.read(size)
            self.append(data)
            while len(data) == size:
                data = f.read(size)
                self.append(data)


class Client(UDPsocket):
    ip = '127.0.0.1'
    win_size = 16
    t_out = 0.2

    def __init__(self):
        super().__init__()
        self.serverHostAddr = (self.ip, SERVER_PORT)
        self.serverAddr = None
        self.window_size = self.win_size
        self.timeout = self.t_out
        self.file_name = input("Input file Name")
        self.buffer = ClientBuffer(self.window_size, self.file_name)
        self.file_size = len(self.buffer)
        self.seq = 0
        self.timestamp = 0
        self.dupli_ack_cnt = 0
        self.last_sent = 0
        self.scheduler = sched.scheduler(time.time, time.sleep)
        self.done = False
        self.ackthr = threading.Thread(target=self.get_ack)
        self.run_send = False
        self.mutex_seq = threading.Lock()
        self.mutex_send = threading.Lock()
        self.mutex_time = threading.Lock()
        self.mutex_log = threading.Lock()
        self.client_log_file_name = self.file_name + "_client_log.txt"
        self.client_log = open(self.client_log_file_name, "w")
        self.log_time = 0

    @classmethod
    def setting(cls):
        cls.ip = input("input server ip")
        cls.win_size = int(input("input window size"))
        cls.t_out = float(input("input time out"))

    def __del__(self):
        try:
            self.client_log.close()
        except AttributeError:
            pass
        super().__del__()

    def send_packet(self):
        with self.mutex_seq:
            seq = self.seq
            # print("self.seq", self.seq)
        # send
        for _ in range(self.window_size):
            with self.mutex_time:
                self.timestamp = time.time()
            if seq < self.file_size:
                header = self.header(seq)
                data = self.buffer[seq]
                packet = header + data
                self.socket.sendto(packet, self.serverAddr)
                # print("server", self.serverAddr)
                self.write_log(seq)
                seq += 1
                # print("seq", seq)
            else:
                break
        with self.mutex_seq:
            self.last_sent = seq
        # print("last sent", seq)
        with self.mutex_send:
            self.run_send = False

    def write_log(self, packet_number, ack=False, time_out=False):
        time_now = time.time() - self.log_time
        string = get_time_string(time_now)

        if ack and time_out:
            raise ValueError
        if ack:
            string += "got ack"
        if time_out:
            self.client_log.write("###########################################\n")
            string += "time out retransmission\n"
        if not ack and not time_out:
            string += "sent"
        string += "\n"
        string = "Packet number " + str(packet_number) + "\t" + string
        with self.mutex_log:
            self.client_log.write(string )
        return

    def write_goodput(self):
        time_now = time.time() - self.log_time
        good_put = self.file_size/time_now
        log_string = str(good_put) + "packets/sec\n"
        with self.mutex_log:
            self.client_log.write("###########################################\n")
            self.client_log.write("Good put:\t")
            self.client_log.write(log_string)

    def timer_execution(self):
        while True :
            time_now = time.time()
            with self.mutex_time:
                time_stamp = self.timestamp
            if time_now - time_stamp> self.timeout and not self.run_send:
                with self.mutex_send:
                    self.run_send = True

                self.write_log(self.seq, time_out=True)
                self.send_packet()
            if self.done:
                return

    def get_ack(self):
        while True:
            with self.mutex_seq:
                seq = self.last_sent

            for _ in range(seq - self.seq):
                msg, server = self.socket.recvfrom(self.packet_size)
                # print("server", server)
                try:
                    ack = int(msg.decode('ascii'), 16)
                except ValueError:
                    print (msg.decode())
                self.write_log(ack, ack=True)
                if ack >= self.seq:
                    with self.mutex_seq:
                        self.seq = ack + 1
                        self.last_sent = ack + 1
                else:
                    self.dupli_ack_cnt += 1
                    if self.dupli_ack_cnt == 3:
                        with self.mutex_send:
                            with self.mutex_log:
                                self.client_log.write("###########################################\n")
                                self.client_log.write("Three duplicate ac:k\t Packet number ")
                                self.client_log.write(str(ack))
                            self.run_send = True
                        self.send_packet()
                with self.mutex_seq:
                    if self.seq == self.file_size:
                        # print('done')
                        self.done = True
                        return

            # print("ack execution")
            with self.mutex_send:
                self.run_send = True
            self.send_packet()

    def header(self, seqnum):
        ret = padhex(seqnum, 8)
        return ret.encode('ascii')

    def handshaking(self, window_size, file_name):
        # window size < 0xffff
        window_size = padhex(window_size, 4)
        filesize = padhex(len(self.buffer), 8)
        # print("pad hex", filesize)
        handshaking_info = window_size + filesize + file_name
        # print(handshaking_info)
        self.socket.sendto(handshaking_info.encode(), self.serverHostAddr)
        server_msg, self.serverAddr = self.socket.recvfrom(self.buf_0)
        # print("server msg", server_msg.decode())
        # print("hand shaking infomation", handshaking_info)
        self.log_time = time.time()
        try:
            if server_msg.decode() != handshaking_info:
                self.NAK(self.serverAddr)
                return True
            else:
                # print("Ack Sent to", self.serverAddr)
                self.socket.sendto("ACK".encode(), self.serverAddr)
                return False
        except:
            self.NAK(self.serverAddr)

    def run(self):
        while self.handshaking(self.window_size, self.file_name):
            pass
        # print (len(self.buffer))
        self.send_packet()
        timer = threading.Thread(target=self.timer_execution)
        self.ackthr.start()
        timer.start()
        while True:
            if self.done:
                self.write_goodput()
                return

    def NAK(self, serverAddress):
        """
        :param clientAddress:
        only for hand shaking
        """
        self.socket.sendto("NAK".encode(), serverAddress)
        return

def run_sender(sender):
    print ("gogogo")
    sender.run()
    time.sleep(0.5)
    sender.client_log.close()

def main():
    Client.setting()
    while True:
        sender = Client()
        thr = threading.Thread(target=run_sender, kwargs={"sender":sender})
        thr.start()

if __name__ == "__main__":
    main()
