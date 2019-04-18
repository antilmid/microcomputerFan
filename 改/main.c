#include "reg52.h"
#include "absacc.h"
#define uint unsigned int
#define uchar unsigned char
#define AD0809IN0 XBYTE[0xCFA0] // Define AD0809 CS adress
#define DA0832_1 XBYTE[0x0CFA8] // DA0832 CS adress pin
#define DA0832_2 XBYTE[0x0CFA9] // DA0832 CS adress pin + 1, Because of the 8279 is double buffer
#define CON8279 XBYTE[0x0CFE9] // gripe of 8279
#define DATA8279 XBYTE[0x0CFE8] // data of 8279
sbit LED = P1^0; // Test led light pin
sbit AD_EOC = P1^1; // AD eoc sigal pin
uchar chooseList[6] = {0x80,0x81,0x82,0x83,0x84,0x85}; //  which led was choosed?
uchar mapping[17] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40}; // The led number mapping
uchar template = 135; // room template;

// delay function use to wait sign. unit is ms;
void delay(uint t){
	uint x,y;
	for(x=t;x>0;x--)
		for(y=59;y>0;y--);
}

// test Functon
void test(){
	delay(1000);
	LED = ~LED;
}
// clear screen
void clear8279(){
	CON8279 = 0xD1;  // Use clear must equel 0xD1
	delay(1);
}
void displayN(uchar n, uchar data){
	CON8279 = chooseList[n];
	delay(1);
	DATA8279 = mapping[data];
	delay(1);
}

// read AD;
uchar getAD(){
	AD0809IN0 = 1;
	while(!AD_EOC){}
	return AD0809IN0;
}
// set DA;
void setDA(uchar a){
	DA0832_1 = a;
	delay(1);
	DA0832_2 = a;
	delay(1);
}

void main(){
	uchar AD = 0;
	AD = getAD();
	template = AD+8; // you can comment this and use fixed number to set
	while(1){
		clear8279();
		if(AD <= template){
			setDA(255);
		}else{
			setDA(0);
		}
		display(3,AD/100);
		display(4,(AD%100)/10);
		display(5,AD%10);
		AD = getAD();
		test();
	}
}
