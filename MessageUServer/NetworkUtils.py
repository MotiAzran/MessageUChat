def validate_port(port: str) -> bool:
    """
    Check port is valid
    :param port: port to check
    :return: True if port is valid, else False
    """
    MAX_PORT = 0xffff

    if not port.isdigit() or not 0 <= int(port) <= MAX_PORT:
        return False

    return True
