
#ifndef BASE__CLOUDBROWSER__XHDY_HELPERS_H
#define BASE__CLOUDBROWSER__XHDY_HELPERS_H

#include <mutex>
#include "base/command_line.h"

__attribute__((__unused__))
static inline bool __is_hdy_headers_on(void)
{
  static bool is_on;
  static bool checked = false;
  static std::mutex mut;
  const base::CommandLine* command_line;

  if (checked)
    return is_on;

  mut.lock();
  if (checked) {
    mut.unlock();
    return is_on;
  }
  checked = true;
  command_line = base::CommandLine::ForCurrentProcess();
  is_on = command_line->HasSwitch("x-hdy-headers");
  mut.unlock();
  return is_on;
}

__attribute__((__unused__))
static inline const char* __get_browser_id(void)
{
  static char browser_id[255];
  static bool checked = false;
  static std::mutex mut;
  const base::CommandLine* command_line = nullptr;

  if (checked)
    return browser_id;

  mut.lock();
  if (checked) {
    mut.unlock();
    return browser_id;
  }
  checked = true;
  command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch("browser-id")) {
    std::string tmp = command_line->GetSwitchValueASCII("browser-id");
    strncpy(browser_id, tmp.c_str(), sizeof(browser_id) - 1);
  }
  mut.unlock();
  return browser_id;
}

bool is_hdy_headers_on(void);
const char* get_browser_id(void);

#endif /* #ifndef BASE__CLOUDBROWSER__XHDY_HELPERS_H */
