# MPLAB IDE generated this makefile for use with GNU make.
# Project: MistWriper.mcp
# Date: Fri Dec 02 19:11:11 2016

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

./bin/MistWriper.cof : Debug/Main.o Debug/UI.o Debug/Key.o Debug/Module.o Debug/Isr.o Debug/salloc.o Debug/Power.o Debug/USB_Module.o Debug/ADC.o Debug/RTC.o Debug/delay.o Debug/Flash.o
	$(LD) /l"C:\MCC18\lib" /l"C:\MCC18\h" "18f45j50_g.lkr" "Debug\Main.o" "Debug\UI.o" "Debug\Key.o" "Debug\Module.o" "Debug\Isr.o" "Debug\salloc.o" "Debug\Power.o" "Debug\USB_Module.o" "Debug\ADC.o" "Debug\RTC.o" "Debug\delay.o" "Debug\Flash.o" /u_CRUNTIME /u_DEBUG /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /z__MPLAB_DEBUGGER_PK3=1 /z__ICD2RAM=1 /m"./bin\MistWriper.map" /w /o"./bin\MistWriper.cof"

Debug/Main.o : Main/Main.c salloc/salloc.h Module/Module.h UI/Key.h UI/UI.h Power/Power.h Power/ADC.h Main/RTC.h Flash/Flash.h Main/Main.c Main/Isr.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h USB/USB_Module.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Main\Main.c" -fo="./Debug\Main.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/UI.o : UI/UI.c Module/Module.h UI/Key.h UI/UI.h salloc/salloc.h Main/RTC.h Power/Power.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h salloc/delay.h Power/ADC.h Flash/Flash.h UI/UI.c C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h USB/USB_Module.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "UI\UI.c" -fo="./Debug\UI.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/Key.o : UI/Key.c UI/Key.h Module/Module.h UI/UI.h Main/RTC.h UI/Key.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "UI\Key.c" -fo="./Debug\Key.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/Module.o : Module/Module.c Module/Module.h UI/Key.h UI/UI.h salloc/salloc.h Module/Module.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h Main/Main.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Module\Module.c" -fo="./Debug\Module.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/Isr.o : Main/Isr.c UI/Key.h Power/ADC.h Main/RTC.h Module/Module.h UI/UI.h Main/Isr.c Main/Isr.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h USB/USB_Module.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Main\Isr.c" -fo="./Debug\Isr.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/salloc.o : salloc/salloc.c
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "salloc\salloc.c" -fo="./Debug\salloc.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/Power.o : Power/Power.c Power/Power.h Module/Module.h UI/Key.h UI/UI.h Main/RTC.h Power/ADC.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h salloc/delay.h Flash/Flash.h Power/Power.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h USB/USB_Module.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Power\Power.c" -fo="./Debug\Power.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/USB_Module.o : USB/USB_Module.c USB/USB_Module.h Module/Module.h UI/Key.h UI/UI.h Main/RTC.h Power/Power.h Power/ADC.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h salloc/Delay.h USB/USB_Module.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "USB\USB_Module.c" -fo="./Debug\USB_Module.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/ADC.o : Power/ADC.c Power/ADC.h Power/Power.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h salloc/Delay.h Power/ADC.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Power\ADC.c" -fo="./Debug\ADC.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/RTC.o : Main/RTC.c Main/RTC.h Module/Module.h UI/Key.h UI/UI.h Power/ADC.h Power/Power.h Main/RTC.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h USB/USB_Module.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Main\RTC.c" -fo="./Debug\RTC.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/delay.o : salloc/delay.c C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h salloc/delay.h salloc/delay.c C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "salloc\delay.c" -fo="./Debug\delay.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

Debug/Flash.o : Flash/Flash.c Flash/Flash.h Flash/Flash.c Main/GenericTypeDefs.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h Main/Main.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/P18Cxxx.h C:/Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f45j50.h
	$(CC) -p=18F45J50 /i"./Flash" -I"./USB" -I"./UI" -I"./salloc" -I"./Power" -I"./Module" -I"./Main" "Flash\Flash.c" -fo="./Debug\Flash.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "Debug\Main.o" "Debug\UI.o" "Debug\Key.o" "Debug\Module.o" "Debug\Isr.o" "Debug\salloc.o" "Debug\Power.o" "Debug\USB_Module.o" "Debug\ADC.o" "Debug\RTC.o" "Debug\delay.o" "Debug\Flash.o" ".\bin\MistWriper.cof" ".\bin\MistWriper.hex" ".\bin\MistWriper.map"

