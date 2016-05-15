/*
 * t-rand
 *
 * Copyright (c) 2016 "0of" Magnus
 * Licensed under the Apache License, Version 2.0
 * https://github.com/0of/t-rand/blob/master/LICENSE
 */

#ifndef TRAND_H
#define TRAND_H

#include "mt19937"

struct CompilerTime {
  static constexpr const char *value = __TIME__;
  static constexpr const size_t GetLengthOf(const char* str) {
    return (*str == 0) ? 0 : GetLengthOf(str + 1) + 1;
  }
};

#ifndef RAND_SEED
#define RAND_SEED GetHash<CompilerTime, CompilerTime::GetLengthOf(CompilerTime::value), 0, 0>::value;
#endif // RAND_SEED

class MersenneTwisterEngine{};

// random generator
template<typename Engine, std::size_t count>
struct RandomGenerator;

template<std::size_t count>
struct RandomGenerator<MersenneTwisterEngine, count> {
  static constexpr auto& seq = MTRandomGenerator<Defn_Seed<std::uint32_t, 624, 397, RAND_SEED>::States, count, 624>::seq;
};

#endif // TRAND_H

