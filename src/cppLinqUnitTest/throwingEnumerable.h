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

#pragma once

#include "CppUnitTest.h"
#include "../cpplinq/ienumerable.h"

class ThrowingEnumerable : public IEnumerable<int>
{
public:
    virtual std::shared_ptr<IEnumerator<int>> GetEnumerator() {
        throw InvalidOperationException();
    }

    // Check that the given function uses deferred execution.
    // A "spiked" source is given to the function: the function
    // call itself shouldn't throw an exception. However, using
    // the result (by calling GetEnumerator() then MoveNext() on it) *should*
    // throw InvalidOperationException.
    template <typename DeferredFunction>
    static void AssertDeferred(DeferredFunction deferredFunction)
    {
        auto source = std::shared_ptr<IEnumerable<int>>(new ThrowingEnumerable());
        auto result = deferredFunction(source);
        auto iterator = result->GetEnumerator();

        Assert::ExpectException<std::exception&>([=]() { iterator->MoveNext(); });
    }
};

