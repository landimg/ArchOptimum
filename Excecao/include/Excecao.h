#ifndef _EXCECAO_H_
#define _EXCECAO_H_

#include <exception>

class Excecao
{
public:
	template < typename TipoExcecao = std::exception >
	[[noreturn]] static inline void lancar()
	{
		throw TipoExcecao();
	}

	template < typename TipoExcecao = std::exception >
	[[noreturn]] static inline void lancar(const std::string &mensagem)
	{
		throw TipoExcecao(mensagem.c_str());
	}

	Excecao() = delete;
	Excecao(const Excecao &) = delete;
	Excecao(const Excecao &&) = delete;
	Excecao &operator=(const Excecao &) = delete;
	Excecao &operator=(const Excecao &&) = delete;
};

#endif