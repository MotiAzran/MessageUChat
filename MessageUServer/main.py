import ClientThread
import ServerDatabase
from NetworkUtils import validate_port
from Common import *
import socket


class MessageUServer(object):
    def __init__(self):
        self._host = (SERVER_IP, self.get_server_port())
        self._sock = self.initialize_server_socket(self._host)

        ServerDatabase.initialize_database()

    @staticmethod
    def get_server_port():
        with open('port.info', 'r') as f:
            data = f.read()

        if not validate_port(data):
            raise ValueError('Invalid port')

        return int(data)

    @staticmethod
    def initialize_server_socket(host):
        sock = socket.socket()
        sock.bind(host)
        sock.listen(5)

        return sock

    def serve(self):
        while True:
            sock, _ = self._sock.accept()
            ClientThread.ClientThread(sock).start()


def main():
    server = MessageUServer()
    server.serve()


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(f'Error: {e}')
    except:
        print('Unknown error')
