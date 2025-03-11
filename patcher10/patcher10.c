/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Patch PellesC 10.0 to let it work well on Windows 11 24H2      *
 *                                                                          *
 * History : Date        Reason                                             *
 *           2025/03/11  Created                                            *
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

int cc_exe[] = {0x8CB, 0xE47, 0xFC8, 0x1058, 0x10E8, 0x124E, 0x136E, 0x13FF, 0x14AF, 0x15D2, 0x1602, 0x1805, 0x22D1, 0x3565, 0x4687, 0x6752, 0xB673, 0xC026};
int cformat_dll[] = {0xB7DF, 0x12199, 0x121E3, 0x12371};
int fdiff_dll[] = {0xA81F, 0x104B9, 0x10503, 0x10691};
int idespawn_exe[] = {0x13E5, 0x2801, 0x2937, 0x3042, 0x4C73, 0x4DE6};
int poasm_exe[] = {0x17B7, 0x4E871, 0x50075, 0x51ED7, 0x540C3, 0x57126};
int pobr_dll[] = {0xA50F, 0x122B9, 0x12303, 0x12CD1};
int pobr_exe[] = {0xAA7, 0xB8C, 0x2549, 0x26B6, 0x402F, 0x9D83, 0x9F11};
int pocc_exe[] = {0xBB519, 0x206041, 0x207635, 0x209BF7, 0x20E803, 0x210F36};
int pocert_exe[] = {0x24F3, 0x3601, 0x46A5, 0x4907, 0xE613, 0xEAE6};
int pocrt64_dll[] = {0xAAE, 0x1F3F, 0x1310F, 0x1C824, 0x1D063, 0x1D0C3, 0x27FD9, 0x38FD9, 0x39161};
int podump_exe[] = {0xF898, 0x29AF1, 0x2B855, 0x2C7D7, 0x36A53, 0x379C6};
int poedit_exe[] = {0x3BA0, 0x426A, 0x4330, 0x449C, 0x4A91, 0x5D15, 0x6046, 0x6073, 0x6A97, 0x7D76};
int poh2inc_exe[] = {0x41E2, 0x79D1, 0x8A75, 0xBEB7, 0xC663, 0xEC76};
int poide_exe[] = {0x107BD1, 0x1083CF, 0x10C9C9, 0x115283, 0x115411};
int polib_exe[] = {0x3518, 0x6FB0, 0x711C, 0x7D51, 0x9055, 0x9306, 0x9333, 0xAAD7, 0xB996};
int polink_exe[] = {0x10B60, 0x1852A, 0x186D0, 0x1883C, 0x1A071, 0x1B195, 0x1B4B6, 0x1B4E3, 0x1B9E7, 0x1DA26};
int pomake_exe[] = {0x3F8E, 0x8741, 0x9BF5, 0xA2C7, 0x11B73, 0x11CE6};
int pomc_exe[] = {0x15CB, 0x40E1, 0x5245, 0x66A7, 0xD143, 0xD2B6};
int porc_dll[] = {0x105EF, 0x19989, 0x199D3, 0x19FA1};
int porc_exe[] = {0x9E4, 0xB0D, 0x2FB1, 0x3105, 0x4307, 0x91B3, 0x9326};
int poreg_exe[] = {0xC5C1, 0xE7AF, 0xF039, 0x10513, 0x12F51};
int posign_exe[] = {0xED2, 0x1882, 0x2C81, 0x2DC6, 0x2DF3, 0x3F85, 0x4A96, 0x5707};
int poviewp_exe[] = {0x4E41, 0x5D0F, 0x6879, 0xEEF3, 0xF081};
int sqlite3_dll[] = {0x938DF, 0x98F79, 0x98FC3, 0x9A421};
int support_dll[] = {0x4523C, 0x453CB, 0x64FF3, 0x678AF, 0x68071, 0x680E9};
int addin_dll[] = {0x2BBF, 0x5CB9, 0x5D03, 0x5E91};
int conapp_dll[] = {0x2A8F, 0x5B89, 0x5BD3, 0x5D61};
int custcntl_dll[] = {0x2EEF, 0x5FE9, 0x6033, 0x61C1};
int winapp_dll[] = {0x33FF, 0x6489, 0x64D3, 0x6661};
int windll_dll[] = {0x2E9F, 0x5F99, 0x5FE3, 0x6171};
int wininst_dll[] = {0x34BF, 0x6DC9, 0x6E13, 0x6FA1};
int msvc_dll[] = {0x974F, 0xCF29, 0xCF73, 0x10561};

struct patch bins[] = {
	{"cc.exe", countOf(cc_exe), cc_exe},
	{"cformat.dll", countOf(cformat_dll), cformat_dll},
	{"fdiff.dll", countOf(fdiff_dll), fdiff_dll},
	{"idespawn.exe", countOf(idespawn_exe), idespawn_exe},
	{"poasm.exe", countOf(poasm_exe), poasm_exe},
	{"pobr.dll", countOf(pobr_dll), pobr_dll},
	{"pobr.exe", countOf(pobr_exe), pobr_exe},
	{"pocc.exe", countOf(pocc_exe), pocc_exe},
	{"pocert.exe", countOf(pocert_exe), pocert_exe},
	{"pocrt64.dll", countOf(pocrt64_dll), pocrt64_dll},
	{"podump.exe", countOf(podump_exe), podump_exe},
	{"poedit.exe", countOf(poedit_exe), poedit_exe},
	{"poh2inc.exe", countOf(poh2inc_exe), poh2inc_exe},
	{"poide.exe", countOf(poide_exe), poide_exe},
	{"polib.exe", countOf(polib_exe), polib_exe},
	{"polink.exe", countOf(polink_exe), polink_exe},
	{"pomake.exe", countOf(pomake_exe), pomake_exe},
	{"pomc.exe", countOf(pomc_exe), pomc_exe},
	{"porc.dll", countOf(porc_dll), porc_dll},
	{"porc.exe", countOf(porc_exe), porc_exe},
	{"poreg.exe", countOf(poreg_exe), poreg_exe},
	{"posign.exe", countOf(posign_exe), posign_exe},
	{"poviewp.exe", countOf(poviewp_exe), poviewp_exe},
	{"sqlite3.dll", countOf(sqlite3_dll), sqlite3_dll},
	{"support.dll", countOf(support_dll), support_dll},
	{"Wizards64\\addin.dll", countOf(addin_dll), addin_dll},
	{"Wizards64\\conapp.dll", countOf(conapp_dll), conapp_dll},
	{"Wizards64\\custcntl.dll", countOf(custcntl_dll), custcntl_dll},
	{"Wizards64\\winapp.dll", countOf(winapp_dll), winapp_dll},
	{"Wizards64\\windll.dll", countOf(windll_dll), windll_dll},
	{"Wizards64\\wininst.dll", countOf(wininst_dll), wininst_dll},
	{"AddIns64\\msvc.dll", countOf(msvc_dll), msvc_dll},
};

struct special {
	int offset;
	unsigned char from;
	unsigned char to;
};

struct special poccs[] = {
	{0x27146A, 0x74, 0x23},
	{0x27146F, 0x74, 0x23},
	{0x271475, 0x74, 0x23},
};

int libCrt64[] = {0x144e2, 0x148d6, 0x155f0, 0x187e1, 0x19255, 0x46fa1, 0x51a31, 0x5bbda, 0x60786, 0x61ea2, 0x6294b, 0x62ba7, 0x88feb, 0xc1d07, 0xc214e};
int libCrtmt64[] = {0x16818, 0x16c86, 0x179a0, 0x1b664, 0x1c127, 0x4d6ff, 0x5b0fa, 0x654df, 0x6a0c6, 0x6b7b3, 0x6c295, 0x6c509, 0x95d25, 0xd6f57, 0xd7537};
int libPocrt64[] = {0x2bc60, 0x2d35e, 0x2d9c2, 0x2e092};

struct patch libs[] = {
	{"crt64.lib", countOf(libCrt64), libCrt64},
	{"crtmt64.lib", countOf(libCrtmt64), libCrtmt64},
	{"pocrt64.lib", countOf(libPocrt64), libPocrt64},
};

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
				}
				UnmapViewOfFile(pData);
			}
			CloseHandle(hMap);
		}
		CloseHandle(hFile);
	}
	if (ret < 0) {
		if (strcmp(p->name, "Wizards64\\addin.dll") == 0) {
			return 0;
		}
		ret = 1;
		fprintf(stderr, "cannot open %s, error %X\n", p->name, GetLastError());
	}
	return ret;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("usage:\n\tpatcher10 <the PellesC dir>\n");
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
