#pragma once
#include <stdexcept>
#include <iostream>
class Matrix
{
public:
	// Konstruktor
	Matrix(int r, int c);
	// Konstruktor z inicjalizacją
	Matrix(int r, int c, double initial);
	// Wyświetlanie elementu
	double operator()(int x, int y) const {
		double val;
		try {
			val = data[x][y];
		}
		catch (const std::out_of_range& e) {
			std::cout << "Index is out of bounds of the matrix: ";
			e.what();
		}

		return val;
	}
	// wyświetlanie całej macierzy
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

