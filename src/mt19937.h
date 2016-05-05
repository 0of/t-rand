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
  std::uint32_t m, 
  template<ValueType index> class InitState
>
struct Defn_GenState {
  template<ValueType> using Truth = std::true_type;
  
  //   for (int i = 0; i < (n - m); ++i)
  //      state[i] = state[i + m] ^ twiddle(state[i], state[i + 1]);
  template<ValueType i>
  using ForBodyExpr_1 = std::integral_constant<ValueType,
      XOR<InitState<i + m>, Twiddle<InitState<i>, InitState<i + 1>>>::value>;
  using For_1 = For<0, n - m, ForBodyExpr_1>;
  
  // for seq
  template<ValueType value> struct InRange : public std::integral_constant<bool, value >= 0 && value < n - m>{}; 
  
  // array
    template<ValueType i> struct At : public std::integral_constant<std::uint32_t, For_1::seq[i] >{};
    
    template<ValueType index> using ForSeq_1 = Case<index,
                CaseClause<ValueType, InRange, At>,
                CaseClause<ValueType, Truth, InitState>>;
    
  // for (int i = n - m; i < (n - 1); ++i)
  //   state[i] = state[i + m - n] ^ twiddle(state[i], state[i + 1]);
  template<ValueType i>
  using ForBodyExpr_2 = std::integral_constant<ValueType,
                                                XOR<typename ForSeq_1<i + m - n>::Evaluator, Twiddle<typename ForSeq_1<i>::Evaluator, InitState<i + 1>>>::value>;
  using For_2 = For<n - m, n - 1, ForBodyExpr_2>;

  template<ValueType value> struct InRange_2 : public std::integral_constant<bool, value >= n - m && value < n - 1>{};
    
  // array
  template<ValueType i> struct At_2 : public std::integral_constant<std::uint32_t, For_2::seq[i - (n - m)] >{};

  template<ValueType index> using ForSeq_2 = Case<index,
                                                  CaseClause<ValueType, InRange, At>,
                                                  CaseClause<ValueType, InRange_2, At_2>,
                                                  CaseClause<ValueType, Truth, InitState>>;
    
    
  // final result
  template<ValueType value> struct IsLast : public std::integral_constant<bool, value == n - 1>{};
  
  template<ValueType i> struct AtLast : public std::integral_constant<std::uint32_t,  XOR<typename ForSeq_2<m - 1>::Evaluator, Twiddle<typename ForSeq_2<n - 1>::Evaluator, typename ForSeq_2<0>::Evaluator> >::value >{};

  template<ValueType index> using ResultState = Case<index,
                                                    CaseClause<ValueType, InRange, At>,
                                                    CaseClause<ValueType, InRange_2, At_2>,
                                                    CaseClause<ValueType, IsLast, AtLast>>;
};

template
<
  typename ValueType, 
  std::uint32_t n, 
  std::uint32_t m, 
  std::uint32_t seed
>
struct Defn_Seed {
  template<std::uint32_t index, std::uint32_t prev>
  struct ForBody : public std::integral_constant<ValueType, 0xFFFFFFFFUL & (1812433253UL * (prev ^ (prev >> 30)) + index) >{};
  
  using States = For_Ex<1, n, ForBody, seed & 0xFFFFFFFFUL>;
  
  // states accessor
  template<ValueType i> using StatesArray = std::integral_constant<std::uint32_t, States::seq[i]>;
};

