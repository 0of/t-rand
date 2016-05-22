/*
 * t-rand
 *
 * Copyright (c) 2016 "0of" Magnus
 * Licensed under the Apache License, Version 2.0
 * https://github.com/0of/t-rand/blob/master/LICENSE
 */

#include "rand.h"

namespace MT_Impl {
  static const int n = 624, m = 397;
  static unsigned long state[n] = {0x0UL};
  static int p = 0;

  unsigned long twiddle(unsigned long u, unsigned long v) {
    return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)
      ^ ((v & 1UL) * 0x9908B0DFUL);
  }

  void gen() {
    for (int i = 0; i < (n - m); ++i)
      state[i] = state[i + m] ^ twiddle(state[i], state[i + 1]);
    for (int i = n - m; i < (n - 1); ++i)
      state[i] = state[i + m - n] ^ twiddle(state[i], state[i + 1]);
    state[n - 1] = state[m - 1] ^ twiddle(state[n - 1], state[0]);
    p = 0;
  }

  void seed(unsigned long s) {
    state[0] = s & 0xFFFFFFFFUL;
    for (int i = 1; i < n; ++i) {
      state[i] = 1812433253UL * (state[i - 1] ^ (state[i - 1] >> 30)) + i;
      state[i] &= 0xFFFFFFFFUL;
    }
    p = n;
  }

  unsigned long rand() {
    if (p == n) gen(); 

    unsigned long x = state[p++];
    x ^= (x >> 11);
    x ^= (x << 7) & 0x9D2C5680UL;
    x ^= (x << 15) & 0xEFC60000UL;
    return x ^ (x >> 18);
  }
}

#include <array>

#include "TestSuite.h"

using TestSpec = LTest::SequentialTestSpec;
using DefaultContainer = LTest::SequentialTestRunnableContainer;

// template function
#define SPEC_TFN(fn) fn<TestSpec>

#define TEST_ENTRY(ContainerType, ...) \
  int main() { \
    auto container = std::make_unique<ContainerType>(); \
    auto spec = std::make_shared<TestSpec>(); \
    \
    SpecInitializer<TestSpec> specInitializer{ *spec }; \
    specInitializer.appendCases(__VA_ARGS__); \
    \
    container->scheduleToRun(spec); \
    container->start(); \
    \
    std::this_thread::sleep_for(5s); \
    \
    return 0; \
  }

template<typename Spec>
class SpecInitializer {
private:
    std::add_lvalue_reference_t<Spec> _spec;
    
public:
    explicit SpecInitializer(std::add_lvalue_reference_t<Spec> spec)
    : _spec{ spec }
    {}
    
public:
    template<typename Functor>
    void appendCases(Functor&& fn) {
        fn(_spec);
    }
    
    template<typename Any, typename... Rest>
    void appendCases(Any&& any, Rest&&... rest) {
        appendCases(std::forward<Any>(any));
        appendCases(std::forward<Rest>(rest)...);
    }
};

template<typename T>
void setup(T& spec) {

  spec
  /* ==> */
  .it("should generate same random values", [] {
    constexpr const int count = 5;
      
    std::array<std::uint32_t, count> runtimeGenValues;

    MT_Impl::seed(RAND_SEED);
    for (auto i = 0; i != count; ++i) {
      runtimeGenValues[i] = MT_Impl::rand();
    }

    auto& values = RandomGenerator<MersenneTwisterEngine, 5>::seq;
      
    if (!std::equal(runtimeGenValues.begin(), runtimeGenValues.end(), std::begin(values)))
      throw 0;
  });
}

TEST_ENTRY(DefaultContainer,
  SPEC_TFN(setup));

 




