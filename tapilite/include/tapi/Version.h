//===-- tapi/Version.h - TAPI Version Interface -----------------*- C++ -*-===*\
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Access the TAPI version information.
/// \since 1.0
///
//===----------------------------------------------------------------------===//
//
// Modified version for "tapilite", a standalone just enough
// implementation for use without LLVM
//
#ifndef TAPI_VERSION_H
#define TAPI_VERSION_H

#include <string>
#include <tapi/Defines.h>

///
/// \defgroup TAPI_VERSION Version methods
/// \ingroup TAPI_CPP_API
///
/// @{
///

namespace tapi {

///
/// \brief Access to version related information about the TAPI dynamic library.
/// \since 1.0
///
class TAPI_PUBLIC Version {
public:
  ///
  /// \name Version Number Methods
  /// @{
  ///

  ///
  /// \brief Get the full library name and version as string.
  /// \return A string with the program name and version number.
  /// \since 1.0
  ///
  static std::string getFullVersionAsString() noexcept;

  ///
  /// @}
  ///
};

} // namespace tapi

///
/// @}
///

#endif // TAPI_VERSION_H
