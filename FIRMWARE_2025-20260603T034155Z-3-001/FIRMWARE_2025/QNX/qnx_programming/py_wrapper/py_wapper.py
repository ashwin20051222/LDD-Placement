# python_client.py
import socket

HOST = "172.16.208.128"   # QNX IP address
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"Hello from Python")
    data = s.recv(1024)

print("Received from QNX:", data.decode())

