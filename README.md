# t-rand
compile-time PRNG
generate random value at complie-time

# Support Algorithm
- MT19937

# Usage guides

```cpp
  // generate 5 random numbers
  constexpr const int generateCount = 5;
  // array of generated numbers with fix size 5
  auto& values = RandomGenerator<MersenneTwisterEngine, generateCount>::seq;
```

# License
Apache License Version 2.0
