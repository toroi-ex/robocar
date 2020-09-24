#include"C:\Users\RCR2013\Documents\Desktop\rcr2019\test2019\YSML.H"
#include <stdio.h>
#include <sysio.h>
#include"C:\Users\RCR2013\Documents\Desktop\rcr2019\test2019\enc_test.h"

/*********************************************************************
*****	�p���X�o�͂̎���
*****
*****	�p���X�����
*****
*****	���荞�݃x�N�^�̐ݒ�
*****	�x�N�^�ԍ�		�֐���		����
*****	�Ȃ�
*****
*****	�o�͒[�q
*****	�`�����l��3		TIOCA3/P21
*****	�`�����l��4		TIOCA4/P25
*****	�`�����l��5		TIOCA5/P26
*****	�`�����l��6		TIOCA6/PJ0(�V���O���`�b�v���[�h�̂�)
*****	�`�����l��7		TIOCA7/PJ4(�V���O���`�b�v���[�h�̂�)
*****	�`�����l��8		TIOCA8/PJ6(�V���O���`�b�v���[�h�̂�)
*****	�`�����l��9		TIOCA9/PK0(�V���O���`�b�v���[�h�̂�)
*****	�`�����l��10		TIOCA10/PK4(�V���O���`�b�v���[�h�̂�)
*****	�`�����l��11		TIOCA11/PK6(�V���O���`�b�v���[�h�̂�)
*********************************************************************/

#define PMDDR (*((volatile unsigned char *)0xffee50))
#define PMDR  (*((volatile unsigned char *)0xffee51))
#define PORTM (*((volatile unsigned char *)0xffee52))

//������I��//
#define	TIM_CLOCK1		0	// 1/1
#define	TIM_CLOCK2		1	//
#define	TIM_CLOCK3		2	//
#define	TIM_CLOCK4		3	//

//PID�֌W//
#define Gp 30
#define Gd 0.020
#define Gpr 40
#define Gdr 0.18
#define Gpl 45
#define Gdl 0.18
#define sampling_time 0.01

//�G���R�[�_�֌W//
#define def 50000
#define Gpe 0.005
#define Gie 0.001

//LPF�̌W��
#define lpf 0.6 

//PID�֌W
int init_d = 0;
int init = 0;
double p_error = 0.0;
double d_error = 0.0;
double prev_cte = 0.0;
volatile int pre_dis = 0;
volatile int total_error;
volatile double cte;

//A/D�ϊ�
volatile int adcFR2;              //      �O����PSD(AN0)��A/D�ϊ��o��(0~1024)
volatile int adcFL2;              //      �O��������PSD(AN1)��A/D�ϊ��o��(0~1024)  1
volatile int adcRF2;              //      �O���E����PSD(AN2)��A/D�ϊ��o��(0~1024)  2
volatile int adcRR2;              //      �E�[�O����PSD(AN3)��A/D�ϊ��o��(0~1024)  3
volatile int adcLF2;              //      ���[�O����PSD(AN4)��A/D�ϊ��o��(0~1024)  4
volatile int adcLR2;              //      ���[�����PSD(AN5)��A/D�ϊ��o��(0~1024)  5
volatile int adcPT;               // �t�H�g�g�����W�X�^(AN6)��A/D�ϊ��o��(0~1024)  

volatile int adcFR;
volatile int adcFL;
volatile int adcRF;
volatile int adcRR;
volatile int adcLF;
volatile int adcLR;

volatile int distance_adcFR;
volatile int distance_adcFL;
volatile int distance_adcRF;
volatile int distance_adcRR;
volatile int distance_adcLF;
volatile int distance_adcLR;

//1�T���v���O��PSD�l
volatile int pre_adcFR = 0;
volatile int pre_adcFL = 0;
volatile int pre_adcRF = 0;
volatile int pre_adcRR = 0;
volatile int pre_adcLF = 0;
volatile int pre_adcLR = 0;

//���S���Ǐ]//
volatile int y_now_l; ////�ǂƏd�S�Ԃ̋���(left)         
volatile int y_now_r; ////�ǂƏd�S�Ԃ̋���(right)

//�G���R�[�_�֌W//
volatile long encCurrent = 0;
volatile long pre_encCurrent = 0;
volatile double omega = 33.0;
volatile double error_enc = 0.0;  
volatile double i_error_enc = 0.0;  
volatile int total_error_enc = 0;

//mode�؂�ւ�
int k;

//�J�E���g�ҋ@//
void wait(unsigned long count)
{
	unsigned long x;
	for (x = 0; x < count; x++)
		;
}

//A/D�ϊ�������
void adInit() {
	PAdInit(0);
	PAdInit(1);
	PAdInit(2);
	PAdInit(3);
	PAdInit(4);
	PAdInit(5);
	PAdInit(6);
}

//A/D�ϊ��o��
void adOutput() {

	adcFR = PAdIn(0);
	adcFL = PAdIn(1);
	adcRF2 = PAdIn(2);
	adcRR2 = PAdIn(3);
	adcLF2 = PAdIn(4);
	adcLR2 = PAdIn(5);
	adcPT = PAdIn(6);

	adcRF = lpf * pre_adcRF + (1 - lpf) * adcRF2;
	adcRR = lpf * pre_adcRR + (1 - lpf) * adcRR2;
	adcLF = lpf * pre_adcLF + (1 - lpf) * adcLF2;
	adcLR = lpf * pre_adcLR + (1 - lpf) * adcLR2;

	//�O��PSD�l
	pre_adcFR = adcFR;
	pre_adcFL = adcFL;
	pre_adcRF = adcRF;
	pre_adcRR = adcRR;
	pre_adcLF = adcLF;
	pre_adcLR = adcLR;

}


void dcmotor(int width) {
	//PulseStop(11);
	PulseInit(11, 30000, width + 1, TIM_CLOCK1);
	PulseStart(11);

}

void servomotor(int error) {
	//PulseStop(10);
	PulseInit(10, 30000, error + 1, TIM_CLOCK2);
	PulseStart(10);

}

void servomotor2(int wi) {
	//PulseStop(10);
	PulseInit(10, 30000, wi, TIM_CLOCK2);
	PulseStart(10);

}


//////////////////////////////PSD2�̋����ϊ�///////////////////////////////////////////
void ad_to_distance_psd2() {
	if (adcRF >= 720) {
		distance_adcRF = 10;
	}
	else if (270 <= adcRF < 720) {
		distance_adcRF = 0.000118*adcRF*adcRF - 0.16*adcRF + 65;
	}
	else if (210 <= adcRF < 260) {
		distance_adcRF = -0.166*adcRF + 75;
	}
	else if (170 <= adcRF < 210) {
		distance_adcRF = -0.5*adcRF + 145;
	}
	else if (adcRF < 170) {
		distance_adcRF = 110;
	}

}

/////////////////////////////PSD3�̋����ϊ�/////////////////////////////////////////////
void ad_to_distance_psd3() {
	if (adcRR >= 750) {
		distance_adcRR = 10;
	}
	else if (295 <= adcRR < 750) {
		distance_adcRR = 0.000117*adcRR*adcRR - 0.167*adcRR + 69;
	}
	else if (235 <= adcRR < 295) {
		distance_adcRR = -0.17*adcRR + 80;
	}
	else if (160 <= adcRR < 235) {
		distance_adcRR = -0.26*adcRR + 101;
	}
	else if (adcRR < 160) {
		distance_adcRR = 110;
	}

}

//////////////////////////////////PSD4�̋����ϊ�/////////////////////////////////////
void ad_to_distance_psd4() {
	if (adcLF >= 720) {
		distance_adcLF = 10;
	}
	else if (280 <= adcLF < 720) {
		distance_adcLF = 0.000118*adcLF*adcLF - 0.1638*adcLF + 67;
	}
	else if (220 <= adcLF < 280) {
		distance_adcLF = -0.16*adcLF + 76;
	}
	else if (160 <= adcLF < 220) {
		distance_adcLF = -0.33*adcLF + 113;
	}
	else if (adcLF < 160) {
		distance_adcLF = 110;
	}

}

//////////////////////////////////PSD5�̋����ϊ�/////////////////////////////////////
void ad_to_distance_psd5() {
	if (adcLR >= 725) {
		distance_adcLR = 10;
	}
	else if (265 <= adcLR < 725) {
		distance_adcLR = 0.000118*adcLR*adcLR - 0.164*adcLR + 67;
	}
	else if (220 <= adcLR < 265) {
		distance_adcLR = -0.2*adcLR + 84;
	}
	else if (160 <= adcLR < 220) {
		distance_adcLR = -0.32*adcLR + 110;
	}
	else if (adcLR < 160) {
		distance_adcLR = 110;
	}

}

//PSD�̋����ϊ�//
void psd_dis() {
	ad_to_distance_psd2();
	ad_to_distance_psd3();
	ad_to_distance_psd4();
	ad_to_distance_psd5();
}

//�ǂƏd�S�Ԃ̋���(���̕Ǘp�j//
void l_center_line() {
	float cos_l = 0.0;

	delt_l = distance_adcRF - distance_adcLF;

	if (delt_l == 0) {
		cos_l = 1;
	}
	else if (0 < delt_l && delt_l <= 6 || -6 <= delt_l && delt_l < 0) {
		cos_l = 0.9;
	}
	else if (6 < delt_l && delt_l <= 9 || -9 <= delt_l && delt_l < -6) {
		cos_l = 0.8;
	}
	else if (9 < delt_l || delt_l < -9) {
		cos_l = 0.7;
	}

	y_now_l = (int)(((distance_adcRF + distance_adcLF) / 2 + 10) * cos_l);

	printf("cos_l=%f y_now_l=%d\n", cos_l, y_now_l);	//�Ȃ���������c���Ƃ��ȓ�����
}

//�ǂƏd�S�Ԃ̋���(�E�̕Ǘp�j//
void r_center_line() {
	float cos_r = 0.0;
	delt_r = distance_adcRR - distance_adcLR;

	if (delt_r == 0) {
		cos_r = 1;
	}
	else if (0 < delt_r && delt_r <= 6 || -6 <= delt_r && delt_r < 0) {
		cos_r = 0.9;
	}
	else if (6 < delt_r && delt_r <= 9 || -9 <= delt_r && delt_r < -6) {
		cos_r = 0.8;
	}
	else if (9 < delt_r || delt_r < -9) {
		cos_r = 0.7;
	}

	y_now_r = (int)(((distance_adcRR + distance_adcLR) / 2 + 10) * cos_r);


	printf("cos_r=%f y_now_r=%d\n", cos_r, y_now_r);	//�Ȃ���������c���Ƃ��ȓ�����
}


//PID �E�̕ǒǏ]//
double right_trace_pid(unsigned int t, int Ga, int Gb, int min_width, int max_width) {

	int e1 = 0;
	int e2 = 0;
	double cte2 = 0;

	if (Ga == Gps) {
		e1 = distance_adcRR - distance_adcLR;
		e2 = 36 - y_now_r;  //���S���̃t�B�[�h�o�b�N

		cte2 = -0.85 * e1 + 0.15 * e2;

		cte = 0.1 * prev_cte + 0.9 * cte2;
	}
	if (Ga == Gpr) {

		cte2 = 36 - y_now_r;

		cte = 0.1 * prev_cte + 0.9 * cte2;

	}

	if (!init) {

		d_error = cte;
		init = 1;
		prev_cte = cte;

	}
	else {

		d_error = cte - prev_cte;
		prev_cte = cte;

	}

	p_error = cte;

	total_error = t + (Ga*p_error + Gb * (d_error / sampling_time));

	if (total_error < min_width) {
		total_error = min_width;
	}
	else if (total_error > max_width) {
		total_error = max_width;
	}

	return total_error;

}

//PID ���̕ǒǏ]//
double left_trace_pid(int t, int Ga, int Gb, int min_width, int max_width) {

	int e1 = 0;
	int e2 = 0;
	double cte2;

	if (Ga == Gp) {
		e1 = distance_adcRF - distance_adcLF;  
		e2 = 36 - y_now_l;  //���S���̃t�B�[�h�o�b�N

		cte2 = -0.15 * e1 + 0.85 * e2;

		cte = 0.1 * prev_cte + 0.9 * cte2;

	}
	if (Ga == Gpl) {

		cte2 = 36 - y_now_l;

		cte = 0.1* prev_cte + 0.9 * cte2;

	}

	if (!init_d) {

		d_error = cte;
		init_d = 1;
		prev_cte = cte;

	}
	else {

		d_error = cte - prev_cte;
		prev_cte = cte;
	}
	p_error = cte;

	total_error = t - (Ga*p_error + Gb * d_error / sampling_time);

	if (total_error < min_width) {
		total_error = min_width;
	}
	else if (total_error > max_width) {
		total_error = max_width;
	}

	return total_error;

}


void encInit()
{
	MSTPCRA &= ~0x0002;		//MSTPCRA &= ~0x0002; MSTPA1 == 0 -> TPU6-11���W���[���X�g�b�v��ԉ���
	PFCRD |= 0x80;			//PFCRD |= 0x80;      �|�[�g�t�@���N�V�����R���g���[�����W�X�^	
	TSTRB &= ~0x02;			//TSTRB &= ~0x02;     TPU7�J�E���g�����~
	PJICR |= 0x0C;			//PJICR |=  0x0C;     ���̓o�b�t�@�R���g���[�����W�X�^ ���͒[�q�Ƃ��ė��p����ꍇ�ɕK�v�i���Y�p�C���͒[�q�ł��g�������j
	TMDR7 = 0x00;			//TMDR7 =  0;         �ʏ퓮��
	TMDR7 |= 0x04;			//TMDR7 |= 0x04;      TPU7 �ʑ��W�����[�h1
	TCNT7 = def;          //A�wB�w�t����
//	TPU7.TIER.BYTE |= 0x30;		//TIER7 |= 0x30;	  overflow,underflow interrupt permission	�����������ƂȂ���������
	TSTRB |= 0x02;       		//�^�C�}�[�X�^�[�g
}

//-----�G���R�[�_�J�E���g�J�n-----//
void encCountStart()
{
	encCurrent = TCNT7;
	TCNT7 = def;
}

//DC��PI����// �@
double PI_plot(int aim_speed, double aim_omega) {

	omega = (double)((def - encCurrent) * 2 * 3.14 / (2087 * 0.03));  //�p���x
	error_enc = aim_omega - omega;
	if (error_enc < 1 && error_enc > -1) {
		i_error_enc = 0.0;
	}
	else {
		i_error_enc += error_enc * 0.03;
	}
	total_error_enc = aim_speed + 50 * (Gpe * error_enc + Gie * i_error_enc);

	if (total_error_enc > 10000) {
		total_error_enc = 10000;
	}

	return total_error_enc;

}

void run_test() {


	int mode;

	//�ً}���
	if (adcFR > 350) {
		if (adcRF < adcRR) {
			if (distance_adcRF > 50) {
				servomotor(12500);
				//PMDDR = 0x00;
			}
		}
	}

	if (adcFL > 350) {
		if (adcRR < adcRF) {
			if (distance_adcRR > 50) {
				servomotor(6800);
				//PMDDR = 0x00;
			}
		}
	}

	if (adcFR < adcFL) {
		if (adcFR < 320) {  //���J�[�u

			if (distance_adcRF > 50) {
				mode = 0;
			}
			else {
				mode = 4;
			}

			k = 2;
		}
		if (distance_adcRF >= 50) {
			mode = 0;
		}
	}
	if (adcFL < adcFR) { //�E�J�[�u
		if (adcFL < 250) {
			if (distance_adcRR >= 50) {
				mode = 1;
			}
			else {
				mode = 3;
			}
			k = 2;
			//PMDDR = 0xff;
		}
	}
	else {

		if (adcRF < adcRR) {
			if (distance_adcRF >= 50) {
				mode = 0;
				//PMDDR = 0xff;
			}
		}

		else if (adcRR < adcRF) {
			if (distance_adcRR >= 50) {
				mode = 1;
				//PMDDR = 0x00;
			}
		}
		else {
			mode = 2;
			 //PMDDR = 0x00;
		}
	}


	switch (mode) {

	case 0:	//��2�i�K��
		PI_plot(6000, 33);
		l_center_line();
		left_trace_pid(11500, Gpl, Gdl, 9500, 12500);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	case 1: //�E��i�K
		PI_plot(6000, 33);
		r_center_line();
		right_trace_pid(8250, Gpr, Gdr, 6700, 9700);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	case 2: //���i
		PI_plot(7000, 40);
		l_center_line();
		left_trace_pid(9300, Gp, Gd, 8000, 11000);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0x00;
		break;

	case 3: //�E��i�K��
		PI_plot(6000, 33);
		r_center_line();
		right_trace_pid(8600, Gpr, Gdr, 6700, 9700);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	case 4: //��1�i�K��
		PI_plot(6000, 33);
		l_center_line();
		left_trace_pid(10500, Gpl, Gdl, 9500, 12500);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	default:
		PI_plot(7000, 40);
		l_center_line();
		left_trace_pid(9300, Gp, Gd, 8500, 10200);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0x00;
		break;

	}
}

void main()
{
	int cnt_b = 0;
	int cnt_w = 0;
	int cnt_x = 0;
	int line_count = 0;

	adInit();
	encInit();

	while (1) {
		adOutput();
		encCountStart();

		PMDDR = 0x00;
		if (adcPT > 400) {
			cnt_x++;
			cnt_b = 10;
		}

		if (adcPT < 400) {
			cnt_w = 5;
			cnt_b = 0;
			cnt_x = 0;
		}

		if (cnt_b - cnt_w == 5) {
			if (cnt_x >= 10) {
				line_count++;
				cnt_b = 0;
				cnt_w = 0;
				cnt_x = 0;
				PMDDR = 0xff;
			}
		}

		if (line_count >= 3) {
			PMDDR = 0xff;
			break;
		}
		else {
			psd_dis();
			run_test();
		}

	}
	dcmotor(0);
}