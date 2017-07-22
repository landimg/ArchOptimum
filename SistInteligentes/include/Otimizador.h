#ifndef _OTIMIZADOR_H_
#define _OTIMIZADOR_H_

#include <array>
#include <string>
#include <vector>
#include <iostream>

#include "../../Excecao/include/Excecao.h"
#include "../../Serializacao/include/Serializacao.h"

class Otimizador :
	public Serializacao
{
public:
	enum class TipoOtimizacao;
	enum class TipoVariavel;
	class Avaliacao;

private:
	inline void mudarIntervaloDominio(const size_t eixo, const std::array < long double, 2 > &intervaloRestricao)
	{
		long double novoIntervaloMinimo = this->intervalos[eixo][0];
		long double novoIntervaloMaximo = this->intervalos[eixo][1];

		if (novoIntervaloMaximo < intervaloRestricao[0] || novoIntervaloMinimo > intervaloRestricao[1])
		{
			return;
		}

		if (novoIntervaloMinimo >= intervaloRestricao[0] && novoIntervaloMinimo <= intervaloRestricao[1])
		{
			novoIntervaloMinimo = intervaloRestricao[1];
		}

		if (novoIntervaloMaximo >= intervaloRestricao[0] && novoIntervaloMaximo <= intervaloRestricao[1])
		{
			novoIntervaloMaximo = intervaloRestricao[0];
		}

		if (novoIntervaloMinimo >= novoIntervaloMaximo)
		{
			std::string excecao = "A nova restrição remove o domíno do eixo ";
			excecao.append(std::to_string(eixo));
			excecao.append(".");

			Excecao::lancar(excecao);
		}

		this->intervalos[eixo][0] = novoIntervaloMinimo;
		this->intervalos[eixo][1] = novoIntervaloMaximo;
	}

protected:
	TipoOtimizacao tipoOtimizacao; // Como a otimização deve ser feita.
	std::vector < TipoVariavel > tiposVariaveis; // Quais são os tipos das variáveis de cada eixo.

	long double alvo; // Valor que o otimizador deve chegar.

	std::vector < std::array < long double, 2 > > intervalos; // Intervalos de trabalho do algoritmo. [eixo][min - max] = intervalo;
	std::vector < std::vector < std::array < long double, 2 > > > intervalosIndevidos; // Intervalos indevidos de trabalho. [eixo][indiceIntervalo][min - max] = limite;
	std::vector < bool(*)(const std::vector < long double > &valores) > relacoesIndevidas; // 

	std::vector < Avaliacao > avaliacoes; // Representação do conjunto de avaliações feitas pelo otimizador.
	std::vector < Avaliacao > melhoresAvaliacoes; // Histórico das melhores avaliações.

	long double(*funcaoAvaliacao)(const std::vector < long double > &valores); // Ponteiro para a função de avaliação.

	// Este método atualiza a lista com os melhores valores obtidos até o momento.
	void atualizarMelhoresAvaliacoes() noexcept
	{
		std::sort(this->avaliacoes.begin(), this->avaliacoes.end(),
			[] (const Avaliacao &anterior, const Avaliacao &posterior) -> bool
				{
					return anterior.erro < posterior.erro;
				});

		for (size_t i = 0, j = 0; i < this->melhoresAvaliacoes.size() && j < this->avaliacoes.size(); i++)
		{
			if (this->melhoresAvaliacoes[i].erro > this->avaliacoes[j].erro)
			{
				this->melhoresAvaliacoes.insert(this->melhoresAvaliacoes.begin() + i, this->avaliacoes[j]);
				this->melhoresAvaliacoes.pop_back();

				j++;
			}
		}
	}

public:
	enum class TipoOtimizacao
	{
		MINIMIZACAO,
		MAXIMIZACAO,
		VALOR_ESPECIFICO,
		TIPO_INVALIDO
	};

	enum class TipoVariavel
	{
		INTEIRA,
		REAL
	};

	// Representação de uma avaliação do otimizador.
	class Avaliacao
	{
	public:
		std::vector < long double > valores; // Conjunto de entradas para a função de avaliação.
		long double resultado; // Resultado da avaliação.
		long double erro; // Erro da avaliação.

		Avaliacao()
		{
			this->resultado = 0;
			this->erro = LDBL_MAX;
		}
		virtual ~Avaliacao()
		{
		}
	};

	virtual void otimizar() noexcept = 0;

	virtual inline void imprimirIndividuos() const noexcept
	{
		for (size_t i = 0; i < this->avaliacoes.size(); i++)
		{
			for (size_t j = 0; j < this->avaliacoes[i].valores.size(); j++)
			{
				std::cout << this->avaliacoes[i].valores[j] << " ";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	inline void adicionarRelacaoIndevida(bool(*relacaoIndevida)(const std::vector < long double > &valores)) noexcept
	{
		this->relacoesIndevidas.push_back(relacaoIndevida);
	}

	inline void removerRelacaoIndevida(const size_t indice)
	{
		if (indice >= this->relacoesIndevidas.size())
		{
			Excecao::lancar("Índice fora dos limites.");
		}

		this->relacoesIndevidas.erase(this->relacoesIndevidas.begin() + indice);
	}

	inline std::vector < Avaliacao > obterMelhoresAvaliacoes(const size_t qtdMelhoresAvaliacoes) const noexcept
	{
		std::vector < Avaliacao > melhoresAvaliacoes;

		for (size_t i = 0; i < qtdMelhoresAvaliacoes && i < this->melhoresAvaliacoes.size(); i++)
		{
			melhoresAvaliacoes.push_back(this->melhoresAvaliacoes[i]);
		}

		return melhoresAvaliacoes;
	}

	inline void definirHistorico(const size_t historicoValores)
	{
		this->melhoresAvaliacoes.resize(historicoValores);
	}

	inline void definirIntervaloIndevido(const size_t eixo, long double min, long double max)
	{
		if (eixo >= this->intervalos.size())
		{
			Excecao::lancar("eixo maior ou igual a quantidade de eixos existentes.");
		}

		if (tiposVariaveis[eixo] == Otimizador::TipoVariavel::INTEIRA)
		{
			min = std::truncl(min);
			max = std::truncl(max);
		}

		if (min == max)
		{
			Excecao::lancar("Valores de intervalo idênticos.");
		}

		if (min > max)
		{
			Excecao::lancar("Valor máximo menor que mínimo.");
		}

		std::array < long double, 2 > intervalo = { min, max };

		this->mudarIntervaloDominio(eixo, intervalo);

		this->intervalosIndevidos[eixo].push_back(intervalo);
	}

	inline void serializar(Arquivo &arquivo) const override
	{
		arquivo.escrever(this->alvo);
		arquivo.escrever(this->intervalos);
		arquivo.escrever(this->intervalosIndevidos);
		arquivo.escrever(this->avaliacoes);
		arquivo.escrever(this->melhoresAvaliacoes);
		arquivo.escrever(this->tiposVariaveis);
		arquivo.escrever(this->tipoOtimizacao);
	}
	
	inline void deserializar(Arquivo &arquivo) override
	{
		arquivo.ler(this->alvo);
		arquivo.ler(this->intervalos);
		arquivo.ler(this->intervalosIndevidos);
		arquivo.ler(this->avaliacoes);
		arquivo.ler(this->melhoresAvaliacoes);
		arquivo.ler(this->tiposVariaveis);
		arquivo.ler(this->tipoOtimizacao);
	}

	Otimizador(long double(*funcaoAvaliacao)(const std::vector < long double > &valores)) noexcept
	{
		if (funcaoAvaliacao == nullptr)
		{
			Excecao::lancar("Não existe uma função de avaliação.");
		}

		this->alvo = std::nanl(nullptr);
		this->tipoOtimizacao = Otimizador::TipoOtimizacao::TIPO_INVALIDO;
		this->funcaoAvaliacao = funcaoAvaliacao;
	}

	Otimizador(const std::vector < std::array < long double, 2 > > &intervalos,
		const std::vector < TipoVariavel > &tiposVariaveis,
		long double(*funcaoAvaliacao)(const std::vector < long double > &valores),
		const Otimizador::TipoOtimizacao tipoOtimizacao = Otimizador::TipoOtimizacao::TIPO_INVALIDO,
		long double alvo = std::nanl(nullptr))
	{
		if (intervalos.size() == 0)
		{
			Excecao::lancar("Não há valores para serem otimizados.");
		}

		if (intervalos.size() != tiposVariaveis.size())
		{
			Excecao::lancar("Quantidade de tipos de variáveis diferente da quantidade declarada em minMax.");
		}

		this->intervalos.resize(intervalos.size());

		for (size_t i = 0; i < intervalos.size(); i++)
		{
			this->intervalos[i][0] = intervalos[i][0];
			this->intervalos[i][1] = intervalos[i][1];

			long double &min = this->intervalos[i][0];
			long double &max = this->intervalos[i][1];

			if (tiposVariaveis[i] == Otimizador::TipoVariavel::INTEIRA)
			{
				min = std::truncl(min);
				max = std::truncl(max);
			}

			if (min == max)
			{
				std::string excecao = "Valores de intervalo idênticos no índice ";
				excecao.append(std::to_string(i));
				excecao.append(".");

				Excecao::lancar(excecao);
			}

			if (min > max)
			{
				std::string excecao = "Valor máximo menor que mínimo no índice ";
				excecao.append(std::to_string(i));
				excecao.append(".");

				Excecao::lancar(excecao);
			}
		}

		if (funcaoAvaliacao == nullptr)
		{
			Excecao::lancar("Não existe uma função de avaliação.");
		}

		this->funcaoAvaliacao = funcaoAvaliacao;

		switch (tipoOtimizacao)
		{
		case Otimizador::TipoOtimizacao::MINIMIZACAO:
			{
				this->alvo = LDBL_MAX;
				break;
			}
		case Otimizador::TipoOtimizacao::MAXIMIZACAO:
			{
				this->alvo = -LDBL_MAX;
				break;
			}
		case Otimizador::TipoOtimizacao::VALOR_ESPECIFICO:
			{
				this->alvo = alvo;
				break;
			}
		default:
			{
				Excecao::lancar("Tipo de otimização não conhecido.");
			}
		}

		this->tipoOtimizacao = tipoOtimizacao;
		this->tiposVariaveis = tiposVariaveis;
		this->intervalosIndevidos.resize(this->intervalos.size());
		this->definirHistorico(1);
	}

	virtual ~Otimizador() noexcept
	{
	}
};

#endif