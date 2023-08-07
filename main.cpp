#include <windows.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;

struct MathVector {
    int id{};
    vector<double> points = { 0, 0 };
};

struct Container {
    vector<MathVector> children;
};

template<typename T>
int findKeyIndexInVector(const T &key, std::vector<T> const &list) {
    const int NOT_FOUND = -1;
    auto it = std::find_if(list.cbegin(), list.cend(), [key](const T &i){ return i == key; });

    if (it != list.cend()) {
        return (int)std::distance(list.cbegin(), it);
    }

    return NOT_FOUND;
}

// Получить первый пропущенный индекс в массиве. Либо новый (т.е. последний + 1)
int getAvailableIndexInRange(vector<int> const &range) {
    vector<int> tempRange = range;
    std::sort(tempRange.begin(), tempRange.end());

    int current = 0;
    while(current < tempRange.size()) {
        if (current != tempRange[current]) break;
        ++current;
    }

    return current;
}

template <class T>
int getAvailableIndexInChildren(vector<T> const &children) {
    vector<int> range;
    range.reserve(children.size());
    for (auto const &child : children) range.push_back(child.id);

    return getAvailableIndexInRange(range);
}

void setMathVector(MathVector &mathVector, vector<double> const &points) {
    mathVector.points = points;
}

MathVector createMathVector(Container const &parent, vector<double> const &points) {
    MathVector mathVector;
    mathVector.id = getAvailableIndexInChildren(parent.children);
    setMathVector(mathVector, points);

    return mathVector;
}

bool addMathVectors(Container const &container, int numberOfLeftItem, int numberOfRightItem, vector<double> &result) {
    if (numberOfRightItem >= container.children.size() || numberOfLeftItem >= container.children.size()) return false;
    if (result.size() != 2) return false;

    result[0] = container.children[numberOfLeftItem].points[0] + container.children[numberOfRightItem].points[0];
    result[1] = container.children[numberOfLeftItem].points[1] + container.children[numberOfRightItem].points[1];

    return true;
}

bool subtractMathVectors(Container const &container, int numberOfLeftItem, int numberOfRightItem, vector<double> &result) {
    if (numberOfRightItem >= container.children.size() || numberOfLeftItem >= container.children.size()) return false;
    if (result.size() != 2) return false;

    result[0] = container.children[numberOfLeftItem].points[0] - container.children[numberOfRightItem].points[0];
    result[1] = container.children[numberOfLeftItem].points[1] - container.children[numberOfRightItem].points[1];

    return true;
}

bool scaleMathVector(Container const &container, int numberItem, double scale, vector<double> &result) {
    if (numberItem >= container.children.size()) return false;
    if (result.size() != 2) return false;

    result[0] = container.children[numberItem].points[0] * scale;
    result[1] = container.children[numberItem].points[1] * scale;

    return true;
}

bool distanceMathVectors(Container const &container, int numberOfLeftItem, int numberOfRightItem, double &distance) {
    vector<double> resultOfSubtract = { 0, 0 };

    if (!subtractMathVectors(container, numberOfLeftItem, numberOfRightItem, resultOfSubtract)) return false;

    distance = sqrt(resultOfSubtract[0] * resultOfSubtract[0] + resultOfSubtract[1] * resultOfSubtract[1]);

    return true;
}

bool normalizeMathVectors(Container const &container, int numberOfLeftItem, int numberOfRightItem, vector<double> &result) {
    double distance;
    vector<double> resultOfSubtract = { 0, 0 };

    if (!subtractMathVectors(container, numberOfLeftItem, numberOfRightItem, resultOfSubtract)) return false;
    if (!distanceMathVectors(container, numberOfLeftItem, numberOfRightItem, distance)) return false;

    result[0] = resultOfSubtract[0] / distance;
    result[1] = resultOfSubtract[1] / distance;

    return true;
}

void printContainer(Container const &container) {
    cout << "----------------" << endl;
    for (auto const &item : container.children) {
        cout << item.id << ": " << item.points[0] << " x " << item.points[1] << endl;
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Container container;
    container.children.emplace_back(createMathVector(container, { 0, 0 }));
    container.children.emplace_back(createMathVector(container, { 3, 4 }));
    container.children.emplace_back(createMathVector(container, { 1, 2 }));
    container.children.emplace_back(createMathVector(container, { 4, 4 }));
    container.children.emplace_back(createMathVector(container, { 5, 5 }));

    printContainer(container);

    setMathVector(container.children[2], { 3, 4 });

    printContainer(container);

    vector<double> addResult = { 0, 0 };
    if (addMathVectors(container, 2, 3, addResult)) {
        cout << "Результат сложения элементов: " << addResult[0] << " x " << addResult[1] << endl;
    } else {
        cout << "Операция не удалась. Один из элементов введен неверно" << endl;
    }

    vector<double> subtractResult = { 0, 0 };
    if (subtractMathVectors(container, 3, 2, subtractResult)) {
        cout << "Результат вычитания элементов: " << subtractResult[0] << " x " << subtractResult[1] << endl;
    } else {
        cout << "Операция не удалась. Один из элементов введен неверно" << endl;
    }

    vector<double> scaleResult = { 0, 0 };
    if (scaleMathVector(container, 1, 0.9, scaleResult)) {
        cout << "Результат умножения на скаляр: " << scaleResult[0] << " x " << scaleResult[1] << endl;
    } else {
        cout << "Умножение не удалось. Один из элементов введен неверно" << endl;
    }

    double distance;
    if (distanceMathVectors(container, 0, 1, distance)) {
        cout << "Расстояние: " << std::fixed << std::setprecision(2) <<  distance << endl;
    }

    vector<double> normaliseResult = { 0, 0 };
    if (normalizeMathVectors(container, 1, 0, normaliseResult)) {
        cout << "Результат нормализации элементов: " << normaliseResult[0] << " x " << normaliseResult[1] << endl;
    } else {
        cout << "Операция не удалась. Один из элементов введен неверно" << endl;
    }
}
