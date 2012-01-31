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
    TEST_CLASS(SingleTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "single\n");

            SingleTest t;
            t.Single_EmptySequenceWithoutPredicate();
            t.Single_SingleElementSequenceWithoutPredicate();
            t.Single_MultipleElementSequenceWithoutPredicate();
            t.Single_EmptySequenceWithPredicate();
            t.Single_SingleElementSequenceWithMatchingPredicate();
            t.Single_SingleElementSequenceWithNonMatchingPredicate();
            t.Single_MultipleElementSequenceWithNoPredicateMatches();
            t.Single_MultipleElementSequenceWithSinglePredicateMatch();
            t.Single_MultipleElementSequenceWithMultiplePredicateMatches();
            t.Single_EarlyOutWithoutPredicate();
            t.Single_EarlyOutWithPredicate();
        }

        //TEST_METHOD(Single_NullPredicate)
        //{
        //  int v[] = { 1, 3, 5 };
        //	std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //	std::function<bool(int)> predicate = nullptr;
        //	Assert::ExpectException<ArgumentNullException&>([source, &predicate]() {
        //		source->Single(predicate);
        //	});
        //}

        TEST_METHOD(Single_EmptySequenceWithoutPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single();
            });

            delete [] v;
        }

        TEST_METHOD(Single_SingleElementSequenceWithoutPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Single());
        }

        TEST_METHOD(Single_MultipleElementSequenceWithoutPredicate)
        {
            int v[] = { 5, 10 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single();
            });
        }

        TEST_METHOD(Single_EmptySequenceWithPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single([](int x) { return x > 3; });
            });

            delete [] v;
        }

        TEST_METHOD(Single_SingleElementSequenceWithMatchingPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Single([](int x) { return x > 3; }));
        }

        TEST_METHOD(Single_SingleElementSequenceWithNonMatchingPredicate)
        {
            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(Single_MultipleElementSequenceWithNoPredicateMatches)
        {
            int v[] = { 1, 2, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(Single_MultipleElementSequenceWithSinglePredicateMatch)
        {
            int v[] = { 1, 2, 5, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Single([](int x) { return x > 3; }));
        }

        TEST_METHOD(Single_MultipleElementSequenceWithMultiplePredicateMatches)
        {
            int v[] = { 1, 2, 5, 10, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(Single_EarlyOutWithoutPredicate)
        {
            int v[] = { 1, 2, 0 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });

            // We don't get as far as the third element - we die when we see the second
            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single();
            });
        }

        TEST_METHOD(Single_EarlyOutWithPredicate)
        {
            int v[] = { 1, 2, 0 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });

            // We don't get as far as the third element - we die when we see the second
            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Single([](int x) { return true; });
            });
        }
    };

    TEST_CLASS(SingleOrDefaultTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "singleOrDefault\n");

            SingleOrDefaultTest t;
            t.SingleOrDefault_EmptySequenceWithoutPredicate();
            t.SingleOrDefault_SingleElementSequenceWithoutPredicate();
            t.SingleOrDefault_MultipleElementSequenceWithoutPredicate();
            t.SingleOrDefault_EmptySequenceWithPredicate();
            t.SingleOrDefault_SingleElementSequenceWithMatchingPredicate();
            t.SingleOrDefault_SingleElementSequenceWithNonMatchingPredicate();
            t.SingleOrDefault_MultipleElementSequenceWithNoPredicateMatches();
            t.SingleOrDefault_MultipleElementSequenceWithSinglePredicateMatch();
            t.SingleOrDefault_MultipleElementSequenceWithMultiplePredicateMatches();
            t.SingleOrDefault_EarlyOutWithoutPredicate();
            t.SingleOrDefault_EarlyOutWithPredicate();
        }

        //TEST_METHOD(SingleOrDefault_NullPredicate)
        //{
        //  int v[] = { 1, 3, 5 };
        //	std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

        //	std::function<bool(int)> predicate = nullptr;
        //	Assert::ExpectException<ArgumentNullException&>([source, &predicate]() {
        //		source->SingleOrDefault(predicate);
        //	});
        //}

        TEST_METHOD(SingleOrDefault_EmptySequenceWithoutPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::AreEqual(int(), source->SingleOrDefault());
        }

        TEST_METHOD(SingleOrDefault_SingleElementSequenceWithoutPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->SingleOrDefault());
        }

        TEST_METHOD(SingleOrDefault_MultipleElementSequenceWithoutPredicate)
        {
            int v[] = { 5, 10 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->SingleOrDefault();
            });
        }

        TEST_METHOD(SingleOrDefault_EmptySequenceWithPredicate)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, 0));

            Assert::AreEqual(int(), source->SingleOrDefault([](int x) { return x > 3; }));

            delete [] v;
        }

        TEST_METHOD(SingleOrDefault_SingleElementSequenceWithMatchingPredicate)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->SingleOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(SingleOrDefault_SingleElementSequenceWithNonMatchingPredicate)
        {
            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(int(),	source->SingleOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(SingleOrDefault_MultipleElementSequenceWithNoPredicateMatches)
        {
            int v[] = { 1, 2, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(int(),	source->SingleOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(SingleOrDefault_MultipleElementSequenceWithSinglePredicateMatch)
        {
            int v[] = { 1, 2, 5, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->SingleOrDefault([](int x) { return x > 3; }));
        }

        TEST_METHOD(SingleOrDefault_MultipleElementSequenceWithMultiplePredicateMatches)
        {
            int v[] = { 1, 2, 5, 10, 2, 1 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->SingleOrDefault([](int x) { return x > 3; });
            });
        }

        TEST_METHOD(SingleOrDefault_EarlyOutWithoutPredicate)
        {
            int v[] = { 1, 2, 0 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });

            // We don't get as far as the third element - we die when we see the second
            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->SingleOrDefault();
            });
        }

        TEST_METHOD(SingleOrDefault_EarlyOutWithPredicate)
        {
            int v[] = { 1, 2, 0 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });

            // We don't get as far as the third element - we die when we see the second
            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->SingleOrDefault([](int x) { return true; });
            });
        }
    };
}
