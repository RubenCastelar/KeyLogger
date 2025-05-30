import socket


s = socket.socket()
s.bind(("0.0.0.0", 9001))
s.listen(1)
print("Escuchando en puerto 9001...")


while True:
    conn, addr = s.accept()
    data = conn.recv(1024).decode(errors="ignore")
    print(f"{addr[0]}: {data}")
    conn.close()
