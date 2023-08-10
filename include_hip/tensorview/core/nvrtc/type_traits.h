// Copyright (C) 2008-2021 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#pragma once

#include "core.h"
#include <tensorview/core/defs.h>

#ifdef __HIPCC_RTC__

namespace std {

// ========== Initializer list ============
template<class E> class initializer_list {
  public:
    using value_type      = E;
    using reference       = const E&;
    using const_reference = const E&;
    using size_type       = size_t;
 
    using iterator        = const E*;
    using const_iterator  = const E*;
 
    constexpr initializer_list() noexcept;
 
    constexpr size_t size() const noexcept;     // number of elements
    constexpr const E* begin() const noexcept;  // first element
    constexpr const E* end() const noexcept;    // one past the last element
  };
 
  // initializer list range access
  template<class E> constexpr const E* begin(initializer_list<E> il) noexcept;
  template<class E> constexpr const E* end(initializer_list<E> il) noexcept;

// =========== End Initializer list ========


// ================== make_index_sequence ============
//// START OF CODE FROM GOOGLE ABSEIL

// integer_sequence
//
// Class template representing a compile-time integer sequence. An instantiation
// of `integer_sequence<T, Ints...>` has a sequence of integers encoded in its
// type through its template arguments (which is a common need when
// working with C++11 variadic templates). `absl::integer_sequence` is designed
// to be a drop-in replacement for C++14's `std::integer_sequence`.
//
// Example:
//
//   template< class T, T... Ints >
//   void user_function(integer_sequence<T, Ints...>);
//
//   int main()
//   {
//     // user_function's `T` will be deduced to `int` and `Ints...`
//     // will be deduced to `0, 1, 2, 3, 4`.
//     user_function(make_integer_sequence<int, 5>());
//   }
template <typename T, T... Ints>
struct integer_sequence
{
    using value_type = T;
    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Ints);
    }
};

// index_sequence
//
// A helper template for an `integer_sequence` of `size_t`,
// `absl::index_sequence` is designed to be a drop-in replacement for C++14's
// `std::index_sequence`.
template <size_t... Ints>
using index_sequence = integer_sequence<size_t, Ints...>;

namespace utility_internal
{

template <typename Seq, size_t SeqSize, size_t Rem>
struct Extend;

// Note that SeqSize == sizeof...(Ints). It's passed explicitly for efficiency.
template <typename T, T... Ints, size_t SeqSize>
struct Extend<integer_sequence<T, Ints...>, SeqSize, 0>
{
    using type = integer_sequence < T, Ints..., (Ints + SeqSize)... >;
};

template <typename T, T... Ints, size_t SeqSize>
struct Extend<integer_sequence<T, Ints...>, SeqSize, 1>
{
    using type = integer_sequence < T, Ints..., (Ints + SeqSize)..., 2 * SeqSize >;
};

// Recursion helper for 'make_integer_sequence<T, N>'.
// 'Gen<T, N>::type' is an alias for 'integer_sequence<T, 0, 1, ... N-1>'.
template <typename T, size_t N>
struct Gen
{
    using type =
        typename Extend < typename Gen < T, N / 2 >::type, N / 2, N % 2 >::type;
};

template <typename T>
struct Gen<T, 0>
{
    using type = integer_sequence<T>;
};

}  // namespace utility_internal

// Compile-time sequences of integers

// make_integer_sequence
//
// This template alias is equivalent to
// `integer_sequence<int, 0, 1, ..., N-1>`, and is designed to be a drop-in
// replacement for C++14's `std::make_integer_sequence`.
template <typename T, T N>
using make_integer_sequence = typename utility_internal::Gen<T, N>::type;

// make_index_sequence
//
// This template alias is equivalent to `index_sequence<0, 1, ..., N-1>`,
// and is designed to be a drop-in replacement for C++14's
// `std::make_index_sequence`.
template <size_t N>
using make_index_sequence = make_integer_sequence<size_t, N>;

// index_sequence_for
//
// Converts a typename pack into an index sequence of the same length, and
// is designed to be a drop-in replacement for C++14's
// `std::index_sequence_for()`
template <typename... Ts>
using index_sequence_for = make_index_sequence<sizeof...(Ts)>;

//// END OF CODE FROM GOOGLE ABSEIL
// ======= end make_index_sequence ==================

// template <typename _Tp, _Tp __v> struct integral_constant {
//   static constexpr _Tp value = __v;
//   typedef _Tp value_type;
//   typedef integral_constant<_Tp, __v> type;
//   TV_HOST_DEVICE_INLINE constexpr operator value_type() const noexcept {
//     return value;
//   }
//   TV_HOST_DEVICE_INLINE constexpr value_type operator()() const noexcept {
//     return value;
//   }
// };
/// The type used as a compile-time boolean with true value.
// typedef integral_constant<bool, true> true_type;

/// The type used as a compile-time boolean with false value.
// typedef integral_constant<bool, false> false_type;

template <bool _Cond, typename _Iftrue, typename _Iffalse> struct conditional {
  typedef _Iftrue type;
};

// Partial specialization for false.
template <typename _Iftrue, typename _Iffalse>
struct conditional<false, _Iftrue, _Iffalse> {
  typedef _Iffalse type;
};

template <bool, typename, typename> struct conditional;

template <typename...> struct __or_;

template <> struct __or_<> : public false_type {};

template <typename _B1> struct __or_<_B1> : public _B1 {};

template <typename _B1, typename _B2>
struct __or_<_B1, _B2> : public conditional<_B1::value, _B1, _B2>::type {};

template <typename _B1, typename _B2, typename _B3, typename... _Bn>
struct __or_<_B1, _B2, _B3, _Bn...>
    : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type {};

template <typename...> struct __and_;

template <> struct __and_<> : public true_type {};

template <typename _B1> struct __and_<_B1> : public _B1 {};

template <typename _B1, typename _B2>
struct __and_<_B1, _B2> : public conditional<_B1::value, _B2, _B1>::type {};

template <typename _B1, typename _B2, typename _B3, typename... _Bn>
struct __and_<_B1, _B2, _B3, _Bn...>
    : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type {};

template <typename _Tp> struct remove_const { typedef _Tp type; };

template <typename _Tp> struct remove_const<_Tp const> { typedef _Tp type; };

template <typename _Tp> struct remove_volatile { typedef _Tp type; };

template <typename _Tp> struct remove_volatile<_Tp volatile> {
  typedef _Tp type;
};

template <typename _Tp> struct remove_cv {
  typedef typename remove_const<typename remove_volatile<_Tp>::type>::type type;
};

template <typename _Tp> struct add_const { typedef _Tp const type; };

/// add_volatile
template <typename _Tp> struct add_volatile { typedef _Tp volatile type; };

/// add_cv
template <typename _Tp> struct add_cv {
  typedef typename add_const<typename add_volatile<_Tp>::type>::type type;
};

template <typename _Tp> using remove_const_t = typename remove_const<_Tp>::type;

/// Alias template for remove_volatile
template <typename _Tp>
using remove_volatile_t = typename remove_volatile<_Tp>::type;

/// Alias template for remove_cv
template <typename _Tp> using remove_cv_t = typename remove_cv<_Tp>::type;

/// Alias template for add_const
template <typename _Tp> using add_const_t = typename add_const<_Tp>::type;

/// Alias template for add_volatile
template <typename _Tp> using add_volatile_t = typename add_volatile<_Tp>::type;

/// Alias template for add_cv
template <typename _Tp> using add_cv_t = typename add_cv<_Tp>::type;
/// is_const
template <typename> struct is_const : public false_type {};

template <typename _Tp> struct is_const<_Tp const> : public true_type {};

/// is_volatile
template <typename> struct is_volatile : public false_type {};

template <typename _Tp> struct is_volatile<_Tp volatile> : public true_type {};

template <typename _Tp, typename> struct __remove_pointer_helper {
  typedef _Tp type;
};

template <typename _Tp, typename _Up>
struct __remove_pointer_helper<_Tp, _Up *> {
  typedef _Up type;
};

/// remove_pointer
template <typename _Tp>
struct remove_pointer
    : public __remove_pointer_helper<_Tp, typename remove_cv<_Tp>::type> {};

template <typename _Tp>
using remove_pointer_t = typename remove_pointer<_Tp>::type;

template <typename _Tp> struct remove_reference { typedef _Tp type; };

template <typename _Tp> struct remove_reference<_Tp &> { typedef _Tp type; };

template <typename _Tp> struct remove_reference<_Tp &&> { typedef _Tp type; };

/// remove_extent
template <typename _Tp> struct remove_extent { typedef _Tp type; };

template <typename _Tp, std::size_t _Size> struct remove_extent<_Tp[_Size]> {
  typedef _Tp type;
};

template <typename _Tp> struct remove_extent<_Tp[]> { typedef _Tp type; };

/// is_array
template <typename> struct is_array : public false_type {};

template <typename _Tp, std::size_t _Size>
struct is_array<_Tp[_Size]> : public true_type {};

template <typename _Tp> struct is_array<_Tp[]> : public true_type {};

template <typename> struct __is_pointer_helper : public false_type {};

template <typename _Tp> struct __is_pointer_helper<_Tp *> : public true_type {};

/// is_pointer
template <typename _Tp>
struct is_pointer
    : public __is_pointer_helper<typename remove_cv<_Tp>::type>::type {};
template <typename> struct is_function;

template <typename> struct is_function : public false_type {};

#define _GLIBCXX_NOEXCEPT_PARM
#define _GLIBCXX_NOEXCEPT_QUAL

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) const _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) const & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) const && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) const _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) const & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) const && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) volatile _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) volatile & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) volatile && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) volatile _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) volatile & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) volatile && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) const volatile _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) const volatile & _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes...) const volatile && _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) const volatile _GLIBCXX_NOEXCEPT_QUAL>
    : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) const volatile &
                   _GLIBCXX_NOEXCEPT_QUAL> : public true_type {};

template <typename _Res, typename... _ArgTypes _GLIBCXX_NOEXCEPT_PARM>
struct is_function<_Res(_ArgTypes......) const volatile &&
                   _GLIBCXX_NOEXCEPT_QUAL> : public true_type {};

template <typename> struct is_lvalue_reference : public false_type {};

template <typename _Tp> struct is_lvalue_reference<_Tp &> : public true_type {};

template <typename _Up, bool _IsArray = is_array<_Up>::value,
          bool _IsFunction = is_function<_Up>::value>
struct __decay_selector;

// NB: DR 705.
template <typename _Up> struct __decay_selector<_Up, false, false> {
  typedef typename remove_cv<_Up>::type __type;
};

template <typename _Up> struct __decay_selector<_Up, true, false> {
  typedef typename remove_extent<_Up>::type *__type;
};

// add support for add_point need lots of code.
// template<typename _Up>
//   struct __decay_selector<_Up, false, true>
//   { typedef typename add_pointer<_Up>::type __type; };

/// decay
template <typename _Tp> class decay {
  typedef typename remove_reference<_Tp>::type __remove_type;

public:
  typedef typename __decay_selector<__remove_type>::__type type;
};
template <typename _Tp> using decay_t = typename decay<_Tp>::type;

// nvrtc seems contain a built-in std::forward...
template <typename _Tp>
constexpr _Tp &&
forward(typename std::remove_reference<_Tp>::type &__t) noexcept {
  return static_cast<_Tp &&>(__t);
}

// /**
//  *  @brief  Forward an rvalue.
//  *  @return The parameter cast to the specified type.
//  *
//  *  This function is used to implement "perfect forwarding".
//  */
// template <typename _Tp>
// constexpr _Tp &&
// forward(typename std::remove_reference<_Tp>::type &&__t) noexcept {
//   static_assert(!std::is_lvalue_reference<_Tp>::value,
//                 "template argument"
//                 " substituting _Tp is an lvalue reference type");
//   return static_cast<_Tp &&>(__t);
// }

// template <typename, typename> struct is_same : public false_type {};

// template <typename _Tp> struct is_same<_Tp, _Tp> : public true_type {};

// Primary template.
/// Define a member typedef @c type only if a boolean constant is true.
// template <bool, typename _Tp = void> struct enable_if {};

// Partial specialization for true.
// template <typename _Tp> struct enable_if<true, _Tp> { typedef _Tp type; };

template <typename... _Cond>
using _Require = typename enable_if<__and_<_Cond...>::value>::type;

template <bool _Cond, typename _Tp = void>
using enable_if_t = typename enable_if<_Cond, _Tp>::type;

template <bool _Cond, typename _Iftrue, typename _Iffalse>
using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type;

template <typename _Tp, typename _Up = _Tp &&>
_Up TV_HOST_DEVICE_INLINE __declval(int);

template <typename _Tp> _Tp TV_HOST_DEVICE_INLINE __declval(long);

template <typename _Tp>
TV_HOST_DEVICE_INLINE auto declval() noexcept -> decltype(__declval<_Tp>(0));

template <typename _Tp> class reference_wrapper;

// Helper which adds a reference to a type when given a reference_wrapper
template <typename _Tp> struct __strip_reference_wrapper {
  typedef _Tp __type;
};

template <typename _Tp>
struct __strip_reference_wrapper<reference_wrapper<_Tp>> {
  typedef _Tp &__type;
};

template <typename _Tp> struct __decay_and_strip {
  typedef typename __strip_reference_wrapper<typename decay<_Tp>::type>::__type
      __type;
};

template <typename _Tp> struct tuple_size;

template <typename _Tp, typename _Up = typename remove_cv<_Tp>::type,
          typename = typename enable_if<is_same<_Tp, _Up>::value>::type,
          size_t = tuple_size<_Tp>::value>
using __enable_if_has_tuple_size = _Tp;

template <typename _Tp>
struct tuple_size<const __enable_if_has_tuple_size<_Tp>>
    : public tuple_size<_Tp> {};

template <typename _Tp>
struct tuple_size<volatile __enable_if_has_tuple_size<_Tp>>
    : public tuple_size<_Tp> {};

template <typename _Tp>
struct tuple_size<const volatile __enable_if_has_tuple_size<_Tp>>
    : public tuple_size<_Tp> {};

template <std::size_t __i, typename _Tp> struct tuple_element;

// Duplicate of C++14's tuple_element_t for internal use in C++11 mode
template <std::size_t __i, typename _Tp>
using __tuple_element_t = typename tuple_element<__i, _Tp>::type;

template <std::size_t __i, typename _Tp> struct tuple_element<__i, const _Tp> {
  typedef typename add_const<__tuple_element_t<__i, _Tp>>::type type;
};

template <std::size_t __i, typename _Tp>
struct tuple_element<__i, volatile _Tp> {
  typedef typename add_volatile<__tuple_element_t<__i, _Tp>>::type type;
};

template <std::size_t __i, typename _Tp>
struct tuple_element<__i, const volatile _Tp> {
  typedef typename add_cv<__tuple_element_t<__i, _Tp>>::type type;
};

template <std::size_t __i, typename _Tp>
using tuple_element_t = typename tuple_element<__i, _Tp>::type;

} // namespace std
#endif