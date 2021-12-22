#include "dz1.h"

BigDecimal::BigDecimal(char* num){
	int count = 0, count1 = 0;
	bool signExist = false;

	if (num[0] == '-') sign = '-', count = 1, signExist = true;
	else if (num[0] == '+') sign = '+', count = 1, signExist = true;
	else sign = '+';

	numWhole = numFraction = 0;
	while ((num[count] != '\0') && (num[count] >= '0') && (num[count++] <= '9')) numWhole++;
	if (numWhole){
		this->whole = new int[numWhole];
		if (signExist) count = 1;
		else count = 0;
		while (count1 < numWhole)
			this->whole[count1++] = num[count++] - '0';
	}
	else{
		this->whole = new int[1];
		this->whole[0] = 0;
	}

	if ((whole[0] == 0) && (numWhole != 1)){
		whole = zeroClearBeginning(whole, numWhole);
		numWhole = arrayLength(whole);
	}

	if ((num[count] == '.') && (num[count + 1] >= '0') && (num[count + 1] <= '9')) {
		int indexSave = ++count;
		while ((num[count] >= '0') && (num[count++] <= '9')) numFraction++;
		this->fraction = new int[numFraction];
		count1 = 0, count = indexSave;
		while (count1 < numFraction)
			this->fraction[count1++] = num[count++] - '0';
		fraction = zeroClear(fraction, arrayLength(fraction));
		numFraction = arrayLength(fraction);
	}
	else
		this->fraction = nullptr;

	if (!fraction && (numWhole == 1) && whole[0] == 0)
		sign = '+';
}

BigDecimal::BigDecimal(int* Whole, int* Fraction){

	if (Whole && (Whole[0] < 0))
		sign = '-', Whole[0] = -Whole[0];
	else sign = '+';

	this->numWhole = arrayLength(Whole);
	if (this->numWhole){
		this->whole = new int[numWhole];
		for (int countWhole = 0; countWhole < this->numWhole; countWhole++)
			this->whole[countWhole] = Whole[countWhole];
	}
	else{
		this->whole = new int[1];
		this->whole[0] = 0;
	}

	this->numFraction = arrayLength(Fraction);
	if (this->numFraction){
		this->fraction = new int[numFraction];
		for (int countFrac = 0; countFrac < this->numFraction; countFrac++)
			this->fraction[countFrac] = Fraction[countFrac];
	}
	else
		this->fraction = nullptr;

	if ((whole[0] == 0) && (numWhole != 1)){
		whole = zeroClearBeginning(whole, numWhole);
		numWhole = arrayLength(whole);
	}

	if (fraction && (fraction[numFraction - 1] == 0)){
		fraction = zeroClear(fraction, numFraction);
		numFraction = arrayLength(fraction);
	}
}

int* zeroClearBeginning(int* arr, int arrSize){
	int zeros = 0;
	while ((arr[zeros] == 0) && (zeros < arrSize)) zeros++;
	if (zeros == arrSize){
		int* newArray1 = new int[1];
		newArray1[0] = 0;
		return newArray1;
	}
	if (zeros){
		int newArrSize = arrSize - zeros;
		int* newArray1 = new int[newArrSize];
		int count = 0;
		while (count < newArrSize) newArray1[count] = arr[count + zeros], count++;
		return newArray1;
	}
	else return arr;
}

int arrayLength(int* array){
	int arrSize = 0;
	if (!array) return 0;
	while ((array[arrSize] >= 0) && (array[arrSize] <= 9))
		arrSize++;
	return arrSize;
}

BigDecimal BigDecimal::abs(){
	return BigDecimal(this->whole, this->fraction);
}

bool BigDecimal::equals(BigDecimal* num){
	if ((sign == num->sign) && (numWhole == num->numWhole) && (numFraction == num->numFraction)){
		int count = 0;
		while ((count < numWhole) && (whole[count] == num->whole[count])) count++;
		if (count != numWhole) return false;

		count = 0;
		while ((count < numFraction) && (fraction[count] == num->fraction[count])) count++;
		if (count == numFraction) return true;
		else return false;
	}
	else return false;
}

bool BigDecimal::greater(BigDecimal* num){
	if (this->equals(num)) return false;
	if ((sign == '+') && (num->sign == '-')) return true;
	else if ((sign == '-') && (num->sign == '+')) return false;
	else{
		bool absGreater = false;
		if (numWhole > num->numWhole) absGreater = true;
		else if ((numWhole == num->numWhole) && (whole[0] > num->whole[0]))
			absGreater = true;
		else if ((numWhole == num->numWhole) && (whole[0] == num->whole[0])){
			int count = 0;
			while ((count < numWhole) && (whole[count] == num->whole[count])) count++;
			if (count == numWhole && fraction && num->fraction){ //whole je isti, postoje fraction-i
				count = 0;
				while ((count < MIN(numFraction, num->numFraction)) && (fraction[count] == num->fraction[count])) count++;
				if ((count == MIN(numFraction, num->numFraction) - 1) && (numFraction > num->numFraction))
					absGreater = true;
				else if (fraction[count] > num->fraction[count])
					absGreater = true;
			}
			else if (count == numWhole && fraction) //ako samo this ima fraction (num->fraction=nullptr)
				absGreater = true;
			else if (whole[count] > num->whole[count]) absGreater = true; //ako nije ceo whole isti, a trenutna cifra je veca kod this->whole-a
		}

		if ((absGreater && (sign == '+')) || (!absGreater && (sign == '-'))) return true;
		else return false;
	}
}

bool BigDecimal::less(BigDecimal* num){
	if ((this->greater(num)) || (this->equals(num))) return false;
	else return true;
}

BigDecimal BigDecimal::shl(int n){
	int* newFraction = new int[numFraction + n];
	int* newWhole = nullptr;
	int count1 = numFraction + n - 1, count2 = numWhole - 1;

	while (count1 >= n) newFraction[count1] = fraction[count1 - n], count1--;
	while ((count1 >= 0) && (count2 >= 0)) newFraction[count1--] = whole[count2--];

	if (count2 >= 0){
		newWhole = new int[numWhole - n];
		while (count2 >= 0) newWhole[count2] = whole[count2--];
	}
	else{
		while (count1 >= 0) newFraction[count1--] = 0;
		newWhole = new int[1];
		newWhole[0] = 0;
	}

	char* newNumArray = intToChr(newWhole, newFraction, sign);
	delete[] newWhole;
	delete[] newFraction;
	BigDecimal* newNum = new BigDecimal(newNumArray);
	delete[] newNumArray;
	return *newNum;
}

BigDecimal BigDecimal::shr(int n){
	int* newFraction = nullptr;
	int* newWhole = new int[numWhole + n];
	int count1 = 0, count2 = 0;

	while (count1 < numWhole) newWhole[count1] = whole[count1++];
	while ((count1 < (numWhole + n)) && (count2 < numFraction)) newWhole[count1++] = fraction[count2++];

	if (count2 != numFraction){
		newFraction = new int[numFraction - n];
		while (count2 < numFraction) newFraction[count2 - n] = fraction[count2++];
	}
	else
	while (count1 < (numWhole + n)) newWhole[count1++] = 0;

	char* newNumArray = intToChr(newWhole, newFraction, sign);
	delete[] newWhole;
	delete[] newFraction;
	BigDecimal* newNum = new BigDecimal(newNumArray);
	delete[] newNumArray;
	return *newNum;
}

BigDecimal BigDecimal::rmd(int* n){
	int* newWhole = new int[numFraction + numWhole];
	int count = 0;
	int countFrac = 0;
	while (count < numWhole) newWhole[count] = whole[count++];
	while (countFrac < numFraction) newWhole[count++] = fraction[countFrac++];
	*n = numFraction;
	char* newNumArray = intToChr(newWhole, nullptr, sign);
	delete[] newWhole;
	BigDecimal* newNum = new BigDecimal(newNumArray);
	delete[] newNumArray;
	return *newNum;
}

BigDecimal::~BigDecimal(){
	delete[] whole;
	delete[] fraction;
}

int* funcAdd(int* arrThis, int numThis, int* arrOther, int numOther, int& transfer, bool ifFraction){
	if (!arrThis) return arrOther;
	if (!arrOther) return arrThis;

	int countThis = numThis - 1, countOther = numOther - 1, countNew = MAX(numThis, numOther) - 1;
	int resault;
	int* newArray = new int[countNew + 1];

	if (ifFraction && (numThis != numOther)){ // za slucaj npr 1.125334 + 3.057 -> da prepise 334, pa da krene sabiranje od petice(1.125)
		if (MAX(countThis, countOther) == countThis){
			while (countThis != (MIN(countThis, countOther))) newArray[countNew--] = arrThis[countThis--];
		}
		else
		while (countOther != (MIN(countThis, countOther))) newArray[countNew--] = arrOther[countOther--];
	}

	while ((countThis >= 0) && (countOther >= 0)){
		resault = arrThis[countThis--] + arrOther[countOther--] + transfer;
		newArray[countNew--] = resault % 10;
		transfer = resault / 10;
	}

	while (countThis >= 0){
		resault = arrThis[countThis--] + transfer;
		newArray[countNew--] = resault % 10;
		transfer = resault / 10;
	}

	while (countOther >= 0){
		resault = arrOther[countOther--] + transfer;
		newArray[countNew--] = resault % 10;
		transfer = resault / 10;
	}

	if (ifFraction && (newArray[MAX(numThis, numOther) - 1] == 0)){ //brise nule sa kraja fraction-a (npr 32.34000 -> 32.34), uslov - da li je BAR poslednja cifra 0
		newArray = zeroClear(newArray, MAX(numThis, numOther)); //STA RADI SA STARIM PODACIMA("STARIM NIZOM")??
	}

	if (!ifFraction && transfer) { //ukoliko postoji prenos koji je van granica max(numThis,numOther) tj za 1 veci
		int arrLength = MAX(numThis, numOther) + 1;
		int* newArray1 = new int[arrLength];
		int count = 1;
		newArray1[0] = transfer;
		while (count < arrLength) newArray1[count] = newArray[count - 1], count++;

		delete[] newArray;
		newArray = newArray1;
		newArray1 = nullptr;
	}

	return newArray;
}

int* zeroClear(int* arr, int arrSize){
	int zeros = 0, count = arrSize - 1;
	while ((count >= 0) && (arr[count] == 0))
		zeros++, count--;
	count = 0;
	int* newArr = nullptr;
	if (arrSize != zeros)
		newArr = new int[arrSize - zeros];
	while (count < (arrSize - zeros))
		newArr[count] = arr[count++];
	return newArr;
}

int* funcSub(int* arrGreat, int numGreat, int* arrLess, int numLess, int& transfer, bool ifFraction){
	int countGreat = numGreat - 1, countLess = numLess - 1;
	int countNew = MAX(numGreat, numLess) - 1;
	int resault;
	int* newArray = nullptr;
	if (arrGreat || arrLess) newArray = new int[countNew + 1];

	if (ifFraction && (numGreat != numLess)){
		if (MAX(countGreat, countLess) == countGreat) //npr great(123.12333) - less(12.45) -> prepisuje trojke(333)
		while (countGreat != (MIN(countGreat, countLess))) newArray[countNew--] = arrGreat[countGreat--];
		else { //npr great(12.45) - less(8.12333) -> pozajmljuje da bi oduzeo poslednje tri cifre od necega(10-3,9-3,9-3 i transfer=-1)
			newArray[countNew--] = 10 - arrLess[countLess--], transfer = -1;
			while (countLess != (MIN(countGreat, countLess))) newArray[countNew--] = 9 - arrLess[countLess--];
		}
	}

	while ((countGreat >= 0) && (countLess >= 0)) {
		resault = arrGreat[countGreat--] + transfer - arrLess[countLess--];
		if (resault < 0)
			newArray[countNew--] = resault + 10, transfer = -1;
		else
			newArray[countNew--] = resault, transfer = 0;
	}

	while (countGreat >= 0){ // ako imamo 3219-202-> da prepise ovu trojku iz prvog broja
		resault = arrGreat[countGreat--] + transfer;
		if (resault < 0)
			newArray[countNew--] = resault + 10, transfer = -1;
		else
			newArray[countNew--] = resault, transfer = 0;
	}

	if (ifFraction && newArray && (newArray[MAX(numGreat, numLess) - 1] == 0)){ //brise nule sa kraja fraction-a (npr 32.34000 -> 32.34), uslov - da li je BAR poslednja cifra 0
		newArray = zeroClear(newArray, MAX(numGreat, numLess));
	}

	else if (!ifFraction && (newArray[0] == 0) && (numGreat != 1)){ //brise nule sa pocetka whole-a, npr 1000-999=0001, provera da li je BAR prvi clan nula
		int newArrSize = arrayLength(newArray);
		newArray = zeroClearBeginning(newArray, newArrSize);
	}

	return newArray;
}

BigDecimal BigDecimal::add(BigDecimal* other){
	int* newFraction = nullptr, *newWhole = nullptr, transfer = 0;
	char newNumSign;

	if (this->sign == other->sign){
		newNumSign = sign;
		newFraction = funcAdd(fraction, numFraction, other->fraction, other->numFraction, transfer, 1);
		newWhole = funcAdd(whole, numWhole, other->whole, other->numWhole, transfer, 0);
	}
	else {
		BigDecimal absThis(whole, fraction);
		BigDecimal absOther(other->whole, other->fraction);

		if ((absThis.equals(&absOther)) && (sign == other->sign)){
			int* zeroArr = new int[1];
			zeroArr[0] = 0;
			BigDecimal zero(zeroArr, nullptr);
			return zero;
		}

		BigDecimal greaterBD(((absThis.greater(&absOther)) ? (absThis) : (absOther)));
		BigDecimal lessBD(((absThis.less(&absOther)) ? (absThis) : (absOther)));

		if (greaterBD.equals(&absThis))
			newNumSign = this->sign;
		else
			newNumSign = other->sign;

		newFraction = funcSub(greaterBD.fraction, greaterBD.numFraction, lessBD.fraction, lessBD.numFraction, transfer, 1);
		newWhole = funcSub(greaterBD.whole, greaterBD.numWhole, lessBD.whole, lessBD.numWhole, transfer, 0);
	}

	char* newNumArray = intToChr(newWhole, newFraction, newNumSign);
	return BigDecimal(newNumArray);
}

BigDecimal BigDecimal::sub(BigDecimal* other){
	int* newFraction = nullptr, *newWhole = nullptr, transfer = 0;
	char newNumSign;

	BigDecimal absThis(whole, fraction);
	BigDecimal absOther(other->whole, other->fraction);

	if ((absThis.equals(&absOther)) && (sign == other->sign)){
		int* zeroArray = new int[0];
		zeroArray[0] = 0;
		return BigDecimal(zeroArray, newFraction);
	}

	BigDecimal greaterBD(((absThis.greater(&absOther)) ? (absThis) : (absOther)));
	BigDecimal lessBD(((absThis.less(&absOther)) ? (absThis) : (absOther)));

	if (sign == other->sign){
		if (greaterBD.equals(&absThis)){
			if (this->sign == '+') newNumSign = '+';
			else newNumSign = '-';
		}
		else{
			if (this->sign == '+') newNumSign = '-';
			else newNumSign = '+';
		}

		newFraction = funcSub(greaterBD.fraction, greaterBD.numFraction, lessBD.fraction, lessBD.numFraction, transfer, 1);
		newWhole = funcSub(greaterBD.whole, greaterBD.numWhole, lessBD.whole, lessBD.numWhole, transfer, 0);
	}
	else {
		if (this->sign == '+') newNumSign = '+';
		else newNumSign = '-';

		newFraction = funcAdd(greaterBD.fraction, greaterBD.numFraction, lessBD.fraction, lessBD.numFraction, transfer, 1);
		newWhole = funcAdd(greaterBD.whole, greaterBD.numWhole, lessBD.whole, lessBD.numWhole, transfer, 0);
	}

	char* newNumArray = intToChr(newWhole, newFraction, newNumSign);
	return BigDecimal(newNumArray);
}

char* intToChr(int* arrWhole, int* arrFrac, char newSign){
	int count = 0;
	int newArrLength = arrayLength(arrWhole) + arrayLength(arrFrac);
	if (newSign == '-') newArrLength++;
	char* newArray = nullptr;
	if (arrFrac)
		newArray = new char[newArrLength + 2], newArrLength++;
	else newArray = new char[newArrLength + 1];
	if (newSign == '-') newArray[count++] = '-';
	int countWhole = 0;
	while (countWhole < arrayLength(arrWhole)) newArray[count++] = arrWhole[countWhole++] + '0';
	int countFrac = 0;
	if (arrFrac) newArray[count++] = '.';
	while (countFrac < arrayLength(arrFrac)) newArray[count++] = arrFrac[countFrac++] + '0';
	newArray[count] = '\0';
	return newArray;
}

ostream& operator<<(ostream& output, const BigDecimal& bigDec){
	if (bigDec.sign == '-')
		output << bigDec.sign;
	int count = 0;
	while (count < bigDec.numWhole)
		output << bigDec.whole[count++];
	if (bigDec.fraction)
		output << '.', count = 0;
	while ((bigDec.fraction) && (count < bigDec.numFraction))
		output << bigDec.fraction[count++];
	return output;
}

BigDecimal::BigDecimal(const BigDecimal& num) {
	this->numFraction = num.numFraction;
	this->numWhole = num.numWhole;
	this->sign = num.sign;
	this->whole = new int[numWhole];
	for (int count = 0; count < numWhole; count++)
		this->whole[count] = num.whole[count];
	if (num.fraction){
		this->fraction = new int[numFraction];
		for (int count = 0; count < numFraction; count++)
			this->fraction[count] = num.fraction[count];
	}
	else
		this->fraction = nullptr;
}

BigDecimal& BigDecimal::operator= (const BigDecimal& num){
	this->numFraction = num.numFraction;
	this->numWhole = num.numWhole;
	this->sign = num.sign;
	delete[] whole;
	delete[] fraction;
	this->whole = new int[numWhole];
	for (int count = 0; count < numWhole; count++)
		this->whole[count] = num.whole[count];
	if (num.fraction){
		this->fraction = new int[numFraction];
		for (int count = 0; count < numFraction; count++)
			this->fraction[count] = num.fraction[count];
	}
	else
		this->fraction = nullptr;

	return *this;
} 

BigDecimal BigDecimal::operator- (const BigDecimal& other){
	BigDecimal Other(other);
	BigDecimal newNum = sub(&Other);
	return newNum;
}

bool BigDecimal::operator< (const BigDecimal& other) {
	BigDecimal newNum(other);
	return less(&newNum);
}

bool BigDecimal::operator> (const BigDecimal& other) {
	BigDecimal newNum(other);
	return greater(&newNum);
}

BigDecimal BigDecimal::operator+ (const BigDecimal& other){
	BigDecimal Other(other);
	BigDecimal newNum = add(&Other);
	return newNum;
}

BigDecimal BigDecimal::operator<< (int n){
	return BigDecimal(shl(n));
}

BigDecimal BigDecimal::operator>> (int n){
	return BigDecimal(shr(n));
}

BigDecimal BigDecimal::operator>>= (int& n){
	return BigDecimal(this->rmd(&n));
}

bool BigDecimal::operator== (const BigDecimal& num){
	BigDecimal newNum(num);
	return this->equals(&newNum);
}

bool BigDecimal::operator!= (const BigDecimal& num){
	BigDecimal newNum(num);
	return !(this->equals(&newNum));
}

bool BigDecimal::operator>= (const BigDecimal& num){
	BigDecimal newNum(num);
	return ((this->greater(&newNum)) || (this->equals(&newNum)));
}

bool BigDecimal::operator<= (const BigDecimal& num){
	BigDecimal newNum(num);
	return ((this->less(&newNum)) || (this->equals(&newNum)));
}

BigDecimal BigDecimal::operator- (){
	BigDecimal newNum(*this);
	if (newNum.sign != '-') newNum.sign = '-';
	return newNum;
}

BigDecimal BigDecimal::operator+ (){
	BigDecimal newNum(*this);
	if (newNum.sign != '+') newNum.sign = '+';
	return newNum;
}