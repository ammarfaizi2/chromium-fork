// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"

// The entry point for all invocations of Chromium, browser and renderer. On
// windows, this does nothing but load chrome.dll and invoke its entry point in
// order to make it easy to update the app from GoogleUpdate. We don't need
// that extra layer with on linux.

extern "C" {
int ChromeMain(int argc, const char** argv);
}

#if defined(__linux__)
#include <cstring>
#include <cstdlib>
#include <cerrno>
#endif

int main(int argc, const char** argv) {

#if defined(__linux__)
  char** argv_p;
  int ret;
  int i;

  argv_p = (char**)calloc((size_t)argc + 1, sizeof(*argv_p));
  if (!argv_p)
    return ENOMEM;

  for (i = 0; i < argc; i++) {
    char *p;
    size_t l;

    l = strlen(argv[i]);
    p = (char*)malloc(l + 1);
    if (!p)
      goto out_free;

    memcpy(p, argv[i], l + 1);
    argv_p[i] = p;
    memset(const_cast<char*>(argv[i]), '\0', l);
  }

  ret = ChromeMain(argc, const_cast<const char**>(argv_p));

out_free:
  for (i = 0; i < argc; i++)
    free(argv_p[i]);

  free(argv_p);

  return ret;
#else
  return ChromeMain(argc, argv);
#endif
}
