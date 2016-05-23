# t-rand
compile-time PRNG
generate random value at complie-time

# Support Algorithm
- MT19937

# Usage Guidelines

## CXXFLAGS
```
  // at least 1400 template recursion depth
  -ftemplate-depth=1400
```

## Example
```cpp
  // generate 5 random numbers
  constexpr const int generateCount = 5;
  // array of generated numbers with fix size 5
  auto& values = RandomGenerator<MersenneTwisterEngine, generateCount>::seq;
  
  template <std::uint32_t value>
  struct If {
    static constexpr bool result = value > 1455537896;
  };
  
  // maybe true or false
  If<values[0]>::result;
```

# License
Apache License Version 2.0
