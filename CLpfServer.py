import socket

HOST = '127.0.0.1'  # 监听的IP地址
PORT = 12346        # 监听的端口号

# 创建socket对象，并绑定ip和port，开始监听传入的连接请求
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()

    # 无限循环，等待新客户端连接
    while True:
        conn, addr = s.accept()   # 接收新连接
        print('Connected by', addr)

        # 不断地循环接收来自客户端的消息，直到客户端发送exit指令或中断连接
        while True:
            data = conn.recv(1024)  # 接收数据
            if not data:
                break
            print(f"Receive from {addr}: {data.decode()}")

            # 将收到的数据加上前缀“Echo: ”后原样返回客户端
            reply = f'Echo: {data.decode()}'
            conn.sendall(reply.encode())

        # 关闭连接
        print('Connection closed by', addr)
        conn.close()
