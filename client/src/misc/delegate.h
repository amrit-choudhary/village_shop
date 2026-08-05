#pragma once

/**
 * Minimal bound-callback primitive: an object pointer plus a plain function pointer taking it.
 * Used for UI callbacks, etc.
 */

namespace ME {

class Delegate {
   public:
    void* object = nullptr;
    void (*invoke)(void*) = nullptr;

    void Execute() {
        if (invoke != nullptr) {
            invoke(object);
        }
    }

    // Binds a member function directly, generating the void*-to-typed-call thunk at compile
    // time so callers never hand-write one. Usage: Delegate::Bind<T, &T::Method>(delegate, this).
    template <typename T, void (T::*Method)()>
    static void Bind(Delegate& delegate, T* instance) {
        delegate.object = instance;
        delegate.invoke = &MethodThunk<T, Method>;
    }

   private:
    template <typename T, void (T::*Method)()>
    static void MethodThunk(void* obj) {
        (static_cast<T*>(obj)->*Method)();
    }
};

}  // namespace ME
