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
    TEST_CLASS(AverageTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "average\n");

            AverageTest t;
            t.Average_EmptySequenceInt32NoSelector();
            t.Average_EmptySequenceInt32WithSelector();
            t.Average_SimpleAverageInt32NoSelector();
            t.Average_SimpleAverageDoubleNoSelector();
            t.Average_SimpleAverageInt32WithSelector();
        }

        TEST_METHOD(Average_EmptySequenceInt32NoSelector)
        {
            int* p = new int[0];
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(p, 0));
            Assert::ExpectException<InvalidOperationException&>([&]() { source->Average(); });
            delete [] p;
        }

        TEST_METHOD(Average_EmptySequenceInt32WithSelector)
        {
            std::string* p = new std::string[0];
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(p, 0));
            Assert::ExpectException<InvalidOperationException&>([&]() {
                source->Average<int>([](const std::string& s) {
                    return s.length();
                });
            });
            delete [] p;
        }

        TEST_METHOD(Average_SimpleAverageInt32NoSelector)
        {
            // Note that 7.5 is exactly representable as a double, so we
            // shouldn't need to worry about floating-point inaccuracies
            int v[] = { 5, 10, 0, 15 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(7.5, source->Average());
        }

        TEST_METHOD(Average_SimpleAverageDoubleNoSelector)
        {
            // Note that 7.5 is exactly representable as a double, so we
            // shouldn't need to worry about floating-point inaccuracies
            double v[] = { 5.0, 10.0, 0.0, 15.0 };
            std::shared_ptr<IEnumerable<double>> source(new Vector<double>(v, ARRAYSIZE(v)));

            Assert::AreEqual(7.5, source->Average());
        }

        TEST_METHOD(Average_SimpleAverageInt32WithSelector)
        {
            std::string v[] = { "", "abcd", "a", "b" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            Assert::AreEqual(1.5, source->Average<int>([](const std::string& x) { return x.length(); }));
        }
    };
}
