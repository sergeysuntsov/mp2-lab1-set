// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <=0)  throw(-5);
	else {
		BitLen = len;
		//MemLen = (len + (sizeof(int) * 8- 2)) / (sizeof(int) * 8);
		MemLen = GetMemIndex(len) + 1;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) { pMem[i] = 0; }
	}
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n /( sizeof(int) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (n % (sizeof(int) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n <= BitLen) && (n >= 0)) {
	int g, m;
	g = GetMemIndex(n);
	m = GetMemMask(n);
	int b;
	b = pMem[g];
	b = b >> m;
	b = b & 1;
	if (b == 0) { 
		b = 1;
		b <<= m;
		pMem[g] |= b;
	}
}
	else throw(-4);
}

void TBitField::ClrBit(const int n) // очистить бит
{	
	if ((n <= BitLen) && (n >= 0)) {
		int g, m, now;
		g = GetMemIndex(n);
		m = GetMemMask(n);
		now = pMem[g];
		now = now >> m;
		now = now & 1;
		if (now == 1) {
			now <<= m;
			pMem[g] &= ~now;
		}
	}
	else throw(-3);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n <= BitLen) && (n >= 0)) {
		int g, m;
		g = GetMemIndex(n);
		m = GetMemMask(n);		
		int mas = 1;
		int b = pMem[g];
		b = b >> m;
		b = b & mas;
		return (b);
	}
	else throw(-3);
	
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (MemLen != bf.MemLen) {
		if (pMem != NULL) delete pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL) {
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen) res = 0;
	else
		for (int i = 0; i<MemLen; i++)
			if (pMem[i] != bf.pMem[i]) { res = 0; break; }
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen) return 1;
	else
	{
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i]) { return 1; }
	}
	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i, len = BitLen;
	if (bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++) temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++) temp.pMem[i] |= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i, len = BitLen;
	if (bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++) temp.pMem[i] = pMem[i];
	for (i = 0; i < bf.MemLen; i++) temp.pMem[i] &= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	int i, len = BitLen;
	for (i = 0; i < len; i++) {
		if (GetBit(i) == 0) { SetBit(i); }
		else { ClrBit(i); }
	}
	return *this;
	
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int a = 0;
	for (int i = bf.BitLen - 1; i >= 0; i--){
		istr >> a;
		if (a == 1){
			bf.SetBit(i);
		}
	}
	cout << endl;
	return istr;
}
//+
ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = bf.BitLen - 1; i >= 0; i--){
		ostr << bf.GetBit(i) << ' ';
	}
	ostr << endl;
	return ostr;
}

