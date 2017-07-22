#include "../include/Vertice.h"
#include "../include/Vetor.h"
#include "../../Excecao/include/Excecao.h"

Vetor::operator bool() const noexcept
{
	return this->X || this->Y || this->Z;
}

bool Vetor::operator==(const Vetor &vetor) const noexcept
{
	if (this->X == vetor.X && this->Y == vetor.Y && this->Z == vetor.Z)
	{
		return true;
	}

	return false;
}

bool Vetor::operator!=(const Vetor &vetor) const noexcept
{
	if (this->X != vetor.X || this->Y != vetor.Y || this->Z != vetor.Z)
	{
		return true;
	}

	return false;
}

Vetor Vetor::operator+(const Vetor &vetor) const noexcept
{
	Vetor resultado;

	resultado.X = this->X + vetor.X;
	resultado.Y = this->Y + vetor.Y;
	resultado.Z = this->Z + vetor.Z;

	return resultado;
}

Vetor Vetor::operator-(const Vetor &vetor) const noexcept
{
	Vetor resultado;

	resultado.X = this->X - vetor.X;
	resultado.Y = this->Y - vetor.Y;
	resultado.Z = this->Z - vetor.Z;

	return resultado;
}

Vertice Vetor::operator+(const Vertice &vertice) const noexcept
{
	return vertice + *this;
}

Vertice Vetor::operator-(const Vertice &vertice) const noexcept
{
	return vertice - *this;
}

Vetor Vetor::operator-() const noexcept
{
	Vetor vetor(-this->X, -this->Y, -this->Z);

	return vetor;
}

Vetor Vetor::operator*(const long double num) const noexcept
{
	Vetor resultado;

	resultado.X = this->X * num;
	resultado.Y = this->Y * num;
	resultado.Z = this->Z * num;

	return resultado;
}

Vetor Vetor::operator/(const long double num) const
{
	if (!num)
	{
		Excecao::lancar("Não é possível dividir por zero.");
	}

	Vetor resultado;

	resultado.X = this->X / num;
	resultado.Y = this->Y / num;
	resultado.Z = this->Z / num;

	return resultado;
}

Vetor &Vetor::operator+=(const Vetor &vetor) noexcept
{
	this->X += vetor.X;
	this->Y += vetor.Y;
	this->Z += vetor.Z;

	return *this;
}

Vetor &Vetor::operator-=(const Vetor &vetor) noexcept
{
	this->X -= vetor.X;
	this->Y -= vetor.Y;
	this->Z -= vetor.Z;

	return *this;
}

Vetor &Vetor::operator*=(const long double num) noexcept
{
	this->X *= num;
	this->Y *= num;
	this->Z *= num;

	return *this;
}

Vetor &Vetor::operator/=(const long double num)
{
	if (!num)
	{
		Excecao::lancar("Não é possível dividir por zero.");
	}

	this->X /= num;
	this->Y /= num;
	this->Z /= num;

	return *this;
}

void Vetor::serializar(Arquivo &arquivo) const
{
	arquivo.escrever(this->X);
	arquivo.escrever(this->Y);
	arquivo.escrever(this->Z);
}

void Vetor::deserializar(Arquivo &arquivo)
{
	arquivo.ler(this->X);
	arquivo.ler(this->Y);
	arquivo.ler(this->Z);
}

Vetor::Vetor(const long double X, const long double Y, const long double Z) noexcept
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

Vetor::Vetor(const cv::Point3_ < long double > &vertice1, const cv::Point3_ < long double > &vertice2) noexcept
{
	this->X = vertice2.x - vertice1.x;
	this->Y = vertice2.y - vertice1.y;
	this->Z = vertice2.z - vertice1.z;
}

Vetor::Vetor(const Vertice &vertice1, const Vertice &vertice2) noexcept
{
	this->X = vertice2.X - vertice1.X;
	this->Y = vertice2.Y - vertice1.Y;
	this->Z = vertice2.Z - vertice1.Z;
}

Vetor::~Vetor() noexcept
{
}
