// Copyright 2009 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_REGEXP_STACK_H_
#define V8_REGEXP_STACK_H_

namespace v8 {
namespace internal {

class RegExpStack;

// Maintains a per-v8thread stack area that can be used by irregexp
// implementation for its backtracking stack.
// Since there is only one stack area, the Irregexp implementation is not
// re-entrant. I.e., no regular expressions may be executed in the same thread
// during a preempted Irregexp execution.
class RegExpStackScope {
 public:
  // Create and delete an instance to control the life-time of a growing stack.

  // Initializes the stack memory area if necessary.
  explicit RegExpStackScope(Isolate* isolate);
  ~RegExpStackScope();  // Releases the stack if it has grown.

  RegExpStack* stack() const { return regexp_stack_; }

 private:
  RegExpStack* regexp_stack_;

  DISALLOW_COPY_AND_ASSIGN(RegExpStackScope);
};


class RegExpStack {
 public:
  // Number of allocated locations on the stack below the limit.
  // No sequence of pushes must be longer that this without doing a stack-limit
  // check.
  static const int kStackLimitSlack = 32;

  // Gives the top of the memory used as stack.
  Address stack_base() {
    ASSERT(thread_local_.memory_size_ != 0);
    return thread_local_.memory_ + thread_local_.memory_size_;
  }

  // The total size of the memory allocated for the stack.
  size_t stack_capacity() { return thread_local_.memory_size_; }

  // If the stack pointer gets below the limit, we should react and
  // either grow the stack or report an out-of-stack exception.
  // There is only a limited number of locations below the stack limit,
  // so users of the stack should check the stack limit during any
  // sequence of pushes longer that this.
  Address* limit_address() { return &(thread_local_.limit_); }

  // Ensures that there is a memory area with at least the specified size.
  // If passing zero, the default/minimum size buffer is allocated.
  Address EnsureCapacity(size_t size);

  // Thread local archiving.
  static int ArchiveSpacePerThread() {
    return static_cast<int>(sizeof(ThreadLocal));
  }
  char* ArchiveStack(char* to);
  char* RestoreStack(char* from);
  void FreeThreadResources() { thread_local_.Free(); }

 private:
  RegExpStack();
  ~RegExpStack();

  // Artificial limit used when no memory has been allocated.
  static const uintptr_t kMemoryTop = static_cast<uintptr_t>(-1);

  // Minimal size of allocated stack area.
  static const size_t kMinimumStackSize = 1 * KB;

  // Maximal size of allocated stack area.
  static const size_t kMaximumStackSize = 64 * MB;

  // Structure holding the allocated memory, size and limit.
  struct ThreadLocal {
    ThreadLocal() { Clear(); }
    // If memory_size_ > 0 then memory_ must be non-NULL.
    Address memory_;
    size_t memory_size_;
    Address limit_;
    void Clear() {
      memory_ = NULL;
      memory_size_ = 0;
      limit_ = reinterpret_cast<Address>(kMemoryTop);
    }
    void Free();
  };

  // Address of allocated memory.
  Address memory_address() {
    return reinterpret_cast<Address>(&thread_local_.memory_);
  }

  // Address of size of allocated memory.
  Address memory_size_address() {
    return reinterpret_cast<Address>(&thread_local_.memory_size_);
  }

  // Resets the buffer if it has grown beyond the default/minimum size.
  // After this, the buffer is either the default size, or it is empty, so
  // you have to call EnsureCapacity before using it again.
  void Reset();

  ThreadLocal thread_local_;
  Isolate* isolate_;

  friend class ExternalReference;
  friend class Isolate;
  friend class RegExpStackScope;

  DISALLOW_COPY_AND_ASSIGN(RegExpStack);
};

}}  // namespace v8::internal

#endif  // V8_REGEXP_STACK_H_
