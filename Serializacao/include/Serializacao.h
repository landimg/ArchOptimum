#ifndef _SERIALIZACAO_H_
#define _SERIALIZACAO_H_

#include "../../Arquivo/include/Arquivo.h"

// Classe utilizada para padronizar a serialização de quaisquer objetos em memória persistente.
class Serializacao
{
public:
	// Método utilizado para serializar os dados de qualquer objeto em memória persistente.
	virtual void serializar(Arquivo &arquivo) const = 0;

	// Método utilizado para deserializar os dados de qualquer objeto em memória persistente.
	virtual void deserializar(Arquivo &arquivo) = 0;

	virtual ~Serializacao() noexcept
	{
	}
};

#endif