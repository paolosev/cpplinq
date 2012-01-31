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
    TEST_CLASS(ConcatTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "concat\n");

            ConcatTest t;
            t.Concat_SimpleConcatenation();
            t.Concat_NullSecondThrowsNullArgumentException();
            t.Concat_FirstSequenceIsntAccessedBeforeFirstUse();
            t.Concat_SecondSequenceIsntAccessedBeforeFirstUse();
        }

        TEST_METHOD(Concat_SimpleConcatenation)
        {
            const std::string v1[] = { "a", "b" };
            std::shared_ptr<IEnumerable<std::string>> first(new Vector<std::string>(v1, ARRAYSIZE(v1)));

            const std::string v2[] = { "c", "d" };
            std::shared_ptr<IEnumerable<std::string>> second(new Vector<std::string>(v2, ARRAYSIZE(v2)));

            std::string exp[] = { "a", "b", "c", "d" };
            Assert::IsTrue(first->Concat(second)->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Concat_NullSecondThrowsNullArgumentException)
        {
            const std::string v1[] = { "hello" };
            std::shared_ptr<IEnumerable<std::string>> first(new Vector<std::string>(v1, ARRAYSIZE(v1)));

            std::shared_ptr<IEnumerable<std::string>> second(nullptr);

            Assert::ExpectException<ArgumentNullException&>([&]() { first->Concat(second); });
        }

        TEST_METHOD(Concat_FirstSequenceIsntAccessedBeforeFirstUse)
        {
            std::shared_ptr<IEnumerable<int>> first(new ThrowingEnumerable());

            int v2[] = { 5 };
            std::shared_ptr<IEnumerable<int>> second(new Vector<int>(v2, ARRAYSIZE(v2)));

            // No exception yet...
            auto query = first->Concat(second);

            // Still no exception...
            auto iterator = query->GetEnumerator();

            // Now it will go bang
            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }

        TEST_METHOD(Concat_SecondSequenceIsntAccessedBeforeFirstUse)
        {
            int v1[] = { 5 };
            std::shared_ptr<IEnumerable<int>> first(new Vector<int>(v1, ARRAYSIZE(v1)));

            std::shared_ptr<IEnumerable<int>> second(new ThrowingEnumerable());

            // No exception yet...
            auto query = first->Concat(second);

            // Still no exception...
            auto iterator = query->GetEnumerator();

            // First element is fine...
            Assert::IsTrue(iterator->MoveNext());
            Assert::AreEqual(5, iterator->get_Current());

            // Now it will go bang, as we move into the second sequence
            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }
    };
}
