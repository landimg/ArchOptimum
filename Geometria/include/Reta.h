#ifndef _RETA_H_
#define _RETA_H_

#include <opencv2/opencv.hpp>

#include "Vertice.h"
#include "Vetor.h"

class Reta :
	public Serializacao
{
protected:
	Vetor vetor;

public:
	Vertice vertice;

	explicit operator bool() const noexcept;

	const Vetor &obterVetor() const noexcept;

	bool operator==(const Reta &reta) const noexcept;
	bool operator!=(const Reta &reta) const noexcept;

	void serializar(Arquivo &arquivo) const override;
	void deserializar(Arquivo &arquivo) override;

	Reta();
	Reta(const cv::Point &vertice, const Vetor &vetor);
	Reta(const cv::Point3_ < long double > &vertice, const Vetor &vetor);
	Reta(const Vertice &vertice, const Vetor &vetor);
	virtual ~Reta() noexcept;
};

#endif