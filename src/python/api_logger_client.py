#!/usr/bin/env python
import os 
import sys
import logging
from enum import Enum

import grpc

import api_logger_pb2 as log_pb
import api_logger_pb2_grpc as log_grpc

_LOGGER = logging.getLogger(__name__)
_LOGGER.setLevel(logging.INFO)


def _load_credential_from_file(filepath):
    real_path = os.path.join(filepath)
    with open(real_path, "rb") as f:
        return f.read()

class ApiLogger():
    
    log_batch_ = list()
    stub_ = None
    batch_size_ = 10

    class log_priority(Enum):
        HIGH = 1
        LOW = 2

    def __init__(self,
                 server_address, 
                 use_tls,
                 client_key,
                 client_cert,
                 CA_cert, 
                 batch_size):
        if (not(use_tls)):
            channel = grpc.insecure_channel(server_address) 
        else:
            channel_credential = grpc.ssl_channel_credentials(
                    _load_credential_from_file(CA_cert),
                    _load_credential_from_file(client_key),
                    _load_credential_from_file(client_cert)
                    )
            channel = grpc.secure_channel(server_address, channel_credential)

        self.stub_ = log_grpc.ApiLoggerStub(channel) 
        self.batch_size_ = batch_size
    
    

    def sendLog(self, logs, priority):
        if type(priority) is not self.log_priority:
            raise TypeError("make sure you are using this class's log_priority")

        if type(logs) is str:
            logs = logs.encode()
        
        if priority is self.log_priority.HIGH :
            request = log_pb.SingleLog()

            request.log_data = logs
            try:
                response = self.stub_.sendSingleLog(request)
                return 0
            except grpc.RpcError as rpc_error:
                _LOGGER.error("Received error: %s", rpc_error)
                return rpc_error
        
        self.log_batch_.append(logs)
        
        if len(self.log_batch_) < self.batch_size_:
            return 0
        

        request = log_pb.BatchLog()
        request.log_batch.extend(self.log_batch_)
        try:
            response = self.stub_.sendBatchLog(request)
            self.log_batch_.clear()
            return 0
        
        except grpc.RpcError as rpc_error:
            _LOGGER.error("Received error: %s", rpc_error)
            return rpc_error


