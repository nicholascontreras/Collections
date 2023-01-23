#include "pch.h"

#include "../util.h"
#include "mock_data.h"

#include "ArrayList.h"

TEST(ArrayListTests, TestAdd) {
    ArrayList<int> list0;

    for(int i = 0; i < 1000; i++) {
        list0.add(i);
    }

    for(int i = 0; i < 1000; i++) {
        ASSERT_EQ(list0.get(i), i);
    }
}

TEST(ArrayListTests, TestOutOfBounds) {
    ArrayList<double> list0;

    ASSERT_THROW(list0.get(-5), IndexOutOfBoundsException);
    ASSERT_THROW(list0.get(0), IndexOutOfBoundsException);

    list0.add(12.4);

    ASSERT_NO_THROW(list0.get(0));
    ASSERT_THROW(list0.get(1), IndexOutOfBoundsException);
}

TEST(ArrayListTests, TestIterator) {
    ArrayList<int> list0;

    list0.add(5);
    list0.add(12);
    list0.add(56);

    int array[3] = { 5, 12, 56 };

    Iterator<int&> itr0 = list0.begin();
    for(int i = 0; i < 3; i++) {
        ASSERT_EQ(*itr0, array[i]);
        ++itr0;
    }
    ASSERT_FALSE(itr0 != list0.end());

    int index0 = 0;
    for(int x : list0) {
        ASSERT_EQ(x, array[index0]);
        x = 55;
        index0++;
    }
    ASSERT_EQ(index0, 3);

    int index1 = 0;
    for(const int& x : list0) {
        ASSERT_EQ(x, array[index1]);
        index1++;
    }
    ASSERT_EQ(index1, 3);

    for(int& x : list0) {
        x = -5;
    }

    int index2 = 0;
    for(const int& x : list0) {
        ASSERT_EQ(x, -5);
        index2++;
    }
    ASSERT_EQ(index2, 3);

    ArrayList<int> list1 = { 55, 128 };
    Iterator<int&> itr1 = list1.begin();

    ASSERT_THROW(itr0 != itr1, UnrelatedIteratorException);
    ASSERT_THROW(list1.end() != itr0, UnrelatedIteratorException);
    ASSERT_TRUE(itr1 != list1.end());

    ArrayList<int> list2 = {};
    ASSERT_FALSE(list2.begin() != list2.end());
}

TEST(ArrayListTests, TestConcurrentModification) {
    ArrayList<int> list0;

    list0.add(5);
    list0.add(12);
    list0.add(56);

    ASSERT_THROW(
        int index = 0;
        for(int x : list0) {
            UNUSED(x);

            if(index == 1) {
                list0.add(11);
            }
            index++;
        }, ConcurrentModificationException);

    ASSERT_NO_THROW(
        int index = 0;
        for(int x : list0) {
            UNUSED(x);

            if(index == 2) {
                list0.add(12);
                break;
            }
            index++;
        });

    ASSERT_EQ(list0.getSize(), 5);
}

TEST(ArrayListTests, TestInitializerList) {
    ArrayList<int> list0 = { 1, 2, 3, 4, 5, 6 };
    ASSERT_EQ(list0.getSize(), 6);

    for(int i = 0; i < list0.getSize(); i++) {
        ASSERT_EQ(list0.get(i), i + 1);
    }
}

TEST(ArrayListTests, TestEquality) {
    ArrayList<int> list0 = { 1, 2, 3, 4, 5, 6 };
    ArrayList<int> list1 = { 1, 2, 3, 4, 5, 6 };
    ArrayList<int> list2 = { 1, 2, 3, 4, 5, 7 };
    ArrayList<int> list3 = { 1, 2, 3, 4, 5, 6, 7 };
    ArrayList<int> list4 = {};

    ASSERT_EQ(list0, list1);
    ASSERT_EQ(list1, list0);
    ASSERT_NE(list0, list2);
    ASSERT_NE(list0, list3);
    ASSERT_NE(list3, list4);
}

TEST(ArrayListTests, TestInsert) {
    ArrayList<int> list0;
    list0.insert(15, 0);
    list0.insert(12, 0);

    ASSERT_EQ(list0, ArrayList<int>({ 12, 15 }));

    list0.insert(55, 1);
    ASSERT_EQ(list0, ArrayList<int>({ 12, 55, 15 }));

    list0.insert(55, 1);
    ASSERT_EQ(list0, ArrayList<int>({ 12, 55, 55, 15 }));

    list0.insert(-14, 3);
    ASSERT_EQ(list0, ArrayList<int>({ 12, 55, 55, -14, 15 }));

    list0.insert(0, 5);
    ASSERT_EQ(list0, ArrayList<int>({ 12, 55, 55, -14, 15, 0 }));

    ASSERT_THROW(list0.insert(12, 7), IndexOutOfBoundsException);
    ASSERT_THROW(list0.insert(12, -1), IndexOutOfBoundsException);
}

TEST(ArrayListTests, TestSet) {
    ArrayList<bool> list0 = {true, false, true};

    list0.set(false, 0);
    ASSERT_EQ(list0, ArrayList<bool>({false, false, true}));

    list0.set(false, 2);
    ASSERT_EQ(list0, ArrayList<bool>({false, false, false}));

    ASSERT_THROW(list0.set(true, 3), IndexOutOfBoundsException);
    ASSERT_THROW(list0.set(true, -1), IndexOutOfBoundsException);
}

TEST(ArrayListTests, TestCopy) {
    ArrayList<int> list0 = { 1, 2, 3 };
    ArrayList<int> list1 = list0;

    ASSERT_EQ(list0, list1);

    list1.set(44, 1);
    ASSERT_NE(list0, list1);

    list0 = list1;
    ASSERT_EQ(list0, list1);
}

TEST(ArrayListTests, TestMove) {
    ArrayList<int> list0 = { 1, 2, 3 };
    ArrayList<int> list1 = std::move(list0);

    ASSERT_EQ(list0.getSize(), 0);
    ASSERT_EQ(list1.getSize(), 3);
    
    list0.add(5);
    ASSERT_EQ(list0.getSize(), 1);
    ASSERT_EQ(list1.getSize(), 3);

    list0 = std::move(list1);
    ASSERT_EQ(list0.getSize(), 3);
    ASSERT_EQ(list1.getSize(), 1);

    list1.add(15);
    ASSERT_EQ(list0, ArrayList<int>({ 1, 2, 3 }));
    ASSERT_EQ(list1, ArrayList<int>({ 5, 15 }));
}

TEST(ArrayListTests, TestConstList) {
    const ArrayList<int> list0 = { 1, 2, 3 };
    Iterator<const int&> itr0 = list0.begin();
    int sum = 0;
    while(itr0 != list0.end()) {
        sum += *itr0;
        ++itr0;
    }
    ASSERT_EQ(sum, 6);

    const int& x = list0.get(1);
    ASSERT_EQ(x, 2);
}

TEST(ArrayListTests, TestCallFunction) {
    ArrayList<ClassWithFunction> list0;
    list0.add(ClassWithFunction());

    EXPECT_CALL(list0.get(0), testFunction()).Times(1);

    for(ClassWithFunction& cur : list0) {
        cur.testFunction();
    }
}