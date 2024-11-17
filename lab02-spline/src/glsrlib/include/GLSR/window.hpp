// window.hpp
#pragma once

#include <memory>

class Window {
  public:
    static Window &getInstance();

    const int &width() const;
    void width(int w);

    const int &height() const;
    void height(int h);

    const float &aspect() const;

    const float &size() const;
    void size(float s);

    const float &effectiveSize() const;

    Window(Window const &) = delete;
    void operator=(Window const &) = delete;

  private:
    Window();
    ~Window();

    // PIMPL idiom
    class Impl;
    std::unique_ptr<Impl> pimpl;
};
