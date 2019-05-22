/*
 * Copyright (C) 2007, 2008, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Justin Haygood (jhaygood@reaktix.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef WEEX_UIKIT_WTF_THREADINGPRIMITIVES_H_
#define WEEX_UIKIT_WTF_THREADINGPRIMITIVES_H_

#include "render/wtf/Assertions.h"
#include "render/wtf/FastAllocBase.h"
#include "render/wtf/Locker.h"
#include "render/wtf/Noncopyable.h"
#include "render/wtf/WTFExport.h"

#if OS(WIN)
#include <windows.h>
#endif

#if USE(PTHREADS)
#include <pthread.h>
#endif

namespace WTF {

#if USE(PTHREADS)
struct PlatformMutex {
  pthread_mutex_t m_internalMutex;
#if ENABLE(ASSERT)
  size_t m_recursionCount;
#endif
};
typedef pthread_cond_t PlatformCondition;
#elif OS(WIN)
struct PlatformMutex {
  CRITICAL_SECTION m_internalMutex;
  size_t m_recursionCount;
};
struct PlatformCondition {
  size_t m_waitersGone;
  size_t m_waitersBlocked;
  size_t m_waitersToUnblock;
  HANDLE m_blockLock;
  HANDLE m_blockQueue;
  HANDLE m_unblockLock;

  bool timedWait(PlatformMutex&, DWORD durationMilliseconds);
  void signal(bool unblockAll);
};
#else
typedef void* PlatformMutex;
typedef void* PlatformCondition;
#endif

class WTF_EXPORT MutexBase {
  WTF_MAKE_NONCOPYABLE(MutexBase);
  WTF_MAKE_FAST_ALLOCATED;

 public:
  ~MutexBase();

  void lock();
  void unlock();
#if ENABLE(ASSERT)
  bool locked() { return m_mutex.m_recursionCount > 0; }
#endif

 public:
  PlatformMutex& impl() { return m_mutex; }

 protected:
  MutexBase(bool recursive);

  PlatformMutex m_mutex;
};

class WTF_EXPORT Mutex : public MutexBase {
 public:
  Mutex() : MutexBase(false) {}
  bool tryLock();
};

class WTF_EXPORT RecursiveMutex : public MutexBase {
 public:
  RecursiveMutex() : MutexBase(true) {}
  bool tryLock();
};

typedef Locker<MutexBase> MutexLocker;

class MutexTryLocker {
  WTF_MAKE_NONCOPYABLE(MutexTryLocker);

 public:
  MutexTryLocker(Mutex& mutex) : m_mutex(mutex), m_locked(mutex.tryLock()) {}
  ~MutexTryLocker() {
    if (m_locked)
      m_mutex.unlock();
  }

  bool locked() const { return m_locked; }

 private:
  Mutex& m_mutex;
  bool m_locked;
};

#if OS(WIN)
// The absoluteTime is in seconds, starting on January 1, 1970. The time is
// assumed to use the same time zone as WTF::currentTime(). Returns an interval
// in milliseconds suitable for passing to one of the Win32 wait functions
// (e.g., ::WaitForSingleObject).
DWORD absoluteTimeToWaitTimeoutInterval(double absoluteTime);
#endif

}  // namespace WTF

using WTF::Mutex;
using WTF::MutexBase;
using WTF::MutexLocker;
using WTF::MutexTryLocker;
using WTF::RecursiveMutex;

#endif  // WEEX_UIKIT_WTF_THREADINGPRIMITIVES_H_
