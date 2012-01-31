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
    TEST_CLASS(TakeTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "take\n");

            TakeTest t;
            t.Take_ExecutionIsDeferred();
            t.Take_NegativeCount();
            t.Take_ZeroCount();
            t.Take_CountShorterThanSource();
            t.Take_CountEqualToSourceLength();
            t.Take_CountGreaterThanSourceLength();
            t.Take_OnlyEnumerateTheGivenNumberOfElements();
        }

        TEST_METHOD(Take_ExecutionIsDeferred)
        {
            auto source = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());
            source->Take(10);
        }

        TEST_METHOD(Take_NegativeCount)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Take(-5);

            int* exp = new int[0];
            Assert::IsTrue(result->SequenceEqual<int>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(Take_ZeroCount)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Take(0);

            int* exp = new int[0];
            Assert::IsTrue(result->SequenceEqual<int>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(Take_CountShorterThanSource)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Take(3);

            int exp[] = { 0, 1, 2 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }


        TEST_METHOD(Take_CountEqualToSourceLength)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Take(5);

            int exp[] = { 0, 1, 2, 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Take_CountGreaterThanSourceLength)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Take(100);

            int exp[] = { 0, 1, 2, 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Take_OnlyEnumerateTheGivenNumberOfElements)
        {
            int v[] = { 1, 2, 0 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            // If we try to move onto the third element, we'll die.
            auto query = source->Select<int>([](int x) { return 10 / x; });
            auto result = query->Take(2);

            int exp[] = { 10, 5 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }
    };

    TEST_CLASS(TakeWhileTest)
    {
    public:
        static void test()
        {
            TakeWhileTest t;
            t.TakeWhile_ExecutionIsDeferred();
            t.TakeWhile_PredicateFailingFirstElement();
            t.TakeWhile_PredicateWithIndexFailingFirstElement();
            t.TakeWhile_PredicateMatchingSomeElements();
            t.TakeWhile_PredicateWithIndexMatchingSomeElements();
            t.TakeWhile_PredicateMatchingAllElements();
            t.TakeWhile_PredicateWithIndexMatchingAllElements();
        }

        TEST_METHOD(TakeWhile_ExecutionIsDeferred)
        {
            auto source = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());
            source->TakeWhile([](int x) { return x > 10; });
        }
        /*
        TEST_METHOD(TakeWhile_NullPredicateNoIndex)
        {
            int v[] = { 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            std::function<bool(int)> predicate = nullptr;

            Assert::ExpectException<ArgumentNullException&>([source, predicate]() {
                source->TakeWhile(predicate);
            });
        }

        TEST_METHOD(TakeWhile_NullPredicateUsingIndex)
        {
            int v[] = { 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            std::function<bool(int, int)> predicate = nullptr;

            Assert::ExpectException<ArgumentNullException&>([source, predicate]() {
                source->TakeWhileIndex(predicate);
            });
        }
        */
        TEST_METHOD(TakeWhile_PredicateFailingFirstElement)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five", "six" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->TakeWhile([](std::string x) { return x.size() > 4; });

            std::string* exp = new std::string[0];
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(TakeWhile_PredicateWithIndexFailingFirstElement)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->TakeWhileIndex([](std::string x, int index) { return index + x.size() > 4; });

            std::string* exp = new std::string[0];
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(TakeWhile_PredicateMatchingSomeElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->TakeWhile([](std::string x) { return x.size() < 5; });

            std::string exp[] = { "zero", "one", "two" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(TakeWhile_PredicateWithIndexMatchingSomeElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->TakeWhileIndex([](std::string x, int index) { return (int)x.size() > index; });

            std::string exp[] = { "zero", "one", "two", "three" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(TakeWhile_PredicateMatchingAllElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->TakeWhile([](std::string x) { return x.size() < 100; });

            Assert::IsTrue(result->SequenceEqual<std::string>(v, ARRAYSIZE(v)));
        }

        TEST_METHOD(TakeWhile_PredicateWithIndexMatchingAllElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->TakeWhileIndex([](std::string x, int index) { return x.size() < 100; });

            Assert::IsTrue(result->SequenceEqual<std::string>(v, ARRAYSIZE(v)));
        }
    };
}
