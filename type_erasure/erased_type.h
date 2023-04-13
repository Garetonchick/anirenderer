#pragma once
#include <memory>

namespace ani {

template <class Interface, template <class> class Caller, template <class, class> class Implementor>
class ErasedType : public Caller<std::shared_ptr<const Interface>> {
    using CallerT = Caller<std::shared_ptr<const Interface>>;

public:
    template <class T>
    explicit ErasedType(T val) : CallerT(std::make_shared<const Implementor<T, Interface>>(val)) {
    }
};

}  // namespace ani