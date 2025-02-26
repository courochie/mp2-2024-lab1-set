// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw ("Length can't be negative");
	BitLen = len;
	MemLen = (len + 8*sizeof(TELEM) - 1)/(8* sizeof(TELEM));
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = 0;	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8*sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen || n < 0) throw ("Invalid index");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n < 0) throw ("Invalid index");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n < 0) throw ("Invalid index");
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (*this != bf) {
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	bool flag = true;
	for (int i = 0; i < std::min(MemLen, bf.MemLen); i++) {
		flag &= pMem[i] == bf.pMem[i];
	}
	return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res = TBitField(std::max(BitLen, bf.BitLen));
	if (MemLen >= bf.MemLen) {
		for (int i = 0; i < bf.MemLen; i++) {
			res.pMem[i] = pMem[i] | bf.pMem[i];
		}
		for (int i = bf.MemLen; i < MemLen; i++) {
			res.pMem[i] = pMem[i];
		}
	}
	else {
		for (int i = 0; i < MemLen; i++) {
			res.pMem[i] = pMem[i] | bf.pMem[i];
		}
		for (int i = MemLen; i < bf.MemLen; i++) {
			res.pMem[i] = pMem[i];
		}
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res = TBitField(std::max(BitLen, bf.BitLen));
	if (MemLen >= bf.MemLen) {
		for (int i = 0; i < bf.MemLen; i++) {
			res.pMem[i] = pMem[i] & bf.pMem[i];
		}
		for (int i = bf.MemLen; i < MemLen; i++) {
			res.pMem[i] = 0;
		}
	}
	else {
		for (int i = 0; i < MemLen; i++) {
			res.pMem[i] = pMem[i] & bf.pMem[i];
		}
		for (int i = MemLen; i < bf.MemLen; i++) {
			res.pMem[i] = 0;
		}
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res = TBitField(BitLen);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~pMem[i];
	}
	res.pMem[MemLen - 1] = res.pMem[MemLen - 1] & ((1 << (BitLen % (8 * sizeof(TELEM)))) - 1);

	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int size;
	std::string input;
	istr >> size >> input;
	bf = TBitField(size);

	for (int i = 0; i < std::min((int)input.size(), size); i++){
		if (input[i] == '1') bf.SetBit(i);
		else if (input[i] != '0') throw ("Nonbinary string in input");
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	std::string bitstr;
	for (int i = 0; i < bf.GetLength(); i++) {
		bitstr.push_back(bf.GetBit(i)+ '0');
	}
	
	return ostr << bitstr;
}
