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
#include "throwingenumerable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
    TEST_CLASS(UnionTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "union\n");

            UnionTest t;
            t.Union_NullSecondWithoutComparer();
            t.Union_NullSecondWithComparer();
            t.Union_UnionWithoutComparer();
            t.Union_UnionWithCaseInsensitiveComparer();
            t.Union_UnionWithEmptyFirstSequence();
            t.Union_UnionWithEmptySecondSequence();
            t.Union_UnionWithTwoEmptySequences();
            t.Union_FirstSequenceIsNotUsedUntilQueryIsIterated();
            t.Union_SecondSequenceIsNotUsedUntilFirstIsExhausted();
        }

        TEST_METHOD(Union_NullSecondWithoutComparer)
        {
            String v[] = { "xyz" };
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(v, ARRAYSIZE(v)));
            std::shared_ptr<IEnumerable<String>> rhs(nullptr);

            Assert::ExpectException<ArgumentNullException&>([lhs, rhs]() { lhs->Union(rhs); });
        }

        TEST_METHOD(Union_NullSecondWithComparer)
        {
            String v[] = { "xyz" };
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(v, ARRAYSIZE(v)));

            std::shared_ptr<IEnumerable<String>> rhs(nullptr);

            Assert::ExpectException<ArgumentNullException&>([lhs, rhs]() { lhs->Union(rhs, CompareIgnoreCase()); });
        }

        TEST_METHOD(Union_UnionWithoutComparer)
        {
            String l[] = { "a", "b", "B", "c", "b" };
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(l, ARRAYSIZE(l)));

            String r[] = { "d", "e", "d", "a" };
            std::shared_ptr<IEnumerable<String>> rhs(new Vector<String>(r, ARRAYSIZE(r)));

            String exp[] = { "a", "b", "B", "c", "d", "e" };
            Assert::IsTrue(lhs->Union(rhs)->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }

        //TEST_METHOD(Union_UnionWithNullComparer)
        //{
        //    String l[] = { "a", "b", "B", "c", "b" };
        //    std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(l, ARRAYSIZE(l)));

        //    String r[] = { "d", "e", "d", "a" };
        //    std::shared_ptr<IEnumerable<String>> rhs(new Vector<String>(r, ARRAYSIZE(r)));

        //    String exp[] = { "a", "b", "B", "c", "d", "e" };
        //    Assert::IsTrue(lhs->Union(rhs, nullptr)->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        //}

        TEST_METHOD(Union_UnionWithCaseInsensitiveComparer)
        {
            String l[] = { "a", "b", "B", "c", "b" };
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(l, ARRAYSIZE(l)));

            String r[] = { "d", "e", "d", "a" };
            std::shared_ptr<IEnumerable<String>> rhs(new Vector<String>(r, ARRAYSIZE(r)));

            String exp[] = { "a", "b", "c", "d", "e" };
            Assert::IsTrue(lhs->Union(rhs, CompareIgnoreCase())->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Union_UnionWithEmptyFirstSequence)
        {
            String* l = new String[0];
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(l, 0));

            String r[] = { "d", "e", "d", "a" };
            std::shared_ptr<IEnumerable<String>> rhs(new Vector<String>(r, ARRAYSIZE(r)));

            String exp[] = { "d", "e", "a" };
            Assert::IsTrue(lhs->Union(rhs)->SequenceEqual<String>(exp, ARRAYSIZE(exp)));

            delete [] l;
        }

        TEST_METHOD(Union_UnionWithEmptySecondSequence)
        {
            String l[] = { "a", "b", "B", "c", "b" };
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(l, ARRAYSIZE(l)));

            String* r = new String[0];
            std::shared_ptr<IEnumerable<String>> rhs(new Vector<String>(r, 0));

            String exp[] = { "a", "b", "B", "c" };
            Assert::IsTrue(lhs->Union(rhs)->SequenceEqual<String>(exp, ARRAYSIZE(exp)));

            delete [] r;
        }

        TEST_METHOD(Union_UnionWithTwoEmptySequences)
        {
            String* l = new String[0];
            std::shared_ptr<IEnumerable<String>> lhs(new Vector<String>(l, 0));

            String* r = new String[0];
            std::shared_ptr<IEnumerable<String>> rhs(new Vector<String>(r, 0));

            Assert::IsTrue(lhs->Union(rhs)->SequenceEqual<String>(l, 0));

            delete [] r;
            delete [] l;
        }

        TEST_METHOD(Union_FirstSequenceIsNotUsedUntilQueryIsIterated)
        {
            auto first = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());

            int v[] = { 2 };
            std::shared_ptr<IEnumerable<int>> second(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = first->Union(second);
            auto iterator = query->GetEnumerator();

            // Still no exception... until we call MoveNext
            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }

        TEST_METHOD(Union_SecondSequenceIsNotUsedUntilFirstIsExhausted)
        {
            int v[] = { 3, 5, 3 };
            std::shared_ptr<IEnumerable<int>> first(new Vector<int>(v, ARRAYSIZE(v)));

            auto second = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());

            auto iterator = first->Union(second)->GetEnumerator();

            // Check the first two elements...
            Assert::IsTrue(iterator->MoveNext());
            Assert::AreEqual(3, iterator->get_Current());
            Assert::IsTrue(iterator->MoveNext());
            Assert::AreEqual(5, iterator->get_Current());

            // But as soon as we move past the first sequence, bang!
            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }
    };
}
