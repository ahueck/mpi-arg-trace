// Modified from:
// TypeART library
//
// Copyright (c) 2017-2022 TypeART Authors
// Distributed under the BSD 3-Clause license.
// (See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/BSD-3-Clause)
//
// Project home: https://github.com/tudasc/TypeART
//
// SPDX-License-Identifier: BSD-3-Clause
//

#include "System.h"

#include <algorithm>
#include <cstdio>
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>
#include <sys/resource.h>

namespace mpitracer {

namespace system {
Process::Process() {
  self_exe = std::filesystem::canonical("/proc/self/exe");
}

const std::string& Process::exe() const {
  return self_exe;
}

const Process& Process::get() {
  static Process self;
  return self;
}

long Process::getMaxRSS() {
  rusage proc;
  getrusage(RUSAGE_SELF, &proc);

  return proc.ru_maxrss;
}

class CommandPipe {
  using UniqueFile = std::unique_ptr<FILE, int (*)(FILE*)>;
  UniqueFile command;

  explicit CommandPipe(UniqueFile command);

 public:
  static std::optional<CommandPipe> create(std::string_view command);

  [[nodiscard]] std::string nextLine() const;
};

CommandPipe::CommandPipe(CommandPipe::UniqueFile command) : command(std::move(command)) {
}

std::optional<CommandPipe> CommandPipe::create(std::string_view command) {
  UniqueFile comm(popen(command.data(), "r"), &pclose);

  if (!comm) {
    return {};
  }

  return CommandPipe{std::move(comm)};
}

std::string CommandPipe::nextLine() const {
  char* buffer{nullptr};
  size_t buffer_length{0};
  std::string result;

  if (getline(&buffer, &buffer_length, command.get()) != -1) {
    result = buffer;
  }

  if (buffer != nullptr) {
    free(buffer);
  }

  if (!result.empty()) {
    result.resize(result.size() - 1);  // remove \n
  }

  return result;
}

bool test_command(std::string_view command, std::string_view test_arg = "-h");

bool test_command(std::string_view command, std::string_view test_arg) {
  const auto available = [](const std::string_view command) -> bool {
    constexpr int command_not_found{127};
    auto* proc        = popen(command.data(), "r");
    const int ret_val = pclose(proc);
    return WEXITSTATUS(ret_val) != command_not_found;
  };

  std::ostringstream os;
  os << command << " " << test_arg;

  const bool avail = available(os.str());

  return avail;
}

class SourceLocHelper {
  bool has_addr2line{false};
  bool has_llvmsymbolizer{false};

  SourceLocHelper() {
    has_addr2line      = test_command("addr2line");
    has_llvmsymbolizer = test_command("llvm-symbolizer");
  }

 public:
  static const SourceLocHelper& get() {
    static SourceLocHelper helper;
    return helper;
  }

  [[nodiscard]] bool hasAddr2line() const {
    return has_addr2line;
  }

  [[nodiscard]] bool hasLLVMSymbolizer() const {
    return has_llvmsymbolizer;
  }
};

struct RemoveEnvInScope {
  explicit RemoveEnvInScope(std::string_view var_name) : var_name_(var_name) {
    old_val_ = getenv(var_name_.data());
    if (!old_val_.empty()) {
      setenv(var_name_.data(), "", true);
    }
  }

  ~RemoveEnvInScope() {
    if (!old_val_.empty()) {
      setenv(var_name_.data(), old_val_.data(), true);
    }
  }

 private:
  std::string_view var_name_;
  std::string_view old_val_;
};

}  // namespace system

std::optional<SourceLocation> SourceLocation::create(const void* addr, intptr_t offset_ptr) {
  // Required, otherwise endless recursion:
  system::RemoveEnvInScope rm_preload_var{"LD_PRELOAD"};

  const auto pipe = [](const void* paddr, intptr_t offset_ptr) -> std::optional<system::CommandPipe> {
    const auto& sloc_helper = system::SourceLocHelper::get();
    const auto& proc        = system::Process::get();

    // FIXME: Inst Pointer points one past what we need with __built_in_return_addr(0), hacky way to fix:
    const intptr_t addr = reinterpret_cast<intptr_t>(paddr) - offset_ptr;

    if (sloc_helper.hasLLVMSymbolizer()) {
      std::ostringstream command;
      command << "llvm-symbolizer --demangle --output-style=GNU -f -e " << proc.exe() << " " << addr;
      auto llvm_symbolizer = system::CommandPipe::create(command.str());
      if (llvm_symbolizer) {
        return llvm_symbolizer;
      }
    }

    if (sloc_helper.hasAddr2line()) {
      std::ostringstream command;
      command << "addr2line --demangle=auto -f -e " << proc.exe() << " " << addr;
      auto addr2line = system::CommandPipe::create(command.str());
      if (addr2line) {
        return addr2line;
      }
    }

    return {};
  }(addr, offset_ptr);

  if (!pipe) {
    return {};
  }

  SourceLocation loc;

  loc.function             = pipe->nextLine();
  const auto file_and_line = pipe->nextLine();
  const auto delimiter     = file_and_line.find(':');
  loc.line                 = file_and_line.substr(delimiter + 1);
  loc.file                 = file_and_line.substr(0, delimiter);

  return loc;
}

}  // namespace mpitracer