
#ifndef __TINYRENDERER_EXCEPTION_H__
#define __TINYRENDERER_EXCEPTION_H__

#include <exception>
#include <string>

// TODO

class tinyrenderer_exception
{
private:
    /* data */
public:
    tinyrenderer_exception(/* args */);
    ~tinyrenderer_exception();

    const char * what () const noexcept;
};

tinyrenderer_exception::tinyrenderer_exception(/* args */)
{
}

tinyrenderer_exception::~tinyrenderer_exception()
{
}

const char * tinyrenderer_exception::what() const noexcept {
    return "";
}

#endif // __TINYRENDERER_EXCEPTION_H__
