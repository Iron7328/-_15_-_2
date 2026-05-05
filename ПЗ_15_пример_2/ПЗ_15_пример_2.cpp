#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

const double M_PI = acos(-1.0);

// Абстрактный класс Point (точка на плоскости)
class Point {
protected:
    double x, y;
public:
    Point(double x0 = 0, double y0 = 0) : x(x0), y(y0) {}

    // Чистая виртуальная функция — длина (расстояние от начала координат)
    virtual double Length() const = 0;

    // Функции доступа к координатам
    double getX() const { return x; }
    double getY() const { return y; }

    virtual void draw() const = 0; // для рисования (можно переопределить)

    virtual ~Point() {}
};

// Производный класс Circle (окружность)
class Circle : public Point {
private:
    double radius;
public:
    Circle(double x0 = 0, double y0 = 0, double r = 1.0)
        : Point(x0, y0), radius(r) {
    }

    // Переопределённая длина — длина окружности (или радиус — по желанию)
    // В условии сказано: "функцию, возвращающую длину соответствующего объекта"
    // Для окружности логично вернуть длину окружности.
    double Length() const override {
        return 2 * M_PI * radius;
    }

    double getRadius() const { return radius; }

    // Радиус описанной окружности вокруг треугольника, образованного центрами трёх окружностей
    static double circumscribedRadius(const Circle& c1, const Circle& c2, const Circle& c3) {
        // Стороны треугольника между центрами
        double a = hypot(c2.getX() - c3.getX(), c2.getY() - c3.getY());
        double b = hypot(c1.getX() - c3.getX(), c1.getY() - c3.getY());
        double c = hypot(c1.getX() - c2.getX(), c1.getY() - c2.getY());

        // Площадь треугольника по формуле Герона
        double p = (a + b + c) / 2.0;
        double area = sqrt(p * (p - a) * (p - b) * (p - c));

        if (area == 0) return 0.0; // вырожденный случай

        // Радиус описанной окружности
        double R = (a * b * c) / (4.0 * area);
        return R;
    }

    void draw() const override {
        cout << "Окружность с центром в (" << x << ", " << y
            << ") и радиусом " << radius << endl;
    }
};

// Для рисования точек (можно просто выводить координаты)
class MyPoint : public Point {
public:
    MyPoint(double x0 = 0, double y0 = 0) : Point(x0, y0) {}

    double Length() const override {
        return hypot(x, y); // расстояние от центра координат
    }

    void draw() const override {
        cout << "Точка с координатами (" << x << ", " << y << ")" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    // Массив указателей на объекты Point
    vector<Point*> objects;

    // Несколько точек
    objects.push_back(new MyPoint(1, 2));
    objects.push_back(new MyPoint(4, 5));
    objects.push_back(new MyPoint(-1, 3));

    // 3 окружности
    Circle c1(0, 0, 3);
    Circle c2(4, 0, 2);
    Circle c3(1, 3, 1.5);

    objects.push_back(&c1); // можно хранить и указатели на существующие объекты
    objects.push_back(&c2);
    objects.push_back(&c3);

    // Вывод всех объектов
    cout << "=== Содержимое набора фигур ===\n";
    for (auto obj : objects) {
        obj->draw();
        cout << "Длина (Length) = " << obj->Length() << endl << endl;
    }

    // Вычисление радиуса описанной окружности вокруг центров трёх окружностей
    double R = Circle::circumscribedRadius(c1, c2, c3);
    cout << "\nРадиус окружности, описанной вокруг треугольника, "
        << "вершинами которого являются центры трёх заданных окружностей: "
        << R << endl;

    // Освобождение памяти
    for (auto obj : objects) {
        // Удаляем только динамические объекты (кроме c1, c2, c3)
        MyPoint* p = dynamic_cast<MyPoint*>(obj);
        if (p) delete p;
    }

    return 0;
}