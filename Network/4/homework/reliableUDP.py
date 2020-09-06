from socket import *
import abc


def padhex(integer, fix_size):
    string = "{:x}".format(integer)
    padsize = fix_size - len(string)
    if padsize < 0:
        raise OverflowError
    for i in range(padsize):
        string = "0" + string
    return string


def get_time_string(time_float):
    hour = int(time_float/3600)
    min = int((time_float%3600)/60)
    sec = int(time_float%60)
    if hour< 10:
        hour = "0"+ str(hour)
    else:
        hour = str(hour)
    if min < 10:
        min = "0" + str(min)
    else:
        min = str(min)
    if sec < 10:
        sec = "0" + str(sec)
    else:
        sec = str(sec)
    time_string = hour + ":" + min + ":"+ sec
    return time_string

class UDPsocket(object):
    def __init__(self):
        """
        sock: socket body of UDP socket
        window size: window size determined in hand shaking process
        handshake: raise error if socket did not handshaking
        assume that this socket is not for duplex
        """
        self.port = 10080
        self.socket = socket(AF_INET, SOCK_DGRAM)
        self.window_size = 0
        self.handshake = False
        self.buf_0 = 1024 # 1 KiBytes, for handshaking
        self.packet_size = 1400  #1

    def __del__(self):
        self.socket.close()

    @abc.abstractmethod
    def handshaking(self):
        pass


