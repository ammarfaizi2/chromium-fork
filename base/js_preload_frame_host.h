// From: Ammar Faizi <ammarfaizi2@gmail.com>
//
// Copyright (c) 2021 Ammar Faizi. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JS_PRELOAD_FRAME_HOST_H_
#define BASE_JS_PRELOAD_FRAME_HOST_H_

// This is used to preload a JavaScript file at initialization. It reads
// JavaScript code from the given filename and saves it on a global
// state. The saved JavaScript code will be reused as much as possible.
// The only thing that requires us to re-open the file is that when we
// have a separate process that doesn't have the `g_js_preload` yet (In
// other words, when `g_js_preload` is still nullptr).

#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include "base/logging.h"
#include "base/check_op.h"

namespace base {

class JSPreloadFrameHost {
 public:
  inline bool hasJavaScriptCode() {
    return js_code_ != nullptr;
  }

  ~JSPreloadFrameHost() {
    if (js_code_)
      free(js_code_);
  }

  explicit JSPreloadFrameHost(const char *filename) {
    if (!filename)
      // The caller does not expect a preloaded JS, skip it.
      return;

    FILE* handle = std::fopen(filename, "r");
    if (!handle) {
      int err = errno;
      DLOG(ERROR)
        << "Cannot open \"" << filename << "\" for JSPreloadFrameHost: "
        << std::strerror(err);
      return;
    }

    std::fseek(handle, 0, SEEK_END);
    int32_t size = (int32_t) ftell(handle);
    CHECK_GE(size, -1);
    std::fseek(handle, 0, SEEK_SET);

    js_code_ = static_cast<char*>(malloc(size + 1));
    if (!js_code_) {
      std::fclose(handle);
      DLOG(ERROR) << "Cannot allocate memory for JSPreloadFrameHost";
      return;
    }

    size_t read_ret = std::fread(js_code_, sizeof(char), size, handle);
    if (read_ret == (size_t) size) {
      js_code_[size] = '\0';
    } else {
      free(js_code_);
      js_code_ = nullptr;
    }
    std::fclose(handle);
  }

  inline const char* getJavaScriptCode() {
    return js_code_;
  }

 private:
  char* js_code_ = nullptr;
};

}  // namespace base

extern base::JSPreloadFrameHost* g_js_preload;

#endif  // BASE_JS_PRELOAD_FRAME_HOST_H_
