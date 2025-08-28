#pragma once

#define DISABLE_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&) = delete;    \
  ClassName& operator=(const ClassName&) = delete
