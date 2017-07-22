#ifndef _SERIALIZACAO_H_
#define _SERIALIZACAO_H_

#include "../../Arquivo/include/Arquivo.h"

// Classe utilizada para padronizar a serializa��o de quaisquer objetos em mem�ria persistente.
class Serializacao
{
public:
	// M�todo utilizado para serializar os dados de qualquer objeto em mem�ria persistente.
	virtual void serializar(Arquivo &arquivo) const = 0;

	// M�todo utilizado para deserializar os dados de qualquer objeto em mem�ria persistente.
	virtual void deserializar(Arquivo &arquivo) = 0;

	virtual ~Serializacao() noexcept
	{
	}
};

#endif