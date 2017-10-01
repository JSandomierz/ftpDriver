#include <iostream>
#include <thread>
#include <chrono>

//C functions (popen, pclose)
#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>

#define GPIO_HDD_PIN 0
#define MAX_PATH 128

using namespace std;

void setup(){
    wiringPiSetup();
    pinMode(GPIO_HDD_PIN, OUTPUT);
}

int getNumberOfConnections(){
    int result = 0;
    //C Part
    char buf[MAX_PATH];
    FILE *fp;
    fp = popen("netstat -ntu | grep 1.7:21", "r");//ip end with port
    if(fp == NULL){
        return -1;
    }
    while(fgets(buf, MAX_PATH, fp) != NULL){
        //traverse chars
        result+=1;
    }
    if(-1 == pclose(fp)){
        return -1;
    }
    return result;
}

void readyUpDisk(){
    digitalWrite(GPIO_HDD_PIN, HIGH);
    //wait for spinup
    this_thread::sleep_for( chrono::seconds(10) );
    system("mount /dev/sda1 /mnt/hdd/a");
}

void disableDisk(){
    system("umount /mnt/hdd/a");
    this_thread::sleep_for( chrono::seconds(2) );
    system("hdparm -y /dev/sda");
    this_thread::sleep_for( chrono::seconds(5) );
    digitalWrite(GPIO_HDD_PIN, LOW);
}

int main(){
    int lines;
    
    auto connectedInterval = chrono::seconds(30);
    auto checkInterval = chrono::milliseconds(150);
    bool isDiskEnabled = false;
    
    setup();
    
    while(true){
        lines = getNumberOfConnections();
        if(lines>0){
            if(isDiskEnabled == false){
                readyUpDisk();
                isDiskEnabled = true;
            }
            this_thread::sleep_for( connectedInterval );
        }else{
            if(isDiskEnabled == true){
                disableDisk();
                isDiskEnabled = false;
            }
            this_thread::sleep_for( checkInterval );
        }
    }   
    return 0;
}
