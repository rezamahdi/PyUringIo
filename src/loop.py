from asyncio import AbstractEventLoop

from _uring_io import Ring


class UringIOEventLoop(AbstractEventLoop):
    """asyncio equivalent loop based on uring_io"""

    def __init__(
        entries,
        *,
        sq_entries,
        cq_entries,
        flags,
        sq_thread_cpu,
        sq_thread_idle,
        features,
        wq_fd
    ):
        super().__init__()
        self._ring = Ring(
            entries=entries,
            sq_entries=sq_entries,
            cq_entries=cq_entries,
            flags=flags,
            sq_thread_cpu=sq_thread_cpu,
            sq_thread_idle=sq_thread_idle,
            features=features,
            wq_fd=wq_fd,
        )
        self._running = False

    def run_forever():
        ...

    async def getaddrinfo():
        ...
