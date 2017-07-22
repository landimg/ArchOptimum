#ifndef _VERTICE_H_
#define _VERTICE_H_

#include <opencv2/opencv.hpp>

#include "../../Serializacao/include/Serializacao.h"
#include "Vetor.h"

class Vetor;

// Classe utilizada para abstrair um v�rtice em espa�o R�.
class Vertice :
	public Serializacao
{
public:
	long double X;
	long double Y;
	long double Z;

	// Verifica��o de igualdade entre v�rtices.
	bool operator==(const Vertice &vertice) const noexcept;

	// Verifica��o de diferen�a entre v�rtices.
	bool operator!=(const Vertice &vertice) const noexcept;

	// Gera��o de um vetor pela diferen�a entre as posi��es dos v�rtices.
	Vetor operator-(const Vertice &vertice) const noexcept;

	// Altera��o da posi��o do v�rtice, de acordo com o vetor.
	Vertice &operator+=(const Vetor &vetor) noexcept;

	// Altera��o da posi��o do v�rtice, de acordo com o vetor.
	Vertice &operator-=(const Vetor &vetor) noexcept;

	// Altera��o da posi��o do v�rtice, de acordo com o vetor.
	Vertice operator+(const Vetor &vetor) const noexcept;

	// Altera��o da posi��o do v�rtice, de acordo com o vetor.
	Vertice operator-(const Vetor &vetor) const noexcept;

	// M�todo utilizado para serializar os dados do objeto em mem�ria persistente.
	void serializar(Arquivo &arquivo) const override;

	// M�todo utilizado para deserializar os dados do objeto em mem�ria persistente.
	void deserializar(Arquivo &arquivo) override;

	// Construtor que utiliza valores para os tr�s eixos do v�rtice.
	explicit Vertice(const long double X = 0, const long double Y = 0, const long double Z = 0) noexcept;

	// Construtor compat�vel com o v�rtice da biblioteca OpenCV.
	explicit Vertice(const cv::Point &vertice) noexcept;

	// Construtor compat�vel com o v�rtice da biblioteca OpenCV.
	explicit Vertice(const cv::Point3_ < long double > &vertice) noexcept;

	// Destrutor da classe Vertice.
	virtual ~Vertice() noexcept;
};

#endif