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

TEST(HashMapTests, TestIterator) {
    HashMap<int, int> map0([](int key) { return key; });

    map0.put(0, 5);
    map0.put(2, -56);

    Iterator<KeyValuePair<int, int>&> itr0 = map0.begin();

    ASSERT_TRUE(itr0 != map0.end());
    ASSERT_EQ((*itr0).key, 0);
    ASSERT_EQ((*itr0).value, 5);
    ++itr0;
    ASSERT_TRUE(itr0 != map0.end());
    ASSERT_EQ((*itr0).key, 2);
    ASSERT_EQ((*itr0).value, -56);
    ++itr0;
    ASSERT_FALSE(itr0 != map0.end());

    HashMap<int, int> map1([](int key) { return key; });
    map1.put(2, -56);

    Iterator<KeyValuePair<int, int>&> itr1 = map1.begin();
    ASSERT_TRUE(itr1 != map1.end());
    ASSERT_EQ((*itr1).key, 2);
    ASSERT_EQ((*itr1).value, -56);
    ++itr1;
    ASSERT_FALSE(itr1 != map1.end());

    HashMap<int, int> map2([](int key) { return key; });
    Iterator<KeyValuePair<int, int>&> itr2 = map2.begin();
    ASSERT_FALSE(itr1 != map1.end());

    HashMap<int, int> map3([](int key) { return key; });
    map3.put(1, 2);
    Iterator<KeyValuePair<int, int>&> itr3 = map3.begin();

    for(KeyValuePair<int, int> cur : map3) {
        ASSERT_EQ(cur.key, 1);
        ASSERT_EQ(cur.value, 2);
        cur.value = 7;
    }

    ASSERT_EQ(map3.get(1), 2);
}

TEST(HashMapTests, TestEquality) {
    HashMap<int, int> map0([](int key) { return key; });
    HashMap<int, int> map1([](int key) { return key; });

    ASSERT_TRUE(map0 == map1);

    map0.put(15, 50);
    ASSERT_FALSE(map0 == map1);

    map1.put(15, 50);
    ASSERT_TRUE(map0 == map1);

    map0.put(0, 11);
    map1.put(0, 12);
    ASSERT_FALSE(map0 == map1);

    map1.put(0, 11);
    ASSERT_TRUE(map0 == map1);
}