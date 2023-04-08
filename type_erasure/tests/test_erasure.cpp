#include <iostream>
#include <type_erasure/erased_type.h>
#include <vector>

class Circle {
public:
    void Draw() const {
        std::cout << "circle";
    }
};

template <class T>
void DrawObject(T obj) {
    std::cout << "I draw: " << obj << std::endl;
}

template <>
void DrawObject(Circle obj) {
    std::cout << "I draw: ";
    obj.Draw();
    std::cout << std::endl;
}

class DrawInterface {
public:
    virtual void Draw() const = 0;
    virtual ~DrawInterface() = default;
};

template <class InterfacePtr>
class DrawCaller {
public:
    DrawCaller(InterfacePtr ptr) : ptr_(ptr) {
    }

    void Draw() const {
        ptr_->Draw();
    }

protected:
    InterfacePtr ptr_;
};

template <class T, class Interface>
class DrawImplementor : public Interface {
public:
    DrawImplementor(T val) : val_(val) {
    }

    void Draw() const override {
        DrawObject(val_);
    }

protected:
    T val_;
};

int main() {
    using Drawable = ani::ErasedType<DrawInterface, DrawCaller, DrawImplementor>;
    std::vector<Drawable> drawables;

    drawables.emplace_back(4);
    drawables.emplace_back(6);
    drawables.emplace_back("kek");
    drawables.emplace_back(0.4f);
    drawables.emplace_back(0.4444);
    drawables.emplace_back(true);
    drawables.emplace_back(Circle());

    for (const auto& drawable : drawables) {
        drawable.Draw();
    }
}