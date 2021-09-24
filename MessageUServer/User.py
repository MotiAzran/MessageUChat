import ServerDatabase
import datetime
import uuid


class User(object):
    MAX_USER_NAME = 254

    def __init__(self, id_, name, public_key, last_seen):
        self._id = id_
        self._name = name
        self._public_key = public_key
        self._last_seen = last_seen

    @property
    def name(self):
        return self._name

    @property
    def identifier(self):
        return self._id.bytes

    @property
    def public_key(self):
        return self._public_key

    @property
    def last_seen(self):
        return self._last_seen

    @classmethod
    def create_user(cls, name, public_key):
        if len(name) > cls.MAX_USER_NAME:
            raise ValueError("User name too long")

        if ServerDatabase.is_user_name_exists(name):
            raise ValueError("User name exists")

        return cls(uuid.uuid4(), name, public_key, datetime.datetime.now())
