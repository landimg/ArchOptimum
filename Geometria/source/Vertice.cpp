#include "../include/Vertice.h"

bool Vertice::operator==(const Vertice &vertice) const noexcept
{
	if (this->X == vertice.X && this->Y == vertice.Y && this->Z == vertice.Z)
	{
		return true;
	}

	return false;
}

bool Vertice::operator!=(const Vertice &vertice) const noexcept
{
	if (this->X != vertice.X || this->Y != vertice.Y || this->Z != vertice.Z)
	{
		return true;
	}

	return false;
}

Vetor Vertice::operator-(const Vertice &vertice) const noexcept
{
	Vetor resultado;

	resultado.X = this->X - vertice.X;
	resultado.Y = this->Y - vertice.Y;
	resultado.Z = this->Z - vertice.Z;

	return resultado;
}

Vertice &Vertice::operator+=(const Vetor &vetor) noexcept
{
	this->X += vetor.X;
	this->Y += vetor.Y;
	this->Z += vetor.Z;

	return *this;
}

Vertice &Vertice::operator-=(const Vetor &vetor) noexcept
{
	this->X -= vetor.X;
	this->Y -= vetor.Y;
	this->Z -= vetor.Z;

	return *this;
}

Vertice Vertice::operator+(const Vetor &vetor) const noexcept
{
	return Vertice(this->X + vetor.X, this->Y + vetor.Y, this->Z + vetor.Z);
}

Vertice Vertice::operator-(const Vetor &vetor) const noexcept
{
	return Vertice(this->X - vetor.X, this->Y - vetor.Y, this->Z - vetor.Z);
}

void Vertice::serializar(Arquivo &arquivo) const
{
	arquivo.escrever(this->X);
	arquivo.escrever(this->Y);
	arquivo.escrever(this->Z);
}

void Vertice::deserializar(Arquivo &arquivo)
{
	arquivo.ler(this->X);
	arquivo.ler(this->Y);
	arquivo.ler(this->Z);
}

Vertice::Vertice(const long double X, const long double Y, const long double Z) noexcept
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}

Vertice::Vertice(const cv::Point &vertice) noexcept
{
	this->X = vertice.x;
	this->Y = vertice.y;
	this->Z = 0;
}

Vertice::Vertice(const cv::Point3_ < long double > &vertice) noexcept
{
	this->X = vertice.x;
	this->Y = vertice.y;
	this->Z = vertice.z;
}

Vertice::~Vertice() noexcept
{
}
