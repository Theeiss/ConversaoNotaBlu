#include "stdafx.h"
#include "InterpretadorNFSE.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CInterpretadorNFSE Interpretador;
	CString sFile = (char*) argv[1]; // O segundo argumento é o nome do arquivo.
	CFile file;

	file.Open(sFile, CFile::modeRead);

	file.Seek(0, CFile::begin);
	CString sDadosArq;

	file.Read(sDadosArq.GetBuffer(file.GetLength() + 1), file.GetLength());
	sDadosArq.ReleaseBuffer();

	file.Close();
	/*cout << sDadosArq << endl;*/

	bool bTratou = true;
	while(true && bTratou)
	{
		int iFimLinha = sDadosArq.Find('\n', 0);
		if(iFimLinha == -1)
			break;

		CString sLinha = sDadosArq.Left(iFimLinha);
		sDadosArq.Delete(0, iFimLinha+1);

		switch(sLinha[0])
		{
		case '1':
			sLinha.Delete(0);
			bTratou = Interpretador.TrataLinhaAbertura(sLinha);
			break;

		case '2':
			sLinha.Delete(0);
			bTratou = Interpretador.TrataLinhaDetalhes(sLinha);
			Interpretador.FinalizaLinha();
			break;
		}

		/*cout << sLinha << endl;*/
	}

	if(bTratou)
	{
		int iPos = sFile.ReverseFind('.');
		int iQtdChar = sFile.GetLength()-iPos;
		CString sExtensao = sFile.Right(iQtdChar);

		sFile.Delete(iPos, iQtdChar);

		sFile += "Convertido";
		sFile += sExtensao;

		Interpretador.ToArquivo(sFile);
	}
	system("pause");
}

