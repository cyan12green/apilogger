from google.protobuf import empty_pb2 as _empty_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class SingleLog(_message.Message):
    __slots__ = ["log_data"]
    LOG_DATA_FIELD_NUMBER: _ClassVar[int]
    log_data: bytes
    def __init__(self, log_data: _Optional[bytes] = ...) -> None: ...

class BatchLog(_message.Message):
    __slots__ = ["log_batch"]
    LOG_BATCH_FIELD_NUMBER: _ClassVar[int]
    log_batch: _containers.RepeatedScalarFieldContainer[bytes]
    def __init__(self, log_batch: _Optional[_Iterable[bytes]] = ...) -> None: ...
