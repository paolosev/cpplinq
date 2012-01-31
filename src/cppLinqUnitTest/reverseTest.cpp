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
    TEST_CLASS(ReverseTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "reverse\n");

            ReverseTest t;
            t.Reverse_ExecutionIsDeferred();
            t.Reverse_InputIsBuffered();
            t.Reverse_ArraysAreBuffered();
            t.Reverse_ReversedRange();
            t.Reverse_ReversedList();
            t.Reverse_EmptyInput();
        }

        TEST_METHOD(Reverse_ExecutionIsDeferred)
        {
            auto source = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());
            source->Reverse();
        }

        TEST_METHOD(Reverse_InputIsBuffered)
        {
            int v[] = { 10, 0, 20 };
            std::shared_ptr<IEnumerable<int>> values(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = values->Select<int>([](int x) { return 10 / x; })->Reverse();

            auto iterator = query->GetEnumerator();
            Assert::ExpectException<std::exception&>([iterator]() {
                iterator->MoveNext();
            });
        }

        TEST_METHOD(Reverse_ArraysAreBuffered)
        {
            int v[] = { 0, 1, 2, 3 };
            Vector<int>* pV = new Vector<int>(v, ARRAYSIZE(v));
            Vector<int>& V = *pV;
            std::shared_ptr<IEnumerable<int>> source(pV);

            auto query = source->Reverse();
            V[1] = 99; // This change *will* be seen due to deferred execution

            auto iterator = query->GetEnumerator();
            iterator->MoveNext();
            Assert::AreEqual(3, iterator->get_Current());

            V[2] = 100; // This change *won't* be seen                
            iterator->MoveNext();
            Assert::AreEqual(2, iterator->get_Current());

            iterator->MoveNext();
            Assert::AreEqual(99, iterator->get_Current());

            iterator->MoveNext();
            Assert::AreEqual(0, iterator->get_Current());
        }

        TEST_METHOD(Reverse_ReversedRange)
        {
            auto query = IEnumerable<int>::Range(5, 5)->Reverse();

            int exp[] = { 9, 8, 7, 6, 5 };
            Assert::IsTrue(query->SequenceEqual<int>(exp, ARRAYSIZE(exp)));

        }

        TEST_METHOD(Reverse_ReversedList)
        {
            int v[] = { 5, 6, 7, 8, 9 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Reverse();

            int exp[] = { 9, 8, 7, 6, 5 };
            Assert::IsTrue(query->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Reverse_EmptyInput)
        {
            int* v = new int[0];
            std::shared_ptr<IEnumerable<int>> input(new Vector<int>(v, 0));

            Assert::IsTrue(input->Reverse()->SequenceEqual<int>(v, 0));
        }
    };
}
