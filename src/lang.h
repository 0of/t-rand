/*
 * t-rand
 *
 * Copyright (c) 2016 "0of" Magnus
 * Licensed under the Apache License, Version 2.0
 * https://github.com/0of/t-rand/blob/master/LICENSE
 */

//
// for generator
//  [index, end]
//   -> seq = [ExprBody(index++)]
// 
template
<
  std::uint32_t index,
  std::uint32_t end,
  template<std::uint32_t> class ExprBody,
  std::uint32_t... values
>
struct For {
    static constexpr auto& seq = For<index + 1, end, ExprBody, ExprBody<index>::value, values...>::seq;
};

template
<
  std::uint32_t end,
  template<std::uint32_t> class ExprBody,
  std::uint32_t... values
>
struct For<end, end, ExprBody, values...> {
    static constexpr std::uint32_t seq[] = { values... };
};

template<
  std::uint32_t end,
  template<std::uint32_t> class ExprBody,
  std::uint32_t... values
>
constexpr const std::uint32_t For<end, end, ExprBody, values...>::seq[];

//
// Case evaluator
//  value 
//    -> Clause Tester? Clause Body
//    -> ...
//    -> Not matched? value
//
template
<
  std::uint32_t value, 
  typename Clause, 
  typename... Clauses
>
struct Case {
    using Evaluator = std::conditional_t<Clause::template TestExpr<value>::value, typename Clause::template ResultExpr<value>, typename Case<value, Clauses...>::Evaluator>;
};

template
<
  std::uint32_t value, 
  typename Clause
>
struct Case<value, Clause> {
    using Evaluator = std::conditional_t<Clause::template TestExpr<value>::value, typename Clause::template ResultExpr<value>, std::integral_constant<std::uint32_t, value>>;
};

template
<
  typename ValueType, 
  template<std::uint32_t> class Test, 
  template<ValueType> class Result
>
struct CaseClause {
    template<ValueType value> using TestExpr = Test<value>;
    template<ValueType value> using ResultExpr = Result<value>;
};

//
// op XOR
//
template
<
  typename Var, 
  typename Var_
>
struct XOR {
    static constexpr auto value = Var::value xor Var_::value;
};

//
// For ex
//
template
<
  std::uint32_t index,
  std::uint32_t end,
  template<std::uint32_t, std::uint32_t pre> class ExprBody,
  std::uint32_t prev,
  std::uint32_t... values
>
struct For_Ex {
  static constexpr auto& seq = For_Ex<index + 1, end, ExprBody, ExprBody<index, prev>::value, prev, values...>::seq;
};

template
<
  std::uint32_t end,
  template<std::uint32_t, std::uint32_t pre> class ExprBody,
  std::uint32_t... values
>
struct For_Ex<end, end, ExprBody, values...> {
  static constexpr std::uint32_t seq[] = { values... };
};

template
<
  std::uint32_t end,
  template<std::uint32_t, std::uint32_t pre> class ExprBody,
  std::uint32_t... values
>
constexpr const std::uint32_t For_Ex<end, end, ExprBody, values...>::seq[];