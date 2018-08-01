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

#include <donerserializer/DonerReflection.h>

#include <rapidjson/document.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#define DONER_SERIALIZE_OBJECT(object_ref, json_document)                      \
  DonerReflection::ApplyFunctionToObjectElements<                              \
      SDonerReflectionClassProperties<typename std::decay<decltype(            \
          object_ref)>::type>::s_propertiesCount -                             \
          1,                                                                   \
      DonerReflection::CSerializationResolver>(object_ref, json_document);

#define DONER_DESERIALIZE_OBJECT(object_ref, json_value)                       \
  DonerReflection::ApplyFunctionToObjectElements<                              \
      SDonerReflectionClassProperties<typename std::decay<decltype(            \
          object_ref)>::type>::s_propertiesCount -                             \
          1,                                                                   \
      DonerReflection::CDeserializationResolver>(object_ref, json_value);

namespace DonerReflection
{
	class ISerializable
	{
	public:
		virtual ~ISerializable() {}
	};

	class CSerializationResolver
	{
	public:
		template<typename MainClassType, typename MemberType>
		static void Apply(const DonerReflection::SProperty<MainClassType, MemberType>& property, MainClassType& object, rapidjson::Document& root)
		{
			Apply(property.m_name, object.*(property.m_member), root);
		}

	private:
		static void Apply(const char* name, std::int32_t value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, std::uint32_t value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, std::int64_t value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, std::uint64_t value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, float value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, double value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, bool value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), value, root.GetAllocator());
		}

		static void Apply(const char* name, const char* value, rapidjson::Document& root)
		{
			if (value)
			{
				root.AddMember(rapidjson::GenericStringRef<char>(name), rapidjson::GenericStringRef<char>(value), root.GetAllocator());
			}
		}

		static void Apply(const char* name, std::string& value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), rapidjson::GenericStringRef<char>(value.c_str()), root.GetAllocator());
		}

		template<typename T>
		static typename std::enable_if<std::is_enum<T>::value>::type Apply(const char* name, T& value, rapidjson::Document& root)
		{
			root.AddMember(rapidjson::GenericStringRef<char>(name), static_cast<std::int32_t>(value), root.GetAllocator());
		}

		template<typename T>
		static typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type Apply(const char* name, T& value, rapidjson::Document& root)
		{
			rapidjson::Document document;
			document.SetObject();
			DONER_SERIALIZE_OBJECT(value, document)
				rapidjson::Value newVal(document, root.GetAllocator());
			root.AddMember(rapidjson::GenericStringRef<char>(name), newVal, root.GetAllocator());
		}

		template<typename T>
		static void Apply(const char* name, std::vector<T>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& member : value)
			{
				SerializeToJsonArray(array, member, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		static void Apply(const char* name, std::vector<bool>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (bool member : value)
			{
				SerializeToJsonArray(array, member, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		template<typename T>
		static void Apply(const char* name, std::list<T>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& member : value)
			{
				SerializeToJsonArray(array, member, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}
		
		static void Apply(const char* name, std::list<bool>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (bool member : value)
			{
				SerializeToJsonArray(array, member, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		template <typename T1, typename T2>
		static void Apply(const char* name, std::map<T1, T2>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& val : value)
			{
				rapidjson::Value element(rapidjson::kArrayType);
				SerializeToJsonArray(element, val.first, root.GetAllocator());
				SerializeToJsonArray(element, val.second, root.GetAllocator());
				array.PushBack(element, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		template <typename T1, typename T2>
		static void Apply(const char* name, std::unordered_map<T1, T2>& value, rapidjson::Document& root)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& val : value)
			{
				rapidjson::Value element(rapidjson::kArrayType);
				SerializeToJsonArray(element, val.first, root.GetAllocator());
				SerializeToJsonArray(element, val.second, root.GetAllocator());
				array.PushBack(element, root.GetAllocator());
			}
			root.AddMember(rapidjson::GenericStringRef<char>(name), array, root.GetAllocator());
		}

		static void SerializeToJsonArray(rapidjson::Value& root, std::int32_t value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, std::uint32_t value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, std::int64_t value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, std::uint64_t value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, float value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, double value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, bool value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(value, allocator);
		}

		static void SerializeToJsonArray(rapidjson::Value& root, const char* value, rapidjson::Document::AllocatorType& allocator)
		{
			if (value)
			{
				root.PushBack(rapidjson::GenericStringRef<char>(value), allocator);
			}
		}

		static void SerializeToJsonArray(rapidjson::Value& root, std::string& value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(rapidjson::GenericStringRef<char>(value.c_str()), allocator);
		}

		template<typename T>
		static typename std::enable_if<std::is_enum<T>::value>::type SerializeToJsonArray(rapidjson::Value& root, T& value, rapidjson::Document::AllocatorType& allocator)
		{
			root.PushBack(static_cast<std::int32_t>(value), allocator);
		}

		template<typename T>
		static typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type SerializeToJsonArray(rapidjson::Value& root, T& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Document document;
			document.SetObject();
			DONER_SERIALIZE_OBJECT(value, document)
				rapidjson::Value newVal(document, allocator);
			root.PushBack(newVal, allocator);
		}

		template<typename T>
		static void SerializeToJsonArray(rapidjson::Value& root, std::vector<T>& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& member : value)
			{
				SerializeToJsonArray(array, member, allocator);
			}
			root.PushBack(array, allocator);
		}

		template<typename T>
		static void SerializeToJsonArray(rapidjson::Value& root, std::list<T>& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& member : value)
			{
				SerializeToJsonArray(array, member, allocator);
			}
			root.PushBack(array, allocator);
		}

		template <typename T1, typename T2>
		static void SerializeToJsonArray(rapidjson::Value& root, std::map<T1, T2>& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& val : value)
			{
				rapidjson::Value element(rapidjson::kArrayType);
				SerializeToJsonArray(element, val.first, allocator);
				SerializeToJsonArray(element, val.second, allocator);
				array.PushBack(element, allocator);
			}
			root.PushBack(array, allocator);
		}

		template <typename T1, typename T2>
		static void SerializeToJsonArray(rapidjson::Value& root, std::unordered_map<T1, T2>& value, rapidjson::Document::AllocatorType& allocator)
		{
			rapidjson::Value array(rapidjson::kArrayType);
			for (auto& val : value)
			{
				rapidjson::Value element(rapidjson::kArrayType);
				SerializeToJsonArray(element, val.first, allocator);
				SerializeToJsonArray(element, val.second, allocator);
				array.PushBack(element, allocator);
			}
			root.PushBack(array, allocator);
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
				Apply(object.*(property.m_member), value[property.m_name]);
			}
		}
	private:
		static void Apply(std::int32_t& value, const rapidjson::Value& att)
		{
			if (att.IsInt())
			{
				value = att.GetInt();
			}
		}

		static void Apply(std::uint32_t& value, const rapidjson::Value& att)
		{
			if (att.IsUint())
			{
				value = att.GetUint();
			}
		}

		static void Apply(std::int64_t& value, const rapidjson::Value& att)
		{
			if (att.IsInt64())
			{
				value = att.GetInt64();
			}
		}

		static void Apply(std::uint64_t& value, const rapidjson::Value& att)
		{
			if (att.IsUint64())
			{
				value = att.GetUint64();
			}
		}

		static void Apply(float& value, const rapidjson::Value& att)
		{
			if (att.IsFloat())
			{
				value = att.GetFloat();
			}
		}

		static void Apply(double& value, const rapidjson::Value& att)
		{
			if (att.IsDouble())
			{
				value = att.GetDouble();
			}
		}

		static void Apply(bool& value, const rapidjson::Value& att)
		{
			if (att.IsBool())
			{
				value = att.GetBool();
			}
		}

		static void Apply(std::string& value, const rapidjson::Value& att)
		{
			if (att.IsString())
			{
				value = std::string(att.GetString());
			}
		}

		template <class T>
		static typename std::enable_if<std::is_enum<T>::value>::type Apply(T& value, const rapidjson::Value& att)
		{
			if (att.IsInt())
			{
				std::int32_t intValue = att.GetInt();
				value = static_cast<T>(intValue);
			}
		}

		template <class T>
		static typename std::enable_if<std::is_base_of<ISerializable, T>::value>::type Apply(T& value, const rapidjson::Value& att)
		{
			DONER_DESERIALIZE_OBJECT(value, att)
		}

		template <class T>
		static void Apply(std::vector<T>& value, const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				for (const rapidjson::Value& att : atts.GetArray())
				{
					T element;
					Apply(element, att);
					value.push_back(element);
				}
			}
		}

		template <class T>
		static void Apply(std::list<T>& value, const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				for (const rapidjson::Value& att : atts.GetArray())
				{
					T element;
					Apply(element, att);
					value.push_back(element);
				}
			}
		}

		template <class K, class V>
		static void Apply(std::map<K, V>& map, const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				for (const rapidjson::Value& att : atts.GetArray())
				{
					K key;
					Apply(key, att[0]);
					V value;
					Apply(value, att[1]);
					map[key] = value;
				}
			}
		}

		template <class K, class V>
		static void Apply(std::unordered_map<K, V>& map, const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				for (const rapidjson::Value& att : atts.GetArray())
				{
					K key;
					Apply(key, att[0]);
					V value;
					Apply(value, att[1]);
					map[key] = value;
				}
			}
		}
	};
}
