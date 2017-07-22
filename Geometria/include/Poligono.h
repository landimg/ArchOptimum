#ifndef _POLIGONO_H_
#define _POLIGONO_H_

#include <opencv2/opencv.hpp>

#include "../../Serializacao/include/Serializacao.h"
#include "Vertice.h"

// Classe utilizada para abstrair um pol�gono em espa�o R�.
class Poligono :
	public Serializacao
{
protected:
	long double erro;
	std::vector < Vertice > poligono;
	std::vector < std::vector < Vertice > > buracos;

public:
	// Verifica��o de pol�gono simples e v�lido.
	explicit operator bool() const noexcept;

	// Verifica��o de igualdade entre pol�gonos.
	bool operator==(const Poligono &poligono) const noexcept;

	// Verifica��o de diferen�a entre pol�gonos.
	bool operator!=(const Poligono &poligono) const noexcept;

	// M�todo utilizado para serializar os dados do objeto em mem�ria persistente.
	void serializar(Arquivo &arquivo) const override;

	// M�todo utilizado para deserializar os dados do objeto em mem�ria persistente.
	void deserializar(Arquivo &arquivo) override;

	// M�todo utilizado para definir o erro num�rico tolerado nas opera��es desta classe.
	void definirErroComparacao(const long double erro) noexcept;

	// M�todo utilizado para adicionar um buraco no pol�gono.
	void adicionarBuraco(const Poligono &buraco);

	Vertice centroDeMassa();

	bool simples();

	// Construtor utilizado para deserializa��o do objeto.
	Poligono() noexcept;
	Poligono(const std::vector < Vertice > &vertices, const long double erro = 1.0e-6);

	// Destrutor da classe Vertice.
	virtual ~Poligono() noexcept;
};

#endif