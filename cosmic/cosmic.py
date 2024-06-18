import serial
import time
import sys

class LoRa():
    def __init__(self, port):
        self.s = serial.Serial(port, 115200)
        self.setup()

    def open(self):
        self.s.open()

    def close(self):
        self.s.close()

    def response(self):
        while self.s.inWaiting() == 0:
            time.sleep(0.1)
        return(self.s.inWaiting())

    def read(self):
        len = self.response()
        line = self.s.readline()
        return line.strip().decode('utf-8')

    def write(self, msg):
        while True:
            if self.s.out_waiting == 0:
                break
        msg += '\r\n'
        len = self.s.write(msg.encode('utf-8'))
        self.s.flush()
        return len

    def setup(self):
        cmds = ['2', 'x', 'c 12', 'd 3', 'e 2345', 'f 1', 'g ffff', 'z']
        print(self.read())
        print(self.read())
        for cmd in cmds:
            self.write(cmd)
            print(self.read())
        print('set up complite')

class Cosmic():
    def __init__(self, port):
        self.s = serial.Serial(port, 9600)

    def read(self):
        line = self.s.readline()
        line = line.strip().decode('utf-8')
        return line


lora = LoRa('/dev/ttyUSB0')
args = sys.argv
print(len(args))
if len(args) > 1 and 'send' in args[1]:
    print('sending mode')
    cosmic = Cosmic('/dev/ttyUSB1')
    while True:
        line = cosmic.read()
        print(line)
        if 'CANDI' in line:
            break

    while True:
        line = cosmic.read()
        print(line)
        lora.write(line)
        time.sleep(0.5)

else:
    print('receiving mode')
    while True:
        line = lora.read()
        print(line)