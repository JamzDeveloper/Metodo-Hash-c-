#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstring>
using namespace std;

struct encabezado
{
	int NRS;
} ed, ec, e;
struct registro
{
	int NR;
	char nombre[20];
	char email[30];
	int SR;
	int disponible;
} r, s;

FILE *fdd, *fdc;
int le = sizeof(struct encabezado);
int lr = sizeof(struct registro);
int pos;

int fhash(char nom[20])
{
	int suma = 0;
	for (int i = 0; i < strlen(nom); i++)
	{
		suma += nom[i];
	}
	cout << "identificador hash : " << suma % 10 + 1 <<endl;
	return suma % 10 + 1;
}

void verElimanados(){
		if ((fdd = fopen("persona.txt", "rt")) == NULL)
	{
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "rt")) == NULL)
	{
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	fread(&ed, le, 1, fdd);
	cout << "Archivo Disperso. NRS:" << ed.NRS << endl;
	cout << "NRS\tnombre\temail\t\tSR\tDisponible\n";
	while (fread(&s, lr, 1, fdd))
	{

		if (s.disponible == 0 && s.NR != 0)
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR<< "\t" << s.disponible << endl;
	}
	fread(&ec, le, 1, fdc);
	cout << "Archivo Colision. NRS:" << ec.NRS << endl;
	cout << "NRS\tnombre\temail    \tSR\tDisponible\n";
	while (fread(&s, lr, 1, fdc))
	{
		if (s.disponible == 0 && s.NR != 0)
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;
	}
	fclose(fdd);
	fclose(fdc);
}
void escribir()
{
	char rpta;
	int sgte;
	if ((fdd = fopen("persona.txt", "w+t")) == NULL)
	{
		cout << "no se pudo crear persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "w+t")) == NULL)
	{
		cout << "no se pudo crear persona.col" << endl;
		return;
	}
	ed.NRS = 0;
	ec.NRS = 0;
	
	fwrite(&ed, le, 1, fdd);
	fwrite(&ec, le, 1, fdc);
	do
	{
		fflush(stdin);
		cout << "nombre:" << endl;
		gets(r.nombre);
		cout << "email:" << endl;
		gets(r.email);
		r.SR = -1;
		r.NR = fhash(r.nombre);

		r.disponible = 1; //////////////////
		pos = (r.NR - 1) * lr + le;
		fseek(fdd, pos, 0);
		fread(&s, lr, 1, fdd);
		if (strcmp(s.nombre, "") == 0)
		{
			//No hay colisi�n, graba en *.txt de forma dispersa
			ed.NRS++;

			fseek(fdd, pos, 0);
			fwrite(&r, lr, 1, fdd);
		}
		else
		{
			//S� hay colisi�n, graba en *.col de forma secuencial
			r.NR = ++ec.NRS;
			pos = (r.NR - 1) * lr + le;
			fseek(fdc, pos, 0);
			fwrite(&r, lr, 1, fdc);
			//enlazar un registro anterior para que apunte al nuevo
			if (s.SR == -1)
			{ //caso 1: el anterior est� en *.txt
				s.SR = r.NR;
				pos = (s.NR - 1) * lr + le;
				fseek(fdd, pos, 0);
				fwrite(&s, lr, 1, fdd);
			}
			else
			{ //caso2: el anterior est� en *.col
				//recorrer la lista enlazada en *.col
				sgte = s.SR;
				while (sgte != -1)
				{
					pos = (sgte - 1) * lr + le;
					fseek(fdc, pos, 0);
					fread(&s, lr, 1, fdc);
					sgte = s.SR;
				}
				s.SR = r.NR;
				pos = (s.NR - 1) * lr + le;
				fseek(fdc, pos, 0);
				fwrite(&s, lr, 1, fdc);
			}
		}
		cout << "desea m�s registros?" << endl;
		cin >> rpta;
	} while (rpta == 'S' || rpta == 's');
	fseek(fdd, 0, 0);
	fseek(fdc, 0, 0);
	fwrite(&ed, le, 1, fdd);
	fwrite(&ec, le, 1, fdc);
	fclose(fdd);
	fclose(fdc);
}

void leerTodo()
{
	if ((fdd = fopen("persona.txt", "rt")) == NULL)
	{
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "rt")) == NULL)
	{
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	fread(&ed, le, 1, fdd);
	cout << "Archivo Disperso. NRS:" << ed.NRS<< endl;
	cout << "NRS\tnombre\temail   \tSR\tDisponible\n";
	while (fread(&s, lr, 1, fdd))
	{

		if (s.disponible == 1)
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR<< "\t" << s.disponible << endl;
	}
	fread(&ec, le, 1, fdc);
	cout << "Archivo Colision. NRS:" << ec.NRS << endl;
	cout << "NRS\tnombre\temail\t\tSR\tDisponible\n";
	while (fread(&s, lr, 1, fdc))
	{
		if (s.disponible == 1)
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR  << "\t" << s.disponible << endl;
	}
	fclose(fdd);
	fclose(fdc);
}

void insertar()
{
	char rpta;
	int sgte;
	if ((fdd = fopen("persona.txt", "r+t")) == NULL)
	{
		cout << "no se pudo crear persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "r+t")) == NULL)
	{
		cout << "no se pudo crear persona.col" << endl;
		return;
	}
	ed.NRS = 0;
	ec.NRS = 0;

	//fwrite(&ed, le, 1, fdd);
	//fwrite(&ec, le, 1, fdc);
	fseek(fdd, 0, 0);
	fread(&ed, le, 1, fdd);
	fseek(fdc, 0, 0);
	fread(&ec, le, 1, fdc);
	do
	{
		fflush(stdin);
		cout << "nombre:" << endl;
		gets(r.nombre);
		cout << "email:" << endl;
		gets(r.email);
		r.SR = -1;
		r.NR = fhash(r.nombre);
		
		r.disponible = 1; //////////////////
		pos = (r.NR - 1) * lr + le;
		fseek(fdd, pos, 0);
		fread(&s, lr, 1, fdd);
		if (strcmp(s.nombre, "") == 0)
		{
			//No hay colisi�n, graba en *.txt de forma dispersa
			ed.NRS++;

			fseek(fdd, pos, 0);
			fwrite(&r, lr, 1, fdd);
		}
		else
		{
			//S� hay colisi�n, graba en *.col de forma secuencial
			r.NR = ++ec.NRS;
			pos = (r.NR - 1) * lr + le;
			fseek(fdc, pos, 0);
			fwrite(&r, lr, 1, fdc);
			//enlazar un registro anterior para que apunte al nuevo
			if (s.SR == -1)
			{ //caso 1: el anterior est� en *.txt
				s.SR = r.NR;
				pos = (s.NR - 1) * lr + le;
				fseek(fdd, pos, 0);
				fwrite(&s, lr, 1, fdd);
			}
			else
			{ //caso2: el anterior est� en *.col
				//recorrer la lista enlazada en *.col
				sgte = s.SR;
				while (sgte != -1)
				{
					pos = (sgte - 1) * lr + le;
					fseek(fdc, pos, 0);
					fread(&s, lr, 1, fdc);
					sgte = s.SR;
				}
				s.SR = r.NR;
				pos = (s.NR - 1) * lr + le;
				fseek(fdc, pos, 0);
				fwrite(&s, lr, 1, fdc);
			}
		}
		cout << "desea m�s registros?" << endl;
		cin >> rpta;
	} while (rpta == 'S' || rpta == 's');
	fseek(fdd, 0, 0);
	fseek(fdc, 0, 0);
	fwrite(&ed, le, 1, fdd);
	fwrite(&ec, le, 1, fdc);
	fclose(fdd);
	fclose(fdc);
}
void buscar()
{
	char nom[20];
	int NR, sgte = 0;
	if ((fdd = fopen("persona.txt", "rt")) == NULL)
	{
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "rt")) == NULL)
	{
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	fflush(stdin);
	cout << "nombre:" << endl;
	gets(nom);
	NR = fhash(nom);
	cout << "Hash:" << NR << endl;
	pos = (NR - 1) * lr + le;
	fseek(fdd, pos, 0);
	fread(&s, lr, 1, fdd);
	if (strcmp(s.nombre, "") == 0)
	{
		cout << "no existe registro con ese hash" << endl;
		return;
	}

	if (strcmp(s.nombre, nom) == 0)
	{
		if (s.disponible == 1)
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR<< "\t" << s.disponible << endl;
		else
			cout << "el registro ya ha sido eliminado\n";

		return;
	}
	cout << "no existe en el archivo *.txt" << endl;
	sgte = s.SR;
	while (sgte != -1)
	{
		pos = (sgte - 1) * lr + le;
		fseek(fdc, pos, 0);
		fread(&s, lr, 1, fdc);
		if (strcmp(nom, s.nombre) == 0)
		{
			if (s.disponible == 1)
				cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;
			else
				cout << "el registro ya ha sido eliminado\n";

			return;
		}
		sgte = s.SR;
	}
	cout << "no existe en *.col" << endl;
	return;
}

void modificar()
{

	char nom[20];
	int NR, sgte = 0;
	if ((fdd = fopen("persona.txt", "r+t")) == NULL)
	{
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "r+t")) == NULL)
	{
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	fflush(stdin);
	cout << "nombre:" << endl;
	gets(nom);
	NR = fhash(nom);
	cout << "Hash:" << NR << endl;
	pos = (NR - 1) * lr + le;
	fseek(fdd, pos, 0);
	fread(&s, lr, 1, fdd);
	if (strcmp(s.nombre, "") == 0)
	{
		cout << "no existe registro con ese hash" << endl;
		return;
	}

	if (strcmp(s.nombre, nom) == 0)
	{
		if (s.disponible == 1)
		{

			cout << "encontro en el archivo txt" << endl;
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;

			fflush(stdin);
			cout << "identificador nombre no se puede cambiar " << endl;
			cout << "email:" << endl;
			gets(s.email);

			fseek(fdd, pos, 0);
			fwrite(&s, lr, 1, fdd); //////////////////////////////////
			fclose(fdd);
		}

		else
			cout << "el registro ya ha sido eliminado\n";

		return;
	}

	cout << "no existe en el archivo *.txt" << endl;
	sgte = s.SR;
	cout << "siguiente: antes del bucle: " << sgte << endl;
	cout << "llego a ";
	//infinito y terminarlo cuando sea -1
	while (sgte != -1)
	{
		cout << "siguiente : " << sgte << endl;

		pos = (sgte - 1) * lr + le;
		fseek(fdc, pos, 0);
		fread(&s, lr, 1, fdc);
		cout << " encontrado nombre" << s.nombre << "disponible " << endl;
		cout << "nombre a buscar--" << nom << "--";
		if (strcmp(nom, s.nombre) == 0)
		{
			cout << " encontrado nombre" << s.nombre << "disponible " << endl;
			if (s.disponible == 1)
			{
				cout << "encontro en el archivo de colision ";

				cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;

				fflush(stdin);
				cout << "identificador nombre no se puede cambiar " << endl;
				cout << "email:" << endl;
				gets(s.email);

				fseek(fdc, pos, 0);
				//agregar a la lista de eliminados
				fwrite(&s, lr, 1, fdc);
				fclose(fdc);
				return;
			}

			else
			{
				cout << "el registro ya ha sido eliminado\n";
			}
			//return;
		}
		sgte = s.SR;
	}
	cout << "no existe en *.col" << endl;

	return;
}
void restaurar()
{

	char nom[20];
	int NR, sgte = 0;
	if ((fdd = fopen("persona.txt", "r+t")) == NULL)
	{
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "r+t")) == NULL)
	{
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	fflush(stdin);
	cout << "nombre:" << endl;
	gets(nom);
	NR = fhash(nom);
	cout << "Hash:" << NR << endl;
	pos = (NR - 1) * lr + le;
	fseek(fdd, pos, 0);
	fread(&s, lr, 1, fdd);
	if (strcmp(s.nombre, "") == 0)
	{
		cout << "no existe registro con ese hash" << endl;
		return;
	}

	if (strcmp(s.nombre, nom) == 0)
	{
		if (s.disponible == 0)
		{
			s.disponible = 1;

			fseek(fdd, pos, 0);
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;
			fwrite(&s, lr, 1, fdd); //////////////////////////////////
			fclose(fdd);
		}

		else
			cout << "el registro ya ha sido eliminado\n";

		return;
	}

	cout << "no existe en el archivo *.txt" << endl;
	sgte = s.SR;
	cout << "llego a restaurar:  sge= " << s.SR << "indentificador" << NR
		 << "nombre actual " << s.nombre << endl;
	while (sgte != -1)
	{
		pos = (sgte - 1) * lr + le;
		fseek(fdc, pos, 0);
		fread(&s, lr, 1, fdc);
		if (strcmp(nom, s.nombre) == 0)
		{
			if (s.disponible == 0)
			{
				cout << "encontro en e archivo de colision\n";
				s.disponible = 1;
				fseek(fdc, pos, 0);
				fwrite(&s, lr, 1, fdc);
				fclose(fdc);
				return;
			}

			else
				cout << "el registro ya esta disponible\n";

			return;
		}
		sgte = s.SR;
	}
	cout << "no existe en *.col" << endl;

	fclose(fdd);
	fclose(fdc);
	return;
}
void eliminar()
{

	char nom[20];
	int NR, sgte = 0;
	if ((fdd = fopen("persona.txt", "r+t")) == NULL)
	{
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "r+t")) == NULL)
	{
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	fflush(stdin);
	cout << "nombre:" << endl;
	gets(nom);
	NR = fhash(nom);
	cout << "Hash:" << NR << endl;
	pos = (NR - 1) * lr + le;
	fseek(fdd, pos, 0);
	fread(&s, lr, 1, fdd);
	if (strcmp(s.nombre, "") == 0)
	{
		cout << "no existe registro con ese hash" << endl;
		return;
	}

	
	if (strcmp(s.nombre, nom) == 0)
	{
		if (s.disponible == 1)
		{
			s.disponible = 0;

			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;
			cout << "Registro eliminado\n" ;
			fseek(fdd, pos, 0);
			fwrite(&s, lr, 1, fdd); //////////////////////////////////
			fclose(fdd);
			
			
		}

		else
			cout << "el registro ya ha sido eliminado\n";

		return;
	}

	cout << "no existe en el archivo *.txt" << endl;
	sgte = s.SR;
	//infinito y terminarlo cuando sea -1
	while (sgte != -1)
	{
		

		pos = (sgte - 1) * lr + le;
		fseek(fdc, pos, 0);
		fread(&s, lr, 1, fdc);
		
		if (strcmp(nom, s.nombre) == 0)
		{
			
			if (s.disponible == 1)
			{
				cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << "\t" << s.disponible << endl;
			cout << "Registro eliminado\n" ;
				s.disponible = 0;
				fseek(fdc, pos, 0);
				//agregar a la lista de eliminados
				fwrite(&s, lr, 1, fdc);
				fclose(fdc);
				return;
			}

			else
			{
				cout << "el registro ya ha sido eliminado\n";
			}
			
		}
		sgte = s.SR;
	}
	cout << "no existe en *.col" << endl;

	return;
}

int main(int argc, char *argv[])
{
	int op;
	do
	{
		cout << " 1.escribir\n 2.leerTodo\n 3.insertar\n 4.buscar\n 5.modificar\n 6.eliminar\n 7.restaurar\n 8.ver registros eliminados\n9.salir" << endl;
		cin >> op;
		switch (op)
		{
		case 1:
			escribir();
			break;
		case 2:
			leerTodo();
			break;
		case 3:
			insertar();
			break;
		case 4:
			buscar();
			break;

		case 5:
			modificar();
			break;

		case 6:
			eliminar();
			break;
		case 7:
			restaurar();
			break;
		case 8:
			verElimanados();
			break;
		}
	} while (op != 9);
	return 0;
}
