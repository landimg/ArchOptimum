#include "../include/AlgGeneticoCFTM.h"
#include <mutex>
#include <random>

class IndiceAvaliacao
{
public:
	size_t indice;
	const Otimizador::Avaliacao *avaliacao;

	IndiceAvaliacao(const size_t indice, const Otimizador::Avaliacao *avaliacao)
	{
		this->indice = indice;
		this->avaliacao = avaliacao;
	}

	IndiceAvaliacao()
	{
		this->indice = SIZE_MAX;
		this->avaliacao = nullptr;
	}
};

size_t populacao = 10;

std::vector < size_t > torneio(const std::vector < Otimizador::Avaliacao > &candidatos)
{
	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));
	std::uniform_int_distribution < size_t > distribuicaoDiscreta(0, populacao - 1);

	std::vector < size_t > _pais;
	std::vector < IndiceAvaliacao > pais;

	for (size_t i = 0; i < 6; i++)
	{
		size_t possivelPai = distribuicaoDiscreta(gerador);

		if (std::find(_pais.begin(), _pais.end(), possivelPai) == _pais.end())
		{
			_pais.push_back(possivelPai);
			pais.push_back(IndiceAvaliacao(possivelPai, &candidatos[possivelPai]));
		}

		else
		{
			i--;
		}
	}

	std::sort(pais.begin(), pais.end(),
		[](const IndiceAvaliacao &anterior, const IndiceAvaliacao &posterior) -> bool
		{
			return anterior.avaliacao->erro < posterior.avaliacao->erro;
		});

	return{ pais[0].indice, pais[1].indice };
}

Otimizador::Avaliacao crossoverBLX1(const std::vector < Otimizador::Avaliacao > &pais)
{
	Otimizador::Avaliacao filho = pais[0];

	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));
	std::uniform_real_distribution < long double > distribuicaoContinua(0.0L, 1.0L);

	long double alfa = distribuicaoContinua(gerador);

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (pais.size()); i++)
	{
		filho.valores[i] = pais[0].valores[i] * alfa + pais[1].valores[i] * (1 - alfa);
	}

	return filho;
}

Otimizador::Avaliacao crossoverBLX2(const std::vector < Otimizador::Avaliacao > &pais)
{
	Otimizador::Avaliacao filho = pais[0];

	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));
	std::uniform_real_distribution < long double > distribuicaoContinua(0.0L, 1.0L);
	std::mutex mtx;

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (pais.size()); i++)
	{
		while (mtx.try_lock() == false)
		{
			std::this_thread::yield();
		}

		long double alfa = distribuicaoContinua(gerador);

		mtx.unlock();

		filho.valores[i] = pais[0].valores[i] * alfa + pais[1].valores[i] * (1 - alfa);
	}

	return filho;
}

Otimizador::Avaliacao crossoverBLX(const std::vector < Otimizador::Avaliacao > &pais)
{
	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));
	std::uniform_real_distribution < long double > distribuicaoContinua(0.0L, 1.0L);

	if (distribuicaoContinua(gerador) <= 0.5)
	{
		return crossoverBLX1(pais);
	}

	return crossoverBLX2(pais);
}

void AlgGeneticoCFTM::gerarPopulacao() noexcept
{
	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));

	for (size_t i = 0; i < this->intervalos.size(); i++)
	{
		std::uniform_real_distribution < long double > distribuicao(
			this->intervalos[i][0], this->intervalos[i][1]);

		for (size_t j = 0; j < populacao; j++)
		{
			this->avaliacoes[j].valores[i] = distribuicao(gerador);
		}
	}
}

void AlgGeneticoCFTM::genocidio() noexcept
{
	std::sort(this->avaliacoes.begin(), this->avaliacoes.end(),
		[] (const Avaliacao &anterior, const Avaliacao &posterior) -> bool
		{
			return anterior.erro < posterior.erro;
		});

	Avaliacao melhor = this->avaliacoes[0];

	this->gerarPopulacao();

	this->avaliacoes[0] = melhor;
}

bool AlgGeneticoCFTM::atualizarFilho(Avaliacao *pai, Avaliacao *mae, const Avaliacao &filho) noexcept
{
	Avaliacao *pior;

	if (pai->erro > mae->erro)
	{
		pior = pai;
	}

	else
	{
		pior = mae;
	}

	if (pior->erro > filho.erro)
	{
		*pior = filho;

		return true;
	}

	return false;
}

void AlgGeneticoCFTM::otimizar() noexcept
{
	this->gerarPopulacao();

	#pragma omp parallel for
	for (long long int i = 0; i < static_cast < long long int > (this->avaliacoes.size()); i++)
	{
		this->avaliacoes[i].resultado = this->funcaoAvaliacao(this->avaliacoes[i].valores);
		this->avaliacoes[i].erro = this->avaliacoes[i].resultado;
	}

	auto tempoInicial = std::chrono::high_resolution_clock::now();
	size_t geracoesSemMelhora = 0;

	auto tempoGravacao = std::chrono::high_resolution_clock::now();

	Arquivo arquivo(this->localGravacao + (std::to_string((std::chrono::high_resolution_clock::now() - tempoInicial).count())), 35 | std::ios_base::trunc);

	for (size_t i = 0; i < this->avaliacoes.size(); i++)
	{
		arquivo.escrever(std::string("Indivíduo ").append(std::to_string(i)).append(":\n"), false);

		for (size_t j = 0; j < this->avaliacoes[i].valores.size(); j++)
		{
			arquivo.escrever(std::string("\tGene ").append(std::to_string(j)).append(": "), false);

			arquivo.escrever(this->avaliacoes[i].valores[j], false, 10);

			arquivo.escrever("\n", false);
		}

		arquivo.escrever("Resultado: ", false);
		arquivo.escrever(this->avaliacoes[i].resultado, false, 10);

		arquivo.escrever("\n", false);
	}

	arquivo.fechar();

	while ((std::chrono::high_resolution_clock::now() - tempoInicial) < this->tempoExecucao)
	{
		if ((std::chrono::high_resolution_clock::now() - tempoGravacao) >= std::chrono::seconds(15 * 60))
		{
			tempoGravacao = std::chrono::high_resolution_clock::now();

			arquivo.abrir(this->localGravacao + (std::to_string((std::chrono::high_resolution_clock::now() - tempoInicial).count())), 35 | std::ios_base::trunc);

			for (size_t i = 0; i < this->avaliacoes.size(); i++)
			{
				arquivo.escrever(std::string("Indivíduo ").append(std::to_string(i)).append(":\n"), false);

				for (size_t j = 0; j < this->avaliacoes[i].valores.size(); j++)
				{
					arquivo.escrever(std::string("\tGene ").append(std::to_string(j)).append(": "), false);

					arquivo.escrever(this->avaliacoes[i].valores[j], false, 10);

					arquivo.escrever("\n", false);
				}

				arquivo.escrever("Resultado: ", false);
				arquivo.escrever(this->avaliacoes[i].resultado, false, 10);

				arquivo.escrever("\n", false);
			}

			arquivo.fechar();
		}

		std::vector < size_t > pais = this->selecaoPais(this->avaliacoes);

		Avaliacao &pai = this->avaliacoes[pais[0]];
		Avaliacao &mae = this->avaliacoes[pais[1]];

		Avaliacao filho = this->crossover({ pai, mae });

		#pragma omp parallel for
		for (long long int i = 0; i < static_cast < long long int > (this->mutacoes.size()); i++)
		{
			if (this->mutacoes[i])
			{
				filho.valores[i] = this->mutacoes[i](filho.valores[i], this->intervalos[i]);
			}
		}

		filho.resultado = this->funcaoAvaliacao(filho.valores);
		filho.erro = filho.resultado;

		if (!AlgGeneticoCFTM::atualizarFilho(&pai, &mae, filho))
		{
			geracoesSemMelhora++;
		}

		else
		{
			geracoesSemMelhora = 0;
		}

		if (geracoesSemMelhora >= this->geracoesSemMelhora)
		{
			geracoesSemMelhora = 0;

			this->atualizarMelhoresAvaliacoes();

			this->genocidio();
		}
	}

	this->atualizarMelhoresAvaliacoes();

	for (size_t i = 0; i < this->melhoresAvaliacoes.size(); i++)
	{
		std::cout << "Indivíduo " << i << ":" << std::endl;

		for (size_t j = 0; j < this->melhoresAvaliacoes[i].valores.size(); j++)
		{
			std::cout << "\t" << this->melhoresAvaliacoes[i].valores[j] << std::endl;
		}

		std::cout << "\tResultado: " << this->melhoresAvaliacoes[i].resultado << std::endl << std::endl;
	}
}

AlgGeneticoCFTM::AlgGeneticoCFTM(long double(*funcaoAvaliacao)(const std::vector < long double > &valores)) :
	AlgGenetico(funcaoAvaliacao,
	std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &_intervalos) >(0,
		[](long double gene, const std::array < long double, 2 > &_intervalos) -> long double
{
	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));
	std::uniform_real_distribution < long double > distribuicaoContinua(0.0L, 1.0L);

	if (distribuicaoContinua(gerador) < 0.3L)
	{
		std::uniform_real_distribution < long double > novoGene(_intervalos[0], _intervalos[1]);

		return novoGene(gerador);
	}

	return gene;
}) , crossoverBLX, torneio)
{
}

AlgGeneticoCFTM::AlgGeneticoCFTM(const std::string &localGravacao, const size_t geracoesSemMelhora, const long long int tempoExecucao,
	const std::vector < std::array < long double, 2 > > &intervalos,
	const std::vector < TipoVariavel > &tiposVariaveis,
	long double(*funcaoAvaliacao)(const std::vector < long double > &valores)) :

AlgGenetico(geracoesSemMelhora, tempoExecucao, intervalos, tiposVariaveis, funcaoAvaliacao,
	std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &_intervalos) >(intervalos.size(),
		[](long double gene, const std::array < long double, 2 > &_intervalos) -> long double
{
	std::default_random_engine gerador(static_cast < unsigned int >(time(nullptr)));
	std::uniform_real_distribution < long double > distribuicaoContinua(0.0L, 1.0L);

	if (distribuicaoContinua(gerador) < 0.3L)
	{
		std::uniform_real_distribution < long double > novoGene(_intervalos[0], _intervalos[1]);

		return novoGene(gerador);
	}

	return gene;
}) ,
	crossoverBLX, torneio, Otimizador::TipoOtimizacao::MINIMIZACAO)
{
	this->localGravacao = localGravacao;

	this->avaliacoes.resize(populacao);

	for (size_t i = 0; i < populacao; i++)
	{
		this->avaliacoes[i].valores.resize(intervalos.size());
	}
}

AlgGeneticoCFTM::~AlgGeneticoCFTM()
{
}
