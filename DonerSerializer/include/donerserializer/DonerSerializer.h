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


#include <donerserializer/akrzemi1_optional.h>

#include <rapidjson/document.h>


#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#define DONER_DEFINE_SERIALIZABLE_DATA(base_class, ...)                        \
  template <> struct SDonerSerializerClassProperties<base_class> {             \
    using T = base_class;                                                      \
    constexpr static auto s_properties = std::make_tuple(__VA_ARGS__);         \
    constexpr static std::size_t s_propertiesCount =                           \
        std::tuple_size<decltype(s_properties)>::value;                        \
  };

#define DONER_ADD_VAR_INFO(var)                                                \
  DonerSerializer::SProperty<T, decltype(T::var)> { &T::var, ##var }

#define DONER_ADD_NAMED_VAR_INFO(var, name)                                    \
  DonerSerializer::SProperty<T, decltype(T::var)> { &T::var, name }

#define DONER_DECLARE_OBJECT_AS_SERIALIZABLE(base_class)                       \
  friend struct SDonerSerializerClassProperties<base_class>;

#define DONER_COMPONENT_SERIALIZATION_IMPL(base_class)                         \
  void base_class::ParseAtts(const rapidjson::Value &atts) {                   \
    ParseAttsInternal<base_class>(this, atts);                                 \
  }

#define DONER_DESERIALIZE_OBJECT_REF(object_ref, json_node)                    \
  DonerSerializer::CSerializationHelper<std::remove_reference<decltype(        \
      object_ref)>::type>::DeserializeFromJson(json_node, &object_ref);

#define DONER_DESERIALIZE_OBJECT_PTR(object_ptr, json_node)                    \
  DonerSerializer::CSerializationHelper<std::remove_pointer<decltype(          \
      object_ptr)>::type>::DeserializeFromJson(json_node, object_ptr);

#define DONER_DESERIALIZE_THIS(json_node)                                      \
  DonerSerializer::CSerializationHelper<std::remove_pointer<decltype(          \
      this)>::type>::DeserializeFromJson(json_node, this);

// Base properties container class (outside any namespace for an easier usage)
template <typename T>
struct SDonerSerializerClassProperties
{
	constexpr static auto s_properties = std::tuple<>();
	constexpr static std::size_t s_propertiesCount = std::tuple_size<decltype(s_properties)>::value;
};

namespace DonerSerializer
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

	template <class T, class Enable = void>
	struct STypeSerializer
	{
		static std::experimental::optional<T> DeserializeFromJson(const rapidjson::Value& /*att*/)
		{
			return std::experimental::nullopt;
		}
	};

	template <std::size_t iteration, typename T>
	void DoDeserializeDataFromJson(T& object, const rapidjson::Value& data)
	{
		// get the property
		const auto property =
			std::get<iteration>(std::decay<SDonerSerializerClassProperties<T>>::type::s_properties);

		// get the type of the property
		using Type = typename decltype(property)::Type;

		// set the value to the member
		auto op = STypeSerializer<Type>::DeserializeFromJson(data[property.m_name]);
		if (op)
		{
			object.*(property.m_member) = op.value();
		}
	}

	template <std::size_t iteration, typename T>
	typename std::enable_if<(iteration == 0)>::type DeserializeDataFromJson(T& object, const rapidjson::Value& data)
	{
		DoDeserializeDataFromJson<iteration>(object, data);
	}

	template <std::size_t iteration, typename T>
	typename std::enable_if<(iteration > 0)>::type DeserializeDataFromJson(T& object, const rapidjson::Value& data)
	{
		DoDeserializeDataFromJson<iteration>(object, data);
		DeserializeDataFromJson<iteration - 1>(object, data);
	}

	template <class T>
	class CSerializationHelper
	{
	public:
		static std::experimental::optional<T> DeserializeFromJson(const rapidjson::Value& att, T* value)
		{
			if (value != nullptr)
			{
				DeserializeDataFromJson<SDonerSerializerClassProperties<T>::s_propertiesCount - 1>(*value, att);
			}
			else
			{
				T object;
				DeserializeDataFromJson<SDonerSerializerClassProperties<T>::s_propertiesCount - 1>(object, att);
				return std::experimental::make_optional<T>(std::move(object));
			}
			return std::experimental::nullopt;
		}
	};
}

// ===========================
// Basic Types Specializations
// ===========================
namespace DonerSerializer
{
	template <>
	struct STypeSerializer<std::int32_t>
	{
		static std::experimental::optional<std::int32_t> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsInt())
			{
				return std::experimental::make_optional<std::int32_t>(att.GetInt());
			}
			return std::experimental::nullopt;
		}
	};

	template <>
	struct STypeSerializer<std::uint32_t>
	{
		static std::experimental::optional<std::uint32_t> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsUint())
			{
				return std::experimental::make_optional<std::uint32_t>(att.GetUint());
			}
			return std::experimental::nullopt;
		}
	};

	template <class T>
	struct STypeSerializer<T, typename std::enable_if<std::is_enum<T>::value>::type>
	{
		static std::experimental::optional<T> DeserializeFromJson(const rapidjson::Value& att)
		{
			auto op = STypeSerializer<std::int32_t>::DeserializeFromJson(att);
			if (op)
			{
				T dummy = static_cast<T>(op.value());
				return std::experimental::make_optional<T>(std::move(dummy));
			}
			return std::experimental::nullopt;
		}
	};

	// std::vector
	template<template<typename, typename> class TT, typename T1, typename T2>
	struct STypeSerializer<TT<T1, T2>>
	{
		using Vector = TT<T1, T2>;
		static std::experimental::optional<Vector> DeserializeFromJson(const rapidjson::Value& atts)
		{
			if (atts.IsArray())
			{
				Vector v;
				for (const rapidjson::Value& att : atts.GetArray())
				{
					auto op = STypeSerializer<T1>::DeserializeFromJson(att);
					if (op)
					{
						v.emplace_back(op.value());
					}
				}
				return std::experimental::make_optional<Vector>(std::move(v));
			}
			return std::experimental::nullopt;
		}
	};

	template<>
	struct STypeSerializer<const char*>
	{
		static std::experimental::optional<const char*> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsString())
			{
				return std::experimental::optional<const char*>(att.GetString());
			}
			return std::experimental::nullopt;
		}
	};

	template<>
	struct STypeSerializer<std::string>
	{
		static std::experimental::optional<std::string> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsString())
			{
				return std::experimental::optional<std::string>(att.GetString());
			}
			return std::experimental::nullopt;
		}
	};

	template<>
	struct STypeSerializer<bool>
	{
		static std::experimental::optional<bool> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsBool())
			{
				return std::experimental::optional<bool>(att.GetBool());
			}
			return std::experimental::optional<bool>();
		}
	};

	template<>
	struct STypeSerializer<std::int64_t>
	{
		static std::experimental::optional<std::int64_t> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsInt64())
			{
				return std::experimental::optional<std::int64_t>(att.GetInt64());
			}
			return std::experimental::nullopt;
		}
	};

	template<>
	struct STypeSerializer<std::uint64_t>
	{
		static std::experimental::optional<std::uint64_t> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsUint64())
			{
				return std::experimental::optional<std::uint64_t>(att.GetUint64());
			}
			return std::experimental::nullopt;
		}
	};

	template<>
	struct STypeSerializer<float>
	{
		static std::experimental::optional<float> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsFloat())
			{
				return std::experimental::optional<float>(att.GetFloat());
			}
			return std::experimental::nullopt;
		}
	};

	template<>
	struct STypeSerializer<double>
	{
		static std::experimental::optional<double> DeserializeFromJson(const rapidjson::Value& att)
		{
			if (att.IsDouble())
			{
				return std::experimental::optional<double>(att.GetDouble());
			}
			return std::experimental::nullopt;
		}
	};
}
