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
    TEST_CLASS(AllTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "all\n");

            AllTest t;
            t.All_EmptySequenceReturnsTrue();
            t.All_PredicateMatchingNoElements();
            t.All_PredicateMatchingSomeElements();
            t.All_PredicateMatchingAllElements();
            t.All_SequenceIsNotEvaluatedAfterFirstNonMatch();
        }

        TEST_METHOD(All_EmptySequenceReturnsTrue)
        {
            int* p = new int[0];
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(p, 0));
            Assert::IsTrue(src->All([](int x) { return x > 0; }));
            delete [] p;
        }

        TEST_METHOD(All_PredicateMatchingNoElements)
        {
            int v[] = { 1, 1, 5, 20, 30 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::IsFalse(src->All([](int x) { return  x < 0; }));
        }

        TEST_METHOD(All_PredicateMatchingSomeElements)
        {
            int v[] = { 1, 5, 8, 9 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::IsFalse(src->All([](int x) { return  x > 3; }));
        }

        TEST_METHOD(All_PredicateMatchingAllElements)
        {
            int v[] = { 1, 5, 8, 9 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            Assert::IsTrue(src->All([](int x) { return  x > 0; }));
        }

        TEST_METHOD(All_SequenceIsNotEvaluatedAfterFirstNonMatch)
        {
            int v[] = { 2, 10, 0, 3 };
            std::shared_ptr<IEnumerable<int>> src(new Vector<int>(v, ARRAYSIZE(v)));
            auto query = src->Select<int>([](int x) { return 10 / x; });
            // This will finish at the second element (x = 10, so 10/x = 1)
            // It won't evaluate 10/0, which would throw an exception
            Assert::IsFalse(query->All([](int y) {return y > 2; }));
        }
    };
}