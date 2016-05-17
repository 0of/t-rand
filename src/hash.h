/*
 * t-rand
 *
 * Copyright (c) 2016 "0of" Magnus
 * Licensed under the Apache License, Version 2.0
 * https://github.com/0of/t-rand/blob/master/LICENSE
 */
#ifndef HASH_H
#define HASH_H

#include <cstddef>

// get hash code at compile-time
template<typename String, std::size_t len, std::size_t i, std::size_t hash>
struct GetHash {
  static constexpr std::size_t value = GetHash<String, len, i + 1, ((31 *  hash) + String::value[i])>::value;
};

template<typename String, std::size_t len, std::size_t hash>
struct GetHash<String, len, len, hash> {
  static constexpr std::size_t value = hash;
};

#endif // HASH_H


