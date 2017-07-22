#ifndef _SEGMENTO_H_
#define _SEGMENTO_H_

#include "../../Serializacao/include/Serializacao.h"
#include "Vertice.h"
#include "Reta.h"

class Segmento :
	Serializacao
{
public:
	Vertice vertice1;
	Vertice vertice2;

	operator bool() const noexcept;
	bool operator==(const Segmento &segmento) const noexcept;
	bool segmentoNulo() const noexcept;
	long double distancia() const noexcept;
	Vetor obterVetor() const noexcept;
	Reta obterReta() const noexcept;

	void serializar(Arquivo &arquivo) const override;
	void deserializar(Arquivo &arquivo) override;

	Segmento() noexcept;
	Segmento(const Vertice &vertice1, const Vertice &vertice2) noexcept;
	virtual ~Segmento() noexcept;
};

#endif