{
  "targets": [
    {
      "target_name": "addon",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [ "addon.cc" ],
        'include_dirs': [
          "<!@(node -p \"require('node-addon-api').include\")",
          "../../../../../src/cpp/",
          "/home/cyangreen/.local/include"
        ],
        'libraries': [
          "/usr/local/lib/libwolfssl.so.35",
          "../../../../../cmake/build/libkeyex_module.so"
        ],
        'dependencies': [
          "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_C_EXCEPTIONS' ]
    }
  ]
}
