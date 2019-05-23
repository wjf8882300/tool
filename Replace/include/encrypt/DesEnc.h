#ifndef __DES_H__
#define __DES_H__

#define IP_TABLE	0
#define IP_1_TABLE	1
#define E_TABLE		2
#define P_TABLE		3
#define PC1_TABLE	4
#define PC2_TABLE	5

#include "windows.h"


typedef struct
{
	BYTE L[32];
	BYTE R[32];
}LRStruct;

typedef struct
{
	BYTE C[28];
	BYTE D[28];
}CDStruct;

typedef struct
{
	BYTE b0 :	1;
	BYTE b1 :	1;
	BYTE b2 :	1;
	BYTE b3 :	1;
	BYTE b4 :	1;
	BYTE b5 :	1;
	BYTE b6 :	1;
	BYTE b7 :	1;
}BYTEStruct;

STDAPI_(void) EnDES (BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst);
STDAPI_(void) DeDES (BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst);
STDAPI_(void) En3DES(BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst);
STDAPI_(void) De3DES(BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst);
STDAPI_(void) MAC   (BYTE *pbyKey, BYTE *pbySrc, int iSrcLen, BYTE *pbyInit, BYTE *pbyDst);
STDAPI_(void) MAC16 (BYTE *pbyKey, BYTE *pbySrc, int iSrcLen, BYTE *pbyInit, BYTE *pbyDst);
STDAPI_(void) DefuseKey(BYTE *key, BYTE *factor, BYTE *DefusedKey);
STDAPI_(void) StrXor(BYTE* SouStr, int iLen, BYTE* DesStr);

STDAPI_(void) StrDefuseKey(char *Key, char *factor, char *DefusedKey);
STDAPI_(void) StrEnDES(char *Key, char *Src, char *Dst);
STDAPI_(void) StrDeDES(char *Key, char *Src, char *Dst);
STDAPI_(void) StrEn3DES(char *Key, char *Src, char *Dst);
STDAPI_(void) StrEn3DES_ECB(char *Key, char *Src, char *Dst);
STDAPI_(void) StrDe3DES(char *Key, char *Src, char *Dst);
STDAPI_(void) Str3MAC(char *Key, char *Src, char *Dst);
STDAPI_(void) StrMAC(char *Key, char *Src, char* InitData, char *Dst);
STDAPI_(void) ByteXorByteEx(BYTE X[],BYTE Y[],BYTE result[],int nCount);

STDAPI EncDecFile(BYTE *key, char *InFile, char *OutFile, int EncDec);

STDAPI_(char*) Trim(char *In);
STDAPI_(void)  LowToUpper(char *Str, int lenth);
STDAPI_(int)   HexStrToBytes(char *strSou, BYTE *BytDes, int bytCount);
STDAPI_(int)   BytesToHexStr(char *strDes, BYTE *bytSou, int bytCount);


STDAPI_(void) ByteXorByteEx(BYTE X[],BYTE Y[],BYTE result[],int nCount);
STDAPI_(void) StrByteXorByte(BYTE X[],BYTE Y[],char *result);

STDAPI_(long) ConverseHexStrToLong(char *pszData, int nLen);
void BitXor(BYTE *Data1, BYTE *Data2, int Len, BYTE *Dest);
void ByteToBit(BYTE *src, BYTE *dst, int n);
void BitToByte(BYTE *src, BYTE *dst, int n);

void DesAlgo(BYTE *key, BYTE *src, BYTE *dst, BYTE bEncrypt);

void Shift(CDStruct *pCD, int iCircle, BYTE bEncrypt);
void RShift(BYTE *buf28, int nShift);
void LShift(BYTE *buf28, int nShift);
void fFunction(BYTE *R, BYTE*K, BYTE *result);
void S_Change(BYTE *src, BYTE *result);
void SelectExchangeFromTable(int nTable, BYTE *src, BYTE *dst);

// Bytes to Ansi String eg: 0X30 0X55 --> "3055"
STDAPI_(void) BytesToStr(unsigned char * pszBits, unsigned int nBitLen, 
				unsigned char * pszOutStr, unsigned int nSize);
// Ansi String to Bytes String eg: "3055" --> 0X30 0X55
STDAPI_(void) StrToBytes(unsigned char * pszStr,  
				unsigned char * pszOutBytes, unsigned int nSize);


#endif