from enum import Enum, unique
import ServerDatabase
import threading


@unique
class MessageType(Enum):
    SYMMETRIC_KEY_REQUEST = 1
    SYMMETRIC_KEY = 2
    NORMAL_MESSAGE = 3


class Message(object):
    counter_lock = threading.Lock()
    message_counter = 0

    def __init__(self, id_, to_id, from_id, message_type, content):
        if not ServerDatabase.is_user_id_exists(to_id) or not ServerDatabase.is_user_id_exists(from_id):
            raise ValueError("User not exists")

        if not isinstance(message_type, MessageType):
            raise ValueError("Invalid message type")

        if 0 == len(content):
            raise ValueError("Can't send empty message")

        self._id = id_
        self._to_id = to_id
        self._from_id = from_id
        self._type = message_type
        self._content = content

    @classmethod
    def create_new_message(cls, to_id, from_id, message_type, content):
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
