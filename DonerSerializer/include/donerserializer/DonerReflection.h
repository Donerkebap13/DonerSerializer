////////////////////////////////////////////////////////////
//
// MIT License
//
// DonerSerializer - A Tweaked Entity-Component System
// Copyright(c) 2017 Donerkebap13
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////

#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#define DONER_DEFINE_SERIALIZABLE_DATA(base_class, ...)                        \
  template <> struct SDonerReflectionClassProperties<base_class> {             \
    using T = base_class;                                                      \
    constexpr static auto s_properties = std::make_tuple(__VA_ARGS__);         \
    constexpr static std::size_t s_propertiesCount =                           \
        std::tuple_size<decltype(s_properties)>::value;                        \
  };

#define DONER_ADD_VAR_INFO(var)                                                \
  DonerReflection::SProperty<T, decltype(T::var)> { &T::var, ##var }

#define DONER_ADD_NAMED_VAR_INFO(var, name)                                    \
  DonerReflection::SProperty<T, decltype(T::var)> { &T::var, name }

#define DONER_DECLARE_OBJECT_AS_SERIALIZABLE(base_class)                       \
  friend struct SDonerReflectionClassProperties<base_class>;

template <typename T>
struct SDonerReflectionClassProperties
{
	constexpr static auto s_properties = std::tuple<>();
	constexpr static std::size_t s_propertiesCount = std::tuple_size<decltype(s_properties)>::value;
};

namespace DonerReflection
{
	template <typename Class, typename T>
	struct SProperty
	{
		constexpr SProperty(T Class::*member, const char* name)
			: m_member(member)
			, m_name(name)
		{
		}

		using Type = T;

		T Class::*m_member;
		const char* m_name;
	};

	template <std::size_t iteration, typename Resolver, typename T, typename... Args>
	void DoApplyFunctionToObjectElements(T& object, Args&&... args)
	{
		constexpr auto property = std::get<iteration>(std::decay<SDonerReflectionClassProperties<T>>::type::s_properties);
		Resolver::Apply(property, object, std::forward<Args>(args)...);
	}

	template <std::size_t iteration, typename Resolver, typename T, typename... Args>
	typename std::enable_if<(iteration == 0)>::type ApplyFunctionToObjectElements(T& object, Args&&... args)
	{
		DoApplyFunctionToObjectElements<iteration, Resolver>(object, std::forward<Args>(args)...);
	}

	template <std::size_t iteration, typename Resolver, typename T, typename... Args>
	typename std::enable_if<(iteration > 0)>::type ApplyFunctionToObjectElements(T& object, Args&&... args)
	{
		DoApplyFunctionToObjectElements<iteration, Resolver>(object, std::forward<Args>(args)...);
		ApplyFunctionToObjectElements<iteration - 1, Resolver>(object, std::forward<Args>(args)...);
	}

	template <std::size_t iteration, typename Resolver, typename T>
	void DoApplyFunctionToObjectElements(T& object)
	{
		constexpr auto property = std::get<iteration>(std::decay<SDonerReflectionClassProperties<T>>::type::s_properties);
		Resolver::Apply(property.m_name, object.*(property.m_member));
	}

	template <std::size_t iteration, typename Resolver, typename T>
	typename std::enable_if<(iteration == 0)>::type ApplyFunctionToObjectElements(T& object)
	{
		DoApplyFunctionToObjectElements<iteration, Resolver>(object);
	}

	template <std::size_t iteration, typename Resolver, typename T>
	typename std::enable_if<(iteration > 0)>::type ApplyFunctionToObjectElements(T& object)
	{
		DoApplyFunctionToObjectElements<iteration, Resolver>(object);
		ApplyFunctionToObjectElements<iteration - 1, Resolver>(object);
	}
}
