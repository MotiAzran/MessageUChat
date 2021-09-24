import struct

from Common import VERSION
from User import User
import ServerDatabase


def _send_response_header(sock, code, payload_size):
    RESPONSE_HEADER_PATTERN = "<BHL"
    sock.sendall(struct.pack(RESPONSE_HEADER_PATTERN, VERSION, code, payload_size))


def send_general_error(sock):
    GENERAL_ERROR_CODE = 9000
    _send_response_header(sock, GENERAL_ERROR_CODE, 0)


def register_handler(sock, client_id, payload_size):
    REGISTER_CODE = 2000
    PAYLOAD_PATTERN = "<255s160s"

    if payload_size != struct.calcsize(PAYLOAD_PATTERN):
        raise Exception("Got invalid payload")

    # Read payload
    name, pub_key = struct.unpack(PAYLOAD_PATTERN, sock.recv(payload_size))
    name = name.decode().strip('\0')

    # Write new user to database
    user = User.create_user(name, pub_key)
    ServerDatabase.register_user(user)

    # Send response
    _send_response_header(sock, REGISTER_CODE, len(user.identifier))
    sock.sendall(user.identifier)
