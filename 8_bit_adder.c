#include <stdio.h>

typedef struct {
    int sum;
    int carry;
} SumResult;

/* a, b must be 0 or 1. */
int and_gate(int a, int b) {
    return a & b;
}

/* a, b must be 0 or 1. */
int xor_gate(int a, int b) {
    return a ^ b;
}

/* a, b must be 0 or 1. */
int or_gate(int a, int b) {
    return a | b;
}

/*
    a half adder is just a `and gate` and a `xor gate`.
*/
SumResult half_adder(int a, int b) {
    SumResult sr;

    sr.carry = and_gate(a, b);
    sr.sum = xor_gate(a, b);
    return sr;
}

/*
    a full adder is 2 half adder with a `or gate`.
*/
SumResult full_adder(int a, int b, int carryIn) {
    SumResult sr_ab;
    SumResult sr_with_carry;
    SumResult sr_result;

    sr_ab = half_adder(a, b);
    sr_with_carry = half_adder(sr_ab.sum, carryIn);

    sr_result.sum = sr_with_carry.sum;
    sr_result.carry = or_gate(sr_ab.carry, sr_with_carry.carry);
    return sr_result;
}

void test_half_adder(void) {
    SumResult sr1 = half_adder(0, 0);
    SumResult sr2 = half_adder(0, 1);
    SumResult sr3 = half_adder(1, 0);
    SumResult sr4 = half_adder(1, 1);

    printf("half adder of 0, 0: %d, %d\n", sr1.sum, sr1.carry);
    printf("half adder of 0, 1: %d, %d\n", sr2.sum, sr2.carry);
    printf("half adder of 1, 0: %d, %d\n", sr3.sum, sr3.carry);
    printf("half adder of 1, 1: %d, %d\n", sr4.sum, sr4.carry);
}

void test_full_adder(void) {
    SumResult sr1 = full_adder(0, 0, 0);
    SumResult sr2 = full_adder(0, 0, 1);
    SumResult sr3 = full_adder(0, 1, 0);
    SumResult sr4 = full_adder(0, 1, 1);
    SumResult sr5 = full_adder(1, 0, 0);
    SumResult sr6 = full_adder(1, 0, 1);
    SumResult sr7 = full_adder(1, 1, 0);
    SumResult sr8 = full_adder(1, 1, 1);

    printf("full adder of 0, 0, 0: %d, %d\n", sr1.sum, sr1.carry);
    printf("full adder of 0, 0, 1: %d, %d\n", sr2.sum, sr2.carry);
    printf("full adder of 0, 1, 0: %d, %d\n", sr3.sum, sr3.carry);
    printf("full adder of 0, 1, 1: %d, %d\n", sr4.sum, sr4.carry);
    printf("full adder of 1, 0, 0: %d, %d\n", sr5.sum, sr5.carry);
    printf("full adder of 1, 0, 1: %d, %d\n", sr6.sum, sr6.carry);
    printf("full adder of 1, 1, 0: %d, %d\n", sr7.sum, sr7.carry);
    printf("full adder of 1, 1, 1: %d, %d\n", sr8.sum, sr8.carry);
}

void test_8_bit_adder(void) {
    SumResult sr;
    int num_a[] = { 1, 0, 0, 0, 0, 0, 0, 1 };    /* decimal: 129. */
    int num_b[] = { 1, 0, 0, 0, 1, 0, 1, 1 };    /* decimal: 139. */
    int result[9];
    int i;

    /*
        a 8 bit adder just uses 8 full adder.
        
        the first carry bit must be 0.
        and 8 bit number plus another 8 bit number would add a carry bit,
        so the result is 9 bits. you can expand this 8 bit adder to a 16, 32, 64 bit adder.
    */
    sr = full_adder(num_a[7], num_b[7], 0);
    result[8] = sr.sum; 
    sr = full_adder(num_a[6], num_b[6], sr.carry);
    result[7] = sr.sum; 
    sr = full_adder(num_a[5], num_b[5], sr.carry);
    result[6] = sr.sum; 
    sr = full_adder(num_a[4], num_b[4], sr.carry);
    result[5] = sr.sum; 
    sr = full_adder(num_a[3], num_b[3], sr.carry);
    result[4] = sr.sum; 
    sr = full_adder(num_a[2], num_b[2], sr.carry);
    result[3] = sr.sum; 
    sr = full_adder(num_a[1], num_b[1], sr.carry);
    result[2] = sr.sum; 
    sr = full_adder(num_a[0], num_b[0], sr.carry);
    result[1] = sr.sum; 
    result[0] = sr.carry;

    /* result is 100001100, which is decimal: 268 = 129 + 139. */
    for (i = 0; i < 9; ++i) {
        printf("%d", result[i]);
    }

    printf("\n");
}

int main() {
    test_8_bit_adder();
    return 0;
}
