#include "reg52.h"
#include "absacc.h"
#define uint unsigned int
#define uchar unsigned char
#define AD0809IN0 XBYTE[0xCFA0]
#define DA0832_1 XBYTE[0x0CFA8]
#define DA0832_2 XBYTE[0x0CFA9]
#define CON8279 XBYTE[0x0CFE9]
#define DATA8279 XBYTE[0x0CFE8]
sbit LED = P1^0;
sbit AD_EOC = P1^1;
sbit KEY_1 = P1^2;
sbit KEY_2 = P1^3;
uchar AUTO = 0;
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
	CON8279 = 0xD1;
	delay(1);
}
// displayed on srceen,a==1 to show first to thrid,2 anther
void display8279 (uchar a, uchar b){
	uchar a_[6] = {0x80,0x81,0x82,0x83,0x84,0x85};
	uchar b_[17] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40};
	uchar hunderth = 0;
	uchar tenth = 0;
	uchar oneth = 0;
	hunderth = b/100;
	tenth = (b%100)/10;
	oneth = b%10;
	if(a==2){
		a=4;
	}
	CON8279 = a_[a-1];
	delay(1);
	DATA8279 = b_[hunderth];
	delay(1);
	CON8279 = a_[a];
	delay(1);
	DATA8279 = b_[tenth];
	delay(1);
	CON8279 = a_[a+1];
	delay(1);
	DATA8279 = b_[oneth];
}
// read AD;
uchar getAD(){
	uchar ret = 1;
	AD0809IN0 = ret;
	while(!AD_EOC){
	}
	ret = AD0809IN0;
	return ret;
}
// set DA;
void setDA(uchar a){
	//DA0832_1 = a;
	DA0832_1 = a;
	delay(1);
	DA0832_2 = a;
	delay(1);
}
void main(){
	uchar template = 135;
	uchar AD = 0;
	clear8279();
	display8279(2,120);
	while(1){
		clear8279();
		if(AD <= template){
			setDA(255);
		}else{
			setDA(0);
		}
		
		display8279(2,AD);
		AD = getAD();
		test();
	}
}
