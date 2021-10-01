from contextlib import contextmanager
import threading


class DatabaseLock(object):
    def __init__(self):
        self._can_read = threading.Condition(threading.Lock())
        self._readers_count = 0

    @contextmanager
    def lock_read(self):
        try:
            with self._can_read:
                self._readers_count += 1

            yield
        finally:
            with self._can_read:
                self._readers_count -= 1
                if self._readers_count == 0:
                    self._can_read.notifyAll()

    @contextmanager
    def lock_write(self):
        try:
            self._can_read.acquire()
            # Wait for all readers to finish
            while self._readers_count > 0:
                self._can_read.wait()

            yield
        finally:
            self._can_read.release()
