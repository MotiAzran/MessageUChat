from NetworkUtils import validate_port
from Common import *


class MessageUServer(object):
    def __init__(self):
        self._server_host = (SERVER_IP, self.get_server_port())

    @staticmethod
    def get_server_port():
        with open('port.info', 'r') as f:
            data = f.read()

        if not validate_port(data):
            raise ValueError('Invalid port')

        return int(data)


def main():
    server = MessageUServer()


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(f'Error: {e}')
    except:
        print('Unknown error')
