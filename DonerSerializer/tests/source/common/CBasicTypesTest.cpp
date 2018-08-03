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

#include <donerserializer/DonerSerializer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <gtest/gtest.h>

namespace CBasicTypesTestInternal
{
	const char* const FOO_JSON_DATA = "{\"enum\":0,\"bool\":true,\"double\":6.0,\"float\":5.0,\"uint64t\":4,\"int64t\":3,\"uint32t\":2,\"int32t\":1}";
	const char* const FOO_JSON_DATA_INHERIT = "{\"int32t_2\":7,\"enum\":0,\"bool\":true,\"double\":6.0,\"float\":5.0,\"uint64t\":4,\"int64t\":3,\"uint32t\":2,\"int32t\":1}";

	class CFoo
	{
		DONER_DECLARE_OBJECT_AS_REFLECTABLE(CFoo)
	public:
		enum class EEnumTest {Test1, Test2};

		CFoo()
			: m_int32t(0)
			, m_uint32t(0)
			, m_int64t(0)
			, m_uint64t(0)
			, m_float(0.f)
			, m_double(0.0)
			, m_bool(false)
			, m_enum(EEnumTest::Test1)
		{}

		std::int32_t m_int32t;
		std::uint32_t m_uint32t;
		std::int64_t m_int64t;
		std::uint64_t m_uint64t;
		float m_float;
		double m_double;
		bool m_bool;
		EEnumTest m_enum;
	};

	class CBar : public CFoo
	{
		DONER_DECLARE_OBJECT_AS_REFLECTABLE(CBar)
	public:
		CBar()
			: CFoo()
			, m_int32t_2(0)
		{}

		std::int32_t m_int32t_2;
	};
}

DONER_DEFINE_REFLECTION_DATA(CBasicTypesTestInternal::CFoo,
							   DONER_ADD_NAMED_VAR_INFO(m_int32t, "int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint32t, "uint32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_int64t, "int64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint64t, "uint64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_float, "float"),
							   DONER_ADD_NAMED_VAR_INFO(m_double, "double"),
							   DONER_ADD_NAMED_VAR_INFO(m_bool, "bool"),
							   DONER_ADD_NAMED_VAR_INFO(m_enum, "enum")
)

DONER_DEFINE_REFLECTION_DATA(CBasicTypesTestInternal::CBar,
							   DONER_ADD_NAMED_VAR_INFO(m_int32t, "int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint32t, "uint32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_int64t, "int64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint64t, "uint64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_float, "float"),
							   DONER_ADD_NAMED_VAR_INFO(m_double, "double"),
							   DONER_ADD_NAMED_VAR_INFO(m_bool, "bool"),
							   DONER_ADD_NAMED_VAR_INFO(m_enum, "enum"),
							   DONER_ADD_NAMED_VAR_INFO(m_int32t_2, "int32t_2")
)

struct SResolver0
{
	template<typename MainClassType, typename MemberType>
	static void Apply(const DonerReflection::SProperty<MainClassType, MemberType>& property, MainClassType& object)
	{
		printf("NEIN\n");
	}
};

struct SResolver1
{
	template<typename MainClassType, typename MemberType>
	static void Apply(const DonerReflection::SProperty<MainClassType, MemberType>& property, MainClassType& object, int one)
	{
		printf("%d\n", one);
	}
};

struct SResolverN
{
	template<typename MainClassType, typename MemberType>
	static void Apply(const DonerReflection::SProperty<MainClassType, MemberType>& property, MainClassType& object, int one, int two, int three)
	{
		printf("%d %d %d\n", one, two, three);
	}
};

namespace DonerSerializer
{
	class CBasicTypesTest : public ::testing::Test
	{
	public:
		CBasicTypesTest() = default;
		~CBasicTypesTest() = default;
	};

	TEST_F(CBasicTypesTest, deserialize_basic_types_from_main_class)
	{
		CBasicTypesTestInternal::CFoo foo;

		EXPECT_EQ(0, foo.m_int32t);
		EXPECT_EQ(0, foo.m_uint32t);
		EXPECT_EQ(0, foo.m_int64t);
		EXPECT_EQ(0, foo.m_uint64t);
		EXPECT_EQ(0.f, foo.m_float);
		EXPECT_EQ(0.0, foo.m_double);
		EXPECT_FALSE(foo.m_bool);

		rapidjson::Document parser;
		rapidjson::Value& root = parser.Parse(CBasicTypesTestInternal::FOO_JSON_DATA);

		DONER_DESERIALIZE_OBJECT_FROM_JSON(foo, root)

		EXPECT_EQ(1, foo.m_int32t);
		EXPECT_EQ(2, foo.m_uint32t);
		EXPECT_EQ(3, foo.m_int64t);
		EXPECT_EQ(4, foo.m_uint64t);
		EXPECT_EQ(5.f, foo.m_float);
		EXPECT_EQ(6.0, foo.m_double);
		EXPECT_TRUE(foo.m_bool);


		APPLY_RESOLVER_TO_OBJECT(foo, SResolver0)
		APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(foo, SResolver1, 1)
		APPLY_RESOLVER_WITH_PARAMS_TO_OBJECT(foo, SResolverN, 1, 2, 3)
	}

	TEST_F(CBasicTypesTest, deserialize_basic_types_from_child_class)
	{
		CBasicTypesTestInternal::CBar bar;

		EXPECT_EQ(0, bar.m_int32t);
		EXPECT_EQ(0, bar.m_uint32t);
		EXPECT_EQ(0, bar.m_int64t);
		EXPECT_EQ(0, bar.m_uint64t);
		EXPECT_EQ(0.f, bar.m_float);
		EXPECT_EQ(0.0, bar.m_double);
		EXPECT_FALSE(bar.m_bool);
		EXPECT_EQ(0, bar.m_int32t_2);

		rapidjson::Document parser;
		rapidjson::Value& root = parser.Parse(CBasicTypesTestInternal::FOO_JSON_DATA_INHERIT);

		DONER_DESERIALIZE_OBJECT_FROM_JSON(bar, root)

		EXPECT_EQ(1, bar.m_int32t);
		EXPECT_EQ(2, bar.m_uint32t);
		EXPECT_EQ(3, bar.m_int64t);
		EXPECT_EQ(4, bar.m_uint64t);
		EXPECT_EQ(5.f, bar.m_float);
		EXPECT_EQ(6.0, bar.m_double);
		EXPECT_TRUE(bar.m_bool);
		EXPECT_EQ(7, bar.m_int32t_2);
	}


	TEST_F(CBasicTypesTest, serialize_basic_types_from_main_class)
	{
		CBasicTypesTestInternal::CFoo foo;

		foo.m_int32t = 1;
		foo.m_uint32t = 2;
		foo.m_int64t = 3;
		foo.m_uint64t = 4;
		foo.m_float = 5.f;
		foo.m_double = 6.0;
		foo.m_bool = true;

		rapidjson::Document root;
		DONER_SERIALIZE_OBJECT_TO_JSON(foo, root)

		rapidjson::StringBuffer strbuf;
		strbuf.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		root.Accept(writer);
		std::string pepe(strbuf.GetString());

		ASSERT_STREQ(CBasicTypesTestInternal::FOO_JSON_DATA, pepe.c_str());
	}

	TEST_F(CBasicTypesTest, serialize_basic_types_from_child_class)
	{
		CBasicTypesTestInternal::CBar foo;

		foo.m_int32t = 1;
		foo.m_uint32t = 2;
		foo.m_int64t = 3;
		foo.m_uint64t = 4;
		foo.m_float = 5.f;
		foo.m_double = 6.0;
		foo.m_bool = true;
		foo.m_int32t_2 = 7;

		rapidjson::Document root;
		DONER_SERIALIZE_OBJECT_TO_JSON(foo, root)

		rapidjson::StringBuffer strbuf;
		strbuf.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		root.Accept(writer);
		std::string result(strbuf.GetString());

		ASSERT_STREQ(CBasicTypesTestInternal::FOO_JSON_DATA_INHERIT, result.c_str());
	}
}