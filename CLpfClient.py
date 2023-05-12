import socket

HOST = '127.0.0.1'  # 服务端IP地址
PORT = 8888        # 服务端监听端口号

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))  # 连接服务端
    print("PORT=",PORT)
    while True:
        message = input('请输入要发送的消息：')
        s.sendall(message.encode())  # 发送数据

        data = s.recv(1024)         # 接收服务端返回的数据
        print('Received:', data.decode())
