/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Patch PellesC 12.0 to let it work well on Windows 11 24H2      *
 *                                                                          *
 * History : Date        Reason                                             *
 *           2025/02/15  Created                                            *
 *                                                                          *
 ****************************************************************************/

#include <windows.h>
#include <stdio.h>

#define countOf(a) (sizeof(a)/sizeof(a[0]))

struct patch {
	char *name;
	int count;
	int *offset;
};

int binPoasm[] = {0x1758,0x4D7F2, 0x4F124, 0x51355};
int binPocc[] = {0x9C439, 0x20ABF2, 0x20C254, 0x20DD85};
int binPocrt64[] = {0x880, 0x1AFD, 0x10420, 0x19C6F, 0x1A4D3, 0x1A4D3, 0x1A533, 0x2006C, 0x303EC, 0x30582};
int binPoedit[] = {0x4AA2, 0X5DE4, 0x6A15};

struct patch bins[] = {
	{"poasm.exe", countOf(binPoasm), binPoasm},
	{"pocc.exe", countOf(binPocc), binPocc},
	{"pocrt64.dll", countOf(binPocrt64), binPocrt64},
	{"poedit.exe", countOf(binPoedit), binPoedit},
};

int libCrt64[] = {0xD38C, 0x1113E, 0x11C6E, 0x3BA2C, 0x45F34, 0x4CC68, 0x54ED1, 0x5662E, 0x570E3, 0x5733F, 0x6EA86, 0xAB066, 0xAB4C9};
int libCrtmt64[] = {0xFEAA, 0x147BE, 0x1530A, 0x42A20, 0x4FEC0, 0x56DE8, 0x5F08B, 0x607E8, 0x612D3, 0x61547, 0x7C400, 0xC1814, 0xC1E1E};
int libPocrt64[] = {0x2DAAE, 0x2F0E2, 0x2F77C, 0x2FE82};

struct patch libs[] = {
	{"crt64.lib", countOf(libCrt64), libCrt64},
	{"crtmt64.lib", countOf(libCrtmt64), libCrtmt64},
	{"pocrt64.lib", countOf(libPocrt64), libPocrt64},
};

struct special {
	int offset;
	unsigned char from;
	unsigned char to;
};

struct special poccs[] = {
	{0x1E0, 0x50, 0},
	{0x1E1, 0x4A, 0x4B},
	{0x2EDB70, 0x43, 0xCF},
	{0x2EDB71, 0x1E, 0xEA},
	{0x2EDB72, 0x2B, 0x2D},
	{0x2EF248, 0x75, 0xB1},
	{0x2EF249, 0xEC, 0xEA},
	{0x2EF24A, 0x2A, 0x2D},
	{0x2EF250, 0x60, 0x89},
	{0x2EF251, 0xEC, 0xEA},
	{0x2EF252, 0x2A, 0x2D},
	{0x2EF258, 0x4A, 0x60},
	{0x2EF259, 0xEC, 0xEA},
	{0x2EF25A, 0x2A, 0x2D},
};

unsigned char pocc2dca60[] = {0x73, 0x75, 0x62, 0x20, 0x72, 0x73, 0x70, 0x2C, 0x38, 0x0A, 0x6F, 0x72, 0x20, 0x72, 0x73, 0x70, 0x2C, 0x38, 0x0A, 0x6D, 0x6F, 0x76, 0x20, 0x65, 0x63, 0x78, 0x2C, 0x25, 0x30, 0x0A, 0x6A, 0x6D, 0x70, 0x20, 0x5F, 0x45, 0x78, 0x69, 0x74, 0x0A, 0x00, 0x73, 0x75, 0x62, 0x20, 0x72, 0x73, 0x70, 0x2C, 0x38, 0x0A, 0x6F, 0x72, 0x20, 0x72, 0x73, 0x70, 0x2C, 0x38, 0x0A, 0x6D, 0x6F, 0x76, 0x20, 0x65, 0x63, 0x78, 0x2C, 0x25, 0x30, 0x0A, 0x6A, 0x6D, 0x70, 0x20, 0x65, 0x78, 0x69, 0x74, 0x0A, 0x00, 0x73, 0x75, 0x62, 0x20, 0x72, 0x73, 0x70, 0x2C, 0x38, 0x0A, 0x6F, 0x72, 0x20, 0x72, 0x73, 0x70, 0x2C, 0x38, 0x0A, 0x6A, 0x6D, 0x70, 0x20, 0x61, 0x62, 0x6F, 0x72, 0x74, 0x0A, 0x00, 0x63, 0x61, 0x6C, 0x6C, 0x20, 0x5F, 0x5F, 0x63, 0x72, 0x74, 0x5F, 0x61, 0x73, 0x73, 0x65, 0x72, 0x74, 0x0A, 0x69, 0x6E, 0x74, 0x33, 0x0A, 0x63, 0x61, 0x6C, 0x6C, 0x20, 0x61, 0x62, 0x6F, 0x72, 0x74, 0x0A};

int dobyte(const char *name, int size, int off, unsigned char *pb, unsigned char from, unsigned char to) {
	if (off >= size) {
		fprintf(stderr, "cannot patch %s at offset 0x%x\n", name, off);
		return 1;
	}
	unsigned char *p = pb + off;
	if (*p == to) {
		return 0;
	} else if (*p == from) {
		*p = to;
		printf("patched %s at offset 0x%x\n", name, off);
		return 0;
	} else {
		fprintf(stderr, "unexpected byte 0x%02x in %s at offset 0x%x\n", *p, name, off);
		return 1;
	}
}

int dofile(char *path, struct patch *p) {
	int ret = 0;
	HANDLE hFile = CreateFile(path, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (hMap != NULL) {
			PVOID pData = MapViewOfFile(hMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
			int size = (int)GetFileSize(hFile, NULL);
			unsigned char *pb = (unsigned char *)pData;
			for (int idx = 0; idx < p->count; ++idx) {
				int off = p->offset[idx];
				if (dobyte(p->name, size, off, pb, 0xE9, 0xE8) > 0) {
					ret = 1;
					break;
				}
			}
			if (ret == 0 && strcmp(p->name, "pocc.exe") == 0) {
				for (int idx = 0; idx < countOf(poccs); ++idx) {
					struct special *sp = &poccs[idx];
					if (dobyte(p->name, size, sp->offset, pb, sp->from, sp->to) > 0) {
						ret = 1;
						break;
					}
				}
				if (ret == 0) {
					for (int idx = 0; idx < countOf(pocc2dca60); ++idx) {
						int off = 0x2dca60 + idx;
						unsigned char to = pocc2dca60[idx];
						if (dobyte(p->name, size, off, pb, 0, to) > 0) {
							ret = 1;
							break;
						}
					}
				}
			}
			UnmapViewOfFile(pData);
			CloseHandle(hMap);
		}
		CloseHandle(hFile);
	}
	return ret;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("usage:\n\tpatcher12 <the PellesC dir>\n");
		return 0;
	}
	char path[MAX_PATH] = {0};
	for (int idx = 0; idx < countOf(bins); ++idx) {
		struct patch *p = &bins[idx];
		strcpy(path, argv[1]);
		strcat(path, "\\Bin\\");
		strcat(path, p->name);
		if (dofile(path, p) > 0) {
			return 1;
		}
	}
	for (int idx = 0; idx < countOf(libs); ++idx) {
		struct patch *p = &libs[idx];
		strcpy(path, argv[1]);
		strcat(path, "\\Lib\\");
		strcat(path, p->name);
		if (dofile(path, p) > 0) {
			return 1;
		}
	}
	return 0;
}
