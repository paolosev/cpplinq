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

#include "StdAfx.h"
#include "Fiber.h"

#include <assert.h>

Fiber::Fiber() :
    _state(FiberCreated),
    _previousFiber(nullptr),
    _exception(),
    _exceptionCaught(false)
{
    // fiber stack size: 256KB
    _fiber = (PFIBER_START_ROUTINE)::CreateFiber(256 * 1024, fiberProc, this);
}

Fiber::~Fiber()
{
    if (_state == FiberRunning) {
        _previousFiber = (PFIBER_START_ROUTINE)::GetCurrentFiber();
        _state = FiberStopPending;
        ::SwitchToFiber(_fiber);
    }
    ::DeleteFiber(_fiber);
}

void* Fiber::main()
{
    _state = FiberRunning;
    _exceptionCaught = false;

    try {
        run();
    }
    catch (StopFiberException&)
    {
        // do nothing
        _state = FiberStopped;
    }
    catch (std::exception& e)
    {
        _exception = e;
        _exceptionCaught = true;
        _state = FiberStopped;
    }
    catch (...)
    {
        _exception = std::exception("win32 exception");
        _exceptionCaught = true;
        _state = FiberStopped;
    }

    _state = FiberStopped;
    return _previousFiber;
}

bool Fiber::resume()
{
    if (nullptr == _fiber || _state == FiberStopped) {
        return false;
    }
    _previousFiber = (PFIBER_START_ROUTINE)::GetCurrentFiber();
    assert(_previousFiber != _fiber);
    ::SwitchToFiber(_fiber);
    if (_exceptionCaught) {
        throw _exception;
    }

    return (FiberRunning == _state);
}

void Fiber::yield(bool goOn)
{
    if (! goOn) {
        // yield break
        _state = FiberStopped;
    }

    ::SwitchToFiber(_previousFiber);
    if (_state == FiberStopPending) {
        // here this object may be deleting ???
        throw StopFiberException();
    }
}

// static
void CALLBACK Fiber::fiberProc(void* pObj)
{
    Fiber* pFiber = (Fiber*)pObj;
    void* previousFiber = pFiber->main();
    ::SwitchToFiber(previousFiber);
}
