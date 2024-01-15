/**
 * @fileoverview gRPC-Web generated client stub for apilogger
 * @enhanceable
 * @public
 */

// Code generated by protoc-gen-grpc-web. DO NOT EDIT.
// versions:
// 	protoc-gen-grpc-web v1.4.2
// 	protoc              v4.23.1
// source: api_logger.proto


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');


var google_protobuf_empty_pb = require('google-protobuf/google/protobuf/empty_pb.js')
const proto = {};
proto.apilogger = require('./api_logger_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.apilogger.ApiLoggerClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname.replace(/\/+$/, '');

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.apilogger.ApiLoggerPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'binary';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname.replace(/\/+$/, '');

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.apilogger.SingleLog,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_ApiLogger_sendSingleLog = new grpc.web.MethodDescriptor(
  '/apilogger.ApiLogger/sendSingleLog',
  grpc.web.MethodType.UNARY,
  proto.apilogger.SingleLog,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.apilogger.SingleLog} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.apilogger.SingleLog} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.apilogger.ApiLoggerClient.prototype.sendSingleLog =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/apilogger.ApiLogger/sendSingleLog',
      request,
      metadata || {},
      methodDescriptor_ApiLogger_sendSingleLog,
      callback);
};


/**
 * @param {!proto.apilogger.SingleLog} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.apilogger.ApiLoggerPromiseClient.prototype.sendSingleLog =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/apilogger.ApiLogger/sendSingleLog',
      request,
      metadata || {},
      methodDescriptor_ApiLogger_sendSingleLog);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.apilogger.BatchLog,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_ApiLogger_sendBatchLog = new grpc.web.MethodDescriptor(
  '/apilogger.ApiLogger/sendBatchLog',
  grpc.web.MethodType.UNARY,
  proto.apilogger.BatchLog,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.apilogger.BatchLog} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.apilogger.BatchLog} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.apilogger.ApiLoggerClient.prototype.sendBatchLog =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/apilogger.ApiLogger/sendBatchLog',
      request,
      metadata || {},
      methodDescriptor_ApiLogger_sendBatchLog,
      callback);
};


/**
 * @param {!proto.apilogger.BatchLog} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.apilogger.ApiLoggerPromiseClient.prototype.sendBatchLog =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/apilogger.ApiLogger/sendBatchLog',
      request,
      metadata || {},
      methodDescriptor_ApiLogger_sendBatchLog);
};


module.exports = proto.apilogger;

