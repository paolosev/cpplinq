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
    TEST_CLASS(DistinctTest)
    {
        // TestString1 and TestString2 are references to different but equal strings
        static const String TestString1;
        static const String TestString2;

    public:
        static void test()
        {
            fprintf(stdout, "distinct\n");

            DistinctTest t;
            t.Distinct_DistinctStringsWithCaseInsensitiveComparer();
            t.Distinct_NoComparerSpecifiedUsesDefault();
        }

        TEST_METHOD(Distinct_NoComparerSpecifiedUsesDefault)
        {
            String v[] = { "xyz", TestString1, "XYZ", TestString2, "def" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            String exp[] = { "xyz", TestString1, "XYZ", "def" };
            Assert::IsTrue(source->Distinct()->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Distinct_DistinctStringsWithCaseInsensitiveComparer)
        {
            String v[] = { "xyz", TestString1, "XYZ", TestString2, "def" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            String exp[] = { "xyz", TestString1, "def" };
            Assert::IsTrue(source->Distinct(CompareIgnoreCase())->SequenceEqual<String>(exp, ARRAYSIZE(exp)));
        }
    };

    // static
    const String DistinctTest::TestString1 = "test";
    const String DistinctTest::TestString2 = "test";
}
