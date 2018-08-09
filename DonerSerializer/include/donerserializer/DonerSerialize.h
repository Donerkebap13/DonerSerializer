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

#define DONER_SERIALIZE_OBJECT_TO_JSON(object_ref, json_document)              \
APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(object_ref, DonerSerializer::CSerializationResolver, json_document)

#define DONER_SERIALIZE_CONST_OBJECT_TO_JSON(object_ref, json_document)              \
APPLY_RESOLVER_WITH_PARAMS_TO_CONST_OBJECT(object_ref, DonerSerializer::CSerializationResolver, json_document)

namespace DonerSerializer
{
	class CSerializationResolver
	{
	public:
		template<typename MainClassType, typename MemberType>
		static void Apply(const DonerReflection::SProperty<MainClassType, MemberType>& property, const MainClassType& object, rapidjson::Document& root)
		{
			if (root.IsNull())
			{
				root.SetObject();
			}
			CSerializationResolverType<MemberType>::Apply(property.m_name, object.*(property.m_member), root);
		}

		template <class T, class Enable = void>
		class CSerializationResolverType
		{
		public:
			static void Apply(const char* name, const T& value, rapidjson::Document& root)
			{}
			static void SerializeToJsonArray(rapidjson::Value& root, const T& value, rapidjson::Document::AllocatorType& allocator)
			{}
		};
	};

	template <class T>
	class CSerializationResolver::CSerializationResolverType<T, typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type>
	{
	public:
		static void Apply(const char* name, const T& value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const T& value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}
	};

	template <class T>
	class CSerializationResolver::CSerializationResolverType<T, typename std::enable_if<std::is_enum<T>::value>::type>
	{
	public:
		static void Apply(const char* name, const T& value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), static_cast<std::int32_t>(value), root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const T& value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(static_cast<std::int32_t>(value), allocator);
		}
	};

	template <>
	class CSerializationResolver::CSerializationResolverType<std::string>
	{
	public:
		static void Apply(const char* name, const std::string& value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), rapidjson::GenericStringRef<char>(value.c_str()), root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const std::string& value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(rapidjson::GenericStringRef<char>(value.c_str()), allocator);
		}
	};

	template<template<typename, typename> class TT, typename T1, typename T2>
	class CSerializationResolver::CSerializationResolverType<TT<T1, T2>>
	{
	public:
		static void Apply(const char* name, const TT<T1, T2>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (const auto& member : value)
			{
				CSerializationResolver::CSerializationResolverType<T1>::SerializeToJsonArray(array, member, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const TT<T1, T2>& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (const auto& member : value)
			{
				CSerializationResolver::CSerializationResolverType<T1>::SerializeToJsonArray(array, member, allocator);
			}
			root.PushBack(array, allocator);
		}
	};

	template <template <typename, typename, typename...> class TT, typename T1, typename T2, typename... Args>
	class CSerializationResolver::CSerializationResolverType<TT<T1, T2, Args...>>
	{
	public:
		static void Apply(const char* name, const TT<T1, T2, Args...>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (const auto& val : value)
			{
				rapidjson::Value element(rapidjson::kArrayType);
				CSerializationResolver::CSerializationResolverType<T1>::SerializeToJsonArray(element, val.first, root.GetAllocator());
				CSerializationResolver::CSerializationResolverType<T2>::SerializeToJsonArray(element, val.second, root.GetAllocator());
				array.PushBack(element, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const TT<T1, T2, Args...>& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (const auto& val : value)
			{
				rapidjson::Value element(rapidjson::kArrayType);
				CSerializationResolver::CSerializationResolverType<T1>::SerializeToJsonArray(element, val.first, allocator);
				CSerializationResolver::CSerializationResolverType<T2>::SerializeToJsonArray(element, val.second, allocator);
				array.PushBack(element, allocator);
			}
			root.PushBack(array, allocator);
		}
	};

	template <class T>
	class CSerializationResolver::CSerializationResolverType<T, typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type>
	{
	public:
		static void Apply(const char* name, const T& value, rapidjson::Document& root)
		{
			rapidjson::Document document;
			DONER_SERIALIZE_CONST_OBJECT_TO_JSON(value, document)
			rapidjson::Value newVal(document, root.GetAllocator());
			root.AddMember(rapidjson::GenericStringRef<char>(name), newVal, root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const T& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Document document;
			DONER_SERIALIZE_CONST_OBJECT_TO_JSON(value, document)
			rapidjson::Value newVal(document, allocator);
			root.PushBack(newVal, allocator);
		}
	};
}
