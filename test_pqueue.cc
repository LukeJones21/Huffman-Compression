#include <gtest/gtest.h>

#include <functional>
#include <iostream>

#include "pqueue.h"

TEST(PQueue, less) {
PQueue<int> pq;

pq.Push(42);
pq.Push(23);
pq.Push(2);
pq.Push(34);

EXPECT_EQ(pq.Top(), 2);
EXPECT_EQ(pq.Size(), 4);
pq.Pop();
EXPECT_EQ(pq.Top(), 23);
}

TEST(PQueue, great) {
PQueue<int, std::greater<int>> pq;

pq.Push(42);
pq.Push(23);
pq.Push(2);
pq.Push(34);

EXPECT_EQ(pq.Top(), 42);
EXPECT_EQ(pq.Size(), 4);
pq.Pop();
EXPECT_EQ(pq.Top(), 34);
}

class MyClass {
public:
    explicit MyClass(int n) : n_(n) {}
    bool operator < (const MyClass &mc) const { return n_ < mc.n_; }
    int n() { return n_; }
private:
    int n_;
};

TEST(PQueue, custom_class) {
std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

PQueue<MyClass> pq;
pq.Push(vec[0]);
pq.Push(vec[1]);
pq.Push(vec[2]);
pq.Push(vec[3]);

EXPECT_EQ(pq.Top().n(), vec[2].n());
EXPECT_EQ(pq.Size(), 4);
pq.Pop();
EXPECT_EQ(pq.Top().n(), vec[1].n());
}

TEST(PQueue, ErrorThrow) {
    PQueue<int> pq;

    EXPECT_THROW(pq.Top(), std::exception);
    EXPECT_THROW(pq.Pop(), std::exception);
    pq.Push(1);
    pq.Push(5);
    pq.Push(10);
    pq.Push(6);
    EXPECT_EQ(pq.Top(), 1);
    EXPECT_EQ(pq.Size(), 4);
    pq.Pop();
    EXPECT_EQ(pq.Top(), 5);
}

class MyClassCompare {
  public:
    bool operator()(MyClass* a, MyClass* b) {
        return a->n() < b->n();
    }
};

TEST(PQueue, custom_class_pointer) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34)};

  PQueue<MyClass*, MyClassCompare> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);
  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);
}


int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
