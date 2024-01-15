#include "../../../../src/cpp/key_exchange_client.h"
#include <napi.h>

Napi::Value EncodeSafe(const Napi::CallbackInfo& info) {
  
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string arg = info[0].As<Napi::String>().Utf8Value();
  Napi::String str = Napi::String::New(env, encodeSafe(arg));

  return str;
}

Napi::Value DecodeSafe(const Napi::CallbackInfo& info) {
  
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::string arg = info[0].As<Napi::String>().Utf8Value();

  Napi::String str = Napi::String::New(env, decodeSafe(arg));

  return str;
}
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "EncodeSafe"),
              Napi::Function::New(env, EncodeSafe));
  
  exports.Set(Napi::String::New(env, "DecodeSafe"),
              Napi::Function::New(env, DecodeSafe));
  return exports;
}

NODE_API_MODULE(addon, Init)
