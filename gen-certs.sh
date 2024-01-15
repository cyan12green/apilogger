#!/usr/bin/env sh

#creating certs for SSL/TLS testing
OUT_DIR=./certs
mkdir -p $OUT_DIR
cd $OUT_DIR

#clean everything
echo clean $OUT_DIR
rm *.key *.pem *.csr

mypass="2048"

printf "\n\nSELF SIGNED CA\n"
printf "EVERYTHING IS DEFAULT FOR PROMPTS\n"
openssl req -x509 -new -newkey rsa:$mypass -nodes -keyout ca.key -out ca.pem \
  -config ../ca-openssl.cnf -days 3650 -extensions v3_req

printf "\n\nCLIENT ISSUED BY CA\n"
printf "EVERYTHING DEFAULT EXCEPT COMMON NAME == testclient\n\n"
openssl genrsa -out client.key.rsa $mypass
openssl pkcs8 -topk8 -in client.key.rsa -out client.key -nocrypt
openssl req -new -key client.key -out client.csr

openssl x509 -req -CA ca.pem -CAkey ca.key -CAcreateserial -in client.csr \
  -out client.pem -days 3650

printf "\n\nSERVER ISSUED BY CA\n"
printf "EVERYTHING DEFAULT EXCEPT COMMON NAME == localhost\n\n"
openssl genrsa -out server.key.rsa $mypass
openssl pkcs8 -topk8 -in server.key.rsa -out server.key -nocrypt
openssl req -new -key server.key -out server.csr

openssl x509 -req -CA ca.pem -CAkey ca.key -CAcreateserial -in server.csr \
  -out server.pem -days 3650

