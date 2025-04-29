#pragma once
#include <stdexcept>
#include <iostream>
#include <ostream>
class Matrix
{
public:
	// Konstruktor
	Matrix(int r, int c);
	// Konstruktor z inicjalizacją
	Matrix(int r, int c, double initial);
	// Pobranie wartości elementu
	double operator()(int x, int y) const;
	// Wyświetlanie całej macierzy (https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170)
	friend std::ostream& operator<<(std::ostream& os, const Matrix& mt);
	// edytowanie elementu
	// dodawanie
	// odejmowanie
	// znajdź wyznacznik
	// dodawanie
	// odejmowanie
	// mnożenie macierzy
	// mnożenie przez liczbę
	// transponowanie
	// macierz odwrotna
	// destruktor?
	~Matrix();
private:
	// dane
	int rows, columns;
	// pointer do tablicy wierszy
	double** data;
};

