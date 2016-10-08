#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <cstdarg>

namespace yak
{
    class String
    {
    public:
        static const size_t npos = std::string::npos;

        enum CaseSensitivity
        {
            CaseSensitive,
            CaseInsensitive
        };

        String(const char *data = 0, size_t len = npos)
        {
            if (data) {
                if (len == npos)
                    len = strlen(data);
                mString.assign(data, len);
            }
        }
        String(const char *start, const char *end)
        {
            if (start) {
                mString.assign(start, end);
            }
        }
        String(size_t len, char fillChar)
            : mString(len, fillChar)
        {}

        String(const String &ba)
            : mString(ba.mString)
        {}

        String(String &&ba)
            : mString(std::move(ba.mString))
        {
        }

        String(const std::string &str)
            : mString(str)
        {}

        friend std::ostream& operator<<(std::ostream& out, const String& str) {
            out << str.mString;
            return out;
        }

        const char *c_str() const
        {
            return mString.c_str();
        }

        const char *constData() const
        {
            return mString.data();
        }

        size_t size() const
        {
            return mString.size();
        }

        size_t length() const
        {
            return size();
        }

        String &operator=(const String &other)
        {
            mString = other.mString;
            return *this;
        }

        void assign(const char *data, size_t len = npos)
        {
            if (data || !len) {
                if (len == npos)
                    len = strlen(data);
                mString.assign(data, len);
            } else {
                clear();
            }
        }

        void clear()
        {
            mString.clear();
        }

        size_t lastIndexOf(char ch, size_t from = npos, CaseSensitivity cs = CaseSensitive) const
        {
            if (cs == CaseSensitive)
                return mString.rfind(ch, from == npos ? std::string::npos : size_t(from));
            const char *data = mString.c_str();
            if (from == npos)
                from = mString.size() - 1;
            ch = tolower(ch);
            int f = static_cast<int>(from);
            while (f >= 0) {
                if (tolower(data[f]) == ch)
                    return f;
                --f;
            }
            return npos;
        }

        size_t indexOf(char ch, size_t from = 0, CaseSensitivity cs = CaseSensitive) const
        {
            if (cs == CaseSensitive)
                return mString.find(ch, from);
            const char *data = mString.c_str();
            ch = tolower(ch);
            const size_t size = mString.size();
            while (from < size) {
                if (tolower(data[from]) == ch)
                    return from;
                ++from;
            }
            return npos;
        }

        size_t lastIndexOf(const char *ch, size_t len, size_t from = npos, CaseSensitivity cs = CaseSensitive) const
        {
            switch (len) {
                case 0: return npos;
                case 1: return lastIndexOf(*ch, from, cs);
                default: {
                    if (cs == CaseSensitive)
                        return mString.rfind(ch, from, len);
                    if (from == npos)
                        from = mString.size() - 1;
                    String lowered(ch, len);
                    lowered.lowerCase();
                    const size_t needleSize = lowered.size();
                    size_t matched = 0;
                    int f = static_cast<int>(from);
                    while (f >= 0) {
                        if (lowered.at(needleSize - matched - 1) != tolower(at(f))) {
                            matched = 0;
                        } else if (++matched == needleSize) {
                            return f;
                        }

                        --f;
                    }
                    break; }
            }
            return npos;
        }

        void lowerCase()
        {
            std::transform(mString.begin(), mString.end(), mString.begin(), ::tolower);
        }

        bool isEmpty() const
        {
            return mString.empty();
        }

        char at(size_t i) const
        {
            return mString.at(i);
        }

        char& operator[](size_t i)
        {
            return mString.operator[](i);
        }

        const char& operator[](size_t i) const
        {
            return mString.operator[](i);
        }

        size_t indexOf(const char *ch, size_t len, size_t from = 0, CaseSensitivity cs = CaseSensitive) const
        {
            switch (len) {
                case 0: return npos;
                case 1: return indexOf(*ch, from, cs);
                default: {
                    if (cs == CaseSensitive)
                        return mString.find(ch, from, len);

                    String lowered(ch, len);
                    lowered.lowerCase();
                    const size_t count = size();
                    size_t matched = 0;

                    for (size_t i=from; i<count; ++i) {
                        if (lowered.at(matched) != tolower(at(i))) {
                            matched = 0;
                        } else if (++matched == lowered.size()) {
                            return i - matched + 1;
                        }
                    }
                    break; }
            }
            return npos;
        }
        
        bool contains(const String &other, CaseSensitivity cs = CaseSensitive) const
        {
            return indexOf(other, 0, cs) != npos;
        }

        bool contains(char ch, CaseSensitivity cs = CaseSensitive) const
        {
            return indexOf(ch, 0, cs) != npos;
        }

        size_t lastIndexOf(const String &ba, size_t from = npos, CaseSensitivity cs = CaseSensitive) const
        {
            return lastIndexOf(ba.constData(), ba.size(), from, cs);
        }

        size_t indexOf(const String &ba, size_t from = 0, CaseSensitivity cs = CaseSensitive) const
        {
            return indexOf(ba.constData(), ba.size(), from, cs);
        }

        size_t chomp(const String &chars)
        {
            size_t idx = size() - 1;
            while (idx > 0) {
                if (chars.contains(at(idx - 1))) {
                    --idx;
                } else {
                    break;
                }
            }
            const size_t ret = size() - idx - 1;
            if (ret)
                resize(idx);
            return ret;
        }

        size_t chomp(char ch)
        {
            return chomp(String(&ch, 1));
        }

        void resize(size_t size)
        {
            mString.resize(size);
        }

        void reserve(size_t size)
        {
            mString.reserve(size);
        }

        char first() const
        {
            return at(0);
        }

        char &first()
        {
            return operator[](0);
        }

        char last() const
        {
            assert(!isEmpty());
            return at(size() - 1);
        }

        char &last()
        {
            assert(!isEmpty());
            return operator[](size() - 1);
        }

        String toLower() const
        {
            std::string ret = mString;
            std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
            return ret;
        }

        String toUpper() const
        {
            std::string ret = mString;
            std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
            return ret;
        }

        void upperCase()
        {
            std::transform(mString.begin(), mString.end(), mString.begin(), ::toupper);
        }

        String trimmed(const String &trim = " \f\n\r\t\v") const
        {
            const size_t start = mString.find_first_not_of(trim);
            if (start == npos)
                return String();

            const size_t end = mString.find_last_not_of(trim);
            assert(end != npos);
            return mid(start, end - start + 1);
        }

        String mid(size_t from, size_t l = npos) const
        {
            if (l == npos)
                l = size() - from;
            if (from == 0 && l == size())
                return *this;
            return mString.substr(from, l);
        }

        String left(size_t l) const
        {
            return mString.substr(0, l);
        }

        String right(size_t l) const
        {
            return mString.substr(size() - l, l);
        }

        operator std::string() const
        {
            return mString;
        }

        std::string& ref()
        {
            return mString;
        }

        const std::string& ref() const
        {
            return mString;
        }

        template <size_t StaticBufSize = 4096>
        static String format(const char *format, ...);

        template <size_t StaticBufSize = 4096>
        static String format(const char *format, va_list args)
        {
            va_list copy;
            va_copy(copy, args);

            char buffer[StaticBufSize];
            const size_t size = ::vsnprintf(buffer, StaticBufSize, format, args);
            assert(size >= 0);
            String ret;
            if (size < StaticBufSize) {
                ret.assign(buffer, size);
            } else {
                ret.resize(size);
                ::vsnprintf(&ret[0], size+1, format, copy);
            }
            va_end(copy);
            return ret;
        }

        String &operator+=(char ch)
        {
            mString += ch;
            return *this;
        }

        String &operator+=(const char *cstr)
        {
            if (cstr)
                mString += cstr;
            return *this;
        }

        String &operator+=(const String &other)
        {
            mString += other.mString;
            return *this;
        }

        bool operator==(const String &other) const
        {
            return mString == other.mString;
        }

        bool operator==(const char *other) const
        {
            return other && !mString.compare(other);
        }

        bool operator!=(const String &other) const
        {
            return mString != other.mString;
        }

        bool operator!=(const char *other) const
        {
            return !other || mString.compare(other);
        }

        bool operator<(const String &other) const
        {
            return mString < other.mString;
        }

        bool operator>(const String &other) const
        {
            return mString > other.mString;
        }

    private:
        std::string mString;

    };

    template <size_t StaticBufSize>
    String String::format(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        const String ret = String::format<StaticBufSize>(format, args);
        va_end(args);
        return ret;
    }

    inline bool operator==(const char *l, const String &r)
    {
        return r.operator==(l);
    }

    inline bool operator!=(const char *l, const String &r)
    {
        return r.operator!=(l);
    }

    inline const String operator+(const String &l, const char *r)
    {
        String ret = l;
        ret += r;
        return ret;
    }

    inline const String operator+(const char *l, const String &r)
    {
        String ret = l;
        ret += r;
        return ret;
    }

    inline const String operator+(const String &l, char ch)
    {
        String ret = l;
        ret += ch;
        return ret;
    }

    inline const String operator+(char l, const String &r)
    {
        String ret;
        ret.reserve(r.size() + 1);
        ret += l;
        ret += r;
        return ret;
    }

    inline const String operator+(const String &l, const String &r)
    {
        String ret = l;
        ret += r;
        return ret;
    }

}
#endif
