import socket
import tkinter as tk

HOST = "172.16.208.128"  # QNX IP
PORT = 8080

def send_message():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            s.sendall(b"Hello from Python GUI")
            data = s.recv(1024)
            response_label.config(text="Response: " + data.decode())
    except Exception as e:
        response_label.config(text="Error: " + str(e))

root = tk.Tk()
root.title("Python → QNX GUI")

send_button = tk.Button(root, text="Send Hello", command=send_message)
send_button.pack(pady=10)

response_label = tk.Label(root, text="Response will appear here")
response_label.pack(pady=10)

root.mainloop()

