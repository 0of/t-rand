/*
 * t-rand
 *
 * Copyright (c) 2016 "0of" Magnus
 * Licensed under the Apache License, Version 2.0
 * https://github.com/0of/t-rand/blob/master/LICENSE
 */
#ifndef HASH_H
#define HASH_H

#include <cstdint>

// get hash code at compile-time
template<typename String, std::uint32_t len, std::uint32_t i, std::uint32_t hash>
struct GetHash {
  static constexpr std::uint32_t value = GetHash<String, len, i + 1, ((31 *  hash) + String::value[i])>::value;
};

template<typename String, std::uint32_t len, std::uint32_t hash>
struct GetHash<String, len, len, hash> {
  static constexpr std::uint32_t value = hash;
};

#endif // HASH_H
