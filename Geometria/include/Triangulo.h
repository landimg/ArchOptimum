#ifndef _TRIANGULO_H_
#define _TRIANGULO_H_

#include "../../Serializacao/include/Serializacao.h"
#include "Vertice.h"
#include "Reta.h"

class Triangulo :
	public Serializacao
{
protected:
	Vertice vertice1;
	Vertice vertice2;
	Vertice vertice3;

public:
	bool operator==(const Triangulo &triangulo) const noexcept;
	bool operator!=(const Triangulo &triangulo) const noexcept;

	long double distancia12() const noexcept;
	long double distancia13() const noexcept;
	long double distancia23() const noexcept;

	long double angulo1() const noexcept;
	long double angulo2() const noexcept;
	long double angulo3() const noexcept;

	const Vertice &obterVertice1() const noexcept;
	const Vertice &obterVertice2() const noexcept;
	const Vertice &obterVertice3() const noexcept;

	const Vertice &obterVertice(const size_t idVertice) const;
	long double angulo(const size_t idVertice) const;

	long double area() const noexcept;
	long double perimetro() const noexcept;

	Vertice incentro() const noexcept;
	Vertice ortocentro() const noexcept;
	Vertice baricentro() const noexcept;
	Vertice circuncentro() const noexcept;

	void inverter() noexcept;
	void escalar(const long double X = 1.0L, const long double Y = 1.0L, const long double Z = 1.0L);
	void transladar(const Vetor &vetor) noexcept;
	void rotacionar(const Reta &reta, const long double angulo = 0.0L) noexcept;
	static bool trianguloValido(const Vertice &vertice1, const Vertice &vertice2, const Vertice &vertice3, const long double erro = 1.0e-6);

	Vetor vetorNormal() const noexcept;

	void serializar(Arquivo &arquivo) const override;
	void deserializar(Arquivo &arquivo) override;

	Triangulo() noexcept;
	Triangulo(const Vertice &vertice1, const Vertice &vertice2, const Vertice &vertice3, const long double erro = 1.0e-6);
	explicit Triangulo(const std::array < Vertice, 3 > &vertices, const long double erro = 1.0e-6);
	explicit Triangulo(const std::vector < Vertice > &vertices, const long double erro = 1.0e-6);
	virtual ~Triangulo() noexcept;
};

#endif