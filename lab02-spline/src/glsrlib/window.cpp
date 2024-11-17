#include "GLSR/window.hpp"

#include <algorithm>

class Window::Impl {
  public:
    Impl()
        : width_(500), height_(500), aspect_(1.0f), size_(10.0f),
          effectiveSize_(size_) {}

    int width_;
    int height_;
    float aspect_;
    float size_;
    float effectiveSize_;

    void updateAspect() {
        aspect_ = static_cast<float>(width_) / height_;
        updateEffectiveSize();
    }
    void updateEffectiveSize() {
        effectiveSize_ = (width_ <= height_) ? size_ : size_ * aspect_;
    }
};

Window &Window::getInstance() {
    static Window instance;
    return instance;
}

Window::Window() : pimpl(std::make_unique<Impl>()) {}
Window::~Window() = default;

const int &Window::width() const { return pimpl->width_; }
void Window::width(int w) {
    pimpl->width_ = w;
    pimpl->updateAspect();
}

const int &Window::height() const { return pimpl->height_; }
void Window::height(int h) {
    pimpl->height_ = h;
    pimpl->updateAspect();
}

const float &Window::aspect() const { return pimpl->aspect_; }

const float &Window::size() const { return pimpl->size_; }
void Window::size(float s) {
    pimpl->size_ = s;
    pimpl->updateEffectiveSize();
}

const float &Window::effectiveSize() const { return pimpl->effectiveSize_; }
