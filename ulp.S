#include "soc/soc_ulp.h"
#include "soc/rtc_io_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc_cntl_reg.h"


.bss//secao das variaveis


.text//secao do codigo


	.global main
	main:

		move r0, 0//r0 = 0
		move r1, 0//r1 = 0
		stage_rst//stage_cnt = 0

		leitura:
			stage_inc 1//stage_cnt++
			adc r1, 0, 4+1//r1 = leitura ADC GPIO32
			add r0, r0, r1//r0 = r0+r1
			jumps leitura, 4, lt//loop responsavel pelas leituras, equivale a um FOR()

		rsh r0, r0, 2//calcula a media das 4 leituras r0 = r0/4
		jumpr wakeup, 496, ge//se a media da leitura que esta em r0 for maior ou igual que 496 (40 graus celsius), acorda o mcu
		halt//coloca o ULP em sleep e ativa o timer de wakeup (definido com 10 segundos no main code)

	wakeup:
		wake//acorda o mcu
		halt
