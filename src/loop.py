from asyncio import AbstractEventLoop

from _uring_io import Ring

class UringIOEventLoop(AbstractEventLoop):
    def __init__(entries, *, sq_entries):
        super().__init__()

        self._ring = Ring()
        self._running = False
    
    def run_forever():
        
