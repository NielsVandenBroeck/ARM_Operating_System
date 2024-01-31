#include "../basic/mem.h"
#include "../basic/malloc.h"
#include "EMMC/EMMC.h"
#include "../Command-Line-Interface/console.h"

void wait(){
    console_println("start wait");
    int a = 0;
    for(int i = 0; i < 2000000000; i++){
        a++;
        a++;
        a++;a++;
        a++;
        a++;

    }
    console_println("end wait");
}

//https://forums.raspberrypi.com/viewtopic.php?p=166470#p166470
void sd_init() {
    unsigned int m;
    unsigned int test;
    mmio_write(CONTROL0_address, 0x00100000);
    mmio_write(CONTROL1_address, 0x070F0F27);
    mmio_write(CONTROL2_address, 0x00000000);

    console_print("Waiting it to reset: ");
    do {
        m = mmio_read(CONTROL1_address);
        test = m & 0x10000000;
    } while (test != 0);  //Waiting it to reset
    console_println("ok");

    mmio_write(CONTROL0_address, 0x00100000);
    mmio_write(CONTROL1_address, 0x000F0F27);
    mmio_write(CONTROL2_address, 0x00000000);
    mmio_write(IRPT_MASK_address, 0xFFFFFFFF);

    console_print("Clock stable: ");
/*
    do {
        m = mmio_read(CONTROL1_address);
        test = m & 0x00000002;
    } while (test != 2); //Waiting for clock to be stable
  */
    console_println("ok");


    //GO_IDLE_STATE
    mmio_write(ARG1_address, 0x00000000);
    mmio_write(CMDTM_address, 0x00000010);


    //SD_SEND_IF_COND
    mmio_write(ARG1_address, 0x000001AA);
    mmio_write(CMDTM_address, 0x08030010);
    wait();




    unsigned int check;
    //APP_CMD
    mmio_write(ARG1_address, 0x00000000);
    mmio_write(CMDTM_address, 0x37020000);
    wait();
/*
    do {
        //SD_SEND_OP_COND (repeat until Powerup bit is set)
        //console_println("IH2:");
        mmio_write(ARG1_address, 0x40FF0000);
        mmio_write(CMDTM_address, 0x29020000);
        wait();
        check = mmio_read(mmio_read(RESP3_address));
        //console_print_int(check);
        check = check & 0x80000000;
        //console_print_int(check);

    } while (check == 0);

    console_println("ok!!!");
    //ALL_SEND_CID
    mmio_write(ARG1_address, 0x00000000);
    mmio_write(CMDTM_address, 0x02020000);*/
}

void sd_card_init(){

/*
    SendCMD00:
    *pARG1 = 0x00000000;
    *pCMDTM = 0x00000010;

    SendCMD08:
    *pARG1 = 0x000001AA;
    *pCMDTM = 0x08030010;
    *
*/
sd_init();
}

int sd_card_write_data(unsigned int adress, unsigned int data){


    return 1;
}


int sd_card_read_data(unsigned int adress){

}