#ifndef GENERIC_NAMED_TYPE_H
#define GENERIC_NAMED_TYPE_H

/**
 * @brief Named Type class implementing strong typing in C++. Prevents accidental type errors
 *        Takem from https://www.fluentcpp.com/2016/12/08/strong-types-for-strong-interfaces/
 */
template <typename T, typename Parameter>
class NamedType
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

#endif // GENERIC_NAMED_TYPE_H