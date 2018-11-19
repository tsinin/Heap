//
// Created by Tsinin on 27.10.2018.
//
#include <ctime>
#include <set>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Heap.h"

using testing::Eq;

namespace {
    class testFixture : public testing::Test {
    public:
        Heap<int> heap;
        Vector<int> testVector;
        std::multiset<int> testSet;
        std::vector<bool> helpVector1, helpVector2, helpVector3;
        testFixture() {
            Heap<int> obj;

            helpVector1.resize(40000, false);
            int count1 = 0;
            while(count1 < 100) {
                int x = rand() % 40000;
                if(!helpVector1[x]) {
                    helpVector1[x] = true;
                    count1++;
                }
            }
            helpVector2.resize(1000000, false);
            int count2 = 0;
            while(count2 < 100) {
                int x = rand() % 1000000;
                if(!helpVector2[x]) {
                    helpVector2[x] = true;
                    count2++;
                }
            }
            helpVector3.resize(20000000, false);
            int count3 = 0;
            while(count3 < 100) {
                int x = rand() % 20000000;
                if(!helpVector3[x]) {
                    helpVector3[x] = true;
                    count3++;
                }
            }
        }
    };
}
TEST_F(testFixture, isEmptyTest1) {
    ASSERT_TRUE(heap.is_empty());
}

TEST_F(testFixture, isEmptyTest2) {
    int t = rand();
    heap.insert(t);
    ASSERT_FALSE(heap.is_empty());
}

TEST_F(testFixture, exceptionTest1) {
    ASSERT_EXIT(heap.get_min(), ::testing::ExitedWithCode(1), "Heap is empty.");
}

TEST_F(testFixture, exceptionTest2) {
    ASSERT_EXIT(heap.extract_min(), ::testing::ExitedWithCode(1), "Heap is empty.");
}

TEST_F(testFixture, exceptionTest3) {
    heap.insert(1);
    ASSERT_EXIT(heap.optimize(1000, 10), ::testing::ExitedWithCode(2), "Heap is not empty.");
}

TEST_F(testFixture, smallTest1) {
    heap.insert(1), heap.insert(2), heap.insert(3);
    ASSERT_EQ(heap.extract_min(), 1);
    ASSERT_EQ(heap.extract_min(), 2);
    ASSERT_EQ(heap.extract_min(), 3);
}

TEST_F(testFixture, smallTest2) {
    Heap<int>::Pointer *p1 = heap.insert(1),
            *p2 = heap.insert(2),
            *p3 = heap.insert(3);
    heap.erase(p1);
    ASSERT_EQ(heap.get_min(), 2);
    heap.erase(p3);
    ASSERT_EQ(heap.get_min(), 2);
    p3 = heap.insert(3);
    heap.extract_min();
    ASSERT_EQ(heap.get_min(), 3);
}

TEST_F(testFixture, smallTest3) {
    Heap<int>::Pointer *p1 = heap.insert(1),
            *p2 = heap.insert(2);
    heap.change(p1, 3);
    ASSERT_EQ(heap.get_min(), 2);
    heap.change(p2, 4);
    ASSERT_EQ(heap.get_min(), 3);
}

TEST_F(testFixture, smallTest4) {
    Heap<int>::Pointer *p1 = heap.insert(1),
            *p2 = heap.insert(2),
            *p3 = heap.insert(3);
    heap.change(p1, 0);
    ASSERT_EQ(heap.get_min(), 0);
    heap.change(p1, 1);
    ASSERT_EQ(heap.get_min(), 1);
    heap.change(p2, 0);
    ASSERT_EQ(heap.get_min(), 0);
    heap.change(p2, 4);
    ASSERT_EQ(heap.extract_min(), 1);
    ASSERT_EQ(heap.extract_min(), 3);
    ASSERT_EQ(heap.extract_min(), 4);
}

TEST_F(testFixture, smallTest5) {
    testVector.push_back(3);
    testVector.push_back(2);
    testVector.push_back(1);
    Vector<int>::Iterator begin = testVector.begin(),
            end = testVector.end();
    Heap<int> heap(begin, end);
    ASSERT_EQ(heap.extract_min(), 1);
    ASSERT_EQ(heap.extract_min(), 2);
    ASSERT_EQ(heap.extract_min(), 3);
}
TEST_F(testFixture, smallTest6) {
    std::vector<int> testvector;
    testvector.push_back(3);
    testvector.push_back(2);
    testvector.push_back(1);
    Heap<int> heap(testvector.begin(), testvector.end());
    ASSERT_EQ(heap.extract_min(), 1);
    ASSERT_EQ(heap.extract_min(), 2);
    ASSERT_EQ(heap.extract_min(), 3);
}

TEST_F(testFixture, smallTest7) {
    heap.optimize(7, 2);
    heap.insert(3), heap.insert(4), heap.insert(1), heap.insert(5);
    ASSERT_EQ(heap.extract_min(), 1);
    heap.insert(2);
    ASSERT_EQ(heap.extract_min(), 2);
}

TEST_F(testFixture, simpleTest1) {
    for(int i = 99999; i >= 0; --i)
        heap.insert(i);
    for(int i = 0; i < 100000; ++i)
        ASSERT_EQ(i, heap.extract_min());
}

TEST_F(testFixture, simpleTest2) {
    for(int i = 0; i < 100000; ++i)
        heap.insert(i);
    for(int i = 0; i < 100000; ++i)
        ASSERT_EQ(i, heap.extract_min());
}

TEST_F(testFixture, randomSmallTest) {
    for(int i = 0; i < 40000; ++i) {
        int operation = rand() % 2;
        if(operation && !testVector.size() || testVector.size() < 5000)
            operation = 0;
        if(operation) {
            int minElementPtr = 0;
            for(int i = 1; i < testVector.size(); ++i)
                if(testVector[i] < testVector[minElementPtr])
                    minElementPtr = i;
            int minElement = testVector[minElementPtr];
            ASSERT_EQ(heap.get_min(), minElement);
            for(int i = minElementPtr; i < testVector.size() - 1; ++i)
                testVector[i] = testVector[i + 1];
            testVector.pop_back();
            heap.extract_min();
        } else {
            int x = rand();
            testVector.push_back(x);
            heap.insert(x);
        }
    }
}

TEST_F(testFixture, randomBigTest1) {
    time_t start = clock(), end;

    for(int i = 0; i < 1000000; ++i) {
        int operation = rand() % 2;
        if(operation && !testSet.size())
            operation = 0;
        if(operation) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(2, ((double)end - start) / (double)(CLOCKS_PER_SEC));
}

TEST_F(testFixture, randomBigTes2) {
    time_t start = clock(), end;

    for(int i = 0; i < 1000000; ++i) {
        int operation = rand() % 2;
        if(operation && !testSet.size() || testSet.size() < 10000)
            operation = 0;
        if(operation) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(2, ((double)end - start) / (double)(CLOCKS_PER_SEC));
}

TEST_F(testFixture, randomBigTest3) {
    time_t start = clock(), end;

    for(int i = 0; i < 1000000; ++i) {
        int operation = rand() % 2;
        if(operation && !testSet.size() || testSet.size() < 70000)
            operation = 0;
        if(operation) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(2, ((double)end - start) / (double)(CLOCKS_PER_SEC));
}

TEST_F(testFixture, mediumCorrectnessTestForOptimize) {
    heap.optimize(39900, 100);

    for(int i = 0; i < 40000; ++i) {
        if(helpVector1[i] && !heap.is_empty()) {
            int minElementPtr = 0;
            for(int i = 1; i < testVector.size(); ++i)
                if(testVector[i] < testVector[minElementPtr])
                    minElementPtr = i;
            int minElement = testVector[minElementPtr];
            ASSERT_EQ(heap.get_min(), minElement);
            for(int i = minElementPtr; i < testVector.size() - 1; ++i)
                testVector[i] = testVector[i + 1];
            testVector.pop_back();
            heap.extract_min();
        } else {
            int x = rand();
            testVector.push_back(x);
            heap.insert(x);
        }
    }
}

TEST_F(testFixture, bigCorrectnessTestForOptimize) {
    heap.optimize(999900, 100);

    for(int i = 0; i < 1000000; ++i) {
        if(helpVector2[i] && !heap.is_empty()) {
            ASSERT_EQ(heap.extract_min(), *(testSet.begin()));
            testSet.erase(testSet.begin());
        } else {
            int x = rand();
            testSet.insert(x);
            heap.insert(x);
        }
    }
}

TEST_F(testFixture, theBiggestTestForOptimize) {
    heap.optimize(19999900, 100);
    time_t start = clock(), end;
    for(int i = 0; i < 20000000; ++i) {
        if(helpVector3[i] && !heap.is_empty())
            heap.extract_min();
        else {
            int x = rand();
            heap.insert(x);
        }
    }
    end = clock();
    ASSERT_GE(5, ((double)end - start) / ((double)CLOCKS_PER_SEC));
}