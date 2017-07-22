#include "../include/Segmento.h"
#include "../include/OperacoesGeometricas.h"

Segmento::operator bool() const noexcept
{
	if (this->segmentoNulo())
	{
		return false;
	}

	return true;
}

bool Segmento::operator==(const Segmento &segmento) const noexcept
{
	if (this->vertice1 == segmento.vertice1 && this->vertice2 == segmento.vertice2 ||
		this->vertice2 == segmento.vertice1 && this->vertice1 == segmento.vertice2)
	{
		return true;
	}

	return false;
}

bool Segmento::segmentoNulo() const noexcept
{
	return this->vertice1 == this->vertice2;
}

long double Segmento::distancia() const noexcept
{
	return OperacoesGeometricas::distancia(this->vertice1, this->vertice2);
}

Vetor Segmento::obterVetor() const noexcept
{
	return this->vertice2 - this->vertice1;
}

Reta Segmento::obterReta() const noexcept
{
	return Reta(this->vertice1, this->obterVetor());
}

void Segmento::serializar(Arquivo &arquivo) const
{
	this->vertice1.serializar(arquivo);
	this->vertice2.serializar(arquivo);
}

void Segmento::deserializar(Arquivo &arquivo)
{
	this->vertice1.deserializar(arquivo);
	this->vertice2.deserializar(arquivo);
}

Segmento::Segmento() noexcept
{
}

Segmento::Segmento(const Vertice &vertice1, const Vertice &vertice2) noexcept
{
	this->vertice1 = vertice1;
	this->vertice2 = vertice2;
}

Segmento::~Segmento() noexcept
{
}
