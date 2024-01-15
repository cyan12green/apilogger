#!/usr/bin/env python

import sys
import yaml

sys.path.insert(1, "../src/python")
import api_logger_client as alc

client_key  = "../certs/client.key"
client_cert = "../certs/client.pem"
CA_cert     = "../certs/ca.pem"

with open('../config.yaml', 'r') as file:
    config = yaml.safe_load(file)

if __name__ == "__main__":
    client = alc.ApiLogger(b'localhost:50051', config['use_tls'], client_key, client_cert, CA_cert, 10)

    print('testing single log')

    client.sendLog('single log', client.log_priority.HIGH)


    for i in range(15):
        client.sendLog('batch log' + str(i), client.log_priority.LOW)
