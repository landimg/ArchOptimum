#ifndef _VETOR_H_
#define _VETOR_H_

#include <opencv2/opencv.hpp>

#include "../../Serializacao/include/Serializacao.h"

class Vertice;

class Vetor :
	public Serializacao
{
public:
	long double X;
	long double Y;
	long double Z;

	// Verificação de vetor não nulo.
	explicit operator bool() const noexcept;

	// Verificação de igualdade entre vetores.
	bool operator==(const Vetor &vetor) const noexcept;

	// Verificação de diferença entre vetores.
	bool operator!=(const Vetor &vetor) const noexcept;

	// Método utilizado para somar um vetor ao objeto.
	Vetor operator+(const Vetor &vetor) const noexcept;

	// Método utilizado para subtrair um vetor ao objeto.
	Vetor operator-(const Vetor &vetor) const noexcept;

	// Método utilizado para deslocar o vértice pelo vetor descrito neste objeto.
	Vertice operator+(const Vertice &vertice) const noexcept;

	// Método utilizado para deslocar o vértice pelo vetor descrito neste objeto.
	Vertice operator-(const Vertice &vertice) const noexcept;

	// Método utilizado para trocar o sentido do vetor.
	Vetor operator-() const noexcept;

	// Método utilizado para multiplicar a magnitude do vetor por num.
	Vetor operator*(const long double num) const noexcept;

	// Método utilizado para dividir a magnitude do vetor por num.
	Vetor operator/(const long double num) const;

	// Método utilizado para somar um vetor ao objeto.
	Vetor &operator+=(const Vetor &vetor) noexcept;

	// Método utilizado para subtrair um vetor ao objeto.
	Vetor &operator-=(const Vetor &vetor) noexcept;

	// Método utilizado para multiplicar a magnitude do vetor por num.
	Vetor &operator*=(const long double num) noexcept;

	// Método utilizado para dividir a magnitude do vetor por num.
	Vetor &operator/=(const long double num);

	// Método utilizado para serializar os dados do objeto em memória persistente.
	void serializar(Arquivo &arquivo) const override;

	// Método utilizado para deserializar os dados do objeto em memória persistente.
	void deserializar(Arquivo &arquivo) override;

	// Construtor que utiliza valores para os três eixos do vetor.
	explicit Vetor(const long double X = 0, const long double Y = 0, const long double Z = 0) noexcept;

	/*
	 * Construtor compatível com os vértices da biblioteca OpenCV.
	 * vetor = vertice2 - vertice1;
	 */
	Vetor(const cv::Point3_ < long double > &vertice1, const cv::Point3_ < long double > &vertice2) noexcept;

	/*
	 * Construtor que, pela diferença de posição dos vértices, cria um vetor.
	 * vetor = vertice2 - vertice1;
	 */
	Vetor(const Vertice &vertice1, const Vertice &vertice2) noexcept;

	// Destrutor da classe Vetor.
	virtual ~Vetor() noexcept;
};

#endif