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
    TEST_CLASS(ElementAtTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "elementAt\n");

            ElementAtTest t;
            t.ElementAt_NegativeIndex();
            t.ElementAt_OvershootIndexOnVector();
            t.ElementAt_OvershootIndexOnLazySequence();
            t.ElementAt_ValidIndexOnVector();
            t.ElementAt_ValidIndexOnLazySequence();
        }

        TEST_METHOD(ElementAt_NegativeIndex)
        {
            int v[] = { 0, 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<ArgumentOutOfRangeException&>( [&source](){ source->ElementAt(-1); });
        }

        TEST_METHOD(ElementAt_OvershootIndexOnVector)
        {
            int v[] = { 0, 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::ExpectException<ArgumentOutOfRangeException&>( [&source](){ source->ElementAt(3); });
        }

        TEST_METHOD(ElementAt_OvershootIndexOnLazySequence)
        {
            std::shared_ptr<IEnumerable<int>> source = IEnumerable<int>::Range(0, 3);

            Assert::ExpectException<ArgumentOutOfRangeException&>( [&source](){ source->ElementAt(3); });
        }

        TEST_METHOD(ElementAt_ValidIndexOnVector)
        {
            int v[] = { 100, 56, 93, 22 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(93, source->ElementAt(2));
        }

        TEST_METHOD(ElementAt_ValidIndexOnLazySequence)
        {
            std::shared_ptr<IEnumerable<int>> source = IEnumerable<int>::Range(10, 5);

            Assert::AreEqual(12, source->ElementAt(2));
        }
    };

    TEST_CLASS(ElementAtOrDefaultTest)
    {
    public:
        static void test()
        {
            ElementAtOrDefaultTest t;
            t.ElementAtOrDefault_NegativeIndex();
            t.ElementAtOrDefault_OvershootIndexOnList();
            t.ElementAtOrDefault_OvershootIndexOnLazySequence();
            t.ElementAtOrDefault_ValidIndexOnList();
            t.ElementAtOrDefault_ValidIndexOnLazySequence();
        }

        TEST_METHOD(ElementAtOrDefault_NegativeIndex)
        {
            int v[] = { 0, 1, 2 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(0, source->ElementAtOrDefault(-1));
            Assert::AreEqual(int(), source->ElementAtOrDefault(-1));
        }

        TEST_METHOD(ElementAtOrDefault_OvershootIndexOnList)
        {
            int v[] = { 90, 91, 92 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(0, source->ElementAtOrDefault(3));
        }

        TEST_METHOD(ElementAtOrDefault_OvershootIndexOnLazySequence)
        {
            std::shared_ptr<IEnumerable<int>> source = IEnumerable<int>::Range(0, 3);

            Assert::AreEqual(0, source->ElementAtOrDefault(3));
        }

        TEST_METHOD(ElementAtOrDefault_ValidIndexOnList)
        {
            int v[] = { 100, 56, 93, 22 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(93, source->ElementAtOrDefault(2));
        }

        TEST_METHOD(ElementAtOrDefault_ValidIndexOnLazySequence)
        {
            std::shared_ptr<IEnumerable<int>> source = IEnumerable<int>::Range(10, 5);

            Assert::AreEqual(12, source->ElementAtOrDefault(2));
        }
    };
}
