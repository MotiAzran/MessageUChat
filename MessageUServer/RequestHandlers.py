import struct

from Message import Message, MessageType
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
    with open("pub.bin", "wb") as f:
        f.write(pub_key)

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


def send_public_key(sock, client_id, payload_size):
    SEND_PUBLIC_KEY_CODE = 2002
    REQUEST_PAYLOAD_PATTERN = "<16s"
    RESPONSE_PAYLOAD_PATTERN = "<16s160s"

    if payload_size != struct.calcsize(REQUEST_PAYLOAD_PATTERN):
        raise Exception("Got invalid payload")

    requested_client_id = struct.unpack(REQUEST_PAYLOAD_PATTERN, sock.recv(payload_size))[0]
    public_key = ServerDatabase.get_client_public_key(requested_client_id)
    with open("spub.bin", "wb") as f:
        f.write(public_key)

    _send_response_header(sock, SEND_PUBLIC_KEY_CODE, struct.calcsize(RESPONSE_PAYLOAD_PATTERN))
    sock.sendall(struct.pack(RESPONSE_PAYLOAD_PATTERN, requested_client_id, public_key))


def send_waiting_messages(sock, client_id, payload_size):
    SEND_WAITING_MESSAGES_CODE = 2004
    RESPONSE_PAYLOAD_PATTERN = "<16sLBL"

    if 0 != payload_size:
        raise Exception("Got invalid payload")

    # Calculate payload size
    response_payload_size = 0
    for message in ServerDatabase.get_client_waiting_messages(client_id):
        response_payload_size += struct.calcsize(RESPONSE_PAYLOAD_PATTERN)
        response_payload_size += len(message.content)

    # Send response
    _send_response_header(sock, SEND_WAITING_MESSAGES_CODE, response_payload_size)
    for message in ServerDatabase.get_client_waiting_messages(client_id):
        sock.sendall(struct.pack(RESPONSE_PAYLOAD_PATTERN, message.from_client_id, message.identifier,
                                 message.message_type, len(message.content)))
        sock.sendall(message.content)

    ServerDatabase.delete_client_waiting_messages(client_id)


def send_message(sock, client_id, payload_size):
    SEND_MESSAGE_CODE = 2003
    REQUEST_PAYLOAD_PATTERN = "<16sBL"
    RESPONSE_PAYLOAD_PATTERN = "<16sL"

    if struct.calcsize(REQUEST_PAYLOAD_PATTERN) > payload_size:
        raise Exception("Got invalid payload")

    to_client_id, message_type, content_size = struct.unpack(REQUEST_PAYLOAD_PATTERN,
                                                             sock.recv(struct.calcsize(REQUEST_PAYLOAD_PATTERN)))
    message_content = sock.recv(content_size)

    message = Message.create_new_message(to_client_id, client_id, MessageType(message_type), message_content)
    ServerDatabase.add_message(message)

    _send_response_header(sock, SEND_MESSAGE_CODE, struct.calcsize(RESPONSE_PAYLOAD_PATTERN))
    sock.sendall(struct.pack(RESPONSE_PAYLOAD_PATTERN, to_client_id, message.identifier))
