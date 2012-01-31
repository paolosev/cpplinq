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
    TEST_CLASS(CountTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "count\n");

            CountTest t;
            t.Count_NonPredicatedCount();
            t.Count_PredicatedCount();
        }

        TEST_METHOD(Count_NonPredicatedCount)
        {
            Assert::AreEqual(5, IEnumerable<int>::Range(2, 5)->Count());
        }

        //TEST_METHOD(Count_PredicatedNullPredicateThrowsArgumentNullException)
        //{
        //    Assert::ExpectException<ArgumentNullException&>([&]() {
        //	      IEnumerable<int>::Range(2, 5)->Count();
        //    });
        //}

        TEST_METHOD(Count_PredicatedCount)
        {
            // Counts even numbers within 2, 3, 4, 5, 6
            Assert::AreEqual(3, IEnumerable<int>::Range(2, 5)->Count([](int x){
                return (x % 2 == 0);
            }));
        }

        // Ignore("Takes an enormous amount of time!")
        //      TEST_METHOD(Count_Overflow)
        //      {
        //          auto largeSequence = IEnumerable<int>::Range(0, INT_MAX)
        //										 ->Concat(IEnumerable<int>::Range(0, 1));
        //	        Assert::ExpectException<OverflowException&>([&largeSequence]() {
        //		        largeSequence->Count();
        //	        });
        //      }

        //// Ignore("Takes an enormous amount of time!")
        //      TEST_METHOD(Count_OverflowWithPredicate)
        //      {
        //          auto largeSequence = IEnumerable<int>::Range(0, INT_MAX)
        //										 ->Concat(IEnumerable<int>::Range(0, 1));
        //	        Assert::ExpectException<OverflowException&>([&largeSequence]() {
        //		        largeSequence->Count([](int x) { return x >= 0; });
        //	        });
        //      }
    };
}
