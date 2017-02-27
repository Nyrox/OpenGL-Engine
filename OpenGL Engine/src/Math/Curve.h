#pragma once
#include <vector>
#include <algorithm>
#include <functional>

template<class val, class sample = float>
class Curve {
public:
	Curve() = default;
	Curve(std::initializer_list<std::pair<sample, val>> points);

	void addPoint(std::pair<sample, val> point);
	val samplePoint(sample point, std::function<val(val, val, sample)> lerp) const;
		
private:
	std::vector<std::pair<sample, val>> nodes;

	void resort();
};

template<class val, class sample = float>
Curve<val, sample>::Curve(std::initializer_list<std::pair<sample, val>> points) : nodes(points) {
	this->resort();
}

template<class val, class sample = float>
void Curve<val, sample>::addPoint(std::pair<sample, val> point) {
	nodes.push_back(point);
	this->resort();
}

template<class val, class sample = float>
val Curve<val, sample>::samplePoint(sample point, std::function<val(val, val, sample)> lerp) const {
	if (point < std::get<sample>(nodes.at(0))) return std::get<val>(nodes.at(0));

	for (auto iterator = nodes.begin(); iterator != nodes.end(); iterator++) {
		sample current = std::get<sample>(*iterator);
		if (current >= point) {
			val currentVal = std::get<val>(*iterator);
			val previousVal = std::get<val>(*(--iterator));
			sample previous = std::get<sample>(*iterator);

			float interp = 0;
			float distance = current - previous;
			interp = (point - previous) / distance;

			return lerp(previousVal, currentVal, interp);
		}
	}
}


template<class val, class sample = float>
void Curve<val, sample>::resort() {
	std::sort(nodes.begin(), nodes.end(), [](const std::pair<sample, val>& left, const std::pair<sample, val>& right) {
		return std::get<sample>(left) < std::get<sample>(right);
	});
}