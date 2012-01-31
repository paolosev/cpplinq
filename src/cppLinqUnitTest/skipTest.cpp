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
    TEST_CLASS(SkipTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "skip\n");

            SkipTest t;
            t.Skip_ExecutionIsDeferred();
            t.Skip_NegativeCount();
            t.Skip_ZeroCount();
            t.Skip_NegativeCountWithArray();
            t.Skip_ZeroCountWithArray();
            t.Skip_CountShorterThanSource();
            t.Skip_CountEqualToSourceLength();
            t.Skip_CountGreaterThanSourceLength();
        }

        TEST_METHOD(Skip_ExecutionIsDeferred)
        {
            auto source = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());
            source->Skip(10);
        }

        TEST_METHOD(Skip_NegativeCount)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Skip(-5);

            int exp[] = { 0, 1, 2, 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Skip_ZeroCount)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Skip(0);

            int exp[] = { 0, 1, 2, 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Skip_NegativeCountWithArray)
        {
            int v[] = { 0, 1, 2, 3, 4 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto result = source->Skip(-5);

            int exp[] = { 0, 1, 2, 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Skip_ZeroCountWithArray)
        {
            int v[] = { 0, 1, 2, 3, 4 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto result = source->Skip(0);

            int exp[] = { 0, 1, 2, 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Skip_CountShorterThanSource)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Skip(3);

            int exp[] = { 3, 4 };
            Assert::IsTrue(result->SequenceEqual<int>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Skip_CountEqualToSourceLength)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Skip(5);

            int* exp = new int[0];
            Assert::IsTrue(result->SequenceEqual<int>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(Skip_CountGreaterThanSourceLength)
        {
            auto result = IEnumerable<int>::Range(0, 5)->Skip(100);

            int* exp = new int[0];
            Assert::IsTrue(result->SequenceEqual<int>(exp, 0));
            delete [] exp;
        }
    };

    TEST_CLASS(SkiWhileTest)
    {
    public:
        static void test()
        {
            SkiWhileTest t;
            t.SkipWhile_ExecutionIsDeferred();
            t.SkipWhile_PredicateFailingFirstElement();
            t.SkipWhile_PredicateWithIndexFailingFirstElement();
            t.SkipWhile_PredicateMatchingSomeElements();
            t.SkipWhile_PredicateWithIndexMatchingSomeElements();
            t.SkipWhile_PredicateMatchingAllElements();
            t.SkipWhile_PredicateWithIndexMatchingAllElements();
        }

        TEST_METHOD(SkipWhile_ExecutionIsDeferred)
        {
            auto source = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());
            source->SkipWhile([](int x) { return x > 10; });
        }
        /*
        TEST_METHOD(SkipWhile_NullPredicateNoIndex)
        {
            int v[] = { 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            std::function<bool(int)> predicate = nullptr;

            Assert::ExpectException<ArgumentNullException&>([source, predicate]() {
                source->SkipWhile(predicate);
            });
        }

        TEST_METHOD(SkipWhile_NullPredicateUsingIndex)
        {
            int v[] = { 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            std::function<bool(int, int)> predicate = nullptr;

            Assert::ExpectException<ArgumentNullException&>([source, predicate]() {
                source->SkipWhileIndex(predicate);
            });
        }
        */
        TEST_METHOD(SkipWhile_PredicateFailingFirstElement)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->SkipWhile([](std::string x) { return x.size() > 4; });

            std::string exp[] = { "zero", "one", "two", "three", "four", "five" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SkipWhile_PredicateWithIndexFailingFirstElement)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->SkipWhileIndex([](std::string x, int index) { return index + x.size() > 4; });

            std::string exp[] = { "zero", "one", "two", "three", "four", "five" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SkipWhile_PredicateMatchingSomeElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->SkipWhile([](std::string x) { return x.size() < 5; });

            std::string exp[] = { "three", "four", "five" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SkipWhile_PredicateWithIndexMatchingSomeElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->SkipWhileIndex([](std::string x, int index) { return (int)x.size() > index; });

            std::string exp[] = { "four", "five" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SkipWhile_PredicateMatchingAllElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->SkipWhile([](std::string x) { return x.size() < 100; });

            std::string* exp = new std::string[0];
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(SkipWhile_PredicateWithIndexMatchingAllElements)
        {
            std::string v[] = { "zero", "one", "two", "three", "four", "five" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            auto result = source->SkipWhileIndex([](std::string x, int index) { return x.size() < 100; });

            std::string* exp = new std::string[0];
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, 0));
            delete [] exp;
        }
    };
}
