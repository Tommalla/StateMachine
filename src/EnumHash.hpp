/* Tomasz [Tommalla] Zakrzewski, 2014
 * EnumHash
 */
#ifndef ENUM_HASH_HPP
#define ENUM_HASH_HPP
#include <functional>
#include <type_traits>

template<class T, bool>
struct hasher {
	inline size_t operator() (const T& elem) {
		return std::hash<T>()(elem);
	}
};

template<class T>
struct hasher<T, true> {
	inline size_t operator() (const T& elem) {
		typedef typename std::underlying_type<T>::type enumType;
		return std::hash<enumType>()(static_cast<enumType>(elem));
	}
};

template<class T>
struct EnumHash {
	inline size_t operator()(const T& elem) const {
		return hasher<T, std::is_enum<T>::value>()(elem);
	}
};

#endif // ENUM_HASH_HPP