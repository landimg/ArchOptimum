#ifndef _NESTING_H_
#define _NESTING_H_

#include "../../Objeto3D/include/Objeto3D.h"
#include "../../Geometria/include/OperacoesGeometricas.h"

class Nesting
{
protected:
	class SegmentoComMarca :
		public Segmento
	{
	public:
		bool marcado;

		SegmentoComMarca(const Vertice &vertice1, const Vertice &vertice2)
		{
			this->marcado = false;
			this->vertice1 = vertice1;
			this->vertice2 = vertice2;
		}
		virtual ~SegmentoComMarca()
		{
		}
	};

	class Poligono
	{
	public:
		long double area;
		long double distanciaDoCentroDeMassa;
		Vertice limitanteInferiorEsquerdo;
		Vertice limitanteSuperiorDireito;
		std::vector < Vertice > vertices;
		std::vector < std::vector < Vertice > > buracos;

		// TODO: SEGMENTO COM MARCA!
		// std::vector < SegmentoComMarca > segmentos;
		std::vector < Segmento > segmentos;

		Poligono(const std::vector < Vertice > &poligono, const std::vector < std::vector < Vertice > > &buracos)
		{
			this->vertices = poligono;
			this->buracos = buracos;
			this->area = OperacoesGeometricas::area(poligono);
			this->distanciaDoCentroDeMassa = 0.0L;
			this->limitanteInferiorEsquerdo = Vertice(LDBL_MAX, LDBL_MAX);
			this->limitanteSuperiorDireito = Vertice(-LDBL_MAX, -LDBL_MAX);
		}
		virtual ~Poligono()
		{
		}
	};

	class Objeto :
		public Poligono
	{
	public:
		long double areaRestante;
		std::vector < Poligono > poligonos;

		Objeto(const std::vector < Vertice > &bordasLimitantes) :
			Poligono(bordasLimitantes, std::vector < std::vector < Vertice > >())
		{
			this->areaRestante = this->area;
		}
		virtual ~Objeto()
		{
		}
	};

private:
	/*
	 * http://stackoverflow.com/questions/11716268/point-in-polygon-algorithm
	 * Caso vertice esteja na borda de poligono, retorno será false.
	 */
	static inline bool verticeDentroDePoligono(const Vertice &vertice, const std::vector < Vertice > &poligono);
	static inline void transladarPoligono(Poligono &poligono, const size_t indice, const Vertice &destino);
	inline void ordenarPoligonosPorArea();
	static inline bool poligonoDentroDeObjeto(const Objeto &formatoObjeto, const std::vector < Vertice > &poligono);
	static inline void rotacionarPoligonos(std::vector < Poligono > &poligonos,
		const size_t qtdRotacoes, const bool houveEspelhamento);
	static inline void espelharPoligonos(std::vector < Poligono > &poligonos);
	static inline void ordenarEscolhas(std::vector < Poligono > &poligonos);
	static inline void criarSegmentos(std::vector < Poligono > &poligonos);
	static inline std::array < size_t, 2 > Nesting::posicionarPoligonos(std::vector < Objeto > &objetos,
		std::vector < Poligono > &poligonos);

	struct Contato
	{
		size_t tipo;
		size_t caso;
		size_t idPoligonoEstatico;
		size_t idSegmentoEstatico;
		size_t idSegmentoMovel;
		Segmento segmentoEstatico;
		Segmento segmentoMovel;
		Vetor translacao;
	};



	static inline void gerarVetorTranslacao(Contato &contato, const Vertice &interseccao);
	static inline void encontrarContatos(std::vector < Contato > &contatos,
		const std::vector < Poligono > &poligonosEstaticos, const Poligono &poligonoMovel);
	static inline void encontrarVetoresFactiveis(std::vector < Contato > &contatos,
		const std::vector < Poligono > &poligonosEstaticos, const Poligono &poligonoMovel);
	static inline void pontoInicial(const std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoMovel,
		Vertice &inicioNFP, Vertice &referenciaPoligonoMovel);
	static inline bool proximoPontoValido(std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoMovel,
		Vertice &proximoInicio, Vertice &referenciaPoligonoMovel);
	static inline void transladarPoligono(Poligono &poligono, const Vetor &vetor);
	static inline bool existeInterseccao(const Poligono &poligono1, const Poligono &poligono2);
	static inline Poligono noFitPolygon(const std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoMovel);



	static inline bool grid(const long double alturaObjeto, const long double larguraObjeto,
		const std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoCandidato, long double &menorX);

protected:
	long double altura;
	long double largura;

	Objeto formatoObjeto;
	std::vector < Poligono > poligonos;
	std::vector < Objeto > objetos;

public:
	void adicionarPoligono(const std::vector < Vertice > &poligono, const std::vector < std::vector < Vertice > > &buracos);

	void rodar(const size_t qtdRotacoes, const bool permitirEspelhamento = true);

	std::vector < cv::Mat > obterImagem(const long double ampliacao) const;
	Objeto3D obterObjeto() const;

	Nesting(const std::vector < Vertice > &bordasLimitantes);
	virtual ~Nesting();
};

#endif