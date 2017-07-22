#include "../include/Nesting.h"

#include <atomic>

#define ALTURA 20
#define LARGURA 20

void Nesting::gerarVetorTranslacao(Contato &contato, const Vertice &interseccao)
{
	if (interseccao == contato.segmentoEstatico.vertice1 || interseccao == contato.segmentoEstatico.vertice2)
	{
		if (interseccao == contato.segmentoMovel.vertice1 || interseccao == contato.segmentoMovel.vertice2)
		{
			contato.tipo = 1;

			// TODO: SERÁ QUE VAI FUNCIONAR???
			if (OperacoesGeometricas::paralelos(contato.segmentoEstatico.obterVetor(), contato.segmentoMovel.obterVetor(), 0.0L))
			{
				contato.translacao = contato.segmentoEstatico.obterVetor();

				contato.caso = 8;
			}

			else if (contato.segmentoEstatico.vertice1 == contato.segmentoMovel.vertice1)
			{
				if (OperacoesGeometricas::produtoVetorial(contato.segmentoEstatico.obterVetor(), contato.segmentoMovel.obterVetor()).Z > 0)
				{
					contato.translacao = -contato.segmentoMovel.obterVetor();
					contato.caso = 1;
				}

				else
				{
					contato.translacao = contato.segmentoEstatico.obterVetor();
					contato.caso = 2;
				}
			}

			else if (contato.segmentoEstatico.vertice1 == contato.segmentoMovel.vertice2 &&
				OperacoesGeometricas::produtoVetorial(contato.segmentoEstatico.obterVetor(), contato.segmentoMovel.obterVetor()).Z > 0)
			{
				contato.translacao = contato.segmentoEstatico.obterVetor();
				contato.caso = 4;
			}

			else if (contato.segmentoEstatico.vertice2 == contato.segmentoMovel.vertice1 &&
				OperacoesGeometricas::produtoVetorial(contato.segmentoEstatico.obterVetor(), contato.segmentoMovel.obterVetor()).Z < 0)
			{
				contato.translacao = -contato.segmentoMovel.obterVetor();
				contato.caso = 6;
			}

			else
			{
				contato.translacao = Vetor();
				contato.caso = SIZE_MAX;
			}
		}

		else
		{
			contato.tipo = 3;
			contato.translacao = -(contato.segmentoMovel.vertice2 - interseccao);
		}
	}

	else
	{
		contato.tipo = 2;
		contato.translacao = contato.segmentoEstatico.vertice2 - interseccao;
	}
}

void Nesting::encontrarContatos(std::vector < Contato > &contatos,
	const std::vector < Poligono > &poligonosEstaticos, const Poligono &poligonoMovel)
{
	for (size_t i = 0; i < poligonosEstaticos.size(); i++)
	{
		for (size_t j = 0; j < poligonosEstaticos[i].segmentos.size(); j++)
		{
			for (size_t k = 0; k < poligonoMovel.segmentos.size(); k++)
			{
				Vertice interseccao;

				// TODO: SERÁ QUE VAI FUNCIONAR???
				if (OperacoesGeometricas::interseccao(poligonosEstaticos[i].segmentos[j], poligonoMovel.segmentos[k], &interseccao))
				{
					Nesting::Contato contato;

					contato.segmentoEstatico = static_cast < Segmento > (poligonosEstaticos[i].segmentos[j]);
					contato.segmentoMovel = static_cast < Segmento > (poligonoMovel.segmentos[k]);

					Nesting::gerarVetorTranslacao(contato, interseccao);

					if (contato.translacao)
					{
						contato.idPoligonoEstatico = i;
						contato.idSegmentoEstatico = j;
						contato.idSegmentoMovel = k;

						contatos.push_back(contato);
					}
				}
			}
		}
	}
}

// TODO: IMPLEMENTAR ISSO!
void Nesting::encontrarVetoresFactiveis(std::vector < Contato > &contatos,
	const std::vector < Poligono > &poligonosEstaticos, const Poligono &poligonoMovel)
{
	/*for (size_t i = 0; i < contatos.size(); i++)
	{
		Vetor candidato = contatos[i].translacao;

		for (size_t j = 0; j < contatos.size(); j++)
		{
			std::vector < Vetor > vetores;

			if (contatos[j].tipo == 1)
			{
				if (contatos[j].caso == 8)
				{
					break;
				}

				vetores.push_back(contatos[j].segmentoEstatico.obterVetor());
				vetores.push_back(-contatos[j].segmentoMovel.obterVetor());
			}

			else if (contatos[j].tipo == 2)
			{
				vetores.push_back(contatos[j].segmentoEstatico.obterVetor());
			}

			else
			{
				vetores.push_back(-contatos[j].segmentoMovel.obterVetor());
			}

			for ()
			{
				
			}
		}
	}*/
}

void Nesting::transladarPoligono(Poligono &poligono, const Vetor &vetor)
{
	OperacoesGeometricas::transladar(poligono.vertices, vetor);

	for (size_t i = 0; i < poligono.buracos.size(); i++)
	{
		OperacoesGeometricas::transladar(poligono.buracos[i], vetor);
	}

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (poligono.segmentos.size()); i++)
	{
		poligono.segmentos[i].vertice1 += vetor;
		poligono.segmentos[i].vertice2 += vetor;
	}

	poligono.limitanteInferiorEsquerdo += vetor;
	poligono.limitanteSuperiorDireito += vetor;
}

bool Nesting::existeInterseccao(const Poligono &poligono1, const Poligono &poligono2)
{
	std::atomic < bool > houveInterseccao = false;

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (poligono1.segmentos.size()); i++)
	{
		std::mutex mtx;

		for (size_t j = 0; j < poligono2.segmentos.size() && !houveInterseccao; j++)
		{
			if (OperacoesGeometricas::interseccao(poligono1.segmentos[i], poligono2.segmentos[j]))
			{
				while (mtx.try_lock() == false)
				{
					std::this_thread::yield();
				}

				houveInterseccao = true;

				break;
			}
		}
	}

	return houveInterseccao;
}

void Nesting::pontoInicial(const std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoMovel,
	Vertice &inicioNFP, Vertice &referenciaPoligonoMovel)
{
	size_t idPoligono = SIZE_MAX;
	size_t idMenor = SIZE_MAX;
	size_t idMaior = SIZE_MAX;
	Vertice verticeMenor(LDBL_MAX, LDBL_MAX);
	Vertice verticeMaior(-LDBL_MAX, -LDBL_MAX);

	for (size_t i = 0; i < poligonosEstaticos.size(); i++)
	{
		for (size_t j = 0; j < poligonosEstaticos[i].vertices.size(); j++)
		{
			const Vertice &vertice = poligonosEstaticos[i].vertices[j];

			if (verticeMenor.Y > vertice.Y)
			{
				idPoligono = i;
				idMenor = j;
				verticeMenor = poligonosEstaticos[i].vertices[j];
			}

			else if (verticeMenor.Y == vertice.Y && verticeMenor.X > vertice.X)
			{
				idPoligono = i;
				idMenor = j;
				verticeMenor = vertice;
			}
		}
	}

	for (size_t i = 0; i < poligonoMovel.vertices.size(); i++)
	{
		const Vertice &vertice = poligonoMovel.vertices[i];

		if (verticeMaior.Y < vertice.Y)
		{
			idMaior = i;
			verticeMaior = vertice;
		}

		else if (verticeMaior.Y == vertice.Y && verticeMaior.X < vertice.X)
		{
			idMenor = i;
			verticeMaior = vertice;
		}
	}

	Nesting::transladarPoligono(poligonoMovel, idMaior, poligonosEstaticos[idPoligono].vertices[idMenor]);

	inicioNFP = verticeMaior;
	referenciaPoligonoMovel = verticeMaior;
}

// TODO: IMPLEMENTAR ISSO!
bool Nesting::proximoPontoValido(std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoMovel,
	Vertice &proximoInicio, Vertice &referenciaPoligonoMovel)
{
	//size_t qtdBordasPoligonoMovel = poligonoMovel.segmentos.size();

	//for (size_t k = 0; k < poligonosEstaticos.size(); k++)
	//{
	//	size_t qtdBordasPoligonoEstatico = poligonosEstaticos[k].segmentos.size();

	//	for (size_t i = 0; i < qtdBordasPoligonoEstatico; i++)
	//	{
	//		if (poligonosEstaticos[k].segmentos[i].marcado)
	//		{
	//			continue;
	//		}

	//		SegmentoComMarca &bordaPoligonoEstatico = poligonosEstaticos[k].segmentos[i];

	//		for (size_t j = 0; j < qtdBordasPoligonoMovel; j++)
	//		{
	//			SegmentoComMarca &bordaMovel = poligonoMovel.segmentos[j];

	//			// translate the PolygonB so that movingEdge start is on the start of the static edge.
	//			bool fimBordaPoligonoMovel = false;
	//			bool existeInterseccao = Nesting::existeInterseccao(poligonosEstaticos[k], poligonoMovel);

	//			Nesting::transladarPoligono(poligonoMovel, bordaMovel.vertice1 - bordaPoligonoEstatico.vertice1);

	//			while (existeInterseccao && !fimBordaPoligonoMovel)
	//			{
	//				// Edge slide until not intersecting or end of staticEdge reached
	//				Toucher currentToucher = MakeToucher(bordaPoligonoEstatico.vertice1);
	//				Toucher trimmedToucher = Trim(currentToucher, PolygonA, PolygonB);

	//				// Nesting::transladarPoligono(poligonoMovel, );
	//				PolygonB.Translate(trimmedToucher.Translation);

	//				existeInterseccao = Nesting::existeInterseccao(poligonosEstaticos[k], poligonoMovel);

	//				// if (existeInterseccao && OperacoesGeometricas::distancia(bordaMovel.vertice1(), bordaPoligonoEstatico.vertice2()) <= 1.0e-6)
	//				if (existeInterseccao && bordaMovel.vertice1 == bordaPoligonoEstatico.vertice2)
	//				{
	//					fimBordaPoligonoMovel = true;
	//				}
	//			}

	//			// mark the traversed edge as seen (whether edge start point found or not)
	//			bordaPoligonoEstatico.marcado = true;

	//			if (!existeInterseccao)
	//			{
	//				// set the references to the points passed in to be the nextStartPoint and return true;
	//				proximoInicio = bordaMovel.vertice1;
	//				referenciaPoligonoMovel = bordaMovel.vertice1;

	//				return true;
	//			}
	//		}
	//	}
	//}

	return false;
}

// TODO: IMPLEMENTAR ISSO!
Nesting::Poligono Nesting::noFitPolygon(const std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoMovel)
{
	return Nesting::Poligono(std::vector < Vertice >(), std::vector < std::vector < Vertice > >());

	//bool bStartPointAvailable = true;
	//Vertice inicioNFP;
	//Vertice referenciaPoligonoMovel;
	//size_t loopCount = 0; // counter for number of loops in NFP

	//std::vector < std::vector < Segmento > > nfpEdges; // an array of arrays of lines to store NFP edges 

	//Nesting::pontoInicial(poligonosEstaticos, poligonoMovel, inicioNFP, referenciaPoligonoMovel);

	//while (bStartPointAvailable)
	//{
	//	bStartPointAvailable = false;

	//	std::vector < Contato > contatos;

	//	// Find touching segments & touching points on those, generate touching structures 
	//	Nesting::encontrarContatos(contatos, poligonosEstaticos, poligonoMovel);

	//	// Eliminate non feasible touchers, ones that cause immediate intersection
	//	Nesting::encontrarVetoresFactiveis(contatos, poligonosEstaticos, poligonoMovel);

	//	// Trim feasible translations against polygon A and B
	//	Touchers[] trimmedTouchers = Trim(feasibleTouchers, poligonosEstaticos, poligonoMovel);

	//	// Sort trimmed translations by length
	//	Touchers[] lengthSortedTouchers = Sort(trimmedTouchers);

	//	// Translate polygon B along longest feasible translation vector
	//	B.Translate(lengthSortedTouchers[0].Translation);

	//	// Add translation to nfpEdges & mark traversed edge on static
	//	nfpEdges[loopCount].Add(lengthSortedTranslations[0].Translation);
	//	A.MarkEdge(lengthSortedTranslations[0].StaticEdgeID);

	//	if (inicioNFP == referenciaPoligonoMovel) // completed an NFP loop
	//	{
	//		Vertice nextStartPoint;
	//		// find next feasible start point – reset PolygonB_RefPoint to relevant point 

	//		if (Nesting::proximoPontoValido(A, B, nextStartPoint, referenciaPoligonoMovel))
	//		{
	//			//Translate polygon B to nextStartPoint 
	//			B.Translate(referenciaPoligonoMovel - nextStartPoint);

	//			inicioNFP = nextStartPoint;
	//			loopCount++;
	//		}
	//	}

	//	else
	//	{
	//		bStartPointAvailable = true; // allow edge traversal to continue 
	//	}
	//}

	//NFPAB = Complete(nfpEdges); //Reconstitute NFP from nfpEdges 
}

bool Nesting::verticeDentroDePoligono(const Vertice &vertice, const std::vector < Vertice > &poligono)
{
	bool dentro = false;

	for (size_t i = 0, j = poligono.size() - 1; i < poligono.size(); j = i++)
	{
		if (((poligono[i].Y > vertice.Y) != (poligono[j].Y > vertice.Y)) &&
			(vertice.X < (poligono[j].X - poligono[i].X) * (vertice.Y - poligono[i].Y) /
				(poligono[j].Y - poligono[i].Y) + poligono[i].X))
		{
			dentro = !dentro;
		}
	}

	return dentro;
}

void Nesting::transladarPoligono(Poligono &poligono, const size_t indice, const Vertice &destino)
{
	Vetor deslocamento = destino - poligono.vertices[indice];

	OperacoesGeometricas::transladar(poligono.vertices, deslocamento);

	for (size_t i = 0; i < poligono.buracos.size(); i++)
	{
		OperacoesGeometricas::transladar(poligono.buracos[i], deslocamento);
	}

	poligono.limitanteSuperiorDireito += deslocamento;
	poligono.limitanteInferiorEsquerdo += deslocamento;
}

void Nesting::ordenarPoligonosPorArea()
{
	std::sort(this->poligonos.rbegin(), this->poligonos.rend(),
		[] (const Poligono &poligono1, const Poligono &poligono2) -> bool
		{
			return poligono1.area < poligono2.area;
		});
}

bool Nesting::poligonoDentroDeObjeto(const Objeto &formatoObjeto, const std::vector < Vertice > &poligono)
{
	for (size_t k = 0; k < formatoObjeto.vertices.size(); k++)
	{
		bool dentro = false;

		for (size_t i = 0, j = poligono.size() - 1; i < poligono.size(); j = i++)
		{
			if (((poligono[i].Y >= formatoObjeto.vertices[k].Y) != (poligono[j].Y >= formatoObjeto.vertices[k].Y)) &&
				(formatoObjeto.vertices[k].X <= (poligono[j].X - poligono[i].X) * (formatoObjeto.vertices[k].Y - poligono[i].Y) /
				(poligono[j].Y - poligono[i].Y) + poligono[i].X))
			{
				dentro = !dentro;
			}
		}

		if (!dentro)
		{
			return false;
		}
	}

	return true;
}

void Nesting::rotacionarPoligonos(std::vector < Poligono > &poligonos,
	const size_t qtdRotacoes, const bool houveEspelhamento)
{
	long double intervalo = (2 * M_PI) / (qtdRotacoes + 1);
	Reta reta(Vertice(), Vetor(0, 0, 1));

	for (size_t i = 1; i < qtdRotacoes + 1; i++)
	{
		OperacoesGeometricas::rotacionarEmTornoDe(poligonos[i].vertices, reta, intervalo * i);

		for (size_t j = 0; j < poligonos[i].buracos.size(); j++)
		{
			OperacoesGeometricas::rotacionarEmTornoDe(poligonos[i].buracos[j], reta, intervalo * i);
		}
	}

	if (houveEspelhamento)
	{
		#pragma omp parallel for
		for (long long int i = static_cast < long long int > (qtdRotacoes + 1);
			i < static_cast < long long int > ((qtdRotacoes + 1) * 2); i++)
		{
			OperacoesGeometricas::rotacionarEmTornoDe(poligonos[i].vertices, reta, intervalo * i);

			for (size_t j = 0; j < poligonos[i].buracos.size(); j++)
			{
				OperacoesGeometricas::rotacionarEmTornoDe(poligonos[i].buracos[j], reta, intervalo * i);
			}
		}
	}
}

void Nesting::espelharPoligonos(std::vector < Poligono > &poligonos)
{
	size_t inicio = poligonos.size() / 2;

	for (size_t i = inicio; i < poligonos.size(); i++)
	{
		OperacoesGeometricas::escalar(poligonos[i].vertices, -1.0L);

		for (size_t j = 0; j < poligonos[i].buracos.size(); j++)
		{
			OperacoesGeometricas::escalar(poligonos[i].buracos[j], -1.0L);
		}
	}
}

void Nesting::ordenarEscolhas(std::vector < Poligono > &poligonos)
{
	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (poligonos.size()); i++)
	{
		Vertice &inferiorEsquerdo = poligonos[i].limitanteInferiorEsquerdo;
		Vertice &superiorDireito = poligonos[i].limitanteSuperiorDireito;

		for (size_t j = 0; j < poligonos[i].vertices.size(); j++)
		{
			Vertice &vertice = poligonos[i].vertices[j];

			if (inferiorEsquerdo.X > vertice.X)
			{
				inferiorEsquerdo.X = vertice.X;
			}

			if (inferiorEsquerdo.Y > vertice.Y)
			{
				inferiorEsquerdo.Y = vertice.Y;
			}

			if (superiorDireito.X < vertice.X)
			{
				superiorDireito.X = vertice.X;
			}

			if (superiorDireito.Y < vertice.Y)
			{
				superiorDireito.Y = vertice.Y;
			}
		}

		Vetor deslocamento = Vertice() - inferiorEsquerdo;

		inferiorEsquerdo += deslocamento;
		superiorDireito += deslocamento;

		OperacoesGeometricas::transladar(poligonos[i].vertices, deslocamento);

		for (size_t j = 0; j < poligonos[i].buracos.size(); j++)
		{
			OperacoesGeometricas::transladar(poligonos[i].buracos[j], deslocamento);
		}

		// TODO: MELHORAR
		/*poligonos[i].distanciaDoCentroDeMassa =
			OperacoesGeometricas::norma(OperacoesGeometricas::centroDeMassa(poligonos[i].vertices) - Vertice());*/
	}

	std::sort(poligonos.begin(), poligonos.end(),
		[](const Poligono &poligono1, const Poligono &poligono2) -> bool
		{
			return poligono1.limitanteSuperiorDireito.X * poligono1.limitanteSuperiorDireito.Y <
				poligono2.limitanteSuperiorDireito.X * poligono2.limitanteSuperiorDireito.Y ||
					
				poligono1.limitanteSuperiorDireito.X * poligono1.limitanteSuperiorDireito.Y ==
				poligono2.limitanteSuperiorDireito.X * poligono2.limitanteSuperiorDireito.Y/* &&
				poligono1.distanciaDoCentroDeMassa < poligono2.distanciaDoCentroDeMassa*/;
		});
}

void Nesting::criarSegmentos(std::vector < Poligono > &poligonos)
{
	size_t qtdVertices = poligonos[0].vertices.size();

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (poligonos.size()); i++)
	{
		for (size_t j = 0 ; j < poligonos[i].segmentos.size(); j++)
		{
			// TODO: SEGMENTO COM MARCA!
			poligonos[i].segmentos[j] = Segmento(poligonos[i].vertices[j], poligonos[i].vertices[(j + 1) % qtdVertices]);
		}
	}
}

void Nesting::adicionarPoligono(const std::vector < Vertice > &poligono, const std::vector < std::vector < Vertice > > &buracos)
{
	for (size_t i = 0; i < poligono.size(); i++)
	{
		if (poligono[i].Z != 0)
		{
			Excecao::lancar("O polígono não está planificado em Z = 0.");
		}
	}

	for (size_t i = 0; i < buracos.size(); i++)
	{
		for (size_t j = 0; j < buracos[i].size(); j++)
		{
			if (buracos[i][j].Z != 0)
			{
				std::string excecao("O buraco ");
				excecao.append(std::to_string(i));
				excecao.append(" não está planificado em Z = 0.");

				Excecao::lancar(excecao);
			}
		}
	}

	if (!OperacoesGeometricas::poligonoSimples(poligono))
	{
		Excecao::lancar("O polígono é complexo.");
	}

	this->poligonos.push_back(Poligono(poligono, buracos));
	this->poligonos.back().segmentos.resize(this->poligonos.back().vertices.size());
}

bool Nesting::grid(const long double alturaObjeto, const long double larguraObjeto,
	const std::vector < Poligono > &poligonosEstaticos, Poligono &poligonoCandidato, long double &menorX)
{
	Vetor movimentacao = Vertice() - poligonoCandidato.limitanteInferiorEsquerdo;

	Nesting::transladarPoligono(poligonoCandidato, movimentacao);

	long double largura = larguraObjeto - poligonoCandidato.limitanteSuperiorDireito.X;
	long double altura = alturaObjeto - poligonoCandidato.limitanteSuperiorDireito.Y;

	if (largura < 0 || altura < 0)
	{
		return false;
	}

	for (size_t i = 0; i < LARGURA; i++)
	{
		for (size_t j = 0; j < ALTURA; j++)
		{
			bool encaixado = true;
			
			for (size_t k = 0; k < poligonosEstaticos.size(); k++)
			{
				if (Nesting::existeInterseccao(poligonosEstaticos[k], poligonoCandidato) ||
					Nesting::verticeDentroDePoligono(poligonoCandidato.vertices[0], poligonosEstaticos[k].vertices))
				{
					encaixado = false;

					break;
				}
			}

			if (encaixado)
			{
				menorX = poligonoCandidato.limitanteSuperiorDireito.X;

				return true;
			}

			Nesting::transladarPoligono(poligonoCandidato, Vetor(0.0L, altura * (1.0L / ALTURA)));
		}

		Nesting::transladarPoligono(poligonoCandidato, Vetor(0.0L, -altura));
		Nesting::transladarPoligono(poligonoCandidato, Vetor(largura * (1.0L / LARGURA)));
	}

	return false;
}

std::array < size_t, 2 > Nesting::posicionarPoligonos(std::vector < Objeto > &objetos,
	std::vector < Poligono > &poligonos)
{
	std::array < size_t, 2 > melhorEscolha = { objetos.size(), poligonos.size() };

	for (size_t i = 0; i < objetos.size(); i++)
	{
		if (objetos[i].areaRestante < poligonos[0].area)
		{
			continue;
		}

		struct Resultado
		{
			bool sucesso;
			size_t posicao;
			long double menorX;
		};

		std::vector < Resultado > encaixado(poligonos.size());

		#pragma omp parallel for
		for (long long int j = 0; j < static_cast < long long int > (poligonos.size()); j++)
		{
			encaixado[j].posicao = j;
			encaixado[j].sucesso = Nesting::grid(objetos[i].limitanteSuperiorDireito.Y, objetos[i].limitanteSuperiorDireito.X,
				objetos[i].poligonos, poligonos[j], encaixado[j].menorX);
		}

		std::stable_sort(encaixado.begin(), encaixado.end(),
			[](const Resultado &resultado1, const Resultado &resultado2) -> bool
			{
				return resultado1.menorX < resultado2.menorX;
			});

		auto posicaoEncaixe = std::find_if(encaixado.begin(), encaixado.end(),
			[](const Resultado &resultado) -> bool
			{
				return resultado.sucesso;
			});

		if (posicaoEncaixe != encaixado.end())
		{
			objetos[i].areaRestante -= poligonos[0].area;

			melhorEscolha[0] = i;
			melhorEscolha[1] = (*posicaoEncaixe).posicao;

			break;
		}
	}

	return melhorEscolha;
}

void Nesting::rodar(const size_t qtdRotacoes, const bool permitirEspelhamento)
{
	this->objetos.clear();
	this->ordenarPoligonosPorArea();

	for (size_t i = 0; i < this->poligonos.size(); i++)
	{
		size_t qtdEscolhas = (1 + qtdRotacoes) * (permitirEspelhamento ? 2 : 1);
		std::vector < Poligono > possiveisEscolhas(qtdEscolhas, Poligono(this->poligonos[i]));

		if (permitirEspelhamento)
		{
			Nesting::espelharPoligonos(possiveisEscolhas);
		}

		Nesting::rotacionarPoligonos(possiveisEscolhas, qtdRotacoes, permitirEspelhamento);
		Nesting::ordenarEscolhas(possiveisEscolhas);
		Nesting::criarSegmentos(possiveisEscolhas);

		std::array < size_t, 2 > melhorEscolha;

		bool novoObjeto = false;

		while ((melhorEscolha = Nesting::posicionarPoligonos(this->objetos, possiveisEscolhas))[1] == qtdEscolhas)
		{
			if (novoObjeto)
			{
				Excecao::lancar("Não foi possível encaixar um dos polígonos.");
			}

			this->objetos.push_back(this->formatoObjeto);

			novoObjeto = true;
		}

		this->objetos[melhorEscolha[0]].poligonos.push_back(possiveisEscolhas[melhorEscolha[1]]);
	}
}

std::vector < cv::Mat > Nesting::obterImagem(const long double ampliacao) const
{
	if (ampliacao <= 0)
	{
		Excecao::lancar("Ampliação nula ou negativa.");
	}

	int altura = static_cast < int > (std::ceil(this->altura * ampliacao));
	int largura = static_cast < int > (std::ceil(this->largura * ampliacao));
	Objeto3D objeto = this->obterObjeto();
	std::vector < cv::Mat > imagens(objeto.obterQtdGrupos() / 2, cv::Mat(altura, largura, CV_8UC1));

	objeto.triangularizarFaces();

	for (size_t i = 0; i < objeto.obterQtdGrupos() / 2; i++)
	{
		#pragma omp parallel for
		for (long long int j = 0; j < static_cast < long long int > (objeto.obterQtdVertices(i)); j++)
		{
			objeto.obterVertice(i, j).X -= i * this->largura;
		}

		OperacoesGeometricas::escalar(objeto.obterVertice(i), ampliacao, ampliacao);

		#pragma omp parallel for
		for (int j = 0; j < altura; j++)
		{
			for (int k = 0; k < largura; k++)
			{
				bool contem = false;
				Vertice vertice(k + 0.5, j + 0.5);

				for (size_t l = 0; l < objeto.obterQtdFaces(i); l++)
				{
					Triangulo triangulo;

					objeto.obterFace(i, l, triangulo);

					if ((contem = OperacoesGeometricas::contem(triangulo, vertice)))
					{
						break;
					}
				}

				imagens[i].at < uchar >(j, k) = (contem ? 255 : 0);
			}
		}
	}

	size_t deslocamentoObjetos = objeto.obterQtdGrupos() / 2;

	for (size_t i = deslocamentoObjetos; i < objeto.obterQtdGrupos(); i++)
	{
		#pragma omp parallel for
		for (long long int j = 0; j < static_cast < long long int > (objeto.obterQtdVertices(i)); j++)
		{
			objeto.obterVertice(i, j).X -= (i - deslocamentoObjetos) * this->largura;
		}

		OperacoesGeometricas::escalar(objeto.obterVertice(i), ampliacao, ampliacao);

		#pragma omp parallel for
		for (int j = 0; j < altura; j++)
		{
			for (int k = 0; k < largura; k++)
			{
				bool contem = false;
				Vertice vertice(k + 0.5, j + 0.5, 1);

				for (size_t l = 0; l < objeto.obterQtdFaces(i); l++)
				{
					Triangulo triangulo;

					objeto.obterFace(i, l, triangulo);

					if ((contem = OperacoesGeometricas::contem(triangulo, vertice)))
					{
						break;
					}
				}

				if (contem)
				{
					imagens[i - deslocamentoObjetos].at < uchar >(j, k) = 0;
				}
			}
		}
	}

	return imagens;
}

Objeto3D Nesting::obterObjeto() const
{
	Objeto3D objeto;

	for (size_t i = 0; i < this->objetos.size(); i++)
	{
		long double deslocamentoVertice = i * this->largura;

		objeto.adicionarGrupo();

		for (size_t j = 0; j < this->objetos[i].poligonos.size(); j++)
		{
			size_t deslocamentoFace = objeto.obterQtdVertices(i);
			std::vector < size_t > face;

			for (size_t k = 0; k < this->objetos[i].poligonos[j].vertices.size(); k++)
			{
				Vertice vertice = this->objetos[i].poligonos[j].vertices[k];

				vertice.X += deslocamentoVertice;
				objeto.adicionarVertice(i, vertice);

				face.push_back(k + deslocamentoFace);
			}

			objeto.adicionarFace(i, face, false);
		}
	}

	size_t deslocamentoObjetos = objeto.obterQtdGrupos();

	for (size_t i = 0; i < this->objetos.size(); i++)
	{
		long double deslocamentoVertice = i * this->largura;

		objeto.adicionarGrupo();

		for (size_t j = 0; j < this->objetos[i].poligonos.size(); j++)
		{
			for (size_t k = 0; k < this->objetos[i].poligonos[j].buracos.size(); k++)
			{
				size_t deslocamentoFace = objeto.obterQtdVertices(i + deslocamentoObjetos);
				std::vector < size_t > face;

				for (size_t l = 0; l < this->objetos[i].poligonos[j].buracos[k].size(); l++)
				{
					Vertice vertice = this->objetos[i].poligonos[j].buracos[k][l];

					vertice.X += deslocamentoVertice;
					vertice.Z = 1;

					objeto.adicionarVertice(i + deslocamentoObjetos, vertice);

					face.push_back(l + deslocamentoFace);
				}

				objeto.adicionarFace(i + deslocamentoObjetos, face, false);
			}
		}
	}

	return objeto;
}

Nesting::Nesting(const std::vector < Vertice > &bordasLimitantes) :
	formatoObjeto(bordasLimitantes)
{
	if (!OperacoesGeometricas::poligonoSimples(bordasLimitantes))
	{
		Excecao::lancar("As bordas limitantes do objeto formam um polígono complexo.");
	}

	Vertice &inferiorEsquerdo = this->formatoObjeto.limitanteInferiorEsquerdo;
	Vertice &superiorDireito = this->formatoObjeto.limitanteSuperiorDireito;

	for (size_t i = 0; i < this->formatoObjeto.vertices.size(); i++)
	{
		Vertice &vertice = this->formatoObjeto.vertices[i];

		if (inferiorEsquerdo.X > vertice.X)
		{
			inferiorEsquerdo.X = vertice.X;
		}

		if (inferiorEsquerdo.Y > vertice.Y)
		{
			inferiorEsquerdo.Y = vertice.Y;
		}

		if (superiorDireito.X < vertice.X)
		{
			superiorDireito.X = vertice.X;
		}

		if (superiorDireito.Y < vertice.Y)
		{
			superiorDireito.Y = vertice.Y;
		}
	}

	Vetor deslocamento = -(inferiorEsquerdo - Vertice());

	inferiorEsquerdo += deslocamento;
	superiorDireito += deslocamento;

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (this->formatoObjeto.vertices.size()); i++)
	{
		this->formatoObjeto.vertices[i] += deslocamento;
	}

	long long int qtdVertices = static_cast < long long int > (this->formatoObjeto.vertices.size());

	this->formatoObjeto.segmentos.resize(qtdVertices);

	#pragma omp parallel for
	for (long long int i = 0; i < qtdVertices; i++)
	{
		this->formatoObjeto.segmentos[i] = Segmento(this->formatoObjeto.vertices[i], this->formatoObjeto.vertices[(i + 1) % qtdVertices]);
	}

	this->altura = superiorDireito.Y;
	this->largura = superiorDireito.X;
}

Nesting::~Nesting()
{
}
