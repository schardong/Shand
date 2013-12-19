#ifndef SHAND_ENCODING_STRING_UTF8_INCLUDE
#define SHAND_ENCODING_STRING_UTF8_INCLUDE

// Copyright Akira Takahashi 2013
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <string>
#include <stdexcept>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include <boost/utility/string_ref.hpp>

namespace shand {

namespace utf8_detail {

inline const char* length_table()
{
    static const char table[256] = {
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1,
    };
    return table;
}

template <class StringDataType, class ElementStringType>
class utf8_codeuinit_iterator :
    public boost::iterator_facade<
        utf8_codeuinit_iterator<StringDataType, ElementStringType>,
        ElementStringType,
        boost::single_pass_traversal_tag,
        ElementStringType
    >
{
    boost::optional<StringDataType> data_;
    boost::optional<std::size_t> pos_;
    boost::optional<std::size_t> size_;
public:
    utf8_codeuinit_iterator() {}

    utf8_codeuinit_iterator(const StringDataType& data)
        : data_(data)
    {
        if (data_.get().empty()) {
            pos_ = boost::none;
        }
        else {
            pos_ = 0;
        }
    }

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        assert(pos_);

        const unsigned char c = data_.get()[pos_.get()];
        const std::size_t size = length_table()[c];

        pos_.get() += size;
        size_ = size;

        if (pos_.get() >= data_.get().size())
            pos_ = boost::none;
    }

    ElementStringType dereference() const
    {
        assert(pos_);

        std::size_t size = 0;
        if (size_) {
            size = size_.get();
        }
        else {
            const unsigned char c = data_.get()[pos_.get()];
            size = length_table()[c];
        }
        return StringDataType(data_.get(), pos_.get(), size).c_str();
    }

    bool equal(const utf8_codeuinit_iterator& other) const
    {
        return pos_ == other.pos_;
    }
};

} // namespace utf8_detail

template <>
class encoding_string<encoding::utf8> {
public:
    using string_type = std::basic_string<char>;
    using value_type = encoding_string<encoding::utf8>;
    using cchar_type = char;
    using iterator = utf8_detail::utf8_codeuinit_iterator<string_type, value_type>;
    using const_iterator = iterator;

    encoding_string() {}
    encoding_string(const char* s)
        : data_(remove_bom(s)) {}

    std::size_t codeunit_size() const
    {
        const std::size_t size = data_.size();
        std::size_t len = 0;
        std::size_t i = 0;
        while (i < size) {
            i += char_size(i);
            ++len;
        }
        return len;
    }

    // O(N)
    value_type codeunit_at(std::size_t index) const
    {
        const std::size_t size = data_.size();
        std::size_t len = 0;
        std::size_t i = 0;
        while (i < size) {
            const std::size_t n = char_size(i);
            if (len == index) {
                return string_type(data_, i, n).c_str();
            }
            i += n;
            ++len;
        }
        throw std::out_of_range("out of range");
    }

    encoding_string<encoding::utf8> codeunit_substr(std::size_t index, std::size_t codeunit_size) const
    {
        const std::size_t size = data_.size();
        std::size_t len = 0;
        std::size_t i = 0;

        boost::optional<std::size_t> start;
        while (i < size) {
            if (!start && len == index) {
                start = i;
            }
            const std::size_t n = char_size(i);
            i += n;
            ++len;
            if (!start)
                continue;

            --codeunit_size;
            if (codeunit_size == 0) {
                return data_.substr(start.get(), i).c_str();
            }
        }
        throw std::out_of_range("out of range");
    }

    encoding_string<encoding::utf8> codeunit_substr(std::size_t index) const
    {
        const std::size_t size = data_.size();
        std::size_t len = 0;
        std::size_t i = 0;

        while (i < size) {
            if (len == index) {
                return data_.substr(i).c_str();
            }
            i += char_size(i);
            ++len;
        }
        throw std::out_of_range("out of range");
    }

    iterator begin()
    { return iterator(data_); }

    const_iterator begin() const
    { return const_iterator(data_); }

    iterator end()
    { return iterator(); }

    const_iterator end() const
    { return const_iterator(); }

    const cchar_type* c_str() const
    { return data_.c_str(); }

    const string_type& raw_str() const
    { return data_; }

    bool empty() const
    { return data_.empty(); }

private:
    const char* remove_bom(const char* s) const
    {
        boost::string_ref ref(s);
        if (ref.size() < 3)
            return s;

        if (ref.substr(0, 3) == "\xef\xbb\xbf")
            return s + 3;

        return s;
    }

    std::size_t char_size(std::size_t index) const
    {
        const unsigned char c = data_[index];
        return utf8_detail::length_table()[c];
    }

    string_type data_;
};

inline bool operator==(const encoding_string<encoding::utf8>& a, const encoding_string<encoding::utf8>& b)
{ return a.raw_str() == b.raw_str(); }

inline bool operator!=(const encoding_string<encoding::utf8>& a, const encoding_string<encoding::utf8>& b)
{ return !(a == b); }

} // namespace shand

#endif // SHAND_ENCODING_STRING_UTF8_INCLUDE

