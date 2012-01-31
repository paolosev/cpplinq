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
    TEST_CLASS(RepeatTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "repeat\n");

            RepeatTest t;
            t.Repeat_SimpleRepeat();
            t.Repeat_EmptyRepeat();
            t.Repeat_NegativeCount();
        }

        TEST_METHOD(Repeat_SimpleRepeat)
        {
            auto result = IEnumerable<std::string>::Repeat(std::string("foo"), 3);

            std::string exp[] = { "foo", "foo", "foo" };
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, ARRAYSIZE(exp)));
        }

        TEST_METHOD(Repeat_EmptyRepeat)
        {
            auto result = IEnumerable<std::string>::Repeat(std::string("foo"), 0);

            std::string* exp = new std::string[0];
            Assert::IsTrue(result->SequenceEqual<std::string>(exp, 0));
            delete [] exp;
        }

        TEST_METHOD(Repeat_NegativeCount)
        {
            Assert::ExpectException<ArgumentOutOfRangeException&>([]() {
                IEnumerable<std::string>::Repeat("foo", -1);
            });
        }
    };
}
