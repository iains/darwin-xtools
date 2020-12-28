//===- libtapi/Version.cpp - TAPI Version Interface -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the C++ version interface.
///
//===----------------------------------------------------------------------===//
//
// Modified version for "tapilite", a standalone just enough
// implementation for use without LLVM
//
#include <tapi/Version.h>
#include <tuple>

/// \brief Helper macro for TAPI_VERSION_STRING.
#define TAPI_MAKE_STRING2(X) #X

/// \brief A string that describes the TAPI version number, e.g., "1.0.0".
#define TAPI_MAKE_STRING(X) TAPI_MAKE_STRING2(X)

namespace tapi {

std::string Version::getFullVersionAsString() noexcept {
  std::string result;
#ifdef TAPI_VENDOR
  result += TAPI_VENDOR;
#endif
#ifdef TAPI_VERSION
  result += TAPI_MAKE_STRING(TAPI_VERSION);
#endif
  result += " based on Apple TAPI";
#ifdef APPLE_VERSION
  result += " version " APPLE_VERSION;
#endif
#ifdef TAPI_REPOSITORY_STRING
  result += " (" TAPI_REPOSITORY_STRING ")";
#endif

  return result;
}

} // end namespace tapi.
