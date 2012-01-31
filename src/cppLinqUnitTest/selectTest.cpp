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
    TEST_CLASS(SelectTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "select\n");

            SelectTest selectTest;
            selectTest.Select_EmptySource();
            selectTest.Select_ExecutionIsDeferred();
            selectTest.Select_SimpleProjection();
            selectTest.Select_SimpleProjectionToDifferentType();
            selectTest.Select_SimpleProjectionWithQueryExpression();
            selectTest.Select_WithIndexEmptySource();
            selectTest.Select_WithIndexExecutionIsDeferred();
            selectTest.Select_WithIndexSimpleProjection();
            selectTest.Select_SideEffectsInProjection();
        }

        //TEST_METHOD(Select_NullProjectionThrowsNullArgumentException)
        //{
        //    int v[] = { 1, 3, 7, 9, 10 };
        //    std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //    std::function<int(int)> projection = nullptr;

        //    Assert::ExpectException<ArgumentNullException&>([source, &projection]() {
        //        source->Select<int>(projection);
        //    });
        //}

        //TEST_METHOD(Select_WithIndexNullPredicateThrowsNullArgumentException)
        //{
        //    int v[] = { 1, 3, 7, 9, 10 };
        //    std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //    std::function<int(int, int)> projection = nullptr;

        //    Assert::ExpectException<ArgumentNullException&>([source, &projection]() {
        //        source->SelectIndex<int>(projection);
        //    });
        //}

        TEST_METHOD(Select_SimpleProjection)
        {
            int v[] = { 1, 5, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto result = source->Select<int>([](int x) { return x * 2; });

            int exp[] = { 2, 10, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Select_SimpleProjectionWithQueryExpression)
        {
            int v[] = { 1, 5, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto result = source->Select<int>([](int x) { return x * 2; });

            int exp[] = { 2, 10, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Select_SimpleProjectionToDifferentType)
        {
            int v[] = { 1, 5, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto result = source->Select<std::string>([](int x) { return IntToString(x); });

            std::string exp[] = { "1", "5", "2" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Select_EmptySource)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            auto result = source->Select<int>([](int x) { return x * 2; });

            Assert::IsTrue(result->SequenceEqual<int>(v, 0));

            delete [] v;
        }

        TEST_METHOD(Select_ExecutionIsDeferred)
        {
            ThrowingEnumerable::AssertDeferred([](std::shared_ptr<IEnumerable<int>> src) {
                return src->Select<int>([=](int x) { return x * 2; } );
            });
        }

        TEST_METHOD(Select_WithIndexSimpleProjection)
        {
            int v[] = { 1, 5, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto result = source->SelectIndex<int>([](int x, int index) { return x + index * 10; });

            int exp[] = { 1, 15, 22 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Select_WithIndexEmptySource)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            auto result = source->SelectIndex<int>([](int x, int index) { return x + index; });

            Assert::IsTrue(result->SequenceEqual<int>(v, 0));

            delete [] v;
        }

        TEST_METHOD(Select_WithIndexExecutionIsDeferred)
        {
            ThrowingEnumerable::AssertDeferred([](std::shared_ptr<IEnumerable<int>> src) {
                return src->SelectIndex<int>([=](int x, int index) { return x + index; } );
            });
        }

        TEST_METHOD(Select_SideEffectsInProjection)
        {
            int v[3] = { 0, 0, 0 }; // Actual values won't be relevant
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            int count = 0;

            auto query = source->Select<int>([&count](int x) {
                return count++;
            });

            int exp1[] = { 0, 1, 2 };
            Assert::IsTrue(query->SequenceEqual<int>(exp1, ARRAYSIZE(exp1)));

            int exp2[] = { 3, 4, 5 };
            Assert::IsTrue(query->SequenceEqual<int>(exp2, ARRAYSIZE(exp2)));

            count = 10;

            int exp3[] = { 10, 11, 12 };
            Assert::IsTrue(query->SequenceEqual<int>(exp3, ARRAYSIZE(exp3)));
        }

    };
}
