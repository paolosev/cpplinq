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
    TEST_CLASS(SumTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "sum\n");

            SumTest t;
            t.Sum_EmptySequenceInt32();
            t.Sum_EmptySequenceInt32WithSelector();
            t.Sum_SimpleSumInt32();
            t.Sum_SimpleSumInt32WithSelector();
            t.Sum_SimpleSumInt64();
            t.Sum_SimpleSumDouble();
        }

        //TEST_METHOD(Sum_NullSelectorInt32)
        //{
        //  std::string* p = new std::string[0];
        //  std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(p, 0));

        //  std::function<int(std::string)> selector = nullptr;
        //  Assert::ExpectException<ArgumentNullException&>([source, selector]() { source->Sum<int>(selector); });

        //  delete [] p;
        //}

        TEST_METHOD(Sum_EmptySequenceInt32)
        {
            int* p = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(p, 0));

            Assert::AreEqual(0, source->Sum());

            delete [] p;
        }

        TEST_METHOD(Sum_EmptySequenceInt32WithSelector)
        {
            std::string* p = new std::string[0];
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(p, 0));

            int result = source->Sum<int>([](std::string x) { return x.length(); });
            Assert::AreEqual(0, result);

            delete [] p;
        }

        TEST_METHOD(Sum_SimpleSumInt32)
        {
            int v[] = { 1, 3, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(6, source->Sum());
        }

        TEST_METHOD(Sum_SimpleSumInt32WithSelector)
        {
            std::string v[] = { "x", "abc", "de" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            Assert::AreEqual(6, source->Sum<int>([](std::string x) { return x.length(); } ));
        }
        /*
        // to do
        TEST_METHOD(Sum_NegativeOverflowInt32)
        {
            int v[] = { INT_MIN, INT_MAX };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<OverflowException&>([source]() { source->Sum(); });
        }

        TEST_METHOD(Sum_OverflowInt32)
        {
            int v[] = { INT_MAX, INT_MAX };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<OverflowException&>([source]() { source->Sum(); });
        }

        TEST_METHOD(Sum_OverflowInt32WithSelector)
        {
            std::string v[] = { "x", "y" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            Assert::ExpectException<OverflowException&>([source](std::string x) { return INT_MAX; });
        }

        TEST_METHOD(Sum_OverflowOfComputableSumInt32)
        {
            int v[] = { INT_MAX, 1, -1, -INT_MAX };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            // In a world where we summed using a long accumulator, the result would be 0.
            Assert::ExpectException<OverflowException&>([source]() { source->Sum(); });
        }
        */
        TEST_METHOD(Sum_SimpleSumInt64)
        {
            __int64 v[] = { 1, 3, 2 };
            std::shared_ptr<IEnumerable<__int64>> source(new Vector<__int64>(v, ARRAYSIZE(v)));

            Assert::AreEqual(__int64(6), source->Sum());
        }

        TEST_METHOD(Sum_SimpleSumDouble)
        {
            double v[] = { 1, 3, 2 };
            std::shared_ptr<IEnumerable<double>> source(new Vector<double>(v, ARRAYSIZE(v)));

            Assert::AreEqual(6.0, source->Sum());
        }
    };
}
