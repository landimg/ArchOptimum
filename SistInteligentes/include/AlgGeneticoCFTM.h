#ifndef _ALG_GENETICO_CFTM_H_
#define _ALG_GENETICO_CFTM_H_

#include "AlgGenetico.h"

class AlgGeneticoCFTM :
	public AlgGenetico
{
private:
	std::string localGravacao;

	inline void gerarPopulacao() noexcept;
	inline void genocidio() noexcept;
	static inline bool atualizarFilho(Avaliacao *pai, Avaliacao *mae, const Avaliacao &filho) noexcept;

public:
	void otimizar() noexcept override;

	AlgGeneticoCFTM(long double(*funcaoAvaliacao)(const std::vector < long double > &valores));

	AlgGeneticoCFTM(const std::string &localGravacao, const size_t geracoesSemMelhora, const long long int tempoExecucao,
		const std::vector < std::array < long double, 2 > > &intervalos,
		const std::vector < TipoVariavel > &tiposVariaveis,
		long double(*funcaoAvaliacao)(const std::vector < long double > &valores));
	virtual ~AlgGeneticoCFTM();
};

#endif