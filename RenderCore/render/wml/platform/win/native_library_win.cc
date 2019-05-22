// Copyright 2018 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "wml/native_library.h"

#include <windows.h>

#include "wml/platform/win/wstring_conversion.h"

namespace wml {

NativeLibrary::NativeLibrary(const char* path)
    : handle_(nullptr), close_handle_(true) {
  if (path == nullptr) {
    return;
  }

  handle_ = ::LoadLibrary(ConvertToWString(path).c_str());
}

NativeLibrary::NativeLibrary(Handle handle, bool close_handle)
    : handle_(handle), close_handle_(close_handle) {}

NativeLibrary::~NativeLibrary() {
  if (handle_ != nullptr && close_handle_) {
    ::FreeLibrary(handle_);
  }
}

NativeLibrary::Handle NativeLibrary::GetHandle() const {
  return handle_;
}

wml::RefPtr<NativeLibrary> NativeLibrary::Create(const char* path) {
  auto library = wml::AdoptRef(new NativeLibrary(path));
  return library->GetHandle() != nullptr ? library : nullptr;
}

wml::RefPtr<NativeLibrary> NativeLibrary::CreateWithHandle(
    Handle handle,
    bool close_handle_when_done) {
  auto library =
      wml::AdoptRef(new NativeLibrary(handle, close_handle_when_done));
  return library->GetHandle() != nullptr ? library : nullptr;
}

wml::RefPtr<NativeLibrary> NativeLibrary::CreateForCurrentProcess() {
  return wml::AdoptRef(new NativeLibrary(::GetModuleHandle(nullptr), false));
}

const uint8_t* NativeLibrary::ResolveSymbol(const char* symbol) {
  if (symbol == nullptr || handle_ == nullptr) {
    return nullptr;
  }
  return reinterpret_cast<const uint8_t*>(::GetProcAddress(handle_, symbol));
}

}  // namespace wml
