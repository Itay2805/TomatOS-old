#ifndef CPPLIB_TEMPLATE_HPP
#define CPPLIB_TEMPLATE_HPP

template<typename T>
inline T Min(T t1, T t2) {
	return t1 > t2 ? t2 : t1;
}

template <typename T>
inline T Min(T t1, T t2, T t3) {
	return t1 > t2 ? (t2 > t3 ? t3 : t2) : (t1 > t3 ? t3 : t1);
}

template<typename T>
inline T Max(T t1, T t2) {
	return t1 < t2 ? t2 : t1;
}

template <typename T>
inline T Max(T t1, T t2, T t3) {
	return t1 < t2 ? (t2 < t3 ? t3 : t2) : (t1 < t3 ? t3 : t1);
}

template<typename T>
inline T Average(T t1, T t2) {
	return (t1 + t2) / 2;
}

template<typename T>
inline T Average(T t1, T t2, T t3) {
	return (t1 + t2 + t.) / 3;
}

template<typename T>
inline T Range(T min, T max, T val) {
	return Min(max, Max(min, val));
}

template<typename T>
inline void Swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

#endif