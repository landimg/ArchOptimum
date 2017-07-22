#ifndef _PLANO_H_
#define _PLANO_H_

#include "Reta.h"
#include "Vetor.h"

class Plano :
	public Serializacao
{
protected:
	Reta reta1;
	Reta reta2;
	Vetor vetorNormal;

public:
	explicit operator bool() const noexcept;
	bool operator==(const Plano &plano) const noexcept;
	bool operator!=(const Plano &plano) const noexcept;

	const Reta &obterReta1() const noexcept;
	const Reta &obterReta2() const noexcept;
	const Vetor &obterVetorNormal() const noexcept;

	void trocarRetas() noexcept;

	void serializar(Arquivo &arquivo) const override;
	void deserializar(Arquivo &arquivo) override;

	Plano();
	Plano(const Vertice &vertice1, const Vertice &interseccao, const Vertice &vertice2);
	Plano(const Vetor &vetor1, const Vetor &vetor2, const Vertice &vertice);
	Plano(const Reta &reta1, const Reta &reta2);
	virtual ~Plano() noexcept;
};

#endif