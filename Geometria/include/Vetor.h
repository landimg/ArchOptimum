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

	// Verifica��o de vetor n�o nulo.
	explicit operator bool() const noexcept;

	// Verifica��o de igualdade entre vetores.
	bool operator==(const Vetor &vetor) const noexcept;

	// Verifica��o de diferen�a entre vetores.
	bool operator!=(const Vetor &vetor) const noexcept;

	// M�todo utilizado para somar um vetor ao objeto.
	Vetor operator+(const Vetor &vetor) const noexcept;

	// M�todo utilizado para subtrair um vetor ao objeto.
	Vetor operator-(const Vetor &vetor) const noexcept;

	// M�todo utilizado para deslocar o v�rtice pelo vetor descrito neste objeto.
	Vertice operator+(const Vertice &vertice) const noexcept;

	// M�todo utilizado para deslocar o v�rtice pelo vetor descrito neste objeto.
	Vertice operator-(const Vertice &vertice) const noexcept;

	// M�todo utilizado para trocar o sentido do vetor.
	Vetor operator-() const noexcept;

	// M�todo utilizado para multiplicar a magnitude do vetor por num.
	Vetor operator*(const long double num) const noexcept;

	// M�todo utilizado para dividir a magnitude do vetor por num.
	Vetor operator/(const long double num) const;

	// M�todo utilizado para somar um vetor ao objeto.
	Vetor &operator+=(const Vetor &vetor) noexcept;

	// M�todo utilizado para subtrair um vetor ao objeto.
	Vetor &operator-=(const Vetor &vetor) noexcept;

	// M�todo utilizado para multiplicar a magnitude do vetor por num.
	Vetor &operator*=(const long double num) noexcept;

	// M�todo utilizado para dividir a magnitude do vetor por num.
	Vetor &operator/=(const long double num);

	// M�todo utilizado para serializar os dados do objeto em mem�ria persistente.
	void serializar(Arquivo &arquivo) const override;

	// M�todo utilizado para deserializar os dados do objeto em mem�ria persistente.
	void deserializar(Arquivo &arquivo) override;

	// Construtor que utiliza valores para os tr�s eixos do vetor.
	explicit Vetor(const long double X = 0, const long double Y = 0, const long double Z = 0) noexcept;

	/*
	 * Construtor compat�vel com os v�rtices da biblioteca OpenCV.
	 * vetor = vertice2 - vertice1;
	 */
	Vetor(const cv::Point3_ < long double > &vertice1, const cv::Point3_ < long double > &vertice2) noexcept;

	/*
	 * Construtor que, pela diferen�a de posi��o dos v�rtices, cria um vetor.
	 * vetor = vertice2 - vertice1;
	 */
	Vetor(const Vertice &vertice1, const Vertice &vertice2) noexcept;

	// Destrutor da classe Vetor.
	virtual ~Vetor() noexcept;
};

#endif