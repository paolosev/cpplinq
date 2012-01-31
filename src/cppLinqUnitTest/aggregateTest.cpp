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
    TEST_CLASS(AggregateTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "aggregate\n");

            AggregateTest aggregateTest;
            aggregateTest.AggregateTest_DifferentSourceAndAccumulatorTypes();
            aggregateTest.AggregateTest_EmptySequenceSeeded();
            aggregateTest.AggregateTest_EmptySequenceSeededWithResultSelector();
            aggregateTest.AggregateTest_EmptySequenceUnseeded();
            aggregateTest.AggregateTest_FirstElementOfInputIsUsedAsSeedForUnseededOverload();
            aggregateTest.AggregateTest_NullFuncSeeded();
            aggregateTest.AggregateTest_NullFuncUnseeded();
            aggregateTest.AggregateTest_SeededAggregation();
            aggregateTest.AggregateTest_SeededAggregationWithResultSelector();
            aggregateTest.AggregateTest_UnseededAggregation();
        }

        TEST_METHOD(AggregateTest_NullFuncUnseeded)
        {
            int v[] = { 1, 3, 4, 2, 8, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            std::function<int(int, int)> func = nullptr;
            Assert::ExpectException<ArgumentNullException&>( [&](){ source->Aggregate(func); });
        }

        TEST_METHOD(AggregateTest_UnseededAggregation)
        {
            int v[] = { 1, 4, 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            // First iteration: 0 * 2 + 1 = 1
            // Second iteration: 1 * 2 + 4 = 6
            // Third iteration: 6 * 2 + 5 = 17
            Assert::AreEqual(17, source->Aggregate([](int current, int value) { return (current * 2 + value); } ));
        }

        TEST_METHOD(AggregateTest_NullFuncSeeded)
        {
            int v[] = { 1, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            std::function<int(int, int)> func = nullptr;
            Assert::ExpectException<ArgumentNullException&>( [&](){ source->Aggregate(5, func); } );
        }

        TEST_METHOD(AggregateTest_SeededAggregation)
        {
            int v[] = { 1, 4, 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            int seed = 5;
            std::function<int(int, int)> func = [](int current, int value) {
                return current * 2 + value;
            };
            // First iteration: 5 * 2 + 1 = 11
            // Second iteration: 11 * 2 + 4 = 26
            // Third iteration: 26 * 2 + 5 = 57
            Assert::AreEqual(57, source->Aggregate(seed, func));
        }
        /*
        TEST_METHOD(AggregateTest_NullFuncSeededWithResultSelector)
        {
            int v[] = { 1, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            // T: int
            // TAccumulate: double
            // TResult: long
            std::function<double(double, int)> accFn = nullptr;
            Assert::ExpectException<ArgumentNullException&>( [&]() {
                source->Aggregate<double, long>(5, accFn,
                    [](double v) { return (long)(v + 1); });
            });
        }

        TEST_METHOD(AggregateTest_NullProjectionSeededWithResultSelector)
        {
            int v[] = { 1, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            // T: int
            // TAccumulate: double
            // TResult: long
            std::function<long(double)> resultSelector = nullptr;
            Assert::ExpectException<ArgumentNullException&>( [&](){
                source->Aggregate<double, long>(5,
                    [](double i, int j) { return i + j; },
                resultSelector);
            });
        }
        */
        TEST_METHOD(AggregateTest_SeededAggregationWithResultSelector)
        {
            int v[] = { 1, 4, 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            int seed = 5;
            std::function<int(int, int)> func = [](int current, int value) { return current * 2 + value; };
            std::function<std::string(int)> resultSelector = [](int result) -> std::string {
                return IntToString(result);
            };
            // First iteration: 5 * 2 + 1 = 11
            // Second iteration: 11 * 2 + 4 = 26
            // Third iteration: 26 * 2 + 5 = 57
            // Result projection: 57.ToString() = "57"
            Assert::AreEqual(0, strcmp(source->Aggregate(seed, func, resultSelector).c_str(), "57"));
        }

        TEST_METHOD(AggregateTest_DifferentSourceAndAccumulatorTypes)
        {
            int largeValue = 2000000000;
            int v[] = { largeValue, largeValue, largeValue };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            __int64 sum = source->Aggregate<__int64>(0L, [](__int64 acc, int value) { return acc + value; });

            Assert::AreEqual(6000000000L, sum);
            // Just to prove we haven't missed off a zero...
            Assert::IsTrue(sum > INT_MAX);
        }


        TEST_METHOD(AggregateTest_EmptySequenceUnseeded)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));
            Assert::ExpectException<InvalidOperationException&>( [&](){
                source->Aggregate([](int x, int y) { return x + y; });
            });
            delete [] v;
        }

        TEST_METHOD(AggregateTest_EmptySequenceSeeded)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));
            Assert::AreEqual(5, source->Aggregate<int>(5, [](int x, int y) { return x + y; }));
            delete [] v;
        }

        TEST_METHOD(AggregateTest_EmptySequenceSeededWithResultSelector)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));
            Assert::AreEqual(0, strcmp("5", source->Aggregate<int, std::string>(5, [](int x, int y) { return x + y; },
                [] (int x) { return IntToString(x); }).c_str()));
            delete [] v;
        }

        TEST_METHOD(AggregateTest_FirstElementOfInputIsUsedAsSeedForUnseededOverload)
        {
            int v[] = { 5, 3, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::AreEqual(30, source->Aggregate([](int acc, int value) { return acc * value; }));
        }
    };
}