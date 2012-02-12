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

class Fiber
{
    enum FiberState
    {
        FiberCreated,
        FiberRunning,
        FiberStopPending,
        FiberStopped
    };

    struct StopFiberException {};

public:
    Fiber();
    virtual ~Fiber();

    static void enableFibersInCurrentThread() {
        ::ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);
    }
    static bool disableFibersInCurrentThread() {
        return (FALSE != ::ConvertFiberToThread());
    }

    void* main();
    bool resume();

protected:
    virtual void run() = 0;
    void yield(bool goOn);

private:
    static void WINAPI fiberProc(void* lpFiberParameter);

    PFIBER_START_ROUTINE _fiber;
    PFIBER_START_ROUTINE _previousFiber;
    FiberState _state;

    std::exception _exception;
    bool _exceptionCaught;
};


