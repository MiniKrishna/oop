#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

struct point {

	double x;
	double y;
};

enum color { RED, ORANGE, GREEN, BLUE, YELLOW, NO_COLOR };

class Shape {

protected:

	vector <point> coordinates;
	point centre;
	double ang;
	color clr;
	long int id;

public:

	Shape(point _centre) {
	
		srand(time(NULL));
		id = rand();

		clr = NO_COLOR;
		ang = 0;

		centre = _centre;
	};

	virtual void scale(double coefficient) = 0;
	
	void move(point _coordinates) {

		centre = _coordinates;
		make_coordinates();
	}

	void rotate(double _angle) {

		double _x, _y;

		for (unsigned int i = 0; i < coordinates.size(); ++i) {

			_x = coordinates[i].x;
			_y = coordinates[i].y;

			coordinates[i].x = ( _x - centre.x ) * cos(_angle) - ( _y - centre.y ) * sin(_angle) + centre.x;
			coordinates[i].y = ( _x - centre.x ) * sin(_angle) + ( _y - centre.y ) * cos(_angle) + centre.y;
		}

		ang = _angle;
	}

	color get_golor () {

		return clr;
	}

	void set_color (color _clr) {

		clr = _clr;
	}

	virtual ~Shape() {};

	friend ostream& operator<< (ostream& out, const Shape& shape) {

		out << "Id: " << shape.id << endl;
		out << "Координаты крайних точек: ";

		for (unsigned int i = 0; i < shape.coordinates.size(); ++i) {

			out << "(" << shape.coordinates[i].x << ", " << shape.coordinates[i].y << ") ";
		}

		out << endl << "Цвет: ";

		switch (shape.clr) {

		case RED:
			out << "красный" << endl;
			break;
		case ORANGE:
			out << "оранжевый" << endl;
			break;
		case GREEN:
			out << "зеленый" << endl;
			break;
		case BLUE:
			out << "голубой" << endl;
			break;
		case YELLOW:
			out << "желтый" << endl;
			break;
		case NO_COLOR:
			out << "не определен" << endl;
			break;
		}

		return out;
	}

private:
	
	virtual void make_coordinates() = 0;
};

class Rectangle : public Shape {

private:

	double length;
	double width;

public:

	Rectangle (double _length, double _width, point _centre) : Shape (_centre) {

		length = _length;
		width = _width;
		make_coordinates();
	}

	void scale (double coefficient) {

		length = length * coefficient;
		width = width * coefficient;
		make_coordinates();
	}

	~Rectangle() {

		coordinates.clear();
	}

	friend std::ostream& operator<< (ostream& out, const Rectangle& rectangle) {

		out << "Прямоугольник" << endl;
		out << (const Shape&)rectangle;
		return out;
	}

private:

	void make_coordinates() {

		coordinates.clear();
		coordinates.push_back({ centre.x - length / 2, centre.y + width / 2 });
		coordinates.push_back({ centre.x + length / 2, centre.y + width / 2 });
		coordinates.push_back({ centre.x + length / 2, centre.y - width / 2 });
		coordinates.push_back({ centre.x - length / 2, centre.y - width / 2 });

		rotate(ang);
	}

};

class Ellipse : public Shape {

private:

	double minDiameter;
	double maxDiameter;

public:

	Ellipse(double diameter, double _diameter, point _centre) : Shape (_centre) {

		if (diameter > _diameter) {

			minDiameter = _diameter;
			maxDiameter = diameter;
		} else {

			minDiameter = diameter;
			maxDiameter = _diameter;
		}

		make_coordinates();
	}

	void scale(double coefficient) {

		minDiameter = minDiameter * coefficient;
		maxDiameter = maxDiameter * coefficient;
		make_coordinates();
	}

	~Ellipse() {

		coordinates.clear();
	}

	friend std::ostream& operator<< (ostream& out, const Ellipse& ellipse) {

		out << "Эллипс" << endl;
		out << (const Shape&)ellipse;
		return out;
	}

private:

	void make_coordinates() {

		coordinates.clear();
		coordinates.push_back({ centre.x, centre.y + minDiameter / 2 });
		coordinates.push_back({ centre.x + maxDiameter / 2, centre.y });
		coordinates.push_back({ centre.x, centre.y - minDiameter / 2 });
		coordinates.push_back({ centre.x - maxDiameter / 2, centre.y });

		rotate(ang);
	}
};

class EllipseSector : public Shape {

	 struct radius {

		double rad;
		double angle;
	 };

private:

	radius rd[2];

public: 
	
	EllipseSector(radius rd0, radius rd1, point _centre) : Shape(_centre) {

		rd[0].rad = rd0.rad;
		rd[1].rad = rd1.rad;
		rd[0].angle = rd0.angle;
		rd[1].angle = rd1.angle;

		make_coordinates();

	}
	
	void scale(double coefficient) {

		for (int i = 0; i < 2; ++i) {

			rd[i].rad = rd[i].rad * coefficient;
		}
		make_coordinates();
	}

	~EllipseSector() {

		coordinates.clear();
	}

	friend std::ostream& operator<< (ostream& out, const EllipseSector& ellipseSector) {

		out << "Сектор эллипса" << endl;
		out << (const Shape&)ellipseSector;
		return out;
	}

private:

	void make_coordinates() {

		coordinates.clear();
		coordinates.push_back(centre);

		for (int i = 0; i < 2; ++i) {

			double _x = rd[i].rad;
			double _y = 0;

			point _point;
			_point.x = _x * cos(rd[i].angle) + centre.x;
			_point.y = _x * sin(rd[i].angle) + centre.y;

			coordinates.push_back(_point);
		}

		rotate(ang);
	}
};

