import struct

from Common import VERSION
from Client import Client
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
    client = Client.create_user(name, pub_key)
    ServerDatabase.register_client(client)

    # Send response
    _send_response_header(sock, REGISTER_CODE, len(client.identifier))
    sock.sendall(client.identifier)


def send_clients_list(sock, client_id, payload_size):
    SEND_LIST_CODE = 2001
    CLIENT_ENTRY_PATTERN = "<16s255s"

    if payload_size != 0:
        raise Exception("Got invalid payload")

    # -1 because we don't send the current client
    clients_count = ServerDatabase.clients_count() - 1
    _send_response_header(sock, SEND_LIST_CODE, clients_count * struct.calcsize(CLIENT_ENTRY_PATTERN))

    # Send clients list
    for client in ServerDatabase.clients_list():
        if client.identifier == client_id:
            continue

        sock.sendall(client.identifier)
        sock.sendall(client.name.ljust(Client.MAX_USER_NAME, '\0').encode())
