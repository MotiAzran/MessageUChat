from Common import DATABASE_PATH
from DatabaseLock import DatabaseLock
import Message
import Client
import sqlite3

CLIENTS_TABLE_NAME = "Clients"
MESSAGES_TABLE_NAME = "Messages"

database_lock = DatabaseLock()


def database_read(func):
    """
    Decorator for function that only
    read once from the database
    Note: If you use this decorator call only to
    _exectue_cammand any other database function
    will cause deadlock
    """
    def inner(*args, **kwargs):
        with database_lock.lock_read():
            return func(*args, **kwargs)

    return inner


def database_write(func):
    """
    Decorator for function that only
    write once to the database
    Note: If you use this decorator call only to
    _exectue_cammand any other database function
    will cause deadlock
    """
    def inner(*args, **kwargs):
        with database_lock.lock_write():
            return func(*args, **kwargs)

    return inner


def _execute_command(*args, **kwargs):
    """
    Execute command and return it output
    """
    with sqlite3.connect(DATABASE_PATH) as db:
        db.text_factory = bytes
        db.row_factory = sqlite3.Row
        c = db.cursor()
        return c.execute(*args, **kwargs)


@database_write
def create_clients_table():
    """
    Create clients table
    """
    _execute_command(f'''CREATE TABLE {CLIENTS_TABLE_NAME}(ID TEXT PRIMARY KEY,
            Name TEXT UNIQUE,
            PublicKey TEXT NOT NULL,
            LastSeen TIMESTAMP);''')


@database_write
def create_messages_table():
    """
    Create messages tables
    """
    _execute_command(f'''CREATE TABLE {MESSAGES_TABLE_NAME}(ID INT PRIMARY KEY,
            ToClient TEXT,
            FromClient TEXT,
            Type int,
            Content BLOB NOT NULL,
            FOREIGN KEY (ToClient, FromClient) REFERENCES Clients (ID, ID));''')


@database_read
def is_table_exists(table_name):
    """
    Checks if given table exists
    :param table_name: Table name to check
    :return: True if the table exists, else false
    """
    cursor = _execute_command("""SELECT name FROM sqlite_master WHERE type='table' AND name=?""", (table_name,))
    return cursor.fetchone() is not None


def initialize_database():
    """
    Create database tables
    """
    if not is_table_exists(CLIENTS_TABLE_NAME):
        create_clients_table()

    if not is_table_exists(MESSAGES_TABLE_NAME):
        create_messages_table()


@database_read
def is_client_name_exists(name):
    """
    Checks if client name already exists
    in the table
    :param name: name to check
    :return: True if the name exists, else false
    """
    cursor = _execute_command(f'SELECT Name FROM {CLIENTS_TABLE_NAME} WHERE Name=?', [name])
    return cursor.fetchone() is not None


@database_read
def is_client_id_exists(id_):
    """
    Checks if client ID already exist
    in the table
    :param id_: ID to check
    :return: True if the ID exists, else false
    """
    cursor = _execute_command(f'SELECT ID FROM {CLIENTS_TABLE_NAME} WHERE ID=?', [id_])
    return cursor.fetchone() is not None


@database_write
def register_client(client):
    """
    Add new client to clients table
    :param client: Client to add
    """
    _execute_command(f'''INSERT INTO {CLIENTS_TABLE_NAME} VALUES (?, ?, ?, ?)''',
                     (client.identifier, client.name, client.public_key, client.last_seen))


@database_read
def clients_count():
    """
    Returns the number of registered clients
    """
    cursor = _execute_command(f'SELECT COUNT(ID) AS CNT FROM {CLIENTS_TABLE_NAME}')
    return cursor.fetchone()["CNT"]


@database_read
def clients_list():
    """
    Returns clients list
    """
    cursor = _execute_command(f'SELECT * FROM {CLIENTS_TABLE_NAME}')
    row = cursor.fetchone()
    while row is not None:
        yield Client.Client(row["ID"], row["Name"], row["PublicKey"], row["LastSeen"])
        row = cursor.fetchone()


@database_read
def get_client_public_key(client_id):
    """
    Get public key of given client
    :param client_id: ID of the requested client
    :return: Client public key
    """
    cursor = _execute_command(f'SELECT ID, PublicKey FROM {CLIENTS_TABLE_NAME} WHERE ID=?', [client_id])
    return cursor.fetchone()["PublicKey"]


@database_write
def add_message(message):
    """
    Add message to messages table
    :param message: Message to add
    """
    _execute_command(f'''INSERT INTO {MESSAGES_TABLE_NAME} VALUES (?, ?, ?, ?, ?)''',
                     (message.identifier, message.to_client_id, message.from_client_id,
                      message.message_type, message.content))


@database_read
def get_client_waiting_messages(client_id):
    """
    Get client waiting messages
    :param client_id: ID of the client
    :return: Client messages
    """
    cursor = _execute_command(f'''SELECT * FROM {MESSAGES_TABLE_NAME} WHERE ToClient=?''', [client_id])
    row = cursor.fetchone()
    while row is not None:
        yield Message.Message(row["ID"], row["ToClient"], row["FromClient"],
                              Message.MessageType(row["Type"]), row["Content"])
        row = cursor.fetchone()


@database_write
def delete_client_waiting_messages(client_id):
    """
    Delete client messages
    :param client_id: ID of the client we want to delete
    his messages
    """
    _execute_command(f'''DELETE FROM {MESSAGES_TABLE_NAME} WHERE ToClient=?''', [client_id])


@database_read
def get_max_message_id():
    """
    Returns max message ID from the database
    """
    try:
        cursor = _execute_command(f'''SELECT MAX(ID) AS MaxID FROM {MESSAGES_TABLE_NAME}''')
        row = cursor.fetchone()
        return -1 if row["MaxID"] is None else row["MaxID"]
    except:
        return -1
