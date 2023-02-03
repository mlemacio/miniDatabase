#ifndef TABLE_TYPE_H
#define TABLE_TYPE_H

#include <concepts>
#include <iterator>

/**
 * @brief Defines a rough definition of a STL container. Taken from https://stackoverflow.com/a/60491447
 *
 * @tparam ContainerType
 */
template <class ContainerType>
concept Container = requires(ContainerType a, const ContainerType b) {
                        requires std::regular<ContainerType>;
                        requires std::swappable<ContainerType>;
                        requires std::destructible<typename ContainerType::value_type>;
                        requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
                        requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
                        requires std::forward_iterator<typename ContainerType::iterator>;
                        requires std::forward_iterator<typename ContainerType::const_iterator>;
                        requires std::signed_integral<typename ContainerType::difference_type>;
                        requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::iterator>::difference_type>;
                        requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
                        {
                            a.begin()
                            } -> std::convertible_to<typename ContainerType::iterator>;
                        {
                            a.end()
                            } -> std::convertible_to<typename ContainerType::iterator>;
                        {
                            b.begin()
                            } -> std::convertible_to<typename ContainerType::const_iterator>;
                        {
                            b.end()
                            } -> std::convertible_to<typename ContainerType::const_iterator>;
                        {
                            a.cbegin()
                            } -> std::convertible_to<typename ContainerType::const_iterator>;
                        {
                            a.cend()
                            } -> std::convertible_to<typename ContainerType::const_iterator>;
                        {
                            a.size()
                            } -> std::convertible_to<typename ContainerType::size_type>;
                        {
                            a.max_size()
                            } -> std::convertible_to<typename ContainerType::size_type>;
                        {
                            a.empty()
                            } -> std::convertible_to<bool>;
                    };

/**
 * @brief Concept describing a Container C that holds a specific type T
 *
 * Example: ContainerOf<double> would accept any "Container" that holds doubles
 */
template <typename C, typename T>
concept ContainerOf = Container<C> && requires(C c) {
                                          std::same_as<typename C::value_type, T>;
                                      };
#endif // TABLE_TYPE_H