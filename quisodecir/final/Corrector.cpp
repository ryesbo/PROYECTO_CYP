/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) BRAYAN REYES ORTEGA, 321031411.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#include "cctype"
#include <algorithm>
#define depuracion 0

//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* fpDicc;
	char* buffer;
	long fileSize;
	int i, j, k;
	int existIndex;
	int frecuen;
	fopen_s(&fpDicc, szNombre, "rb");

	if (fpDicc != NULL)
	{
		// Obtiene el tama�o del archivo
		fseek(fpDicc, 0, SEEK_END);
		fileSize = ftell(fpDicc);
		fseek(fpDicc, 0, SEEK_SET);

		// Lee el archivo completo en memoria
		buffer = (char*)malloc(fileSize + 1);
		fread(buffer, 1, fileSize, fpDicc);
		buffer[fileSize] = '\0';

		fclose(fpDicc);

		// Procesa el contenido en memoria
		int indicepd = 0;
		int palabrauni = 0;
		char palabradetec[TAMTOKEN];

		for (i = 0; buffer[i] != '\0'; i++)
		{
			if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r' || buffer[i] == '.' || buffer[i] == ',' || buffer[i] == ';')
			{
				if (indicepd > 0)
				{
					palabradetec[indicepd] = '\0';

					for ( j = 0; j < indicepd; j++)
					{
						palabradetec[j] = tolower(palabradetec[j]);
					}

					// Verifica si la palabra ya existe en el array
					existIndex = -1;
					for ( k = 0; k < palabrauni; k++)
					{
						if (strcmp(szPalabras[k], palabradetec) == 0)
						{
							existIndex = k;
							break;
						}
					}

					if (existIndex != -1)
					{
						// La palabra ya existe, incrementa la estad�stica
						iEstadisticas[existIndex]++;
					}
					else
					{
						// Agrega la nueva palabra al array
						strcpy_s(szPalabras[palabrauni], TAMTOKEN, palabradetec);
						iEstadisticas[palabrauni] = 1;
						palabrauni++;
					}

					indicepd = 0;
				}
			}
			else
			{
				if (buffer[i] != '(' && buffer[i] != ')')
				{
					palabradetec[indicepd] = buffer[i];
					indicepd++;
				}
			}
		}

		// Actualiza iNumElementos con la cantidad de palabras �nicas
		iNumElementos = palabrauni;

		// Resto de la l�gica de procesamiento (ordenar y eliminar duplicados)
		bool ordenado = false;
		for ( j = 0; j < iNumElementos - 1 && ordenado == false; j++)
		{
			ordenado = true;
			for ( k = 0; k < iNumElementos - j - 1; k++)
			{
				if (strcmp(szPalabras[k], szPalabras[k + 1]) > 0)
				{
					ordenado = false;
					// Intercambia palabras
					char almatem[TAMTOKEN];
					strcpy_s(almatem, TAMTOKEN, szPalabras[k]);
					strcpy_s(szPalabras[k], TAMTOKEN, szPalabras[k + 1]);
					strcpy_s(szPalabras[k + 1], TAMTOKEN, almatem);

					// Intercambia frecuencias
					frecuen = iEstadisticas[k];
					iEstadisticas[k] = iEstadisticas[k + 1];
					iEstadisticas[k + 1] = frecuen;
				}
			}
		}

		// Elimina duplicados
		palabrauni = 0;
		for (k = 1; k < iNumElementos; k++)
		{
			if (strcmp(szPalabras[k], szPalabras[k - 1]) == 0)
			{
				iEstadisticas[palabrauni] += iEstadisticas[k];
			}
			else
			{
				palabrauni++;
				strcpy_s(szPalabras[palabrauni], TAMTOKEN, szPalabras[k]);
				iEstadisticas[palabrauni] = iEstadisticas[k];
			}
		}

		iNumElementos = palabrauni + 1;

		free(buffer);
	}
	else
	{
		if (depuracion == 1)
			printf("\nnopi no abre\n");
	}
}

/** {
	FILE* fpDicc;
	char linea[55555];
	char palabradetec[TAMTOKEN];
	char almatem[TAMTOKEN];
	int i, j, k;
	int indicepd;
	int palabrauni;
	int frecuen;
	iNumElementos = 0;
	//abre arch
	if (depuracion == 1)
	{
		printf(" % s", szNombre);
	}
	fopen_s(&fpDicc, szNombre, "r");
	if (fpDicc != NULL)
	{
		if (depuracion == 1)
		{
			printf("\nsipi si abre\n");
		}
		indicepd = 0;
		while (fgets(linea, sizeof(linea), fpDicc) != NULL) //(!feof(fpDicc))
		{
			//fgets(linea, sizeof(linea), fpDicc);

			for (i = 0; linea[i] != '\0'; i++)//for (i = 0; i < strlen(linea); i++)
			{
				if (linea[i] == ' ' || linea[i] == '\n' || linea[i] == '\t' || linea[i] == '\r' || linea[i] == '.' || linea[i] == ',' || linea[i] == ';')
				{
					if (indicepd > 0)
					{
						palabradetec[indicepd] = '\0';

						/*for (j = 0; j < indicepd; j++)
						{
							palabradetec[j] = tolower(palabradetec[j]);
						}*/

						/*/strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabradetec);
						iEstadisticas[iNumElementos] = 1;
						indicepd = 0;
						iNumElementos++;
					}
				}
				else
				{
					if (linea[i] != '(' && linea[i] != ')')
					{
						palabradetec[indicepd] = linea[i];
						indicepd++;
					}
				}
			}
			if (indicepd > 0)
			{
				palabradetec[indicepd] = '\0';
				strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabradetec);
				iEstadisticas[iNumElementos] = 1;
				iNumElementos++;
			}
			palabrauni = 0;
			bool ordenado = false;
			for (j = 0; j < iNumElementos - 1 && ordenado == false; j++)
			{
				ordenado = true;
				for (k = 0; k < iNumElementos - j - 1; k++)
				{
					if (strcmp(szPalabras[k], szPalabras[k + 1]) > 0)
					{
						ordenado = false;
						strcpy_s(almatem, TAMTOKEN, szPalabras[k]);
						strcpy_s(szPalabras[k], TAMTOKEN, szPalabras[k + 1]);
						strcpy_s(szPalabras[k + 1], TAMTOKEN, almatem);
						frecuen = iEstadisticas[k];
						iEstadisticas[k] = iEstadisticas[k + 1];
						iEstadisticas[k + 1] = frecuen;
					}
				}
			}
			for (k = 1; k < iNumElementos; k++)
			{
				if (strcmp(szPalabras[k], szPalabras[k - 1]) == 0)
				{
					iEstadisticas[palabrauni] += iEstadisticas[k];
				}
				else
				{
					palabrauni++;
					strcpy_s(szPalabras[palabrauni], TAMTOKEN, szPalabras[k]);
					iEstadisticas[palabrauni] = iEstadisticas[k];
				}
			}

			iNumElementos = palabrauni + 1;
		}
		fclose(fpDicc);
	}
	else
	{
		if (depuracion == 1)
			printf("\nnopi no abre\n");
	}
	*/

	//Sustituya estas lineas por su c�digo
	/*iNumElementos = 1;
	strcpy_s(szPalabras[0],"AquiVaElDiccionario");
	iEstadisticas[0] = 1;  la primer palabra aparece solo una vez.*/
//}


/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
{

	
	//Sustituya estas lineas por su c�digo
	/*strcpy_s(szListaFinal[0], szPalabrasSugeridas[0]); //la palabra candidata
	iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata
	
	iNumLista = 1;	*/						//Una sola palabra candidata
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char* szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int& iNumSugeridas)						//Numero de elementos en la lista
	
{
	int i, j, a, k;
	iNumSugeridas = 0;

	// Operaci�n de sustituci�n
	for (i = 0; i < strlen(szPalabraLeida); i++) {
		char poslet[] = "abcdefghijklmn�opqrstuvwxyz�����"; 	for (  j = 0; j < strlen(poslet); j++) {
			char palabraSustituida[TAMTOKEN];
			strcpy_s(palabraSustituida, TAMTOKEN, szPalabraLeida);
			palabraSustituida[i] = poslet[j];
			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraSustituida);
			iNumSugeridas++;
		}
	}

	
	for (i = 0; i <= strlen(szPalabraLeida); i++) {
		char poslet[] = "abcdefghijklmn�opqrstuvwxyz�����";  

		for (j = 0; j < strlen(poslet); j++) {
			char palaex[TAMTOKEN];
			k = 0;

			for (a= 0; a< i; a++) {
				palaex[k++] = szPalabraLeida[a];
			}

			palaex[k++] = poslet[j];

			for (a= i; a< strlen(szPalabraLeida); a++) {
				palaex[k++] = szPalabraLeida[a];
			}

			palaex[k] = '\0';

			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palaex);
			iNumSugeridas++;
		}
	}

	
	for (i = 0; i < strlen(szPalabraLeida); i++) {
		char palabraEliminada[TAMTOKEN];
		k = 0;

		for (j = 0; j < strlen(szPalabraLeida); j++) {
			if (j != i) {
				palabraEliminada[k++] = szPalabraLeida[j];
			}
		}

		palabraEliminada[k] = '\0';

		strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraEliminada);
		iNumSugeridas++;
	}

	for (i = 0; i < iNumSugeridas - 1; i++) {
		for ( j = 0; j < iNumSugeridas - i - 1; j++) {
			if (strcmp(szPalabrasSugeridas[j], szPalabrasSugeridas[j + 1]) > 0) {
				char temp[TAMTOKEN];
				strcpy_s(temp, TAMTOKEN, szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], TAMTOKEN, szPalabrasSugeridas[j + 1]);
				strcpy_s(szPalabrasSugeridas[j + 1], TAMTOKEN, temp);
			}
		}
	}
	//Sustituya estas lineas por su c�digo
	//strcpy_s(szPalabrasSugeridas[0], szPalabraLeida); //lo que sea que se capture, es sugerencia
	//iNumSugeridas = 1;				//Una sola palabra sugerida
}
