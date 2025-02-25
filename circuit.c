/*
	@author yuanluo2
	@brief  a digital circuit simulator in C99.
*/
#include <stdio.h>
#include <math.h>

#define INPUT
#define OUTPUT

int nmos(int gate, int source) {
	return (gate == 1 ? source : 0);
}

int pmos(int gate, int source) {
	return (gate == 0 ? source : 0);
}

int gate_not(int a) {
	return nmos(a, 0) || pmos(a, 1);
}

int gate_nand(int a, int b) {
	int d = nmos(b, 0);
	return nmos(a, d) || pmos(b, 1) || pmos(a, 1);
}

int gate_and(int a, int b) {
	int x = gate_nand(a, b);
	return gate_nand(x, x);
}

int gate_or(int a, int b) {
	int out1 = gate_nand(a, a);
	int out2 = gate_nand(b, b);
	return gate_nand(out1, out2);
}

int gate_nor(int a, int b) {
	int x = gate_or(a, b);
	return gate_not(x);
}

int gate_xor(int a, int b) {
	int out1 = gate_or(a, b);
	int out2 = gate_nand(a, b);
	return gate_and(out1, out2);
}

void half_adder(int a, int b, OUTPUT int* sum, OUTPUT int* carry) {
	*sum = gate_xor(a, b);
	*carry = gate_and(a, b);
}

void full_adder(int a, int b, int carryIn, OUTPUT int* sum, OUTPUT int* carryOut) {
	int sum1, sum2, carry1, carry2;
	
	half_adder(a, b, &sum1, &carry1);
	half_adder(carryIn, sum1, &sum2, &carry2);
	
	*sum = sum2;
	*carryOut = gate_or(carry1, carry2);
}

/* of course, this can be extended to 32-bit or 64-bit. */
void adder_16_bit(INPUT int a[16], INPUT int b[16], OUTPUT int out[16], OUTPUT int carry[16]) {
	full_adder(a[15], b[15], 0, &(out[15]), &(carry[15]));
	
	for (int i = 14; i >= 0; --i) {
		full_adder(a[i], b[i], carry[i + 1], &(out[i]), &(carry[i]));
	}
}

/* 
	m = 0 means adder
	m = 1 means subtractor. 
*/
void adder_subtractor_16_bit(INPUT int a[16], INPUT int b[16], OUTPUT int out[16], OUTPUT int carry[16], int m) {
	full_adder(a[15], gate_xor(b[15], m), m, &(out[15]), &(carry[15]));
	
	for (int i = 14; i >= 0; --i) {
		full_adder(a[i], gate_xor(b[i], m), carry[i + 1], &(out[i]), &(carry[i]));
	}
}

/* 
	only for signed number. assume a[16] + b[16], if we want to know if 
	the result is overflow, then just look at the sign bit and the most
	significant bit, only 16 cases.
	
	the a[0], b[0] means the sign bit, a[1], b[1] means the most significant bit,
	then:
	
	a[0]  b[0]  a[1]  b[1]  carry[0]  carry[1]   result
	 0     0     0     0       0         0
	 0     0     0     1       0         0
	 0     1     0     0       0         0
	 0     1     0     1       0         0
	 0     0     1     0       0         0
	 0     0     1     1       0         1       overflow
	 0     1     1     0       0         0
	 0     1     1     1       1         1
	 1     0     0     0       0         0
	 1     0     0     1       0         0
	 1     1     0     0       1         0       overflow
	 1     1     0     1       1         0       overflow
	 1     0     1     0       0         0
	 1     0     1     1       1         1
	 1     1     1     0       1         0       overflow
	 1     1     1     1       1         1
	 
	so use a xor gate on carry[0] and carry[1] could detect overflow.
	
*/
int adder_16_bit_overflow_flag(int carry[16]) {
	return gate_xor(carry[0], carry[1]);
}

int convert_16_bit_to_decimal_unsigned(int a[16]) {
	int result = 0;
	int y = 0;
	
	for (int i = 15; i >= 0; --i) {
		result += a[i] * pow(2, y);
		++y;
	}
	
	return result;
}

int convert_16_bit_to_decimal_signed(int a[16]) {
	int result = 0;
	int y = 0;
	
	if (a[0] == 0) {  /* positive. */
		for (int i = 15; i >= 1; --i) {
			result += a[i] * pow(2, y);
			++y;
		}
	}
	else {  /* negative. */
		for (int i = 15; i >= 1; --i) {
			result += (!(a[i]) * pow(2, y));
			++y;
		}
		
		result = -(result + 1);
	}
	
	return result;
}

/*
	gcc circuit.c -std=c99 -lm
*/
int main() {
	int a[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
    int b[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	int out[16];
	int carry[16];
	
	adder_subtractor_16_bit(a, b, out, carry, 1);
	
	printf("%d - \n", convert_16_bit_to_decimal_signed(a));
	printf("%d = \n", convert_16_bit_to_decimal_signed(b));
	printf("%d\n", convert_16_bit_to_decimal_signed(out));
	printf("overflow flag is %d\n", adder_16_bit_overflow_flag(carry));
	return 0;
}
