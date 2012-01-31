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

#include <exception>

class ArgumentNullException : public std::exception {
    typedef std::exception Inherited;
public:
    ArgumentNullException() {}
    ArgumentNullException(const char* text) : Inherited(text) {}
};

class OverflowException : public std::exception {
};

class DivideByZeroException : public std::exception {
};

class InvalidOperationException : public std::exception {
    typedef std::exception Inherited;
public:
    InvalidOperationException() {}
    InvalidOperationException(const char* text) : Inherited(text) {}
};

class ArgumentOutOfRangeException : public std::exception {
    typedef std::exception Inherited;
public:
    ArgumentOutOfRangeException() {}
    ArgumentOutOfRangeException(const char* text) : Inherited(text) {}
};

