#pragma once

#define UNUSED(x) static_cast<void>(x)

#define GL_DISALLOW_COPY_MOVE(ClassName)             \
	ClassName(const ClassName&) = delete;            \
	ClassName(ClassName&&) noexcept = delete;        \
	ClassName& operator=(const ClassName&) = delete; \
	ClassName& operator=(ClassName&&) noexcept = delete

#define GL_DISALLOW_COPY(ClassName)       \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete

#define GL_DISALLOW_MOVE(ClassName)           \
	ClassName(ClassName&&) noexcept = delete; \
	ClassName& operator=(ClassName&&) noexcept = delete

#define GL_DEFAULT_COPY_MOVE(ClassName)               \
	ClassName(const ClassName&) = default;            \
	ClassName(ClassName&&) noexcept = default;        \
	ClassName& operator=(const ClassName&) = default; \
	ClassName& operator=(ClassName&&) noexcept = default

#define GL_DEFAULT_COPY(ClassName)         \
	ClassName(const ClassName&) = default; \
	ClassName& operator=(const ClassName&) = default

#define GL_DEFAULT_MOVE(ClassName)             \
	ClassName(ClassName&&) noexcept = default; \
	ClassName& operator=(ClassName&&) noexcept = default
