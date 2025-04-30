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
	// Mnożenie przez liczbę
	Matrix operator*(double n);
	friend Matrix operator*(double n, Matrix& mt);
	// Mnożenie macierzy
	Matrix operator*(Matrix& mt);
	// Transponowanie macierzy
	Matrix transpose();
	// Liczenie wyznacznika macierzy
	// double getDet();
	// Macierz odwrotna
	// Matrix inverse();
	// Destruktor
	~Matrix();
private:
	// Liczba wierszy/kolumn macierzy
	int rows, columns;
	double** data; // Pointer do tablicy wierszy
	void freeDataMemory() const; // Funkcja usuwajca tablice i czyszacza dane macierzy.
};
