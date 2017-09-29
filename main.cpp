#include "mbed.h"

Timer t;
DigitalIn echo(D9);
DigitalOut trig(D8);
DigitalOut LED(D7);
DigitalOut faucet(D6);
DigitalOut multiPin0(D10);
DigitalOut multiPin1(D11);
DigitalOut multiPin2(D12);
DigitalOut multiPin3(D13);
AnalogIn readLight(A0);
float toCM[15]={16.2,15.7,13.8,12.9,11.6,11.1,10.6,10.0,9.2,7.4,6.5,5.8,4.4,3.8,3.1};
float tmp[15];
float minus[14];

float getDepth(){
        trig = 1;
        wait(0.00001); // 10 us TTL wave
        trig = 0;
        while(echo.read()!=1){}
        t.start();
        while(echo.read()!=0){}
        t.stop();
        float val;
        if(t.read()<0.06f)val=t.read()*17160;
        wait(0.1);
        t.reset();
        return val;
        }
float toMulti(int num){
    if(num<0)num=0;
    if(num>14)num=14;
    multiPin0=num%2;
    num=num/2;
    multiPin1=num%2;
    num=num/2;
    multiPin2=num%2;
    num=num/2;
    multiPin3=num%2;
    wait(0.05);
    //printf("%f\n",readLight.read());    
    return readLight.read();
    }
float getDepth_LED(){
    for(int i=14;i>0;--i){
        if(toMulti(i)<0.15f){
            return toCM[i-1];
        }
    }
    return 17.5;
    }

void fillWater(float maximum){
    while(getDepth()>maximum){
        faucet=1;
        printf("current water level:%.3f      target:%.3f\n\r",getDepth(),maximum);
        }
        faucet=0;
    }
void eighty(float maximum){
    fillWater(maximum*1.0f);}
void create(){
    for(int i=14;i>=0;--i)tmp[i]=toMulti(i);
    //for(int i=0;i<15;++i)printf("check:%f\n",tmp[i]);
    for(int i=0;i<14;++i)minus[i]=tmp[i+1]-tmp[i];
    }
    
int main() {
    LED=1;
    float maximum;
    bool word=true;
    wait(2);
    while(true){
    if(word==true){
        word=false;
        printf("Ready to fill water!\n\r");}
    //printf("%.3f\n\r",getDepth());
    //printf("==============================\n\r");
    create();
    for(int i=0;i<14;++i)if(minus[i]>0.15f){
        printf("Container found!!\n\rtriggerNum=%i,value=%f,target=%.3f\n\r",i,minus[i],toCM[i]);
        maximum=toCM[i];
        //printf("final:%f\n",maximum);
        wait(3);
        fillWater(maximum);
        printf("Water filled!!\n\r");
        wait(10);
        word=true;
        break;}
    //printf("------------------------------\n\r");
    /*faucet=1;
    wait(10);
    faucet=0;
    wait(10);*/
    }
}