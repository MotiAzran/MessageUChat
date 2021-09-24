from Common import DATABASE_PATH
import Message
import sqlite3

CLIENTS_TABLE_NAME = "Clients"
MESSAGES_TABLE_NAME = "Messages"


def _execute_command(*args, **kwargs):
    """
    Execute command and return command output
    """
    with sqlite3.connect(DATABASE_PATH) as db:
        c = db.cursor()
        return c.execute(*args, **kwargs)


def _execute_commands(commands):
    """
    Execute commands without getting commands output
    """
    with sqlite3.connect(DATABASE_PATH) as db:
        c = db.cursor()
        for command in commands:
            c.execute(command)


def _is_table_exists(table_name):
    output = _execute_command("""SELECT name FROM sqlite_master WHERE type='table' AND name=?""", (table_name,))
    return output.fetchone() is not None


def initialize_database():
    commands = []
    if not _is_table_exists(CLIENTS_TABLE_NAME):
        commands.append(f'''CREATE TABLE {CLIENTS_TABLE_NAME}(ID INT PRIMARY KEY,
            Name TEXT UNIQUE,
            PublicKey TEXT NOT NULL,
            LastSeen TIMESTAMP);''')

    if not _is_table_exists(MESSAGES_TABLE_NAME):
        commands.append(f'''CREATE TABLE {MESSAGES_TABLE_NAME}(ID INT PRIMARY KEY,
            ToClient INT,
            FromClient INT,
            Type int,
            Content BLOB NOT NULL,
            FOREIGN KEY (ToClient, FromClient) REFERENCES Clients (ID, ID));''')

    if commands != []:
        _execute_commands(commands)


def is_user_name_exists(name):
    output = _execute_command('SELECT Name FROM Clients WHERE Name=?', [name])
    return output.fetchone() is not None


def is_user_id_exists(id_):
    output = _execute_command('SELECT ID FROM Clients WHERE ID=?', [id_])
    return output.fetchone() is not None


def register_user(user):
    _execute_command('''INSERT INTO Clients VALUES (?, ?, ?, ?)''',
                     (user.identifier, user.name, user.public_key, user.last_seen))


def users_list():
    output = _execute_command('''SELECT ID, Name FROM Clients''')
    user = output.fetchone()
    while user is not None:
        yield user
        user = output.fetchone()


def add_message(message):
    _execute_command('''INSERT INTO Messages VALUES (?, ?, ?, ?, ?)''',
                     (message.identifier, message.to_client_id, message.from_client_id,
                      message.message_type, message.content))


def get_user_waiting_messages(user):
    output = _execute_command('''SELECT * FROM Messages WHERE ToClient=?''', [user.identifier])
    message = output.fetchone()
    while message is not None:
        message_id, to_client, from_client, message_type, content = message
        yield Message.Message(message_id, to_client, from_client, Message.MessageType(message_type), content)
