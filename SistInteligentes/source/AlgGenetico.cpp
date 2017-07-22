#include "../include/AlgGenetico.h"

void AlgGenetico::serializar(Arquivo &arquivo) const
{
	Otimizador::serializar(arquivo);

	arquivo.escrever(this->geracoesSemMelhora);
	arquivo.escrever(this->tempoExecucao);
}

void AlgGenetico::deserializar(Arquivo &arquivo)
{
	Otimizador::deserializar(arquivo);

	if (mutacoes.size() < this->intervalos.size())
	{
		Excecao::lancar("Nem todos os genes possuem uma função de mutação.");
	}

	arquivo.ler(this->geracoesSemMelhora);
	arquivo.ler(this->tempoExecucao);
}

AlgGenetico::AlgGenetico(long double(*funcaoAvaliacao)(const std::vector < long double > &valores),
	const std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &_intervalos) > &mutacoes,
	Avaliacao(*crossover)(const std::vector < Avaliacao > &pais),
	std::vector < size_t >(*selecaoPais)(const std::vector < Avaliacao > &candidatos)) :
	Otimizador(funcaoAvaliacao)
{
	if (crossover == nullptr)
	{
		Excecao::lancar("Não existe uma função de crossover.");
	}

	this->geracoesSemMelhora = 0;
	this->tempoExecucao = std::chrono::seconds(0);
	this->mutacoes = mutacoes;
	this->crossover = crossover;
	this->selecaoPais = selecaoPais;
}

AlgGenetico::AlgGenetico(const size_t geracoesSemMelhora, const long long int tempoExecucao,
	const std::vector < std::array < long double, 2 > > &intervalos,
	const std::vector < Otimizador::TipoVariavel > &tiposVariaveis,
	long double(*funcaoAvaliacao)(const std::vector < long double > &valores),
	const std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &_intervalos) > &mutacoes,
	Otimizador::Avaliacao(*crossover)(const std::vector < Otimizador::Avaliacao > &pais),
	std::vector < size_t >(*selecaoPais)(const std::vector < Avaliacao > &candidatos),
	const Otimizador::TipoOtimizacao tipoOtimizacao, long double alvo) :
	Otimizador(intervalos, tiposVariaveis, funcaoAvaliacao, tipoOtimizacao, alvo)
{
	this->definirGeracoesSemMelhora(geracoesSemMelhora);
	this->definirTempoExecucao(tempoExecucao);

	if (mutacoes.size() < this->intervalos.size())
	{
		Excecao::lancar("Nem todos os genes possuem uma função de mutação.");
	}

	if (crossover == nullptr)
	{
		Excecao::lancar("Não existe uma função de crossover.");
	}

	this->mutacoes = mutacoes;
	this->crossover = crossover;
	this->selecaoPais = selecaoPais;
}

AlgGenetico::~AlgGenetico()
{
}
