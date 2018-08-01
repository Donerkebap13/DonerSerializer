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
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <gtest/gtest.h>

#include <map>
#include <unordered_map>
#include <vector>

namespace CStdContainersTestInternal
{
	const char* const FOO_JSON_DATA = "{\"v_map\":[[0,false],[1,true],[2,false]],\"v_vector\":[[0,1,2],[3,4,5],[6,7,8]],\"v_bool\":[true,false,true],\"v_string\":[\"zero\",\"one\",\"two\"],\"v_double\":[0.0,1.0,2.0],\"v_float\":[0.0,1.0,2.0],\"v_uint64t\":[0,1,2],\"v_int64t\":[0,1,2],\"v_uint32t\":[0,1,2],\"v_int32t\":[0,1,2]}";
	const char* const FOO_JSON_DATA_INHERIT = "{\"v_int32t_2\":[3,4,5],\"v_int32t\":[0,1,2]}";

	class CFoo : public DonerReflection::ISerializable
	{
		DONER_DECLARE_OBJECT_AS_SERIALIZABLE(CFoo)
	public:
		std::vector<std::int32_t> m_vInt32t;
		std::vector<std::uint32_t> m_vUint32t;
		std::vector<std::int64_t> m_vInt64t;
		std::vector<std::uint64_t> m_vUint64t;
		std::vector<float> m_vFloat;
		std::vector<double> m_vDouble;
		std::vector<std::string> m_vString;
		std::vector<bool> m_vBool;

		std::vector<std::vector<std::int32_t>> m_vVector;

		std::map<std::int32_t, bool> m_map;
	};

	class CBar : public CFoo
	{
		DONER_DECLARE_OBJECT_AS_SERIALIZABLE(CBar)
	public:
		std::vector<std::int32_t> m_vInt32t_2;
	};
}

DONER_DEFINE_SERIALIZABLE_DATA(CStdContainersTestInternal::CBar,
							   DONER_ADD_NAMED_VAR_INFO(m_vInt32t, "v_int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_vInt32t_2, "v_int32t_2")
)

DONER_DEFINE_SERIALIZABLE_DATA(CStdContainersTestInternal::CFoo,
							   DONER_ADD_NAMED_VAR_INFO(m_vInt32t, "v_int32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_vUint32t, "v_uint32t"),
							   DONER_ADD_NAMED_VAR_INFO(m_vInt64t, "v_int64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_vUint64t, "v_uint64t"),
							   DONER_ADD_NAMED_VAR_INFO(m_vFloat, "v_float"),
							   DONER_ADD_NAMED_VAR_INFO(m_vDouble, "v_double"),
							   DONER_ADD_NAMED_VAR_INFO(m_vString, "v_string"),
							   DONER_ADD_NAMED_VAR_INFO(m_vBool, "v_bool"),
							   DONER_ADD_NAMED_VAR_INFO(m_vVector, "v_vector"),
							   DONER_ADD_NAMED_VAR_INFO(m_map, "v_map")
)

namespace DonerSerializer
{
	class CStdContainersTest : public ::testing::Test
	{
	public:
		CStdContainersTest() = default;
		~CStdContainersTest() = default;
	};

	TEST_F(CStdContainersTest, deserialize_vector_from_main_class)
	{
		CStdContainersTestInternal::CFoo foo;

		EXPECT_EQ(0, foo.m_vInt32t.size());

		rapidjson::Document parser;
		rapidjson::Value& root = parser.Parse(CStdContainersTestInternal::FOO_JSON_DATA);

		DONER_DESERIALIZE_OBJECT(foo, root)

		EXPECT_EQ(3, foo.m_vInt32t.size());
		EXPECT_EQ(0, foo.m_vInt32t[0]);
		EXPECT_EQ(1, foo.m_vInt32t[1]);
		EXPECT_EQ(2, foo.m_vInt32t[2]);

		EXPECT_EQ(3, foo.m_vUint32t.size());
		EXPECT_EQ(0, foo.m_vUint32t[0]);
		EXPECT_EQ(1, foo.m_vUint32t[1]);
		EXPECT_EQ(2, foo.m_vUint32t[2]);

		EXPECT_EQ(3, foo.m_vInt64t.size());
		EXPECT_EQ(0, foo.m_vInt64t[0]);
		EXPECT_EQ(1, foo.m_vInt64t[1]);
		EXPECT_EQ(2, foo.m_vInt64t[2]);

		EXPECT_EQ(3, foo.m_vUint64t.size());
		EXPECT_EQ(0, foo.m_vUint64t[0]);
		EXPECT_EQ(1, foo.m_vUint64t[1]);
		EXPECT_EQ(2, foo.m_vUint64t[2]);

		EXPECT_EQ(3, foo.m_vFloat.size());
		EXPECT_EQ(0.f, foo.m_vFloat[0]);
		EXPECT_EQ(1.f, foo.m_vFloat[1]);
		EXPECT_EQ(2.f, foo.m_vFloat[2]);

		EXPECT_EQ(3, foo.m_vDouble.size());
		EXPECT_EQ(0.0, foo.m_vDouble[0]);
		EXPECT_EQ(1.0, foo.m_vDouble[1]);
		EXPECT_EQ(2.0, foo.m_vDouble[2]);

		EXPECT_EQ(3, foo.m_vString.size());
		ASSERT_STREQ("zero", foo.m_vString[0].c_str());
		ASSERT_STREQ("one", foo.m_vString[1].c_str());
		ASSERT_STREQ("two", foo.m_vString[2].c_str());

		EXPECT_EQ(3, foo.m_vBool.size());
		EXPECT_TRUE(foo.m_vBool[0]);
		EXPECT_FALSE(foo.m_vBool[1]);
		EXPECT_TRUE(foo.m_vBool[2]);

		EXPECT_EQ(3, foo.m_vVector.size());

		EXPECT_EQ(0, foo.m_vVector[0][0]);
		EXPECT_EQ(1, foo.m_vVector[0][1]);
		EXPECT_EQ(2, foo.m_vVector[0][2]);

		EXPECT_EQ(3, foo.m_vVector[1][0]);
		EXPECT_EQ(4, foo.m_vVector[1][1]);
		EXPECT_EQ(5, foo.m_vVector[1][2]);

		EXPECT_EQ(6, foo.m_vVector[2][0]);
		EXPECT_EQ(7, foo.m_vVector[2][1]);
		EXPECT_EQ(8, foo.m_vVector[2][2]);

		EXPECT_FALSE(foo.m_map[0]);
		EXPECT_TRUE(foo.m_map[1]);
		EXPECT_FALSE(foo.m_map[2]);
	}

	TEST_F(CStdContainersTest, deserialize_vector_from_child_class)
	{
		CStdContainersTestInternal::CBar bar;

		EXPECT_EQ(0, bar.m_vInt32t.size());
		EXPECT_EQ(0, bar.m_vInt32t_2.size());

		rapidjson::Document parser;
		rapidjson::Value& root = parser.Parse(CStdContainersTestInternal::FOO_JSON_DATA_INHERIT);

		DONER_DESERIALIZE_OBJECT(bar, root)

		EXPECT_EQ(3, bar.m_vInt32t.size());
		EXPECT_EQ(0, bar.m_vInt32t[0]);
		EXPECT_EQ(1, bar.m_vInt32t[1]);
		EXPECT_EQ(2, bar.m_vInt32t[2]);

		EXPECT_EQ(3, bar.m_vInt32t_2.size());
		EXPECT_EQ(3, bar.m_vInt32t_2[0]);
		EXPECT_EQ(4, bar.m_vInt32t_2[1]);
		EXPECT_EQ(5, bar.m_vInt32t_2[2]);
	}

	TEST_F(CStdContainersTest, serialize_vector_from_main_class)
	{
		CStdContainersTestInternal::CFoo foo;

		foo.m_vInt32t.push_back(0);
		foo.m_vInt32t.push_back(1);
		foo.m_vInt32t.push_back(2);

		foo.m_vUint32t.push_back(0);
		foo.m_vUint32t.push_back(1);
		foo.m_vUint32t.push_back(2);

		foo.m_vInt64t.push_back(0L);
		foo.m_vInt64t.push_back(1L);
		foo.m_vInt64t.push_back(2L);

		foo.m_vUint64t.push_back(0UL);
		foo.m_vUint64t.push_back(1UL);
		foo.m_vUint64t.push_back(2UL);

		foo.m_vFloat.push_back(0.f);
		foo.m_vFloat.push_back(1.f);
		foo.m_vFloat.push_back(2.f);

		foo.m_vDouble.push_back(0.0);
		foo.m_vDouble.push_back(1.0);
		foo.m_vDouble.push_back(2.0);

		foo.m_vString.push_back("zero");
		foo.m_vString.push_back("one");
		foo.m_vString.push_back("two");

		foo.m_vBool.push_back(true);
		foo.m_vBool.push_back(false);
		foo.m_vBool.push_back(true);

		foo.m_map[0] = false;
		foo.m_map[1] = true;
		foo.m_map[2] = false;

		foo.m_vVector.push_back({ 0, 1, 2 });
		foo.m_vVector.push_back({ 3, 4, 5 });
		foo.m_vVector.push_back({ 6, 7, 8 });

		rapidjson::Document root;
		root.SetObject();

		DONER_SERIALIZE_OBJECT(foo, root)

		rapidjson::StringBuffer strbuf;
		strbuf.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		root.Accept(writer);
		std::string result(strbuf.GetString());

		ASSERT_STREQ(CStdContainersTestInternal::FOO_JSON_DATA, result.c_str());
	}

	TEST_F(CStdContainersTest, serialize_vector_from_child_class)
	{
		CStdContainersTestInternal::CBar bar;

		bar.m_vInt32t.push_back(0);
		bar.m_vInt32t.push_back(1);
		bar.m_vInt32t.push_back(2);

		bar.m_vInt32t_2.push_back(3);
		bar.m_vInt32t_2.push_back(4);
		bar.m_vInt32t_2.push_back(5);

		rapidjson::Document root;
		root.SetObject();

		DONER_SERIALIZE_OBJECT(bar, root)

		rapidjson::StringBuffer strbuf;
		strbuf.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		root.Accept(writer);
		std::string result(strbuf.GetString());

		ASSERT_STREQ(CStdContainersTestInternal::FOO_JSON_DATA_INHERIT, result.c_str());
	}
}