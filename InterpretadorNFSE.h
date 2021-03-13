#pragma once
#include "Dados.h"

class CInterpretadorNFSE
{
public:
	bool TrataLinhaAbertura(CString& sLinha);
	bool TrataLinhaDetalhes(CString& sLinha);

	CString TrataDataEmissao(CString sDataEmissao);
	void ToArquivo(CString sNomeSaida);

	void FinalizaLinha();

	CString m_sDataFinal;
	CDados m_Dados;

	CArray<CDados> m_arrDados;
};

