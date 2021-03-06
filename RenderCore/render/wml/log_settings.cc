// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "wml/log_settings.h"

#include <fcntl.h>
#include <string.h>

#include <algorithm>
#include <iostream>

#include "wml/logging.h"

namespace wml {
namespace state {

// Defined in log_settings_state.cc.
extern LogSettings g_log_settings;

}  // namespace state

void SetLogSettings(const LogSettings& settings) {
  // Validate the new settings as we set them.
  state::g_log_settings.min_log_level =
      std::min(LOG_FATAL, settings.min_log_level);
}

LogSettings GetLogSettings() {
  return state::g_log_settings;
}

int GetMinLogLevel() {
  return std::min(state::g_log_settings.min_log_level, LOG_FATAL);
}

}  // namespace wml
