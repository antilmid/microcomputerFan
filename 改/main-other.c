#include "reg52.h"
#include "absacc.h"
#define uint unsigned int
#define uchar unsigned char
#define AD0809IN0 XBYTE[0xCFA0] // AD0809片选地址，这里是0
#define DA0832_1 XBYTE[0x0CFA8] // DA0832片选地址，这里是1
#define DA0832_2 XBYTE[0x0CFA9] // DA0832片选缓存层
#define CON8279 XBYTE[0x0CFE9]
#define DATA8279 XBYTE[0x0CFE8]
sbit AD_EOC = P1^1; // AD转换完成的信号

// 延时，毫秒级
void delay(uint t){
	uint x,y;
	for(x=t;x>0;x--)
		for(y=59;y>0;y--);
}

// 清屏
void clear(){
	CON8279 = 0xD1;
	delay(1);
}
void display(uchar data){
	uchar led[17] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40};
	uchar bai = 0;
	uchar shi = 0;
	uchar ge = 0;
	bai = data/100;
	shi = (data%100)/10;
	ge = data%10;
	CON8279 = 0x83;
	delay(1);
	DATA8279 = led[bai];
	delay(1);

	CON8279 = 0x84;
	delay(1);
	DATA8279 = led[shi];
	delay(1);

	CON8279 = 0x85;
	delay(1);
	DATA8279 = led[ge];
	delay(1);
}

// 读取AD
uchar getAD(){
	AD0809IN0 = 1;
	while(!AD_EOC){}
	return AD0809IN0;
}
// 设置DA
void setDA(uchar a){
	DA0832_1 = a;
	delay(1);
	DA0832_2 = a;
	delay(1);
}

void main(){
	uchar result = 0;
	uchar template = 255; // 根据实际情况，自己设置
	result = getAD();
	while(1){
		clear();
		if(result <= template){
			setDA(255); // 自己设置
		}else{
			setDA(0); // 自己设置
		}
		display(3,result);
		result = getAD();
	}
}
