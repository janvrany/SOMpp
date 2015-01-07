#include "VMThread.h"
#include "VMString.h"

#include <vm/SafePoint.h>
#include <vm/Universe.h>

#include <sstream>


const int VMThread::VMThreadNumberOfFields = 1;
mutex VMThread::threads_map_mutex;
map<thread::id, GCThread*> VMThread::threads;


VMThread::VMThread() :
                VMObject(VMThreadNumberOfFields),
                    thread(nullptr),
                    name(reinterpret_cast<GCString*>(nilObject)) {}

VMString* VMThread::GetName() const {
    return load_ptr(name);
}

void VMThread::SetName(VMString* val) {
    store_ptr(name, val);
}

void VMThread::SetThread(std::thread* t) {
    thread = t;
}

void VMThread::Join() {
# warning there is a race condition on the thread field, \
  because it is set after construction, \
  thank you C++11 API which doesn't allow me to create threads without executing them.
    try {
        // this join() needs always to be in tail position with respect to all
        // operations, before going back to the interpreter loop.
        // Specifically, we can't use any object pointers that might have moved.
        SafePoint::AnnounceBlockingMutator();
        thread->join();
        SafePoint::ReturnFromBlockingMutator();
    } catch(const std::system_error& e) {
        Universe::ErrorPrint("Error when joining thread: error code " +
                             to_string(e.code().value()) + " meaning " +
                             e.what() + "\n");
    }
}

StdString VMThread::AsDebugString() const {
    auto id = thread->get_id();
    stringstream id_ss;
    id_ss << id;
    
    return "Thread(" + load_ptr(name)->GetStdString() + ", " + id_ss.str() + ")";
}

VMThread* VMThread::Clone() const {
// TODO: Clone() should be renamed to Move or Reallocate or something,
// it should indicate that the old copy is going to be invalidated.
    VMThread* clone = new (GetHeap<HEAP_CLS>(), GetAdditionalSpaceConsumption() ALLOC_MATURE) VMThread();
    clone->clazz  = clazz;
    clone->thread = thread;
    clone->name   = name;
    return clone;
}

void VMThread::Yield() {
    this_thread::yield();
}

VMThread* VMThread::Current() {
    thread::id id = this_thread::get_id();
    
    lock_guard<mutex> lock(threads_map_mutex);
    auto thread_i = threads.find(id);
    if (thread_i != threads.end()) {
        return load_ptr(thread_i->second);
    } else {
        Universe::ErrorExit("Did not find object for current thread. "
                            "This is a bug, i.e., should not happen.");
        return reinterpret_cast<VMThread*>(load_ptr(nilObject));
    }
}

void VMThread::Initialize() {
    // this is initialization time, should be done before any GC stuff starts
    // so, don't need a write barrier
    threads[this_thread::get_id()] = reinterpret_cast<GCThread*>(nilObject);
    SafePoint::RegisterMutator(); // registers the main thread with the safepoint
}

void VMThread::RegisterThread(thread::id threadId, VMThread* thread) {
    auto thread_i = threads.find(threadId);
    assert(thread_i == threads.end()); // should not be in the map
    
#warning this is a global data structure, so, _store_ptr should be ok
    threads[threadId] = _store_ptr(thread);
    
    SafePoint::RegisterMutator();
}

void VMThread::UnregisterThread(thread::id threadId) {
    size_t numRemoved = threads.erase(threadId);
    assert(numRemoved == 1);
    
    SafePoint::UnregisterMutator();
}

void VMThread::WalkGlobals(walk_heap_fn walk) {
    for (auto& pair : threads) {
        pair.second = static_cast<GCThread*>(walk(pair.second));
    }
}
