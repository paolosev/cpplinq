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

#include "ienumerable.h"

////////////////////////////////////////////////////////////////////////////

template <typename TSource>
class IteratorBlock : public IEnumerable<TSource>,
    public IEnumerator<TSource>,
    public Fiber
{
public:
    // IEnumerable
    virtual std::shared_ptr<IEnumerator<TSource>> GetEnumerator() {
        if (::GetCurrentThreadId() == _threadId && ! _enumeratorCreated) {
            _enumeratorCreated = true;
            return std::dynamic_pointer_cast<IEnumerator<TSource>>(shared_from_this());
        }

        std::shared_ptr<IteratorBlock<TSource>> cloned = clone();
        return cloned->GetEnumerator();
    }

    // IEnumerator
    virtual void Reset() {
        throw InvalidOperationException();
    }

    virtual bool MoveNext() {
        return resume();
    }

    virtual TSource& get_Current() {
        return _current;
    }

    void yieldReturn(TSource returnValue) {
        _current = returnValue;
        yield(true);
    }

    void yieldBreak() {
        yield(false);
    }

protected:
    IteratorBlock() :
        _current(),
        _enumeratorCreated(false),
        _threadId(::GetCurrentThreadId())
    {
    }
    virtual ~IteratorBlock() {}

    virtual std::shared_ptr<IteratorBlock<TSource>> clone() const = 0;

private:
    TSource _current;
    bool _enumeratorCreated;
    DWORD _threadId;
};

////////////////////////////////////////////////////////////////////////////

template <typename TSource>
class _IteratorBlock : public IteratorBlock<TSource>
{
protected:
    struct IF {
        virtual void run(IteratorBlock<TSource>* pThis) = 0;
    };

    template <typename Func>
    struct F : public IF
    {
        F(Func func) : _func(func) {
        }

        virtual void run(IteratorBlock<TSource>* pThis) {
            _func(pThis);
        }

        Func _func;
    };

public:
    template <typename _F>
    _IteratorBlock(_F f) :
        _f(std::shared_ptr<IF>(new F<_F>(f))) {
    }

    _IteratorBlock(const _IteratorBlock& rhs) :
        _f(rhs._f) {
    }

protected:
    virtual void run() {
        _f->run(this);
    }

    virtual std::shared_ptr<IteratorBlock<TSource>> clone() const {
        return std::shared_ptr<IteratorBlock<TSource>>(new _IteratorBlock<TSource>(*this));
    }

private:
    std::shared_ptr<IF> _f;
};

////////////////////////////////////////////////////////////////////////////
