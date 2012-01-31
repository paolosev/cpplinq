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
    TEST_CLASS(AnyTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "any\n");

            AnyTest t;
            t.Any_EmptySequenceWithoutPredicate();
            t.Any_EmptySequenceWithPredicate();
            t.Any_NonEmptySequenceWithoutPredicate();
            t.Any_NonEmptySequenceWithPredicateMatchingElement();
            t.Any_NonEmptySequenceWithPredicateNotMatchingElement();
            t.Any_SequenceIsNotEvaluatedAfterFirstMatch();
        }

        TEST_METHOD(Any_EmptySequenceWithoutPredicate)
        {
            int* p = new int[0];
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(p, 0));
            Assert::IsFalse(src->Any());
            delete [] p;
        }

        TEST_METHOD(Any_EmptySequenceWithPredicate)
        {
            int* p = new int[0];
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(p, 0));
            Assert::IsFalse(src->Any([](int x) { return x > 10; }));
            delete [] p;
        }

        TEST_METHOD(Any_NonEmptySequenceWithoutPredicate)
        {
            int v[] = { 0 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::IsTrue(src->Any());
        }

        TEST_METHOD(Any_NonEmptySequenceWithPredicateMatchingElement)
        {
            int v[] = { 1, 5, 20, 30 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::IsTrue(src->Any([](int x) { return x > 10; }));
        }

        TEST_METHOD(Any_NonEmptySequenceWithPredicateNotMatchingElement)
        {
            int v[] = { 1, 5, 8, 9 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::IsFalse(src->Any([](int x) { return x > 10; }));
        }

        TEST_METHOD(Any_SequenceIsNotEvaluatedAfterFirstMatch)
        {
            int v[] = { 10, 2, 0, 3 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = src->Select<int>([](int x) { return 10 / x; });
            // This will finish at the second element (x = 2, so 10/x = 5)
            // It won't evaluate 10/0, which would throw an exception
            Assert::IsTrue(query->Any([](int y) { return y > 2; }));
        }
    };
}