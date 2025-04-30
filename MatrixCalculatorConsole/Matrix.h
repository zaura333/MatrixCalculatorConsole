#pragma once
#include <stdexcept>
#include <iostream>
#include <ostream>
class Matrix
{
public:
	// Konstruktor
	Matrix(int r = 0, int c = 0);
	// Konstruktor z inicjalizacją
	Matrix(int r, int c, double initial);
	//Konstruktor kopia
	Matrix(const Matrix& mt);
	// Pobranie wartości elementu, zwraca referencje do obiektu i umożliwia edytowanie, np.: M(2,3)=5
	double& operator()(int x, int y) const;
	// Wyświetlanie całej macierzy (https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170)
	friend std::ostream& operator<<(std::ostream& os, const Matrix& mt);
	// Kopiowanie macierzy
	Matrix& operator=(const Matrix& toCopy);
	// Dodawanie macierzy
	Matrix operator+(Matrix& mt);
	// Odejmowanie macierzy
	Matrix operator-(Matrix& mt);
	// Liczenie wyznacznika macierzy
	//double getDet();
	// Mnożenie przez liczbę
	Matrix operator*(double n);
	friend Matrix operator*(double n, Matrix& mt);
	// Mnożenie macierzy
	Matrix operator*(Matrix& mt);
	// transponowanie
	// macierz odwrotna
	// Destruktor
	~Matrix();
private:
	// Liczba wierszy/kolumn
	int rows, columns;
	// Pointer do tablicy wierszy
	double** data;
	void freeDataMemory() const;
};
