#pragma once
#include <cstdint>

template<class T>
class Rect {
public:
	Rect(T t_top = 0, T t_left = 0, T t_width = 0, T t_height = 0) : top(t_top), left(t_left), width(t_width), height(t_height) { }

	T top, left;
	T width, height;

	bool contains(T x, T y);
};

template<class T>
bool Rect<T>::contains(T x, T y) {
	return x > left && x < left + width && y > top && y < top + height;
}





using IntRect		= Rect<int32_t>;
using UintRect		= Rect<uint32_t>;
using FloatRect		= Rect<float>;
using DoubleRect	= Rect<double>;