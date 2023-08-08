Instructions:

```
cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j $(nproc)
build/quill_bad_variant
```

May need to run the executable a few times for it to fail. When it fails, it
fails with:

```
23:54:20.364940003 [126094] main.cc:28                   LOG_INFO Hello world #96
23:54:20.364940216 [126094] main.cc:28                   LOG_INFO Hello world #97
23:54:20.364940425 [126094] main.cc:28                   LOG_INFO Hello world #98
23:54:20.364940634 [126094] main.cc:28                   LOG_INFO Hello world #99
23:54:20.364940900 [126094] main.cc:28                   LOG_INFO Hello world #100
23:54:20.364941110 [126094] main.cc:28                   LOG_INFO Hello world #101
23:54:21 Quill INFO: BoundedNonBlocking queue dropped 898 log messages from thread 126094
terminate called after throwing an instance of 'std::bad_variant_access'
  what():  std::get: wrong index for variant
```

Using GDB, we can see the following backtrace:

```
(gdb) bt
#0  __pthread_kill_implementation (no_tid=0, signo=6, threadid=140737345746496) at ./nptl/pthread_kill.c:44
#1  __pthread_kill_internal (signo=6, threadid=140737345746496) at ./nptl/pthread_kill.c:78
#2  __GI___pthread_kill (threadid=140737345746496, signo=signo@entry=6) at ./nptl/pthread_kill.c:89
#3  0x00007ffff7842476 in __GI_raise (sig=sig@entry=6) at ../sysdeps/posix/raise.c:26
#4  0x00007ffff78287f3 in __GI_abort () at ./stdlib/abort.c:79
#5  0x00007ffff7ca2b9e in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#6  0x00007ffff7cae20c in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#7  0x00007ffff7cad1e9 in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#8  0x00007ffff7cad959 in __gxx_personality_v0 () from /lib/x86_64-linux-gnu/libstdc++.so.6
#9  0x00007ffff7eb3884 in ?? () from /lib/x86_64-linux-gnu/libgcc_s.so.1
#10 0x00007ffff7eb3f41 in _Unwind_RaiseException () from /lib/x86_64-linux-gnu/libgcc_s.so.1
#11 0x00007ffff7cae4cb in __cxa_throw () from /lib/x86_64-linux-gnu/libstdc++.so.6
#12 0x0000555555560bee in std::__throw_bad_variant_access (__what=0x5555555ea320 "std::get: wrong index for variant") at /usr/include/c++/11/variant:1314
#13 0x0000555555560c25 in std::__throw_bad_variant_access (__valueless=false) at /usr/include/c++/11/variant:1322
#14 0x000055555557105f in std::get<2ul, std::monostate, quill::detail::UnboundedQueue, quill::detail::BoundedQueueImpl<unsigned int> > (
    __v=std::variant<std::monostate, quill::detail::UnboundedQueue, quill::detail::BoundedQueueImpl<unsigned int>> [index 1] = {...})
    at /usr/include/c++/11/variant:1681
#15 0x000055555556d0a8 in std::get<quill::detail::BoundedQueueImpl<unsigned int>, std::monostate, quill::detail::UnboundedQueue, quill::detail::BoundedQueueImpl<unsigned int> > (__v=std::variant<std::monostate, quill::detail::UnboundedQueue, quill::detail::BoundedQueueImpl<unsigned int>> [index 1] = {...})
    at /usr/include/c++/11/variant:1122
#16 0x0000555555562289 in quill::detail::ThreadContext::spsc_queue<(quill::detail::QueueType)4> (this=0x7ffff7fb6080)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContext.h:98
#17 quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}::operator()(quill::detail::ThreadContext*) const (__closure=0x7ffff77feaef, thread_context=0x7ffff7fb6080)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContextCollection.h:340
#18 0x0000555555574d43 in __gnu_cxx::__ops::_Iter_pred<quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}>::operator()<__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > > >(__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >) (this=0x7ffff77feaef, __it=0x7ffff7fb6080) at /usr/include/c++/11/bits/predefined_ops.h:318
#19 0x0000555555571d1b in std::__find_if<__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, __gnu_cxx::__ops::_Iter_pred<quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}> >(__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, __gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, __gnu_cxx::__ops::_Iter_pred<quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}>, std::random_access_iterator_tag) (__first=0x7ffff7fb6080, __last=0x7ffff7fb6080, __pred=...)
    at /usr/include/c++/11/bits/stl_algobase.h:2099
#20 0x000055555556d5c4 in std::__find_if<__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, __gnu_cxx::__ops::_Iter_pred<quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}> >(__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::all--Type <RET> for more, q to quit, c to continue without paging--
::allocator<quill::detail::ThreadContext*> > >, __gnu_cxx::__ops::_Iter_pred<quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}>) (__first=0x7ffff7fb6080, 
    __last=0x7ffff7fb6080, __pred=...) at /usr/include/c++/11/bits/stl_algobase.h:2114
#21 0x0000555555568b37 in std::find_if<__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}>(__gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, __gnu_cxx::__normal_iterator<quill::detail::ThreadContext**, std::vector<quill::detail::ThreadContext*, std::allocator<quill::detail::ThreadContext*> > >, quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts()::{lambda(quill::detail::ThreadContext*)#2}) (__first=0x7ffff7fb6080, __last=0x7ffff7fb6080, __pred=...) at /usr/include/c++/11/bits/stl_algo.h:3910
#22 0x000055555556237d in quill::detail::ThreadContextCollection::_find_and_remove_invalidated_thread_contexts (this=0x555555629280 <quill::detail::LogManagerSingleton::instance()::instance+384>)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContextCollection.h:333
#23 0x0000555555561e93 in quill::detail::ThreadContextCollection::clear_invalid_and_empty_thread_contexts (this=0x555555629280 <quill::detail::LogManagerSingleton::instance()::instance+384>)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContextCollection.h:202
#24 0x0000555555566539 in quill::detail::BackendWorker::_main_loop (this=0x555555629500 <quill::detail::LogManagerSingleton::instance()::instance+1024>)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/backend/BackendWorker.h:1068
#25 0x000055555556294f in quill::detail::BackendWorker::run()::{lambda()#1}::operator()() const (__closure=0x55555563e058)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/backend/BackendWorker.h:313
#26 0x000055555557aa66 in std::__invoke_impl<void, quill::detail::BackendWorker::run()::{lambda()#1}>(std::__invoke_other, quill::detail::BackendWorker::run()::{lambda()#1}&&) (__f=...) at /usr/include/c++/11/bits/invoke.h:61
#27 0x000055555557a9d2 in std::__invoke<quill::detail::BackendWorker::run()::{lambda()#1}>(quill::detail::BackendWorker::run()::{lambda()#1}&&) (__fn=...) at /usr/include/c++/11/bits/invoke.h:96
#28 0x000055555557a942 in std::thread::_Invoker<std::tuple<quill::detail::BackendWorker::run()::{lambda()#1}> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (this=0x55555563e058) at /usr/include/c++/11/bits/std_thread.h:259
#29 0x000055555557a8de in std::thread::_Invoker<std::tuple<quill::detail::BackendWorker::run()::{lambda()#1}> >::operator()() (this=0x55555563e058) at /usr/include/c++/11/bits/std_thread.h:266
#30 0x000055555557a61e in std::thread::_State_impl<std::thread::_Invoker<std::tuple<quill::detail::BackendWorker::run()::{lambda()#1}> > >::_M_run() (this=0x55555563e050) at /usr/include/c++/11/bits/std_thread.h:211
#31 0x00007ffff7cdc253 in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#32 0x00007ffff7894b43 in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#33 0x00007ffff7926a00 in clone3 () at ../sysdeps/unix/sysv/linux/x86_64/clone3.S:81
```

Specifically, on `#17`:

```
#16 0x0000555555562289 in quill::detail::ThreadContext::spsc_queue<(quill::detail::QueueType)4> (this=0x7ffff7fb6080) at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContext.h:98
98	      return std::get<BoundedQueue>(_spsc_queue);
(gdb) p _spsc_queue
$1 = std::variant<std::monostate, quill::detail::UnboundedQueue, quill::detail::BoundedQueueImpl<unsigned int>> [index 1] = {{_producer = 0x555555641d00, _consumer = 0x555555641d00, _huge_pages = false}}
```

We can see that the `_spsc_queue` is a `UnboundedQueue`, even though we are trying to access a `BoundedQueue`.

The reason this `UnboundedQueue` is constructed can be illustrated if we set a
breakpoint at `ThreadContext::ThreadContext` and `quill::flush`:

```
Reading symbols from build/quill_bad_variant...
(gdb) break ThreadContext::ThreadContext
Breakpoint 1 at 0xd746: file /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContext.h, line 37.
(gdb) break quill::flush
Breakpoint 2 at 0x5dfba: file /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/src/Quill.cpp, line 169.
(gdb) run
Starting program: /home/shuhao/projects/quill-bug-report/build/quill_bad_variant 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[New Thread 0x7ffff77ff640 (LWP 127159)]
[New Thread 0x7ffff6ffe640 (LWP 127160)]
[Switching to Thread 0x7ffff6ffe640 (LWP 127160)]

Thread 3 "quill_bad_varia" hit Breakpoint 1, quill::detail::ThreadContext::ThreadContext (this=0x7ffff7fba080, queue_type=quill::detail::QueueType::BoundedNonBlocking, default_queue_capacity=4096, initial_transit_event_buffer_capacity=64, huge_pages=false) at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContext.h:37
37	  explicit ThreadContext(QueueType queue_type, uint32_t default_queue_capacity,
(gdb) c
Continuing.
[Thread 0x7ffff6ffe640 (LWP 127160) exited]
[Switching to Thread 0x7ffff7e98780 (LWP 127156)]

Thread 1 "quill_bad_varia" hit Breakpoint 2, quill::flush () at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/src/Quill.cpp:169
169	void flush() { detail::LogManagerSingleton::instance().log_manager().flush(); }
(gdb) c
Continuing.

Thread 1 "quill_bad_varia" hit Breakpoint 1, quill::detail::ThreadContext::ThreadContext (this=0x7ffff7fb6080, queue_type=quill::detail::QueueType::UnboundedBlocking, default_queue_capacity=4096, initial_transit_event_buffer_capacity=64, huge_pages=false) at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContext.h:37
37	  explicit ThreadContext(QueueType queue_type, uint32_t default_queue_capacity,
(gdb) n
39	    : _transit_event_buffer(initial_transit_event_buffer_capacity)
(gdb) p queue_type
$1 = quill::detail::QueueType::UnboundedBlocking
(gdb) bt
#0  quill::detail::ThreadContext::ThreadContext (this=0x7ffff7fb6080, queue_type=quill::detail::QueueType::UnboundedBlocking, default_queue_capacity=4096, initial_transit_event_buffer_capacity=64, huge_pages=false)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContext.h:39
#1  0x000055555559cdb5 in quill::detail::ThreadContextCollection::ThreadContextWrapper<(quill::detail::QueueType)0>::ThreadContextWrapper (this=0x7ffff7e98760, thread_context_collection=..., default_queue_capacity=4096, 
    initial_transit_event_buffer_capacity=64, huge_pages=false) at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContextCollection.h:57
#2  0x00005555555b28ac in quill::detail::ThreadContextCollection::local_thread_context<(quill::detail::QueueType)0> (this=0x555555629280 <quill::detail::LogManagerSingleton::instance()::instance+384>)
    at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/ThreadContextCollection.h:146
#3  quill::detail::LogManager::flush (this=0x555555629100 <quill::detail::LogManagerSingleton::instance()::instance>) at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/include/quill/detail/LogManager.h:157
#4  0x00005555555b1fcf in quill::flush () at /home/shuhao/projects/quill-bug-report/build/_deps/quill-src/quill/src/Quill.cpp:169
#5  0x000055555555fb56 in main () at /home/shuhao/projects/quill-bug-report/main.cc:33
```

It seems like during `quill::flush`, we initialize a new ThreadContext with the
wrong queue type.
