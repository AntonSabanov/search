#include "pch.h"
#include "../Dictionary/Dictionary.cpp"

class TestClass
{
public:
	std::string name;
	int id;
	float number;

	~TestClass()
	{
		id = 0;
		number = 0.0f;
	}
};

TEST(TestCaseName, TestName) 
{
	//Dictionary<int, int> test;
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(Put_Test, IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	EXPECT_TRUE(test.Contains(1));
	EXPECT_TRUE(test.Contains(2));
	EXPECT_TRUE(test.Contains(3));
}

TEST(Size_Test, IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	EXPECT_EQ(test.Size(), 3);
}

TEST(Put_Test, StrKey_IntVal)
{
	Dictionary<std::string, int> test;
	test.Put("abs", 10);
	test.Put("cbs", 11);
	test.Put("bbs", 12);
	EXPECT_TRUE(test.Contains("abs"));
	EXPECT_TRUE(test.Contains("cbs"));
	EXPECT_TRUE(test.Contains("bbs"));
}

TEST(Contains_Test, IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	test.Put(0, 13);
	EXPECT_TRUE(test.Contains(1));
	EXPECT_TRUE(test.Contains(2));
	EXPECT_TRUE(test.Contains(3));
	EXPECT_TRUE(test.Contains(0));
	EXPECT_FALSE(test.Contains(5));
	EXPECT_FALSE(test.Contains(4));
	EXPECT_FALSE(test.Contains(6));
}

TEST(Contains_Test, IntKey_ClassVal)
{
	Dictionary<int, TestClass> test;
	TestClass testElement1 = { "test1", 4, 0.2f };
	TestClass testElement2 = { "test1", 7, 3.567f };
	TestClass testElement3 = { "test1", 2, 3.5f };
	test.Put(1, testElement1);
	test.Put(3, testElement3);
	test.Put(2, testElement2);
	EXPECT_TRUE(test.Contains(1));
	EXPECT_TRUE(test.Contains(2));
	EXPECT_TRUE(test.Contains(3));
	EXPECT_EQ(test[1].id, 4);
	EXPECT_EQ(test[2].id, 7);
	EXPECT_EQ(test[3].id, 2);
	test.Remove(3);
}

TEST(Remove_Test, IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 101);
	test.Put(3, 103);
	test.Put(2, 102);
	test.Put(0, 100);
	test.Put(10, 110);
	test.Put(6, 106);
	EXPECT_TRUE(test.Contains(0));
	EXPECT_TRUE(test.Contains(1));
	EXPECT_TRUE(test.Contains(2));
	EXPECT_TRUE(test.Contains(3));
	EXPECT_TRUE(test.Contains(6));
	EXPECT_TRUE(test.Contains(10));
	test.Remove(0);
	test.Remove(3);
	test.Remove(10);
	test.Remove(6);	
	test.Put(7, 107);
	EXPECT_FALSE(test.Contains(0));
	EXPECT_TRUE(test.Contains(1));
	EXPECT_TRUE(test.Contains(2));
	EXPECT_FALSE(test.Contains(3));
	EXPECT_FALSE(test.Contains(6));
	EXPECT_FALSE(test.Contains(10));
	EXPECT_TRUE(test.Contains(7));
}

TEST(Operator_Test, IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	test.Put(0, 13);
	EXPECT_EQ(test[2], 12);
	EXPECT_EQ(test[1], 10);
	EXPECT_EQ(test[0], 13);
	EXPECT_EQ(test[3], 11);
}

TEST(Iterator_Test, Key_IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	test.Put(0, 13);
	auto it = test.GetIterator();
	EXPECT_EQ(it.Key(), 0);
}

TEST(Iterator_Test, Get_IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	auto it = test.GetIterator();
	EXPECT_EQ(it.Get(), 10);
}

TEST(Iterator_Test, Set_IntKey_IntVal)
{
	Dictionary<int, int> test;
	test.Put(1, 10);
	test.Put(3, 11);
	test.Put(2, 12);
	auto it = test.GetIterator();
	it.Set(100);
	EXPECT_EQ(test[1], 100);
}

