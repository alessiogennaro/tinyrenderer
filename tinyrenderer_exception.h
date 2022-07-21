
#ifndef __TINYRENDERER_EXCEPTION_H__
#define __TINYRENDERER_EXCEPTION_H__

#include <exception>
#include <string>


class tinyrenderer_exception: public std::exception {
private:
    std::string err;

public:
    tinyrenderer_exception(const std::string& e);
    ~tinyrenderer_exception() = default;

    const char * what () const noexcept;
};

tinyrenderer_exception::tinyrenderer_exception(const std::string& e): err(e)
{}

const char * tinyrenderer_exception::what() const noexcept {
    return err.c_str();
}

#endif // __TINYRENDERER_EXCEPTION_H__
