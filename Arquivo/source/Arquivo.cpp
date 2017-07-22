#include "../include/Arquivo.h"

void Arquivo::validarAcao() const
{
	if (this->arquivo.bad())
	{
		Excecao::lancar("O arquivo está corrompido, ou não pôde ser aberto.");
	}
}

void Arquivo::ler(void *p, const size_t qtdBytes)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.read(static_cast < char * >(p), qtdBytes);

	this->validarAcao();
}

void Arquivo::ler(bool &info)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.read(reinterpret_cast < char * >(&info), sizeof(bool));

	this->validarAcao();
}

void Arquivo::ler(unsigned char &caractere)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.read(reinterpret_cast < char * > (&caractere), sizeof(unsigned char));

	this->validarAcao();
}

void Arquivo::ler(char &caractere)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.read(reinterpret_cast < char * > (&caractere), sizeof(char));

	this->validarAcao();
}

void Arquivo::ler(unsigned char *string)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	char aux;
	size_t indice = 0;

	//Leitura de todos os bytes, até encontrar um '\n', EOF, ou '\0'.
	while ((aux = this->arquivo.get()) != '\n' && aux != EOF && aux != '\0')
	{
		this->validarAcao();

		string[indice] = static_cast < unsigned char > (aux);
		indice++;
	}

	string[indice] = 0;
}

void Arquivo::ler(char *string)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	char aux;
	size_t indice = 0;

	//Leitura de todos os bytes, até encontrar um '\n', EOF, ou '\0'.
	while ((aux = this->arquivo.get()) != '\n' && aux != EOF && aux != '\0')
	{
		this->validarAcao();

		string[indice] = aux;
		indice++;
	}

	string[indice] = 0;
}

void Arquivo::ler(std::string &string)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	char aux;

	//Leitura de todos os bytes, até encontrar um '\n', EOF, ou '\0'.
	while ((aux = this->arquivo.get()) != '\n' && aux != EOF && aux != '\0')
	{
		this->validarAcao();

		string.push_back(aux);
	}
}

void Arquivo::ler(unsigned int &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char * >(&numero), sizeof(unsigned int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(int &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char* >(&numero), sizeof(int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(unsigned long int &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast<char*>(&numero), sizeof(long));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(long int &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast<char*>(&numero), sizeof(long));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(unsigned long long int &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char * >(&numero), sizeof(unsigned long long int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(long long int &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char * >(&numero), sizeof(long long int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(float &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char * >(&numero), sizeof(float));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(double &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char * >(&numero), sizeof(double));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::ler(long double &numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.read(reinterpret_cast < char * >(&numero), sizeof(long double));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo >> numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const void *p, const size_t qtdBytes)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.write(static_cast<const char *>(p), qtdBytes);

	this->validarAcao();
}

void Arquivo::escrever(const bool info)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.write(reinterpret_cast<const char*>(&info), sizeof(bool));

	this->validarAcao();
}

void Arquivo::escrever(const unsigned char caractere)
{
	this->escrever(static_cast<const char>(caractere));
}

void Arquivo::escrever(const char caractere)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.put(caractere);

	this->validarAcao();
}

void Arquivo::escrever(const unsigned char *string, const bool barraZero)
{
	this->escrever(reinterpret_cast < const char * >(string), barraZero);
}

void Arquivo::escrever(const char *string, const bool barraZero)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.write(string, strlen(string) + (barraZero ? 1 : 0));

	this->validarAcao();
}

void Arquivo::escrever(const std::string &string, const bool barraZero)
{
	this->escrever(string.c_str(), barraZero);
}

void Arquivo::escrever(const unsigned short int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(unsigned short int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const short int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(short int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const unsigned int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(unsigned int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const unsigned long int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(unsigned long int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const long int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(long int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const unsigned long long int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(unsigned long long int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const long long int numero, const bool formato)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(long long int));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const float numero, const bool formato, const size_t numeroCasasDecimais)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(float));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << std::fixed << std::setprecision(numeroCasasDecimais) << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const double numero, const bool formato, const size_t numeroCasasDecimais)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(double));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << std::fixed << std::setprecision(numeroCasasDecimais) << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

void Arquivo::escrever(const long double numero, const bool formato, const size_t numeroCasasDecimais)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	if (formato)
	{
		this->arquivo.write(reinterpret_cast<const char*>(&numero), sizeof(long double));
	}

	else
	{
		int flagsArquivo = this->arquivo.flags();

		this->arquivo.flags(this->arquivo.flags() & 0xdf);

		this->arquivo << std::fixed << std::setprecision(numeroCasasDecimais) << numero;

		this->arquivo.flags(flagsArquivo);
	}

	this->validarAcao();
}

unsigned long long int Arquivo::obterPosicao()
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	unsigned long long int pos = this->arquivo.tellg();

	this->validarAcao();

	return pos;
}

std::string Arquivo::obterCaminho() const
{
	return this->caminho;
}

void Arquivo::definirPosicao(const size_t deslocamento, const int origem)
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.seekg(deslocamento, origem);
	this->validarAcao();
}

bool Arquivo::fimArquivo() const
{
	bool fim = this->arquivo.eof();

	this->validarAcao();

	return fim;
}

void Arquivo::abrir(const std::string &caminho, const int modoAbertura)
{
	if (this->arquivo.is_open())
	{
		Excecao::lancar("Um arquivo já está aberto.");
	}

	this->arquivo.open(caminho, modoAbertura | std::ios::in | std::ios::out);
	this->caminho = caminho;

	if (this->estaAberto() == false)
	{
		Excecao::lancar("Não foi possível abrir o arquivo.");
	}

	this->validarAcao();
}

void Arquivo::abrir(const int modoAbertura)
{
	if (this->arquivo.is_open())
	{
		Excecao::lancar("Um arquivo já está aberto.");
	}

	this->arquivo.open(this->caminho, modoAbertura | std::ios::in | std::ios::out);

	if (this->estaAberto() == false)
	{
		Excecao::lancar("Não foi possível abrir o arquivo.");
	}

	this->validarAcao();
}

void Arquivo::fechar()
{
	if (!this->arquivo.is_open())
	{
		Excecao::lancar("Não há um arquivo aberto.");
	}

	this->arquivo.close();

	this->validarAcao();
}

bool Arquivo::estaAberto() const
{
	return this->arquivo.is_open();
}

void Arquivo::escrever()
{
	this->arquivo.flush();

	this->validarAcao();
}

Arquivo::Arquivo(const std::string &caminho, const int modoAbertura)
{
	this->abrir(caminho, modoAbertura);
}

Arquivo::~Arquivo() noexcept
{
	if (this->arquivo.is_open())
	{
		this->arquivo.close();
	}
}
