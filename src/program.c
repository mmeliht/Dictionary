#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fields.h"
#include "../include/dllist.h"

int main(int argc, char **argv)
{

	IS input, sozluk;
	char *dil;
	char *tr = "tr";
	char *eng = "eng";
	int durum = 0;
	char **ingilizceKelimeler = malloc(sizeof(char *) * 100000);
	char **turkceKelimeler = malloc(sizeof(char *) * 100000);
	char **inputKelimeleri = malloc(sizeof(char *) * 100000);
	int sozlukSayisi = 0, okunanSayisi = 0, ceviriSayisi = 0;

	Dllist DLL = new_dllist(), tDLL;

	if (argc != 4)
	{
		fprintf(stderr, "usage:program dil input sozluk \n");
		exit(1);
	}

	dil = argv[1];
	input = new_inputstruct(argv[2]);
	sozluk = new_inputstruct(argv[3]);

	if (dil == NULL)
	{
		perror(argv[1]);
		exit(1);
	}
	if (input == NULL)
	{
		perror(argv[2]);
		exit(2);
	}
	if (sozluk == NULL)
	{
		perror(argv[3]);
		exit(3);
	}

	for (sozlukSayisi = 0; get_line(sozluk); ++sozlukSayisi)
	{
		*(ingilizceKelimeler + (sozlukSayisi)) = strdup(sozluk->fields[1]);
		*(turkceKelimeler + (sozlukSayisi)) = strdup(sozluk->fields[2]);
	}

	while (get_line(input) > 0)
	{
		for (int i = 0; i < input->NF; i++)
		{
			*(inputKelimeleri + (okunanSayisi)) = strdup(input->fields[i]);
			okunanSayisi++;
		}
	}
	if (strcmp(dil, tr) == 0)
	{
		for (int i = 0; i < okunanSayisi; ++i)
		{
			for (int j = 0; j < sozlukSayisi; ++j)
			{
				if (strcmp(*(inputKelimeleri + i), *(ingilizceKelimeler + j)) == 0)
				{
					dll_append(DLL, new_jval_s(strdup(*(turkceKelimeler + j))));
					ceviriSayisi++;
					durum = 1;
				}
			}
			if (durum == 0)
				dll_append(DLL, new_jval_s(strdup(*(inputKelimeleri + i))));
			durum = 0;
		}
	}
	if (strcmp(dil, eng) == 0)
	{
		for (int i = 0; i < okunanSayisi; ++i)
		{
			for (int j = 0; j < sozlukSayisi; ++j)
			{
				if (strcmp(*(inputKelimeleri + i), *(turkceKelimeler + j)) == 0)
				{
					dll_append(DLL, new_jval_s(strdup(*(ingilizceKelimeler + j))));
					ceviriSayisi++;
				}
			}
			if (durum == 0)
				dll_append(DLL, new_jval_s(strdup(*(inputKelimeleri + i))));
			durum = 0;
		}
	}

	FILE *dosya = fopen("txt/output.txt", "a+");
	tDLL = DLL->flink;
	while (tDLL != DLL)
	{
		fprintf(dosya, "%s ", tDLL->val.s);
		tDLL = tDLL->flink;
	}

	printf("BILGI\n");
	printf("Karsilastirilan Kelime:%d\n", okunanSayisi);
	printf("Cevirilen Kelime:%d\n", ceviriSayisi);
	printf("Cevrilmeyen Kelime:%d\n", (okunanSayisi - ceviriSayisi));
	jettison_inputstruct(input);
	jettison_inputstruct(sozluk);

	return 0;
}
