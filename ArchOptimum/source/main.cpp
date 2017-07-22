#include "../../Objeto3D/include/Objeto3D.h"
#include "../../Arquivo/include/Arquivo.h"
#include "../../SistInteligentes/include/Nesting.h"
#include "../../SistInteligentes/include/AlgGeneticoCFTM.h"

#include <Windows.h>

std::mutex mtxArquitetura;

int execucao = 0;

std::string gravacao;
std::wstring gravacaoWide;

inline bool existeArquivo(const std::string& name)
{
	std::ifstream f(name.c_str());

	return f.good();
}

void ultimoNesting(const std::vector < long double > &valores)
{
	using namespace std::chrono_literals;
	bool liberado = false;
	Arquivo arquivo;

	try
	{
		while (existeArquivo(gravacao + "Valores"))
		{
			std::this_thread::sleep_for(50ms);
		}

		arquivo.abrir(gravacao + "Valores", 35 | std::ios_base::trunc);

		arquivo.escrever(valores[0]);
		arquivo.escrever(valores[1]);
		arquivo.escrever(valores[2]);
		arquivo.escrever(valores[3]);

		arquivo.fechar();

		std::this_thread::sleep_for(100ms);

	reabrir:
		try
		{
			arquivo.abrir(gravacao + "Geometrias");
		}
		catch (...)
		{
			std::this_thread::sleep_for(1s);

			goto reabrir;
		}

		char valido;

		arquivo.ler(valido);

		if (!valido)
		{
			arquivo.fechar();

			std::wstring wstring(gravacaoWide + L"Geometrias");

			DeleteFile(wstring.c_str());
		}

		Nesting nesting({ Vertice(), Vertice(2750), Vertice(2750, 1830), Vertice(0, 1830) });

		unsigned int qtdMeshes;
		arquivo.ler(qtdMeshes);

		std::vector < std::vector < Vertice > > poligonos;
		std::vector < std::vector < std::vector < Vertice > > > buracos;

		for (size_t i = 0; i < qtdMeshes; i++)
		{
			unsigned int qtdVertices;
			arquivo.ler(qtdVertices);

			std::vector < Vertice > poligono(qtdVertices);

			for (size_t j = 0; j < qtdVertices; j++)
			{
				arquivo.ler(poligono[j].X);
				arquivo.ler(poligono[j].Y);
				arquivo.ler(poligono[j].Z);

				poligono[j].Z = 0;
			}

			for (size_t j = 1; j < poligono.size(); j++)
			{
				if (poligono[j - 1] == poligono[j])
				{
					poligono.erase(poligono.begin() + j);

					j--;
				}
			}

			if (poligono.size() < 3)
			{
				continue;
			}

			poligonos.push_back(poligono);
		}

		arquivo.fechar();

		if (!DeleteFile((gravacaoWide + L"Geometrias").c_str()))
		{
			std::cout << "ERRO AO DELETAR O ARQUIVO COM A GEOMETRIA!" << std::endl;
		}

		mtxArquitetura.unlock();

		liberado = true;

		std::vector < long long int > buracoDe(poligonos.size(), -1);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligonos.size()); i++)
		{
			for (size_t j = 0; j < poligonos.size(); j++)
			{
				if (i == j || buracoDe[j] != -1)
				{
					continue;
				}

				if (OperacoesGeometricas::contem(poligonos[j], poligonos[i]))
				{
					buracoDe[i] = j;

					break;
				}
			}
		}

		buracos.resize(poligonos.size());

		for (size_t i = 0; i < poligonos.size(); i++)
		{
			if (buracoDe[i] != -1)
			{
				buracos[buracoDe[i]].push_back(poligonos[i]);
			}
		}

		for (size_t i = 0; i < poligonos.size(); i++)
		{
			if (buracoDe[i] != -1)
			{
				poligonos.erase(poligonos.begin() + i);
				buracos.erase(buracos.begin() + i);
				buracoDe.erase(buracoDe.begin() + i);

				i--;
			}
		}

		for (size_t i = 0; i < poligonos.size(); i++)
		{
			try
			{
				nesting.adicionarPoligono(poligonos[i], buracos[i]);
			}
			catch (...) {}
		}

		nesting.rodar(3, false);

		Objeto3D objeto = nesting.obterObjeto();
		objeto.salvar(true);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	if (arquivo.estaAberto())
	{
		arquivo.fechar();
	}

	DeleteFile((gravacaoWide + L"Geometrias").c_str());

	if (!liberado)
	{
		mtxArquitetura.unlock();
	}
}

long double funcaoArquitetura(const std::vector < long double > &valores)
{
	using namespace std::chrono_literals;
	bool liberado = false;
	Arquivo arquivo;

	while (!mtxArquitetura.try_lock())
	{
		std::this_thread::sleep_for(1s);
	}

	std::cout << "Indivíduo " << execucao << std::endl;

	int individuo = execucao;

	execucao++;

	try
	{
		while (existeArquivo(gravacao + "Valores"))
		{
			std::this_thread::sleep_for(50ms);
		}

		arquivo.abrir(gravacao + "Valores", 35 | std::ios_base::trunc);

		arquivo.escrever(valores[0]);
		arquivo.escrever(valores[1]);
		arquivo.escrever(valores[2]);
		arquivo.escrever(valores[3]);

		arquivo.fechar();

		std::this_thread::sleep_for(100ms);

	reabrir:
		try
		{
			arquivo.abrir(gravacao + "Geometrias");
		}
		catch (...)
		{
			std::this_thread::sleep_for(1s);

			goto reabrir;
		}

		char valido;

		arquivo.ler(valido);

		if (!valido)
		{
			std::cout << "Indivíduo " << individuo << " --> INVALIDADO." << std::endl;

			arquivo.fechar();

			DeleteFile((gravacaoWide + L"Geometrias").c_str());

			mtxArquitetura.unlock();

			return LDBL_MAX;
		}

		Nesting nesting({ Vertice(), Vertice(2750), Vertice(2750, 1830), Vertice(0, 1830) });

		unsigned int qtdMeshes;
		arquivo.ler(qtdMeshes);

		std::vector < std::vector < Vertice > > poligonos;
		std::vector < std::vector < std::vector < Vertice > > > buracos;

		for (size_t i = 0; i < qtdMeshes; i++)
		{
			unsigned int qtdVertices;
			arquivo.ler(qtdVertices);

			std::vector < Vertice > poligono(qtdVertices);

			for (size_t j = 0; j < qtdVertices; j++)
			{
				arquivo.ler(poligono[j].X);
				arquivo.ler(poligono[j].Y);
				arquivo.ler(poligono[j].Z);

				poligono[j].Z = 0;
			}

			for (size_t j = 1; j < poligono.size(); j++)
			{
				if (poligono[j - 1] == poligono[j])
				{
					poligono.erase(poligono.begin() + j);

					j--;
				}
			}

			if (poligono.size() < 3)
			{
				continue;
			}

			poligonos.push_back(poligono);
		}

		arquivo.fechar();

		if (!DeleteFile((gravacaoWide + L"Geometrias").c_str()))
		{
			std::cout << "ERRO AO DELETAR O ARQUIVO COM A GEOMETRIA!" << std::endl;
		}

		mtxArquitetura.unlock();

		liberado = true;

		std::vector < long long int > buracoDe(poligonos.size(), -1);

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (poligonos.size()); i++)
		{
			for (size_t j = 0; j < poligonos.size(); j++)
			{
				if (i == j || buracoDe[j] != -1)
				{
					continue;
				}

				if (OperacoesGeometricas::contem(poligonos[j], poligonos[i]))
				{
					buracoDe[i] = j;

					break;
				}
			}
		}

		buracos.resize(poligonos.size());

		for (size_t i = 0; i < poligonos.size(); i++)
		{
			if (buracoDe[i] != -1)
			{
				buracos[buracoDe[i]].push_back(poligonos[i]);
			}
		}

		for (size_t i = 0; i < poligonos.size(); i++)
		{
			if (buracoDe[i] != -1)
			{
				poligonos.erase(poligonos.begin() + i);
				buracos.erase(buracos.begin() + i);
				buracoDe.erase(buracoDe.begin() + i);

				i--;
			}
		}

		for (size_t i = 0; i < poligonos.size(); i++)
		{
			try
			{
				nesting.adicionarPoligono(poligonos[i], buracos[i]);
			}
			catch(...){}
		}

		nesting.rodar(3, false);

		long double fitness = 0.0L;
		Objeto3D objeto = nesting.obterObjeto();

		objeto.salvar(true);

		for (long long int i = 0; i < static_cast < long long int > ((objeto.obterQtdGrupos() / 2) - 1); i++)
		{
			fitness += (2750 * 1830) / 1000000.0L;
		}

		long double maiorLargura = 0;
		const std::vector < Vertice > &ultimaPlaca = objeto.obterVertice((objeto.obterQtdGrupos() / 2) - 1);

		for (size_t i = 0; i < ultimaPlaca.size(); i++)
		{
			if (ultimaPlaca[i].X > maiorLargura)
			{
				maiorLargura = ultimaPlaca[i].X;
			}
		}

		fitness += (maiorLargura * 1830) / 1000000.0L;

		std::cout << "Indivíduo " << individuo << " --> SUCESSO." << std::endl;

		return fitness;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	if (arquivo.estaAberto())
	{
		arquivo.fechar();
	}

	std::string excecao = gravacao;
	excecao.append(std::to_string(std::chrono::system_clock::now().time_since_epoch().count()));

	arquivo.abrir(excecao, 35 | std::ios_base::trunc);

	arquivo.escrever(valores[0], false);
	arquivo.escrever('\n');
	arquivo.escrever(valores[1], false);
	arquivo.escrever('\n');
	arquivo.escrever(valores[2], false);
	arquivo.escrever('\n');
	arquivo.escrever(valores[3], false);

	arquivo.fechar();

	DeleteFile((gravacaoWide + L"Geometrias").c_str());

	if (!liberado)
	{
		mtxArquitetura.unlock();
	}

	std::cout << "Indivíduo " << individuo << " finalizado." << std::endl;

	return LDBL_MAX;
}

int main()
{
	std::cout << "Digite o diretório configurado no Rhino, finalizando com o símbolo \": ";
	std::cin >> gravacao;

	gravacaoWide = std::wstring(gravacao.length(), L' ');

	std::copy(gravacao.begin(), gravacao.end(), gravacaoWide.begin());

	std::vector < std::array < long double, 2 > > intervalos(4);
	std::vector < Otimizador::TipoVariavel > tiposVariaveis(intervalos.size(), Otimizador::TipoVariavel::INTEIRA);
	std::vector < size_t > qtdQuadrantes(intervalos.size(), 2);

	intervalos[0][0] = 2300.0L;
	intervalos[0][1] = 2800.0L;
	intervalos[1][0] = 3200.0L;
	intervalos[1][1] = 4000.0L;
	intervalos[2][0] = 2500.0L;
	intervalos[2][1] = 4000.0L;
	intervalos[3][0] = -1800.0L;
	intervalos[3][1] = 1800.0L;

	AlgGeneticoCFTM otimizador(gravacao, 10, 10800, intervalos, tiposVariaveis, funcaoArquitetura);

	otimizador.definirHistorico(10);

	otimizador.otimizar();

	std::vector < Otimizador::Avaliacao > melhoresAvaliacoes = otimizador.obterMelhoresAvaliacoes(10);

	Arquivo arquivo(gravacao + "MelhoresResultados.txt", 35 | std::ios_base::trunc);

	for (size_t i = 0; i < melhoresAvaliacoes.size(); i++)
	{
		std::string mensagem("Valores da avaliação ");
		mensagem.append(std::to_string(i));
		mensagem.append(":\n");

		arquivo.escrever(mensagem, false);

		for (size_t j = 0; j < melhoresAvaliacoes[i].valores.size(); j++)
		{
			arquivo.escrever('\t');
			arquivo.escrever(melhoresAvaliacoes[i].valores[j], false, 8);
			arquivo.escrever('\n');
		}

		arquivo.escrever('\n');

		mensagem = "Resultado da avaliação ";
		mensagem.append(std::to_string(i));
		mensagem.append(": ");

		arquivo.escrever(mensagem, false);
		arquivo.escrever(melhoresAvaliacoes[i].resultado, false, 8);
	}

	arquivo.fechar();

	ultimoNesting(melhoresAvaliacoes[0].valores);

	arquivo.abrir(gravacao + "ACABOU", 35 | std::ios_base::trunc);
	arquivo.fechar();

	return 0;
}
