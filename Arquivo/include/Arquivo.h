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

// Classe utilizada para manipula��o de dados mantidos em mem�ria persistente.
class Arquivo
{
private:
	// Nome do arquivo, podendo conter diret�rio que ele se encontra.
	std::string caminho;

	// Manipulador para os dados contidos em mem�ria persistente.
	std::fstream arquivo;

	// Verifica��o da integridade do arquivo, ap�s a realiza��o de qualquer a��o.
	inline void validarAcao() const;

public:
	/*
	 * M�todo usado para a abertura de arquivos.
	 * A abertura � feita compulsoriamente para leitura e escrita.
	 *
	 * Os modos de abertura est�o atrelados � ios_base:
	 *		app --> Faz um "seek" para o fim do arquivo a cada escrita.
	 *		ate --> Define o fim do arquivo como posi��o inicial.
	 *		binary --> Define o arquivo como bin�rio.
	 *		in --> Define o arquivo como entrada.
	 *		out --> Define o arquivo como sa�da.
	 *		trunc --> Apaga o conte�do do arquivo.
	 */
	void abrir(const std::string &caminho, const int modoAbertura = std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	/*
	 * M�todo usado para a abertura de arquivos.
	 * A abertura � feita compulsoriamente para leitura e escrita.
	 * Caso algum arquivo j� tenho sido aberto e fechado, utiliza-se o �ltimo caminho dado.
	 *
	 * Os modos de abertura est�o atrelados � ios_base:
	 *		app --> Faz um "seek" para o fim do arquivo a cada escrita.
	 *		ate --> Define o fim do arquivo como posi��o inicial.
	 *		binary --> Define o arquivo como bin�rio.
	 *		in --> Define o arquivo como entrada.
	 *		out --> Define o arquivo como sa�da.
	 *		trunc --> Apaga o conte�do do arquivo.
	 */
	void abrir(const int modoAbertura = std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	// M�todo usado para o fechamento do arquivo.
	void fechar();

	// M�todo usado para verificar se h� algum arquivo aberto para este objeto.
	bool estaAberto() const;

	/*
	 * M�todo usado para a leitura de quaisquer informa��es e armazen�-las em p.
	 * � obrigat�rio que a aloca��o de mem�ria de p tenha sido feita previamente.
	 */
	void ler(void *p, const size_t qtdBytes);

	// M�todo usado para a leitura de uma informa��o booleana.
	void ler(bool &info);

	// M�todo usado para a leitura de um caractere.
	void ler(unsigned char &caractere);

	// M�todo usado para a leitura de um caractere.
	void ler(char &caractere);

	/*
	 * M�todo usado para a leitura de uma linha inteira do arquivo.
	 * Caracteres de parada: '\n', EOF e '\0'.
	 * � obrigat�rio que a aloca��o de mem�ria de string tenha sido feita previamente.
	 */
	void ler(unsigned char *string);

	/*
	 * M�todo usado para a leitura de uma linha inteira do arquivo.
	 * Caracteres de parada: '\n', EOF e '\0'.
	 * � obrigat�rio que a aloca��o de mem�ria de string tenha sido feita previamente.
	 */
	void ler(char *string);

	/*
	 * M�todo usado para a leitura de uma linha inteira do arquivo.
	 * Caracteres de parada: '\n', EOF e '\0'.
	 */
	void ler(std::string &string);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo unsigned int do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(unsigned int &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo int do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(int &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo unsigned long int do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(unsigned long int &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo long int do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(long int &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo unsigned long long do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(unsigned long long int &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo long long do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(long long int &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo float do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(float &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo double do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(double &numero, const bool formato = true);

	/*
	 * M�todo usado para a leitura de um n�mero de tipo long double do arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void ler(long double &numero, const bool formato = true);

	// M�todo usado para a leitura de um dado de tipo gen�rico do arquivo.
	template < typename TipoDado >
	inline void ler(TipoDado &dado)
	{
		if (!this->arquivo.is_open())
		{
			Excecao::lancar("N�o h� um arquivo aberto.");
		}

		char *aux = new char[sizeof(TipoDado)];

		this->arquivo.read(aux, sizeof(TipoDado));

		memmove_s(&dado, sizeof(TipoDado), aux, sizeof(TipoDado));

		delete[] aux;

		this->validarAcao();
	}

	// M�todo usado para a leitura de uma ordem de dados gen�ricos do arquivo.
	template < typename TipoDados, size_t qtdElementos >
	inline void ler(std::array < TipoDados, qtdElementos > &vetor)
	{
		for (size_t i = 0; i < qtdElementos; i++)
		{
			this->ler(vetor[i]);
		}
	}

	// M�todo usado para a leitura de um vetor de dados gen�ricos do arquivo.
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

	// M�todo usado para a leitura de um par de dados gen�ricos do arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void ler(std::pair < TipoDados1, TipoDados2 > &par)
	{
		this->ler(par.first);
		this->ler(par.second);
	}

	// M�todo usado para a leitura de uma tabela hash do arquivo.
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

	// M�todo usado para a leitura de uma tabela hash do arquivo, com uma hash espec�fica.
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

	// M�todo usado para a leitura de uma tabela hash sem ordem do arquivo.
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

	// M�todo usado para a leitura de uma tabela hash sem ordem do arquivo, com uma hash espec�fica.
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

	// M�todo usado para a leitura de uma �rvore de dados do arquivo.
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

	// M�todo usado para a leitura de uma �rvore de dados do arquivo, com uma hash espec�fica.
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

	// M�todo usado para a leitura de uma �rvore de dados sem ordem do arquivo.
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

	// M�todo usado para a leitura de uma �rvore de dados sem ordem do arquivo, com uma hash espec�fica.
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

	// M�todo usado para a escrita de um tempo no arquivo.
	template < typename TipoDados, typename Periodo >
	inline void ler(std::chrono::duration < TipoDados, Periodo > &tempo)
	{
		TipoDados _tempo;

		this->ler(_tempo);

		tempo = std::chrono::duration < TipoDados, Periodo > (_tempo);
	}

	// M�todo usado para a escrita de quaisquer informa��es de p no arquivo.
	void escrever(const void *p, const size_t qtdBytes);

	// M�todo usado para a escrita de uma informa��o booleana no arquivo.
	void escrever(const bool info);

	// M�todo usado para a escrita de um caractere no arquivo.
	void escrever(const unsigned char caractere);

	// M�todo usado para a escrita de um caractere no arquivo.
	void escrever(const char caractere);

	/*
	 * M�todo usado para a escrita de uma string no arquivo.
	 * barraZero insere um caractere nulo ('/0') ao fim da string, se true.
	 */
	void escrever(const unsigned char *string, const bool barraZero = true);

	/*
	 * M�todo usado para a escrita de uma string no arquivo.
	 * barraZero insere um caractere nulo ('/0') ao fim da string, se true.
	 */
	void escrever(const char *string, const bool barraZero = true);

	/*
	 * M�todo usado para a escrita de uma string no arquivo.
	 * barraZero insere um caractere nulo ('/0') ao fim da string, se true.
	 */
	void escrever(const std::string &string, const bool barraZero = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo unsigned short int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const unsigned short int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo short int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const short int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo unsigned int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const unsigned int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo unsigned long int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const unsigned long int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo long int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const long int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo unsigned long long int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const unsigned long long int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo long long int no arquivo.
	 * formato: false - texto; true - bin�rio.
	 */
	void escrever(const long long int numero, const bool formato = true);

	/*
	 * M�todo usado para a escrita um n�mero de tipo float no arquivo.
	 * formato: false - texto; true - bin�rio.
	 * numeroCasasDecimais: quantas casas decimais, depois da v�rgula, ser�o gravadas no arquivo.
	 */
	void escrever(const float numero, const bool formato = true, const size_t numeroCasasDecimais = 5);

	/*
	 * M�todo usado para a escrita um n�mero de tipo double no arquivo.
	 * formato: false - texto; true - bin�rio.
	 * numeroCasasDecimais: quantas casas decimais, depois da v�rgula, ser�o gravadas no arquivo.
	 */
	void escrever(const double numero, const bool formato = true, const size_t numeroCasasDecimais = 5);

	/*
	 * M�todo usado para a escrita um n�mero de tipo double no arquivo.
	 * formato: false - texto; true - bin�rio.
	 * numeroCasasDecimais: quantas casas decimais, depois da v�rgula, ser�o gravadas no arquivo.
	 */
	void escrever(const long double numero, const bool formato = true, const size_t numeroCasasDecimais = 5);

	// M�todo usado para a escrita de um dado de tipo gen�rico no arquivo.
	template < typename TipoDado >
	inline void escrever(const TipoDado &dado)
	{
		if (!this->arquivo.is_open())
		{
			Excecao::lancar("N�o h� um arquivo aberto.");
		}

		this->arquivo.write(reinterpret_cast < const char * > (&dado), sizeof(TipoDado));

		this->validarAcao();
	}

	// M�todo usado para a escrita de uma ordem de dados gen�ricos no arquivo.
	template < typename TipoDados, size_t qtdElementos >
	inline void escrever(const std::array < TipoDados, qtdElementos > &vetor)
	{
		for (size_t i = 0; i < qtdElementos; i++)
		{
			this->escrever(vetor[i]);
		}
	}

	// M�todo usado para a escrita de um vetor de dados gen�ricos no arquivo.
	template < typename TipoDados >
	inline void escrever(const std::vector < TipoDados > &vetor)
	{
		this->escrever(vetor.size());

		for (size_t i = 0; i < vetor.size(); i++)
		{
			this->escrever(vetor[i]);
		}
	}

	// M�todo usado para a escrita de um par de dados gen�ricos no arquivo.
	template < typename TipoDados1, typename TipoDados2 >
	inline void escrever(const std::pair < TipoDados1, TipoDados2 > &par)
	{
		this->escrever(par.first);
		this->escrever(par.second);
	}

	// M�todo usado para a escrita de uma tabela hash no arquivo.
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

	// M�todo usado para a escrita de uma tabela hash no arquivo, com uma hash espec�fica.
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

	// M�todo usado para a escrita de uma tabela hash sem ordem no arquivo.
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

	// M�todo usado para a escrita de uma tabela hash sem ordem no arquivo, com uma hash espec�fica.
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

	// M�todo usado para a escrita de uma �rvore de dados no arquivo.
	template < typename TipoDados >
	inline void escrever(const std::set < TipoDados > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &dado : mapa)
		{
			this->escrever(dado);
		}
	}

	// M�todo usado para a escrita de uma �rvore de dados no arquivo, com uma hash espec�fica, no arquivo.
	template < typename TipoDados, typename EstruturaHash >
	inline void escrever(const std::set < TipoDados, EstruturaHash > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &dado : mapa)
		{
			this->escrever(dado);
		}
	}

	// M�todo usado para a escrita de uma �rvore de dados sem ordem no arquivo.
	template < typename TipoDados >
	inline void escrever(const std::unordered_set < TipoDados > &mapa)
	{
		this->escrever(mapa.size());

		for (const auto &dado : mapa)
		{
			this->escrever(dado);
		}
	}

	// M�todo usado para a escrita de uma �rvore de dados sem ordem no arquivo, com uma hash espec�fica, no arquivo.
	template < typename TipoDados, typename EstruturaHash >
	inline void escrever(const std::unordered_set < TipoDados, EstruturaHash > &mapa)
	{
		this->escrever(mapa.size());

		for (const TipoDados &dado: mapa)
		{
			this->escrever(dado);
		}
	}
	
	// M�todo usado para a escrita de um tempo no arquivo.
	template < typename TipoDados, typename Periodo >
	inline void escrever(const std::chrono::duration < TipoDados, Periodo > &tempo)
	{
		this->escrever(tempo.count());
	}

	// M�todo usado para for�ar a escrita dos dados contidos na mem�ria principal.
	void escrever();

	/*
	 * M�todo usado para saber a dist�ncia, em bytes, do in�cio do arquivo at� a posi��o atual do cursor do manipulador.
	 * Cursores de leitura e escrita s�o unificados.
	 * Retorna a dist�ncia supracitada.
	 */
	unsigned long long int obterPosicao();

	/*
	 * M�todo usado para saber o nome do arquivo, podendo conter diret�rio que ele se encontra.
	 */
	std::string obterCaminho() const;

	/*
	 * M�todo usado para definir a dist�ncia, em bytes, do cursor do manipulador at� o in�cio do arquivo.
	 * Cursores de leitura e escrita s�o unificados.
	 */
	void definirPosicao(const size_t deslocamento, const int origem);

	/*
	 * M�todo usado para saber se o cursor do manipulador se encontra no fim do arquivo.
	 * Retorno: true, se chegou ao fim. false, caso contr�rio.
	 */
	bool fimArquivo() const;

	/*
	 * Construtor da classe Arquivo.
	 * Adicionalmente, o arquivo � aberto.
	 */
	explicit Arquivo(const std::string &caminho, const int modoAbertura = std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	// Construtor padr�o da classe Arquivo.
	Arquivo() noexcept
	{
	}

	/*
	 * Destrutor da classe Arquivo.
	 * O usu�rio n�o ser� avisado sobre qualquer falha envolvendo o fechamento do arquivo.
	 */
	virtual ~Arquivo() noexcept;
};

#endif