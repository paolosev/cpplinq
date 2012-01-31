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

#include <memory>
#include <functional>
#include <set>
#include <stack>
#include "exceptions.h"
#include "fiber.h"

template <typename T> 
class IEnumerator
{
public:
    virtual void Reset() = 0;
    virtual bool MoveNext() = 0;
    virtual T& get_Current() = 0;
    virtual ~IEnumerator() {}
};

template <typename T> 
class IEnumerable : public std::enable_shared_from_this<IEnumerable<T>>
{
public:
    virtual std::shared_ptr<IEnumerator<T>> GetEnumerator() = 0;
    virtual ~IEnumerable() {}

    ////////////////////////////////////////////////////////////////////////////
    // Aggregate

    template <typename Func>
    T Aggregate(Func func) {
        return _Aggregate<T, Func>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), func);
    }
    T Aggregate(std::function<T(T, T)> func) {
        if (nullptr == func) {
            throw ArgumentNullException();
        }
        return _Aggregate<T, decltype(func)>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), func);
    }

    template <typename TAccumulate, typename Func>
    TAccumulate Aggregate(const TAccumulate& seed, Func func) {
        return _Aggregate<T, TAccumulate, Func>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), seed, func);
    }
    template <typename TAccumulate>
    TAccumulate Aggregate(const TAccumulate& seed, std::function<TAccumulate(TAccumulate, T)> func) {
        if (nullptr == func) {
            throw ArgumentNullException();
        }
        return _Aggregate<T, TAccumulate, decltype(func)>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), seed, func);
    }

    template <typename TAccumulate, typename TResult, typename Func, typename ResultSelector>
    TResult Aggregate(const TAccumulate& seed, Func func, ResultSelector resultSelector) {
        return _Aggregate<T, TAccumulate, TResult, Func, ResultSelector>(
            std::shared_ptr<IEnumerable<T>>(shared_from_this()), seed, func, resultSelector);
    }
    template <typename TAccumulate, typename TResult>
    TResult Aggregate(const TAccumulate& seed, std::function<TAccumulate(TAccumulate, T)> func, std::function<TResult(TAccumulate)> resultSelector) {
        if (nullptr == func || nullptr == resultSelector) {
            throw ArgumentNullException();
        }
        return _Aggregate<T, TAccumulate, TResult, decltype(func), decltype(resultSelector)>(
            std::shared_ptr<IEnumerable<T>>(shared_from_this()), seed, func, resultSelector);
    }

    template <typename TSource, typename Func>
    TSource _Aggregate(
        const std::shared_ptr<IEnumerable<TSource>> source,
        Func func)
    {
        TSource result;
        bool first = true;
        foreach<TSource>(source, [&result, &first, func](TSource& item) {
            if (first) {
                first = false;
                result = item;
            }
            else {
                result = func(result, item);
            }
        });
        if (first) {
            throw InvalidOperationException();
        }
        return result;
    }

    template <typename TSource, typename TAccumulate, typename Func>
    TAccumulate _Aggregate(
        const std::shared_ptr<IEnumerable<TSource>> source,
        const TAccumulate& seed,
        Func func)
    {
        TAccumulate result = seed;
        foreach<TSource>(source, [&result, func](TSource& item) {
            result = func(result, item);
        });
        return result;
    }

    template <typename TSource, typename TAccumulate, typename TResult, typename Func, typename ResultSelector>
    TResult _Aggregate(
        const std::shared_ptr<IEnumerable<TSource>> source,
        const TAccumulate& seed,
        Func func,
        ResultSelector resultSelector)
    {
        TAccumulate result = seed;
        foreach<TSource>(source, [&result, func](TSource& item) {
            result = func(result, item);
        });
        return resultSelector(result);
    };

    ////////////////////////////////////////////////////////////////////////////
    // All

    // Determines whether all elements of a sequence satisfy a condition.
    template <typename Predicate>
    bool All(Predicate predicate) {
        // code review: how to implement foreach loops that return values?
        //	foreach<TSource>(source, [predicate](TSource& item) -> bool {
        //		if (! predicate(item)) {
        //			return false;
        //		}
        //	});
        //	return true;

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext())
        {
            if (! predicate(e->get_Current())) {
                return false;
            }
        }
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Any

    // Determines whether a sequence contains any elements.
    bool Any() {
        return Any([](const T& item) { return true; });
    }

    // Determines whether any element of a sequence satisfies a condition.
    template <typename Predicate>
    bool Any(Predicate predicate)
    {
        std::function<bool(const T&)> func = [predicate](const T& item) { return !predicate(item); };
        return !All<decltype(func)>(func); 
    }

    ////////////////////////////////////////////////////////////////////////////
    // Average

    // Computes the average of a sequence of T values.
    double Average() {
        long count = 0;
        T total = 0;

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext())
        {
            // code review: overflow?
            total += e->get_Current();
            count++;
        }

        if (count == 0) {
            throw InvalidOperationException("Sequence was empty");
        }

        return (double)total / (double)count;
    }

    // Computes the average of a sequence of TResult values that are obtained by invoking
    // a transform function on each element of the input sequence.
    template <typename TResult, typename Selector>
    double Average(Selector selector) {
        return Select<TResult, Selector>(selector)->Average();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Concat

    // Concatenates two sequences.
    std::shared_ptr<IEnumerable<T>> Concat(std::shared_ptr<IEnumerable<T>> rhs) {
        if (! rhs) {
            throw ArgumentNullException();
        }

        // deferred
        std::shared_ptr<IEnumerable<T>> lhs = shared_from_this();
        auto fn =  [lhs, rhs](IteratorBlock<T>* it) {
            foreach<T>(lhs, [it](T& item) { 
                it->yieldReturn(item);
            });

            foreach<T>(rhs, [it](T& item) { 
                it->yieldReturn(item);
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Contains

    // Determines whether a sequence contains a specified element by using the default equality comparer.
    bool Contains(const T& value) {
        return Any([value](const T& item) { return value == item; });
    }
    // Code review: does not compile with T == std::string

    // Determines whether a sequence contains a specified element by using a specified Comparer function.
    template <typename Predicate>
    bool Contains(const T& value, Predicate comparer) {
        // this does not compile with VS11 developer preview
        //if (comparer == nullptr) {
        //	return false;
        //}
        std::function<bool(const T& rhs)> func = [&value, &comparer](const T& item) { return comparer(value, item); };
        return Any<decltype(func)>(func);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Count

    // Returns the number of elements in a sequence.
    int Count() {
        int count = 0;
        foreach<T>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), [&count](const T& item) {
            if (count == INT_MAX) {
                throw OverflowException();
            }
            count++;
        });
        return count;
    }

    // Returns a number that represents how many elements in the specified sequence satisfy a condition.
    template <typename Predicate>
    int Count(Predicate predicate) {
        //if (predicate == nullptr) {
        //	throw ArgumentNullException("predicate");
        //}

        int count = 0;
        foreach<T>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), [&count, predicate](const T& item) {
            if (predicate(item)) {
                if (count == INT_MAX) {
                    throw OverflowException();
                }
                count++;
            }
        });
        return count;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Distinct

    // Returns distinct elements from a sequence.
    std::shared_ptr<IEnumerable<T>> Distinct() {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source](IteratorBlock<T>* it) {
            std::set<T> seenElements;
            foreach<T>(source, [it, &seenElements](T item) { 
                std::pair<std::set<T>::iterator, bool> result = seenElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    // Returns distinct elements from a sequence by using a specified Comparer to compare values.
    template <typename Comparer>
    std::shared_ptr<IEnumerable<T>> Distinct(Comparer comparer) {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, comparer](IteratorBlock<T>* it) {
            std::set<T, Comparer> seenElements(comparer);
            foreach<T>(source, [it, &seenElements](T item) { 
                std::pair<std::set<T, Comparer>::iterator, bool> result = seenElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // ElementAt

    T ElementAt(int index)
    {
        T ret;
        if (!TryElementAt(index, ret)) {
            throw ArgumentOutOfRangeException("index");
        }
        return ret;
    }

    T ElementAtOrDefault(int index)
    {
        T ret;
        // We don't care about the return value - ret will be T() if it's false
        TryElementAt(index, ret);
        return ret;
    }

    bool TryElementAt(int index, T& element)
    {
        // type T must have a default constructor
        element = T();

        if (index < 0) { 
            return false;
        }

        // We don't need to fetch the current value each time - get to the right place first.
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();

        // Note use of -1 so that we start off my moving onto element 0.
        // Don't want to use i <= index in case index == int.MaxValue!
        for (int i = -1; i < index; i++)
        {
            if (!iterator->MoveNext()) {
                return false;
            }
        }
        element = iterator->get_Current();
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Empty

    static std::shared_ptr<IEnumerable<T>> Empty() {
        // deferred
        auto fn =  [](IteratorBlock<T>* it) {
            it->yieldBreak();
        };
        static std::shared_ptr<IEnumerable<T>> instance(new _IteratorBlock<T>(fn));
        return instance;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Except

    std::shared_ptr<IEnumerable<T>> Except(std::shared_ptr<IEnumerable<T>> rhs) {
        if (rhs.get() == nullptr) {
            throw ArgumentNullException();
        }

        // deferred execution
        std::shared_ptr<IEnumerable<T>> lhs = shared_from_this();
        auto fn =  [lhs, rhs](IteratorBlock<T>* it) {
            std::set<T> bannedElements;
            foreach<T>(rhs, [&bannedElements](T& item) { 
                bannedElements.insert(item);
            } );

            foreach<T>(lhs, [it, &bannedElements](T& item) { 
                std::pair<std::set<T>::iterator, bool> result = bannedElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            } );
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    template <typename Comparer>
    std::shared_ptr<IEnumerable<T>> Except(std::shared_ptr<IEnumerable<T>> rhs, Comparer comparer) {
        //if (comparer == nullptr) {
        //	return Except(rhs);
        //}

        if (rhs.get() == nullptr) {
            throw ArgumentNullException();
        }

        // deferred execution
        std::shared_ptr<IEnumerable<T>> lhs = shared_from_this();
        auto fn =  [lhs, rhs, comparer](IteratorBlock<T>* it) {
            std::set<T, Comparer> bannedElements(comparer);
            foreach<T>(rhs, [&bannedElements](T& item) { 
                bannedElements.insert(item);
            } );

            foreach<T>(lhs, [it, &bannedElements](T& item) { 
                std::pair<std::set<T, Comparer>::iterator, bool> result = bannedElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            } );
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // First

    T First() {
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        if (iterator->MoveNext()) {
            return iterator->get_Current();
        }

        throw InvalidOperationException("Sequence was empty");
    }

    template <typename Predicate>
    T First(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext()) {
            T& item = e->get_Current();
            if (predicate(item)) {
                return item;
            }
        }

        throw InvalidOperationException("No items matched the predicate");
    }

    ////////////////////////////////////////////////////////////////////////////
    // FirstOrDefault

    T FirstOrDefault() {
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        return (iterator->MoveNext()) ? iterator->get_Current() : T();
    }

    template <typename Predicate>
    T FirstOrDefault(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext()) {
            T& item = e->get_Current();
            if (predicate(item)) {
                return item;
            }
        }

        return T();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Last

    T Last() {
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        if (!iterator->MoveNext()) {
            throw InvalidOperationException("Sequence was empty");
        }

        T last = iterator->get_Current();
        while (iterator->MoveNext()) {
            last = iterator->get_Current();
        }
        return last;
    }

    template <typename Predicate>
    T Last(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        bool foundAny = false;
        T last = T();

        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        while (iterator->MoveNext()) {
            T& item = iterator->get_Current();
            if (predicate(item)) {
                foundAny = true;
                last = item;
            }
        }

        if (!foundAny) {
            throw InvalidOperationException("No items matched the predicate");
        }
        return last;
    }

    ////////////////////////////////////////////////////////////////////////////
    // LastOrDefault

    T LastOrDefault() {
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();

        if (!iterator->MoveNext()) {
            return T();
        }

        T last = iterator->get_Current();
        while (iterator->MoveNext()) {
            last = iterator->get_Current();
        }
        return last;
    }

    template <typename Predicate>
    T LastOrDefault(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        bool foundAny = false;
        T last = T();

        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        while (iterator->MoveNext()) {
            T& item = iterator->get_Current();
            if (predicate(item)) {
                foundAny = true;
                last = item;
            }
        }

        return foundAny ? last : T();
    }

    ////////////////////////////////////////////////////////////////////////////
    // LongCount

    // Returns the number of elements in a sequence.
    __int64 LongCount() {
        __int64 count = 0;
        foreach<T>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), [&count](const T& item) {
            if (count == INT64_MAX) {
                throw OverflowException();
            }
            count++;
        });
        return count;
    }

    // Returns a number that represents how many elements in the specified sequence satisfy a condition.
    template <typename Predicate>
    __int64 LongCount(Predicate predicate) {
        //if (predicate == nullptr) {
        //	throw ArgumentNullException("predicate");
        //}

        __int64 count = 0;
        foreach<T>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), [&count, predicate](const T& item) {
            if (predicate(item)) {
                if (count == INT64_MAX) {
                    throw OverflowException();
                }
                count++;
            }
        });
        return count;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Max

    T Max() {
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        if (! iterator->MoveNext()) {
            throw InvalidOperationException("Sequence was empty");
        }

        T max = iterator->get_Current();
        while (iterator->MoveNext())
        {
            T& item = iterator->get_Current();
            if (max < item) {
                max = item;
            }
        }
        return max;
    }

    template <typename TResult, typename Selector>
    TResult Max(Selector selector) {
        return Select<TResult>(selector)->Max();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Min

    T Min() {
        std::shared_ptr<IEnumerator<T>> iterator = GetEnumerator();
        if (! iterator->MoveNext()) {
            throw InvalidOperationException("Sequence was empty");
        }

        T min = iterator->get_Current();
        while (iterator->MoveNext())
        {
            T& item = iterator->get_Current();
            if (min > item) {
                min = item;
            }
        }
        return min;
    }

    template <typename TResult, typename Selector>
    TResult Min(Selector selector) {
        return Select<TResult>(selector)->Min();
    }

    ////////////////////////////////////////////////////////////////////////////
    // OrderBy
    /*
    // forward declaration
    //template <typename T> struct IComparer;
    //template <typename T, typename TKey, typename KeySelector> struct IOrderedEnumerable;

    template <typename TKey>
    std::shared_ptr<IOrderedEnumerable<T, TKey, std::function<TKey(T)>>> OrderBy(
    std::function<TKey(T)> keySelector,
    IComparer<T>* comparer)
    {
    if (keySelector == nullptr) {
    throw ArgumentNullException("keySelector");
    }

    IComparer<T>* sourceComparer = new ProjectionComparer<T, TKey>(keySelector, comparer);
    sourceComparer = new ReverseComparer<T>(sourceComparer);
    return std::shared_ptr<IOrderedEnumerable<T, TKey, std::function<TKey(T)>>>(
    new OrderedEnumerable<T, TKey, std::function<TKey(T)>>(std::shared_ptr<IEnumerable<T>>(shared_from_this()), sourceComparer));
    }
    */
    ////////////////////////////////////////////////////////////////////////////
    // Range

    // Generates a sequence of integral numbers within a specified range.
    static std::shared_ptr<IEnumerable<int>> Range(int start, int count) {
        if (count < 0) {
            throw ArgumentOutOfRangeException("count");
        }

        // Convert everything to __int64 to avoid overflows.
        if ((__int64)start + (__int64)count - 1 > INT_MAX) {
            throw ArgumentOutOfRangeException("count");
        }

        // deferred
        auto fn =  [start, count](IteratorBlock<int>* it) {
            for (int i = 0; i < count; i++) {
                it->yieldReturn(start + i);
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Repeat

    // Generates a sequence that contains one repeated value.
    static std::shared_ptr<IEnumerable<T>> Repeat(T element, int count) {
        if (count < 0) {
            throw ArgumentOutOfRangeException("count");
        }

        // deferred
        auto fn =  [element, count](IteratorBlock<T>* it) {
            for (int i = 0; i < count; i++) {
                it->yieldReturn(element);
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Reverse

    // Inverts the order of the elements in a sequence.
    std::shared_ptr<IEnumerable<T>> Reverse() {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source](IteratorBlock<T>* it) {
            std::stack<T> stack;
            foreach<T>(source, [it, &stack](T& item) { 
                stack.push(item);
            } );

            while (! stack.empty()) {
                T item = stack.top();
                stack.pop();
                it->yieldReturn(item);
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Select

    // Projects each element of a sequence into a new form.
    template <typename TResult, typename Selector>
    std::shared_ptr<IEnumerable<TResult>> Select(Selector selector) {
        //if (nullptr == selector) {
        //	throw ArgumentNullException("selector");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, selector](IteratorBlock<TResult>* it) {
            foreach<T>(source, [it, selector](T& item) {
                it->yieldReturn(selector(item));
            });
        };
        return std::shared_ptr<IEnumerable<TResult>>(new _IteratorBlock<TResult>(fn));
    }

    // Projects each element of a sequence into a new form by incorporating the element's index.
    template <typename TResult, typename Selector>
    std::shared_ptr<IEnumerable<TResult>> SelectIndex(Selector selector) {
        //if (nullptr == selector) {
        //	throw ArgumentNullException("selector");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, selector](IteratorBlock<TResult>* it) {
            int index = 0;
            foreach<T>(source, [it, &index, selector](T& item) {
                it->yieldReturn(selector(item, index));
                index++;
            });
        };
        return std::shared_ptr<IEnumerable<TResult>>(new _IteratorBlock<TResult>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // SelectMany

    template <typename TResult, typename Selector>
    std::shared_ptr<IEnumerable<TResult>> SelectMany(Selector selector) {
        //if (selector == nullptr) {
        //	throw ArgumentNullException("selector");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, selector](IteratorBlock<TResult>* it) {
            foreach<T>(source, [it, selector](T& item) {
                std::shared_ptr<IEnumerable<TResult>> e = selector(item);
                foreach<TResult>(e, [it](TResult result) {
                    it->yieldReturn(result);
                });
            });
        };
        return std::shared_ptr<IEnumerable<TResult>>(new _IteratorBlock<TResult>(fn));
    }

    template <typename TResult, typename TCollection, typename CollectionSelector, typename ResultSelector>
    std::shared_ptr<IEnumerable<TResult>> SelectMany(CollectionSelector collectionSelector, ResultSelector resultSelector) {
        //if (collectionSelector == nullptr || resultSelector == nullptr) {
        //	throw ArgumentNullException("selector");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, collectionSelector, resultSelector](IteratorBlock<TResult>* it) {
            foreach<T>(source, [it, collectionSelector, resultSelector](T& item) {
                std::shared_ptr<IEnumerable<TCollection>> e = collectionSelector(item);

                foreach<TCollection>(e, [it, resultSelector, item](TCollection collectionItem) {
                    it->yieldReturn(resultSelector(item, collectionItem));
                });
            });
        };
        return std::shared_ptr<IEnumerable<TResult>>(new _IteratorBlock<TResult>(fn));
    }

    template <typename TResult, typename Selector>
    std::shared_ptr<IEnumerable<TResult>> SelectManyIndex(Selector selector) {
        //if (selector == nullptr) {
        //	throw ArgumentNullException("selector");
        //}
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, selector](IteratorBlock<TResult>* it) {
            int index = 0;
            foreach<T>(source, [it, selector, &index](T& item) {
                std::shared_ptr<IEnumerable<TResult>> e = selector(item, index);

                foreach<TResult>(e, [it, &index, item](TResult result) {
                    it->yieldReturn(result);
                });

                index++;
            });
        };
        return std::shared_ptr<IEnumerable<TResult>>(new _IteratorBlock<TResult>(fn));
    }

    template <typename TResult, typename TCollection, typename CollectionSelector, typename ResultSelector>
    std::shared_ptr<IEnumerable<TResult>> SelectManyIndex(CollectionSelector collectionSelector, ResultSelector resultSelector) {
        //if (collectionSelector == nullptr || resultSelector == nullptr) {
        //	throw ArgumentNullException("selector");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, collectionSelector, resultSelector](IteratorBlock<TResult>* it) {
            int index = 0;
            foreach<T>(source, [it, collectionSelector, resultSelector, &index](T& item) {
                std::shared_ptr<IEnumerable<TCollection>> e = collectionSelector(item, index);

                foreach<TCollection>(e, [it, resultSelector, item](TCollection collectionItem) {
                    it->yieldReturn(resultSelector(item, collectionItem));
                });

                index++;
            });
        };
        return std::shared_ptr<IEnumerable<TResult>>(new _IteratorBlock<TResult>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // SequenceEqual

    // Determines whether two sequences are equal by comparing the elements by using the default equality comparer for their type.
    bool SequenceEqual(std::shared_ptr<IEnumerable<T>> rhs) {
        if (rhs == null) {
            throw ArgumentNullException("rhs");
        }

        std::shared_ptr<IEnumerator<T>> lEnum = GetEnumerator();
        std::shared_ptr<IEnumerator<T>> rEnum = rhs->GetEnumerator();

        while (true) {
            bool lNext = lEnum->MoveNext();
            bool rNext = rEnum->MoveNext();

            // Sequences aren't of same length. We don't care which way round.
            if (lNext != rNext) {
                return false;
            }

            // Both sequences have finished - done
            if (!lNext) {
                return true;
            }

            if (!(lNext->get_Current() == rNext->get_Current())) {
                return false;
            }
        }
    }

    template <typename T>
    bool SequenceEqual(const T* p, size_t len) {
        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        for (size_t i = 0; i < len; i++) {
            if ((! e->MoveNext()) || (!(e->get_Current() == p[i]))) {
                return false;
            }
        }
        return ! e->MoveNext();
    }

    template <>
    bool SequenceEqual<std::string>(const std::string* p, size_t len) {
        std::shared_ptr<IEnumerator<std::string>> e = GetEnumerator();
        for (size_t i = 0; i < len; i++) {
            if ((! e->MoveNext()) || 0 != strcmp(e->get_Current().c_str(), p[i].c_str())) {
                return false;
            }
        }
        return ! e->MoveNext();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Single

    T Single() {
        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();

        if (!e->MoveNext()) {
            throw InvalidOperationException("Sequence was empty");
        }

        T ret = e->get_Current();
        if (e->MoveNext()) {
            throw InvalidOperationException("Sequence contained multiple elements");
        }

        return ret;
    }

    template <typename Predicate>
    T Single(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        T ret;
        bool foundAny = false;

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext()) {
            T& item = e->get_Current();
            if (predicate(item)) {
                if (foundAny) {
                    throw InvalidOperationException("Sequence contained multiple matching elements");
                }
                foundAny = true;
                ret = item;
            }
        }

        if (! foundAny) {
            throw InvalidOperationException("No items matched the predicate");
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////////////////////
    // SingleOrDefault

    T SingleOrDefault() {
        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();

        if (!e->MoveNext()) {
            return T();
        }

        T ret = e->get_Current();
        if (e->MoveNext()) {
            throw InvalidOperationException("Sequence contained multiple elements");
        }

        return ret;
    }

    template <typename Predicate>
    T SingleOrDefault(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        T ret = T();
        bool foundAny = false;

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext()) {
            T& item = e->get_Current();
            if (predicate(item)) {
                if (foundAny) {
                    throw InvalidOperationException("Sequence contained multiple matching elements");
                }
                foundAny = true;
                ret = item;
            }
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Skip

    // Bypasses a specified number of elements in a sequence and then returns the remaining elements.
    std::shared_ptr<IEnumerable<T>> Skip(int count) {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, count](IteratorBlock<T>* it) {
            std::shared_ptr<IEnumerator<T>> iterator = source->GetEnumerator();

            for (int i = 0; i < count; i++) {
                if (! iterator->MoveNext()) {
                    it->yieldBreak();
                }
            }

            while (iterator->MoveNext()) {
                it->yieldReturn(iterator->get_Current());
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    // Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements.
    template <typename Predicate>
    std::shared_ptr<IEnumerable<T>> SkipWhile(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, predicate](IteratorBlock<T>* it) {
            std::shared_ptr<IEnumerator<T>> iterator = source->GetEnumerator();

            while (iterator->MoveNext()) {
                T& item = iterator->get_Current();
                if (! predicate(item)) {
                    // Stop skipping now, and yield this item
                    it->yieldReturn(item);
                    break;
                }
            }

            while (iterator->MoveNext()) {
                it->yieldReturn(iterator->get_Current());
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    // Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements. The element's index is used in the logic of the predicate function.
    template <typename Predicate>
    std::shared_ptr<IEnumerable<T>> SkipWhileIndex(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException("predicate");
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, predicate](IteratorBlock<T>* it) {
            int index = 0;
            std::shared_ptr<IEnumerator<T>> iterator = source->GetEnumerator();
            while (iterator->MoveNext()) {
                T& item = iterator->get_Current();
                if (! predicate(item, index)) {
                    // Stop skipping now, and yield this item
                    it->yieldReturn(item);
                    break;
                }
                index++;
            }

            while (iterator->MoveNext()) {
                it->yieldReturn(iterator->get_Current());
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Sum

    // Computes the sum of a sequence of T values.
    T Sum() {
        T total = 0;

        std::shared_ptr<IEnumerator<T>> e = GetEnumerator();
        while (e->MoveNext()) {
            // code review: overflow?
            total += e->get_Current();
        }

        return total;
    }

    // Computes the sum of a sequence of TResult values that are obtained by invoking
    // a transform function on each element of the input sequence.
    template <typename TResult, typename Selector>
    TResult Sum(Selector selector) {
        //if (nullptr == selector) {
        //	throw ArgumentNullException("selector");
        //}

        return Select<TResult, Selector>(selector)->Sum();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Take

    // Returns a specified number of contiguous elements from the start of a sequence.
    std::shared_ptr<IEnumerable<T>> Take(int count) {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, count](IteratorBlock<T>* it) {
            std::shared_ptr<IEnumerator<T>> iterator = source->GetEnumerator();
            for (int i = 0; i < count && iterator->MoveNext(); i++) {
                it->yieldReturn(iterator->get_Current());
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    // Returns elements from a sequence as long as a specified condition is true.
    template <typename Predicate>
    std::shared_ptr<IEnumerable<T>> TakeWhile(Predicate predicate) {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, predicate](IteratorBlock<T>* it) {
            std::shared_ptr<IEnumerator<T>> iterator = source->GetEnumerator();
            while (iterator->MoveNext()) {
                T& item = iterator->get_Current();
                if (! predicate(item)) {
                    it->yieldBreak();
                }
                it->yieldReturn(item);
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    // Returns elements from a sequence as long as a specified condition is true. The element's index is used in the logic of the predicate function.
    template <typename Predicate>
    std::shared_ptr<IEnumerable<T>> TakeWhileIndex(Predicate predicate) {
        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, predicate](IteratorBlock<T>* it) {
            int index = 0;
            std::shared_ptr<IEnumerator<T>> iterator = source->GetEnumerator();
            while (iterator->MoveNext()) {
                T& item = iterator->get_Current();
                if (! predicate(item, index)) {
                    it->yieldBreak();
                }
                index++;
                it->yieldReturn(item);
            }
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Union

    // Produces the set union of two sequences by using the default equality comparer.
    std::shared_ptr<IEnumerable<T>> Union(std::shared_ptr<IEnumerable<T>> rhs) {
        if (! rhs.get()) {
            throw ArgumentNullException("rhs");
        }

        // deferred
        std::shared_ptr<IEnumerable<T>> lhs = shared_from_this();
        auto fn =  [lhs, rhs](IteratorBlock<T>* it) {
            std::set<T> seenElements;

            foreach<T>(lhs, [it, &seenElements](T& item) { 
                std::pair<std::set<T>::iterator, bool> result = seenElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            });

            foreach<T>(rhs, [it, &seenElements](T& item) { 
                std::pair<std::set<T>::iterator, bool> result = seenElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    // Produces the set union of two sequences by using a specified Comparer.
    template <typename Comparer>
    std::shared_ptr<IEnumerable<T>> Union(std::shared_ptr<IEnumerable<T>> rhs, Comparer comparer) {
        if (! rhs.get()) {
            throw ArgumentNullException("rhs");
        }

        //if (nullptr == comparer) {
        //	return Union(rhs);
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> lhs = shared_from_this();
        auto fn =  [lhs, rhs, comparer](IteratorBlock<T>* it) {
            std::set<T, Comparer> seenElements(comparer);

            foreach<T>(lhs, [it, &seenElements](T& item) { 
                std::pair<std::set<T, Comparer>::iterator, bool> result = seenElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            });

            foreach<T>(rhs, [it, &seenElements](T& item) { 
                std::pair<std::set<T, Comparer>::iterator, bool> result = seenElements.insert(item);
                if (result.second) {
                    it->yieldReturn(item);
                }
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }


    ////////////////////////////////////////////////////////////////////////////
    // Where

    template <typename Predicate>
    std::shared_ptr<IEnumerable<T>> Where(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException();
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, predicate](IteratorBlock<T>* it) {
            foreach<T>(source, [it, predicate](T& item){
                if (predicate(item)) {
                    it->yieldReturn(item);
                }
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }

    template <typename Predicate>
    std::shared_ptr<IEnumerable<T>> WhereIndex(Predicate predicate) {
        //if (nullptr == predicate) {
        //	throw ArgumentNullException();
        //}

        // deferred
        std::shared_ptr<IEnumerable<T>> source = shared_from_this();
        auto fn =  [source, predicate](IteratorBlock<T>* it) {
            int index = 0;
            foreach<T>(source, [it, predicate, &index](T& item){
                if (predicate(item, index)) {
                    it->yieldReturn(item);
                }
                index++;
            });
        };
        return std::shared_ptr<IEnumerable<T>>(new _IteratorBlock<T>(fn));
    }
};

template <typename T, typename F>
static void foreach(std::shared_ptr<IEnumerable<T>> enumerable, F f)
{
    std::shared_ptr<IEnumerator<T>> e = enumerable->GetEnumerator();
    while (e->MoveNext()) {
        f(e->get_Current());
    }
    //e->Dispose();
}

// unfortunately VS11 does not support alias templates
//template <typename T>
//using IEnumerablePtr = std::shared_ptr<IEnumerable<T>;

////////////////////////////////////////////////////////////////////////////
