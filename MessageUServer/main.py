from ClientHandler import ClientHandler
import ServerDatabase
from NetworkUtils import validate_port
from Common import *
import selectors
import socket


class MessageUServer(object):
    def __init__(self):
        ServerDatabase.initialize_database()

        self._sock = self.initialize_server_socket((SERVER_IP, self.get_server_port()))

        self._sel = selectors.DefaultSelector()
        self._sel.register(self._sock, selectors.EVENT_READ, self.accept)

    @staticmethod
    def get_server_port():
        with open('port.info', 'r') as f:
            data = f.read()

        if not validate_port(data):
            raise ValueError('Invalid port')

        return int(data)

    @staticmethod
    def initialize_server_socket(host: tuple[str, int]):
        sock = socket.socket()
        sock.bind(host)
        sock.listen(100)
        sock.setblocking(False)

        return sock

    @staticmethod
    def initialize_database():
        if not ServerDatabase.is_table_exists(ServerDatabase.CLIENTS_TABLE_NAME):
            ServerDatabase.create_clients_table()

        if not ServerDatabase.is_table_exists(ServerDatabase.MESSAGES_TABLE_NAME):
            ServerDatabase.create_messages_table()

    def accept(self, sock: socket.socket):
        client_sock, _ = sock.accept()
        client_sock.setblocking(False)
        self._sel.register(client_sock, selectors.EVENT_READ, self.handle_client)

    def handle_client(self, sock: socket.socket):
        try:
            ClientHandler(sock).handle()
        finally:
            self._sel.unregister(sock)
            sock.close()

    def serve(self):
        while True:
            events = self._sel.select()
            for key, _ in events:
                callback = key.data
                callback(key.fileobj)


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
