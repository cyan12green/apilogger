const google_protobuf_empty_pb = require('google-protobuf/google/protobuf/empty_pb.js');

const {
  SingleLog,
  BatchLog,
} = require('./api_logger_pb.js');

const {ApiLoggerClient} = require('./api_logger_grpc_web_pb.js');

const grpc = {}
grpc.web = require('grpc-web');

class ApiLoggerClientImpl {
  static client;
  static batch_size_;
  static log_batch_ = [];
  static log_priority = Object.freeze({
    HIGH: 0,
    LOW: 1
  });
  
  constructor(server_url, batch_size) {
    ApiLoggerClientImpl.client = new ApiLoggerClient(server_url);
    ApiLoggerClientImpl.batch_size_ = batch_size;
  }
  
  //priority is assumed to be low by default
  sendLogs(logs, priority) {
    if ( !(typeof logs === 'string' || logs instanceof String) )
      throw new TypeError("input not a string")
    
    logs = new TextEncoder().encode(logs)

    if (priority == ApiLoggerClientImpl.log_priority.HIGH) {
      var request = new SingleLog();
      request.setLogData(logs)
      ApiLoggerClientImpl.client.sendSingleLog(request, {}, (err, response) => {
        if (err) {
          console.log(`Unexpected error for sendSingleLog: code = ${err.code}` +
             `, message = "${err.message}"`);
          return -1;
        } else {
          return 0;
        }
      })
    }
   else { 
     if ( ApiLoggerClientImpl.log_batch_.push(logs) < ApiLoggerClientImpl.batch_size_ )
       return 0;

     var request = new BatchLog();
     request.setLogBatchList(ApiLoggerClientImpl.log_batch_);
     ApiLoggerClientImpl.log_batch_ = []
     ApiLoggerClientImpl.client.sendBatchLog(request, {}, (err, response) => {
       if (err) {
         console.log(`Unexpected error for sendBatchLog: code = ${err.code}` +
           `, message = "${err.message}"`);
         return -1;

       } else {
         return 0;
       }
     })  
   }
  }
}

/*
var client = new ApiLoggerClientImpl('http://localhost:8080', 10)

client.sendLogs("something to log", ApiLoggerClientImpl.log_priority.HIGH)

for (var t = 0; t < 10; t++) {
  client.sendLogs("js logs1", ApiLoggerClientImpl.log_priority.LOW)
}
*/

