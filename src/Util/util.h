/*
 * MIT License
 *
 * Copyright (c) 2016 xiongziliang <771730766@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef UTIL_UTIL_H_
#define UTIL_UTIL_H_

#if defined(_WIN32)
#include <WinSock2.h>
#pragma comment (lib,"WS2_32")
#else
#include <unistd.h>
#include <sys/time.h>
#endif // defined(_WIN32)

#if defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define OS_IPHONE
#elif TARGET_OS_IPHONE
#define OS_IPHONE
#endif
#endif //__APPLE__


#define INSTANCE_IMP(class_name, ...) \
class_name &class_name::Instance() { \
    static std::shared_ptr<class_name> s_instance(new class_name(__VA_ARGS__)); \
    static class_name &s_insteanc_ref = *s_instance; \
    return s_insteanc_ref; \
}

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

namespace CMDToolkit {

class _StrPrinter : public string {
public:
    _StrPrinter() {}

    template<typename T>
    _StrPrinter& operator <<(T && data) {
        _stream << std::forward<T>(data);
        this->string::operator=(_stream.str());
        return *this;
    }

    string operator <<(std::ostream&(*f)(std::ostream&)) const {
        return *this;
    }

private:
    stringstream _stream;
};

//禁止拷贝基类
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    //禁止拷贝
    noncopyable(const noncopyable &that) = delete;
    noncopyable(noncopyable &&that) = delete;
    noncopyable &operator=(const noncopyable &that) = delete;
    noncopyable &operator=(noncopyable &&that) = delete;
};

struct variant:public string{
    template<typename T>
    variant(const T &t):string(to_string(t)){}

    template<size_t N>
    variant(const char (&s)[N]):string(s,N){}

    variant(const char *cstr):string{cstr}{}

    variant(const string &other=string()):string{other}{}

    template<typename T>
    operator T() const {
        T t;
        stringstream ss;
        return ss << *this && ss >> t ? t : T();
    }

    template<typename T>
    bool operator ==(const T &t) const {
        return 0==this->compare(variant(t));
    }

    bool operator ==(const char *t) const {
        return  0==this->compare(t);
    }

    template<typename T>
    T as() const{
        return (T)(*this);
    }
};

#define StrPrinter _StrPrinter()


}  // namespace CMDToolkit

#endif /* UTIL_UTIL_H_ */
