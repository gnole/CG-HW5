//
// Created by oleg on 19.12.2021.
//
#include <SFML/Graphics.hpp>
#include <unistd.h>

#include <cmath>
#include <iostream>

void drawLineRed(int x1, int y1, int x2, int y2, sf::RenderWindow &window) {
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	sf::Vertex point(sf::Vector2f(x2, y2), sf::Color::Red);
	window.draw(&point, 1, sf::Points);
	while (x1 != x2 || y1 != y2) {
		sf::Vertex point1(sf::Vector2f(x1, y1), sf::Color::Red);
		window.draw(&point1, 1, sf::Points);
		int error2 = error * 2;
		if (error2 > -deltaY) {
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX) {
			error += deltaX;
			y1 += signY;
		}
	}
}

void fillPolygon(std::vector<std::pair<int, int>> &vertices, sf::RenderWindow &window) {
	int miny = vertices[0].second, maxy = vertices[0].second;
	for (const auto &vertice : vertices) {
		if (vertice.second < miny)
			miny = vertice.second;
		if (vertice.second > maxy)
			maxy = vertice.second;
	}
	std::vector<std::vector<double>> yarr(maxy);
	for (std::size_t i = 0; i < vertices.size(); ++i) {
		std::size_t next = 0;
		if (i != vertices.size() - 1)
			next = i + 1;
		std::size_t up = 0, down = 0;
		if (vertices[i].second > vertices[next].second) {
			up = i;
			down = next;
		} else if (vertices[i].second < vertices[next].second) {
			up = next;
			down = i;
		} else {
			continue;
		}

		double k = double(vertices[up].second - vertices[down].second) / (vertices[up].first - vertices[down].first);
		for (std::size_t j = vertices[down].second; j < vertices[up].second; ++j) {
			yarr[j].push_back((j - vertices[down].second) / k + vertices[down].first);
		}
		for (std::size_t y = miny; y < maxy; ++y) {
			auto xarr = yarr[y];
			std::sort(xarr.begin(), xarr.end());
			for (std::size_t j = 0; j < xarr.size() / 2; ++j) {
				for (auto x = xarr[j << 1]; x < xarr[(j << 1) + 1]; ++x) {
					sf::Vertex point(sf::Vector2f(std::floor(x), std::floor(y)), sf::Color::Red);
					window.draw(&point, 1, sf::Points);
				}
			}
		}
	}
}
int main() {
	sf::RenderWindow window(sf::VideoMode(740, 680), "HW5");
	window.setFramerateLimit(50);
	std::vector<std::pair<int, int>> vec_points;
	bool dr = true;
	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: {
				window.close();
				return 0;
			}
			case sf::Event::MouseButtonPressed: {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (dr) {
						vec_points.push_back(std::make_pair(event.mouseButton.x, event.mouseButton.y));
					}
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					dr = false;
					auto itend = vec_points.begin();
					vec_points.push_back(std::make_pair(itend->first, itend->second));
				}
			}
			}
		}
		window.clear(sf::Color::White);
		if (!vec_points.empty()) {
			auto it0 = vec_points.begin();
			auto it1 = vec_points.begin();
			++it1;
			for (; it1 != vec_points.end(); ++it0, ++it1) {
				drawLineRed(it0->first, it0->second, it1->first, it1->second, window);
			}
		}
		if (!dr) {
			fillPolygon(vec_points, window);
		}
		window.display();
	}
	return 0;
}