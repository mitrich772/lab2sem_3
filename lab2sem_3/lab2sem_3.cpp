// lab2sem_3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <cmath>
#include <string> 
#include <numbers>

#define CIR_GEOMETRY 3
#define RECT_GEOMETRY 4
#define R 5
#define SIDE_MYLTIPLIER 10
using namespace std;

double get_PI() {
    return std::numbers::pi;
}

class Figure {
public:
    static inline int numberOfFigures = 0;
    double* geometry;
    double angle;

    // Чисто виртуальная функция (абстрактная функция)
    virtual void rotate() = 0;
    virtual double square() = 0;
    virtual string toString() = 0;
    virtual ~Figure() = default;
};

class Circle : public Figure {
public:
    double* geometry = new double[CIR_GEOMETRY]; // xp yp r
    
    Circle() {
        geometry[0] = 0.0;
        geometry[1] = 0.0;
        geometry[2] = 1.0;
        angle = 0.0;
        Figure::numberOfFigures++;
    }
    Circle(double square) {
        geometry[0] = 0.0;
        geometry[1] = 0.0;
        geometry[2] = sqrt(square / get_PI());
        angle = 0.0;
        Figure::numberOfFigures++;
    }
    Circle(double xp, double yp, double r, double angle) {
        geometry[0] = xp, geometry[1] = yp, geometry[2] = r;
        this->angle = angle;
        Figure::numberOfFigures++;
    }
    Circle(const Circle& other) {
        geometry = new double[CIR_GEOMETRY];
        for (int i = 0; i < CIR_GEOMETRY; ++i) {
            geometry[i] = other.geometry[i];
        }
        angle = other.angle;
        Figure::numberOfFigures++;
    }

   
    void rotate() override {}

    double square() override {
        return get_PI() * geometry[2] * geometry[2];
    }
    static double getCircleSquare(const Circle cir) {
        return ((cir.geometry[2] + cir.geometry[2]) * 2 * get_PI());
    }


    string toString() override {
        return string(typeid(*this).name()) +
            " radius:" + to_string(geometry[2]) +
            " c_point(" + to_string(geometry[0]) + ";" + to_string(geometry[1]) + ")";
    }

    static bool checkCircleInBox(Circle circle) {

        double x = circle.geometry[0];
        double y = circle.geometry[1];
        double r = circle.geometry[2]; // && y_point > r && 10*R - x_point > r && 10*R - y_point > r

        return (x - r >= 0 && y - r >= 0 && x + r <= 10 * R && y + r <= 10 * R);
    }

    bool checkIsInBox() {

        double x = geometry[0];
        double y = geometry[1];
        double r = geometry[2]; // && y_point > r && 10*R - x_point > r && 10*R - y_point > r

        return (x - r >= 0 && y - r >= 0 && x + r <= 10 * R && y + r <= 10 * R);
    }

    Circle& operator=(Circle other) {
        swap(geometry, other.geometry);
        swap(angle, other.angle);
        return *this;
    }
    Circle operator + (const Circle& cir) {
        double sq = this->square()  + getCircleSquare(cir);
        //cout << "sq =" << sq << "|" << this->square() << "|" << ((cir.geometry[2] + cir.geometry[2]) * get_PI()) << endl;
        return Circle(sq);
    }
    Circle operator / (const Circle& cir) {
        double sq = this->square() / getCircleSquare(cir);
        return Circle(sq); 
    }
    bool operator > (const Circle& cir) const{
        return geometry[2] > cir.geometry[2];
    }
    bool operator==(const Circle& cir) const {
        return geometry[2] == cir.geometry[2]; // сравнение радиусов
    }
    ~Circle() {
        delete[] geometry;
        Figure::numberOfFigures--;
    }
};

class Rectangle : public Figure {
public:
    double* geometry = new double[RECT_GEOMETRY]; // xp yp a b

    Rectangle() {
        geometry[0] = 0.0; // x координата центра
        geometry[1] = 0.0; // y координата центра
        geometry[2] = 1.0; // ширина (a), например 1
        geometry[3] = 1.0; // высота (b), например 1
        angle = 0.0; // угол
        Figure::numberOfFigures++;
    }
    Rectangle(double xp, double yp, double a, double b, double angle) {
        geometry[0] = xp, geometry[1] = yp, geometry[2] = a, geometry[3] = b;
        this->angle = angle;
        Figure::numberOfFigures++;
    }
    Rectangle(const Rectangle& other) {
        geometry = new double[RECT_GEOMETRY];
        for (int i = 0; i < RECT_GEOMETRY; ++i) {
            geometry[i] = other.geometry[i];
        }
        angle = other.angle;
        Figure::numberOfFigures++;
    }

    

    void rotate() override {}
    double square() override {
        return geometry[2] * geometry[3];
    }
    static double getRectangleSquare(const Rectangle rect){
        return rect.geometry[2] * rect.geometry[3];
    }
    string toString() override {
        return string(typeid(*this).name()) +
            " sides(a;b):(" + to_string(geometry[2]) + ";" + to_string(geometry[3]) + ")" +
            " c_point(" + to_string(geometry[0]) + ";" + to_string(geometry[1]) + ")";
    }
    static bool checkRectangleInBox(Rectangle rectangle) {

        double x = rectangle.geometry[0]; // x координата центра
        double y = rectangle.geometry[1]; // y координата центра
        double a = rectangle.geometry[2]; // ширина прямоугольника
        double b = rectangle.geometry[3]; // высота прямоугольника
        double angleRad = rectangle.angle * get_PI() / 180.0; // угол в радианах


        double half_a = a / 2;
        double half_b = b / 2;

        double corners[4][2] = {
            {-half_a, -half_b}, // нижний левый
            { half_a, -half_b}, // нижний правый
            { half_a,  half_b}, // верхний правый
            {-half_a,  half_b}  // верхний левый
        };

        for (int i = 0; i < 4; ++i) {
            double local_x = corners[i][0];
            double local_y = corners[i][1];

            // Применяем матрицу поворота
            double rotated_x = x + (local_x * cos(angleRad) - local_y * sin(angleRad));
            double rotated_y = y + (local_x * sin(angleRad) + local_y * cos(angleRad));

            // Проверяем, что угол находится внутри границ
            if (rotated_x < 0 || rotated_x > 30 || rotated_y < 0 || rotated_y > 30) {
                return false; // Если хотя бы один угол выходит за пределы, возвращаем false
            }
        }

        return true; // Все углы внутри коробки
    }
    bool checkIsInBox() {
        double x = geometry[0]; // x координата центра
        double y = geometry[1]; // y координата центра
        double a = geometry[2]; // ширина прямоугольника
        double b = geometry[3]; // высота прямоугольника
        double angleRad = angle * get_PI() / 180.0; // угол в радианах


        double half_a = a / 2;
        double half_b = b / 2;

        double corners[4][2] = {
            {-half_a, -half_b}, // нижний левый
            { half_a, -half_b}, // нижний правый
            { half_a,  half_b}, // верхний правый
            {-half_a,  half_b}  // верхний левый
        };

        for (int i = 0; i < 4; ++i) {
            double local_x = corners[i][0];
            double local_y = corners[i][1];

            // Применяем матрицу поворота
            double rotated_x = x + (local_x * cos(angleRad) - local_y * sin(angleRad));
            double rotated_y = y + (local_x * sin(angleRad) + local_y * cos(angleRad));

            // Проверяем, что угол находится внутри границ
            if (rotated_x < 0 || rotated_x > 30 || rotated_y < 0 || rotated_y > 30) {
                return false; // Если хотя бы один угол выходит за пределы, возвращаем false
            }
        }

        return true; // Все углы внутри коробки
    }
 
    Rectangle& operator = (Rectangle other) {
        // Меняем содержимое текущего объекта с временным объектом `other`
        swap(geometry, other.geometry);
        swap(angle, other.angle);

        // Возвращаем текущий объект
        return *this;
    }
    bool operator > (const Rectangle& rect) {
        return this->square() > rect.geometry[2] * rect.geometry[3];
    }
    bool operator == (const Rectangle& rect) {
        return this->square() == getRectangleSquare(rect);
    }
    Rectangle operator + (const Rectangle& rect) {
        // Складываем площади и создаем новый прямоугольник с той же высотой, но измененной шириной
        double newSquare = this->square() + getRectangleSquare(rect);
        double newA = sqrt(newSquare / (geometry[3])); 
        return Rectangle(geometry[0], geometry[1], newA, geometry[3], angle);
    }
    Rectangle operator / (const Rectangle& rect) {
        double newWidth = this->square() / getRectangleSquare(rect);
        double newHeight = geometry[3]; // Сохраняем высоту
        return Rectangle(geometry[0], geometry[1], sqrt(newWidth), newHeight, angle); // Центр остается прежним
    }

    
    ~Rectangle() {
        delete[] geometry;
        Figure::numberOfFigures--;
    }
};

std::ostream& operator << (std::ostream& stream, Figure& c1) {
    stream << "Value of class: ";
    stream << c1.toString();
    return stream;
}


template<typename T>
void swape(T arr[], int i, int j) {
    T tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
} // 3

template<typename T>
void heapify(T arr[], int n, int i) { // дерево -> куча
    int largest = i; // корень
    int l = 2 * i + 1; // левый 
    int r = 2 * i + 2; // правый 

    // Если левый дочерний элемент больше корня
    if (l < n && arr[l] > arr[largest]){
        largest = l;
    }
    // Если правый дочерний элемент больше, чем самый большой элемент на данный момент
    if (r < n && arr[r] > arr[largest]) {
        largest = r;
    }
    // Если самый большой элемент не корень
    if (largest != i) {
        swap(arr[i], arr[largest]);
 
        // Рекурсивно преобразуем в двоичную кучу затронутое поддерево
        heapify(arr, n, largest);
    }
} // 1

template<typename T>
void heapSort(T arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--){
        heapify(arr, n, i);
    }
    for (int i = n - 1; i >= 0; i--){
        swap(arr[0], arr[i]);

        heapify(arr, i, 0);
    }
} // 1

template<typename T>

void printArray(T arr[], int n){
    for (int i = 0; i < n; ++i)
        cout << arr[i] << endl;
} // 1

template<typename T>
int findInSorted(T arr[], T key, int n) { // 4

    bool flag = false;
    int l = 0; 
    int r = n; 
    int mid;

    while ((l <= r) && (flag != true)) {
        mid = (l + r) / 2; 

        if (arr[mid] == key) { flag = true; } 
        if (arr[mid] > key) { r = mid - 1; } 
        else { l = mid + 1; }
    }

    if (flag) {
        return mid;
    }

    return -1;
}
template<typename T>
int findInNonSorted(T arr[],T key,int n) {
    for (int i = 0; i < n;i++) {
        if (arr[i] == key) return i;
    }
    return -1;
}

template<typename T>
void inverse(T arr[], int n) {
    int left = 0;
    int right = n-1;
    T cp;
    while (left < right) {
        cp = arr[left];
        arr[left] = arr[right];
        arr[right] = cp;
        left++;
        right--;
    }
}

template<typename T>
T averageOfArray(T arr[], int size) {
    T sum = 0;
    for (int i = 0; i < size; i++) {
        sum = sum + arr[i];  
    }
    return sum / T(size);
      
}



int main() {
    Circle arr[] = { Circle(52), Circle(32), Circle(3), Circle(42) };
    int n = sizeof(arr) / sizeof(arr[0]);
    swape(arr,1, 3);
    heapSort(arr, n);
    cout << "Sorted array is \n";
    printArray(arr, n);
    cout << "In sorted : " << findInSorted(arr, Circle(32), 4) << endl;
    cout << "In nonsorted : " << findInNonSorted(arr, Circle(32), 4) << endl;
    inverse<Circle>(arr,4);
    printArray(arr, 4);
    cout << averageOfArray<Circle>(arr, 4).toString();
}

