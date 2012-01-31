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
#include "CppUnitTest.h"

#include "testUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
    TEST_CLASS(LongCountTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "longCount\n");

            LongCountTest t;
            t.LongCount_NonPredicatedCount();
            t.LongCount_PredicatedCount();
        }

        TEST_METHOD(LongCount_NonPredicatedCount)
        {
            Assert::AreEqual((__int64)5, IEnumerable<int>::Range(2, 5)->LongCount());
        }

        TEST_METHOD(LongCount_PredicatedCount)
        {
            // Counts even numbers within 2, 3, 4, 5, 6
            Assert::AreEqual((__int64)3, IEnumerable<int>::Range(2, 5)->LongCount([](int x){
                return (x % 2 == 0);
            }));
        }

        // Ignore("Takes an enormous amount of time!")
        //TEST_METHOD(LongCount_CollectionBiggerThanIntMaxCanBeCountedWithLongCount)
        //{
        //    auto hugeCollection = IEnumerable<int>::Range(0, INT_MAX)->Concat(IEnumerable<int>::Range(0, 1));
        //    Assert::AreEqual(INT_MAX + (__int64)1, hugeCollection->LongCount());
        //}

        //// Ignore("Takes an enormous amount of time!")
        //TEST_METHOD(LongCount_CollectionBiggerThanIntMaxCanBeCountedWithLongCountWithPredicate)
        //{
        //    auto hugeCollection = IEnumerable<int>::Range(0, INT_MAX)->Concat(IEnumerable<int>::Range(0, 1));
        //    Assert::AreEqual(INT_MAX + (__int64)1, hugeCollection->LongCount([](int x) { return x >= 0; }));
        //}
    };
}
