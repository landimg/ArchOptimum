#ifndef _VERTICE_H_
#define _VERTICE_H_

#include <opencv2/opencv.hpp>

#include "../../Serializacao/include/Serializacao.h"
#include "Vetor.h"

class Vetor;

// Classe utilizada para abstrair um vértice em espaço R³.
class Vertice :
	public Serializacao
{
public:
	long double X;
	long double Y;
	long double Z;

	// Verificação de igualdade entre vértices.
	bool operator==(const Vertice &vertice) const noexcept;

	// Verificação de diferença entre vértices.
	bool operator!=(const Vertice &vertice) const noexcept;

	// Geração de um vetor pela diferença entre as posições dos vértices.
	Vetor operator-(const Vertice &vertice) const noexcept;

	// Alteração da posição do vértice, de acordo com o vetor.
	Vertice &operator+=(const Vetor &vetor) noexcept;

	// Alteração da posição do vértice, de acordo com o vetor.
	Vertice &operator-=(const Vetor &vetor) noexcept;

	// Alteração da posição do vértice, de acordo com o vetor.
	Vertice operator+(const Vetor &vetor) const noexcept;

	// Alteração da posição do vértice, de acordo com o vetor.
	Vertice operator-(const Vetor &vetor) const noexcept;

	// Método utilizado para serializar os dados do objeto em memória persistente.
	void serializar(Arquivo &arquivo) const override;

	// Método utilizado para deserializar os dados do objeto em memória persistente.
	void deserializar(Arquivo &arquivo) override;

	// Construtor que utiliza valores para os três eixos do vértice.
	explicit Vertice(const long double X = 0, const long double Y = 0, const long double Z = 0) noexcept;

	// Construtor compatível com o vértice da biblioteca OpenCV.
	explicit Vertice(const cv::Point &vertice) noexcept;

	// Construtor compatível com o vértice da biblioteca OpenCV.
	explicit Vertice(const cv::Point3_ < long double > &vertice) noexcept;

	// Destrutor da classe Vertice.
	virtual ~Vertice() noexcept;
};

#endif