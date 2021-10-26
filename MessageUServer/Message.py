from enum import Enum, unique
from ServerDatabase import get_max_message_id, is_client_id_exists
import threading


@unique
class MessageType(Enum):
    SYMMETRIC_KEY_REQUEST = 1
    SYMMETRIC_KEY = 2
    NORMAL_MESSAGE = 3


class Message(object):
    counter_lock = threading.Lock()
    message_counter = get_max_message_id() + 1

    def __init__(self, id_: int, to_id: bytes, from_id: bytes, message_type: MessageType, content: bytes):
        if not is_client_id_exists(to_id) or not is_client_id_exists(from_id):
            raise ValueError("User not exists")

        if not isinstance(message_type, MessageType):
            raise ValueError("Invalid message type")

        self._id = id_
        self._to_id = to_id
        self._from_id = from_id
        self._type = message_type
        self._content = content

    @classmethod
    def create_new_message(cls, to_id: bytes, from_id: bytes, message_type: MessageType, content: bytes):
        """
        Create new message instance
        :param to_id: ID of the message receiver
        :param from_id: ID of the message sender
        :param message_type: Message type
        :param content: Message content
        :return: Message
        """
        with cls.counter_lock:
            id_ = cls.message_counter
            cls.message_counter += 1

        return cls(id_, to_id, from_id, message_type, content)

    @property
    def identifier(self):
        return self._id

    @property
    def to_client_id(self):
        return self._to_id

    @property
    def from_client_id(self):
        return self._from_id

    @property
    def message_type(self):
        return self._type.value

    @property
    def content(self):
        return self._content
