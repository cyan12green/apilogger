# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: api_logger.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x10\x61pi_logger.proto\x12\tapilogger\"\x1d\n\tSingleLog\x12\x10\n\x08log_data\x18\x01 \x01(\x0c\"\x1d\n\x08\x42\x61tchLog\x12\x11\n\tlog_batch\x18\x01 \x03(\x0c\"\x07\n\x05\x45mpty2\x7f\n\tApiLogger\x12\x39\n\rsendSingleLog\x12\x14.apilogger.SingleLog\x1a\x10.apilogger.Empty\"\x00\x12\x37\n\x0csendBatchLog\x12\x13.apilogger.BatchLog\x1a\x10.apilogger.Empty\"\x00\x42\x17\n\x13\x63om.proto.apiloggerP\x01\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'api_logger_pb2', _globals)
if _descriptor._USE_C_DESCRIPTORS == False:
  DESCRIPTOR._options = None
  DESCRIPTOR._serialized_options = b'\n\023com.proto.apiloggerP\001'
  _globals['_SINGLELOG']._serialized_start=31
  _globals['_SINGLELOG']._serialized_end=60
  _globals['_BATCHLOG']._serialized_start=62
  _globals['_BATCHLOG']._serialized_end=91
  _globals['_EMPTY']._serialized_start=93
  _globals['_EMPTY']._serialized_end=100
  _globals['_APILOGGER']._serialized_start=102
  _globals['_APILOGGER']._serialized_end=229
# @@protoc_insertion_point(module_scope)
