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
    TEST_CLASS(ExceptTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "except\n");

            ExceptTest t;
            t.Except_NullSecondWithoutComparer();
            t.Except_NullSecondWithComparer();
            t.Except_NoComparerSpecified();
            t.Except_CaseInsensitiveComparerSpecified();
            t.Except_NoSequencesUsedBeforeIteration();
            t.Except_SecondSequenceReadFullyOnFirstResultIteration();
            t.Except_FirstSequenceOnlyReadAsResultsAreRead();
        }

        TEST_METHOD(Except_NullSecondWithoutComparer)
        {
            int v[] = { 5 };
            std::shared_ptr<IEnumerable<int>> first(new Vector<int>(v, ARRAYSIZE(v)));
            std::shared_ptr<IEnumerable<int>> second(nullptr);

            Assert::ExpectException<ArgumentNullException&>([&first, &second]() {
                first->Except(second);
            });
        }

        TEST_METHOD(Except_NullSecondWithComparer)
        {
            String v0[] = { "A" };
            std::shared_ptr<IEnumerable<String>> first(new Vector<String>(v0, ARRAYSIZE(v0)));
            std::shared_ptr<IEnumerable<String>> second(nullptr);

            Assert::ExpectException<ArgumentNullException&>([&first, &second]() {
                first->Except(second, CompareIgnoreCase());
            });
        }

        TEST_METHOD(Except_NoComparerSpecified)
        {
            String v0[] = { "A", "a", "b", "c", "b", "c" };
            std::shared_ptr<IEnumerable<String>> first(new Vector<String>(v0, ARRAYSIZE(v0)));

            String v1[] = {  "b", "a", "d", "a" };
            std::shared_ptr<IEnumerable<String>> second(new Vector<String>(v1, ARRAYSIZE(v1)));

            String exp[] = { "A", "c" };
            Assert::IsTrue(first->Except(second)->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }
        /*
        TEST_METHOD(Except_NullComparerSpecified)
        {
        String v0[] = { "A", "a", "b", "c", "b", "c" };
        std::shared_ptr<IEnumerable<String>> first(new Vector<String>(v0, ARRAYSIZE(v0)));

        String v1[] = {  "b", "a", "d", "a" };
        std::shared_ptr<IEnumerable<String>> second(new Vector<String>(v1, ARRAYSIZE(v1)));

        String exp[] = { "A", "c" };
        std::function<bool(String, String)> comparer = nullptr;
        Assert::IsTrue(first->Except(second, comparer)->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }
        */
        TEST_METHOD(Except_CaseInsensitiveComparerSpecified)
        {
            String v0[] = { "A", "a", "b", "c", "b" };
            std::shared_ptr<IEnumerable<String>> first(new Vector<String>(v0, ARRAYSIZE(v0)));

            String v1[] = {  "b", "a", "d", "a" };
            std::shared_ptr<IEnumerable<String>> second(new Vector<String>(v1, ARRAYSIZE(v1)));

            String exp[] = { "c" };
            Assert::IsTrue(first->Except(second, CompareIgnoreCase())->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Except_NoSequencesUsedBeforeIteration)
        {
            std::shared_ptr<IEnumerable<int>> first(new ThrowingEnumerable());
            std::shared_ptr<IEnumerable<int>> second(new ThrowingEnumerable());

            // No exceptions!
            auto query = first->Except(second);

            // Still no exceptions... we're not calling MoveNext.
            auto iterator = query->GetEnumerator();
        }

        TEST_METHOD(Except_SecondSequenceReadFullyOnFirstResultIteration)
        {
            int v0[] = { 1 };
            std::shared_ptr<IEnumerable<int>> first(new Vector<int>(v0, ARRAYSIZE(v0)));

            int v1[] = { 10, 2, 0 };
            std::shared_ptr<IEnumerable<int>> second(new Vector<int>(v1, ARRAYSIZE(v1)));
            auto secondQuery = second->Select<int>([](int x) -> int {
                return 10 / x;
            });

            auto query = first->Except(secondQuery);
            auto iterator = query->GetEnumerator();

            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }

        TEST_METHOD(Except_FirstSequenceOnlyReadAsResultsAreRead)
        {
            int v0[] = { 10, 2, 0, 2 };
            std::shared_ptr<IEnumerable<int>> first(new Vector<int>(v0, ARRAYSIZE(v0)));
            auto firstQuery = first->Select<int>([](int x) { return 10 / x; });

            int v1[] = { 1 };
            std::shared_ptr<IEnumerable<int>> second(new Vector<int>(v1, ARRAYSIZE(v1)));

            auto query = firstQuery->Except(second);
            auto iterator = query->GetEnumerator();

            // We can get the first value with no problems
            Assert::IsTrue(iterator->MoveNext());
            Assert::AreEqual(5, iterator->get_Current());

            // Getting at the *second* value of the result sequence requires
            // reading from the first input sequence until the "bad" division
            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }
    };
}
