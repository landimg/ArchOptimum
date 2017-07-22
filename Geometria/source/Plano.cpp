#include "../include/Plano.h"
#include "../include/OperacoesGeometricas.h"
#include "../../Excecao/include/Excecao.h"

Plano::operator bool() const noexcept
{
	return (this->reta1 && this->reta2 ? true : false);
}

bool Plano::operator==(const Plano &plano) const noexcept
{
	if (OperacoesGeometricas::coincidentes(*this, plano))
	{
		return true;
	}

	return false;
}

bool Plano::operator!=(const Plano &plano) const noexcept
{
	if (OperacoesGeometricas::coincidentes(*this, plano))
	{
		return false;
	}

	return true;
}

const Reta &Plano::obterReta1() const noexcept
{
	return this->reta1;
}

const Reta &Plano::obterReta2() const noexcept
{
	return this->reta2;
}

const Vetor &Plano::obterVetorNormal() const noexcept
{
	return this->vetorNormal;
}

void Plano::trocarRetas() noexcept
{
	Reta aux = this->reta1;
	this->reta1 = this->reta2;
	this->reta2 = aux;

	this->vetorNormal = -this->vetorNormal;
}

void Plano::serializar(Arquivo &arquivo) const
{
	this->reta1.serializar(arquivo);
	this->reta2.serializar(arquivo);
	this->vetorNormal.serializar(arquivo);
}

void Plano::deserializar(Arquivo &arquivo)
{
	this->reta1.deserializar(arquivo);
	this->reta2.deserializar(arquivo);
	this->vetorNormal.deserializar(arquivo);
}

Plano::Plano()
{
}

Plano::Plano(const Vertice &vertice1, const Vertice &interseccao, const Vertice &vertice2)
{
	Vetor vetor1 = vertice1 - interseccao;
	Vetor vetor2 = vertice2 - interseccao;

	this->vetorNormal = OperacoesGeometricas::produtoVetorial(vetor1, vetor2);

	if (OperacoesGeometricas::vetorNulo(this->vetorNormal))
	{
		Excecao::lancar("Os vertices estão na mesma reta.");
	}

	this->reta1 = Reta(interseccao, vetor1);
	this->reta2 = Reta(interseccao, vetor2);
}

Plano::Plano(const Vetor &vetor1, const Vetor &vetor2, const Vertice &vertice)
{
	this->vetorNormal = OperacoesGeometricas::produtoVetorial(vetor1, vetor2);

	if (OperacoesGeometricas::vetorNulo(this->vetorNormal))
	{
		Excecao::lancar("Os vetores são linearmente dependentes.");
	}

	this->reta1 = Reta(vertice, vetor1);
	this->reta2 = Reta(vertice, vetor2);
}

Plano::Plano(const Reta &reta1, const Reta &reta2)
{
	this->vetorNormal = OperacoesGeometricas::produtoVetorial(reta1.obterVetor(), reta2.obterVetor());

	if (OperacoesGeometricas::vetorNulo(this->vetorNormal))
	{
		Excecao::lancar("Os vetores das retas são linearmente dependentes.");
	}

	if (!OperacoesGeometricas::concorrentes(reta1, reta2))
	{
		Excecao::lancar("As duas retas não são concorrentes.");
	}

	this->reta1 = Reta(reta1);
	this->reta2 = Reta(reta2);
}

Plano::~Plano() noexcept
{
}
