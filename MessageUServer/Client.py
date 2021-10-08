import ServerDatabase
import datetime
import uuid


class Client(object):
    MAX_USER_NAME = 255

    def __init__(self, id_: bytes, name: str, public_key: bytes, last_seen: datetime.datetime):
        self._id = id_
        self._name = name
        self._public_key = public_key
        self._last_seen = last_seen

    @classmethod
    def create_client(cls, name: str, public_key: bytes):
        """
        Create new client instance
        :param name: Client name
        :param public_key: Client public key
        :return: Client
        """
        if len(name) > cls.MAX_USER_NAME:
            raise ValueError("User name too long")

        if ServerDatabase.is_client_name_exists(name):
            raise ValueError("User name exists")

        return cls(uuid.uuid4().bytes_le, name, public_key, datetime.datetime.now())

    @property
    def name(self):
        return self._name

    @property
    def identifier(self):
        return self._id

    @property
    def public_key(self):
        return self._public_key

    @property
    def last_seen(self):
        return self._last_seen
