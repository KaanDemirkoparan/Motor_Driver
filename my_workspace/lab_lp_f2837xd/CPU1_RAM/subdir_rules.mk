################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-219622433: ../lab_f2837xd_launchpad.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1271/ccs/utils/sysconfig_1.20.0/sysconfig_cli.bat" --script "C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd/lab_f2837xd_launchpad.syscfg" -o "syscfg" -s "C:/ti/c2000/C2000Ware_5_02_00_00/.metadata/sdk.json" -b "/boards/LAUNCHXL_F28379D" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-219622433 ../lab_f2837xd_launchpad.syscfg
syscfg/board.h: build-219622433
syscfg/board.cmd.genlibs: build-219622433
syscfg/board.opt: build-219622433
syscfg/board.json: build-219622433
syscfg/pinmux.csv: build-219622433
syscfg/epwm.dot: build-219622433
syscfg/adc.dot: build-219622433
syscfg/c2000ware_libraries.cmd.genlibs: build-219622433
syscfg/c2000ware_libraries.opt: build-219622433
syscfg/c2000ware_libraries.c: build-219622433
syscfg/c2000ware_libraries.h: build-219622433
syscfg/clocktree.h: build-219622433
syscfg: build-219622433

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00" --include_path="C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd/device" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f2837xd/driverlib/" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=CPU1 --define=RAM --define=_LAUNCHXL_F28379D --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00" --include_path="C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd/device" --include_path="C:/ti/c2000/C2000Ware_5_02_00_00/driverlib/f2837xd/driverlib/" --include_path="C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=DEBUG --define=CPU1 --define=RAM --define=_LAUNCHXL_F28379D --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/kaand/Desktop/Projeler/Powerlab/motor_driver/my_workspace/lab_lp_f2837xd/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


