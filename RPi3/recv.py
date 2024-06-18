import serial

class LoRa():
    def __init__(self):
        self.s = serial.Serial('/dev/ttyUSB0', 115200) # シリアルポートを115200kbps, 8bit, Non parity, 1 stop-bitでオープン

    def open(self):
        self.s.open()

    def close(self):
        self.s.close()

    def readline(self, timeout = None):
        if timeout != None:
            self.s.close()
            self.s.timeout = timeout
            self.s.open()
        line = self.s.readline()
        if timeout != None:
            self.s.close()
            self.s.timeout = None
            self.s.open()
        return line.decode('utf-8')

    def write(self, msg):
        self.s.write(msg.encode('utf-8'))

lr = LoRa()
def printable(l):
    x = struct.unpack(str(len(l)) + 'b', l)
    y = ''
    for i in range(len(x)):
        if x[i] >= 0:
            y = y + chr(x[i])
    return y

def sendcmd(cmd):
    lr.write(cmd+'\r\n')
    recvcmd()
    return

def recvcmd():
    print(lr.readline().rstrip('\n'))
    return

def recvdata():
    data = lr.readline().rstrip('\n')
    return data


recvcmd()
recvcmd()
sendcmd('2')
sendcmd('x')
sendcmd('c 12')
sendcmd('d 3')
sendcmd('e 2345')
sendcmd('f 0')
sendcmd('g ffff')
sendcmd('z')
print('set up complite')

# while True:
#     data = recvdata()
#     print(f"recv: {data}")

rx = lr.s.readline()
print("rx: ", rx)
rx = lr.s.readline()
print("rx: ", rx)