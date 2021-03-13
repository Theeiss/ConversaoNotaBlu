#pragma once
#include "stdafx.h"

class CBase
{
public:
	virtual void TrataDados(){};
};

class CDadosTomador : public CBase
{
public:
	CString m_sCNPJ;
	CString m_sRazaoSocial;
	CString m_sLogradouro;
	CString m_sEndereco;
	CString m_sNumero;
	CString m_sComplemento;
	CString m_sMunicipio;
	CString m_sEstado;
	CString m_sIndicadorCPFCNPJ;

	void Limpa()
	{
		m_sCNPJ.Empty();
		m_sRazaoSocial.Empty();
		m_sLogradouro.Empty();
		m_sEndereco.Empty();
		m_sNumero.Empty();
		m_sComplemento.Empty();
		m_sMunicipio.Empty();
		m_sEstado.Empty();
		m_sIndicadorCPFCNPJ.Empty();
	}

	void TrataDados()
	{
		bool bEhSoNumero = true;
		for(long l=0; l < m_sNumero.GetLength(); ++l)
		{
			if(m_sNumero[l] < 48 || m_sNumero[l] > 57)
			{
				bEhSoNumero = false;
			}
		}

		if( !bEhSoNumero ) // permitir apenas número no nº do endereço
		{
			m_sNumero.Empty();
		}

		if( m_sMunicipio.IsEmpty() && m_sEstado.IsEmpty() ) // Se não tiver municipio e estado, o default é blumenau - sc
		{
			m_sMunicipio = "Blumenau";
			m_sEstado = "SC";
		}
	}
};

class CDadosNFSE : public CBase
{
public:
	CString m_sNumeroNFSE;
	CString m_sDataEmissao;
	CString m_sValorTotal;
	CString m_sSituacao;
	CString m_sComplemento;
	CString m_sNFSECancelada;

	void Limpa()
	{
		m_sNumeroNFSE.Empty();
		m_sDataEmissao.Empty();
		m_sValorTotal.Empty();
		m_sSituacao.Empty();
		m_sComplemento.Empty();
		m_sNFSECancelada.Empty();
	}
};

class CDadosCFOP : public CBase
{
public:
	CString m_sValorTotal;
	CString m_sAliquotaISS;
	CString m_sValorISS;
	CString m_sOutrasISS;
	CString m_sBaseCalculoISS;

	void Limpa()
	{
		m_sValorTotal.Empty();
		m_sAliquotaISS.Empty();
		m_sValorISS.Empty();
		m_sOutrasISS.Empty();
		m_sBaseCalculoISS.Empty();
	}

	void SetZero()
	{
		m_sValorTotal = "0000000000000,00";
		m_sAliquotaISS = "0000000000000,00";
		m_sValorISS = "0000000000000,00";
		m_sOutrasISS = "0000000000000,00";
		m_sBaseCalculoISS = "0000000000000,00";
	}
};

class CDadosItemNFSE : public CBase
{
public:
	CString m_sCodigoServico;
	CString m_sValorTotal;

	void Limpa()
	{
		m_sCodigoServico.Empty();
		m_sValorTotal.Empty();
	}

	void TrataDados()
	{
		//Remover '0' à esquerda do código do serviço.
		long lCodServico = atol(m_sCodigoServico);
		_ltoa_s(lCodServico, m_sCodigoServico.GetBuffer(10), 10, 10);
		m_sCodigoServico.ReleaseBuffer();
	}
};

class CDadosImpostosRetidos : public CBase
{
public:
	CString m_sBaseCalculo;
	CString m_sValor;

	virtual void Limpa()
	{
		m_sBaseCalculo = "0,00";
		m_sValor = "0,00";
	}

	virtual void TrataDados()
	{
		if(atoi(m_sValor) == 0)
		{
			Limpa();
		}
	}
};

class CDadosISSRetido : public CDadosImpostosRetidos
{
public:
	CString m_sAliquota;

	virtual void Limpa()
	{
		CDadosImpostosRetidos::Limpa();

		m_sAliquota.Empty();
	}
};

class CDados
{
public:
	CDadosTomador	m_DadosTomador;
	CDadosNFSE		m_DadosNFSE;
	CDadosCFOP		m_DadosCFOP;
	CDadosItemNFSE	m_DadosItem;

	CDadosImpostosRetidos	m_DadosINSS;
	CDadosImpostosRetidos	m_DadosIRRF;
	CDadosImpostosRetidos	m_DadosPIS;
	CDadosImpostosRetidos	m_DadosCOFINS;
	CDadosImpostosRetidos	m_DadosCSLL;
	CDadosISSRetido			m_DadosISS;

	void Limpa()
	{
		m_DadosTomador.Limpa();
		m_DadosNFSE.Limpa();
		m_DadosCFOP.Limpa();
		m_DadosItem.Limpa();

		m_DadosINSS.Limpa();
		m_DadosIRRF.Limpa();
		m_DadosPIS.Limpa();
		m_DadosCOFINS.Limpa();
		m_DadosCSLL.Limpa();
		m_DadosISS.Limpa();
	};

	void TrataDados()
	{
		if( m_DadosNFSE.m_sSituacao == "C" )
		{ // Se a nota está cancelada, então zera os impostos retidos.
			m_DadosINSS.Limpa();
			m_DadosIRRF.Limpa();
			m_DadosPIS.Limpa();
			m_DadosCOFINS.Limpa();
			m_DadosCSLL.Limpa();
			m_DadosISS.Limpa();

			//Zerar dados do lançamento CFOP
			m_DadosCFOP.SetZero();

			// e o valor total da nota
			m_DadosItem.m_sValorTotal = "0000000000000,00";
		}

		m_DadosTomador.TrataDados();
		m_DadosNFSE.TrataDados();
		m_DadosCFOP.TrataDados();
		m_DadosItem.TrataDados();

		m_DadosINSS.TrataDados();
		m_DadosIRRF.TrataDados();
		m_DadosPIS.TrataDados();
		m_DadosCOFINS.TrataDados();
		m_DadosCSLL.TrataDados();
		m_DadosISS.TrataDados();
	}
};
