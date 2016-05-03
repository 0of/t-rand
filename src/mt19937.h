/*
 * t-rand
 *
 * Copyright (c) 2016 "0of" Magnus
 * Licensed under the Apache License, Version 2.0
 * https://github.com/0of/t-rand/blob/master/LICENSE
 */

#include <type_traits>

#include "lang.h"

template 
<
  typename Var, 
  typename Var_
>
struct Twiddle {
    static constexpr auto value = (((Var::value & 0x80000000UL) | (Var_::value & 0x7FFFFFFFUL)) >> 1)
    ^ ((Var_::value & 1UL) * 0x9908B0DFUL);
};


template
<
  typename ValueType, 
  std::uint32_t n, 
  std::uint32_t m
>
struct Defn_GenState {
  template<ValueType> using Truth = std::true_type;
  template<ValueType> using Zero = std::integral_constant<ValueType, 0>;
  
  template<ValueType index> using InitState = typename Case<index, CaseClause<ValueType, Truth, Zero>>::Evaluator;
  

  //   for (int i = 0; i < (n - m); ++i)
  //      state[i] = state[i + m] ^ twiddle(state[i], state[i + 1]);
  template<ValueType i>
  using ForBodyExpr_1 = std::integral_constant<ValueType,
      XOR<InitState<i + m>, Twiddle<InitState<i>, InitState<i + 1>>>::value>;
  using For_1 = For<0, n - m, ForBodyExpr_1>;
  
  // for seq
  template<ValueType value> struct InRange : public std::integral_constant<bool, value >= 0 && value <= n - m>{};
  
  // array
  template<ValueType i> struct At : public std::integral_constant<decltype(For_1::seq[0]), For_1::seq[i] >{};

  // TODO
};
