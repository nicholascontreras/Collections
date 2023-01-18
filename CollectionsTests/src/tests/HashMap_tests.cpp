#include "pch.h"

#include "../util.h"

#include "HashMap.h"

TEST(HashMapTests, TestPut) {
    HashMap<int, bool> map0([](int key) { return key; });

    map0.put(1, true);
    map0.put(2, false);
    map0.put(8, true);

    ASSERT_EQ(map0.get(1), true);
    ASSERT_EQ(map0.get(2), false);
    ASSERT_EQ(map0.get(8), true);

    ASSERT_THROW(map0.get(3), NoSuchItemException);

    ASSERT_EQ(map0.getSize(), 3);

    map0.put(8, false);

    ASSERT_EQ(map0.get(8), false);
}

TEST(HashMapTests, TestContainsKey) {
    HashMap<int, int> map0([](int key) { return key; });

    map0.put(0, 5);
    map0.put(16, 5);

    ASSERT_EQ(map0.getSize(), 2);
    ASSERT_EQ(map0.containsKey(0), true);
    ASSERT_EQ(map0.containsKey(16), true);
    ASSERT_EQ(map0.containsKey(1), false);
    ASSERT_EQ(map0.containsKey(17), false);
    ASSERT_EQ(map0.containsKey(15), false);
    ASSERT_EQ(map0.containsKey(5), false);
    ASSERT_EQ(map0.containsKey(-1), false);
}