#ifndef STR_HPP
#define STR_HPP

#include <algorithm>
#include <cstring>
#include <vector>

class str
{
private:
    char *buf_;
    size_t len_;

    void assign_from_cstr(const char *s) {
        if (!s) {
            buf_ = new char[1];
            buf_[0] = '\0';
            len_ = 0;
            return;
        }
        len_ = std::strlen(s);
        buf_ = new char[len_ + 1];
        std::memcpy(buf_, s, len_ + 1);
    }

public:
    str() : buf_(new char[1]), len_(0) {
        buf_[0] = '\0';
    }

    str(const char &c) : buf_(new char[2]), len_(1) {
        buf_[0] = c;
        buf_[1] = '\0';
    }

    str(const char *&& s_) : buf_(nullptr), len_(0) {
        assign_from_cstr(s_);
    }

    str &operator=(const char *&& s_) {
        if (reinterpret_cast<const void *>(buf_) == reinterpret_cast<const void *>(s_)) {
            return *this;
        }
        delete[] buf_;
        buf_ = nullptr;
        len_ = 0;
        assign_from_cstr(s_);
        return *this;
    }

    str(const str &other) : buf_(new char[other.len_ + 1]), len_(other.len_) {
        std::memcpy(buf_, other.buf_, len_ + 1);
    }

    str &operator=(const str &other) {
        if (this == &other) {
            return *this;
        }
        char *new_buf = new char[other.len_ + 1];
        std::memcpy(new_buf, other.buf_, other.len_ + 1);
        delete[] buf_;
        buf_ = new_buf;
        len_ = other.len_;
        return *this;
    }

    char &operator[](size_t pos) {
        return buf_[pos];
    }

    size_t len() const {
        return len_;
    }

    str join(const std::vector<str> &strs) const {
        if (strs.empty()) {
            return str();
        }
        size_t total = 0;
        for (const auto &s : strs) {
            total += s.len_;
        }
        total += len_ * (strs.size() - 1);

        str result;
        delete[] result.buf_;
        result.buf_ = new char[total + 1];
        result.len_ = total;

        char *out = result.buf_;
        for (size_t i = 0; i < strs.size(); ++i) {
            if (i != 0 && len_ != 0) {
                std::memcpy(out, buf_, len_);
                out += len_;
            } else if (i != 0 && len_ == 0) {
                // no separator to copy
            }
            std::memcpy(out, strs[i].buf_, strs[i].len_);
            out += strs[i].len_;
        }
        result.buf_[total] = '\0';
        return result;
    }

    str slice(size_t l, size_t r) const {
        if (l >= r || l >= len_) {
            return str();
        }
        if (r > len_) {
            r = len_;
        }
        size_t n = r - l;
        str result;
        delete[] result.buf_;
        result.buf_ = new char[n + 1];
        result.len_ = n;
        std::memcpy(result.buf_, buf_ + l, n);
        result.buf_[n] = '\0';
        return result;
    }

    ~str() {
        delete[] buf_;
    }
};

#endif
