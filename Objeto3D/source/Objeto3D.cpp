#include "../include/Objeto3D.h"
#include "../../Excecao/include/Excecao.h"
#include "../../Geometria/include/OperacoesGeometricas.h"

size_t Objeto3D::obterQtdGrupos() const noexcept
{
	return this->nomeGrupo.size();
}

size_t Objeto3D::obterQtdVertices(const std::string &nomeGrupo) const
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->obterQtdVertices(i);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

size_t Objeto3D::obterQtdVertices(const size_t idGrupo) const
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice fora dos limites.");
	}

	return this->objeto[idGrupo].size();
}

size_t Objeto3D::obterQtdFaces(const std::string &nomeGrupo) const
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->obterQtdFaces(i);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

size_t Objeto3D::obterQtdFaces(const size_t idGrupo) const
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice fora dos limites.");
	}

	return this->face[idGrupo].size();
}

std::string Objeto3D::obterNomeObjeto() const noexcept
{
	return this->nomeObjeto;
}

std::string Objeto3D::obterNomeGrupo(const size_t idGrupo) const
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice fora dos limites.");
	}

	return this->nomeGrupo[idGrupo];
}

size_t Objeto3D::deslocamentoIndiceFace(const size_t idGrupo) const
{
	size_t deslocamento = 0;

	for (size_t i = 0; i < idGrupo; i++)
	{
		deslocamento += this->obterQtdVertices(i);
	}

	return deslocamento;
}

bool Objeto3D::existeTextura(const size_t idGrupo) const noexcept
{
	if (idGrupo < this->obterQtdGrupos() && this->textura[idGrupo])
	{
		return true;
	}

	return false;
}

cv::Mat Objeto3D::obterTextura(const size_t idGrupo) const
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice fora dos limites.");
	}

	if (!(this->textura[idGrupo]))
	{
		Excecao::lancar("Este grupo não possui uma textura válida.");
	}

	return *(this->textura[idGrupo]);
}

std::vector < std::vector < Vertice > > &Objeto3D::obterVertice() noexcept
{
	return this->objeto;
}

std::vector < Vertice > &Objeto3D::obterVertice(const size_t idGrupo)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo fora dos limites.");
	}

	return this->objeto[idGrupo];
}

Vertice &Objeto3D::obterVertice(const size_t idGrupo, size_t idVertice)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo fora dos limites.");
	}

	if (idVertice >= this->obterQtdVertices(idGrupo))
	{
		Excecao::lancar("Índice de vértice fora dos limites.");
	}

	return this->objeto[idGrupo][idVertice];
}

void Objeto3D::obterFace(const size_t idGrupo, const size_t idFace, Triangulo &triangulo) const
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo fora dos limites.");
	}

	if (idFace >= this->obterQtdFaces(idGrupo))
	{
		Excecao::lancar("Índice de face fora dos limites.");
	}

	if (this->face[idGrupo][idFace].size() != 3)
	{
		Excecao::lancar("Esta face não foi triangularizada.");
	}

	size_t idVertice1 = this->face[idGrupo][idFace][0];
	size_t idVertice2 = this->face[idGrupo][idFace][1];
	size_t idVertice3 = this->face[idGrupo][idFace][2];

	triangulo = Triangulo(
		this->objeto[idGrupo][idVertice1],
		this->objeto[idGrupo][idVertice2],
		this->objeto[idGrupo][idVertice3]);
}

void Objeto3D::obterFace(const size_t idGrupo, const size_t idFace, std::vector < size_t > &indicesVertices) const
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo fora dos limites.");
	}

	if (idFace >= this->obterQtdFaces(idGrupo))
	{
		Excecao::lancar("Índice de face fora dos limites.");
	}

	indicesVertices.resize(this->face[idGrupo][idFace].size());

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (this->face[idGrupo][idFace].size()); i++)
	{
		indicesVertices[i] = this->face[idGrupo][idFace][i];
	}
}

size_t Objeto3D::adicionarGrupo(std::string nomeGrupo)
{
	if (nomeGrupo.length() == 0)
	{
		nomeGrupo = "Grupo";
		nomeGrupo.append(std::to_string(this->obterQtdGrupos() + 1));
	}

	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			Excecao::lancar(std::string("Já existe um grupo com o nome: ").append(nomeGrupo));
		}
	}

	this->nomeGrupo.push_back(nomeGrupo);
	this->objeto.push_back(std::vector < Vertice >());
	this->mapeamentoTextura.push_back(std::vector < Mapeamento2D >());
	this->face.push_back(std::vector < std::vector < size_t > >());
	this->textura.push_back(nullptr);

	return this->obterQtdGrupos() - 1;
}

size_t Objeto3D::adicionarVertice(const std::string &nomeGrupo, const Vertice &vertice)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->adicionarVertice(i, vertice);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

size_t Objeto3D::adicionarVertice(const size_t idGrupo, const Vertice &vertice)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	this->objeto[idGrupo].push_back(vertice);
	this->mapeamentoTextura[idGrupo].push_back(Mapeamento2D());

	return this->obterQtdVertices(idGrupo) - 1;
}

std::array < size_t, 2 > Objeto3D::adicionarVertice(const std::string &nomeGrupo, const std::vector < Vertice > &vertices,
	const bool adicaoFace, const bool facePlanoUnico)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->adicionarVertice(i, vertices, adicaoFace, facePlanoUnico);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

std::array < size_t, 2 > Objeto3D::adicionarVertice(const size_t idGrupo, const std::vector < Vertice > &vertices,
	const bool adicaoFace, const bool facePlanoUnico)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	std::array < size_t, 2 > indices; // [0] --> início de idVertice; [1] --> início de idFace.
	std::vector < Mapeamento2D > mapeamento(vertices.size());

	indices[0] = this->obterQtdVertices(idGrupo);

	this->objeto[idGrupo].insert(this->objeto[idGrupo].end(), vertices.begin(), vertices.end());
	this->mapeamentoTextura[idGrupo].insert(
		this->mapeamentoTextura[idGrupo].end(), mapeamento.begin(), mapeamento.end());

	if (adicaoFace)
	{
		std::vector < size_t > indiceFace(vertices.size());

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (indiceFace.size()); i++)
		{
			indiceFace[i] = indices[0] + i;
		}

		indices[1] = this->adicionarFace(idGrupo, indiceFace, facePlanoUnico);
	}

	else
	{
		indices[1] = SIZE_MAX;
	}

	return indices;
}

size_t Objeto3D::adicionarFace(const std::string &nomeGrupo, const std::vector < size_t > &vertices, const bool facePlanoUnico)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->adicionarFace(i, vertices, facePlanoUnico);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

size_t Objeto3D::adicionarFace(const size_t idGrupo, const std::vector < size_t > &vertices, const bool facePlanoUnico)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	if (vertices.size() < 3)
	{
		Excecao::lancar("Face com menos de três vértices.");
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (vertices[i] >= this->obterQtdVertices(idGrupo))
		{
			std::string excecao("Índice do vértice ");
			excecao.append(std::to_string(i));
			excecao.append(" maior ou igual à quantidade de vértices existentes no grupo.");

			Excecao::lancar(excecao);
		}
	}

	for (size_t i = 0; i < vertices.size(); i++)
	{
		for (size_t j = i + 1; j < vertices.size(); j++)
		{
			if (vertices[i] == vertices[j])
			{
				Excecao::lancar("Existem vértices com índices iguais.");
			}
		}
	}

	if (facePlanoUnico)
	{
		std::vector < Vertice > verticesPoligono(vertices.size());

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
		{
			verticesPoligono[i] = this->objeto[idGrupo][vertices[i]];
		}

		if (!OperacoesGeometricas::planoContido(verticesPoligono, 1.0e-3))
		{
			Excecao::lancar("A face não está contida em um único plano e isso pode degenerar a figura.");
		}
	}

	// TODO: REMOVER O TRECHO DE CÓDIGO PARA O ACADIA.
	// Adaptado de OperacoesGeometricas::poligonoSimples.
	//for (long long int i = 0; i < static_cast < long long int > (vertices.size()); i++)
	//{
	//	long long int anterior = i - 1;
	//	long long int inicioSegmento = i;
	//	long long int fimSegmento = (i + 1) % vertices.size();
	//	long long int posterior = (i + 2) % vertices.size();

	//	if (anterior < 0)
	//	{
	//		anterior += vertices.size();
	//	}

	//	Segmento segmentoAnterior(this->objeto[idGrupo][vertices[anterior]], this->objeto[idGrupo][vertices[inicioSegmento]]);
	//	Segmento segmentoAnalisado(this->objeto[idGrupo][vertices[inicioSegmento]], this->objeto[idGrupo][vertices[fimSegmento]]);
	//	Segmento segmentoPosterior(this->objeto[idGrupo][vertices[fimSegmento]], this->objeto[idGrupo][vertices[posterior]]);

	//	// Verificação de sobreposição de arestas vizinhas.
	//	if (OperacoesGeometricas::linearmenteIndependentes(segmentoAnterior.obterVetor(), segmentoAnalisado.obterVetor()) == false &&
	//		!OperacoesGeometricas::mesmoOctante(segmentoAnterior.obterVetor(), segmentoAnalisado.obterVetor()) ||

	//		OperacoesGeometricas::linearmenteIndependentes(segmentoAnalisado.obterVetor(), segmentoPosterior.obterVetor()) == false &&
	//		!OperacoesGeometricas::mesmoOctante(segmentoAnalisado.obterVetor(), segmentoPosterior.obterVetor()))
	//	{
	//		Excecao::lancar("A face não pode ser um polígono complexo.");
	//	}

	//	// Verificação de cruzamento em arestas não vizinhas.
	//	for (size_t j = 0; j < vertices.size(); j++)
	//	{
	//		if (j == anterior || j == inicioSegmento || j == fimSegmento)
	//		{
	//			continue;
	//		}

	//		Segmento segmentoAux(this->objeto[idGrupo][vertices[j]], this->objeto[idGrupo][vertices[(j + 1) % vertices.size()]]);

	//		Vertice interseccao;

	//		if (OperacoesGeometricas::interseccao(segmentoAnalisado, segmentoAux, &interseccao))
	//		{
	//			std::cout << interseccao.X << " " << interseccao.Y << " " << interseccao.Z << std::endl;

	//			Excecao::lancar("A face não pode ser um polígono complexo.");
	//		}
	//	}
	//}

	this->face[idGrupo].push_back(std::vector < size_t >(vertices));

	size_t idFace = this->obterQtdFaces(idGrupo) - 1;

	return idFace;
}

size_t Objeto3D::adicionarFace(const std::string &nomeGrupo,
	const size_t idVertice1, const size_t idVertice2, const size_t idVertice3)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->adicionarFace(i, idVertice1, idVertice2, idVertice3);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

size_t Objeto3D::adicionarFace(const size_t idGrupo,
	const size_t idVertice1, const size_t idVertice2, const size_t idVertice3)
{
	std::vector < size_t > vertices = { idVertice1, idVertice2, idVertice3 };

	return this->adicionarFace(idGrupo, vertices, false);
}

void Objeto3D::definirMapeamentoTextura(const std::string &nomeGrupo, const size_t idVertice, const long double Y, const long double X)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->definirMapeamentoTextura(i, idVertice, Y, X);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::definirMapeamentoTextura(const size_t idGrupo, const size_t idVertice, const long double Y, const long double X)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	if (idVertice >= this->obterQtdVertices(idGrupo))
	{
		Excecao::lancar("Índice de vértice maior ou igual à quantidade de vértices existentes neste grupo.");
	}

	this->mapeamentoTextura[idGrupo][idVertice].X = X;
	this->mapeamentoTextura[idGrupo][idVertice].Y = Y;
}

void Objeto3D::definirTextura(const std::string &nomeGrupo, const cv::Mat &textura)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			this->definirTextura(i, textura);

			return;
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::definirTextura(const size_t idGrupo, const cv::Mat &textura)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	if (this->textura[idGrupo])
	{
		delete this->textura[idGrupo];
	}

	this->textura[idGrupo] = new cv::Mat(textura);
}

void Objeto3D::deslocarVertice(const std::string &nomeGrupo, const size_t idVertice, const Vertice &novaPosicao)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->deslocarVertice(i, idVertice, novaPosicao);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::deslocarVertice(const size_t idGrupo, const size_t idVertice, const Vertice &novaPosicao)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	this->objeto[idGrupo][idVertice] = novaPosicao;
}

void Objeto3D::deslocarVertice(const std::string &nomeGrupo,
	const size_t idVertice, const long double X, const long double Y, const long double Z)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->deslocarVertice(i, idVertice, X, Y, Z);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::deslocarVertice(const size_t idGrupo,
	const size_t idVertice, const long double X, const long double Y, const long double Z)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	this->objeto[idGrupo][idVertice].X += X;
	this->objeto[idGrupo][idVertice].Y += Y;
	this->objeto[idGrupo][idVertice].Z += Z;
}

void Objeto3D::deslocarVertice(const std::string &nomeGrupo, const size_t idVertice, const Vetor &deslocamento)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			return this->deslocarVertice(i, idVertice, deslocamento);
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::deslocarVertice(const size_t idGrupo, const size_t idVertice, const Vetor &deslocamento)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	this->objeto[idGrupo][idVertice] += deslocamento;
}

void Objeto3D::removerGrupo(const std::string &nomeGrupo)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			this->removerGrupo(i);

			return;
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::removerGrupo(const size_t idGrupo)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	this->removerTextura(idGrupo);
	this->textura.erase(this->textura.begin() + idGrupo);

	this->nomeGrupo.erase(this->nomeGrupo.begin() + idGrupo);
	this->objeto.erase(this->objeto.begin() + idGrupo);
	this->face.erase(this->face.begin() + idGrupo);
}

void Objeto3D::removerVertice(const std::string &nomeGrupo, const size_t idVertice)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			this->removerVertice(i, idVertice);

			return;
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::removerVertice(const size_t idGrupo, const size_t idVertice)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	if (idVertice >= this->obterQtdVertices(idGrupo))
	{
		Excecao::lancar("Índice de vértice maior ou igual à quantidade de vértices existentes neste grupo.");
	}

	this->objeto[idGrupo].erase(this->objeto[idGrupo].begin() + idVertice);
	this->mapeamentoTextura[idGrupo].erase(this->mapeamentoTextura[idGrupo].begin() + idVertice);

	for (size_t i = 0; i < this->obterQtdFaces(idGrupo); i++)
	{
		for (size_t j = 0; j < this->face[idGrupo][i].size(); j++)
		{
			if (this->face[idGrupo][i][j] == idVertice)
			{
				this->removerFace(idGrupo, i);

				i--;

				break;
			}
		}
	}
}

void Objeto3D::removerFace(const std::string &nomeGrupo, const size_t idFace)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			this->removerFace(i, idFace);

			return;
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::removerFace(const size_t idGrupo, const size_t idFace)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	if (idFace >= this->obterQtdFaces(idGrupo))
	{
		Excecao::lancar("Índice de face maior ou igual à quantidade de faces existentes neste grupo.");
	}

	this->face[idGrupo].erase(this->face[idGrupo].begin() + idFace);
}

void Objeto3D::removerTextura(const std::string &nomeGrupo)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->nomeGrupo[i] == nomeGrupo)
		{
			this->removerTextura(i);

			return;
		}
	}

	Excecao::lancar("Não há um grupo com este nome.");
}

void Objeto3D::removerTextura(const size_t idGrupo)
{
	if (idGrupo >= this->obterQtdGrupos())
	{
		Excecao::lancar("Índice de grupo maior ou igual à quantidade de grupos existentes.");
	}

	if (this->textura[idGrupo])
	{
		delete this->textura[idGrupo];
	}

	this->textura[idGrupo] = nullptr;
}

void Objeto3D::salvarVertices(Arquivo &arquivo, const size_t idGrupo) const
{
	for (size_t i = 0; i < this->obterQtdVertices(idGrupo); i++)
	{
		std::string aux = "v  ";
		aux.append(std::to_string(this->objeto[idGrupo][i].X));
		aux.append(" ");
		aux.append(std::to_string(this->objeto[idGrupo][i].Y));
		aux.append(" ");
		aux.append(std::to_string(this->objeto[idGrupo][i].Z));
		aux.append("\n");

		arquivo.escrever(aux, false);
	}

	arquivo.escrever('\n');
}

void Objeto3D::salvarMaterial(Arquivo &arquivo, const size_t idGrupo) const
{
	std::string aux = this->nomeGrupo[idGrupo];
	aux.append(".mtl");

	aux = this->nomeGrupo[idGrupo];
	aux.append("_textura.png");

	arquivo.escrever("newmtl ", false);
	arquivo.escrever(this->nomeGrupo[idGrupo], false);
	arquivo.escrever('\n');
	arquivo.escrever("\tKd 1.000 1.000 1.000\n", false);
	arquivo.escrever("\tillum 2\n", false);
	arquivo.escrever("\td 1.0\n\n", false);

	arquivo.escrever("map_Kd ", false);
	arquivo.escrever(aux, false);
	arquivo.escrever('\n');

	if (this->textura[idGrupo])
	{
		cv::imwrite(aux, *(this->textura[idGrupo]));
	}

	else
	{
		cv::Mat imagemPreta(1, 1, CV_8UC1);

		cv::imwrite(aux, imagemPreta);
	}

	arquivo.escrever('\n');
}

void Objeto3D::salvarTextura(Arquivo &arquivo, const size_t idGrupo) const
{
	for (size_t i = 0; i < this->obterQtdVertices(idGrupo); i++)
	{
		std::string aux = "vt ";
		aux.append(std::to_string(this->mapeamentoTextura[idGrupo][i].X));
		aux.append(" ");
		aux.append(std::to_string(this->mapeamentoTextura[idGrupo][i].Y));
		aux.append("\n");

		arquivo.escrever(aux, false);
	}

	arquivo.escrever('\n');
}

void Objeto3D::salvarFaces(Arquivo &arquivo, const bool material) const
{
	std::string aux = "o ";
	aux.append(this->nomeObjeto);
	aux.append("\n");

	arquivo.escrever(aux, false);

	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->obterQtdVertices(i))
		{
			aux = "\tg ";
			aux.append(this->nomeGrupo[i]);
			aux.append("\n");

			arquivo.escrever(aux, false);

			if (material)
			{
				aux = "\tusemtl ";
				aux.append(this->nomeGrupo[i]);
				aux.append("\n");

				arquivo.escrever(aux, false);
			}

			size_t deslocamento = this->deslocamentoIndiceFace(i) + 1;

			for (size_t j = 0; j < this->obterQtdFaces(i); j++)
			{
				aux = "\tf  ";

				for (size_t k = 0; k < this->face[i][j].size(); k++)
				{
					size_t idFace = this->face[i][j][k] + deslocamento;

					aux.append(std::to_string(idFace));

					if (material)
					{
						aux.append("/");
						aux.append(std::to_string(idFace));
					}

					aux.append(" ");
				}

				aux.append("\n");

				arquivo.escrever(aux, false);
			}

			arquivo.escrever('\n');
		}

		arquivo.escrever('\n');
	}
}

void Objeto3D::salvar(const bool faces, const bool material) const
{
	std::string aux = this->nomeObjeto;

	Arquivo arquivoObj(aux.append(".obj"), 35 | std::ios_base::trunc);
	Arquivo arquivoMtl;

	if (material)
	{
		aux = this->nomeObjeto;

		arquivoMtl.abrir(aux.append(".mtl"), 35 | std::ios_base::trunc);

		aux = "mtllib ";
		aux.append(this->nomeObjeto);
		aux.append(".mtl\n\n");

		arquivoObj.escrever(aux, false);
	}

	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->obterQtdVertices(i))
		{
			aux = "#";
			aux.append(this->nomeGrupo[i]);
			aux.append("\n");

			arquivoObj.escrever(aux, false);

			this->salvarVertices(arquivoObj, i);

			if (material)
			{
				this->salvarMaterial(arquivoMtl, i);
				this->salvarTextura(arquivoObj, i);
			}

			arquivoObj.escrever('\n');
		}
	}

	if (faces)
	{
		this->salvarFaces(arquivoObj, material);
	}
}

void Objeto3D::serializar(Arquivo &arquivo) const
{
	arquivo.escrever(this->nomeObjeto);

	arquivo.escrever(this->nomeGrupo.size());

	for (size_t i = 0; i < this->nomeGrupo.size(); i++)
	{
		arquivo.escrever(this->nomeGrupo[i]);
	}

	arquivo.escrever(this->objeto);
	arquivo.escrever(this->mapeamentoTextura);
	arquivo.escrever(this->face);

	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->textura[i])
		{
			arquivo.escrever(true);

			arquivo.escrever(this->textura[i]->rows);
			arquivo.escrever(this->textura[i]->cols);
			arquivo.escrever(this->textura[i]->type());

			size_t qtdBytes = this->textura[i]->dataend - this->textura[i]->datastart;

			arquivo.escrever(qtdBytes);
			arquivo.escrever(static_cast < void * > (this->textura[i]->data), qtdBytes);
		}

		else
		{
			arquivo.escrever(false);
		}
	}
}

void Objeto3D::deserializar(Arquivo &arquivo)
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->textura[i])
		{
			delete this->textura[i];
		}
	}

	arquivo.ler(this->nomeObjeto);

	size_t qtdGrupos;

	arquivo.ler(qtdGrupos);

	this->nomeGrupo.resize(qtdGrupos);
	this->textura.resize(qtdGrupos);

	for (size_t i = 0; i < qtdGrupos; i++)
	{
		arquivo.ler(this->nomeGrupo[i]);
	}

	arquivo.ler(this->objeto);
	arquivo.ler(this->mapeamentoTextura);
	arquivo.ler(this->face);

	for (size_t i = 0; i < qtdGrupos; i++)
	{
		bool existeTextura;

		arquivo.ler(existeTextura);

		if (existeTextura)
		{
			int linhas;
			int colunas;
			int tipo;
			size_t qtdBytes;

			arquivo.ler(linhas);
			arquivo.ler(colunas);
			arquivo.ler(tipo);
			arquivo.ler(qtdBytes);

			this->textura[i] = new cv::Mat(linhas, colunas, tipo);

			arquivo.ler(static_cast < void * > (this->textura[i]->data), qtdBytes);
		}

		else
		{
			this->textura[i] = nullptr;
		}
	}
}

std::vector < bool > Objeto3D::verticesConvexos(const size_t idGrupo,
	const std::vector < size_t > &poligono) const
{
	if (poligono.size() < 3)
	{
		return std::vector < bool >(poligono.size(), true);
	}

	std::vector < bool > marcadorVerticesConvexos(poligono.size());
	std::vector < Vertice > _poligono(poligono.size());
	std::vector < IndiceVertice > mapeamento(poligono.size());

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (poligono.size()); i++)
	{
		Vertice vertice = this->objeto[idGrupo][poligono[i]];

		mapeamento[i] = IndiceVertice(poligono[i], vertice);
		_poligono[i] = vertice;
	}

	std::vector < Vertice > _poligonoConvexo = OperacoesGeometricas::convexHull2D(_poligono);
	std::vector < size_t > poligonoConvexo(_poligonoConvexo.size());

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (poligonoConvexo.size()); i++)
	{
		for (size_t j = 0; j < mapeamento.size(); j++)
		{
			if (_poligonoConvexo[i] == mapeamento[j])
			{
				poligonoConvexo[i] = mapeamento[j].indice;

				break;
			}
		}
	}

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

			Vetor vetor1 = this->objeto[idGrupo][poligono[i]] - this->objeto[idGrupo][poligono[anterior]];
			Vetor vetor2 = this->objeto[idGrupo][poligono[posterior]] - this->objeto[idGrupo][poligono[i]];

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

				Vetor _vetor1 = this->objeto[idGrupo][poligono[j]] - this->objeto[idGrupo][poligono[_anterior]];
				Vetor _vetor2 = this->objeto[idGrupo][poligono[_posterior]] - this->objeto[idGrupo][poligono[j]];

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

bool Objeto3D::interseccao(const std::vector < Vertice > &poligono, const Segmento &segmento, const long double erro)
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

void Objeto3D::triangularizarFaces()
{
	const long double erro = 1.0e-3;

	for (size_t i = 0; i < this->face.size(); i++)
	{
		for (size_t j = 0; j < this->face[i].size(); j++)
		{
			if (this->face[i][j].size() != 3)
			{
				std::vector < Vertice > _poligono(this->face[i][j].size());
				std::vector < Vertice > poligonoOriginal(this->face[i][j].size());
				std::vector < size_t > indices(_poligono.size());

				#pragma omp parallel for
				for (long long int k = 0; k < static_cast < long long int > (_poligono.size()); k++)
				{
					_poligono[k] = this->objeto[i][this->face[i][j][k]];
					poligonoOriginal[k] = this->objeto[i][this->face[i][j][k]];
					indices[k] = this->face[i][j][k];
				}

				if (OperacoesGeometricas::poligonoSimples(_poligono, true, erro) == false)
				{
					Excecao::lancar("O polígono é complexo.");
				}

				std::vector < std::vector < size_t > > triangulos;

				while (_poligono.size() >= 3)
				{
					std::vector < Vertice > poligonoConvexo;

					std::vector < bool > verticesConvexos =
						OperacoesGeometricas::verticesConvexos(_poligono, poligonoConvexo, erro);

					for (long long int k = 0; k < static_cast < long long int > (_poligono.size()) && _poligono.size() >= 3; k++)
					{
						if (verticesConvexos[k])
						{
							long long int anterior = k - 1;
							long long int posterior = (k + 1) % _poligono.size();

							if (anterior < 0)
							{
								anterior += _poligono.size();
							}

							Segmento segmento(_poligono[anterior], _poligono[posterior]);

							if (Objeto3D::interseccao(poligonoOriginal, segmento, erro) == false)
							{
								if (anterior < 0)
								{
									anterior += _poligono.size();
								}

								triangulos.push_back({ indices[anterior], indices[k], indices[posterior] });

								_poligono.erase(_poligono.begin() + k);
								indices.erase(indices.begin() + k);

								k--;
							}
						}
					}
				}

				for (size_t k = 0; k < triangulos.size(); k++)
				{
					this->adicionarFace(i, triangulos[k], false);
				}

				this->removerFace(i, j);

				j--;
			}
		}
	}
}

Objeto3D::Objeto3D()
{
	this->nomeObjeto = "Objeto3D";
}

Objeto3D::Objeto3D(const std::string &nome)
{
	this->nomeObjeto = std::string(nome);
}

Objeto3D::~Objeto3D()
{
	for (size_t i = 0; i < this->obterQtdGrupos(); i++)
	{
		if (this->textura[i])
		{
			delete this->textura[i];
		}
	}
}
