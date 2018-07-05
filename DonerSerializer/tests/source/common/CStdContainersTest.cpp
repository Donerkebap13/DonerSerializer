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

namespace DonerSerializer
{
	namespace CStdContainersTestInternal
	{
		const char* const FOO_JSON_DATA = "{\"v_int32t\":[0,1,2], \"v_int32t_2\":[3,4,5]}";

		class CFoo
		{
			DONER_DECLARE_OBJECT_AS_SERIALIZABLE(CFoo)
		public:
			std::vector<int> m_vInt32t;
		};

		DONER_DEFINE_SERIALIZABLE_DATA(CFoo,
			DONER_ADD_NAMED_VAR_INFO(m_vInt32t, "v_int32t")
			
		)

		class CBar : public CFoo
		{
			DONER_DECLARE_OBJECT_AS_SERIALIZABLE(CBar)
		public:
			std::vector<int> m_vInt32t_2;
		};

		DONER_DEFINE_SERIALIZABLE_DATA(CBar,
			DONER_ADD_NAMED_VAR_INFO(m_vInt32t, "v_int32t"),
			DONER_ADD_NAMED_VAR_INFO(m_vInt32t_2, "v_int32t_2")
		)
	}

	class CStdContainersTest : public ::testing::Test
	{
	public:
		CStdContainersTest()
			: m_root(m_parser.Parse(CStdContainersTestInternal::FOO_JSON_DATA))
		{
		}

		~CStdContainersTest()
		{
		}

		rapidjson::Document m_parser;
		rapidjson::Value& m_root;

	};

	TEST_F(CStdContainersTest, deserialize_basic_types_from_main_class)
	{
		CStdContainersTestInternal::CFoo foo;

		EXPECT_EQ(0, foo.m_vInt32t.size());

		DONER_DESERIALIZE_OBJECT_REF(foo, m_root)

		EXPECT_EQ(3, foo.m_vInt32t.size());
		EXPECT_EQ(0, foo.m_vInt32t[0]);
		EXPECT_EQ(1, foo.m_vInt32t[1]);
		EXPECT_EQ(2, foo.m_vInt32t[2]);
	}

	TEST_F(CStdContainersTest, deserialize_basic_types_from_child_class)
	{
		CStdContainersTestInternal::CBar bar;

		EXPECT_EQ(0, bar.m_vInt32t.size());
		EXPECT_EQ(0, bar.m_vInt32t_2.size());

		DONER_DESERIALIZE_OBJECT_REF(bar, m_root)

		EXPECT_EQ(3, bar.m_vInt32t.size());
		EXPECT_EQ(0, bar.m_vInt32t[0]);
		EXPECT_EQ(1, bar.m_vInt32t[1]);
		EXPECT_EQ(2, bar.m_vInt32t[2]);

		EXPECT_EQ(3, bar.m_vInt32t_2.size());
		EXPECT_EQ(3, bar.m_vInt32t_2[0]);
		EXPECT_EQ(4, bar.m_vInt32t_2[1]);
		EXPECT_EQ(5, bar.m_vInt32t_2[2]);
	}
}