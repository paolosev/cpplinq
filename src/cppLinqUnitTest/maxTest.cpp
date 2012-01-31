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
    TEST_CLASS(MaxTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "max\n");

            MaxTest t;
            t.Max_EmptySequenceInt32NoSelector();
            t.Max_EmptySequenceInt32WithSelector();
            t.Max_SimpleSequenceInt32NoSelector();
            t.Max_SimpleSequenceInt32WithSelector();
            t.Max_SequenceContainingBothInfinities();
            t.Max_SimpleSequenceGenericNoSelector();
            t.Max_SimpleSequenceGenericWithSelector();
        }

        /*TEST_METHOD(Max_NullInt32Selector)
        {
            String v[] = { "" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            std::function<int(String)> selector = nullptr;

            Assert::ExpectException<ArgumentNullException&>([source, &selector]() {
                source->Max(selector);
            });
        }
        */
        TEST_METHOD(Max_EmptySequenceInt32NoSelector)
        {
            String* v = new String[0];
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Max();
            });

            delete [] v;
        }

        TEST_METHOD(Max_EmptySequenceInt32WithSelector)
        {
            String* v = new String[0];
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Max<int>([](String s) { return s.length(); });
            });

            delete [] v;
        }

        TEST_METHOD(Max_SimpleSequenceInt32NoSelector)
        {
            int v[] = { 5, 10, 6, 2, 13, 8 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(13, source->Max());
        }

        TEST_METHOD(Max_SimpleSequenceInt32WithSelector)
        {
            String v[] = { "xyz", "ab", "abcde", "0" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            Assert::AreEqual(5, source->Max<int>([](String x) { return x.length(); }));
        }

        TEST_METHOD(Max_SequenceContainingBothInfinities)
        {
            double v[] = { 1.0, DBL_MAX, DBL_MIN };
            std::shared_ptr<IEnumerable<double>> source(new Vector<double>(v, ARRAYSIZE(v)));

            Assert::AreEqual(DBL_MAX, source->Max());
        }

        TEST_METHOD(Max_SimpleSequenceGenericNoSelector)
        {
            std::string v = "alphabet soup";
            std::shared_ptr<IEnumerable<char>> source(new StlEnumerable<std::string, char>(v));

            Assert::AreEqual('u', source->Max());
        }

        TEST_METHOD(Max_SimpleSequenceGenericWithSelector)
        {
            std::string v[] = { "zyx", "ab", "abcde", "0" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            Assert::AreEqual('z', source->Max<char>([](std::string x) { return x[0]; }));
        }
    };
}
