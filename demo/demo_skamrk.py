import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = (socket.gethostname, 5555)
sock.bind(server_address)
sock.listen(1)

while True:
        print >>sys.stderr, 'waiting for a connection'
        connection, client_addr = sock.accept()
        try:
            print >>sys.stderr, 'connection from', client_addr
            while True:
                data = connection.recv(16)
                print >>sys.stderr, 'received "%s"' % data
                if data:
                    print >>sys.stderr, 'sending data back to the client'
                    connection.sendall(data)
                    print >>sys.stderr, connection.getsockopt(
                            socket.SOL_SOCKET, 36
                    )
                else:
                    print >>sys.stderr, 'no more data from', client_addr
                    break
        finally:
            connection.close()
