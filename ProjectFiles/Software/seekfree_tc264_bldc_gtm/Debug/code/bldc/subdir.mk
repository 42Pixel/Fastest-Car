################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/bldc/gtm_pwm.c \
../code/bldc/motor.c 

COMPILED_SRCS += \
./code/bldc/gtm_pwm.src \
./code/bldc/motor.src 

C_DEPS += \
./code/bldc/gtm_pwm.d \
./code/bldc/motor.d 

OBJS += \
./code/bldc/gtm_pwm.o \
./code/bldc/motor.o 


# Each subdirectory must supply rules for building sources it contributes
code/bldc/%.src: ../code/bldc/%.c code/bldc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fE:/Project/23���ܳ��ο�����/��ˢ���/TC264_GTM_BLDC_Project-master/��Դ�롿���ص��������Դ��/seekfree_tc264_bldc_gtm/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/bldc\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/bldc/%.o: ./code/bldc/%.src code/bldc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code-2f-bldc

clean-code-2f-bldc:
	-$(RM) ./code/bldc/gtm_pwm.d ./code/bldc/gtm_pwm.o ./code/bldc/gtm_pwm.src ./code/bldc/motor.d ./code/bldc/motor.o ./code/bldc/motor.src

.PHONY: clean-code-2f-bldc
