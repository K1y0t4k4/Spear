import sys
import time
from threading import Thread

class Loading(Thread):
    def __init__(self):
        super().__init__()
        self.stop     = False
        self.daemon   = True
        self.progress = ['|', '\\', '-', '/']

    def run(self):
        progress_index = 0
        text_index     = 0
        sys.stdout.write("\033[F")
        sys.stdout.write("\033[K")
        while not self.stop:
            text             = list("waiting for a connection...")
            text[text_index] = text[text_index].capitalize()
            time.sleep(.1)
            print(f"{self.progress[progress_index]} " + ''.join(text))
            sys.stdout.write("\033[F")
            sys.stdout.write("\033[K")
            progress_index += 1
            text_index     += 1
            if progress_index == len(self.progress): progress_index = 0
            if text_index     == len(text) - 3: text_index     = 0
            