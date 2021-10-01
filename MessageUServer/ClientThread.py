from RequestHandlers import *
from threading import Thread
import struct


class ClientThread(Thread):
    """
    Thread to handle client request session
    """
    def __init__(self, sock):
        super().__init__()
        self._sock = sock

        self._handlers = {1000: register_handler, 1001: send_clients_list,
                          1002: send_public_key, 1003: send_message, 1004: send_waiting_messages}

    def _get_request_header(self):
        REQUEST_HEADER_PATTERN = "<16sBHL"
        header = self._sock.recv(struct.calcsize(REQUEST_HEADER_PATTERN))

        return struct.unpack(REQUEST_HEADER_PATTERN, header)

    def run(self):
        client_id, version, code, payload_size = self._get_request_header()

        if code in self._handlers:
            try:
                self._handlers[code](self._sock, client_id, payload_size)
            except:
                send_general_error(self._sock)
        else:
            send_general_error(self._sock)
            raise Exception(f"Got invalid request from client: {code}")

