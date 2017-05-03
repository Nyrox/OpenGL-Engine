#pragma once
#include <stdint.h>
#include <memory>


using uint64	= uint64_t;
using uint32	= uint32_t;
using uint16	= uint16_t;
using uint8		= uint8_t;


template<class T>
using uptr = std::unique_ptr<T>;






