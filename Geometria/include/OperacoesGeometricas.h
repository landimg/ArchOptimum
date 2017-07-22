#ifndef _OPERACOES_GEOMETRICAS_H_
#define _OPERACOES_GEOMETRICAS_H_

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES

#endif

#include <math.h>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>
#include <mutex>

#include "Vertice.h"
#include "Vetor.h"
#include "Segmento.h"
#include "Reta.h"
#include "Plano.h"
#include "Triangulo.h"
#include "../../Excecao/include/Excecao.h"

class OperacoesGeometricas
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

	enum class WeilerAthertonTipo
	{
		POLIGONO_1,
		POLIGONO_2,
		INTERSECCAO,
		INVALIDO
	};
	
	class WeilerAthertonVertice :
		public Vertice
	{
	public:
		bool dentro;
		bool visitado;
		long double norma;
		WeilerAthertonTipo tipo;
		std::array < WeilerAthertonVertice *, 2 > verticeProximo; // 0 --> poligono1; 1 --> poligono2.
		std::array < WeilerAthertonVertice *, 2 > verticeAnterior; // 0 --> poligono1; 1 --> poligono2.

		WeilerAthertonVertice(const Vertice &vertice = Vertice(), const WeilerAthertonTipo tipo = WeilerAthertonTipo::INVALIDO,
			const long double norma = LDBL_MAX) :
			Vertice(vertice)
		{
			this->dentro = true;
			this->visitado = false;
			this->verticeProximo = { nullptr, nullptr };
			this->verticeAnterior = { nullptr, nullptr };
			this->tipo = tipo;
			this->norma = norma;
		}
		virtual ~WeilerAthertonVertice()
		{
		}
	};

	static inline Vertice multiplicarMatriz(const std::array < std::array < long double, 3 >, 3 > &matriz, const Vertice &vertice) noexcept
	{
		Vertice aux;

		aux.X = vertice.X * matriz[0][0] + vertice.Y * matriz[0][1] + vertice.Z * matriz[0][2];
		aux.Y = vertice.X * matriz[1][0] + vertice.Y * matriz[1][1] + vertice.Z * matriz[1][2];
		aux.Z = vertice.X * matriz[2][0] + vertice.Y * matriz[2][1] + vertice.Z * matriz[2][2];

		return aux;
	}

	static inline Vetor multiplicarMatriz(const std::array < std::array < long double, 3 >, 3 > &matriz, const Vetor &vetor) noexcept
	{
		Vetor aux;

		aux.X = vetor.X * matriz[0][0] + vetor.Y * matriz[0][1] + vetor.Z * matriz[0][2];
		aux.Y = vetor.X * matriz[1][0] + vetor.Y * matriz[1][1] + vetor.Z * matriz[1][2];
		aux.Z = vetor.X * matriz[2][0] + vetor.Y * matriz[2][1] + vetor.Z * matriz[2][2];

		return aux;
	}

	static inline void transporMatriz(std::array < std::array < long double, 3 >, 3> &matriz)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i + 1; j < 3; j++)
			{
				long double aux = matriz[i][j];
				matriz[i][j] = matriz[j][i];
				matriz[j][i] = aux;
			}
		}
	}

	static inline int sinal(long double numero)
	{
		return (0 < numero) - (numero < 0);
	}

	static inline IndiceVertice multiplicarMatriz(const std::array < std::array < long double, 3 >, 3 > &matriz, const IndiceVertice &vertice) noexcept
	{
		IndiceVertice aux;

		aux.indice = vertice.indice;

		aux.X = vertice.X * matriz[0][0] + vertice.Y * matriz[0][1] + vertice.Z * matriz[0][2];
		aux.Y = vertice.X * matriz[1][0] + vertice.Y * matriz[1][1] + vertice.Z * matriz[1][2];
		aux.Z = vertice.X * matriz[2][0] + vertice.Y * matriz[2][1] + vertice.Z * matriz[2][2];

		return aux;
	}

	static inline void rotacionarZ(std::vector < IndiceVertice > &vertices, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = -sin(angulo);
		matriz[0][2] = 0;
		matriz[1][0] = sin(angulo);
		matriz[1][1] = cos(angulo);
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = 1;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vertices[i]);
		}
	}

	static inline void rotacionarEmTornoDe(std::vector < IndiceVertice > &vertices, const Reta &referencia, const long double angulo)
	{
		Vetor u(1);
		Vetor w = referencia.obterVetor();

		if (!OperacoesGeometricas::produtoVetorial(u, w))
		{
			u = Vetor(0, 1);
		}

		OperacoesGeometricas::normalizar(w);

		Vetor v = OperacoesGeometricas::produtoVetorial(u, w);

		OperacoesGeometricas::normalizar(v);

		u = OperacoesGeometricas::produtoVetorial(v, w);

		OperacoesGeometricas::normalizar(u);

		std::array < std::array < long double, 3 >, 3 > matrizRotacao;

		matrizRotacao[0][0] = u.X;
		matrizRotacao[0][1] = u.Y;
		matrizRotacao[0][2] = u.Z;
		matrizRotacao[1][0] = v.X;
		matrizRotacao[1][1] = v.Y;
		matrizRotacao[1][2] = v.Z;
		matrizRotacao[2][0] = w.X;
		matrizRotacao[2][1] = w.Y;
		matrizRotacao[2][2] = w.Z;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i].X -= referencia.vertice.X;
			vertices[i].Y -= referencia.vertice.Y;
			vertices[i].Z -= referencia.vertice.Z;

			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matrizRotacao, vertices[i]);
		}

		OperacoesGeometricas::rotacionarZ(vertices, angulo);

		OperacoesGeometricas::transporMatriz(matrizRotacao);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matrizRotacao, vertices[i]);

			vertices[i].X += referencia.vertice.X;
			vertices[i].Y += referencia.vertice.Y;
			vertices[i].Z += referencia.vertice.Z;
		}
	}

	static inline void preRequisitosWeilerAtherton(const std::vector < Vertice > &poligono1, const std::vector < Vertice > &poligono2,
		Plano &plano, const long double erro)
	{
		if (poligono1.size() < 3 || poligono2.size() < 3)
		{
			Excecao::lancar("Um dos polígonos possui menos de três vértices.");
		}

		if (!OperacoesGeometricas::poligonoSimples(poligono1, true, erro) ||
			!OperacoesGeometricas::poligonoSimples(poligono2, true, erro))
		{
			Excecao::lancar("Um dos polígonos é complexo.");
		}

		plano = OperacoesGeometricas::planoContido(poligono1, erro);

		for (size_t i = 0; i < poligono2.size(); i++)
		{
			if (!OperacoesGeometricas::contem(plano, poligono2[i], erro))
			{
				Excecao::lancar("Os polígonos não estão contidos no mesmo plano.");
			}
		}

		Vetor orientacaoPoligono1 = plano.obterVetorNormal();
		Vetor orientacaoPoligono2 = OperacoesGeometricas::planoContido(poligono2, erro).obterVetorNormal();;

		if (!OperacoesGeometricas::mesmoOctante(orientacaoPoligono1, orientacaoPoligono2))
		{
			Excecao::lancar("Os polígonos não possuem as mesmas orientações.");
		}
	}

	static inline bool verticeDentroDePoligonoWeilerAtherton(const Vertice &vertice, const std::vector < Vertice > &poligono)
	{
		bool dentro = false;

		for (size_t i = 0, j = poligono.size() - 1; i < poligono.size(); j = i++)
		{
			if (((poligono[i].Y >= vertice.Y) != (poligono[j].Y >= vertice.Y)) &&
				(vertice.X <= (poligono[j].X - poligono[i].X) * (vertice.Y - poligono[i].Y) /
				(poligono[j].Y - poligono[i].Y) + poligono[i].X))
			{
				dentro = !dentro;
			}
		}

		return dentro;
	}

	static inline void verticesContidosWeilerAtherton(const std::vector < Vertice > &poligono1, const std::vector < Vertice > &poligono2,
		std::vector < WeilerAthertonVertice > &poligono, const Plano &plano, const long double erro)
	{
		bool coincidentes;
		std::vector < Vertice > _poligono1 = poligono1;
		std::vector < Vertice > _poligono2 = poligono2;
		Reta interseccao;
		Plano _plano;

		if (OperacoesGeometricas::interseccao(plano, _plano = Plano(Vetor(1), Vetor(0, 1), Vertice()),
			&interseccao, &coincidentes, erro) &&
			!coincidentes)
		{
			long double angulo = OperacoesGeometricas::angulo(plano, _plano);

			OperacoesGeometricas::rotacionarEmTornoDe(_poligono1, interseccao, angulo);
			OperacoesGeometricas::rotacionarEmTornoDe(_poligono2, interseccao, angulo);
		}

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int >(_poligono1.size()); i++)
		{
			poligono[i].dentro =
				OperacoesGeometricas::verticeDentroDePoligonoWeilerAtherton(_poligono1[i], _poligono2);
		}

		size_t deslocamento = _poligono1.size();

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int >(_poligono2.size()); i++)
		{
			poligono[i + deslocamento].dentro =
				OperacoesGeometricas::verticeDentroDePoligonoWeilerAtherton(_poligono2[i], _poligono1);
		}
	}

	static inline std::vector < WeilerAthertonVertice > WeilerAtherton(
		const std::vector < Vertice > &poligono1, const std::vector < Vertice > &poligono2, const long double erro)
	{
		Plano plano;

		OperacoesGeometricas::preRequisitosWeilerAtherton(poligono1, poligono2, plano, erro);

		long long int qtdVerticesPoligono1 = static_cast < long long int > (poligono1.size());
		long long int qtdVerticesPoligono2 = static_cast < long long int > (poligono2.size());
		std::vector < WeilerAthertonVertice > vertices(qtdVerticesPoligono1 + qtdVerticesPoligono2);

		for (long long int i = qtdVerticesPoligono1 - 1, indicePoligono1 = 0; i >= 0; i--, indicePoligono1++)
		{
			vertices[indicePoligono1] = WeilerAthertonVertice(poligono1[i], WeilerAthertonTipo::POLIGONO_1);
		}

		for (long long int i = qtdVerticesPoligono2 - 1, indicePoligono2 = qtdVerticesPoligono1; i >= 0; i--, indicePoligono2++)
		{
			vertices[indicePoligono2] = WeilerAthertonVertice(poligono2[i], WeilerAthertonTipo::POLIGONO_2);
		}

		OperacoesGeometricas::verticesContidosWeilerAtherton(poligono1, poligono2, vertices, plano, erro);

		for (long long int i = 0; i < qtdVerticesPoligono1; i++)
		{
			std::vector < WeilerAthertonVertice > interseccoes;
			Segmento segmento1(poligono1[i], poligono1[(i + 1) % qtdVerticesPoligono1]);

			for (long long int j = 0; j < qtdVerticesPoligono2; j++)
			{
				Vertice interseccao;

				Segmento segmento2(poligono2[j], poligono2[(j + 1) % qtdVerticesPoligono2]);

				if (OperacoesGeometricas::interseccao(segmento1, segmento2, &interseccao, erro))
				{
					WeilerAthertonVertice vertice(
						interseccao, WeilerAthertonTipo::INTERSECCAO, OperacoesGeometricas::norma(interseccao - poligono1[i]));

					vertice.verticeProximo[1] = &(vertices[j + qtdVerticesPoligono1]);

					interseccoes.push_back(vertice);
				}
			}

			std::sort(interseccoes.begin(), interseccoes.end(),
				[] (const WeilerAthertonVertice &vertice1, const WeilerAthertonVertice &vertice2) -> bool
				{
					return vertice1.norma < vertice2.norma;
				});

			long long int atual = i;

			for (size_t j = 0; j < interseccoes.size(); j++)
			{
				long long int proximo = static_cast < long long int > (vertices.size());

				vertices.push_back(interseccoes[j]);
				vertices[atual].verticeProximo[0] = &(vertices[proximo]);
				vertices[proximo].verticeAnterior[0] = &(vertices[atual]);

				atual = proximo;
			}

			vertices[atual].verticeProximo[0] = &(vertices[(i + 1) % qtdVerticesPoligono1]);
			vertices[(i + 1) % qtdVerticesPoligono1].verticeAnterior[0] = &(vertices[atual]);
		}

		for (long long int i = 0; i < qtdVerticesPoligono2; i++)
		{
			long long int inicio = i + qtdVerticesPoligono1;

			std::vector < WeilerAthertonVertice * > interseccoes;

			for (size_t j = qtdVerticesPoligono1 + qtdVerticesPoligono2; j < vertices.size(); j++)
			{
				if (vertices[j].verticeProximo[1] == &(vertices[inicio]))
				{
					vertices[j].norma = OperacoesGeometricas::norma(vertices[inicio] - vertices[j]);

					interseccoes.push_back(&(vertices[j]));
				}
			}

			interseccoes.push_back(&(vertices[inicio]));
			interseccoes.push_back(&(vertices[((i + 1) % qtdVerticesPoligono2) + qtdVerticesPoligono1]));

			vertices[inicio].norma = 0.0L;
			vertices[((i + 1) % qtdVerticesPoligono2) + qtdVerticesPoligono1].norma =
				OperacoesGeometricas::norma(vertices[((i + 1) % qtdVerticesPoligono2) + qtdVerticesPoligono1] - vertices[inicio]);

			std::sort(interseccoes.begin(), interseccoes.end(),
				[](const WeilerAthertonVertice *vertice1, const WeilerAthertonVertice *vertice2) -> bool
			{
				return vertice1->norma < vertice2->norma;
			});

			for (size_t j = 0; j < interseccoes.size() - 1; j++)
			{
				interseccoes[j]->verticeProximo[1] = interseccoes[(j + 1) % interseccoes.size()];
				interseccoes[(j + 1) % interseccoes.size()]->verticeAnterior[1] = interseccoes[j];
			}
		}

		return vertices;
	}

	static inline bool contemSegmentos(const std::vector < Vertice > &poligono, const Vertice &vertice, const long double erro)
	{
		bool contem = false;
		std::mutex mtx;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			Segmento segmento(poligono[i], poligono[(i + 1) % poligono.size()]);

			if (OperacoesGeometricas::contem(segmento, vertice, erro))
			{
				while (mtx.try_lock() == false)
				{
					std::this_thread::yield();
				}

				contem = true;

				mtx.unlock();
			}
		}

		return contem;
	}

	static inline bool _contemXY(const std::vector < Vertice > &poligono, const Vertice &vertice)
	{
		bool dentro = false;

		for (size_t i = 0, j = poligono.size() - 1; i < poligono.size(); j = i++)
		{
			if (((poligono[i].Y >= vertice.Y) != (poligono[j].Y >= vertice.Y)) &&
				(vertice.X <= (poligono[j].X - poligono[i].X) * (vertice.Y - poligono[i].Y) /
				(poligono[j].Y - poligono[i].Y) + poligono[i].X))
			{
				dentro = !dentro;
			}
		}

		return dentro;
	}

	static inline bool contemXY(const std::vector < Vertice > &poligono, Plano &planoPoligono, const Vertice &vertice, const long double erro)
	{
		std::vector < Vertice > poligonoProjetado = poligono;

		Vertice verticeProjetado = OperacoesGeometricas::projetar(vertice, planoPoligono, planoPoligono.obterVetorNormal());
		Plano xy(Vertice(1), Vertice(), Vertice(0, 1));
		Vetor vetorXY = xy.obterVetorNormal();

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			poligonoProjetado[i] = OperacoesGeometricas::projetar(poligonoProjetado[i], xy, vetorXY);
		}

		verticeProjetado = OperacoesGeometricas::projetar(verticeProjetado, xy, vetorXY);

		return OperacoesGeometricas::_contemXY(poligonoProjetado, verticeProjetado) ||
			OperacoesGeometricas::contemSegmentos(poligonoProjetado, verticeProjetado, erro);
	}

	static inline bool _contemXZ(const std::vector < Vertice > &poligono, const Vertice &vertice)
	{
		bool dentro = false;

		for (size_t i = 0, j = poligono.size() - 1; i < poligono.size(); j = i++)
		{
			if (((poligono[i].Z >= vertice.Z) != (poligono[j].Z >= vertice.Z)) &&
				(vertice.X <= (poligono[j].X - poligono[i].X) * (vertice.Z - poligono[i].Z) /
				(poligono[j].Z - poligono[i].Z) + poligono[i].X))
			{
				dentro = !dentro;
			}
		}

		return dentro;
	}

	static inline bool contemXZ(const std::vector < Vertice > &poligono, Plano &planoPoligono, const Vertice &vertice, const long double erro)
	{
		std::vector < Vertice > poligonoProjetado = poligono;

		Vertice verticeProjetado = OperacoesGeometricas::projetar(vertice, planoPoligono, planoPoligono.obterVetorNormal());
		Plano xz(Vertice(1), Vertice(), Vertice(0, 0, 1));
		Vetor vetorXZ = xz.obterVetorNormal();

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			poligonoProjetado[i] = OperacoesGeometricas::projetar(poligonoProjetado[i], xz, vetorXZ);
		}

		verticeProjetado = OperacoesGeometricas::projetar(verticeProjetado, xz, vetorXZ);

		return OperacoesGeometricas::_contemXZ(poligonoProjetado, verticeProjetado) ||
			OperacoesGeometricas::contemSegmentos(poligonoProjetado, verticeProjetado, erro);
	}

	static inline bool _contemYZ(const std::vector < Vertice > &poligono, const Vertice &vertice)
	{
		bool dentro = false;

		for (size_t i = 0, j = poligono.size() - 1; i < poligono.size(); j = i++)
		{
			if (((poligono[i].Z >= vertice.Z) != (poligono[j].Z >= vertice.Z)) &&
				(vertice.Y <= (poligono[j].Y - poligono[i].Y) * (vertice.Z - poligono[i].Z) /
				(poligono[j].Z - poligono[i].Z) + poligono[i].Y))
			{
				dentro = !dentro;
			}
		}

		return dentro;
	}

	static inline bool contemYZ(const std::vector < Vertice > &poligono, Plano &planoPoligono, const Vertice &vertice, const long double erro)
	{
		std::vector < Vertice > poligonoProjetado = poligono;

		Vertice verticeProjetado = OperacoesGeometricas::projetar(vertice, planoPoligono, planoPoligono.obterVetorNormal());
		Plano yz(Vertice(0, 1), Vertice(), Vertice(0, 0, 1));
		Vetor vetorYZ = yz.obterVetorNormal();

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			poligonoProjetado[i] = OperacoesGeometricas::projetar(poligonoProjetado[i], yz, vetorYZ);
		}

		verticeProjetado = OperacoesGeometricas::projetar(verticeProjetado, yz, vetorYZ);

		return OperacoesGeometricas::_contemYZ(poligonoProjetado, verticeProjetado) ||
			OperacoesGeometricas::contemSegmentos(poligonoProjetado, verticeProjetado, erro);
	}

	static inline long double _area(const std::vector < Vertice > &poligono, const Vetor &normal) noexcept
	{
		if (poligono.size() < 3)
		{
			return 0.0L;
		}

		std::vector < Vetor > somatorio(poligono.size());

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			somatorio[i] = OperacoesGeometricas::produtoVetorial(poligono[i] - Vertice(), poligono[(i + 1) % poligono.size()] - Vertice());
		}

		return OperacoesGeometricas::produtoEscalar(normal, std::accumulate(somatorio.begin(), somatorio.end(), Vetor()));
	}

	static inline bool interseccao(const std::vector < Vertice > &poligono, const Segmento &segmento, const long double erro)
	{
		bool houveInterseccao = false;
		std::mutex mtx;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			Vertice interseccao;
			Segmento segmentoPoligono(poligono[i], poligono[(i + 1) % poligono.size()]);

			if (OperacoesGeometricas::interseccao(segmentoPoligono, segmento, &interseccao, erro) &&
				OperacoesGeometricas::distancia(interseccao, segmentoPoligono.vertice1) <= erro &&
				OperacoesGeometricas::distancia(interseccao, segmentoPoligono.vertice2) <= erro)
			{
				while (mtx.try_lock() == false)
				{
					std::this_thread::yield();
				}

				houveInterseccao = true;

				mtx.unlock();
			}
		}

		return houveInterseccao;
	}

public:
	static inline long double angulo(const Vertice &ponto1, const Vertice &interseccao, const Vertice &ponto2)
	{
		return OperacoesGeometricas::angulo(Vetor(interseccao, ponto1), Vetor(interseccao, ponto2));
	}

	static inline long double angulo(const Vetor &vetor1, const Vetor &vetor2)
	{
		if (OperacoesGeometricas::vetorNulo(vetor1))
		{
			Excecao::lancar("vetor1 é um vetor nulo.");
		}

		if (OperacoesGeometricas::vetorNulo(vetor2))
		{
			Excecao::lancar("vetor2 é um vetor nulo.");
		}

		return atan2(
			OperacoesGeometricas::norma(OperacoesGeometricas::produtoVetorial(vetor1, vetor2)),
			OperacoesGeometricas::produtoEscalar(vetor1, vetor2));
	}

	static inline long double angulo(const Reta &reta1, const Reta &reta2)
	{
		if (OperacoesGeometricas::distancia(reta1, reta2))
		{
			Excecao::lancar("As retas não se cruzam para formar um ângulo.");
		}

		long double angulo = OperacoesGeometricas::angulo(reta1.obterVetor(), reta2.obterVetor());

		if (angulo > M_PI_2)
		{
			return angulo - M_PI_2;
		}

		return angulo;
	}

	static inline long double angulo(const Reta &reta, const Plano &plano)
	{
		if (OperacoesGeometricas::distancia(plano, reta))
		{
			Excecao::lancar("A reta não cruza o plano para formar um ângulo.");
		}

		long double angulo = OperacoesGeometricas::angulo(reta.obterVetor(), plano.obterVetorNormal());

		if (angulo > M_PI_2)
		{
			angulo -= M_PI_2;
		}

		return M_PI_2 - angulo;
	}

	static inline long double angulo(const Plano &plano1, const Plano &plano2)
	{
		if (OperacoesGeometricas::distancia(plano1, plano2))
		{
			Excecao::lancar("Os planos não se cruzam para formar um ângulo.");
		}

		long double angulo = OperacoesGeometricas::angulo(plano1.obterVetorNormal(), plano2.obterVetorNormal());

		if (angulo > M_PI_2)
		{
			return angulo - M_PI_2;
		}

		return angulo;
	}

	static inline long double distancia(const Vertice &vertice1, const Vertice &vertice2) noexcept
	{
		return sqrt(
			pow(vertice1.X - vertice2.X, 2) +
			pow(vertice1.Y - vertice2.Y, 2) +
			pow(vertice1.Z - vertice2.Z, 2));
	}

	// http://geomalgorithms.com/a02-_lines.html
	static inline long double distancia(const Segmento &segmento, const Vertice &vertice, Vertice *verticeSegmento = nullptr)
	{
		Vetor v = segmento.obterVetor();
		Vetor w = vertice - segmento.vertice1;

		long double c1 = OperacoesGeometricas::produtoEscalar(w, v);

		if (c1 <= 0)
		{
			if (verticeSegmento)
			{
				*verticeSegmento = segmento.vertice1;
			}

			return OperacoesGeometricas::distancia(vertice, segmento.vertice1);
		}

		long double c2 = OperacoesGeometricas::produtoEscalar(v, v);

		if (c2 <= c1)
		{
			if (verticeSegmento)
			{
				*verticeSegmento = segmento.vertice2;
			}

			return OperacoesGeometricas::distancia(vertice, segmento.vertice2);
		}

		long double b = c1 / c2;

		Vertice _verticeSegmento = segmento.vertice1 + v * b;

		if (verticeSegmento)
		{
			*verticeSegmento = _verticeSegmento;
		}

		return OperacoesGeometricas::distancia(vertice, _verticeSegmento);
	}

	// http://geomalgorithms.com/a07-_distance.html
	static inline long double distancia(const Segmento &segmento1, const Segmento &segmento2,
		Vertice *verticeSegmento1 = nullptr, Vertice *verticeSegmento2 = nullptr)
	{
		Vetor u = segmento1.vertice2 - segmento1.vertice1;
		Vetor v = segmento2.vertice2 - segmento2.vertice1;
		Vetor w = segmento1.vertice1 - segmento2.vertice1;
		long double a = OperacoesGeometricas::produtoEscalar(u, u);
		long double b = OperacoesGeometricas::produtoEscalar(u, v);
		long double c = OperacoesGeometricas::produtoEscalar(v, v);
		long double d = OperacoesGeometricas::produtoEscalar(u, w);
		long double e = OperacoesGeometricas::produtoEscalar(v, w);
		long double D = a * c - b * b;
		long double sN;
		long double sD = D;
		long double tN;
		long double tD = D;

		if (D == 0.0L)
		{
			sN = 0.0L;
			sD = 1.0L;
			tN = e;
			tD = c;
		}

		else
		{
			sN = (b * e - c * d);
			tN = (a * e - b * d);

			if (sN < 0.0L)
			{
				sN = 0.0L;
				tN = e;
				tD = c;
			}

			else if (sN > sD)
			{
				sN = sD;
				tN = e + b;
				tD = c;
			}
		}

		if (tN < 0.0L)
		{
			tN = 0.0L;

			if (-d < 0.0L)
			{
				sN = 0.0L;
			}

			else if (-d > a)
			{
				sN = sD;
			}

			else
			{
				sN = -d;
				sD = a;
			}
		}

		else if (tN > tD)
		{
			tN = tD;

			if ((-d + b) < 0.0L)
			{
				sN = 0;
			}

			else if ((-d + b) > a)
			{
				sN = sD;
			}

			else
			{
				sN = (-d + b);
				sD = a;
			}
		}

		long double sc = sN / sD;
		long double tc = tN / tD;

		Vetor vetorSegmento1 = u * sc;
		Vetor vetorSegmento2 = v * tc;

		Vetor dP = w + vetorSegmento1 - vetorSegmento2;

		if (verticeSegmento1)
		{
			*verticeSegmento1 = segmento1.vertice1 + vetorSegmento1;
		}

		if (verticeSegmento2)
		{
			*verticeSegmento2 = segmento2.vertice1 + vetorSegmento2;
		}

		return OperacoesGeometricas::norma(dP);
	}

	static inline long double distancia(const Reta &reta, const Vertice &vertice, Vertice *verticeReta = nullptr) noexcept
	{
		Vetor vetorNormalizadoReta = reta.obterVetor();

		OperacoesGeometricas::normalizar(vetorNormalizadoReta);

		Vetor _verticeReta1 = vertice - reta.vertice;
		Vetor _verticeReta2 = _verticeReta1 - vetorNormalizadoReta *
			OperacoesGeometricas::produtoEscalar(_verticeReta1, vetorNormalizadoReta);

		if (verticeReta)
		{
			*verticeReta = vertice - _verticeReta2;
		}
		
		return OperacoesGeometricas::norma(_verticeReta2);
	}

	// http://geomalgorithms.com/a07-_distance.html
	static inline long double distancia(const Reta &reta1, const Reta &reta2,
		Vertice *verticeReta1 = nullptr, Vertice *verticeReta2 = nullptr) noexcept
	{
		Vetor u = reta1.obterVetor();
		Vetor v = reta2.obterVetor();
		Vetor w = reta1.vertice - reta2.vertice;
		long double a = OperacoesGeometricas::produtoEscalar(u, u);
		long double b = OperacoesGeometricas::produtoEscalar(u, v);
		long double c = OperacoesGeometricas::produtoEscalar(v, v);
		long double d = OperacoesGeometricas::produtoEscalar(u, w);
		long double e = OperacoesGeometricas::produtoEscalar(v, w);
		long double D = a * c - b * b;
		long double sc;
		long double tc;

		if (D == 0.0L)
		{
			sc = 0.0;
			tc = (b > c ? d / b : e / c);
		}

		else
		{
			sc = (b * e - c * d) / D;
			tc = (a * e - b * d) / D;
		}

		Vetor vetorReta1 = u * sc;
		Vetor vetorReta2 = v * tc;

		if (verticeReta1)
		{
			*verticeReta1 = reta1.vertice + vetorReta1;
		}

		if (verticeReta2)
		{
			*verticeReta2 = reta2.vertice + vetorReta2;
		}

		Vetor dP = w + vetorReta1 - vetorReta2;

		return OperacoesGeometricas::norma(dP);
	}

	// http://geomalgorithms.com/a04-_planes.html
	static inline long double distancia(const Plano &plano, const Vertice &vertice, Vertice *verticePlano = nullptr) noexcept
	{
		Vetor normal = plano.obterVetorNormal();

		long double sn = -OperacoesGeometricas::produtoEscalar(normal, (vertice - plano.obterReta1().vertice));
		long double sd = OperacoesGeometricas::produtoEscalar(normal, normal);
		long double sb = sn / sd;

		Vertice _verticePlano = vertice + normal * sb;

		if (verticePlano)
		{
			*verticePlano = _verticePlano;
		}

		return OperacoesGeometricas::distancia(vertice, _verticePlano);
	}

	static inline long double distancia(const Plano &plano, const Reta &reta) noexcept
	{
		if (OperacoesGeometricas::paralelos(plano, reta))
		{
			return OperacoesGeometricas::distancia(plano, reta.vertice);
		}

		return 0.0L;
	}

	static inline long double distancia(const Plano &plano1, const Plano &plano2) noexcept
	{
		if (OperacoesGeometricas::paralelos(plano1, plano2))
		{
			Vertice vertice1 = plano1.obterReta1().vertice;
			Vertice vertice2 = plano2.obterReta1().vertice;
			Vetor vetorNormal = plano1.obterVetorNormal();

			return std::abs(OperacoesGeometricas::produtoEscalar(Vetor(vertice1, vertice2), vetorNormal)) /
				OperacoesGeometricas::norma(vetorNormal);
		}

		return 0.0L;
	}

	static inline bool linearmenteIndependentes(const Vetor &vetor1, const Vetor &vetor2, long double erro = 1.0e-6) noexcept
	{
		if (OperacoesGeometricas::vetorNulo(vetor1) || OperacoesGeometricas::vetorNulo(vetor2))
		{
			return false;
		}

		long double angulo = OperacoesGeometricas::angulo(vetor1, vetor2);
		erro = std::abs(erro);

		if (erro > M_PI)
		{
			erro = M_PI;
		}

		if (angulo <= erro || angulo >= M_PI - erro)
		{
			return false;
		}

		return true;
	}

	static inline Vertice pontoMedio(const std::vector < Vertice > &pontos) noexcept
	{
		Vertice vertice;

		if (pontos.size())
		{
			vertice = std::accumulate(pontos.begin(), pontos.end(), Vertice(),
				[](const Vertice &vertice1, const Vertice &vertice2) -> Vertice
				{
					return Vertice(vertice1.X + vertice2.X, vertice1.Y + vertice2.Y, vertice1.Z + vertice2.Z);
				});

			vertice.X /= pontos.size();
			vertice.Y /= pontos.size();
			vertice.Z /= pontos.size();
		}

		return vertice;
	}

	// http://geomalgorithms.com/a01-_area.html#3D%20Polygons
	static inline long double area(const std::vector < Vertice > &poligono) noexcept
	{
		Vetor normal = OperacoesGeometricas::planoContido(poligono).obterVetorNormal();

		OperacoesGeometricas::normalizar(normal);

		return std::abs(OperacoesGeometricas::_area(poligono, normal) / 2.0L);
	}

	/*
	 * http://geomalgorithms.com/a01-_area.html#3D%20Polygons
	 * false --> horário; true --> anti-horário.
	 */
	static inline bool orientacao(const std::vector < Vertice > &poligono, const Vertice &pontoDeVista) noexcept
	{
		Vertice verticePlano;

		OperacoesGeometricas::distancia(OperacoesGeometricas::planoContido(poligono), pontoDeVista, &verticePlano);

		if (OperacoesGeometricas::_area(poligono, pontoDeVista - verticePlano) >= 0.0L)
		{
			return true;
		}

		return false;
	}

	// http://www.seas.upenn.edu/~sys502/extra_materials/Polygon%20Area%20and%20Centroid.pdf
	static inline Vertice centroDeMassa(const std::vector < Vertice > &poligono, const long double erro = 1.0e-6)
	{
		Vertice baricentro;
		std::vector < std::vector < Vertice > > _poligono =
			OperacoesGeometricas::triangularizarPoligono(poligono, erro);
		std::vector < long double > areas(_poligono.size());

		std::mutex mtx;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (_poligono.size()); i++)
		{
			Triangulo triangulo = Triangulo(_poligono[i]);
			Vertice _baricentro = triangulo.baricentro();

			areas[i] = triangulo.area();

			while (!mtx.try_lock())
			{
				std::this_thread::yield();
			}

			baricentro.X += _baricentro.X * areas[i];
			baricentro.Y += _baricentro.Y * areas[i];
			baricentro.Z += _baricentro.Z * areas[i];

			mtx.unlock();
		}

		long double areaTotal = std::accumulate(areas.begin(), areas.end(), 0.0L);

		baricentro.X /= areaTotal;
		baricentro.Y /= areaTotal;
		baricentro.Z /= areaTotal;

		return baricentro;
	}

	static inline long double volume(const std::vector < std::vector < Vertice > > &poliedro, const long double erro = 1.0e-6)
	{
		std::vector < long double > somatorio(poliedro.size());

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poliedro.size()); i++)
		{
			Vetor vetor1 = OperacoesGeometricas::planoContido(poliedro[i], erro).obterVetorNormal();
			OperacoesGeometricas::normalizar(vetor1);
			vetor1 *= OperacoesGeometricas::produtoEscalar(vetor1, poliedro[i][0] - Vertice());

			if (vetor1)
			{
				Vetor vetor2;

				for (size_t j = 0; j < poliedro[i].size(); j++)
				{
					vetor2 += OperacoesGeometricas::produtoVetorial(
						poliedro[i][j] - poliedro[i][0], poliedro[i][(j + 1) % poliedro[i].size()] - poliedro[i][0]);
				}

				somatorio[i] = OperacoesGeometricas::produtoEscalar(vetor1, vetor2);
			}

			else
			{
				somatorio[i] = 0.0L;
			}
		}

		return (1.0L / 6) * std::accumulate(somatorio.begin(), somatorio.end(), 0.0L);
	}

	static inline long double norma(const Vetor &vetor) noexcept
	{
		return sqrt(pow(vetor.X, 2) + pow(vetor.Y, 2) + pow(vetor.Z, 2));
	}

	static inline bool vetorNulo(const Vetor &vetor) noexcept
	{
		return !vetor;
	}

	static inline bool mesmoOctante(const Vetor &vetor1, const Vetor &vetor2) noexcept
	{
		if (OperacoesGeometricas::sinal(vetor1.X) != 0 &&
			OperacoesGeometricas::sinal(vetor2.X) != 0)
		{
			if (OperacoesGeometricas::sinal(vetor1.X) != OperacoesGeometricas::sinal(vetor2.X))
			{
				return false;
			}
		}

		if (OperacoesGeometricas::sinal(vetor1.Y) != 0 &&
			OperacoesGeometricas::sinal(vetor2.Y) != 0)
		{
			if (OperacoesGeometricas::sinal(vetor1.Y) != OperacoesGeometricas::sinal(vetor2.Y))
			{
				return false;
			}
		}

		if (OperacoesGeometricas::sinal(vetor1.Z) != 0 &&
			OperacoesGeometricas::sinal(vetor2.Z) != 0)
		{
			if (OperacoesGeometricas::sinal(vetor1.Z) != OperacoesGeometricas::sinal(vetor2.Z))
			{
				return false;
			}
		}

		return true;
	}

	static inline bool interseccao(const Plano &plano1, const Plano &plano2,
		Reta *interseccao = nullptr, bool *coincidentes = nullptr, const long double erro = 1.0e-6)
	{
		if (coincidentes)
		{
			*coincidentes = false;
		}

		if (!OperacoesGeometricas::linearmenteIndependentes(plano1.obterVetorNormal(), plano2.obterVetorNormal(), erro))
		{
			if (OperacoesGeometricas::coincidentes(plano1, plano2, erro))
			{
				if (coincidentes)
				{
					*coincidentes = true;
				}

				if (interseccao)
				{
					*interseccao = plano1.obterReta1();
				}

				return true;
			}

			return false;
		}

		if (interseccao)
		{
			Vertice vertice;

			OperacoesGeometricas::interseccao(plano1, plano2.obterReta2(), &vertice, nullptr, erro);
			Vetor vetor = OperacoesGeometricas::produtoVetorial(plano1.obterVetorNormal(), plano2.obterVetorNormal());

			*interseccao = Reta(vertice, vetor);
		}

		return true;
	}

	static inline bool interseccao(const Plano &plano, const Reta &reta,
		Vertice *interseccao = nullptr, bool *retaContidaEmPlano = nullptr, const long double erro = 1.0e-6)
	{
		if (retaContidaEmPlano)
		{
			*retaContidaEmPlano = false;
		}

		if (OperacoesGeometricas::perpendiculares(plano.obterVetorNormal(), reta.obterVetor(), erro))
		{
			if (OperacoesGeometricas::contem(plano, reta, erro))
			{
				if (interseccao)
				{
					*interseccao = reta.vertice;
				}

				if (retaContidaEmPlano)
				{
					*retaContidaEmPlano = true;
				}

				return true;
			}

			return false;
		}

		if (interseccao)
		{
			const Vertice &verticePlano = plano.obterReta1().vertice;
			const Vetor &vetorNormalPlano = plano.obterVetorNormal();

			const Vertice &verticeReta = reta.vertice;
			const Vetor &vetorReta = reta.obterVetor();

			long double d = vetorNormalPlano.X * verticePlano.X + vetorNormalPlano.Y * verticePlano.Y + vetorNormalPlano.Z * verticePlano.Z;

			long double lambda = (d - vetorNormalPlano.X * verticeReta.X - vetorNormalPlano.Y * verticeReta.Y - vetorNormalPlano.Z * verticeReta.Z) /
				OperacoesGeometricas::produtoEscalar(vetorNormalPlano, vetorReta);

			(*interseccao).X = verticeReta.X + lambda * vetorReta.X;
			(*interseccao).Y = verticeReta.Y + lambda * vetorReta.Y;
			(*interseccao).Z = verticeReta.Z + lambda * vetorReta.Z;
		}

		return true;
	}

	// http://mathworld.wolfram.com/Line-LineIntersection.html
	static inline bool interseccao(const Reta &reta1, const Reta &reta2, 
		Vertice *interseccao = nullptr, bool *coincidentes = nullptr, const long double erro = 1.0e-6)
	{
		if (coincidentes)
		{
			*coincidentes = false;
		}

		if (OperacoesGeometricas::distancia(reta1, reta2) > std::abs(erro))
		{
			return false;
		}

		if (OperacoesGeometricas::coincidentes(reta1, reta2, erro))
		{
			if (coincidentes)
			{
				*coincidentes = true;
			}

			return true;
		}

		if (interseccao)
		{
			Vetor a = reta1.obterVetor();
			Vetor b = reta2.obterVetor();
			Vetor c = reta2.vertice - reta1.vertice;

			Vetor produtoVetorial1 = OperacoesGeometricas::produtoVetorial(c, b);
			Vetor produtoVetorial2 = OperacoesGeometricas::produtoVetorial(a, b);

			long double numerador = OperacoesGeometricas::produtoEscalar(produtoVetorial1, produtoVetorial2);
			long double denominador = pow(OperacoesGeometricas::norma(produtoVetorial2), 2);

			*interseccao = reta1.vertice + (a * (numerador / denominador));
		}

		return true;
	}

	static inline bool interseccao(const Segmento &segmento1, const Segmento &segmento2,
		Vertice *interseccao = nullptr, const long double erro = 1.0e-6)
	{
		if (OperacoesGeometricas::distancia(segmento1, segmento2, interseccao) <= erro)
		{
			return true;
		}

		return false;
	}

	static inline bool contem(const Segmento &segmento, const Vertice &vertice, const long double erro = 1.0e-6) noexcept
	{
		Vetor vetor1 = vertice - segmento.vertice1;
		Vetor vetor2 = segmento.obterVetor();

		if (OperacoesGeometricas::paralelos(vetor1, vetor2, erro) &&
			OperacoesGeometricas::norma(vetor1) <= OperacoesGeometricas::norma(vetor2) &&
			OperacoesGeometricas::mesmoOctante(vetor1, vetor2))
		{
			return true;
		}

		return false;
	}

	static inline bool contem(const Reta &reta, const Vertice &vertice, const long double erro = 1.0e-6) noexcept
	{
		Vetor vetor1(vertice, reta.vertice);
		Vetor vetor2 = reta.obterVetor();

		if (OperacoesGeometricas::paralelos(vetor1, vetor2, erro))
		{
			return true;
		}

		return false;
	}

	static inline bool contem(const Plano &plano, const Vertice &vertice, const long double erro = 1.0e-6) noexcept
	{
		Vetor vetor(vertice, plano.obterReta1().vertice);

		if (OperacoesGeometricas::perpendiculares(plano.obterVetorNormal(), vetor, erro))
		{
			return true;
		}

		return false;
	}

	static inline bool contem(const Plano &plano, const Reta &reta, const long double erro = 1.0e-6) noexcept
	{
		if (OperacoesGeometricas::contem(plano, reta.vertice, erro) &&
			OperacoesGeometricas::perpendiculares(plano.obterVetorNormal(), reta.obterVetor(), erro))
		{
			return true;
		}

		return false;
	}

	static inline bool contem(const Triangulo &triangulo, const Vertice &vertice, const long double erro = 1.0e-6) noexcept
	{
		long double area1 = 0.0L;
		long double area2 = 0.0L;
		long double area3 = 0.0L;

		if (Triangulo::trianguloValido(triangulo.obterVertice1(), triangulo.obterVertice2(), vertice))
		{
			Triangulo _triangulo(triangulo.obterVertice1(), triangulo.obterVertice2(), vertice);

			area1 = _triangulo.area();
		}

		if (Triangulo::trianguloValido(triangulo.obterVertice1(), vertice, triangulo.obterVertice3()))
		{
			Triangulo _triangulo(triangulo.obterVertice1(), vertice, triangulo.obterVertice3());

			area2 = _triangulo.area();
		}

		if (Triangulo::trianguloValido(vertice, triangulo.obterVertice2(), triangulo.obterVertice3()))
		{
			Triangulo _triangulo(vertice, triangulo.obterVertice2(), triangulo.obterVertice3());

			area3 = _triangulo.area();
		}

		if (std::abs(area1 + area2 + area3 - triangulo.area()) <= std::abs(erro))
		{
			return true;
		}

		return false;
	}

	static inline bool contem(const std::vector < Vertice > &poligono, const Vertice &vertice, const long double erro = 1.0e-6)
	{
		Plano planoPoligono;

		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
			long long int anterior = i - 1;
			long long int posterior = (i + 1) % poligono.size();

			if (anterior < 0)
			{
				anterior += poligono.size();
			}

			if (OperacoesGeometricas::linearmenteIndependentes(poligono[anterior] - poligono[i], poligono[posterior] - poligono[i], erro))
			{
				planoPoligono = Plano(poligono[anterior], poligono[i], poligono[posterior]);

				break;
			}
		}

		return OperacoesGeometricas::distancia(planoPoligono, vertice) <= std::abs(erro) &&
			OperacoesGeometricas::contemXY(poligono, planoPoligono, vertice, erro) &&
			OperacoesGeometricas::contemXZ(poligono, planoPoligono, vertice, erro) &&
			OperacoesGeometricas::contemYZ(poligono, planoPoligono, vertice, erro);
	}

	static inline bool contem(const std::vector < Vertice > &poligono1, const std::vector < Vertice > &poligono2, const long double erro = 1.0e-6) noexcept
	{
		std::mutex mtx;
		bool existeInterseccao = false;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono1.size()); i++)
		{
			for (size_t j = 0; j < poligono2.size(); j++)
			{
				Vertice interseccao;
				Segmento segmento1(poligono1[i], poligono1[(i + 1) % poligono1.size()]);
				Segmento segmento2(poligono2[j], poligono2[(j + 1) % poligono2.size()]);

				if (OperacoesGeometricas::interseccao(segmento1, segmento2, &interseccao, erro) &&
					!OperacoesGeometricas::contem(segmento1, interseccao, erro) &&
					!OperacoesGeometricas::contem(segmento2, interseccao, erro))
				{
					while (mtx.try_lock() == false)
					{
						std::this_thread::yield();
					}

					existeInterseccao = true;

					mtx.unlock();
				}
			}
		}

		if (existeInterseccao)
		{
			return false;
		}

		std::vector < bool > contido(poligono2.size());

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligono2.size()); i++)
		{
			contido[i] = OperacoesGeometricas::contem(poligono1, poligono2[i], erro);
		}

		return std::find(contido.begin(), contido.end(), false) == contido.end();
	}

	static inline Plano planoContido(const Triangulo &triangulo)
	{
		return Plano(triangulo.obterVertice2() - triangulo.obterVertice1(),
			triangulo.obterVertice3() - triangulo.obterVertice2(), triangulo.obterVertice1());
	}

	static inline Plano planoContido(const std::vector < Vertice > &vertices, const long double erro = 1.0e-6)
	{
		Plano planoPoligono;
		std::vector < Vertice > poligonoConvexo;
		std::vector < bool > verticesConvexos = OperacoesGeometricas::verticesConvexos(vertices, poligonoConvexo, erro);

		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			long long int anterior = i - 1;
			long long int posterior = (i + 1) % vertices.size();

			if (anterior < 0)
			{
				anterior += vertices.size();
			}

			if (OperacoesGeometricas::linearmenteIndependentes(vertices[anterior] - vertices[i], vertices[posterior] - vertices[i], erro) &&
				verticesConvexos[i])
			{
				Plano candidato = Plano(vertices[i] - vertices[anterior], vertices[posterior] - vertices[i], vertices[i]);
				std::vector < bool > contido(vertices.size(), false);

				#pragma omp parallel for
				for (long long int j = 0; j < static_cast < long long int > (vertices.size()); j++)
				{
					if (OperacoesGeometricas::distancia(candidato, vertices[i]) <= erro)
					{
						contido[j] = true;
					}
				}

				if (std::find(contido.begin(), contido.end(), false) == contido.end())
				{
					planoPoligono = candidato;

					break;
				}
			}
		}

		return planoPoligono;
	}

	static inline bool coincidentes(const Reta &reta1, const Reta &reta2, const long double erro = 1.0e-6) noexcept
	{
		if (OperacoesGeometricas::paralelos(reta1.obterVetor(), reta2.obterVetor(), erro) &&
			OperacoesGeometricas::paralelos(Vetor(reta1.vertice, reta2.vertice), reta1.obterVetor(), erro))
		{
			return true;
		}

		return false;
	}

	static inline bool coincidentes(const Plano &plano1, const Plano &plano2, const long double erro = 1.0e-6) noexcept
	{
		if (OperacoesGeometricas::contem(plano1, plano2.obterReta1(), erro) &&
			OperacoesGeometricas::contem(plano1, plano2.obterReta2(), erro))
		{
			return true;
		}

		return false;
	}

	static inline bool concorrentes(const Reta &reta1, const Reta &reta2, const long double erro = 1.0e-6) noexcept
	{
		return !OperacoesGeometricas::coincidentes(reta1, reta2, erro) &&
			OperacoesGeometricas::distancia(reta1, reta2) <= std::abs(erro);
	}

	static inline bool concorrentes(const Plano &plano1, const Plano &plano2, const long double erro = 1.0e-6) noexcept
	{
		return !OperacoesGeometricas::coincidentes(plano1, plano2, erro) &&
			OperacoesGeometricas::distancia(plano1, plano2) <= std::abs(erro);
	}

	static inline bool perpendiculares(const Vetor &vetor1, const Vetor &vetor2, long double erro = 1.0e-6) noexcept
	{
		if (OperacoesGeometricas::vetorNulo(vetor1) || OperacoesGeometricas::vetorNulo(vetor2))
		{
			return true;
		}

		long double angulo = OperacoesGeometricas::angulo(vetor1, vetor2);

		erro = std::abs(erro);

		if (erro > M_PI_2)
		{
			erro = M_PI_2;
		}

		if (angulo >= M_PI_2 - erro && angulo <= M_PI_2 + erro)
		{
			return true;
		}

		return false;
	}

	static inline bool perpendiculares(const Reta &reta1, const Reta &reta2, const long double erro = 1.0e-6) noexcept
	{
		return OperacoesGeometricas::perpendiculares(reta1.obterVetor(), reta2.obterVetor(), erro) &&
			OperacoesGeometricas::distancia(reta1, reta2) <= std::abs(erro);
	}

	static inline bool perpendiculares(const Plano &plano, const Reta &reta, const long double erro = 1.0e-6) noexcept
	{
		return OperacoesGeometricas::paralelos(reta.obterVetor(), plano.obterVetorNormal(), erro);
	}

	static inline bool perpendiculares(const Plano &plano1, const Plano &plano2, const long double erro = 1.0e-6) noexcept
	{
		return OperacoesGeometricas::perpendiculares(plano1.obterVetorNormal(), plano2.obterVetorNormal(), erro);
	}

	static inline bool paralelos(const Vetor &vetor1, const Vetor &vetor2, long double erro = 1.0e-6) noexcept
	{
		if (OperacoesGeometricas::vetorNulo(vetor1) || OperacoesGeometricas::vetorNulo(vetor2))
		{
			return true;
		}

		long double angulo = OperacoesGeometricas::angulo(vetor1, vetor2);

		erro = std::abs(erro);

		if (erro > M_PI)
		{
			erro = M_PI;
		}

		if (angulo <= erro || angulo >= M_PI - erro)
		{
			return true;
		}

		return false;
	}

	static inline bool paralelos(const Reta &reta1, const Reta &reta2, const long double erro = 1.0e-6) noexcept
	{
		return OperacoesGeometricas::paralelos(reta1.obterVetor(), reta2.obterVetor(), erro) &&
			OperacoesGeometricas::distancia(reta1, reta2.vertice) > std::abs(erro);
	}

	static inline bool paralelos(const Plano &plano, const Reta &reta, const long double erro = 1.0e-6) noexcept
	{
		return !OperacoesGeometricas::interseccao(plano, reta, nullptr, nullptr, erro);
	}

	static inline bool paralelos(const Plano &plano1, const Plano &plano2, const long double erro = 1.0e-6) noexcept
	{
		return !OperacoesGeometricas::interseccao(plano1, plano2, nullptr, nullptr, erro);
	}

	static inline void transladar(Vertice &vertice,
		const long double X, const long double Y, const long double Z)
	{
		vertice.X = X;
		vertice.Y = Y;
		vertice.Z = Z;
	}

	static inline void transladar(Vertice &vertice, const Vetor &vetor)
	{
		vertice += vetor;
	}

	static inline void transladar(std::vector < Vertice > &vertices,
		const long double X, const long double Y, const long double Z)
	{
		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i].X = X;
			vertices[i].Y = Y;
			vertices[i].Z = Z;
		}
	}

	static inline void transladar(std::vector < Vertice > &vertices, const Vetor &vetor)
	{
		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] += vetor;
		}
	}

	static inline void escalar(Vertice &vertice,
		const long double X = 1.0L, const long double Y = 1.0L, const long double Z = 1.0L) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = X;
		matriz[0][1] = 0;
		matriz[0][2] = 0;
		matriz[1][0] = 0;
		matriz[1][1] = Y;
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = Z;

		vertice = OperacoesGeometricas::multiplicarMatriz(matriz, vertice);
	}

	static inline void escalar(Vertice &vertice, const Plano &base, const long double fator = 1.0L)
	{
		Plano regular(Vetor(1), Vetor(0, 1), Vertice());

		long double angulo = OperacoesGeometricas::angulo(base.obterVetorNormal(), regular.obterVetorNormal());

		if (angulo == 0 || angulo == M_PI)
		{
			long double deslocamento = base.obterReta1().vertice.Z;

			vertice.Z -= deslocamento;
			OperacoesGeometricas::escalar(vertice, 1.0L, 1.0L, fator);
			vertice.Z += deslocamento;

			return;
		}

		Reta eixoRotacao;

		OperacoesGeometricas::interseccao(base, regular, &eixoRotacao, nullptr, 0.0L);
		OperacoesGeometricas::rotacionarEmTornoDe(vertice, eixoRotacao, -angulo);
		OperacoesGeometricas::escalar(vertice, 1.0L, 1.0L, fator);
		OperacoesGeometricas::rotacionarEmTornoDe(vertice, eixoRotacao, angulo);
	}

	static inline void escalar(std::vector < Vertice > &vertices,
		const long double X = 1.0L, const long double Y = 1.0L, const long double Z = 1.0L) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = X;
		matriz[0][1] = 0;
		matriz[0][2] = 0;
		matriz[1][0] = 0;
		matriz[1][1] = Y;
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = Z;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vertices[i]);
		}
	}

	static inline void escalar(std::vector < Vertice > &vertices, const Plano &base, const long double fator = 1.0L)
	{
		Plano regular(Vetor(1), Vetor(0, 1), Vertice());

		long double angulo = OperacoesGeometricas::angulo(base.obterVetorNormal(), regular.obterVetorNormal());

		if (angulo == 0 || angulo == M_PI)
		{
			long double deslocamento = base.obterReta1().vertice.Z;

			#pragma omp parallel for
			for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
			{
				vertices[i].Z -= deslocamento;
			}

			OperacoesGeometricas::escalar(vertices, 1.0L, 1.0L, fator);

			#pragma omp parallel for
			for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
			{
				vertices[i].Z += deslocamento;
			}

			return;
		}

		Reta eixoRotacao;

		OperacoesGeometricas::interseccao(base, regular, &eixoRotacao, nullptr, 0.0L);
		OperacoesGeometricas::rotacionarEmTornoDe(vertices, eixoRotacao, -angulo);
		OperacoesGeometricas::escalar(vertices, 1.0L, 1.0L, fator);
		OperacoesGeometricas::rotacionarEmTornoDe(vertices, eixoRotacao, angulo);
	}

	static inline void rotacionarX(Vertice &vertice, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = 1;
		matriz[0][1] = 0;
		matriz[0][2] = 0;
		matriz[1][0] = 0;
		matriz[1][1] = cos(angulo);
		matriz[1][2] = -sin(angulo);
		matriz[2][0] = 0;
		matriz[2][1] = sin(angulo);
		matriz[2][2] = cos(angulo);

		vertice = OperacoesGeometricas::multiplicarMatriz(matriz, vertice);
	}

	static inline void rotacionarY(Vertice &vertice, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = 0;
		matriz[0][2] = sin(angulo);
		matriz[1][0] = 0;
		matriz[1][1] = 1;
		matriz[1][2] = 0;
		matriz[2][0] = -sin(angulo);
		matriz[2][1] = 0;
		matriz[2][2] = cos(angulo);

		vertice = OperacoesGeometricas::multiplicarMatriz(matriz, vertice);
	}

	static inline void rotacionarZ(Vertice &vertice, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = -sin(angulo);
		matriz[0][2] = 0;
		matriz[1][0] = sin(angulo);
		matriz[1][1] = cos(angulo);
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = 1;

		vertice = OperacoesGeometricas::multiplicarMatriz(matriz, vertice);
	}

	static inline void rotacionarX(std::vector < Vertice > &vertices, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = 1;
		matriz[0][1] = 0;
		matriz[0][2] = 0;
		matriz[1][0] = 0;
		matriz[1][1] = cos(angulo);
		matriz[1][2] = -sin(angulo);
		matriz[2][0] = 0;
		matriz[2][1] = sin(angulo);
		matriz[2][2] = cos(angulo);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vertices[i]);
		}
	}

	static inline void rotacionarY(std::vector < Vertice > &vertices, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = 0;
		matriz[0][2] = sin(angulo);
		matriz[1][0] = 0;
		matriz[1][1] = 1;
		matriz[1][2] = 0;
		matriz[2][0] = -sin(angulo);
		matriz[2][1] = 0;
		matriz[2][2] = cos(angulo);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vertices[i]);
		}
	}

	static inline void rotacionarZ(std::vector < Vertice > &vertices, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = -sin(angulo);
		matriz[0][2] = 0;
		matriz[1][0] = sin(angulo);
		matriz[1][1] = cos(angulo);
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = 1;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vertices[i]);
		}
	}

	static inline void rotacionarX(Vetor &vetor, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = 1;
		matriz[0][1] = 0;
		matriz[0][2] = 0;
		matriz[1][0] = 0;
		matriz[1][1] = cos(angulo);
		matriz[1][2] = -sin(angulo);
		matriz[2][0] = 0;
		matriz[2][1] = sin(angulo);
		matriz[2][2] = cos(angulo);

		vetor = OperacoesGeometricas::multiplicarMatriz(matriz, vetor);
	}

	static inline void rotacionarY(Vetor &vetor, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = 0;
		matriz[0][2] = sin(angulo);
		matriz[1][0] = 0;
		matriz[1][1] = 1;
		matriz[1][2] = 0;
		matriz[2][0] = -sin(angulo);
		matriz[2][1] = 0;
		matriz[2][2] = cos(angulo);

		vetor = OperacoesGeometricas::multiplicarMatriz(matriz, vetor);
	}

	static inline void rotacionarZ(Vetor &vetor, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = -sin(angulo);
		matriz[0][2] = 0;
		matriz[1][0] = sin(angulo);
		matriz[1][1] = cos(angulo);
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = 1;

		vetor = OperacoesGeometricas::multiplicarMatriz(matriz, vetor);
	}

	static inline void rotacionarX(std::vector < Vetor > &vetores, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = 1;
		matriz[0][1] = 0;
		matriz[0][2] = 0;
		matriz[1][0] = 0;
		matriz[1][1] = cos(angulo);
		matriz[1][2] = -sin(angulo);
		matriz[2][0] = 0;
		matriz[2][1] = sin(angulo);
		matriz[2][2] = cos(angulo);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vetores.size()); i++)
		{
			vetores[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vetores[i]);
		}
	}

	static inline void rotacionarY(std::vector < Vetor > &vetores, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = 0;
		matriz[0][2] = sin(angulo);
		matriz[1][0] = 0;
		matriz[1][1] = 1;
		matriz[1][2] = 0;
		matriz[2][0] = -sin(angulo);
		matriz[2][1] = 0;
		matriz[2][2] = cos(angulo);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vetores.size()); i++)
		{
			vetores[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vetores[i]);
		}
	}

	static inline void rotacionarZ(std::vector < Vetor > &vetores, const long double angulo) noexcept
	{
		std::array < std::array < long double, 3 >, 3 > matriz;

		matriz[0][0] = cos(angulo);
		matriz[0][1] = -sin(angulo);
		matriz[0][2] = 0;
		matriz[1][0] = sin(angulo);
		matriz[1][1] = cos(angulo);
		matriz[1][2] = 0;
		matriz[2][0] = 0;
		matriz[2][1] = 0;
		matriz[2][2] = 1;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vetores.size()); i++)
		{
			vetores[i] = OperacoesGeometricas::multiplicarMatriz(matriz, vetores[i]);
		}
	}

	static inline void rotacionarEmTornoDe(Vertice &vertice, const Reta &referencia, const long double angulo)
	{
		Vetor u(1);
		Vetor w = referencia.obterVetor();

		if (!OperacoesGeometricas::produtoVetorial(u, w))
		{
			u = Vetor(0, 1);
		}

		OperacoesGeometricas::normalizar(w);

		Vetor v = OperacoesGeometricas::produtoVetorial(u, w);

		OperacoesGeometricas::normalizar(v);

		u = OperacoesGeometricas::produtoVetorial(v, w);

		OperacoesGeometricas::normalizar(u);

		std::array < std::array < long double, 3 >, 3 > matrizRotacao;

		matrizRotacao[0][0] = u.X;
		matrizRotacao[0][1] = u.Y;
		matrizRotacao[0][2] = u.Z;
		matrizRotacao[1][0] = v.X;
		matrizRotacao[1][1] = v.Y;
		matrizRotacao[1][2] = v.Z;
		matrizRotacao[2][0] = w.X;
		matrizRotacao[2][1] = w.Y;
		matrizRotacao[2][2] = w.Z;

		vertice.X -= referencia.vertice.X;
		vertice.Y -= referencia.vertice.Y;
		vertice.Z -= referencia.vertice.Z;

		vertice = OperacoesGeometricas::multiplicarMatriz(matrizRotacao, vertice);

		OperacoesGeometricas::rotacionarZ(vertice, angulo);

		OperacoesGeometricas::transporMatriz(matrizRotacao);

		vertice = OperacoesGeometricas::multiplicarMatriz(matrizRotacao, vertice);

		vertice.X += referencia.vertice.X;
		vertice.Y += referencia.vertice.Y;
		vertice.Z += referencia.vertice.Z;
	}

	static inline void rotacionarEmTornoDe(std::vector < Vertice > &vertices, const Reta &referencia, const long double angulo)
	{
		Vetor u(1);
		Vetor w = referencia.obterVetor();

		if (!OperacoesGeometricas::produtoVetorial(u, w))
		{
			u = Vetor(0, 1);
		}

		OperacoesGeometricas::normalizar(w);

		Vetor v = OperacoesGeometricas::produtoVetorial(u, w);

		OperacoesGeometricas::normalizar(v);

		u = OperacoesGeometricas::produtoVetorial(v, w);

		OperacoesGeometricas::normalizar(u);

		std::array < std::array < long double, 3 >, 3 > matrizRotacao;

		matrizRotacao[0][0] = u.X;
		matrizRotacao[0][1] = u.Y;
		matrizRotacao[0][2] = u.Z;
		matrizRotacao[1][0] = v.X;
		matrizRotacao[1][1] = v.Y;
		matrizRotacao[1][2] = v.Z;
		matrizRotacao[2][0] = w.X;
		matrizRotacao[2][1] = w.Y;
		matrizRotacao[2][2] = w.Z;

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i].X -= referencia.vertice.X;
			vertices[i].Y -= referencia.vertice.Y;
			vertices[i].Z -= referencia.vertice.Z;

			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matrizRotacao, vertices[i]);
		}

		OperacoesGeometricas::rotacionarZ(vertices, angulo);

		OperacoesGeometricas::transporMatriz(matrizRotacao);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			vertices[i] = OperacoesGeometricas::multiplicarMatriz(matrizRotacao, vertices[i]);

			vertices[i].X += referencia.vertice.X;
			vertices[i].Y += referencia.vertice.Y;
			vertices[i].Z += referencia.vertice.Z;
		}
	}

	static inline void rotacionarEmTornoDe(Vertice &vertice, const Vertice &referenciaOrigem, const Vertice &referenciaPonta,
		const long double angulo)
	{
		Reta reta(referenciaOrigem, Vetor(referenciaOrigem, referenciaPonta));

		OperacoesGeometricas::rotacionarEmTornoDe(vertice, reta, angulo);
	}

	static inline void rotacionarEmTornoDe(std::vector < Vertice > &vertices, const Vertice &referenciaOrigem,
		const Vertice &referenciaPonta, const long double angulo)
	{
		Reta reta(referenciaOrigem, Vetor(referenciaOrigem, referenciaPonta));

		OperacoesGeometricas::rotacionarEmTornoDe(vertices, reta, angulo);
	}

	static inline void rotacionarEmTornoDe(Vetor &vetor, const Vetor &referencia, const long double angulo)
	{
		Vertice vertice(vetor.X, vetor.Y, vetor.Z);

		OperacoesGeometricas::rotacionarEmTornoDe(vertice, Reta(Vertice(), referencia), angulo);

		vetor = vertice - Vertice();
	}

	static inline void rotacionarEmTornoDe(std::vector < Vetor > &vetores, const Vetor &referencia, const long double angulo)
	{
		std::vector < Vertice > vertices(vetores.size());

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vetores.size()); i++)
		{
			vertices[i] += vetores[i];
		}

		OperacoesGeometricas::rotacionarEmTornoDe(vertices, Reta(Vertice(), referencia), angulo);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vetores.size()); i++)
		{
			vetores[i] = vertices[i] - Vertice();
		}
	}

	static inline long double produtoEscalar(const Vetor &vetor1, const Vetor &vetor2) noexcept
	{
		return vetor1.X * vetor2.X + vetor1.Y * vetor2.Y + vetor1.Z * vetor2.Z;
	}

	static inline Vetor produtoVetorial(const Vertice &vertice1, const Vertice &interseccao, const Vertice &vertice2) noexcept
	{
		return OperacoesGeometricas::produtoVetorial(Vetor(interseccao, vertice1), Vetor(interseccao, vertice2));
	}

	static inline Vetor produtoVetorial(const Vetor &vetor1, const Vetor &vetor2) noexcept
	{
		Vetor resultado;

		resultado.X = vetor1.Y * vetor2.Z - vetor1.Z * vetor2.Y;
		resultado.Y = vetor1.Z * vetor2.X - vetor1.X * vetor2.Z;
		resultado.Z = vetor1.X * vetor2.Y - vetor1.Y * vetor2.X;

		return resultado;
	}

	static inline long double radianosParaGraus(const long double angulo) noexcept
	{
		return (angulo / M_PI) * 180.0;
	}

	static inline long double grausParaRadianos(const long double angulo) noexcept
	{
		return (angulo * M_PI) / 180.0;
	}

	static inline void normalizar(Vetor &vetor) noexcept
	{
		const long double norma = OperacoesGeometricas::norma(vetor);

		if (norma == 0)
		{
			return;
		}

		vetor.X /= norma;
		vetor.Y /= norma;
		vetor.Z /= norma;
	}

	static inline Vetor vetorResultante(const size_t nVetores, const Vetor *vetores) noexcept
	{
		Vetor resultado;

		if (nVetores == 0)
		{
			return resultado;
		}

		resultado = std::accumulate(vetores, vetores + nVetores, Vetor());

		resultado.X /= nVetores;
		resultado.Y /= nVetores;
		resultado.Z /= nVetores;

		return resultado;
	}

	static inline Vetor vetorResultante(const std::vector < Vetor > &vetores) noexcept
	{
		return OperacoesGeometricas::vetorResultante(vetores.size(), vetores.data());
	}

	static inline std::vector < bool > verticesConvexos(const std::vector < Vertice > &poligono,
		std::vector < Vertice > &poligonoConvexo, long double erro = 1.0e-6)
	{
		if (poligono.size() < 3)
		{
			poligonoConvexo = poligono;

			return std::vector < bool >(poligono.size(), true);
		}

		std::vector < bool > marcadorVerticesConvexos(poligono.size());
		poligonoConvexo = OperacoesGeometricas::convexHull2D(poligono, erro);

		for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
		{
		inicio:
			auto iterador = std::search(poligonoConvexo.begin(), poligonoConvexo.end(),
				poligono.begin() + i, poligono.begin() + i + 1);

			if (iterador != poligonoConvexo.end())
			{
				long long int anterior = i - 1;
				long long int posterior = (i + 1) % poligono.size();

				if (anterior < 0)
				{
					anterior += poligono.size();
				}

				Vetor vetor1 = poligono[i] - poligono[anterior];
				Vetor vetor2 = poligono[posterior] - poligono[i];

				Vetor vetorNormal = OperacoesGeometricas::produtoVetorial(vetor1, vetor2);

				if (!vetorNormal)
				{
					i++;

					goto inicio;
				}

				for (long long int j = 0; j < static_cast < long long int > (poligono.size()); j++)
				{
					long long int _anterior = j - 1;
					long long int _posterior = (j + 1) % poligono.size();

					if (_anterior < 0)
					{
						_anterior += poligono.size();
					}

					Vetor _vetor1 = poligono[j] - poligono[_anterior];
					Vetor _vetor2 = poligono[_posterior] - poligono[j];

					Vetor _vetorNormal = OperacoesGeometricas::produtoVetorial(_vetor1, _vetor2);

					if (OperacoesGeometricas::mesmoOctante(vetorNormal, _vetorNormal))
					{
						marcadorVerticesConvexos[j] = true;
					}

					else
					{
						marcadorVerticesConvexos[j] = false;
					}
				}

				break;
			}
		}

		return marcadorVerticesConvexos;
	}

	/*
	 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
	 * Funciona para quaisquer coordenadas, desde que os vértices estejam contidos no mesmo plano.
	 */
	static inline std::vector < Vertice > convexHull2D(const std::vector < Vertice > &vertices, const long double erro = 1.0e-6)
	{
		if (vertices.size() >= 3)
		{
			Plano plano;

			for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
			{
				long long int anterior = i - 1;
				long long int posterior = (i + 1) % vertices.size();

				if (anterior < 0)
				{
					anterior += vertices.size();
				}

				Vetor vetor1 = vertices[i] - vertices[anterior];
				Vetor vetor2 = vertices[posterior] - vertices[i];

				if (OperacoesGeometricas::produtoVetorial(vetor1, vetor2))
				{
					plano = Plano(vetor1, vetor2, vertices[i]);

					break;
				}
			}

			for (size_t i = 0; i < vertices.size(); i++)
			{
				if (!OperacoesGeometricas::contem(plano, vertices[i], erro))
				{
					Excecao::lancar("O plano não contém todos os vértices descritos.");
				}
			}

			std::vector < OperacoesGeometricas::IndiceVertice > poligonoEntrada(vertices.size());

			#pragma omp parallel for
			for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
			{
				poligonoEntrada[i] = OperacoesGeometricas::IndiceVertice(i, vertices[i]);
			}

			bool coincidentes;
			Reta eixoRotacao;
			Plano auxPlano(Vertice(1), Vertice(), Vertice(0, 1));

			if (OperacoesGeometricas::interseccao(plano, auxPlano, &eixoRotacao, &coincidentes, erro) && !coincidentes)
			{
				long double anguloRotacao = OperacoesGeometricas::angulo(plano, auxPlano);

				OperacoesGeometricas::rotacionarEmTornoDe(poligonoEntrada, eixoRotacao, anguloRotacao);
			}

			size_t n = poligonoEntrada.size();
			long long int k = 0;
			std::vector < Vertice > poligonoConvexo(2 * n);
			std::vector < size_t > indicesPoligonoConvexo(2 * n);

			std::sort(poligonoEntrada.begin(), poligonoEntrada.end(),
				[] (const IndiceVertice &vertice1, const IndiceVertice &vertice2) -> bool
				{
					return vertice1.X < vertice2.X ||
						(vertice1.X == vertice2.X && vertice1.Y < vertice2.Y);
				});

			// Porção inferior.
			for (size_t i = 0; i < n; ++i)
			{
				while (k >= 2 &&
					OperacoesGeometricas::produtoVetorial(
						poligonoConvexo[k - 2], poligonoConvexo[k - 1], poligonoEntrada[i]).Z <= 0)
				{
					k--;
				}

				indicesPoligonoConvexo[k] = poligonoEntrada[i].indice;
				poligonoConvexo[k++] = static_cast < Vertice > (poligonoEntrada[i]);
			}

			// Porção superior.
			for (long long int i = static_cast < long long int > (n) - 2, t = k + 1; i >= 0; i--)
			{
				while (k >= t &&
					OperacoesGeometricas::produtoVetorial(
						poligonoConvexo[k - 2], poligonoConvexo[k - 1], poligonoEntrada[i]).Z <= 0)
				{
					k--;
				}

				indicesPoligonoConvexo[k] = poligonoEntrada[i].indice;
				poligonoConvexo[k++] = static_cast < Vertice > (poligonoEntrada[i]);
			}

			poligonoConvexo.clear();

			for (size_t i = 0; i < static_cast < size_t > (k - 1); i++)
			{
				poligonoConvexo.push_back(vertices[indicesPoligonoConvexo[i]]);
			}

			return poligonoConvexo;
		}

		return vertices;
	}

	static inline bool poligonoSimples(const std::vector < Vertice > &poligono, const bool poligonoFechado = true, const long double erro = 1.0e-6) noexcept
	{
		for (long long int i = 0; i < static_cast < long long int > (poligono.size()) - (poligonoFechado ? 0 : 1); i++)
		{
			long long int anterior = i - 1;
			long long int inicioSegmento = i;
			long long int fimSegmento = (i + 1) % poligono.size();
			long long int posterior = (i + 2) % poligono.size();

			if (anterior < 0)
			{
				anterior += poligono.size();
			}

			Segmento segmentoAnterior(poligono[anterior], poligono[inicioSegmento]);
			Segmento segmentoAnalisado(poligono[inicioSegmento], poligono[fimSegmento]);
			Segmento segmentoPosterior(poligono[fimSegmento], poligono[posterior]);

			// Verificação de sobreposição de arestas vizinhas.
			if (OperacoesGeometricas::linearmenteIndependentes(segmentoAnterior.obterVetor(), segmentoAnalisado.obterVetor(), erro) == false &&
				!OperacoesGeometricas::mesmoOctante(segmentoAnterior.obterVetor(), segmentoAnalisado.obterVetor()) ||

				OperacoesGeometricas::linearmenteIndependentes(segmentoAnalisado.obterVetor(), segmentoPosterior.obterVetor(), erro) == false &&
				!OperacoesGeometricas::mesmoOctante(segmentoAnalisado.obterVetor(), segmentoPosterior.obterVetor()))
			{
				return false;
			}

			// Verificação de cruzamento em arestas não vizinhas.
			for (size_t j = 0; j < poligono.size() - (poligonoFechado ? 0 : 1); j++)
			{
				if (j == anterior || j == inicioSegmento || j == fimSegmento)
				{
					continue;
				}

				Segmento segmentoAux(poligono[j], poligono[(j + 1) % poligono.size()]);

				if (OperacoesGeometricas::interseccao(segmentoAnalisado, segmentoAux, nullptr, erro))
				{
					return false;
				}
			}
		}

		return true;
	}

	// TODO: IMPLEMENTAR ISSO.
	static inline std::vector < std::vector < Vertice > > interseccao(
		const std::vector < Vertice > &poligono1, const std::vector < Vertice > &poligono2, const long double erro = 1.0e-6)
	{
		std::vector < WeilerAthertonVertice > vertices = OperacoesGeometricas::WeilerAtherton(poligono1, poligono2, erro);
		std::vector < std::vector < Vertice > > interseccoes;

		if (poligono1.size() + poligono2.size() == vertices.size())
		{
			if (vertices[0].dentro)
			{
				return std::vector < std::vector < Vertice > > (1, poligono1);
			}

			if (vertices[poligono1.size()].dentro)
			{
				return std::vector < std::vector < Vertice > > (1, poligono2);
			}

			return interseccoes;
		}

		size_t nPoligono = 0;

		for (size_t i = poligono1.size() + poligono2.size(); i < vertices.size(); i++)
		{
			WeilerAthertonVertice &verticeInicial = vertices[i];

			if (verticeInicial.tipo == WeilerAthertonTipo::INTERSECCAO && verticeInicial.visitado == false &&
				verticeInicial.verticeProximo[0]->dentro)
			{
				verticeInicial.visitado = true;

				interseccoes.push_back(std::vector < Vertice >());

				bool poligono = false;
				WeilerAthertonVertice *pVerticeInicial = &verticeInicial;
				WeilerAthertonVertice *pVerticeAtual = verticeInicial.verticeProximo[0];

				interseccoes[nPoligono].push_back(verticeInicial);

				while (pVerticeInicial != pVerticeAtual)
				{
					pVerticeAtual->visitado = true;
					interseccoes[nPoligono].push_back(*pVerticeAtual);

					if (pVerticeAtual->tipo == WeilerAthertonTipo::INTERSECCAO)
					{
						poligono = !poligono;
					}

					if (poligono)
					{
						pVerticeAtual = pVerticeAtual->verticeProximo[1];
					}

					else
					{
						pVerticeAtual = pVerticeAtual->verticeProximo[0];
					}
				}

				nPoligono++;
			}
		}

		return interseccoes;
	}

	// TODO: IMPLEMENTAR ISSO.
	static inline std::vector < Vertice > uniao()
	{

	}

	static inline Vetor projetar(const Vetor &vetor, const Vetor &vetorComponente)
	{
		if (OperacoesGeometricas::vetorNulo(vetorComponente))
		{
			Excecao::lancar("vetorComponente é um vetor nulo.");
		}

		return vetorComponente * (OperacoesGeometricas::produtoEscalar(vetor, vetorComponente) /
			OperacoesGeometricas::produtoEscalar(vetorComponente, vetorComponente));
	}

	// projecaoValida == true --> retorno é válido; senão --> não existe uma projeção do vértice.
	static inline Vertice projetar(const Vertice &vertice, const Plano &plano, const Vetor &vetorDiretor, bool *existeProjecao = nullptr)
	{
		if (OperacoesGeometricas::contem(plano, vertice, 0.0L))
		{
			if (existeProjecao)
			{
				*existeProjecao = true;
			}

			return vertice;
		}

		Vertice interseccao;
		Reta reta(vertice, vetorDiretor);

		if (OperacoesGeometricas::interseccao(plano, reta, &interseccao, nullptr, 0.0L))
		{
			if (existeProjecao)
			{
				*existeProjecao = true;
			}

			return interseccao;
		}

		if (existeProjecao)
		{
			*existeProjecao = false;
		}

		return Vertice();
	}

	/*
	 * existeProjecao == true --> existe uma projeção; senão --> não existe uma projeção.
	 * projecaoValida == true --> retorno é válido; senão --> apenas Vertice em retorno é válido.
	 */
	static inline Reta projetar(const Reta &reta, const Plano &plano, const Vetor &vetorDiretor,
		bool *existeProjecao = nullptr, bool *projecaoValida = nullptr, Vertice *verticeErro = nullptr)
	{
		if (OperacoesGeometricas::contem(plano, reta, 0.0L))
		{
			if (existeProjecao)
			{
				*existeProjecao = true;
			}

			if (projecaoValida)
			{
				*projecaoValida = true;
			}

			return reta;
		}

		Vertice vertice1;

		if (!OperacoesGeometricas::interseccao(plano, reta, &vertice1, nullptr, 0.0L))
		{
			vertice1 = reta.vertice;
		}

		Vertice vertice2 = Vertice(
			vertice1.X + reta.obterVetor().X, vertice1.Y + reta.obterVetor().Y, vertice1.Z + reta.obterVetor().Z);

		bool existeProjecao1;
		bool existeProjecao2;

		Vertice vertice1Projetado = OperacoesGeometricas::projetar(vertice1, plano, vetorDiretor, &existeProjecao1);
		Vertice vertice2Projetado = OperacoesGeometricas::projetar(vertice2, plano, vetorDiretor, &existeProjecao2);

		if (existeProjecao1)
		{
			if (existeProjecao)
			{
				*existeProjecao = true;
			}

			if (existeProjecao2)
			{
				if (projecaoValida)
				{
					*projecaoValida = true;
				}

				return Reta(vertice1Projetado, vertice1Projetado - vertice2Projetado);
			}

			else
			{
				if (projecaoValida)
				{
					*projecaoValida = false;
				}

				if (verticeErro)
				{
					*verticeErro = vertice1Projetado;
				}

				return Reta();
			}
		}

		if (existeProjecao)
		{
			*existeProjecao = false;
		}

		if (projecaoValida)
		{
			*projecaoValida = false;
		}

		return Reta();
	}

	/*
	 * existeProjecao == true --> existe uma projeção; senão --> não existe uma projeção.
	 * projecaoValida == true --> retorno é válido; senão --> vértices de erro são válidos, formando um segmento.
	 */
	static inline Triangulo projetar(const Triangulo &triangulo, const Plano &plano, const Vetor &vetorDiretor,
		bool *existeProjecao = nullptr, bool *projecaoValida = nullptr, Segmento *segmentoErro = nullptr)
	{
		bool projecaoValida1;
		bool projecaoValida2;
		bool projecaoValida3;

		Vertice vertice1 = OperacoesGeometricas::projetar(triangulo.obterVertice1(), plano, vetorDiretor, &projecaoValida1);
		Vertice vertice2 = OperacoesGeometricas::projetar(triangulo.obterVertice2(), plano, vetorDiretor, &projecaoValida2);
		Vertice vertice3 = OperacoesGeometricas::projetar(triangulo.obterVertice3(), plano, vetorDiretor, &projecaoValida3);

		if (projecaoValida1 || projecaoValida2 || projecaoValida3)
		{
			if (existeProjecao)
			{
				*existeProjecao = true;
			}

			if (projecaoValida1 && projecaoValida2 && projecaoValida3 &&
				Triangulo::trianguloValido(vertice1, vertice2, vertice3))
			{
				if (projecaoValida)
				{
					*projecaoValida = true;
				}

				return Triangulo(vertice1, vertice2, vertice3);
			}

			if (projecaoValida)
			{
				*projecaoValida = false;
			}

			long double distancia1 = OperacoesGeometricas::distancia(vertice1, vertice2);
			long double distancia2 = OperacoesGeometricas::distancia(vertice2, vertice3);
			long double distancia3 = OperacoesGeometricas::distancia(vertice1, vertice3);

			long double maior = (std::max)((std::max)(distancia1, distancia2), distancia3);

			if (segmentoErro)
			{
				if (distancia1 == maior)
				{
					*segmentoErro = Segmento(vertice1, vertice2);
				}

				else if (distancia2 == maior)
				{
					*segmentoErro = Segmento(vertice2, vertice3);
				}

				else
				{
					*segmentoErro = Segmento(vertice1, vertice3);
				}
			}

			return Triangulo();
		}

		if (existeProjecao)
		{
			*existeProjecao = false;
		}

		return Triangulo();
	}

	static inline std::vector < std::vector < Vertice > > triangularizarPoligono(const std::vector < Vertice > &poligono,
		long double erro = 1.0e-6)
	{
		if (OperacoesGeometricas::poligonoSimples(poligono, true, erro) == false)
		{
			Excecao::lancar("O polígono é complexo.");
		}

		std::vector < std::vector < Vertice > > triangulos;
		std::vector < Vertice > _poligono = poligono;

		while (_poligono.size() >= 3)
		{
			std::vector < Vertice > poligonoConvexo;

			std::vector < bool > verticesConvexos =
				OperacoesGeometricas::verticesConvexos(_poligono, poligonoConvexo, erro);

			for (long long int i = 0; i < static_cast < long long int > (_poligono.size()) && _poligono.size() >= 3; i++)
			{
				if (verticesConvexos[i])
				{
					long long int anterior = i - 1;
					long long int posterior = (i + 1) % _poligono.size();

					if (anterior < 0)
					{
						anterior += _poligono.size();
					}

					Segmento segmento(_poligono[anterior], _poligono[posterior]);

					if (OperacoesGeometricas::interseccao(poligono, segmento, erro) == false)
					{
						if (anterior < 0)
						{
							anterior += _poligono.size();
						}

						triangulos.push_back({ _poligono[anterior], _poligono[i], _poligono[posterior] });

						_poligono.erase(_poligono.begin() + i);

						i--;
					}
				}
			}
		}

		return triangulos;
	}

	// TODO: IMPLEMENTAR ISSO.
	static inline std::vector < Vertice > somaMinkowski()
	{
		
	}

	OperacoesGeometricas() = delete;
	OperacoesGeometricas(const OperacoesGeometricas &) = delete;
	OperacoesGeometricas(const OperacoesGeometricas &&) = delete;
	OperacoesGeometricas &operator=(const OperacoesGeometricas &) = delete;
	OperacoesGeometricas &operator=(const OperacoesGeometricas &&) = delete;
};

#endif