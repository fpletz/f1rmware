#include <r0ketlib/config.h>
#include <r0ketlib/fonts.h>
#include <r0ketlib/fonts/smallfonts.h>
#include <r0ketlib/display.h>
#include <r0ketlib/print.h>
#include <r0ketlib/render.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/select.h>
#include <r0ketlib/stringin.h>
#include <r0ketlib/image.h>
#include <rad1olib/pins.h>
#include <rad1olib/systick.h>

/**************************************************************************/

//# MENU usb_storage
void msc_menu(void){
    MSCenable();
    lcdPrintln("MSC enabled.");
    lcdDisplay();
    getInputWaitRelease();
    getInputWait();
    lcdPrintln("disconnect");
    lcdDisplay();
    MSCdisable();
    fsReInit();
    getInputWaitRelease();
}

void tick_alive(void){
    static int foo=0;

    if(GLOBAL(alivechk)==0)
        return;

	if(foo++>500/SYSTICKSPEED){
		foo=0;
        TOGGLE(LED2);
	};
    return;
}

//# MENU img img
void t_img(void){
    char fname[FLEN];
    selectFile(fname, "LCD");
    lcdShowImageFile(fname);
    getInputWait();
}

//# MENU img anim
void t_ani(void){
    char fname[FLEN];
    selectFile(fname, "AN1");
    lcdShowAnim(fname);
}

//# MENU night
void night(void){
    while(getInputRaw()!=BTN_ENTER){
        lcdClear();
        lcdPrint("night:");
        lcdPrint(IntToStr(isNight(),3,0));
        lcdNl();
        lcdPrint("light:");
        lcdPrint(IntToStr(GetLight(),3,0));
        lcdNl();
        lcdDisplay();
    };
}

void infoscreen();
//# MENU INFO
void info(void){
    infoscreen();
}
