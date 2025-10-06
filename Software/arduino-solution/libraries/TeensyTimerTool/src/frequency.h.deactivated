// <chrono> -*- C++ -*-

// Copyright (C) 2008-2015 Free Software Foundation, Inc.
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

#include <chrono>
#include <limits>
#include <ratio>
#include <type_traits>

using std::common_type;
using std::enable_if;
using std::is_convertible;
using std::ratio;
using std::ratio_divide;

namespace TeensyTimerTool
{
    template <typename _Rep, typename _Period = ratio<1>>
    struct frequency;

    template <typename _CT, typename _Period1, typename _Period2>
    struct __frequency_common_type_wrapper
    {
     private:
        typedef std::__static_gcd<_Period1::num, _Period2::num> __gcd_num;
        typedef std::__static_gcd<_Period1::den, _Period2::den> __gcd_den;
        typedef typename _CT::type __cr;
        typedef ratio<__gcd_num::value,
                      (_Period1::den / __gcd_den::value) * _Period2::den>
            __r;

     public:
        typedef std::__success_type<TeensyTimerTool::frequency<__cr, __r>> type;
    };

    template <typename _Period1, typename _Period2>
    struct __frequency_common_type_wrapper<std::__failure_type, _Period1, _Period2>
    {
        typedef std::__failure_type type;
    };
}

namespace std
{
    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    struct common_type<TeensyTimerTool::frequency<_Rep1, _Period1>, TeensyTimerTool::frequency<_Rep2, _Period2>>
        : public TeensyTimerTool::__frequency_common_type_wrapper<typename __member_type_wrapper<std::common_type<_Rep1, _Rep2>>::type, _Period1, _Period2>::type
    {
    };
}

namespace TeensyTimerTool
{
    using namespace std::chrono;

    // Primary template for frequency_cast impl.
    template <typename _ToDur, typename _CF, typename _CR,
              bool _NumIsOne = false, bool _DenIsOne = false>
    struct __frequency_cast_impl
    {
        template <typename _Rep, typename _Period>
        static constexpr _ToDur
        __cast(const frequency<_Rep, _Period>& __d)
        {
            typedef typename _ToDur::rep __to_rep;
            return _ToDur(static_cast<__to_rep>(static_cast<_CR>(__d.count()) * static_cast<_CR>(_CF::num) / static_cast<_CR>(_CF::den)));
        }
    };

    template <typename _ToDur, typename _CF, typename _CR>
    struct __frequency_cast_impl<_ToDur, _CF, _CR, true, true>
    {
        template <typename _Rep, typename _Period>
        static constexpr _ToDur
        __cast(const frequency<_Rep, _Period>& __d)
        {
            typedef typename _ToDur::rep __to_rep;
            return _ToDur(static_cast<__to_rep>(__d.count()));
        }
    };

    template <typename _ToDur, typename _CF, typename _CR>
    struct __frequency_cast_impl<_ToDur, _CF, _CR, true, false>
    {
        template <typename _Rep, typename _Period>
        static constexpr _ToDur
        __cast(const frequency<_Rep, _Period>& __d)
        {
            typedef typename _ToDur::rep __to_rep;
            return _ToDur(static_cast<__to_rep>(
                static_cast<_CR>(__d.count()) / static_cast<_CR>(_CF::den)));
        }
    };

    template <typename _ToDur, typename _CF, typename _CR>
    struct __frequency_cast_impl<_ToDur, _CF, _CR, false, true>
    {
        template <typename _Rep, typename _Period>
        static constexpr _ToDur
        __cast(const frequency<_Rep, _Period>& __d)
        {
            typedef typename _ToDur::rep __to_rep;
            return _ToDur(static_cast<__to_rep>(
                static_cast<_CR>(__d.count()) * static_cast<_CR>(_CF::num)));
        }
    };

    template <typename _Tp>
    struct __is_frequency
        : std::false_type
    {
    };

    template <typename _Rep, typename _Period>
    struct __is_frequency<frequency<_Rep, _Period>>
        : std::true_type
    {
    };

    /// duration_cast
    template <typename _ToDur, typename _Rep, typename _Period>
    constexpr typename enable_if<__is_frequency<_ToDur>::value, _ToDur>::type duration_cast(const frequency<_Rep, _Period>& __d)
    {
        typedef typename _ToDur::period __to_period;
        typedef typename _ToDur::rep __to_rep;
        typedef ratio_divide<_Period, __to_period> __cf;
        typedef typename common_type<__to_rep, _Rep, intmax_t>::type __cr;
        typedef __frequency_cast_impl<_ToDur, __cf, __cr, __cf::num == 1, __cf::den == 1> __dc;

        return __dc::__cast(__d);
    }

    /// frequency
    template <typename _Rep, typename _Period>
    struct frequency
    {
        typedef _Rep rep;
        typedef _Period period;

        static_assert(!__is_frequency<_Rep>::value, "rep cannot be a frequency");
        static_assert(std::chrono::__is_ratio<_Period>::value, "period must be a specialization of ratio");
        static_assert(_Period::num > 0, "period must be positive");

        // 20.11.5.1 construction / copy / destroy
        constexpr frequency() = default;

        // NB: Make constexpr implicit. This cannot be explicitly
        // constexpr, as any UDT that is not a literal type with a
        // constexpr copy constructor will be ill-formed.
        frequency(const frequency&) = default;

        template <typename _Rep2, typename = typename enable_if<is_convertible<_Rep2, rep>::value && (std::chrono::treat_as_floating_point<rep>::value || !std::chrono::treat_as_floating_point<_Rep2>::value)>::type>
        constexpr explicit frequency(const _Rep2& __rep)
            : __r(static_cast<rep>(__rep))
        {
        }

        template <typename _Rep2, typename _Period2, typename = typename enable_if<treat_as_floating_point<rep>::value || (ratio_divide<_Period2, period>::den == 1 && !treat_as_floating_point<_Rep2>::value)>::type>
        constexpr frequency(const frequency<_Rep2, _Period2>& __d)
            : __r(duration_cast<frequency>(__d).count()) {}

        ~frequency() = default;
        frequency& operator=(const frequency&) = default;

        // 20.11.5.2 observer
        constexpr rep count() const { return __r; }

        constexpr frequency operator+() const { return *this; }
        constexpr frequency operator-() const { return frequency(-__r); }

        frequency& operator++()
        {
            ++__r;
            return *this;
        }

        frequency operator++(int)
        {
            return frequency(__r++);
        }

        frequency& operator--()
        {
            --__r;
            return *this;
        }

        frequency
        operator--(int)
        {
            return frequency(__r--);
        }

        frequency&
        operator+=(const frequency& __d)
        {
            __r += __d.count();
            return *this;
        }

        frequency&
        operator-=(const frequency& __d)
        {
            __r -= __d.count();
            return *this;
        }

        frequency&
        operator*=(const rep& __rhs)
        {
            __r *= __rhs;
            return *this;
        }

        frequency&
        operator/=(const rep& __rhs)
        {
            __r /= __rhs;
            return *this;
        }

        // DR 934.
        template <typename _Rep2 = rep>
        typename enable_if<!treat_as_floating_point<_Rep2>::value,
                           frequency&>::type
        operator%=(const rep& __rhs)
        {
            __r %= __rhs;
            return *this;
        }

        template <typename _Rep2 = rep>
        typename enable_if<!treat_as_floating_point<_Rep2>::value,
                           frequency&>::type
        operator%=(const frequency& __d)
        {
            __r %= __d.count();
            return *this;
        }

        // 20.11.5.4 special values
        static constexpr frequency
        zero()
        {
            return frequency(duration_values<rep>::zero());
        }

        static constexpr frequency
        min()
        {
            return frequency(duration_values<rep>::min());
        }

        static constexpr frequency
        max()
        {
            return frequency(duration_values<rep>::max());
        }

     private:
        rep __r;
    };

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr typename common_type<frequency<_Rep1, _Period1>, frequency<_Rep2, _Period2>>::type operator+(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        typedef frequency<_Rep1, _Period1> __dur1;
        typedef frequency<_Rep2, _Period2> __dur2;
        typedef typename common_type<__dur1, __dur2>::type __cd;
        return __cd(__cd(__lhs).count() + __cd(__rhs).count());
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr typename common_type<frequency<_Rep1, _Period1>, frequency<_Rep2, _Period2>>::type operator-(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        typedef frequency<_Rep1, _Period1> __dur1;
        typedef frequency<_Rep2, _Period2> __dur2;
        typedef typename common_type<__dur1, __dur2>::type __cd;
        return __cd(__cd(__lhs).count() - __cd(__rhs).count());
    }

    template <typename _Rep1, typename _Period, typename _Rep2>
    constexpr frequency<typename __common_rep_type<_Rep1, _Rep2>::type, _Period> operator*(const frequency<_Rep1, _Period>& __d, const _Rep2& __s)
    {
        typedef frequency<typename common_type<_Rep1, _Rep2>::type, _Period> __cd;
        return __cd(__cd(__d).count() * __s);
    }

    template <typename _Rep1, typename _Rep2, typename _Period>
    constexpr frequency<typename __common_rep_type<_Rep2, _Rep1>::type, _Period> operator*(const _Rep1& __s, const frequency<_Rep2, _Period>& __d)
    {
        return __d * __s;
    }

    template <typename _Rep1, typename _Period, typename _Rep2>
    constexpr frequency<typename __common_rep_type<_Rep1, typename enable_if<!__is_frequency<_Rep2>::value, _Rep2>::type>::type, _Period> operator/(const frequency<_Rep1, _Period>& __d, const _Rep2& __s)
    {
        typedef frequency<typename common_type<_Rep1, _Rep2>::type, _Period> __cd;
        return __cd(__cd(__d).count() / __s);
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr typename common_type<_Rep1, _Rep2>::type operator/(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        typedef frequency<_Rep1, _Period1> __dur1;
        typedef frequency<_Rep2, _Period2> __dur2;
        typedef typename common_type<__dur1, __dur2>::type __cd;
        return __cd(__lhs).count() / __cd(__rhs).count();
    }

    // DR 934.
    template <typename _Rep1, typename _Period, typename _Rep2>
    constexpr frequency<typename __common_rep_type<_Rep1, typename enable_if<!__is_frequency<_Rep2>::value, _Rep2>::type>::type, _Period> operator%(const frequency<_Rep1, _Period>& __d, const _Rep2& __s)
    {
        typedef frequency<typename common_type<_Rep1, _Rep2>::type, _Period> __cd;
        return __cd(__cd(__d).count() % __s);
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr typename common_type<frequency<_Rep1, _Period1>, frequency<_Rep2, _Period2>>::type operator%(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        typedef frequency<_Rep1, _Period1> __dur1;
        typedef frequency<_Rep2, _Period2> __dur2;
        typedef typename common_type<__dur1, __dur2>::type __cd;
        return __cd(__cd(__lhs).count() % __cd(__rhs).count());
    }

    // comparisons
    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr bool operator==(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        typedef frequency<_Rep1, _Period1> __dur1;
        typedef frequency<_Rep2, _Period2> __dur2;
        typedef typename common_type<__dur1, __dur2>::type __ct;
        return __ct(__lhs).count() == __ct(__rhs).count();
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr bool operator<(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        typedef frequency<_Rep1, _Period1> __dur1;
        typedef frequency<_Rep2, _Period2> __dur2;
        typedef typename common_type<__dur1, __dur2>::type __ct;
        return __ct(__lhs).count() < __ct(__rhs).count();
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr bool operator!=(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        return !(__lhs == __rhs);
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr bool operator<=(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        return !(__rhs < __lhs);
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr bool operator>(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        return __rhs < __lhs;
    }

    template <typename _Rep1, typename _Period1, typename _Rep2, typename _Period2>
    constexpr bool operator>=(const frequency<_Rep1, _Period1>& __lhs, const frequency<_Rep2, _Period2>& __rhs)
    {
        return !(__lhs < __rhs);
    }

    using hertz = frequency<float>;
    using kilohertz = frequency<float, std::kilo>;
    using megahertz = frequency<float, std::mega>;
    using gigahertz = frequency<float, std::giga>;

    constexpr hertz operator""_Hz(long double hz) { return hertz{hz}; }
    constexpr hertz operator""_Hz(uint64_t hz) { return hertz{hz}; }
    constexpr kilohertz operator""_kHz(long double kHz) { return kilohertz{kHz}; }
    constexpr kilohertz operator""_kHz(uint64_t kHz) { return kilohertz{kHz}; }
    constexpr megahertz operator""_MHz(long double MHz) { return megahertz{MHz}; }
    constexpr megahertz operator""_MHz(uint64_t MHz) { return megahertz{MHz}; }
    constexpr gigahertz operator""_GHz(long double GHz) { return gigahertz{GHz}; }
    constexpr gigahertz operator""_GHz(uint64_t GHz) { return gigahertz{GHz}; }
}
