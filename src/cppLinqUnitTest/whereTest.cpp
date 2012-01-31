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
#include "throwingEnumerable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
    TEST_CLASS(WhereTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "where\n");

            WhereTest t;
            t.WhereTest_SimpleFiltering();
            t.WhereTest_SimpleFiltering2();
            t.WhereTest_SimpleFilteringWithComplexExpression();
            t.WhereTest_EmptySource();
            t.WhereTest_ExecutionIsDeferred();
            t.WhereTest_WithIndexSimpleFiltering();
            t.WhereTest_WithIndexEmptySource();
            t.WhereTest_ExecutionIsDeferred();
        }

        //TEST_METHOD(NullPredicateThrowsNullArgumentException)
        //{
        //    int v[] = { 1, 3, 7, 9, 10 };
        //	  std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
        //    std::function<bool(int)> predicate = nullptr;
        //	  Assert::ExpectException<ArgumentNullException&>( [&](){ source->Where(predicate);} );
        //}

        //TEST_METHOD(WithIndexNullPredicateThrowsNullArgumentException)
        //{
        //    int v[] = { 1, 3, 7, 9, 10 };
        //	  std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
        //    std::function<bool(int, int)> predicate = nullptr;
        //    Assert::ExpectException<ArgumentNullException&>( [&](){ source->Where(predicate);} );
        //}

        TEST_METHOD(WhereTest_SimpleFiltering)
        {
            int v[] = { 1, 3, 4, 2, 8, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            auto result = source->Where([](int x) { return x < 4; } );
            int expected[] = { 1, 3, 2, 1 };
            Assert::IsTrue(result->SequenceEqual(expected, ARRAYSIZE(expected)));
        }

        TEST_METHOD(WhereTest_SimpleFiltering2)
        {
            int v[] = { 1, 3, 4, 2, 8, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            std::function<bool(const int&)> predicate = [](int x) { return x < 4; };
            auto result = source->Where(predicate);
            int expected[] = { 1, 3, 2, 1 };
            Assert::IsTrue(result->SequenceEqual(expected, ARRAYSIZE(expected)));
        }

        TEST_METHOD(WhereTest_SimpleFilteringWithComplexExpression)
        {
            int v[] = { 1, 3, 4, 2, 8, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            auto result = source->Where([](int x) { return x < 4; })->
                Select<int>([](int y) { return y; });
            int expected[] = { 1, 3, 2, 1 };
            Assert::IsTrue(result->SequenceEqual(expected, ARRAYSIZE(expected)));
        }

        TEST_METHOD(WhereTest_EmptySource)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));
            auto result = source->Where([](int x) { return x < 4; } );
            Assert::IsTrue(result->SequenceEqual(v, 0));
            delete[] v;
        }

        TEST_METHOD(WhereTest_ExecutionIsDeferred)
        {
            ThrowingEnumerable::AssertDeferred([](std::shared_ptr<IEnumerable<int>> src) {
                return src->Where([=](int x) { return x > 0; } );
            });
        }

        TEST_METHOD(WhereTest_WithIndexSimpleFiltering)
        {
            int v[] = { 1, 3, 4, 2, 8, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));
            auto result = source->WhereIndex([](int x, int index) { return x < index; } );
            int expected[] = { 2, 1 };
            Assert::IsTrue(result->SequenceEqual(expected, ARRAYSIZE(expected)));
        }

        TEST_METHOD(WhereTest_WithIndexEmptySource)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));
            auto result = source->WhereIndex([](int x, int index) { return x < 4; } );
            Assert::IsTrue(result->SequenceEqual(v, 0));
            delete[] v;
        }

        TEST_METHOD(WhereTest_WithIndexExecutionIsDeferred)
        {
            ThrowingEnumerable::AssertDeferred([](std::shared_ptr<IEnumerable<int>> src) {
                return src->WhereIndex([=](int x, int index) { return x > 0; } );
            });
        }
    };
}