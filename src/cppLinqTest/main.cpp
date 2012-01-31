// Copyright 2012 Paolo Severini
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "stdafx.h"

#include "../cpplinqunittest/aggregateTest.cpp"
#include "../cpplinqunittest/allTest.cpp"
#include "../cpplinqunittest/anyTest.cpp"
#include "../cpplinqunittest/averageTest.cpp"
#include "../cpplinqunittest/concatTest.cpp"
#include "../cpplinqunittest/containsTest.cpp"
#include "../cpplinqunittest/countTest.cpp"
#include "../cpplinqunittest/distinctTest.cpp"
#include "../cpplinqunittest/elementAtTest.cpp"
#include "../cpplinqunittest/emptyTest.cpp"
#include "../cpplinqunittest/exceptTest.cpp"
#include "../cpplinqunittest/firstTest.cpp"
#include "../cpplinqunittest/lastTest.cpp"
#include "../cpplinqunittest/longCountTest.cpp"
#include "../cpplinqunittest/maxTest.cpp"
#include "../cpplinqunittest/minTest.cpp"
#include "../cpplinqunittest/rangeTest.cpp"
#include "../cpplinqunittest/repeatTest.cpp"
#include "../cpplinqunittest/reverseTest.cpp"
#include "../cpplinqunittest/selectManyTest.cpp"
#include "../cpplinqunittest/selectTest.cpp"
#include "../cpplinqunittest/singleTest.cpp"
#include "../cpplinqunittest/skipTest.cpp"
#include "../cpplinqunittest/sumTest.cpp"
#include "../cpplinqunittest/takeTest.cpp"
#include "../cpplinqunittest/unionTest.cpp"
#include "../cpplinqunittest/whereTest.cpp"
using namespace UnitTest;

void test()
{
    AggregateTest::test();
    AllTest::test();
    AnyTest::test();
    AverageTest::test();
    ConcatTest::test();
    ContainsTest::test();
    CountTest::test();
    DistinctTest::test();
    ElementAtTest::test();
    ElementAtOrDefaultTest::test();
    EmptyTest::test();
    ExceptTest::test();
    FirstTest::test();
    FirstOrDefaultTest::test();
    LastTest::test();
    LastOrDefaultTest::test();
    LongCountTest::test();
    MaxTest::test();
    MinTest::test();
    RangeTest::test();
    RepeatTest::test();
    ReverseTest::test();
    SelectTest::test();
    SelectManyTest::test();
    SingleTest::test();
    SingleOrDefaultTest::test();
    SkipTest::test();
    SkiWhileTest::test();
    SumTest::test();
    TakeTest::test();
    TakeWhileTest::test();
    UnionTest::test();
    WhereTest::test();
}

int _tmain(int argc, _TCHAR* argv[])
{
    Fiber::enableFibersInCurrentThread();
    test();
}