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
    TEST_CLASS(FirstTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "first\n");

            FirstTest t;
            t.First_EmptySequenceWithoutPredicate();
            t.First_SingleElementSequenceWithoutPredicate();
            t.First_MultipleElementSequenceWithoutPredicate();
            t.First_EmptySequenceWithPredicate();
            t.First_SingleElementSequenceWithMatchingPredicate();
            t.First_SingleElementSequenceWithNonMatchingPredicate();
            t.First_MultipleElementSequenceWithNoPredicateMatches();
            t.First_MultipleElementSequenceWithSinglePredicateMatch();
            t.First_MultipleElementSequenceWithMultiplePredicateMatches();
            t.First_EarlyOutAfterFirstElementWithoutPredicate();
            t.First_EarlyOutAfterFirstElementWithPredicate();
        }

        //TEST_METHOD(First_NullPredicate)
        //{
        //  int v[] = { 1, 3, 5 };
        //	std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //	std::function<bool(int)> predicate = nullptr;
        //	Assert::ExpectException<ArgumentNullException&>([source, &predicate]() {
        //		source->First(predicate);
        //	});
        //}

        TEST_METHOD(First_EmptySequenceWithoutPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->First();
            });

            delete [] v;
        }

        TEST_METHOD(First_SingleElementSequenceWithoutPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->First());
        }

        TEST_METHOD(First_MultipleElementSequenceWithoutPredicate)
        {
            int v[] = { 5, 10 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->First());
        }

        TEST_METHOD(First_EmptySequenceWithPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->First([](int x) { return x > 3; });
            });

            delete [] v;
        }

        TEST_METHOD(First_SingleElementSequenceWithMatchingPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->First([](int x) { return x > 3; }));
        }

        TEST_METHOD(First_SingleElementSequenceWithNonMatchingPredicate)
        {
            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->First([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(First_MultipleElementSequenceWithNoPredicateMatches)
        {
            int v[] = { 1, 2, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->First([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(First_MultipleElementSequenceWithSinglePredicateMatch)
        {
            int v[] = { 1, 2, 5, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->First([](int x) { return x > 3; }));
        }

        TEST_METHOD(First_MultipleElementSequenceWithMultiplePredicateMatches)
        {
            int v[] = { 1, 2, 5, 10, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->First([](int x) { return x > 3; }));
        }

        TEST_METHOD(First_EarlyOutAfterFirstElementWithoutPredicate)
        {
            int v[] = { 15, 1, 0, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });
            // We finish before getting as far as dividing by 0
            Assert::AreEqual(0, query->First());
        }

        TEST_METHOD(First_EarlyOutAfterFirstElementWithPredicate)
        {
            int v[] = { 15, 1, 0, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });
            // We finish before getting as far as dividing by 0
            Assert::AreEqual(10, query->First([](int y) { return y > 5; }));
        }
    };

    TEST_CLASS(FirstOrDefaultTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "firstOrDefault\n");

            FirstOrDefaultTest t;
            t.FirstOrDefault_EmptySequenceWithoutPredicate();
            t.FirstOrDefault_SingleElementSequenceWithoutPredicate();
            t.FirstOrDefault_MultipleElementSequenceWithoutPredicate();
            t.FirstOrDefault_EmptySequenceWithPredicate();
            t.FirstOrDefault_SingleElementSequenceWithMatchingPredicate();
            t.FirstOrDefault_SingleElementSequenceWithNonMatchingPredicate();
            t.FirstOrDefault_MultipleElementSequenceWithNoPredicateMatches();
            t.FirstOrDefault_MultipleElementSequenceWithSinglePredicateMatch();
            t.FirstOrDefault_MultipleElementSequenceWithMultiplePredicateMatches();
            t.FirstOrDefault_EarlyOutAfterFirstOrDefaultElementWithoutPredicate();
            t.FirstOrDefault_EarlyOutAfterFirstOrDefaultElementWithPredicate();
        }

        //TEST_METHOD(FirstOrDefault_NullPredicate)
        //{
        //  int v[] = { 1, 3, 5 };
        //	std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //	std::function<bool(int)> predicate = nullptr;
        //	Assert::ExpectException<ArgumentNullException&>([source, &predicate]() {
        //		source->FirstOrDefault(predicate);
        //	});
        //}

        TEST_METHOD(FirstOrDefault_EmptySequenceWithoutPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::AreEqual(int(), source->FirstOrDefault());
        }

        TEST_METHOD(FirstOrDefault_SingleElementSequenceWithoutPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->FirstOrDefault());
        }

        TEST_METHOD(FirstOrDefault_MultipleElementSequenceWithoutPredicate)
        {
            int v[] = { 5, 10 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->FirstOrDefault());
        }

        TEST_METHOD(FirstOrDefault_EmptySequenceWithPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::AreEqual(int(), source->FirstOrDefault([](int x) { return x > 3; }));

            delete [] v;
        }

        TEST_METHOD(FirstOrDefault_SingleElementSequenceWithMatchingPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->FirstOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(FirstOrDefault_SingleElementSequenceWithNonMatchingPredicate)
        {
            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(0,	source->FirstOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(FirstOrDefault_MultipleElementSequenceWithNoPredicateMatches)
        {
            int v[] = { 1, 2, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(0,	source->FirstOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(FirstOrDefault_MultipleElementSequenceWithSinglePredicateMatch)
        {
            int v[] = { 1, 2, 5, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->FirstOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(FirstOrDefault_MultipleElementSequenceWithMultiplePredicateMatches)
        {
            int v[] = { 1, 2, 5, 10, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->FirstOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(FirstOrDefault_EarlyOutAfterFirstOrDefaultElementWithoutPredicate)
        {
            int v[] = { 15, 1, 0, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });
            // We finish before getting as far as dividing by 0
            Assert::AreEqual(0, query->FirstOrDefault());
        }

        TEST_METHOD(FirstOrDefault_EarlyOutAfterFirstOrDefaultElementWithPredicate)
        {
            int v[] = { 15, 1, 0, 3 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });
            // We finish before getting as far as dividing by 0
            Assert::AreEqual(10, query->FirstOrDefault([](int y) { return y > 5; }));
        }
    };
}
