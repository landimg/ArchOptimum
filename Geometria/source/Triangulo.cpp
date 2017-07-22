#include "../include/Triangulo.h"
#include "../include/OperacoesGeometricas.h"

bool Triangulo::operator==(const Triangulo &triangulo) const noexcept
{
	if (this->vertice1 == triangulo.vertice1 &&
		this->vertice2 == triangulo.vertice2 &&
		this->vertice3 == triangulo.vertice3)
	{
		return true;
	}

	return false;
}

bool Triangulo::operator!=(const Triangulo &triangulo) const noexcept
{
	if (this->vertice1 != triangulo.vertice1 ||
		this->vertice2 != triangulo.vertice2 ||
		this->vertice3 != triangulo.vertice3)
	{
		return true;
	}

	return false;
}

long double Triangulo::angulo1() const noexcept
{
	return OperacoesGeometricas::angulo(this->vertice2, this->vertice1, this->vertice3);
}

long double Triangulo::angulo2() const noexcept
{
	return OperacoesGeometricas::angulo(this->vertice1, this->vertice2, this->vertice3);
}

long double Triangulo::angulo3() const noexcept
{
	return OperacoesGeometricas::angulo(this->vertice1, this->vertice3, this->vertice2);
}

long double Triangulo::distancia12() const noexcept
{
	return OperacoesGeometricas::distancia(this->vertice1, this->vertice2);
}

long double Triangulo::distancia13() const noexcept
{
	return OperacoesGeometricas::distancia(this->vertice1, this->vertice3);
}

long double Triangulo::distancia23() const noexcept
{
	return OperacoesGeometricas::distancia(this->vertice2, this->vertice3);
}

const Vertice &Triangulo::obterVertice1() const noexcept
{
	return this->vertice1;
}

const Vertice &Triangulo::obterVertice2() const noexcept
{
	return this->vertice2;
}

const Vertice &Triangulo::obterVertice3() const noexcept
{
	return this->vertice3;
}

const Vertice &Triangulo::obterVertice(const size_t idVertice) const
{
	if (idVertice >= 3)
	{
		Excecao::lancar("id maior, ou igual, a 3.");
	}

	if (idVertice == 0)
	{
		return this->vertice1;
	}

	if (idVertice == 1)
	{
		return this->vertice2;
	}

	return this->vertice3;
}

long double Triangulo::angulo(const size_t idVertice) const
{
	if (idVertice >= 3)
	{
		Excecao::lancar("idVertice maior, ou igual, a 3.");
	}

	if (idVertice == 0)
	{
		return this->angulo1();
	}

	if (idVertice == 1)
	{
		return this->angulo2();
	}

	return this->angulo3();
}

long double Triangulo::area() const noexcept
{
	return 0.5 * this->distancia12() * this->distancia23() * sin(this->angulo2());
}

long double Triangulo::perimetro() const noexcept
{
	return this->distancia12() + this->distancia23() + this->distancia13();
}

Vertice Triangulo::incentro() const noexcept
{
	Vertice incentro;
	const long double perimetro = this->perimetro();

	incentro.X =
		(this->distancia23() * this->vertice1.X +
			this->distancia13() * this->vertice2.X +
			this->distancia12() * this->vertice3.X) / perimetro;

	incentro.Y =
		(this->distancia23() * this->vertice1.Y +
			this->distancia13() * this->vertice2.Y +
			this->distancia12() * this->vertice3.Y) / perimetro;

	incentro.Z =
		(this->distancia23() * this->vertice1.Z +
			this->distancia13() * this->vertice2.Z +
			this->distancia12() * this->vertice3.Z) / perimetro;

	return incentro;
}

Vertice Triangulo::ortocentro() const noexcept
{
	for (size_t i = 0; i < 3; i++)
	{
		if (this->angulo(i) == M_PI_2)
		{
			return this->obterVertice(i);
		}
	}

	long double denominador = tan(this->angulo1()) + tan(this->angulo2()) + tan(this->angulo3());
	Vertice ortocentro;

	ortocentro.X = (this->vertice1.X * tan(this->angulo1()) + this->vertice2.X * tan(this->angulo2()) +
		this->vertice3.X * tan(this->angulo3())) / denominador;

	ortocentro.Y = (this->vertice1.Y * tan(this->angulo1()) + this->vertice2.Y * tan(this->angulo2()) +
		this->vertice3.Y * tan(this->angulo3())) / denominador;

	ortocentro.Z = (this->vertice1.Z * tan(this->angulo1()) + this->vertice2.Z * tan(this->angulo2()) +
		this->vertice3.Z * tan(this->angulo3())) / denominador;

	return ortocentro;
}

Vertice Triangulo::baricentro() const noexcept
{
	Vertice baricentro;

	baricentro.X = (this->vertice1.X + this->vertice2.X + this->vertice3.X) / 3;
	baricentro.Y = (this->vertice1.Y + this->vertice2.Y + this->vertice3.Y) / 3;
	baricentro.Z = (this->vertice1.Z + this->vertice2.Z + this->vertice3.Z) / 3;

	return baricentro;
}

Vertice Triangulo::circuncentro() const noexcept
{
	long double denominador = sin(2 * this->angulo1()) + sin(2 * this->angulo2()) + sin(2 * this->angulo3());
	Vertice circuncentro;

	circuncentro.X = (this->vertice1.X * sin(2 * this->angulo1()) + this->vertice2.X * sin(2 * this->angulo2()) +
		this->vertice3.X * sin(2 * this->angulo3())) / denominador;

	circuncentro.Y = (this->vertice1.Y * sin(2 * this->angulo1()) + this->vertice2.Y * sin(2 * this->angulo2()) +
		this->vertice3.Y * sin(2 * this->angulo3())) / denominador;

	circuncentro.Z = (this->vertice1.Z * sin(2 * this->angulo1()) + this->vertice2.Z * sin(2 * this->angulo2()) +
		this->vertice3.Z * sin(2 * this->angulo3())) / denominador;

	return circuncentro;
}

void Triangulo::inverter() noexcept
{
	Vertice aux = this->vertice1;
	this->vertice1 = this->vertice2;
	this->vertice2 = aux;
}

void Triangulo::escalar(const long double X, const long double Y, const long double Z)
{
	std::array < Vertice, 3 > vertices;

	vertices[0] = this->vertice1;
	vertices[1] = this->vertice2;
	vertices[2] = this->vertice3;

	#pragma omp parallel for
	for (long long int i = 0; i < 3; i++)
	{
		OperacoesGeometricas::escalar(vertices[i], X, Y, Z);
	}

	if (!Triangulo::trianguloValido(vertices[0], vertices[1], vertices[2]))
	{
		Excecao::lancar("A operação de escala está degenerando o triângulo.");
	}

	this->vertice1 = vertices[0];
	this->vertice2 = vertices[1];
	this->vertice3 = vertices[2];
}

void Triangulo::transladar(const Vetor &vetor) noexcept
{
	this->vertice1 += vetor;
	this->vertice2 += vetor;
	this->vertice3 += vetor;
}

void Triangulo::rotacionar(const Reta &reta, const long double angulo) noexcept
{
	OperacoesGeometricas::rotacionarEmTornoDe(this->vertice1, reta, angulo);
	OperacoesGeometricas::rotacionarEmTornoDe(this->vertice2, reta, angulo);
	OperacoesGeometricas::rotacionarEmTornoDe(this->vertice3, reta, angulo);
}

bool Triangulo::trianguloValido(const Vertice &vertice1, const Vertice &vertice2, const Vertice &vertice3, const long double erro)
{
	return OperacoesGeometricas::linearmenteIndependentes(vertice1 - vertice2, vertice3 - vertice2, erro) == false;
}

Vetor Triangulo::vetorNormal() const noexcept
{
	Vetor vetor1 = this->vertice2 - this->vertice1;
	Vetor vetor2 = this->vertice3 - this->vertice2;

	return OperacoesGeometricas::produtoVetorial(vetor1, vetor2);
}

void Triangulo::serializar(Arquivo &arquivo) const
{
	this->vertice1.serializar(arquivo);
	this->vertice2.serializar(arquivo);
	this->vertice3.serializar(arquivo);
}

void Triangulo::deserializar(Arquivo &arquivo)
{
	this->vertice1.deserializar(arquivo);
	this->vertice2.deserializar(arquivo);
	this->vertice3.deserializar(arquivo);
}

Triangulo::Triangulo() noexcept
{
}

Triangulo::Triangulo(const Vertice &vertice1, const Vertice &vertice2, const Vertice &vertice3, const long double erro)
{
	if (OperacoesGeometricas::linearmenteIndependentes(vertice1 - vertice2, vertice3 - vertice2, erro) == false)
	{
		Excecao::lancar("Os três vértices são colineares.");
	}

	this->vertice1 = vertice1;
	this->vertice2 = vertice2;
	this->vertice3 = vertice3;
}

Triangulo::Triangulo(const std::array < Vertice, 3 > &vertices, const long double erro)
{
	if (OperacoesGeometricas::linearmenteIndependentes(vertices[0] - vertices[1], vertices[2] - vertices[1], erro) == false)
	{
		Excecao::lancar("Os três vértices são colineares.");
	}

	this->vertice1 = vertices[0];
	this->vertice2 = vertices[1];
	this->vertice3 = vertices[2];
}

Triangulo::Triangulo(const std::vector < Vertice > &vertices, const long double erro)
{
	if (vertices.size() < 3)
	{
		Excecao::lancar("Existem menos de três vértices.");
	}

	if (OperacoesGeometricas::linearmenteIndependentes(vertices[0] - vertices[1], vertices[2] - vertices[1], erro) == false)
	{
		Excecao::lancar("Os três vértices são colineares.");
	}

	this->vertice1 = vertices[0];
	this->vertice2 = vertices[1];
	this->vertice3 = vertices[2];
}

Triangulo::~Triangulo() noexcept
{
}
