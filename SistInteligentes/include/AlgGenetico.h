#ifndef _ALG_GENETICO_H_
#define _ALG_GENETICO_H_

#include "Otimizador.h"
#include <chrono>

using namespace std::chrono_literals;

class AlgGenetico :
	public Otimizador
{
protected:
	size_t geracoesSemMelhora; // Condi��o de parada por quantidade de execu��es que n�o houve atualiza��es no melhor indiv�duo.
	std::chrono::seconds tempoExecucao; // Condi��o de parada por tempo em segundos.
	std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &intervalos) > mutacoes; // Fun��o de muta��o para cada gene.
	Avaliacao(*crossover)(const std::vector < Avaliacao > &pais); // Fun��o para crossover.
	std::vector < size_t > (*selecaoPais)(const std::vector < Avaliacao > &candidatos); // Fun��o para escolha dos pais; Retorna os �ndices dos pais escolhidos.

public:
	void definirGeracoesSemMelhora(const size_t geracoesSemMelhora) noexcept
	{
		this->geracoesSemMelhora = geracoesSemMelhora;
	}

	void definirTempoExecucao(const long long int tempoExecucao)
	{
		if (tempoExecucao <= 0)
		{
			Excecao::lancar("Tempo de execu��o deve ser positivo.");
		}

		this->tempoExecucao = std::chrono::seconds(tempoExecucao);
	}

	void serializar(Arquivo &arquivo) const override;
	void deserializar(Arquivo &arquivo) override;

	AlgGenetico(long double(*funcaoAvaliacao)(const std::vector < long double > &valores),
		const std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &_intervalos) > &mutacoes,
		Avaliacao(*crossover)(const std::vector < Avaliacao > &pais),
		std::vector < size_t >(*selecaoPais)(const std::vector < Avaliacao > &candidatos));

	AlgGenetico(const size_t geracoesSemMelhora, const long long int tempoExecucao,
		const std::vector < std::array < long double, 2 > > &intervalos,
		const std::vector < TipoVariavel > &tiposVariaveis,
		long double(*funcaoAvaliacao)(const std::vector < long double > &valores),
		const std::vector < long double(*)(const long double gene, const std::array < long double, 2 > &_intervalos) > &mutacoes,
		Avaliacao(*crossover)(const std::vector < Avaliacao > &pais),
		std::vector < size_t >(*selecaoPais)(const std::vector < Avaliacao > &candidatos),
		const Otimizador::TipoOtimizacao tipoOtimizacao = Otimizador::TipoOtimizacao::TIPO_INVALIDO,
		long double alvo = std::nanl(nullptr));
	virtual ~AlgGenetico();
};

#endif