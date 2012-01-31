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
    TEST_CLASS(RangeTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "range\n");

            RangeTest t;
            t.Range_NegativeCount();
            t.Range_CountTooLarge();
            t.Range_LargeButValidCount();
            t.Range_ValidRange();
            t.Range_NegativeStart();
            t.Range_EmptyRange();
            t.Range_SingleValueOfMaxInt32();
            t.Range_EmptyRangeStartingAtMinInt32();
        }

        TEST_METHOD(Range_NegativeCount)
        {
            Assert::ExpectException<ArgumentOutOfRangeException&>( [](){
                IEnumerable<int>::Range(10, -1);
            });
        }

        TEST_METHOD(Range_CountTooLarge)
        {
            Assert::ExpectException<ArgumentOutOfRangeException&>([](){
                IEnumerable<int>::Range(INT_MAX, 2);
            });
            Assert::ExpectException<ArgumentOutOfRangeException&>([](){
                IEnumerable<int>::Range(2, INT_MAX);
            });
            // INT_MAX is odd, hence the +3 instead of +2
            Assert::ExpectException<ArgumentOutOfRangeException&>([](){
                IEnumerable<int>::Range(INT_MAX / 2, (INT_MAX / 2) + 3);
            });
        }

        TEST_METHOD(Range_LargeButValidCount)
        {
            // Essentially the edge conditions for CountTooLarge, but just below the boundary
            IEnumerable<int>::Range(INT_MAX, 1);
            IEnumerable<int>::Range(1, INT_MAX);
            IEnumerable<int>::Range(INT_MAX / 2, (INT_MAX / 2) + 2);
        }

        TEST_METHOD(Range_ValidRange)
        {
            int exp[] = { 5, 6, 7 };
            Assert::IsTrue(IEnumerable<int>::Range(5, 3)->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Range_NegativeStart)
        {
            int exp[] = { -2, -1, 0, 1, 2 };
            Assert::IsTrue(IEnumerable<int>::Range(-2, 5)->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Range_EmptyRange)
        {
            int* p = new int[0];
            Assert::IsTrue(IEnumerable<int>::Range(100, 0)->SequenceEqual<int>(p, 0));
            delete [] p;
        }

        TEST_METHOD(Range_SingleValueOfMaxInt32)
        {
            int exp[] = { INT_MAX };
            Assert::IsTrue(IEnumerable<int>::Range(INT_MAX, 1)->SequenceEqual(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Range_EmptyRangeStartingAtMinInt32)
        {
            int* p = new int[0];
            Assert::IsTrue(IEnumerable<int>::Range(INT_MAX, 0)->SequenceEqual(p, 0));
            delete [] p;
        }
    };
}
