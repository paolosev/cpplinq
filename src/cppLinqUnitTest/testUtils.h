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

#include "../cpplinq/iteratorBlock.h"
#include <string>
#include <vector>

template <typename Container, typename T> 
class StlEnumerable : public IEnumerable<T>
{
    class Enumerator : public IEnumerator<T>
    {
    public:
        Enumerator(Container& v) : _v(v), _pos(-1), _it() {
        }

        // IEnumerable
        virtual void Reset() {
            _pos = -1;
        }
        virtual bool MoveNext() {
            if (_pos == -1) {
                _pos = 0;
                _it = _v.begin();
                return _it != _v.end();
            }
            else if (_pos >= (int)_v.size()) {
                _it = _v.begin();
                return false;
            }
            else {
                _it++;
                _pos++;
                return _pos < (int)_v.size();
            }
        }
        virtual T& get_Current() {
            if (_pos < 0 || _pos >= (int)_v.size()) {
                throw new std::exception();
            }
            return *_it;
        }

    private:
        Container& _v;
        typename Container::iterator _it;
        int _pos;
    };

public:
    StlEnumerable(Container& v) : _v(v) {}
    virtual ~StlEnumerable() {}
    virtual std::shared_ptr<IEnumerator<T>> GetEnumerator() {
        return std::shared_ptr<IEnumerator<T>>(new Enumerator(_v));
    }

private:
    Container& _v;
};

template <typename T> 
class Vector : public StlEnumerable<std::vector<T>, T>
{
    typedef StlEnumerable<std::vector<T>, T> Inherited;
public:
    Vector(const T* pT, size_t len) : Inherited(_vector)
    {
        for (size_t i = 0; i < len; i++) {
            _vector.push_back(pT[i]);
        }
    }

    T& operator[](int index) {
        return _vector[index];
    }

private:
    std::vector<T> _vector;
};

class String : public StlEnumerable<std::string, char>
{
    typedef StlEnumerable<std::string, char> Inherited;

public:
    String(const std::string& s) : Inherited(_s), _s(s) {}
    String(const char* p) :  Inherited(_s), _s(p) {}
    String() : Inherited(_s), _s() {}

    String& operator = (const String& rhs) {
        _s = rhs._s;
        return *this;
    }

    bool operator==(const String& rhs) const {
        return 0 == strcmp(_s.c_str(), rhs._s.c_str());
    }
    bool CompareIgnoreCase(const String& rhs) const {
        return 0 == _stricmp(_s.c_str(), rhs._s.c_str());
    }
    bool operator<(const String& rhs) const {
        return 0 < strcmp(_s.c_str(), rhs._s.c_str());
    }
    bool operator>(const String& rhs) const {
        return ! operator<(rhs);
    }

    const char* c_str() const {
        return _s.c_str();
    }

    int length() const {
        return _s.size();
    }

private:
    std::string _s;
};

struct CompareIgnoreCase
    : public std::binary_function<String, String, bool>
{	// functor for operator<
    bool operator()(const String& _Left, const String& _Right) const
    {	// apply operator< to operands
        return _stricmp(_Left.c_str(), _Right.c_str()) < 0;
    }
};

inline std::string IntToString(int value)
{
    char buff[16];
    _itoa_s(value, buff, 16, 10);
    return std::string(buff);
}
