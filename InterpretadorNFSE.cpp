#include "StdAfx.h"
#include "InterpretadorNFSE.h"

CString PegaCampo(CString& sLinha, long lCount)
{
	CString s = sLinha.Left(lCount);
	sLinha.Delete(0, lCount);
	s.Trim();
	return s;
}

bool CInterpretadorNFSE::TrataLinhaAbertura(CString& sLinha)
{
	/*Esta linha é identificada pelo primeiro caractere como sendo o número "1" e trará informações da empresa ao que o arquivo se refere.
	  Ao final da linha, constará a versão do arquivo. A versão atual será definida em VERSAO_NOTABLU*/

	CString sVersao = PegaCampo(sLinha, 3);
	if(sVersao != VERSAO_NOTABLU)
	{
		cout << "Este aplicativo foi desenvolvido para a versão 002 do layout NotaBlu." << endl;
		return false;
	}

	PegaCampo(sLinha, 23);

	m_sDataFinal = PegaCampo(sLinha, 8);

	return true;
}

bool CInterpretadorNFSE::TrataLinhaDetalhes(CString& sLinha)
{
	/* Esta linha contém as informações das NFSE's emitidas como o nº da mesma e identfiação do tomador do serviço */

	m_Dados.m_DadosNFSE.m_sNumeroNFSE = PegaCampo(sLinha, 15);

	m_Dados.m_DadosNFSE.m_sDataEmissao = TrataDataEmissao(PegaCampo(sLinha, 14));

	PegaCampo(sLinha, 568); //Remove informações desprezíveis.

	m_Dados.m_DadosNFSE.m_sSituacao = PegaCampo(sLinha, 1);

	PegaCampo(sLinha, 31); //Remove informações desprezíveis.

	m_Dados.m_DadosNFSE.m_sValorTotal = PegaCampo(sLinha, 15);
	m_Dados.m_DadosNFSE.m_sValorTotal.Insert(13, ',');

	PegaCampo(sLinha, 15); //Remove informações desprezíveis.

	m_Dados.m_DadosItem.m_sCodigoServico = PegaCampo(sLinha, 8);
	m_Dados.m_DadosItem.m_sValorTotal = m_Dados.m_DadosNFSE.m_sValorTotal;

	m_Dados.m_DadosCFOP.m_sAliquotaISS = PegaCampo(sLinha, 5);
	m_Dados.m_DadosCFOP.m_sAliquotaISS.Insert(3, ',');

	m_Dados.m_DadosCFOP.m_sValorISS = PegaCampo(sLinha, 15);
	m_Dados.m_DadosCFOP.m_sValorISS.Insert(13, ',');

	if(m_Dados.m_DadosCFOP.m_sAliquotaISS == "0000000000000,00")
	{
		m_Dados.m_DadosCFOP.m_sBaseCalculoISS = m_Dados.m_DadosNFSE.m_sValorTotal;
	}
	else
	{
		m_Dados.m_DadosCFOP.m_sBaseCalculoISS = "0000000000000,00";
	}

	if(m_Dados.m_DadosCFOP.m_sValorISS == "0000000000000,00")
	{
		m_Dados.m_DadosCFOP.m_sOutrasISS = m_Dados.m_DadosNFSE.m_sValorTotal;
	}
	else
	{
		m_Dados.m_DadosCFOP.m_sOutrasISS = "0000000000000,00";
	}

	m_Dados.m_DadosCFOP.m_sValorTotal = m_Dados.m_DadosNFSE.m_sValorTotal;

	PegaCampo(sLinha, 15);//Remove informações desprezíveis.

	char cISSRetido = PegaCampo(sLinha, 1)[0];

	if(cISSRetido == '1')
	{
		m_Dados.m_DadosISS.m_sValor =		m_Dados.m_DadosCFOP.m_sValorISS;
		m_Dados.m_DadosISS.m_sBaseCalculo = m_Dados.m_DadosCFOP.m_sValorTotal;
		m_Dados.m_DadosISS.m_sAliquota =	m_Dados.m_DadosCFOP.m_sAliquotaISS;
	}

	m_Dados.m_DadosTomador.m_sIndicadorCPFCNPJ = PegaCampo(sLinha, 1);

	m_Dados.m_DadosTomador.m_sCNPJ = PegaCampo(sLinha, 14);

	if(m_Dados.m_DadosTomador.m_sIndicadorCPFCNPJ == "1") //CPF
	{
		m_Dados.m_DadosTomador.m_sCNPJ.Delete(0, 3);
	}

	PegaCampo(sLinha, 30); //Remove informações desprezíveis.

	m_Dados.m_DadosTomador.m_sRazaoSocial = PegaCampo(sLinha, 115);

	m_Dados.m_DadosTomador.m_sLogradouro = PegaCampo(sLinha, 3);

	m_Dados.m_DadosTomador.m_sEndereco = PegaCampo(sLinha, 100);
	m_Dados.m_DadosTomador.m_sNumero = PegaCampo(sLinha, 10);
	m_Dados.m_DadosTomador.m_sComplemento = PegaCampo(sLinha, 60);

	PegaCampo(sLinha, 72); //Remove informações desprezíveis.

	m_Dados.m_DadosTomador.m_sMunicipio = PegaCampo(sLinha, 50);
	m_Dados.m_DadosTomador.m_sEstado = PegaCampo(sLinha, 2);

	PegaCampo(sLinha, 88); //Remove informações desprezíveis

	m_Dados.m_DadosCOFINS.m_sValor = PegaCampo(sLinha, 15);
	m_Dados.m_DadosCOFINS.m_sValor.Insert(13, ',');

	m_Dados.m_DadosCOFINS.m_sBaseCalculo = m_Dados.m_DadosNFSE.m_sValorTotal;

	m_Dados.m_DadosCSLL.m_sValor = PegaCampo(sLinha, 15);
	m_Dados.m_DadosCSLL.m_sValor.Insert(13, ',');

	m_Dados.m_DadosCSLL.m_sBaseCalculo = m_Dados.m_DadosNFSE.m_sValorTotal;

	m_Dados.m_DadosINSS.m_sValor = PegaCampo(sLinha, 15);
	m_Dados.m_DadosINSS.m_sValor.Insert(13, ',');

	m_Dados.m_DadosINSS.m_sBaseCalculo = m_Dados.m_DadosNFSE.m_sValorTotal;

	m_Dados.m_DadosIRRF.m_sValor = PegaCampo(sLinha, 15);
	m_Dados.m_DadosIRRF.m_sValor.Insert(13, ',');

	m_Dados.m_DadosIRRF.m_sBaseCalculo = m_Dados.m_DadosNFSE.m_sValorTotal;

	m_Dados.m_DadosPIS.m_sValor = PegaCampo(sLinha, 15);
	m_Dados.m_DadosPIS.m_sValor.Insert(13, ',');

	m_Dados.m_DadosPIS.m_sBaseCalculo = m_Dados.m_DadosNFSE.m_sValorTotal;

	m_Dados.m_DadosNFSE.m_sComplemento = PegaCampo(sLinha, sLinha.GetLength());
	return true;
}

void CInterpretadorNFSE::ToArquivo(CString sNomeSaida)
{
	CFile file;
	file.Open(sNomeSaida, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	CString sLinha;
	for(long l=0; l < m_arrDados.GetCount(); ++l)
	{
		CDados Linha = m_arrDados[l];
		Linha.TrataDados();

		//Dados do Tomador
		sLinha.Format("|1|%s|%s|%s|%s|%s|%s|%s|%s|%s\r\n", Linha.m_DadosTomador.m_sIndicadorCPFCNPJ, Linha.m_DadosTomador.m_sCNPJ, Linha.m_DadosTomador.m_sRazaoSocial,
			Linha.m_DadosTomador.m_sLogradouro, Linha.m_DadosTomador.m_sEndereco, Linha.m_DadosTomador.m_sNumero,
			Linha.m_DadosTomador.m_sComplemento, Linha.m_DadosTomador.m_sMunicipio, Linha.m_DadosTomador.m_sEstado);

		file.Write(sLinha, sLinha.GetLength());

		//Dados da NF
		sLinha.Format("|2|%s|%s|%s|%s|%s|\"%s\"|%s\r\n", Linha.m_DadosNFSE.m_sNumeroNFSE, Linha.m_DadosNFSE.m_sDataEmissao, Linha.m_DadosNFSE.m_sValorTotal, Linha.m_DadosNFSE.m_sSituacao,
			Linha.m_DadosNFSE.m_sSituacao == "C" ? "1" : "0", Linha.m_DadosNFSE.m_sComplemento, Linha.m_DadosTomador.m_sCNPJ);

		file.Write(sLinha, sLinha.GetLength());

		//Dados da CFOP
		Linha.m_DadosISS.TrataDados(); // Como os dados do ISS já serão usados nesta linha, temos que tratar os dados já.
		sLinha.Format("|3|%s|%s|%s|%s|%s\r\n", Linha.m_DadosCFOP.m_sValorTotal, Linha.m_DadosCFOP.m_sBaseCalculoISS, Linha.m_DadosCFOP.m_sAliquotaISS, Linha.m_DadosCFOP.m_sValorISS, Linha.m_DadosCFOP.m_sOutrasISS);

		file.Write(sLinha, sLinha.GetLength());

		//Itens da NF
		sLinha.Format("|4|%s|%s\r\n", Linha.m_DadosItem.m_sCodigoServico, Linha.m_DadosItem.m_sValorTotal);

		file.Write(sLinha, sLinha.GetLength());

		//Impostos Retidos
		Linha.m_DadosINSS.TrataDados();
		Linha.m_DadosIRRF.TrataDados();
		Linha.m_DadosPIS.TrataDados();
		Linha.m_DadosCOFINS.TrataDados();
		Linha.m_DadosCSLL.TrataDados();
		sLinha.Format("|5|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\r\n", Linha.m_DadosINSS.m_sBaseCalculo, Linha.m_DadosINSS.m_sValor, Linha.m_DadosIRRF.m_sBaseCalculo, Linha.m_DadosIRRF.m_sValor,
			Linha.m_DadosPIS.m_sBaseCalculo, Linha.m_DadosPIS.m_sValor, Linha.m_DadosCOFINS.m_sBaseCalculo, Linha.m_DadosCOFINS.m_sValor, 
			Linha.m_DadosCSLL.m_sBaseCalculo, Linha.m_DadosCSLL.m_sValor, Linha.m_DadosISS.m_sBaseCalculo, Linha.m_DadosISS.m_sValor, Linha.m_DadosISS.m_sAliquota);

		file.Write(sLinha, sLinha.GetLength());
	}

	file.Close();
}

CString CInterpretadorNFSE::TrataDataEmissao(CString sDataEmissao)
{
	CString sDataFinal, sAnoFinal, sAnoEmissao, sMesFinal, sMesEmissao, sDiaFinal, sDiaEmissao;
	CString sAnoRetorno, sMesRetorno, sDiaRetorno;

	sDataFinal = m_sDataFinal;

	sAnoFinal = sDataFinal.Left(4);
	sDataFinal.Delete(0, 4);

	sAnoEmissao = sDataEmissao.Left(4);
	sDataEmissao.Delete(0, 4);

	sMesFinal = sDataFinal.Left(2);
	sDataFinal.Delete(0, 2);

	sMesEmissao = sDataEmissao.Left(2);
	sDataEmissao.Delete(0, 2);

	sDiaFinal = sDataFinal.Left(2);
	sDataFinal.Delete(0, 2);

	sDiaEmissao = sDataEmissao.Left(2);
	sDataEmissao.Delete(0, 2);

	sAnoRetorno = sAnoEmissao;
	sMesRetorno = sMesEmissao;
	sDiaRetorno = sDiaEmissao;

	if(sAnoEmissao > sAnoFinal)
	{
		sAnoRetorno = sAnoFinal;
		sMesRetorno = sMesFinal;
		sDiaRetorno = sDiaFinal;
	}
	else if(sMesEmissao > sMesFinal && sAnoEmissao == sAnoFinal)
	{
		sMesRetorno = sMesFinal;
		sDiaRetorno = sDiaFinal;
	}
	else if(sDiaEmissao > sDiaFinal && sMesEmissao == sMesFinal && sAnoEmissao == sAnoFinal)
	{
		sDiaRetorno = sDiaFinal;
	}

	return sAnoRetorno + sMesRetorno + sDiaRetorno;
}

void CInterpretadorNFSE::FinalizaLinha()
{
	m_arrDados.Add(m_Dados);
	m_Dados.Limpa();
}