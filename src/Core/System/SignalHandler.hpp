#pragma once

#include <csignal>

namespace ogl
{
namespace SignalHandler
{
extern bool signaled;
void initialize(Function<void> inOnError = nullptr);
void handler(const i32 inSignal);
}
}
