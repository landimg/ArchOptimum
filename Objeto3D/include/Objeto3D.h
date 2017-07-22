#ifndef _OBJETO_3D_H_
#define _OBJETO_3D_H_

#include <opencv2/opencv.hpp>
#include <vector>

#include "../../Arquivo/include/Arquivo.h"
#include "../../Geometria/include/Vertice.h"
#include "../../Geometria/include/Segmento.h"
#include "../../Geometria/include/Triangulo.h"
#include "../../Geometria/include/Plano.h"
#include "../../Serializacao/include/Serializacao.h"

class Objeto3D :
	public Serializacao
{
private:
	class IndiceVertice :
		public Vertice
	{
	public:
		size_t indice;

		IndiceVertice(const size_t indice, const Vertice &vertice) :
			Vertice(vertice)
		{
			this->indice = indice;
		}

		IndiceVertice() :
			Vertice()
		{
			this->indice = SIZE_MAX;
		}
	};

	class IndiceSegmento :
		public Segmento
	{
	public:
		size_t indice1;
		size_t indice2;

		IndiceSegmento(const size_t indice1, const size_t indice2, const Segmento &segmento) :
			Segmento(segmento)
		{
			this->indice1 = indice1;
			this->indice2 = indice2;
		}
	};

	static inline bool interseccao(const std::vector < Vertice > &poligono, const Segmento &segmento, const long double erro);
	inline size_t deslocamentoIndiceFace(const size_t idGrupo) const;
	inline void salvarVertices(Arquivo &arquivo, const size_t idGrupo) const;
	inline void salvarMaterial(Arquivo &arquivo, const size_t idGrupo) const;
	inline void salvarTextura(Arquivo &arquivo, const size_t idGrupo) const;
	inline void salvarFaces(Arquivo &arquivo, const bool material) const;
	inline std::vector < bool > verticesConvexos(const size_t idGrupo,
		const std::vector < size_t > &poligono) const;

protected:
	class Mapeamento2D
	{
	public:
		long double X;
		long double Y;

		explicit Mapeamento2D(const long double X = 0, const long double Y = 0)
		{
			this->X = X;
			this->Y = Y;
		}
	};

	std::string nomeObjeto;
	std::vector < std::string > nomeGrupo;

	std::vector < std::vector < Vertice > > objeto; //objeto[grupo][vertice].atributoVertice;
	std::vector < std::vector < Vetor > > vetorNormal; //objeto[grupo][vertice].atributoVetor;
	std::vector < std::vector < Mapeamento2D > > mapeamentoTextura; //mapeamentoTextura[grupo][vertice].atributoVertice;
	std::vector < std::vector < std::vector < size_t > > > face; //face[grupo][face][id] = idVertice;

	std::vector < cv::Mat * > textura;

public:
	bool existeTextura(const size_t idGrupo) const noexcept;
	cv::Mat obterTextura(const size_t idGrupo) const;

	std::vector < std::vector < Vertice > > &obterVertice() noexcept;
	std::vector < Vertice > &obterVertice(const size_t idGrupo);
	Vertice &obterVertice(const size_t idGrupo, const size_t idVertice);
	void obterFace(const size_t idGrupo, const size_t idFace, Triangulo &triangulo) const;
	void obterFace(const size_t idGrupo, const size_t idFace, std::vector < size_t > &indicesVertices) const;

	size_t obterQtdGrupos() const noexcept;
	size_t obterQtdVertices(const std::string &nomeGrupo) const;
	size_t obterQtdVertices(const size_t idGrupo) const;
	size_t obterQtdFaces(const std::string &nomeGrupo) const;
	size_t obterQtdFaces(const size_t idGrupo) const;

	std::string obterNomeObjeto() const noexcept;
	std::string obterNomeGrupo(const size_t idGrupo) const;

	size_t adicionarGrupo(std::string nomeGrupo = "");

	size_t adicionarVertice(const std::string &nomeGrupo, const Vertice &vertice);
	size_t adicionarVertice(const size_t idGrupo, const Vertice &vertice);
	std::array < size_t, 2 > adicionarVertice(const std::string &nomeGrupo, const std::vector < Vertice > &vertices,
		const bool adicaoFace, const bool facePlanoUnico);
	std::array < size_t, 2 > adicionarVertice(const size_t idGrupo, const std::vector < Vertice > &vertices,
		const bool adicaoFace, const bool facePlanoUnico);

	size_t adicionarFace(const std::string &nomeGrupo, const std::vector < size_t > &vertices, const bool facePlanoUnico);
	size_t adicionarFace(const size_t idGrupo, const std::vector < size_t > &vertices, const bool facePlanoUnico);
	size_t adicionarFace(const std::string &nomeGrupo,
		const size_t idVertice1, const size_t idVertice2, const size_t idVertice3);
	size_t adicionarFace(const size_t idGrupo,
		const size_t idVertice1, const size_t idVertice2, const size_t idVertice3);

	void definirMapeamentoTextura(const std::string &nomeGrupo, const size_t idVertice, const long double Y, const long double X);
	void definirMapeamentoTextura(const size_t idGrupo, const size_t idVertice, const long double Y, const long double X);
	void definirTextura(const std::string &nomeGrupo, const cv::Mat &textura);
	void definirTextura(const size_t idGrupo, const cv::Mat &textura);

	void deslocarVertice(const std::string &nomeGrupo, const size_t idVertice, const Vertice &novaPosicao);
	void deslocarVertice(const size_t idGrupo, const size_t idVertice, const Vertice &novaPosicao);
	void deslocarVertice(const std::string &nomeGrupo, const size_t idVertice, const long double X, const long double Y, const long double Z);
	void deslocarVertice(const size_t idGrupo, const size_t idVertice, const long double X, const long double Y, const long double Z);
	void deslocarVertice(const std::string &nomeGrupo, const size_t idVertice, const Vetor &deslocamento);
	void deslocarVertice(const size_t idGrupo, const size_t idVertice, const Vetor &deslocamento);

	void removerGrupo(const std::string &nomeGrupo);
	void removerGrupo(const size_t idGrupo);
	void removerVertice(const std::string &nomeGrupo, const size_t idVertice);
	void removerVertice(const size_t idGrupo, const size_t idVertice);
	void removerFace(const std::string &nomeGrupo, const size_t idFace);
	void removerFace(const size_t idGrupo, const size_t idFace);
	void removerTextura(const std::string &nomeGrupo);
	void removerTextura(const size_t idGrupo);

	void salvar(const bool faces = false, const bool material = false) const;

	void serializar(Arquivo &arquivo) const override;
	void deserializar(Arquivo &arquivo) override;

	void triangularizarFaces();

	Objeto3D();
	explicit Objeto3D(const std::string &nomeObjeto);
	virtual ~Objeto3D();
};

#endif