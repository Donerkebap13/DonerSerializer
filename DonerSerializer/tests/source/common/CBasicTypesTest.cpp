////////////////////////////////////////////////////////////
//
// MIT License
//
// DonerECS - A Tweaked Entity-Component System
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

#include <donerserializer/DonerSerializer.h>

#include <gtest/gtest.h>

namespace CBasicTypesTestInternal
{
	const char* const FOO_JSON_DATA = "{\"int32t\":1, \"uint32t\":2, \"int64t\":3, \"uint64t\":4, \"float\":5.0, \"double\":6.0, \"cstring\":\"foo\", \"bool\":true, \"int32t_2\":7}";

	class CFoo
	{
		DONER_DECLARE_OBJECT_AS_SERIALIZABLE(CFoo)
	public:
		CFoo()
			: m_int32t(0)
			, m_uint32t(0)
			, m_int64t(0)
			, m_uint64t(0)
			, m_float(0.f)
			, m_double(0.0)
			, m_cstring(nullptr)
			, m_bool(false)
		{}

		std::int32_t m_int32t;
		std::uint32_t m_uint32t;
		std::int64_t m_int64t;
		std::uint64_t m_uint64t;
		float m_float;
		double m_double;
		const char* m_cstring;
		bool m_bool;
	};

	class CBar : public CFoo
	{
		DONER_DECLARE_OBJECT_AS_SERIALIZABLE(CBar)
	public:
		CBar()
			: CFoo()
			, m_int32t_2(0)
		{}

		std::int32_t m_int32t_2;
	};
}

DONER_DEFINE_SERIALIZABLE_DATA(CBasicTypesTestInternal::CFoo,
							   DONER_ADD_NAMED_VAR_INFO(m_int32t, "int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint32t, "uint32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_int64t, "int64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint64t, "uint64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_float, "float"),
							   DONER_ADD_NAMED_VAR_INFO(m_double, "double"),
							   DONER_ADD_NAMED_VAR_INFO(m_cstring, "cstring"),
							   DONER_ADD_NAMED_VAR_INFO(m_bool, "bool")
)

DONER_DEFINE_SERIALIZABLE_DATA(CBasicTypesTestInternal::CBar,
							   DONER_ADD_NAMED_VAR_INFO(m_int32t, "int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint32t, "uint32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_int64t, "int64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_uint64t, "uint64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_float, "float"),
							   DONER_ADD_NAMED_VAR_INFO(m_double, "double"),
							   DONER_ADD_NAMED_VAR_INFO(m_cstring, "cstring"),
							   DONER_ADD_NAMED_VAR_INFO(m_bool, "bool"),
							   DONER_ADD_NAMED_VAR_INFO(m_int32t_2, "int32t_2")
)

namespace DonerSerializer
{
	class CBasicTypesTest : public ::testing::Test
	{
	public:
		CBasicTypesTest()
			: m_root(m_parser.Parse(CBasicTypesTestInternal::FOO_JSON_DATA))
		{
		}

		~CBasicTypesTest()
		{
		}

		rapidjson::Document m_parser;
		rapidjson::Value& m_root;

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
		EXPECT_EQ(nullptr, foo.m_cstring);
		EXPECT_FALSE(foo.m_bool);

		DONER_DESERIALIZE_OBJECT_REF(foo, m_root)

		EXPECT_EQ(1, foo.m_int32t);
		EXPECT_EQ(2, foo.m_uint32t);
		EXPECT_EQ(3, foo.m_int64t);
		EXPECT_EQ(4, foo.m_uint64t);
		EXPECT_EQ(5.f, foo.m_float);
		EXPECT_EQ(6.0, foo.m_double);
		EXPECT_EQ(0, strcmp("foo", foo.m_cstring));
		EXPECT_TRUE(foo.m_bool);
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
		EXPECT_EQ(nullptr, bar.m_cstring);
		EXPECT_FALSE(bar.m_bool);
		EXPECT_EQ(0, bar.m_int32t_2);

		DONER_DESERIALIZE_OBJECT_REF(bar, m_root)

		EXPECT_EQ(1, bar.m_int32t);
		EXPECT_EQ(2, bar.m_uint32t);
		EXPECT_EQ(3, bar.m_int64t);
		EXPECT_EQ(4, bar.m_uint64t);
		EXPECT_EQ(5.f, bar.m_float);
		EXPECT_EQ(6.0, bar.m_double);
		EXPECT_EQ(0, strcmp("foo", bar.m_cstring));
		EXPECT_TRUE(bar.m_bool);
		EXPECT_EQ(7, bar.m_int32t_2);
	}
}