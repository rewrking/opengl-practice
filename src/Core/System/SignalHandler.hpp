#pragma once

#include <csignal>

namespace ogl
{
namespace SignalHandler
{
void initialize();
void handler(const i32 inSignal);
}
}
