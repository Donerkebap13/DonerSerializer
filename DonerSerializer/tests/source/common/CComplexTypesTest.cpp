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

#include <donerserializer/DonerSerialize.h>
#include <donerserializer/DonerDeserialize.h>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <gtest/gtest.h>

#include <vector>
#include <map>
#include <string>

namespace CComplexTypesTestInternal
{
	const char* const FOO_JSON_DATA = "{\"map\":[[1,{\"basic\":{\"bool\":true,\"float\":3.0,\"int32t\":2}}],[7,{\"basic\":{\"bool\":false,\"float\":2.0,\"int32t\":1}}]],\"vector\":[{\"basic\":{\"bool\":false,\"float\":2.0,\"int32t\":1}},{\"basic\":{\"bool\":true,\"float\":3.0,\"int32t\":2}}]}";

	class CBasic : public DonerSerializer::ISerializable
	{
		DONER_DECLARE_OBJECT_AS_REFLECTABLE(CBasic)
	public:
		CBasic()
			: m_int32t(0)
			, m_float(0.f)
			, m_bool(false)
		{}

		CBasic(std::int32_t int32t, float _float, bool _bool)
			: m_int32t(int32t)
			, m_float(_float)
			, m_bool(_bool)
		{}

		std::int32_t m_int32t;
		float m_float;
		bool m_bool;
	};

	class CFoo : public DonerSerializer::ISerializable
	{
		DONER_DECLARE_OBJECT_AS_REFLECTABLE(CFoo)
	public:
		CFoo()
		{}

		CBasic m_basic;
	};

	class CBar
	{
		DONER_DECLARE_OBJECT_AS_REFLECTABLE(CBar)
	public:
		CBar()
		{}

		std::vector<CFoo> m_vector;
		std::map<std::uint64_t, CFoo> m_map;
	};
}

DONER_DEFINE_REFLECTION_DATA(CComplexTypesTestInternal::CBasic,
							   DONER_ADD_NAMED_VAR_INFO(m_int32t, "int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_float, "float"),
							   DONER_ADD_NAMED_VAR_INFO(m_bool, "bool")
)

DONER_DEFINE_REFLECTION_DATA(CComplexTypesTestInternal::CFoo,
							   DONER_ADD_NAMED_VAR_INFO(m_basic, "basic")
)

DONER_DEFINE_REFLECTION_DATA(CComplexTypesTestInternal::CBar,
							   DONER_ADD_NAMED_VAR_INFO(m_vector, "vector"),
							   DONER_ADD_NAMED_VAR_INFO(m_map, "map")
)

namespace DonerReflection
{
	class CComplexTypesTest : public ::testing::Test
	{
	public:
		CComplexTypesTest() = default;
		~CComplexTypesTest() = default;
	};

	TEST_F(CComplexTypesTest, deserialize_complex_type)
	{
		CComplexTypesTestInternal::CBar bar;

		rapidjson::Document parser;
		rapidjson::Value& root = parser.Parse(CComplexTypesTestInternal::FOO_JSON_DATA);

		DONER_DESERIALIZE_OBJECT_FROM_JSON(bar, root)

		EXPECT_EQ(2U, bar.m_vector.size());

		EXPECT_EQ(1, bar.m_vector[0].m_basic.m_int32t);
		EXPECT_EQ(2.f, bar.m_vector[0].m_basic.m_float);
		EXPECT_FALSE(bar.m_vector[0].m_basic.m_bool);

		EXPECT_EQ(2, bar.m_vector[1].m_basic.m_int32t);
		EXPECT_EQ(3.f, bar.m_vector[1].m_basic.m_float);
		EXPECT_TRUE(bar.m_vector[1].m_basic.m_bool);

		EXPECT_EQ(2U, bar.m_map.size());

		auto it = bar.m_map.find(1);
		EXPECT_TRUE(it != bar.m_map.end());

		it = bar.m_map.find(7);
		EXPECT_TRUE(it != bar.m_map.end());

		EXPECT_EQ(1, bar.m_map[7].m_basic.m_int32t);
		EXPECT_EQ(2.f, bar.m_map[7].m_basic.m_float);
		EXPECT_FALSE(bar.m_map[7].m_basic.m_bool);

		EXPECT_EQ(2, bar.m_map[1].m_basic.m_int32t);
		EXPECT_EQ(3.f, bar.m_map[1].m_basic.m_float);
		EXPECT_TRUE(bar.m_map[1].m_basic.m_bool);
	}

	TEST_F(CComplexTypesTest, serialize_complex_type)
	{
		CComplexTypesTestInternal::CBasic b1 { 1, 2.f, false };
		CComplexTypesTestInternal::CBasic b2 { 2, 3.f, true };

		CComplexTypesTestInternal::CFoo foo1;
		foo1.m_basic = b1;

		CComplexTypesTestInternal::CFoo foo2;
		foo2.m_basic = b2;

		CComplexTypesTestInternal::CBar bar;

		bar.m_vector.push_back(foo1);
		bar.m_vector.push_back(foo2);

		bar.m_map[1] = foo2;
		bar.m_map[7] = foo1;

		rapidjson::Document root;
		DONER_SERIALIZE_OBJECT_TO_JSON(bar, root)

		rapidjson::StringBuffer strbuf;
		strbuf.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		root.Accept(writer);
		std::string pepe(strbuf.GetString());

		ASSERT_STREQ(CComplexTypesTestInternal::FOO_JSON_DATA, pepe.c_str());
	}
}