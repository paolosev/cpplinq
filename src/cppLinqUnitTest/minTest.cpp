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
    TEST_CLASS(MinTest)
    {
    public:
        static void test()
        {
            fprintf(stdout, "min\n");

            MinTest t;
            t.Min_EmptySequenceInt32NoSelector();
            t.Min_EmptySequenceInt32WithSelector();
            t.Min_SimpleSequenceInt32NoSelector();
            t.Min_SimpleSequenceInt32WithSelector();
            t.Min_SimpleSequenceDouble();
            t.Min_SequenceContainingBothInfinities();
            t.Min_EmptyCharSequenceGenericNoSelector();
            t.Min_EmptyCharSequenceGenericWithSelector();
            t.Min_SimpleSequenceGenericNoSelector();
            t.Min_SimpleSequenceGenericWithSelector();
        }

        /*TEST_METHOD(Min_NullInt32Selector)
        {
            String v[] = { "" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            std::function<int(String)> selector = nullptr;

            Assert::ExpectException<ArgumentNullException&>([source, &selector]() {
                source->Min(selector);
            });
        }
        */
        TEST_METHOD(Min_EmptySequenceInt32NoSelector)
        {
            String* v = new String[0];
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Min();
            });

            delete [] v;
        }

        TEST_METHOD(Min_EmptySequenceInt32WithSelector)
        {
            String* v = new String[0];
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Min<int>([](String s) { return s.length(); });
            });

            delete [] v;
        }

        TEST_METHOD(Min_SimpleSequenceInt32NoSelector)
        {
            int v[] = { 5, 10, 6, 2, 13, 8 };
            std::shared_ptr<IEnumerable<int>> source(new Vector<int>(v, ARRAYSIZE(v)));

            Assert::AreEqual(2, source->Min());
        }

        TEST_METHOD(Min_SimpleSequenceInt32WithSelector)
        {
            String v[] = { "xyz", "ab", "abcde", "0" };
            std::shared_ptr<IEnumerable<String>> source(new Vector<String>(v, ARRAYSIZE(v)));

            Assert::AreEqual(1, source->Min<int>([](String x) { return x.length(); }));
        }

        TEST_METHOD(Min_SimpleSequenceDouble)
        {
            double v[] = { -2.5, 2.5, 0.0 };
            std::shared_ptr<IEnumerable<double>> source(new Vector<double>(v, ARRAYSIZE(v)));

            Assert::AreEqual(-2.5, source->Min());
        }

        TEST_METHOD(Min_SequenceContainingBothInfinities)
        {
            double v[] = { 1.0, DBL_MIN, DBL_MIN };
            std::shared_ptr<IEnumerable<double>> source(new Vector<double>(v, ARRAYSIZE(v)));

            Assert::AreEqual(DBL_MIN, source->Min());
        }

        TEST_METHOD(Min_EmptyCharSequenceGenericNoSelector)
        {
            std::string v = "";
            std::shared_ptr<IEnumerable<char>> source(new StlEnumerable<std::string, char>(v));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Min();
            });
        }

        TEST_METHOD(Min_EmptyCharSequenceGenericWithSelector)
        {
            std::string* v = new std::string[0];
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, 0));

            Assert::ExpectException<InvalidOperationException&>([source]() {
                source->Min<char>([](std::string x) { return x[0]; });
            });

            delete [] v;
        }

        TEST_METHOD(Min_SimpleSequenceGenericNoSelector)
        {
            std::string v = "alphabet soup";
            std::shared_ptr<IEnumerable<char>> source(new StlEnumerable<std::string, char>(v));

            Assert::AreEqual(' ', source->Min());
        }

        TEST_METHOD(Min_SimpleSequenceGenericWithSelector)
        {
            std::string v[] = { "zyx", "ab", "abcde", "0" };
            std::shared_ptr<IEnumerable<std::string>> source(new Vector<std::string>(v, ARRAYSIZE(v)));

            Assert::AreEqual('0', source->Min<char>([](std::string x) { return x[0]; }));
        }
    };
}
