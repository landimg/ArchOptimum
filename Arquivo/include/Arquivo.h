#ifndef _ARQUIVO_H_
#define _ARQUIVO_H_

#include <array>
#include <fstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <chrono>

#include "../../Excecao/include/Excecao.h"

// Classe utilizada para manipulação de dados mantidos em memória persistente.
class Arquivo
{
private:
	// Nome do arquivo, podendo conter diretório que ele se encontra.
	std::string caminho;

	// Manipulador para os dados contidos em memória persistente.
	std::fstream arquivo;

	// Verificação da integridade do arquivo, após a realização de qualquer ação.
	inline void validarAcao() const;

public:
	/*
	 * Método usado para a abertura de arquivos.
	 * A abertura é feita compulsoriamente para leitura e escrita.
	 *
	 * Os modos de abertura estão atrelados à ios_base:
	 *		app --> Faz um "seek" para o fim do arquivo a cada escrita.
	 *		ate --> Define o fim do arquivo como posição inicial.
	 *		binary --> Define o arquivo como binário.
	 *		in --> Define o arquivo como entrada.
	 *		out --> Define o arquivo como saída.
	 *		trunc --> Apaga o conteúdo do arquivo.
	 */
	void abrir(const std::string &caminho, const int modoAbertura = std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	/*
	 * Método usado para a abertura de arquivos.
	 * A abertura é feita compulsoriamente para leitura e escrita.
	 * Caso algum arquivo já tenho sido aberto e fechado, utiliza-se o último caminho dado.
	 *
	 * Os modos de abertura estão atrelados à ios_base:
	 *		app --> Faz um "seek" para o fim do arquivo a cada escrita.
	 *		ate --> Define o fim do arquivo como posição inicial.
	 *		binary --> Define o arquivo como binário.
	 *		in --> Define o arquivo como entrada.
	 *		out --> Define o arquivo como saída.
	 *		trunc --> Apaga o conteúdo do arquivo.
	 */
	void abrir(const int modoAbertura = std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	// Método usado para o fechamento do arquivo.
	void fechar();

	// Método usado para verificar se há algum arquivo aberto para este objeto.
	bool estaAberto() const;

	/*
	 * Método usado para a leitura de quaisquer informações e armazená-las em p.
	 * É obrigatório que a alocação de memória de p tenha sido feita previamente.
	 */
	void ler(void *p, const size_t qtdBytes);

	// Método usado para a leitura de uma informação booleana.
	void ler(bool &info);

	// Método usado para a leitura de um caractere.
	void ler(unsigned char &caractere);

	// Método usado para a leitura de um caractere.
	void ler(char &caractere);

	/*
	 * Método usado para a leitura de uma linha inteira do arquivo.
	 * Caracteres de parada: '\n', EOF e '\0'.
	 * É obrigatório que a alocação de memória de string tenha sido feita previamente.
	 */
	void ler(unsigned char *string);

	/*
	 * Método usado para a leitura de uma linha inteira do arquivo.
	 * Caracteres de parada: '\n', EOF e '\0'.
	 * É obrigatório que a alocação de memória de string tenha sido feita previamente.
	 */
	void ler(char *string);

	/*
	 * Método usado para a leitura de uma linha inteira do arquivo.
	 * Caracteres de parada: '\n', EOF e '\0'.
	 */
	void ler(std::string &string);

	/*
	 * Método usado para a leitura de um número de tipo unsigned int do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(unsigned int &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo int do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(int &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo unsigned long int do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(unsigned long int &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo long int do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(long int &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo unsigned long long do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(unsigned long long int &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo long long do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(long long int &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo float do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(float &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo double do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(double &numero, const bool formato = true);

	/*
	 * Método usado para a leitura de um número de tipo long double do arquivo.
	 * formato: false - texto; true - binário.
	 */
	void ler(long double &numero, const bool formato = true);

	// Método usado para a leitura de um dado de tipo genérico do arquivo.
	template < typename TipoDado >
	inline void ler(TipoDado &dado)
	{
		if (!this->arquivo.is_open())
		{
			Excecao::lancar("Não há um arquivo aberto.");
		}

		char *aux = new char[sizeof(TipoDado)];

		this->arquivo.read(aux, sizeof(TipoDado));

		memmove_s(&dado, sizeof(TipoDado), aux, sizeof(TipoDado));

		delete[] aux;

		this->validarAcao();
	}

	// Método usado para a leitura de uma ordem de dados genéricos do arquivo.
	template < typename TipoDados, size_t qtdElementos >
	inline void ler(std::array < TipoDados, qtdElementos > &vetor)
	{
		for (size_t i = 0; i < qtdElementos; i++)
		{
			this->ler(vetor[i]);
		}
	}

	// Método usado para a leitura de um vetor de dados genéricos do arquivo.
	template < typename TipoDados >
	inline void ler(std::vector < TipoDados > &vetor)
	{
		size_t qtdElementos;

		this->ler(qtdElementos);
		vetor.resize(qtdElementos);

		for (size_t i = 0; i < qtdElementos; i++)
		{
			this->ler(vetor[i]);
		}
	}

	// Método usado para a leitura de um par de dados genéricos do arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void ler(std::pair < TipoDados1, TipoDados2 > &par)
	{
		this->ler(par.first);
		this->ler(par.second);
	}

	// Método usado para a leitura de uma tabela hash do arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void ler(std::map < TipoDados1, TipoDados2 > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados1 dado1;
			TipoDados2 dado2;

			this->ler(dado1);
			this->ler(dado2);

			mapa.insert(std::pair < TipoDados1, TipoDados2 >(dado1, dado2));
		}
	}

	// Método usado para a leitura de uma tabela hash do arquivo, com uma hash específica.
	template < typename TipoDados1, typename TipoDados2, typename Hash >
	inline void ler(std::map < TipoDados1, TipoDados2, Hash > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados1 dado1;
			TipoDados2 dado2;

			this->ler(dado1);
			this->ler(dado2);

			mapa.insert(std::pair < TipoDados1, TipoDados2 >(dado1, dado2));
		}
	}

	// Método usado para a leitura de uma tabela hash sem ordem do arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void ler(std::unordered_map < TipoDados1, TipoDados2 > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados1 dado1;
			TipoDados2 dado2;

			this->ler(dado1);
			this->ler(dado2);

			mapa.insert(std::pair < TipoDados1, TipoDados2 >(dado1, dado2));
		}
	}

	// Método usado para a leitura de uma tabela hash sem ordem do arquivo, com uma hash específica.
	template < typename TipoDados1, typename TipoDados2, typename EstruturaHash >
	inline void ler(std::unordered_map < TipoDados1, TipoDados2, EstruturaHash > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados1 dado1;
			TipoDados2 dado2;

			this->ler(dado1);
			this->ler(dado2);

			mapa.insert(std::pair < TipoDados1, TipoDados2 >(dado1, dado2));
		}
	}

	// Método usado para a leitura de uma árvore de dados do arquivo.
	template < typename TipoDados >
	inline void ler(std::set < TipoDados > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados dado;

			this->ler(dado);

			mapa.insert(dado);
		}
	}

	// Método usado para a leitura de uma árvore de dados do arquivo, com uma hash específica.
	template < typename TipoDados, typename EstruturaHash >
	inline void ler(std::set < TipoDados, EstruturaHash > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados dado;

			this->ler(dado);

			mapa.insert(dado);
		}
	}

	// Método usado para a leitura de uma árvore de dados sem ordem do arquivo.
	template < typename TipoDados >
	inline void ler(std::unordered_set < TipoDados > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados dado;

			this->ler(dado);

			mapa.insert(dado);
		}
	}

	// Método usado para a leitura de uma árvore de dados sem ordem do arquivo, com uma hash específica.
	template < typename TipoDados, typename EstruturaHash >
	inline void ler(std::unordered_set < TipoDados, EstruturaHash > &mapa)
	{
		size_t qtdRelacoes;

		this->ler(qtdRelacoes);

		for (size_t i = 0; i < qtdRelacoes; i++)
		{
			TipoDados dado;

			this->ler(dado);

			mapa.insert(dado);
		}
	}

	// Método usado para a escrita de um tempo no arquivo.
	template < typename TipoDados, typename Periodo >
	inline void ler(std::chrono::duration < TipoDados, Periodo > &tempo)
	{
		TipoDados _tempo;

		this->ler(_tempo);

		tempo = std::chrono::duration < TipoDados, Periodo > (_tempo);
	}

	// Método usado para a escrita de quaisquer informações de p no arquivo.
	void escrever(const void *p, const size_t qtdBytes);

	// Método usado para a escrita de uma informação booleana no arquivo.
	void escrever(const bool info);

	// Método usado para a escrita de um caractere no arquivo.
	void escrever(const unsigned char caractere);

	// Método usado para a escrita de um caractere no arquivo.
	void escrever(const char caractere);

	/*
	 * Método usado para a escrita de uma string no arquivo.
	 * barraZero insere um caractere nulo ('/0') ao fim da string, se true.
	 */
	void escrever(const unsigned char *string, const bool barraZero = true);

	/*
	 * Método usado para a escrita de uma string no arquivo.
	 * barraZero insere um caractere nulo ('/0') ao fim da string, se true.
	 */
	void escrever(const char *string, const bool barraZero = true);

	/*
	 * Método usado para a escrita de uma string no arquivo.
	 * barraZero insere um caractere nulo ('/0') ao fim da string, se true.
	 */
	void escrever(const std::string &string, const bool barraZero = true);

	/*
	 * Método usado para a escrita um número de tipo unsigned short int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const unsigned short int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo short int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const short int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo unsigned int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const unsigned int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo unsigned long int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const unsigned long int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo long int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const long int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo unsigned long long int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const unsigned long long int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo long long int no arquivo.
	 * formato: false - texto; true - binário.
	 */
	void escrever(const long long int numero, const bool formato = true);

	/*
	 * Método usado para a escrita um número de tipo float no arquivo.
	 * formato: false - texto; true - binário.
	 * numeroCasasDecimais: quantas casas decimais, depois da vírgula, serão gravadas no arquivo.
	 */
	void escrever(const float numero, const bool formato = true, const size_t numeroCasasDecimais = 5);

	/*
	 * Método usado para a escrita um número de tipo double no arquivo.
	 * formato: false - texto; true - binário.
	 * numeroCasasDecimais: quantas casas decimais, depois da vírgula, serão gravadas no arquivo.
	 */
	void escrever(const double numero, const bool formato = true, const size_t numeroCasasDecimais = 5);

	/*
	 * Método usado para a escrita um número de tipo double no arquivo.
	 * formato: false - texto; true - binário.
	 * numeroCasasDecimais: quantas casas decimais, depois da vírgula, serão gravadas no arquivo.
	 */
	void escrever(const long double numero, const bool formato = true, const size_t numeroCasasDecimais = 5);

	// Método usado para a escrita de um dado de tipo genérico no arquivo.
	template < typename TipoDado >
	inline void escrever(const TipoDado &dado)
	{
		if (!this->arquivo.is_open())
		{
			Excecao::lancar("Não há um arquivo aberto.");
		}

		this->arquivo.write(reinterpret_cast < const char * > (&dado), sizeof(TipoDado));

		this->validarAcao();
	}

	// Método usado para a escrita de uma ordem de dados genéricos no arquivo.
	template < typename TipoDados, size_t qtdElementos >
	inline void escrever(const std::array < TipoDados, qtdElementos > &vetor)
	{
		for (size_t i = 0; i < qtdElementos; i++)
		{
			this->escrever(vetor[i]);
		}
	}

	// Método usado para a escrita de um vetor de dados genéricos no arquivo.
	template < typename TipoDados >
	inline void escrever(const std::vector < TipoDados > &vetor)
	{
		this->escrever(vetor.size());

		for (size_t i = 0; i < vetor.size(); i++)
		{
			this->escrever(vetor[i]);
		}
	}

	// Método usado para a escrita de um par de dados genéricos no arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void escrever(const std::pair < TipoDados1, TipoDados2 > &par)
	{
		this->escrever(par.first);
		this->escrever(par.second);
	}

	// Método usado para a escrita de uma tabela hash no arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void escrever(const std::map < TipoDados1, TipoDados2 > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &relacao : mapa)
		{
			this->escrever(relacao.first);
			this->escrever(relacao.second);
		}
	}

	// Método usado para a escrita de uma tabela hash no arquivo, com uma hash específica.
	template < typename TipoDados1, typename TipoDados2, typename EstruturaHash >
	inline void escrever(const std::map < TipoDados1, TipoDados2, EstruturaHash > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &relacao : mapa)
		{
			this->escrever(relacao.first);
			this->escrever(relacao.second);
		}
	}

	// Método usado para a escrita de uma tabela hash sem ordem no arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void escrever(const std::unordered_map < TipoDados1, TipoDados2 > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &relacao : mapa)
		{
			this->escrever(relacao.first);
			this->escrever(relacao.second);
		}
	}

	// Método usado para a escrita de uma tabela hash sem ordem no arquivo, com uma hash específica.
	template < typename TipoDados1, typename TipoDados2, typename EstruturaHash >
	inline void escrever(const std::unordered_map < TipoDados1, TipoDados2, EstruturaHash > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &relacao : mapa)
		{
			this->escrever(relacao.first);
			this->escrever(relacao.second);
		}
	}

	// Método usado para a escrita de uma árvore de dados no arquivo.
	template < typename TipoDados >
	inline void escrever(const std::set < TipoDados > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &dado : mapa)
		{
			this->escrever(dado);
		}
	}

	// Método usado para a escrita de uma árvore de dados no arquivo, com uma hash específica, no arquivo.
	template < typename TipoDados, typename EstruturaHash >
	inline void escrever(const std::set < TipoDados, EstruturaHash > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &dado : mapa)
		{
			this->escrever(dado);
		}
	}

	// Método usado para a escrita de uma árvore de dados sem ordem no arquivo.
	template < typename TipoDados >
	inline void escrever(const std::unordered_set < TipoDados > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &dado : mapa)
		{
			this->escrever(dado);
		}
	}

	// Método usado para a escrita de uma árvore de dados sem ordem no arquivo, com uma hash específica, no arquivo.
	template < typename TipoDados, typename EstruturaHash >
	inline void escrever(const std::unordered_set < TipoDados, EstruturaHash > &mapa)
	{
		this->escrever(mapa.size());

		for (const TipoDados &dado: mapa)
		{
			this->escrever(dado);
		}
	}
	
	// Método usado para a escrita de um tempo no arquivo.
	template < typename TipoDados, typename Periodo >
	inline void escrever(const std::chrono::duration < TipoDados, Periodo > &tempo)
	{
		this->escrever(tempo.count());
	}

	// Método usado para forçar a escrita dos dados contidos na memória principal.
	void escrever();

	/*
	 * Método usado para saber a distância, em bytes, do início do arquivo até a posição atual do cursor do manipulador.
	 * Cursores de leitura e escrita são unificados.
	 * Retorna a distância supracitada.
	 */
	unsigned long long int obterPosicao();

	/*
	 * Método usado para saber o nome do arquivo, podendo conter diretório que ele se encontra.
	 */
	std::string obterCaminho() const;

	/*
	 * Método usado para definir a distância, em bytes, do cursor do manipulador até o início do arquivo.
	 * Cursores de leitura e escrita são unificados.
	 */
	void definirPosicao(const size_t deslocamento, const int origem);

	/*
	 * Método usado para saber se o cursor do manipulador se encontra no fim do arquivo.
	 * Retorno: true, se chegou ao fim. false, caso contrário.
	 */
	bool fimArquivo() const;

	/*
	 * Construtor da classe Arquivo.
	 * Adicionalmente, o arquivo é aberto.
	 */
	explicit Arquivo(const std::string &caminho, const int modoAbertura = std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	// Construtor padrão da classe Arquivo.
	Arquivo() noexcept
	{
	}

	/*
	 * Destrutor da classe Arquivo.
	 * O usuário não será avisado sobre qualquer falha envolvendo o fechamento do arquivo.
	 */
	virtual ~Arquivo() noexcept;
};

#endif