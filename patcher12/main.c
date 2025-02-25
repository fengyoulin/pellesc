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

int binCc[] = {0x94B, 0xEB9, 0x1058, 0x10E8, 0x1178, 0x13A7, 0x14A7, 0x152B, 0x15CB, 0x16D2, 0x1702, 0x1905, 0x2432, 0x3754, 0x3DC5, 0x6BED};
int binFdiff[] = {0x999D, 0xF0EC};
int binIdespawn[] = {0x1214, 0x1E15, 0x28E2, 0x2FCD};
int binPobrDll[] = {0xC42D, 0x1306C};
int binPobrExe[] = {0xAB7, 0xBA0, 0x264C, 0x27D4, 0x2DFD};
int binPocert[] = {0x28B3, 0x3B02, 0x4C34, 0x4E35};
int binPodump[] = {0x8B85, 0x1E322, 0x1FF54, 0x20F85};
int binPofmtDll[] = {0x54DD, 0x8DDC};
int binPofmtExe[] = {0xDEE, 0xECB, 0x343C, 0x35C4, 0x568D};
int binPoh2inc[] = {0x419F, 0x7682, 0x87B4, 0x9E05};
int binPoide[] = {0x109024, 0x1095ED, 0x10CAFC};
int binPolib[] = {0x344B, 0x7B12, 0x8EC4, 0xA875};
int binPolink[] = {0x10964, 0x19CF2, 0x1AEA4, 0x1B555};
int binPomake[] = {0x410E, 0x8F62, 0xA4C4, 0xABE5};
int binPomc[] = {0x1591, 0x4022, 0x5204, 0x5C25};
int binPope[] = {0xD5A4, 0xD5D4, 0x15982, 0x16EA4, 0x173C5};
int binPorcDll[] = {0x1099D, 0x18A9C};
int binPorcExe[] = {0x9F4, 0xB14, 0x2DC2, 0x2E94, 0x3445};
int binPoreg[] = {0xC864, 0xE18D, 0xF43C};
int binPosign[] = {0xE82, 0x1773, 0x2B22, 0x2C78, 0x2CA3, 0x3EE4, 0x4C27, 0x4E65};
int binPoviewp[] = {0x4FE4, 0x523D, 0x6A9C};
int binSqlite3[] = {0x9745D, 0x9D73C};
int binSupport[] = {0x67D3D, 0x6EC2C};
int binMsvc[] = {0x961D, 0xCE3C};
int binConapp[] = {0x252D, 0x5ABC};
int binCustcntl[] = {0x29AD, 0x5F3C};
int binWinapp[] = {0x22DD, 0x53FC};
int binWindll[] = {0x294D, 0x5EDC};
int binWininst[] = {0x25ED, 0x5E7C};

struct patch opts[] = {
	{"cc.exe", countOf(binCc), binCc},
	{"fdiff.dll", countOf(binFdiff), binFdiff},
	{"idespawn.exe", countOf(binIdespawn), binIdespawn},
	{"pobr.dll", countOf(binPobrDll), binPobrDll},
	{"pobr.exe", countOf(binPobrExe), binPobrExe},
	{"pocert.exe", countOf(binPocert), binPocert},
	{"podump.exe", countOf(binPodump), binPodump},
	{"pofmt.dll", countOf(binPofmtDll), binPofmtDll},
	{"pofmt.exe", countOf(binPofmtExe), binPofmtExe},
	{"poh2inc.exe", countOf(binPoh2inc), binPoh2inc},
	{"poide.exe", countOf(binPoide), binPoide},
	{"polib.exe", countOf(binPolib), binPolib},
	{"polink.exe", countOf(binPolink), binPolink},
	{"pomake.exe", countOf(binPomake), binPomake},
	{"pomc.exe", countOf(binPomc), binPomc},
	{"pope.exe", countOf(binPope), binPope},
	{"porc.dll", countOf(binPorcDll), binPorcDll},
	{"porc.exe", countOf(binPorcExe), binPorcExe},
	{"poreg.exe", countOf(binPoreg), binPoreg},
	{"posign.exe", countOf(binPosign), binPosign},
	{"poviewp.exe", countOf(binPoviewp), binPoviewp},
	{"sqlite3.dll", countOf(binSqlite3), binSqlite3},
	{"support.dll", countOf(binSupport), binSupport},
	{"AddIns64\\msvc.dll", countOf(binMsvc), binMsvc},
	{"Wizards64\\conapp.dll", countOf(binConapp), binConapp},
	{"Wizards64\\custcntl.dll", countOf(binCustcntl), binCustcntl},
	{"Wizards64\\winapp.dll", countOf(binWinapp), binWinapp},
	{"Wizards64\\windll.dll", countOf(binWindll), binWindll},
	{"Wizards64\\wininst.dll", countOf(binWininst), binWininst},
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
	{0x1E0, 0x50, 0x80},
	{0x270D11, 0x74, 0x23},
	{0x270D16, 0x74, 0x23},
	{0x270D1C, 0x74, 0x23},
	{0x2EDB70, 0x43, 0x50},
	{0x2EDB71, 0x1E, 0xEA},
	{0x2EDB72, 0x2B, 0x2D},
};

unsigned char pocc2dca50[] = {0x63, 0x61, 0x6C, 0x6C, 0x20, 0x5F, 0x5F, 0x63, 0x72, 0x74, 0x5F, 0x61, 0x73, 0x73, 0x65, 0x72, 0x74, 0x0A, 0x69, 0x6E, 0x74, 0x33, 0x0A, 0x63, 0x61, 0x6C, 0x6C, 0x20, 0x61, 0x62, 0x6F, 0x72, 0x74, 0x0A, 0x00};

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
	int ret = -1;
	HANDLE hFile = CreateFile(path, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (hMap != NULL) {
			PVOID pData = MapViewOfFile(hMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
			if (pData != NULL) {
				ret = 0;
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
						for (int idx = 0; idx < countOf(pocc2dca50); ++idx) {
							int off = 0x2dca50 + idx;
							unsigned char to = pocc2dca50[idx];
							if (dobyte(p->name, size, off, pb, 0, to) > 0) {
								ret = 1;
								break;
							}
						}
					}
				}
				UnmapViewOfFile(pData);
			}
			CloseHandle(hMap);
		}
		CloseHandle(hFile);
	}
	if (ret < 0) {
		ret = 1;
		fprintf(stderr, "cannot open %s, error %X\n", path, GetLastError());
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
	for (int idx = 0; idx < countOf(opts); ++idx) {
		struct patch *p = &opts[idx];
		strcpy(path, argv[1]);
		strcat(path, "\\Bin\\");
		strcat(path, p->name);
		if (dofile(path, p) > 0) {
			return 1;
		}
	}
	return 0;
}
