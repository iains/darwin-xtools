//===- libtapi/LinkerInterfaceFile.cpp - TAPI File Interface ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the C++ linker interface file API.
///
//===----------------------------------------------------------------------===//
//
// Modified version for "tapilite", a standalone just enough
// implementation for use without LLVM
//
#include <string>
#include <tapi/LinkerInterfaceFile.h>
#include <tapi/PackedVersion32.h>
#include <tapi/Symbol.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>

#include <yaml.h>

#include <MachOFileAbstraction.hpp>

TAPI_NAMESPACE_V1_BEGIN

//using namespace tapi::internal;

static PackedVersion32 parseVersion32(std::string str) {
  size_t    len = 0;
  uint32_t  version = 0;
  size_t    num = 0;

  if (str.empty())
    return 0;

  len = str.find_first_of(".");
  if (len == std::string::npos)
    return 0;

  num = std::stoull(str, &len, 10);
  if (len == 0)
    return 0;

  if (num > UINT16_MAX)
    return 0;

  version = num << 16;
  str = str.substr(len + 1);

  len = str.find_first_of(".");
  if (len != std::string::npos) {
    num = std::stoull(str, &len, 10);
    if (len == 0)
      return 0;

    if (num > UINT8_MAX)
      return 0;

    version |= (num << 8);
    str = str.substr(len + 1);
  }

  if (!str.empty()) {
    num = std::stoull(str, &len, 10);
    if (len == 0)
      return 0;

    if (num > UINT8_MAX)
      return 0;

    version |= num;
  }

  return version;
}

class LinkerInterfaceFile::ImplData {
public:
  std::string _path;
  FileType _fileType{FileType::Unsupported};
  Platform _platform{Platform::Unknown};
  std::string _installName;

  PackedVersion32 _currentVersion;
  PackedVersion32 _compatibilityVersion;
  unsigned _swiftABIVersion;
  ObjCConstraint _objcConstraint;

  std::vector<std::string> _reexportedLibraries;
  std::vector<std::string> _allowableClients;
  std::vector<Symbol *> _exports;
  std::vector<Symbol *> _undefineds;

  std::vector<std::string> _arches;
  std::string _selectedArch;

  LinkerInterfaceFile::ImplData *next;

  ImplData() noexcept = default;

  static LinkerInterfaceFile::ImplData *
    loadFile(const std::string &path, const uint8_t *data, size_t size,
        cpu_type_t cpuType, cpu_subtype_t cpuSubType,
        std::string &errorMessage);
};

class LinkerInterfaceFile::Impl {
public:
  std::shared_ptr<const LinkerInterfaceFile::ImplData> _data;

  std::string _installName;
  std::string _parentFrameworkName;
  PackedVersion32 _compatibilityVersion;
  std::vector<std::string> _ignoreExports;
  std::vector<std::string> _inlinedFrameworkNames;
  std::vector<Symbol> _exports;
  std::vector<Symbol> _undefineds;

  bool _hasTwoLevelNamespace{false};
  bool _isAppExtensionSafe{false};
  bool _hasWeakDefExports{false};
  bool _installPathOverride{false};

  Impl() noexcept = default;

  bool init(const std::shared_ptr<const LinkerInterfaceFile::ImplData> &data,
            cpu_type_t cpuType, cpu_subtype_t cpuSubType, ParsingFlags flags,
            PackedVersion32 minOSVersion, std::string &errorMessage) noexcept;

  template <typename T> void addSymbol(T &&name, SymbolFlags flags) {
    if (std::find(_ignoreExports.begin(), _ignoreExports.end(),
				name) == _ignoreExports.end())
      _exports.emplace_back(std::forward<T>(name), flags);
  }

  void processSymbol(std::string name, PackedVersion32 minOSVersion,
                     bool disallowWeakImports) {
    // $ld$ <action> $ <condition> $ <symbol-name>
    if (name.substr(0, sizeof("$ld$") - 1).compare("$ld$") != 0)
      return;

    name = name.substr(sizeof("$ld$") - 1);

    size_t pos = name.find_first_of("$");
    if (pos == std::string::npos)
      return;
    auto action = name.substr(0, pos);
    name = name.substr(pos + 1);

    pos = name.find_first_of("$");
    if (pos == std::string::npos)
      return;
    auto condition = name.substr(0, pos);
    name = name.substr(pos + 1);

    if (name.empty())
      return;
    auto symbolName = name; 

    if (condition.substr(0, sizeof("os") - 1).compare("os") != 0)
      return;

    auto version = parseVersion32(condition.substr(sizeof("os") - 1));
    if (version != minOSVersion)
      return;

    if (action.compare("hide") == 0) {
      _ignoreExports.emplace_back(symbolName);
      return;
    }

    if (action.compare("add") == 0) {
      _exports.emplace_back(symbolName);
      return;
    }

    if (action.compare("weak") == 0) {
      if (disallowWeakImports)
        _ignoreExports.emplace_back(symbolName);

      return;
    }

    if (action.compare("install_name") == 0) {
      _installName = symbolName;
      _installPathOverride = true;
      if (_installName == "/System/Library/Frameworks/"
                          "ApplicationServices.framework/Versions/A/"
                          "ApplicationServices") {
        _compatibilityVersion = parseVersion32("1.0");
      }
      return;
    }

    if (action.compare("compatibility_version") == 0) {
      _compatibilityVersion = parseVersion32(symbolName);
      return;
    }
  }
};

static std::string getArchForCPU(cpu_type_t cpuType, cpu_subtype_t cpuSubType,
                                  bool enforceCpuSubType) {
  /* this function is deliberately stupid, for the fine details of
   * subtypes we need more checking, which isn't necessary for the
   * general case */

  switch (cpuType) {
    case CPU_TYPE_I386:
      return "i386";
    case CPU_TYPE_X86_64:
      return "x86_64";
    case CPU_TYPE_ARM:
      return "arm";
    case CPU_TYPE_POWERPC:
      return "ppc";
    case CPU_TYPE_POWERPC64:
      return "ppc64";
    case CPU_TYPE_ARM64:
      return "arm64";
	case CPU_TYPE_ANY:
	  return "";  /* meant to match first arch */
    default:
      return "unsupported";
  }
}

LinkerInterfaceFile::LinkerInterfaceFile() noexcept
    : _pImpl{new LinkerInterfaceFile::Impl} {}
LinkerInterfaceFile::~LinkerInterfaceFile() noexcept = default;
LinkerInterfaceFile::LinkerInterfaceFile(LinkerInterfaceFile &&) noexcept =
    default;
LinkerInterfaceFile &
LinkerInterfaceFile::operator=(LinkerInterfaceFile &&) noexcept = default;

std::vector<std::string>
LinkerInterfaceFile::getSupportedFileExtensions() noexcept {
  return {".tbd"};
}


/// \brief Load and parse the provided TBD file in the buffer and return on
///        success the interface file.
LinkerInterfaceFile::ImplData *LinkerInterfaceFile::ImplData::loadFile(
    const std::string &path, const uint8_t *data, size_t size,
    cpu_type_t cpuType, cpu_subtype_t cpuSubType,
    std::string &errorMessage) {
  yaml_parser_t parser;
  yaml_event_t  event;
  yaml_char_t  *p;
  bool          selectSection = false;
  bool          inList        = false;

  LinkerInterfaceFile::ImplData *ret = nullptr;
  LinkerInterfaceFile::ImplData *cur = nullptr;

  /* Because platform isn't passed onto us by ld, we cannot know what to
   * select -- this is probably the problem meant to be solved by TAPIv4
   * which explicitly refers to targets such that arm64-macos is
   * different from arm64-ios.  Alas, for now it seems this mix isn't
   * there for (system) tbd files, so we just ignore platform, and
   * return the first one that matches the cpu specification. */
  std::string myarch = getArchForCPU(cpuType, cpuSubType, false);

  enum {
    TAPILITE_INIT,
    TAPILITE_HEADER,
    TAPILITE_EXPORTS,
    TAPILITE_UNDEFINEDS,
    TAPILITE_REEXPORTS,
    TAPILITE_DONE
  }             state = TAPILITE_INIT;
  enum {
    TAPILITE_FINDKEY,
    TAPILITE_ARCHS,
    TAPILITE_UUIDS,
    TAPILITE_PLATFORM,
    TAPILITE_FLAGS,
    TAPILITE_INSTALLNAME,
    TAPILITE_CURVERSION,
    TAPILITE_COMPATVERSION,
    TAPILITE_SWIFTVERSION,
    TAPILITE_OBJCCONSTR,
    TAPILITE_PARENTUMBR,
    TAPILITE_ALLOWED_CLNTS,
    TAPILITE_SYMBOLS,
    TAPILITE_RE_EXPORTS,
    TAPILITE_OBJCCLASSES,
    TAPILITE_OBJCEHTYPES,
    TAPILITE_OBJCIVARS,
    TAPILITE_WEAKDEFSYMS,
    TAPILITE_THRLOCSYMS,
    TAPILITE_VERSION
  }             substate = TAPILITE_FINDKEY;
  enum {
    TAPILITE_V_INVALID,
    TAPILITE_V1,
    TAPILITE_V2,
    TAPILITE_V3,
    TAPILITE_V4_OR_LATER,
    TAPILITE_V4
  }             version = TAPILITE_V1;  /* default for tagless */

  yaml_parser_initialize(&parser);
  yaml_parser_set_input_string(&parser,
      reinterpret_cast<const unsigned char *>(data), size);

#define error(...) { \
  char errbuf[256]; \
  snprintf(errbuf, sizeof(errbuf), __VA_ARGS__); \
  errorMessage = errbuf; \
  return nullptr; \
}
#ifdef tapilite_debug
# define dprintf(...) printf(__VA_ARGS)
#else
# define dprintf(...)
#endif
  /* syntax:
   * https://github.com/apple/llvm-project/blob/apple/main/llvm/lib/TextAPI/MachO/TextStub.cpp */
  while (state != TAPILITE_DONE) {
    if (state != TAPILITE_INIT)
      yaml_event_delete(&event);
    else
      state = TAPILITE_HEADER;

    if (!yaml_parser_parse(&parser, &event))
      break;

    dprintf("yaml: %d\n", event.type);
#define ymlcmp(X, Y) strcmp((char *)X, Y)
#define ymlncmp(X, Y, Z) strncmp((char *)X, Y, Z)
    /* process */
    switch (event.type) {
      case YAML_DOCUMENT_START_EVENT:
        if (cur == nullptr) {
          ret = cur = new LinkerInterfaceFile::ImplData;
        } else {
          cur = cur->next = new LinkerInterfaceFile::ImplData;
        }
        cur->_path = path;
        cur->next = nullptr;
        break;
      case YAML_STREAM_END_EVENT:
        state = TAPILITE_DONE;
        break;
      case YAML_SCALAR_EVENT:
#define yamlscalar event.data.scalar
        if (yamlscalar.value != NULL && substate == TAPILITE_FINDKEY) {
          switch (state) {
            case TAPILITE_HEADER:
              dprintf("header| key: %s\n", yamlscalar.value);
              if (ymlcmp(yamlscalar.value, "archs") == 0)
                substate = TAPILITE_ARCHS;
              if (ymlcmp(yamlscalar.value, "targets") == 0)
                substate = TAPILITE_ARCHS;
              else if (ymlcmp(yamlscalar.value, "platform") == 0)
                substate = TAPILITE_PLATFORM;
              else if (ymlcmp(yamlscalar.value, "install-name") == 0)
                substate = TAPILITE_INSTALLNAME;
              else if (ymlcmp(yamlscalar.value, "current-version") == 0)
                substate = TAPILITE_CURVERSION;
              else if (ymlcmp(yamlscalar.value, "compatibility-version") == 0)
                substate = TAPILITE_COMPATVERSION;
              else if (ymlcmp(yamlscalar.value, "swift-version") == 0)
                substate = TAPILITE_SWIFTVERSION;
              else if (ymlcmp(yamlscalar.value, "swift-abi-version") == 0)
                substate = TAPILITE_SWIFTVERSION;
              else if (ymlcmp(yamlscalar.value, "objc-constraint") == 0)
                substate = TAPILITE_OBJCCONSTR;
              else if (ymlcmp(yamlscalar.value, "uuids") == 0)
                substate = TAPILITE_UUIDS;
              else if (ymlcmp(yamlscalar.value, "parent-umbrella") == 0)
                substate = TAPILITE_PARENTUMBR;
              else if (ymlcmp(yamlscalar.value, "allowable-clients") == 0)
                substate = TAPILITE_ALLOWED_CLNTS;
              else if (ymlcmp(yamlscalar.value, "flags") == 0)
                substate = TAPILITE_FLAGS;
              else if (ymlcmp(yamlscalar.value, "tbd-version") == 0)
                substate = TAPILITE_VERSION;
              else if (ymlcmp(yamlscalar.value, "exports") == 0)
                state = TAPILITE_EXPORTS;
              else if (ymlcmp(yamlscalar.value, "reexported-libraries") == 0)
                state = TAPILITE_REEXPORTS;
              else if (ymlcmp(yamlscalar.value, "undefineds") == 0)
                state = TAPILITE_UNDEFINEDS;
              break;
            case TAPILITE_EXPORTS:
            case TAPILITE_REEXPORTS:
              dprintf("exports| key: %s\n", yamlscalar.value);
              if (ymlcmp(yamlscalar.value, "archs") == 0)
                substate = TAPILITE_ARCHS;
              if (ymlcmp(yamlscalar.value, "targets") == 0)
                substate = TAPILITE_ARCHS;
              else if (ymlcmp(yamlscalar.value, "allowed-clients") == 0)
                substate = TAPILITE_ALLOWED_CLNTS;
              else if (ymlcmp(yamlscalar.value, "re-exports") == 0)
                substate = TAPILITE_RE_EXPORTS;
              else if (ymlcmp(yamlscalar.value, "libraries") == 0)
                substate = TAPILITE_RE_EXPORTS;
              else if (ymlcmp(yamlscalar.value, "symbols") == 0)
                substate = TAPILITE_SYMBOLS;
              else if (ymlcmp(yamlscalar.value, "objc-classes") == 0)
                substate = TAPILITE_OBJCCLASSES;
              else if (ymlcmp(yamlscalar.value, "objc-eh-types") == 0)
                substate = TAPILITE_OBJCEHTYPES;
              else if (ymlcmp(yamlscalar.value, "objc-ivars") == 0)
                substate = TAPILITE_OBJCIVARS;
              else if (ymlcmp(yamlscalar.value, "weak-def-symbols") == 0)
                substate = TAPILITE_WEAKDEFSYMS;
              else if (ymlcmp(yamlscalar.value, "weak-symbols") == 0)
                substate = TAPILITE_WEAKDEFSYMS;
              else if (ymlcmp(yamlscalar.value, "thread-local-symbols") == 0)
                substate = TAPILITE_THRLOCSYMS;
              break;
            case TAPILITE_UNDEFINEDS:
              if (ymlcmp(yamlscalar.value, "archs") == 0)
                substate = TAPILITE_ARCHS;
              if (ymlcmp(yamlscalar.value, "targets") == 0)
                substate = TAPILITE_ARCHS;
              else if (ymlcmp(yamlscalar.value, "symbols") == 0)
                substate = TAPILITE_SYMBOLS;
              else if (ymlcmp(yamlscalar.value, "objc-classes") == 0)
                substate = TAPILITE_OBJCCLASSES;
              else if (ymlcmp(yamlscalar.value, "objc-eh-types") == 0)
                substate = TAPILITE_OBJCEHTYPES;
              else if (ymlcmp(yamlscalar.value, "objc-ivars") == 0)
                substate = TAPILITE_OBJCIVARS;
              else if (ymlcmp(yamlscalar.value, "weak-ref-symbols") == 0)
                substate = TAPILITE_WEAKDEFSYMS;
              else if (ymlcmp(yamlscalar.value, "weak-symbols") == 0)
                substate = TAPILITE_WEAKDEFSYMS;
              break;
          }

          /* reset selection selector */
          if (substate == TAPILITE_ARCHS)
            selectSection = false;
        } else if (substate != TAPILITE_FINDKEY) {
          /* we have a real value, need to append it to whatever is
           * currently the open thing we're writing for */
          switch (state) {
            case TAPILITE_HEADER:
              dprintf("header|%d val: %s\n", substate, yamlscalar.value);
              switch (substate) {
                case TAPILITE_VERSION:
                  if (version == TAPILITE_V4_OR_LATER) {
                    if (ymlcmp(yamlscalar.value, "4") == 0)
                      version = TAPILITE_V4;
                    else
                      error("unsupported TAPI version %s", yamlscalar.value);
                  } else {
                    error("found tbd-version for TAPI version <v4");
                  }
                  break;
                case TAPILITE_ARCHS:
                  /* this is the point where version needs to be set */
                  if (version == TAPILITE_V4_OR_LATER ||
                      version == TAPILITE_V_INVALID)
                    error("unknown tbd file, unknown version specified");

                  if (cur->_fileType == FileType::Unsupported) {
                    /* apply defaults for various settings based on the
                     * TAPI version */
                    switch (version) {
                      case TAPILITE_V1:
                        cur->_currentVersion = parseVersion32("1.0");
                        cur->_compatibilityVersion = parseVersion32("1.0");
                        cur->_swiftABIVersion = 0;
                        cur->_objcConstraint = ObjCConstraint::None;
                        break;
                      case TAPILITE_V2:
                      case TAPILITE_V3:
                      case TAPILITE_V4:
                        cur->_currentVersion = parseVersion32("1.0");
                        cur->_compatibilityVersion = parseVersion32("1.0");
                        cur->_swiftABIVersion = 0;
                        cur->_objcConstraint = ObjCConstraint::Retain_Release;
                        break;
                    }

                    if (version == TAPILITE_V1)
                      cur->_fileType = FileType::TBD_V1;
                    else if (version == TAPILITE_V2)
                      cur->_fileType = FileType::TBD_V2;
                    else if (version == TAPILITE_V3)
                      cur->_fileType = FileType::TBD_V3;
                    else if (version == TAPILITE_V4)
                      cur->_fileType = FileType::TBD_V4;
                  }

                  cur->_arches.emplace_back(
                      std::string((const char *)yamlscalar.value));
                  if (!cur->_selectedArch.empty())
                    break;  /* take first matching arch */

                  switch (version) {
                    case TAPILITE_V4:
                      {
                        /* extract arch, platform out of target, set
                         * platform if arch matches */
                        size_t len = myarch.length();
                        if (ymlncmp(yamlscalar.value, myarch.c_str(), len) == 0
                            && (len == 0 || yamlscalar.value[len] == '-'))
                        {
                          const char *p = (const char *)yamlscalar.value;
                          cur->_selectedArch = std::string(p);
                          if (len == 0)
                            len = cur->_selectedArch.find_first_of('-');
                          /* this almost is a copy of the PLATFORM case below */
                          if (cur->_selectedArch.
                              substr(len + 1).compare("macos") == 0)
                            cur->_platform = Platform::OSX;
                          else if (cur->_selectedArch.
                              substr(len + 1).compare("ios") == 0)
                            cur->_platform = Platform::iOS;
                          /* TODO: see below */
                        }
                      }
                      break;
                    default:
                      if (myarch.empty()) {
                        cur->_selectedArch =
                          std::string((const char *)yamlscalar.value);
                      } else {
                        /* see if arch matches */
                        if (ymlcmp(yamlscalar.value, myarch.c_str()))
                          cur->_selectedArch = myarch;
                      }
                      break;
                  }
                  break;
                case TAPILITE_PLATFORM:
                  if (ymlcmp(yamlscalar.value, "macosx") == 0)
                    cur->_platform = Platform::OSX;
                  else if (ymlcmp(yamlscalar.value, "ios") == 0)
                    cur->_platform = Platform::iOS;
                  /* TODO: does it really make a difference to check for
                   * the rest? */
                  break;
                case TAPILITE_INSTALLNAME:
                  cur->_installName =
                    std::string((const char *)yamlscalar.value);
                  break;
                case TAPILITE_CURVERSION:
                  cur->_currentVersion =
                    parseVersion32((const char *)yamlscalar.value);
                  break;
                case TAPILITE_COMPATVERSION:
                  cur->_compatibilityVersion =
                    parseVersion32((const char *)yamlscalar.value);
                  break;
                case TAPILITE_SWIFTVERSION:
                  cur->_swiftABIVersion =
                    std::stoull((const char *)yamlscalar.value, NULL, 10);
                  break;
                case TAPILITE_OBJCCONSTR:
                  if (ymlcmp(yamlscalar.value, "none") == 0)
                    cur->_objcConstraint = ObjCConstraint::None;
                  else if (ymlcmp(yamlscalar.value, "retain_release") == 0)
                    cur->_objcConstraint = ObjCConstraint::Retain_Release;
                  else if (ymlcmp(yamlscalar.value,
                        "retain_release_for_simulator") == 0)
                    cur->_objcConstraint =
                      ObjCConstraint::Retain_Release_For_Simulator;
                  else if (ymlcmp(yamlscalar.value,
                        "retain_release_or_gc") == 0)
                    cur->_objcConstraint =
                      ObjCConstraint::Retain_Release_Or_GC;
                  else if (ymlcmp(yamlscalar.value, "gc") == 0)
                    cur->_objcConstraint = ObjCConstraint::GC;
                  break;
                case TAPILITE_ALLOWED_CLNTS:
                  cur->_allowableClients.emplace_back(
                      std::string((const char *)yamlscalar.value));
                  break;
                case TAPILITE_UUIDS:
                case TAPILITE_PARENTUMBR:
                case TAPILITE_FLAGS:
                  /* ignored: there's currently no consumer for it from
                   * ld64 */
                  break;
              }
              break;
            case TAPILITE_EXPORTS:
            case TAPILITE_REEXPORTS:
              dprintf("exports|%d val: %s\n", substate, yamlscalar.value);
              switch (substate) {
                case TAPILITE_ARCHS:
                  /* remember: for V4, we store target in selectedArch */
                  if (ymlcmp(yamlscalar.value, cur->_selectedArch.c_str()) == 0)
                    selectSection = true;
                  break;
                case TAPILITE_ALLOWED_CLNTS:
                  /* should respect this, but for now we just ignore it */
                  break;
                case TAPILITE_RE_EXPORTS:
                  if (!selectSection)
                    break;
                  cur->_reexportedLibraries.emplace_back(
                      std::string((const char *)yamlscalar.value));
                  break;
                case TAPILITE_SYMBOLS:
                  if (!selectSection)
                    break;
                  cur->_exports.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None, SymbolKind::GlobalSymbol));
                  break;
                case TAPILITE_OBJCCLASSES:
                  if (!selectSection)
                    break;
                  cur->_exports.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None, SymbolKind::ObjectiveCClass));
                  break;
                case TAPILITE_OBJCEHTYPES:
                  if (!selectSection)
                    break;
                  cur->_exports.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None, SymbolKind::ObjectiveCClassEHType));
                  break;
                case TAPILITE_OBJCIVARS:
                  if (!selectSection)
                    break;
                  cur->_exports.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None,
                      SymbolKind::ObjectiveCInstanceVariable));
                  break;
                case TAPILITE_WEAKDEFSYMS:
                  if (!selectSection)
                    break;
                  cur->_exports.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::WeakDefined, SymbolKind::GlobalSymbol));
                  break;
                case TAPILITE_THRLOCSYMS:
                  if (!selectSection)
                    break;
                  cur->_exports.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::ThreadLocalValue, SymbolKind::GlobalSymbol));
                  break;
              }
              break;
            case TAPILITE_UNDEFINEDS:
              switch (substate) {
                case TAPILITE_ARCHS:
                  /* remember: for V4, we store target in selectedArch */
                  if (ymlcmp(yamlscalar.value, cur->_selectedArch.c_str()) == 0)
                    selectSection = true;
                  break;
                case TAPILITE_SYMBOLS:
                  if (!selectSection)
                    break;
                  cur->_undefineds.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None, SymbolKind::GlobalSymbol));
                  break;
                case TAPILITE_OBJCCLASSES:
                  if (!selectSection)
                    break;
                  cur->_undefineds.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None, SymbolKind::ObjectiveCClass));
                  break;
                case TAPILITE_OBJCEHTYPES:
                  if (!selectSection)
                    break;
                  cur->_undefineds.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None, SymbolKind::ObjectiveCClassEHType));
                  break;
                case TAPILITE_OBJCIVARS:
                  if (!selectSection)
                    break;
                  cur->_undefineds.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::None,
                      SymbolKind::ObjectiveCInstanceVariable));
                  break;
                case TAPILITE_WEAKDEFSYMS:
                  if (!selectSection)
                    break;
                  cur->_undefineds.emplace_back(new Symbol(
                      std::string((const char *)yamlscalar.value),
                      SymbolFlags::WeakReferenced, SymbolKind::GlobalSymbol));
                  break;
              }
              break;
          }
          if (!inList)
            substate = TAPILITE_FINDKEY;
        }
        break;
      case YAML_SEQUENCE_START_EVENT:
        inList = true;
        break;
      case YAML_SEQUENCE_END_EVENT:
        substate = TAPILITE_FINDKEY;
        inList = false;
        break;
      case YAML_MAPPING_START_EVENT:
#define yamlms event.data.mapping_start
        switch (state) {
          case TAPILITE_HEADER:
            if (yamlms.tag == NULL)
              break;
            if (ymlncmp(yamlms.tag, "!tapi-tbd",
                  sizeof("!tapi-tbd") - 1) != 0)
              break;
            p = yamlms.tag + sizeof("!tapi-tbd") - 1;
            if (*p == '\0') {
              /* this could be version 4 or later, in which case we
               * expect a tbd-version key */
              version = TAPILITE_V4_OR_LATER;
            } else if (*p == '-' && p[1] == 'v') {
              p += 2;
              switch (*p) {
                case '1':
                  /* technically shouldn't be in use */
                  version = TAPILITE_V1;
                  break;
                case '2':
                  version = TAPILITE_V2;
                  break;
                case '3':
                  version = TAPILITE_V3;
                  break;
                default:
                  version = TAPILITE_V_INVALID;
                  break;
              }
            }
            break;
          default:
            /* ignore */
            break;
        }
        substate = TAPILITE_FINDKEY;
        inList = false;
        break;
      case YAML_MAPPING_END_EVENT:
        inList = false;
        /* TODO: could use this to close list pointers or something */
        break;
      default:
        break;
    }
  }

  yaml_event_delete(&event);
  yaml_parser_delete(&parser);
#undef error

  return ret;
}

bool LinkerInterfaceFile::isSupported(const std::string &path,
    const uint8_t *data,
    size_t size) noexcept {
  std::string err;
  auto file = LinkerInterfaceFile::ImplData::loadFile(
      path, data, size, CPU_TYPE_ANY, CPU_SUBTYPE_MULTIPLE, err);
  
  return file && file->_platform != Platform::Unknown;
}

bool LinkerInterfaceFile::shouldPreferTextBasedStubFile(
    const std::string &path) noexcept {
  // Never prefer this, if a dylib exists, take it because it will be
  // more reliable than this "lite" stub, see also below
  return false;
}

bool LinkerInterfaceFile::areEquivalent(const std::string &tbdPath,
                                        const std::string &dylibPath) noexcept {
  // Simple decision, always prefer dylib over textstub if it exists,
  // don't check anything, so we don't have to implement Mach-O reading
  // to check that UUIDs match
  return false;
}

bool LinkerInterfaceFile::Impl::init(
    const std::shared_ptr<const LinkerInterfaceFile::ImplData> &data,
    cpu_type_t cpuType, cpu_subtype_t cpuSubType, ParsingFlags flags,
    PackedVersion32 minOSVersion, std::string &errorMessage) noexcept {
  _data = data;
  bool enforceCpuSubType = flags & ParsingFlags::ExactCpuSubType;
  auto arch = getArchForCPU(cpuType, cpuSubType, enforceCpuSubType);
  if (arch.compare("unsupported") == 0) {
    auto count = data->_arches.size();
    if (count > 1)
      errorMessage = "missing required architecture " +
                     arch + " in file " +
                     data->_path + " (" + std::to_string(count) +
                     " slices)";
    else
      errorMessage = "missing required architecture " +
                     arch + " in file " +
                     data->_path;
    return false;
  }

  _compatibilityVersion = data->_compatibilityVersion;
  _installName = data->_installName;

  // Remove the patch level.
  minOSVersion =
      PackedVersion32(minOSVersion.getMajor(), minOSVersion.getMinor(), 0);

  // Pre-scan for special linker symbols.
  for (auto *symbol : _data->_exports) {
    if (symbol->getKind() != SymbolKind::GlobalSymbol)
      continue;

    processSymbol(symbol->getName(), minOSVersion,
                  flags & ParsingFlags::DisallowWeakImports);
  }
  std::sort(_ignoreExports.begin(), _ignoreExports.end());
  auto last = std::unique(_ignoreExports.begin(), _ignoreExports.end());
  _ignoreExports.erase(last, _ignoreExports.end());

  bool useObjC1ABI =
      (data->_platform == Platform::OSX) && (arch.compare("i386") == 0);
  for (const auto *symbol : data->_exports) {
    switch (symbol->getKind()) {
    case SymbolKind::GlobalSymbol:
      if (symbol->getName().substr(0, 4).compare("$ld$") == 0)
        continue;
      addSymbol(symbol->getName(), symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCClass:
      if (useObjC1ABI) {
        addSymbol(".objc_class_name_" + symbol->getName(),
                  symbol->getFlags());
      } else {
        addSymbol("_OBJC_CLASS_$_" + symbol->getName(),
                  symbol->getFlags());
        addSymbol("_OBJC_METACLASS_$_" + symbol->getName(),
                  symbol->getFlags());
      }
      break;
    case SymbolKind::ObjectiveCClassEHType:
      addSymbol("_OBJC_EHTYPE_$_" + symbol->getName(),
                symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCInstanceVariable:
      addSymbol("_OBJC_IVAR_$_" + symbol->getName(), symbol->getFlags());
      break;
    }

    if (symbol->isWeakDefined())
      _hasWeakDefExports = true;
  }

  for (const auto *symbol : data->_undefineds) {
    switch (symbol->getKind()) {
    case SymbolKind::GlobalSymbol:
      _undefineds.emplace_back(symbol->getName(), symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCClass:
      if (useObjC1ABI) {
        _undefineds.emplace_back(".objc_class_name_" + symbol->getName(),
                                 symbol->getFlags());
      } else {
        _undefineds.emplace_back("_OBJC_CLASS_$_" + symbol->getName(),
                                 symbol->getFlags());
        _undefineds.emplace_back("_OBJC_METACLASS_$_" + symbol->getName(),
                                 symbol->getFlags());
      }
      break;
    case SymbolKind::ObjectiveCClassEHType:
      _undefineds.emplace_back("_OBJC_EHTYPE_$_" + symbol->getName(),
                               symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCInstanceVariable:
      _undefineds.emplace_back("_OBJC_IVAR_$_" + symbol->getName(),
                               symbol->getFlags());
      break;
    }
  }

  for (auto *file = data->next; file != nullptr; file = file->next)
    _inlinedFrameworkNames.emplace_back(file->_installName);

  return true;
}

LinkerInterfaceFile *LinkerInterfaceFile::create(
    const std::string &path, const uint8_t *data, size_t size,
    cpu_type_t cpuType, cpu_subtype_t cpuSubType,
    CpuSubTypeMatching matchingMode, PackedVersion32 minOSVersion,
    std::string &errorMessage) noexcept {

  ParsingFlags flags = (matchingMode == CpuSubTypeMatching::Exact)
                           ? ParsingFlags::ExactCpuSubType
                           : ParsingFlags::None;

  return create(path, data, size, cpuType, cpuSubType, flags, minOSVersion,
                errorMessage);
}

LinkerInterfaceFile *LinkerInterfaceFile::create(
    const std::string &path, const uint8_t *data, size_t size,
    cpu_type_t cpuType, cpu_subtype_t cpuSubType, ParsingFlags flags,
    PackedVersion32 minOSVersion, std::string &errorMessage) noexcept {

  if (path.empty() || data == nullptr || size < 8) {
    errorMessage = "invalid argument";
    return nullptr;
  }

  auto impldata = LinkerInterfaceFile::ImplData::loadFile(
      path, data, size, cpuType, cpuSubType, errorMessage);
  if (impldata == nullptr)
    return nullptr;

  auto file = new LinkerInterfaceFile;
  if (file == nullptr) {
    errorMessage = "could not allocate memory";
    return nullptr;
  }

  if (file->_pImpl->init(
        std::shared_ptr<LinkerInterfaceFile::ImplData>(impldata),
        cpuType, cpuSubType, flags, minOSVersion, errorMessage)) {
    return file;
  }

  delete file;
  return nullptr;
}

LinkerInterfaceFile *
LinkerInterfaceFile::create(const std::string &path, cpu_type_t cpuType,
                            cpu_subtype_t cpuSubType, ParsingFlags flags,
                            PackedVersion32 minOSVersion,
                            std::string &errorMessage) noexcept {
  std::ifstream ifs;
  ifs.open(path, std::ifstream::in);
  auto buf = std::string(std::istreambuf_iterator<char>{ifs}, {});
  auto data = LinkerInterfaceFile::ImplData::loadFile(
      path, (const uint8_t *)buf.c_str(), buf.length(),
      cpuType, cpuSubType, errorMessage);
  if (data == nullptr)
    return nullptr;

  auto file = new LinkerInterfaceFile;
  if (file == nullptr) {
    errorMessage = "could not allocate memory";
    return nullptr;
  }

  if (file->_pImpl->init(
			  std::shared_ptr<LinkerInterfaceFile::ImplData>(data),
			  cpuType, cpuSubType, flags, minOSVersion, errorMessage)) {
    return file;
  }

  delete file;
  return nullptr;
}

FileType LinkerInterfaceFile::getFileType() const noexcept {
  return _pImpl->_data->_fileType;
}

Platform LinkerInterfaceFile::getPlatform() const noexcept {
  return _pImpl->_data->_platform;
}

const std::string &LinkerInterfaceFile::getInstallName() const noexcept {
  return _pImpl->_installName;
}

bool LinkerInterfaceFile::isInstallNameVersionSpecific() const noexcept {
  return _pImpl->_installPathOverride;
}

PackedVersion32 LinkerInterfaceFile::getCurrentVersion() const noexcept {
  return _pImpl->_data->_currentVersion;
}

PackedVersion32 LinkerInterfaceFile::getCompatibilityVersion() const noexcept {
  return _pImpl->_compatibilityVersion;
}

unsigned LinkerInterfaceFile::getSwiftVersion() const noexcept {
  return _pImpl->_data->_swiftABIVersion;
}

ObjCConstraint LinkerInterfaceFile::getObjCConstraint() const noexcept {
  return _pImpl->_data->_objcConstraint;
}

bool LinkerInterfaceFile::hasTwoLevelNamespace() const noexcept {
  return _pImpl->_hasTwoLevelNamespace;
}

bool LinkerInterfaceFile::isApplicationExtensionSafe() const noexcept {
  return _pImpl->_isAppExtensionSafe;
}

bool LinkerInterfaceFile::hasAllowableClients() const noexcept {
  return !_pImpl->_data->_allowableClients.empty();
}

bool LinkerInterfaceFile::hasReexportedLibraries() const noexcept {
  return !_pImpl->_data->_reexportedLibraries.empty();
}

bool LinkerInterfaceFile::hasWeakDefinedExports() const noexcept {
  return _pImpl->_hasWeakDefExports;
}

const std::string &LinkerInterfaceFile::getParentFrameworkName() const
    noexcept {
  return _pImpl->_parentFrameworkName;
}

const std::vector<std::string> &LinkerInterfaceFile::allowableClients() const
    noexcept {
  return _pImpl->_data->_allowableClients;
}

const std::vector<std::string> &LinkerInterfaceFile::reexportedLibraries() const
    noexcept {
  return _pImpl->_data->_reexportedLibraries;
}

const std::vector<std::string> &LinkerInterfaceFile::ignoreExports() const
    noexcept {
  return _pImpl->_ignoreExports;
}

const std::vector<Symbol> &LinkerInterfaceFile::exports() const noexcept {
  return _pImpl->_exports;
}

const std::vector<Symbol> &LinkerInterfaceFile::undefineds() const noexcept {
  return _pImpl->_undefineds;
}

const std::vector<std::string> &
LinkerInterfaceFile::inlinedFrameworkNames() const noexcept {
  return _pImpl->_inlinedFrameworkNames;
}

LinkerInterfaceFile *LinkerInterfaceFile::getInlinedFramework(
    const std::string &installName, cpu_type_t cpuType,
    cpu_subtype_t cpuSubType, ParsingFlags flags, PackedVersion32 minOSVersion,
    std::string &errorMessage) const noexcept {

  for (auto *it = _pImpl->_data->next; it != nullptr; it = it->next) {
    if (it->_installName.compare(installName) != 0)
      continue;

    auto file = new LinkerInterfaceFile;
    if (file == nullptr) {
      errorMessage = "could not allocate memory";
      return nullptr;
    }

    if (file->_pImpl->init(
          std::shared_ptr<LinkerInterfaceFile::ImplData>(it),
          cpuType, cpuSubType, flags, minOSVersion, errorMessage)) {
      return file;
    }
  }

  errorMessage = "no such inlined framework";
  return nullptr;
}

TAPI_NAMESPACE_V1_END

/* vim:set ts=2 sw=2 expandtab: */
