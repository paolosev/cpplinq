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
    TEST_CLASS(ContainsTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "contains\n");

            ContainsTest t;
            t.Contains_NoMatchNoComparer();
            t.Contains_NoMatchNoComparer2();
            t.Contains_MatchNoComparer();
            t.Contains_NoMatchWithCustomComparer();
            t.Contains_MatchWithCustomComparer();
            t.Contains_ImmediateReturnWhenMatchIsFound();
        }

        TEST_METHOD(Contains_NoMatchNoComparer)
        {
            int v[] = { 11, 3, 69 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::IsFalse(source->Contains(42));
        }

        TEST_METHOD(Contains_NoMatchNoComparer2)
        {
            String v[] = { String("foo"), String("bar"), String("baz") };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            Assert::IsFalse(source->Contains(String("BAR")));
        }

        TEST_METHOD(Contains_MatchNoComparer)
        {
            // Default equality comparer is ordinal
            String v[] = { "foo", "bar", "baz" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            Assert::IsTrue(source->Contains(String("bar")));
        }

        //     TEST_METHOD(Contains_NoMatchNullComparer)
        //     {
        //         // Default equality comparer is ordinal
        //         String v[] = { "foo", "bar", "baz" };
        //         std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

        //         Assert::IsFalse(source->Contains(String("BAR"), nullptr));
        //     }

        //     TEST_METHOD(Contains_MatchNullComparer)
        //     {
        //         // Default equality comparer is ordinal
        //         String v[] = { "foo", "bar", "baz" };
        //         std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

        //         Assert::IsTrue(source->Contains(String("bar"), nullptr));
        //     }

        TEST_METHOD(Contains_NoMatchWithCustomComparer)
        {
            String v[] = { "foo", "bar", "baz" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            Assert::IsFalse(source->Contains("gronk", [](const String& lhs, const String& rhs){ return (lhs == rhs); }));
        }

        TEST_METHOD(Contains_MatchWithCustomComparer)
        {
            // Default equality comparer is ordinal
            String v[] = { "foo", "bar", "baz" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            Assert::IsTrue(source->Contains("BAR", [](const String& lhs, const String& rhs) {
                // compare ignore case
                return (lhs.CompareIgnoreCase(rhs));
            }));
        }

        TEST_METHOD(Contains_ImmediateReturnWhenMatchIsFound)
        {
            int v[] = { 10, 1, 5, 0 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            auto query = source->Select<int>([](int x) { return 10 / x; });

            // If we continued past 2, we'd see a division by zero exception
            Assert::IsTrue(query->Contains(2));
        }
    };
}
