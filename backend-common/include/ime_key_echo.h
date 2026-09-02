// Copyright 2025 Divy Srivastava. All rights reserved. MIT license.
//
// Drop the extra keyDown Japanese IMEs post for the same physical key.

#ifndef LAUFEY_IME_KEY_ECHO_H_
#define LAUFEY_IME_KEY_ECHO_H_

#include <laufey.h>

#include <chrono>
#include <string>

namespace laufey_common {

struct ImeKeyEcho {
  std::string key;
  std::string code;
  std::chrono::steady_clock::time_point time{};
  bool valid = false;
};

inline bool ConsumeImeKeyEcho(ImeKeyEcho* last, int state, const char* key,
                              const char* code) {
  if (!last || state != LAUFEY_KEY_PRESSED) {
    return false;
  }
  auto now = std::chrono::steady_clock::now();
  std::string k = key ? key : "";
  std::string c = code ? code : "";
  if (last->valid &&
      (now - last->time) < std::chrono::milliseconds(40)) {
    bool same_code =
        !c.empty() && c != "Unidentified" && c == last->code;
    bool same_key = !k.empty() && k != "Unidentified" && k == last->key;
    if (same_code || same_key) {
      return true;
    }
  }
  last->key = std::move(k);
  last->code = std::move(c);
  last->time = now;
  last->valid = true;
  return false;
}

}  // namespace laufey_common

#endif  // LAUFEY_IME_KEY_ECHO_H_
