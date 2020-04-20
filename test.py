import serial
import struct
from time import sleep

def C(val):
    return struct.pack('!H', val)

ser = serial.Serial(port='/dev/cu.SLAB_USBtoUART', baudrate=9600)

ser.write([0xA0])
print "sent: 0xA0"
cmd = bytearray([0x01, 0x05, 0b00000010, 0x00, 0x00])
crc = 0

for b in cmd:
  ser.write([b])
  print "sent: " + hex(b)
  crc = crc + b

ser.write([crc])
print "sent: " + hex(crc) + " <- CRC"

while True:
  data = ser.read().encode('hex')
  print "read: " + data

ser.close()