#include "../include/Reta.h"
#include "../include/OperacoesGeometricas.h"
#include "../../Excecao/include/Excecao.h"

Reta::operator bool() const noexcept
{
	return this->vetor ? true : false;
}

const Vetor &Reta::obterVetor() const noexcept
{
	return this->vetor;
}

bool Reta::operator==(const Reta &reta) const noexcept
{
	if (OperacoesGeometricas::coincidentes(*this, reta))
	{
		return true;
	}

	return false;
}

bool Reta::operator!=(const Reta &reta) const noexcept
{
	if (OperacoesGeometricas::coincidentes(*this, reta))
	{
		return false;
	}

	return true;
}

void Reta::serializar(Arquivo &arquivo) const
{
	this->vertice.serializar(arquivo);
	this->vetor.serializar(arquivo);
}

void Reta::deserializar(Arquivo &arquivo)
{
	this->vertice.deserializar(arquivo);
	this->vetor.deserializar(arquivo);
}

Reta::Reta()
{
}

Reta::Reta(const cv::Point &vertice, const Vetor &vetor)
{
	if (!vetor)
	{
		Excecao::lancar("O vetor diretor não pode ser nulo.");
	}

	this->vertice.X = vertice.x;
	this->vertice.Y = vertice.y;
	this->vetor = vetor;
}

Reta::Reta(const cv::Point3_ < long double > &vertice, const Vetor &vetor)
{
	if (!vetor)
	{
		Excecao::lancar("O vetor diretor não pode ser nulo.");
	}

	this->vertice.X = vertice.x;
	this->vertice.Y = vertice.y;
	this->vertice.Z = vertice.z;
	this->vetor = vetor;
}

Reta::Reta(const Vertice &vertice, const Vetor &vetor)
{
	if (!vetor)
	{
		Excecao::lancar("O vetor diretor não pode ser nulo.");
	}

	this->vertice = vertice;
	this->vetor = vetor;
}

Reta::~Reta() noexcept
{
}
