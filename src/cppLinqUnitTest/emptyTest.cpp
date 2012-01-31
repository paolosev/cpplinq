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
    TEST_CLASS(EmptyTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "empty\n");

            EmptyTest t;
            t.Empty_EmptyContainsNoElements();
            t.Empty_EmptyIsASingletonPerElementType();
        }

        TEST_METHOD(Empty_EmptyContainsNoElements)
        {
            auto empty = IEnumerable<int>::Empty()->GetEnumerator();

            Assert::IsFalse(empty->MoveNext());
        }

        TEST_METHOD(Empty_EmptyIsASingletonPerElementType)
        {
            Assert::IsTrue(IEnumerable<int>::Empty().get() == IEnumerable<int>::Empty().get());
            Assert::IsTrue(IEnumerable<long>::Empty().get() == IEnumerable<long>::Empty().get());
            Assert::IsTrue(IEnumerable<std::string>::Empty().get() == IEnumerable<std::string>::Empty().get());
            Assert::IsFalse((void*)IEnumerable<long>::Empty().get() == (void*)IEnumerable<int>::Empty().get());
        }
    };
}
