#include "stdafx.h"
#include "encrypt/DesEnc.h"

//STD DES
////////////////////////////////////////////////////////
// g_nOut: number of output bits from each table
int g_nOut[6] = {64,64,48,32,56,48};

int TABLE_shift[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
                       
////////////////////////////////////////////////////////
// TABLE_ip: INITIAL PERMUTATION
int TABLE_ip[64] = {
		57,49,41,33,25,17, 9, 1,59,51,43,35,27,19,11, 3,
		61,53,45,37,29,21,13, 5,63,55,47,39,31,23,15, 7,
		56,48,40,32,24,16, 8, 0,58,50,42,34,26,18,10, 2,
		60,52,44,36,28,20,12, 4,62,54,46,38,30,22,14, 6};
////////////////////////////////////////////////////////
// TABLE_ip_1: INVERSE INITIAL PERMUTATION
int TABLE_ip_1[64] = {
		39, 7,47,15,55,23,63,31,38, 6,46,14,54,22,62,30,
		37, 5,45,13,53,21,61,29,36, 4,44,12,52,20,60,28,
		35, 3,43,11,51,19,59,27,34, 2,42,10,50,18,58,26,
		33, 1,41, 9,49,17,57,25,32, 0,40, 8,48,16,56,24} ;

////////////////////////////////////////////////////////
// p: Permutation Function P
int TABLE_p[32] = {
		15, 6,19,20,28,11,27,16, 0,14,22,25, 4,17,30, 9,
		 1, 7,23,13,31,26, 2, 8,18,12,29, 5,21,10, 3,24};
////////////////////////////////////////////////////////
// pc_1: Permuted Choice1
int TABLE_pc_1[56] = {
		56,48,40,32,24,16, 8, 0,57,49,41,33,25,17, 9, 1,
		58,50,42,34,26,18,10, 2,59,51,43,35,62,54,46,38,
		30,22,14, 6,61,53,45,37,29,21,13, 5,60,52,44,36,
		28,20,12, 4,27,19,11, 3};
////////////////////////////////////////////////////////
// TABLE_e: Expansion Function
int TABLE_ePro[48] = {
		31, 4, 0, 1, 2, 3, 3, 8, 4, 5, 6, 7, 7,12, 8, 9,
		10,11,11,16,12,13,14,15,15,20,16,17,18,19,19,24,
		20,21,22,23,23,28,24,25,26,27,27, 0,28,29,30,31} ;
////////////////////////////////////////////////////////
// pc_2: Permuted Choice2
int TABLE_pc_2Pro[48] = {
		13, 4,16,10,23, 0, 2, 9,27,14, 5,20,22, 7,18,11,
		 3,25,15, 1, 6,26,19,12,40,54,51,30,36,46,29,47,
		39,50,44,32,43,52,48,38,55,33,45,31,41,49,35,28} ;
////////////////////////////////////////////////////////
// s: Selection Function (s1...s8)
BYTE TABLE_s[8][64] = 
{
		{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7, 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8, 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13},
		{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10, 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5, 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9},
		{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7, 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12},
		{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4, 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14},
		{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6, 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3},
		{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8, 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6, 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13},
		{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6, 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2, 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12},
		{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7, 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2, 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8, 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}
};
////////////////////////////////////////////////////////
// TABLE_shift: Left SHifts of C and D blocks

void BitXor(BYTE *Data1, BYTE *Data2, int Len, BYTE *Dest)
{
	int i;

	for (i = 0; i < Len; i++)	Dest[i] = Data1[i] ^ Data2[i];
}

void BitToByte(BYTE *src, BYTE *dst, int n)
{
	int		i;
	BYTEStruct 	*BS;
	for (i=0; i<n; i++)
	{
		BS = (BYTEStruct *)(&src[i]);

		dst[i*8+7] = BS->b0;
		dst[i*8+6] = BS->b1;
		dst[i*8+5] = BS->b2;
		dst[i*8+4] = BS->b3;
		dst[i*8+3] = BS->b4;
		dst[i*8+2] = BS->b5;
		dst[i*8+1] = BS->b6;
		dst[i*8+0] = BS->b7;
	}
}

void ByteToBit(BYTE *src, BYTE *dst, int n)
{
	int	i;
	BYTEStruct 	*BS;
	for (i=0; i<n; i++)
	{
		BS = (BYTEStruct *)(&dst[i]);
		BS->b0 = src[i*8+7];
		BS->b1 = src[i*8+6];
		BS->b2 = src[i*8+5];
		BS->b3 = src[i*8+4];
		BS->b4 = src[i*8+3];
		BS->b5 = src[i*8+2];
		BS->b6 = src[i*8+1];
		BS->b7 = src[i*8+0];
	}
}

STDAPI_(int) HexStrToBytes(char *strSou, BYTE *BytDes, int bytCount)
{
	int i;
	int len;
	BYTE HighByte,LowByte;

	len = strlen(Trim(strSou));
	if (bytCount * 2 < len) len = bytCount * 2;

	LowToUpper (strSou,len);
	if ((len-len/2*2)==0)
	{
		for (i = 0; i < len; i += 2)
		{
			HighByte = strSou[i];
			LowByte  = strSou[i + 1];

			if (HighByte > 0x39) 	HighByte -= 0x37;
			else	HighByte -= 0x30;
						
			if (LowByte > 0x39)	LowByte -= 0x37;
			else	LowByte -= 0x30;
				
			BytDes[i / 2] = (HighByte << 4) | LowByte;
		}
		for (; i < bytCount * 2; i += 2)
		{
			BytDes[i / 2] = 0;
		}
		return (len / 2);
	}
	else
		return 0;
}

STDAPI_(int) BytesToHexStr(char *strDes, BYTE *bytSou, int bytCount)
{
	int i;
	int len;
	BYTE HighByte, LowByte;
	
	for (i = 0; i < bytCount; i++)
	{
		HighByte = bytSou[i] >> 4;
		LowByte  = bytSou[i] & 0x0F;
		
		HighByte += 0x30;
		if (HighByte > 0x39)	strDes[i * 2] = HighByte + 0x07;
		else	strDes[i * 2] = HighByte;

		LowByte += 0x30;
		if (LowByte > 0x39) 	strDes[i * 2 + 1] = LowByte + 0x07;
		else	strDes[i * 2 + 1] = LowByte;
	}
	len = strlen(strDes);
	return len;
}

STDAPI_(char*) Trim(char *In)
{
	int i,len;

	len = strlen(In);

	for (i=len-1; i>=0; i--)
	{
		if( In[i]!=0x20 && In[i]!=0x00 ) break;
	}

	for (i++; i<len; i++) In[i] = 0x00;

	return In;
}

STDAPI_(void) LowToUpper(char *Str, int lenth)
{
	int i;
	for (i=0;i<lenth;i++)	*(Str+i)=toupper(*(Str+i));
}

void DesAlgo(BYTE *key, BYTE *src, BYTE *dst, BYTE bEncrypt)
{
	int  i, j, k;
	BYTE input[64];			// store input data
	BYTE output[64];		// store output data
	BYTE KEY[64];			// store key
	BYTE tempA[7];
	BYTE tempB[64];
	BYTE tempC[8];
	BYTE tempD[32];
	BYTE K[48];
	BYTE fResult[32];
	
	LRStruct LR;			// store L[i],R[i] --- temp data after every circle
	CDStruct CD;			// store C[i],D[i] --- temp Result of Shift()

	BitToByte(src, input, 8); // src (64 bits) ==>input (64 Bytes)
	SelectExchangeFromTable(IP_TABLE,input,(BYTE*)&LR);  // INPUT ==> IP ==> L0,R0

	BitToByte(key, KEY, 8); // key (64 bits) ==>KEY (64 Bytes)
	SelectExchangeFromTable(PC1_TABLE,KEY,(BYTE*)&CD);  // KEY ==> PC_1 ==> C0,D0

	for (i=0; i<16; i++)
	{
		// get K(i+1)
		Shift(&CD, i, bEncrypt);	// C[i],D[i] ==> shifting ==> C[i+1],D[i+1]
		ByteToBit((BYTE*)&CD,tempA,7);
		SelectExchangeFromTable(PC2_TABLE,(BYTE*)&CD, K);  //  C[i+1],D[i+1] ==> PC_2 ==> K[i+1]
		for(k=0; k<8; k++)
		{
			tempB[k*8+0]=0;
			tempB[k*8+1]=0;
			for (j=0; j<6; j++)	tempB[k*8+(j+2)]=K[k*6+j];
		}
		ByteToBit(tempA,tempC,8);
		// fResult[i] <== f( R[i], K[i+1] )
		fFunction(LR.R, K, fResult);
		// fResult[i] <== L[i] .XOR. f[i]
		BitXor (LR.L, fResult, 32, fResult);

		// L(i+1) <== R[i]
		memcpy(LR.L, LR.R, sizeof(BYTE)*32);
		// R(i+1) <== fResult[i]
		memcpy(LR.R, fResult, sizeof(BYTE)*32);
	}
	// LR <== RL
	memcpy(tempD, LR.L,sizeof(BYTE)*32);
	memcpy(LR.L, LR.R,sizeof(BYTE)*32);
	memcpy(LR.R, tempD,sizeof(BYTE)*32);

	SelectExchangeFromTable(IP_1_TABLE, (BYTE*)&LR, output);  // L16R16 ==> IP_1 ==> output
	ByteToBit(output, dst, 8);// dst (64 bits) <==  output (64 Bytes)
}

 
void SelectExchangeFromTable(int nTable, BYTE *src, BYTE *dst)
{
	static int * table[6] = {TABLE_ip, TABLE_ip_1, TABLE_ePro, TABLE_p, TABLE_pc_1, TABLE_pc_2Pro};

	int i, nOut;
	int *pTable;
	
	nOut = g_nOut[nTable];
	pTable = table[nTable];
	
	for (i=0; i< nOut; i++)		dst[i] = src[pTable[i]];
}

void Shift(CDStruct *pCD, int iCircle, BYTE bEncrypt)
{
	
	if ( 0 == bEncrypt)
	{
		if ( iCircle != 0)
		{
			RShift(pCD->C, TABLE_shift[iCircle]);
			RShift(pCD->D, TABLE_shift[iCircle]);
		}
	}
	else
	{
		LShift(pCD->C, TABLE_shift[iCircle]);
		LShift(pCD->D, TABLE_shift[iCircle]);
	}
}

// 28 bytes Right shift nShift time(s)
void RShift(BYTE *buf28, int nShift)
{
	int i;
	BYTE temp[2];

	for (i=0; i<nShift; i++)		temp[(nShift-1)-i] = buf28[27-i];

	for (i=27; i>=nShift; i--)		buf28[i] = buf28[i-nShift];

	for (i=0; i<nShift; i++)		buf28[i] = temp[i];

}

// 28 bytes Left shift nShift time(s)
void LShift(BYTE *buf28, int nShift)
{
	int i;
	BYTE temp[2];

	for (i=0; i<nShift; i++)		temp[i] = buf28[i];

	for (i=nShift; i<28; i++)		buf28[i-nShift] = buf28[i];

	for (i=0; i<nShift; i++)		buf28[27-(nShift-1)+i] = temp[i];

}

// result <== f( R, K )
void fFunction(BYTE *R, BYTE *K, BYTE *result)
{
	BYTE temp[48];	// store temp data 
	BYTE SResult[32];	// store result of S_Change
	
	// R ==> E_TABLE ==> temp
	SelectExchangeFromTable(E_TABLE, R, temp);

	// temp .XOR. K ==> temp
	BitXor(K,temp,48,temp);

	// temp ==> S_Change ==> SResult
	S_Change(temp, SResult);

	// SResult ==> P_TABLE ==> result
	SelectExchangeFromTable(P_TABLE, SResult, result);
}


// src ==> S_Change ==> src
void S_Change(BYTE *src, BYTE *result)
{
	BYTE 	sTemp;
	BYTE 	nPos;
	int 	i;
	BYTEStruct	*rBSPos;
	
	for (i=0; i<8; i++)
	{
		rBSPos = (BYTEStruct *)(&nPos) ;
		nPos = 0;

		rBSPos->b0 = src[i*6+5];
		rBSPos->b1 = src[i*6+4];
		rBSPos->b2 = src[i*6+3];
		rBSPos->b3 = src[i*6+2];
		rBSPos->b4 = src[i*6+1];
		rBSPos->b5 = src[i*6+0];
		if (i%2==0)	sTemp= TABLE_s[i][nPos]*16;		// S Table Result ==> High 4 bits of sTemp
		else
		{	// S Table Result ==> Low 4 bits of sTemp
			sTemp += TABLE_s[i][nPos];
			// sTemp Expand to 8 bytes
			BitToByte(&sTemp, &result[i/2*8], 1);
		}
	}
}


STDAPI_(void) EnDES(BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst)
{
	DesAlgo(pbyKey, pbySrc, pbyDst, 1);
}

STDAPI_(void) DeDES(BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst)
{
	DesAlgo(pbyKey, pbySrc, pbyDst, 0);
}

STDAPI_(void) En3DES(BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst)
{
	BYTE LKey[8];
	BYTE RKey[8];
	BYTE TmpDest[8];

	memcpy(LKey,pbyKey,8);
	memcpy(RKey,pbyKey+8,8);
	
	EnDES(LKey,pbySrc,pbyDst);
	DeDES(RKey,pbyDst,TmpDest);
	EnDES(LKey,TmpDest,pbyDst);
}

STDAPI_(void) De3DES(BYTE *pbyKey, BYTE *pbySrc, BYTE *pbyDst)
{
	BYTE LKey[8];
	BYTE RKey[8];
	BYTE TmpDest[8];

	memcpy(LKey,pbyKey,8);
	memcpy(RKey,pbyKey+8,8);
	
	DeDES(LKey,pbySrc,pbyDst);
	EnDES(RKey,pbyDst,TmpDest);
	DeDES(LKey,TmpDest,pbyDst);
}

STDAPI_(void) DefuseKey(BYTE *key, BYTE *factor, BYTE *DefusedKey)
{

	BYTE xornum[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	BYTE XorFactor[8];
	
	En3DES(key,factor,DefusedKey);
	
	BitXor(factor,xornum,8,XorFactor);

	En3DES(key,XorFactor,DefusedKey+8);
}

STDAPI_(void) StrDefuseKey(char *Key, char *factor, char *DefusedKey)
{
	BYTE bytKey[16];
	BYTE bytFactor[8];
	BYTE bytDefusedKey[16];
	
	HexStrToBytes(Key,bytKey,16);
	HexStrToBytes(factor,bytFactor,8);

	DefuseKey(bytKey, bytFactor, bytDefusedKey);

	BytesToHexStr(DefusedKey,bytDefusedKey,16);

}

STDAPI_(void) StrEnDES(char *Key, char *Src, char *Dst)
{
	BYTE bytKey[8];
	BYTE bytSrc[8];
	BYTE bytDst[8];

	HexStrToBytes(Key,bytKey,8);
	HexStrToBytes(Src,bytSrc,8);
	DesAlgo(bytKey, bytSrc, bytDst, 1);
	BytesToHexStr(Dst,bytDst,8);
}

STDAPI_(void) StrDeDES(char *Key, char *Src, char *Dst)
{
	BYTE bytKey[8];
	BYTE bytSrc[8];
	BYTE bytDst[8];

	HexStrToBytes(Key,bytKey,8);
	HexStrToBytes(Src,bytSrc,8);
	DesAlgo(bytKey, bytSrc, bytDst, 0);
	BytesToHexStr(Dst,bytDst,8);
}

STDAPI_(void) StrEn3DES(char *Key, char *Src, char *Dst)
{
	BYTE LKey[8];
	BYTE RKey[8];
	BYTE bytSrc[8];
	BYTE bytDst[8];

	HexStrToBytes(Key,     LKey,  8);
	HexStrToBytes(&Key[16],RKey,  8);
	HexStrToBytes(Src,     bytSrc,8);

	EnDES(LKey,bytSrc,bytDst);
	DeDES(RKey,bytDst,bytSrc);
	EnDES(LKey,bytSrc,bytDst);

	BytesToHexStr(Dst,bytDst,8);
}

STDAPI_(void) StrDe3DES(char *Key, char *Src, char *Dst)
{
	BYTE LKey[8];
	BYTE RKey[8];
	BYTE bytSrc[8];
	BYTE bytDst[8];

	HexStrToBytes(Key,     LKey,  8);
	HexStrToBytes(&Key[16],RKey,  8);
	HexStrToBytes(Src,     bytSrc,8);

	DeDES(LKey,bytSrc,bytDst);
	EnDES(RKey,bytDst,bytSrc);
	DeDES(LKey,bytSrc,bytDst);

	BytesToHexStr(Dst,bytDst,8);
}

STDAPI_(void) MAC(BYTE *pbyKey, BYTE *pbySrc, int iSrcLen, BYTE *pbyInit, BYTE *pbyDst)
{ 
	BYTE	pbySrcTemp[8];
	BYTE	pbyInitData[8];
	BYTE	pbyDeaSrc[8];
	int	i, j, n, iAppend;
	
	n = iSrcLen / 8 + 1;
	iAppend = 8 - ( n * 8 - iSrcLen);

	memcpy (pbyInitData,pbyInit,8);
	for(i = 0; i < n; i++)
	{
		memset(pbySrcTemp, 0x00, 8);
		if(i==(n-1))
		{
			memcpy(pbySrcTemp, &pbySrc[i*8], iAppend); 
			pbySrcTemp[iAppend] = 0x80;
			for (j = iAppend + 1; j < 8; j++) pbySrcTemp[j] = 0x00;
		}
		else memcpy(pbySrcTemp, &pbySrc[i*8], 8); 

		BitXor(pbySrcTemp, pbyInitData, 8, pbyDeaSrc);
		memset(pbyInitData, 0x00, 8);
		EnDES(pbyKey, pbyDeaSrc, pbyInitData);
	}
	memcpy(pbyDst, pbyInitData, 4);
}

STDAPI_(void) MAC16(BYTE *pbyKey, BYTE *pbySrc, int iSrcLen, BYTE *pbyInit, BYTE *pbyDst)
{ 
	BYTE	pbySrcTemp[8];
	BYTE	pbyInitData[8];
	BYTE	pbyDeaSrc[8];
	BYTE	KeyA[8];
	BYTE	KeyB[8];
	int	i, j, n, iAppend;
	
	n = iSrcLen / 8 + 1;
	iAppend = 8 - ( n * 8 - iSrcLen);

	memcpy (pbyInitData,pbyInit,8);
	memcpy (KeyA,pbyKey,8);
	memcpy (KeyB,&pbyKey[8],8);

	for(i = 0; i < n; i++)
	{
		memset(pbySrcTemp, 0x00, 8);
		if(i==(n-1))
		{
			memcpy(pbySrcTemp, &pbySrc[i*8], iAppend); 
			pbySrcTemp[iAppend] = 0x80;
			for (j = iAppend + 1; j < 8; j++) pbySrcTemp[j] = 0x00;
		}
		else memcpy(pbySrcTemp, &pbySrc[i*8], 8); 

		BitXor(pbySrcTemp, pbyInitData, 8, pbyDeaSrc);
		memset(pbyInitData, 0x00, 8);
		EnDES(KeyA, pbyDeaSrc, pbyInitData);
	}
	DeDES(KeyB, pbyInitData, pbyDeaSrc);
	EnDES(KeyA, pbyDeaSrc, pbyInitData);
	memcpy(pbyDst, pbyInitData, 4);
}

STDAPI_(void) StrMAC(char *Key, char *Src, char* InitData, char *Dst)
{
	BYTE bytKey[8] = {0};
	BYTE bytSrc[8192] = {0};
	BYTE bytDst[4] = {0};
	BYTE bytInit[8] = {0};
	int len;

	len = strlen(Src);
	if (len % 2 != 0)
	{
		return;
	}
	len = len / 2;

	HexStrToBytes(Key,     bytKey,  8);
	HexStrToBytes(InitData,bytInit, 8);
	HexStrToBytes(Src,     bytSrc,  len);

	MAC(bytKey, bytSrc, len, bytInit, bytDst);
	BytesToHexStr(Dst,bytDst,4);
}


STDAPI_(void) Str3MAC(char *Key, char *Src, char *Dst)
{
	BYTE byKey[16];
	BYTE bytSrc[256] = {0};
	BYTE pbyInit[8];
	BYTE pbyDst[8];
	int iSrcLen = 0;

	if (NULL == Src || NULL == Key)
	{
		return ;
	}

	iSrcLen = strlen(Src) - 16;
	if (iSrcLen < 0 || iSrcLen % 2 != 0)
	{
		return ;
	}
	HexStrToBytes(Key,     byKey,  16);
	HexStrToBytes(&Src[0],pbyInit,  8);
	iSrcLen /= 2;
	HexStrToBytes(&Src[16],     bytSrc, iSrcLen);


	MAC16(byKey, bytSrc, iSrcLen, pbyInit, pbyDst);

	BytesToHexStr(Dst,pbyDst,4);
}
STDAPI EncDecFile(BYTE *key, char *InFile, char *OutFile, int EncDec)
{
	return 0;
}


STDAPI_(void) ByteXorByteEx(BYTE X[],BYTE Y[],BYTE result[],int nCount)
{
	int icount;
	BYTE TEMP[256];
	for(icount=0;icount<nCount;icount++)
	{
		TEMP[icount]=X[icount]^Y[icount];
	}
	memcpy(result,TEMP,nCount);
}

STDAPI_(void) StrByteXorByte(BYTE X[],BYTE Y[],char * result)
{
	int nCount = strlen((const char *)X) / 2;
	BYTE byX[256] = {0};
	BYTE byY[256] = {0};
	BYTE byResult[256] = {0};

	HexStrToBytes((char *)X,     byX,  nCount);
	HexStrToBytes((char *)Y,     byY,  nCount);

	ByteXorByteEx(byX, byY, byResult, nCount);
	
	BytesToHexStr((char *)result, (BYTE*)byResult, nCount);
}

STDAPI_(void) StrEn3DES_ECB(char *Key, char *Src, char *Dst)
{
	BYTE LKey[8];
	BYTE RKey[8];
	BYTE bytSrc[256];
	BYTE bytDst[256];
	int nSrcLen = 0;
	int nGrp = 0;

	nSrcLen = strlen(Src) / 2;

	memset(bytSrc, 0, sizeof(bytSrc));
	HexStrToBytes(Key,     LKey,  8);
	HexStrToBytes(&Key[16],RKey,  8);
	HexStrToBytes(Src,     bytSrc,nSrcLen);

	bytSrc[nSrcLen] = 0x80;

	if (nSrcLen % 8)
	{
		nGrp = nSrcLen / 8 + 1;
	}
	else
	{
		nGrp = nSrcLen / 8;
	}

	memset(bytDst, 0, sizeof(bytDst));
	for (int nCur = 0; nCur < nGrp; nCur++)
	{
		EnDES(LKey,(unsigned char *)&bytSrc[nCur * 8],(unsigned char *)&bytDst[nCur * 8]);
		DeDES(RKey,(unsigned char *)&bytDst[nCur * 8],(unsigned char *)&bytSrc[nCur * 8]);
		EnDES(LKey,(unsigned char *)&bytSrc[nCur * 8],(unsigned char *)&bytDst[nCur * 8]);
	}

	BytesToHexStr(Dst,bytDst,nGrp * 8);
}



#include <math.h>
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )

// get char hign 8 bits eg. '0' return '3'
unsigned char HignBitToChar(unsigned char szChar)
{
	unsigned char szH4 = (unsigned char)(szChar >> 4 & 0X0F);

	if (szH4 >= 0 && szH4 <= 9)
	{
		return (unsigned char)(szH4 + 0X30);
	}
	else if (szH4 >= 'A' && szH4 <= 'F')
	{
		return (unsigned char)(szH4 - 0X0A + 'A');
	}
	else if (szH4 >= 0X0A && szH4 <= 0X0F)
	{
		return (unsigned char)(szH4 - 0X0A + 'A');
	}
	else 
	{
		return (unsigned char)'0';
	}
}
// get char low 8 bits eg. '0' return '3'
unsigned char LowBitToChar(unsigned char szChar)
{
	unsigned char szL4 = (unsigned char)(szChar & 0X0F);

	if (szL4 >= 0 && szL4 <= 9)
	{
		return (unsigned char)(szL4 + 0X30);
	}
	else if (szL4 >= 0X0A && szL4 <= 0X0F)
	{
		return (unsigned char)(szL4 - 0X0A + 'A');
	}
	else if (szL4 >= 'A' && szL4 <= 'F')
	{
		return (unsigned char)(szL4 - 0X0A + 'A');
	}
	else 
	{
		return '0';
	}
}
unsigned char CharToBit(unsigned char szChar)
{
	unsigned char szBit = 0X00;

	if (szChar >= 0X30 && szChar <= 0X39)
	{
		szBit = (unsigned char)(szChar - 0X30);
	}
	else if (szChar >= 0X41 && szChar <= 0X5A)
	{
		szBit = (unsigned char)(szChar - 0X41 + 0X0A);
	}
	else if (szChar >= 0X61 && szChar <= 0X7A)
	{
		szBit = (unsigned char)(szChar - 0X61 + 0X0A);
	}

	return szBit;
}
unsigned char GetOneBit(unsigned char szH8, unsigned char szL8)
{
	unsigned char szBit = 0X00;

	szBit = (unsigned char)CharToBit((unsigned char)(szH8)) << 4 | CharToBit((unsigned char)szL8);
	return szBit;
}
unsigned char BitToChar(unsigned char szBit)
{
	if (szBit >= 0X00 && szBit <= 0X09)
	{
		return (unsigned char)(0X30 | szBit);
	}
	else if (szBit >= 0X0A && szBit <= 0X0F)
	{
		return (unsigned char)(0X41 + szBit - 0X0A);
	}
	else 
	{
		return (unsigned char)(0X30);
	}
}
// Bytes to Ansi String eg: 0X30 0X55 --> "3055"
STDAPI_(void)  BytesToStr(unsigned char * pszBits, unsigned int nBitLen, 
				unsigned char * pszOutStr, unsigned int nSize)
{
	unsigned int nCur = 0;
	unsigned char *pszPos = pszOutStr;
	if (NULL == pszOutStr || nBitLen * 2 > nSize)
	{
		return ;
	}

	for (nCur = 0; nCur < nBitLen; nCur++)
	{
		*pszPos++ = BitToChar((unsigned char)((pszBits[nCur] & 0XF0) >> 4));
		*pszPos++ = BitToChar((unsigned char)((pszBits[nCur] & 0X0F)));
	}
	*pszPos = '\0';
}
// Ansic string to byte string eg. "0000" --> "303030"
STDAPI_(void) AnsiStrToByteStr(const char *pszAnsiIn, 
							unsigned char *pszByteStrOut, unsigned int nLen)
{
	char *pszPos = (char *)pszAnsiIn;
	unsigned int nCur = 0;
	unsigned char szTemp = ' ';
	memset(pszByteStrOut, 0, nLen);
	if (NULL == pszAnsiIn)
	{
		return;
	}
	for (;;)
	{
		szTemp = *pszPos++;
		if (szTemp == '\0')
		{
			break;
		}
		if (nCur >= nLen)
		{
			break;
		}
		
		pszByteStrOut[nCur++] = HignBitToChar(szTemp);
		pszByteStrOut[nCur++] = LowBitToChar(szTemp);
	}
	for (;;)
	{
		if (nCur >= nLen)
		{
			break;
		}
		
		pszByteStrOut[nCur++] = '0';
		pszByteStrOut[nCur++] = '0';
	}
}
// Ansi String to Bytes String eg: "3055" --> 0X30 0X55
STDAPI_(void) StrToBytes(unsigned char * pszStr,  
				unsigned char * pszOutBytes, unsigned int nSize)
{
	unsigned int nComLen = 0;
	int nCur = 0;
	unsigned char szCmdAPDU[512] = {0};
	unsigned char szCmd = 0;

	memset(pszOutBytes, 0, nSize);
	if (NULL == pszStr || '\0' == pszStr[0])
	{
		return;
	}
	nComLen = strlen((const char *)pszStr);
	if (nComLen % 2 != 0 && nComLen < 2)
	{
		return;
	}
	nComLen = 0;
	for (nCur = 0; pszStr[nCur] != '\0'; nCur += 2)
	{
		szCmd = GetOneBit(pszStr[nCur + 0], pszStr[nCur + 1]);;
		szCmdAPDU[nComLen++] = szCmd;
	}
	memcpy(pszOutBytes, szCmdAPDU, MIN(nComLen, nSize));
}
STDAPI_(long) HexStrToLong(char *SouStr, int iLen)
{
	int i;
	long ret = 0;
    BYTE temp;
	
	for (i=0; i<iLen; i++)
    {
        if      (SouStr[i] >= 0x61 && SouStr[i] <= 0x66)
            temp = SouStr[i] - 0x57;
        else if (SouStr[i] >= 0x41 && SouStr[i] <= 0x46)
            temp = SouStr[i] - 0x37;
        else if (SouStr[i] >= 0x30 && SouStr[i] <= 0x39)
            temp = SouStr[i] - 0x30;

		ret = ret + (long)(temp * pow(16.0,iLen-i-1));
    }    
	return ret;
}

void HexStrToBytes(BYTE *SouStr, int Len, BYTE *DesByte)
{

	int i ;
	BYTE HighByte, LowByte;
  
	for (i = 0; i < Len; i += 2)
	{
		HighByte = SouStr[i];
		LowByte  = SouStr[i + 1];

        if  (HighByte >= 0x61 && HighByte <= 0x66)
            HighByte -= 0x57;
        else if (HighByte >= 0x41 && HighByte <= 0x46)
            HighByte -= 0x37;
        else if (HighByte >= 0x30 && HighByte <= 0x39)
            HighByte -= 0x30;
        
        if  (LowByte >= 0x61 && LowByte <= 0x66)
            LowByte -= 0x57;
        else if (LowByte >= 0x41 && LowByte <= 0x46)
            LowByte -= 0x37;
        else if (LowByte >= 0x30 && LowByte <= 0x39)
            LowByte -= 0x30;
	
		DesByte[i / 2] = (HighByte << 4) | LowByte;
	}
}

void BytesToHexStr(BYTE *SouByte, int Len, BYTE *DesStr)
{

	int i;
	BYTE HighByte, LowByte;
	
	for (i = 0; i < Len; i++)
	{
		HighByte = SouByte[i] >> 4;
		LowByte  = SouByte[i] & 0x0f ;
	    
		HighByte += 0x30;
		if (HighByte > 0x39) DesStr[i * 2] = HighByte + 0x07;
		else DesStr[i * 2] = HighByte;

		LowByte += 0x30;
		if (LowByte > 0x39) DesStr[i * 2 + 1] = LowByte + 0x07;
		else DesStr[i * 2 + 1] = LowByte;
	}
}


void strRevHexString(char *SouStr, int iLen, char *DesStr)
{
	int		i, j;

	if ( iLen % 2 != 0 ) exit(0);

	for (i=iLen-2, j=0; i>=0 ; i-=2,j+=2)
	{
		memcpy(&DesStr[j],&SouStr[i],2);
	}
}

STDAPI_(void) StrXor(BYTE* SouStr, int iLen, BYTE* DesStr)
{
	int i;
	BYTE temp=0;
	BYTE temp1[255];
	
	if ( iLen % 2 != 0 ) exit(0);

	HexStrToBytes(SouStr, iLen, temp1);
	for (i=0; i<iLen/2; i++) temp = temp ^ temp1[i];
	BytesToHexStr( &temp, 1, DesStr );
}

STDAPI_(long) LongToConverseHexStr(long nData, char *pszOut)
{
	long nTemp = nData;
	char szLongTemp[5] = {0};
	if (NULL == pszOut)
	{
		return -1;
	}
	memset(pszOut, 0, 8);
	memcpy(szLongTemp, &nTemp, 4);
	BytesToStr((unsigned char *)szLongTemp, 4, (unsigned char *)pszOut, 8);
	return 0;
}


STDAPI_(long) ConverseHexStrToLong(char *pszData, int nLen)
{
	long nTemp = 0;
	char szLongTemp[5] = {0};

	if (NULL == pszData || 8 != nLen)
	{
		return -1;
	}
	StrToBytes((unsigned char *)pszData, (unsigned char *)szLongTemp, 5);
	nTemp = *((long *)&szLongTemp);
	return nTemp;
}
