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
    TEST_CLASS(SelectManyTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "selectMany\n");

            SelectManyTest t;
            t.SelectMany_SimpleFlatten();
            t.SelectMany_SimpleFlattenWithIndex();
            t.SelectMany_FlattenWithProjection();
            t.SelectMany_FlattenWithProjectionAndIndex();
        }

        TEST_METHOD(SelectMany_SimpleFlatten)
        {
            int v[] = { 3, 5, 20, 15 };
            std::shared_ptr<IEnumerable<int>> numbers(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = numbers->SelectMany<char>([](int x) -> std::shared_ptr<IEnumerable<char>> {
                return std::shared_ptr<IEnumerable<char>>(new String(IntToString(x)));
            });

            char exp[] = { '3', '5', '2', '0', '1', '5' };
            Assert::IsTrue(query->SequenceEqual<char>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SelectMany_SimpleFlattenWithIndex)
        {
            int v[] = { 3, 5, 20, 15 };
            std::shared_ptr<IEnumerable<int>> numbers(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = numbers->SelectManyIndex<char>([](int x, int index) -> std::shared_ptr<IEnumerable<char>> {
                std::string s = IntToString(x + index);
                return std::shared_ptr<IEnumerable<char>>(new String(s));
            });
            // 3 => '3'
            // 5 => '6'
            // 20 => '2', '2'
            // 15 => '1', '8'
            char exp[] = { '3', '6', '2', '2', '1', '8' };
            Assert::IsTrue(query->SequenceEqual<char>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SelectMany_FlattenWithProjection)
        {
            int v[] = { 3, 5, 20, 15 };
            std::shared_ptr<IEnumerable<int>> numbers(new Vector<int>(v, ARRAYSIZE(v)));

            // Flatten each number to its constituent characters, but then project each character
            // to a string of the original element which is responsible for "creating" that character,
            // as well as the character itself. So 20 will go to "20: 2" and "20: 0".
            auto query = numbers->SelectMany<std::string, char>(
                [](int x) { return std::shared_ptr<IEnumerable<char>>(new String(IntToString(x))); },
                [](int x, char c) { return IntToString(x) + ": " + c; }
            );

            std::string exp[] = { "3: 3", "5: 5", "20: 2", "20: 0", "15: 1", "15: 5" };
            Assert::IsTrue(query->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(SelectMany_FlattenWithProjectionAndIndex)
        {
            int v[] = { 3, 5, 20, 15 };
            std::shared_ptr<IEnumerable<int>> numbers(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = numbers->SelectManyIndex<std::string, char>(
                [](int x, int index) { return std::shared_ptr<IEnumerable<char>>(new String(IntToString(x + index))); },
                [](int x, char c) { return IntToString(x) + ": " + c; }
            );
            // 3 => "3: 3"
            // 5 => "5: 6"
            // 20 => "20: 2", "20: 2"
            // 15 => "15: 1", "15: 8"
            std::string exp[] = { "3: 3", "5: 6", "20: 2", "20: 2", "15: 1", "15: 8" };
            Assert::IsTrue(query->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }
    };
}
