#ifndef _POLIGONO_H_
#define _POLIGONO_H_

#include <opencv2/opencv.hpp>

#include "../../Serializacao/include/Serializacao.h"
#include "Vertice.h"

// Classe utilizada para abstrair um polígono em espaço R³.
class Poligono :
	public Serializacao
{
protected:
	long double erro;
	std::vector < Vertice > poligono;
	std::vector < std::vector < Vertice > > buracos;

public:
	// Verificação de polígono simples e válido.
	explicit operator bool() const noexcept;

	// Verificação de igualdade entre polígonos.
	bool operator==(const Poligono &poligono) const noexcept;

	// Verificação de diferença entre polígonos.
	bool operator!=(const Poligono &poligono) const noexcept;

	// Método utilizado para serializar os dados do objeto em memória persistente.
	void serializar(Arquivo &arquivo) const override;

	// Método utilizado para deserializar os dados do objeto em memória persistente.
	void deserializar(Arquivo &arquivo) override;

	// Método utilizado para definir o erro numérico tolerado nas operações desta classe.
	void definirErroComparacao(const long double erro) noexcept;

	// Método utilizado para adicionar um buraco no polígono.
	void adicionarBuraco(const Poligono &buraco);

	Vertice centroDeMassa();

	bool simples();

	// Construtor utilizado para deserialização do objeto.
	Poligono() noexcept;
	Poligono(const std::vector < Vertice > &vertices, const long double erro = 1.0e-6);

	// Destrutor da classe Vertice.
	virtual ~Poligono() noexcept;
};

#endif