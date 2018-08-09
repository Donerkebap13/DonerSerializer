////////////////////////////////////////////////////////////
//
// MIT License
//
// DonerSerializer
// Copyright(c) 2018 Donerkebap13
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

#include <donerserializer/ISerializable.h>

#include <donerreflection/DonerReflection.h>

#include <rapidjson/document.h>

#include <string>

namespace DonerSerializer
{
	class CJsonDeserializer
	{
	public:
		template<class T>
		static void Deserialize(T& object, rapidjson::Value& value)
		{
			APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(object, DonerSerializer::CDeserializationResolver, value)
		}

		template<class T>
		static void Deserialize(const T& object, rapidjson::Value& value)
		{
			APPLY_RESOLVER_WITH_PARAMS_TO_CONST_OBJECT(object, DonerSerializer::CDeserializationResolver, value)
		}

		template<class T>
		static void Deserialize(T& object, const char* const jsonStr)
		{
			rapidjson::Document parser;
			rapidjson::Value& root = parser.Parse(jsonStr);
			APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(object, DonerSerializer::CDeserializationResolver, root)
		}

		template<class T>
		static void Deserialize(const T& object, const char* const jsonStr)
		{
			rapidjson::Document parser;
			rapidjson::Value& root = parser.Parse(jsonStr);
			APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(object, DonerSerializer::CDeserializationResolver, root)
		}
	};

	class CDeserializationResolver
	{
	public:
		template<typename MainClassType, typename MemberType>
		static void Apply(const DonerReflection::SProperty<MainClassType, MemberType>& property, MainClassType& object, const rapidjson::Value& value)
		{
			if (value.HasMember(property.m_name))
			{
				CDeserializationResolverType<MemberType>::Apply(object.*(property.m_member), value[property.m_name]);
			}
		}

		template <class T, class Enable = void>
		class CDeserializationResolverType
		{
		public:
			static void Apply(T& value, const rapidjson::Value& att)
			{}
		};
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<std::int32_t>
	{
	public:
		static void Apply(std::int32_t& value, const rapidjson::Value& att)
		{
			if (att.IsInt())
			{
				value = att.GetInt();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<std::uint32_t>
	{
	public:
		static void Apply(std::uint32_t& value, const rapidjson::Value& att)
		{
			if (att.IsUint())
			{
				value = att.GetUint();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<std::int64_t>
	{
	public:
		static void Apply(std::int64_t& value, const rapidjson::Value& att)
		{
			if (att.IsInt64())
			{
				value = att.GetInt64();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<std::uint64_t>
	{
	public:
		static void Apply(std::uint64_t& value, const rapidjson::Value& att)
		{
			if (att.IsUint64())
			{
				value = att.GetUint64();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<float>
	{
	public:
		static void Apply(float& value, const rapidjson::Value& att)
		{
			if (att.IsFloat())
			{
				value = att.GetFloat();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<double>
	{
	public:
		static void Apply(double& value, const rapidjson::Value& att)
		{
			if (att.IsDouble())
			{
				value = att.GetDouble();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<bool>
	{
	public:
		static void Apply(bool& value, const rapidjson::Value& att)
		{
			if (att.IsBool())
			{
				value = att.GetBool();
			}
		}
	};

	template <>
	class CDeserializationResolver::CDeserializationResolverType<std::string>
	{
	public:
		static void Apply(std::string& value, const rapidjson::Value& att)
		{
			if (att.IsString())
			{
				value = std::string(att.GetString());
			}
		}
	};

	template <class T>
	class CDeserializationResolver::CDeserializationResolverType<T, typename std::enable_if<std::is_enum<T>::value>::type>
	{
	public:
		static void Apply(T& value, const rapidjson::Value& att)
		{
			if (att.IsInt())
			{
				value = static_cast<T>(att.GetInt());
			}
		}
	};

	template<template<typename, typename> class TT, typename T1, typename T2>
	class CDeserializationResolver::CDeserializationResolverType<TT<T1, T2>>
	{
	public:
		static void Apply(TT<T1, T2>& value, const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				for (const rapidjson::Value& att : atts.GetArray())
				{
					T1 element;
					CDeserializationResolver::CDeserializationResolverType<T1>::Apply(element, att);
					value.push_back(element);
				}
			}
		}
	};

	template <template <typename, typename, typename...> class TT, typename T1, typename T2, typename... Args>
	class CDeserializationResolver::CDeserializationResolverType<TT<T1, T2, Args...>>
	{
	public:
		static void Apply(TT<T1, T2, Args...>& map, const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				for (const rapidjson::Value& att : atts.GetArray())
				{
					T1 key;
					CDeserializationResolver::CDeserializationResolverType<T1>::Apply(key, att[0]);
					T2 value;
					CDeserializationResolver::CDeserializationResolverType<T2>::Apply(value, att[1]);
					map[key] = value;
				}
			}
		}
	};

	template <class T>
	class CDeserializationResolver::CDeserializationResolverType<T, typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type>
	{
	public:
		static void Apply(T& value, const rapidjson::Value& att)
		{
			APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(value, CDeserializationResolver, att)
		}
	};
}
