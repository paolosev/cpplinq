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
    TEST_CLASS(LastTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "last\n");

            LastTest t;
            t.Last_EmptySequenceWithoutPredicate();
            t.Last_SingleElementSequenceWithoutPredicate();
            t.Last_MultipleElementSequenceWithoutPredicate();
            t.Last_EmptySequenceWithPredicate();
            t.Last_SingleElementSequenceWithMatchingPredicate();
            t.Last_SingleElementSequenceWithNonMatchingPredicate();
            t.Last_MultipleElementSequenceWithNoPredicateMatches();
            t.Last_MultipleElementSequenceWithSinglePredicateMatch();
            t.Last_MultipleElementSequenceWithMultiplePredicateMatches();
        }

        //TEST_METHOD(Last_NullPredicate)
        //{
        //  int v[] = { 1, 3, 5 };
        //	std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //	std::function<bool(int)> predicate = nullptr;
        //	Assert::ExpectException<ArgumentNullException&>([source, &predicate]() {
        //		source->Last(predicate);
        //	});
        //}

        TEST_METHOD(Last_EmptySequenceWithoutPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Last();
            });

            delete [] v;
        }

        TEST_METHOD(Last_SingleElementSequenceWithoutPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Last());
        }

        TEST_METHOD(Last_MultipleElementSequenceWithoutPredicate)
        {
            int v[] = { 5, 10 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(10, source->Last());
        }

        TEST_METHOD(Last_EmptySequenceWithPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Last([](int x) { return x > 3; });
            });

            delete [] v;
        }

        TEST_METHOD(Last_SingleElementSequenceWithMatchingPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Last([](int x) { return x > 3; }));
        }

        TEST_METHOD(Last_SingleElementSequenceWithNonMatchingPredicate)
        {
            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Last([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(Last_MultipleElementSequenceWithNoPredicateMatches)
        {
            int v[] = { 1, 2, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Last([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(Last_MultipleElementSequenceWithSinglePredicateMatch)
        {
            int v[] = { 1, 2, 5, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Last([](int x) { return x > 3; }));
        }

        TEST_METHOD(Last_MultipleElementSequenceWithMultiplePredicateMatches)
        {
            int v[] = { 1, 2, 5, 10, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(10, source->Last([](int x) { return x > 3; }));
        }
    };

    TEST_CLASS(LastOrDefaultTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "lastOrDefault\n");

            LastOrDefaultTest t;
            t.LastOrDefault_EmptySequenceWithoutPredicate();
            t.LastOrDefault_SingleElementSequenceWithoutPredicate();
            t.LastOrDefault_MultipleElementSequenceWithoutPredicate();
            t.LastOrDefault_EmptySequenceWithPredicate();
            t.LastOrDefault_SingleElementSequenceWithMatchingPredicate();
            t.LastOrDefault_SingleElementSequenceWithNonMatchingPredicate();
            t.LastOrDefault_MultipleElementSequenceWithNoPredicateMatches();
            t.LastOrDefault_MultipleElementSequenceWithSinglePredicateMatch();
            t.LastOrDefault_MultipleElementSequenceWithMultiplePredicateMatches();
        }

        //TEST_METHOD(LastOrDefault_NullPredicate)
        //{
        //  int v[] = { 1, 3, 5 };
        //	std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //	std::function<bool(int)> predicate = nullptr;
        //	Assert::ExpectException<ArgumentNullException&>([source, &predicate]() {
        //		source->LastOrDefault(predicate);
        //	});
        //}

        TEST_METHOD(LastOrDefault_EmptySequenceWithoutPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::AreEqual(int(), source->LastOrDefault());
        }

        TEST_METHOD(LastOrDefault_SingleElementSequenceWithoutPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->LastOrDefault());
        }

        TEST_METHOD(LastOrDefault_MultipleElementSequenceWithoutPredicate)
        {
            int v[] = { 5, 10 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(10, source->LastOrDefault());
        }

        TEST_METHOD(LastOrDefault_EmptySequenceWithPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::AreEqual(int(), source->LastOrDefault([](int x) { return x > 3; }));

            delete [] v;
        }

        TEST_METHOD(LastOrDefault_SingleElementSequenceWithMatchingPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->LastOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(LastOrDefault_SingleElementSequenceWithNonMatchingPredicate)
        {
            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(int(),	source->LastOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(LastOrDefault_MultipleElementSequenceWithNoPredicateMatches)
        {
            int v[] = { 1, 2, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(int(),	source->LastOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(LastOrDefault_MultipleElementSequenceWithSinglePredicateMatch)
        {
            int v[] = { 1, 2, 5, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->LastOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(LastOrDefault_MultipleElementSequenceWithMultiplePredicateMatches)
        {
            int v[] = { 1, 2, 5, 10, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(10, source->LastOrDefault([](int x) { return x > 3; }));
        }
    };
}
