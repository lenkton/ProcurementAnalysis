#include "windows.h"
#include "Shldisp.h"

int Unzip() {
	HRESULT a = OleInitialize(NULL);

	char cInPath[] = "C:\\NewDir1\\purchasedocs\\purchasedoc_Permskij_kraj_2014050100_2014060100_369.xml.zip\\fcsPurchaseDocsIP_0156100012814000005_910240.xml";
	char cOutPath[] = "C:\\NewDir1\\purchasedocs\\1";
	int nLen;
	BSTR bstrZip, bstrFol;
	HRESULT hres;
	long lCount;

	//OLECHAR O;


	

	IShellDispatch* pSD = NULL;
	//Интерфейсы IID_Folder
	Folder* pZipFolder = NULL;
	Folder* pOutFolder = NULL;
	//Интерфейс IID_FolderItems
	FolderItems* pFolderItems = NULL;

	hres = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IShellDispatch, (LPVOID*)& pSD);

	if (SUCCEEDED(hres))
	{
		VARIANT vZipDir, vOutDir, vOptions;

		VariantInit(&vZipDir);
		vZipDir.vt = VT_BSTR;
		nLen = (strlen(cInPath) + 1) * sizeof(WCHAR);
		bstrZip = SysAllocStringByteLen(NULL, nLen);

		MultiByteToWideChar(CP_ACP, 0, cInPath, -1, bstrZip, nLen);
		vZipDir.bstrVal = bstrZip;

		VariantInit(&vOutDir);
		vOutDir.vt = VT_BSTR;
		nLen = (strlen(cOutPath) + 1) * sizeof(WCHAR);
		bstrFol = SysAllocStringByteLen(NULL, nLen);

		MultiByteToWideChar(CP_ACP, 0, cOutPath, -1, bstrFol, nLen);
		vOutDir.bstrVal = bstrFol;

		if (SUCCEEDED(pSD->NameSpace(vOutDir, &pOutFolder)))
		{
			//Мы можем получить количество файлов в ZIP папке
			if (SUCCEEDED(pOutFolder->Items(&pFolderItems)))
			{
				pFolderItems->get_Count(&lCount);
			}
			//Опции
			//4 Do not display a progress dialog box.  
			//8 Give the file being operated on a new name in a move, copy, or rename operation if a file with the target name already exists.  
			//16 Respond with "Yes to All" for any dialog box that is displayed.  
			//64 Preserve undo information, if possible.
			//128 Perform the operation on files only if a wildcard file name (*.*) is specified.  
			//256 Display a progress dialog box but do not show the file names.  
			//512 Do not confirm the creation of a new directory if the operation requires one to be created.  
			//1024 Do not display a user interface if an error occurs.  
			//2048 Version 4.71. Do not copy the security attributes of the file.
			//4096 Only operate in the local directory. Don't operate recursively into subdirectories.
			//9182 Version 5.0. Do not copy connected files as a group. Only copy the specified files.

			VariantInit(&vOptions);
			vOptions.vt = VT_I4;
			vOptions.lVal = 0;

			hres = pOutFolder->CopyHere(vZipDir, vOptions);

			pOutFolder->Release();

			VariantClear(&vOptions);
		}

		pSD->Release();

		//SysFreeString(bstrZip);
		VariantClear(&vZipDir);

		//SysFreeString(bstrFol);
		VariantClear(&vOutDir);


	}
	return 0;
}