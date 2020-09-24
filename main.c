#include"C:\Users\RCR2013\Documents\Desktop\rcr2019\test2019\YSML.H"
#include <stdio.h>
#include <sysio.h>
#include"C:\Users\RCR2013\Documents\Desktop\rcr2019\test2019\enc_test.h"

/*********************************************************************
*****	パルス出力の実験
*****
*****	パルスを作る
*****
*****	割り込みベクタの設定
*****	ベクタ番号		関数名		言語
*****	なし
*****
*****	出力端子
*****	チャンネル3		TIOCA3/P21
*****	チャンネル4		TIOCA4/P25
*****	チャンネル5		TIOCA5/P26
*****	チャンネル6		TIOCA6/PJ0(シングルチップモードのみ)
*****	チャンネル7		TIOCA7/PJ4(シングルチップモードのみ)
*****	チャンネル8		TIOCA8/PJ6(シングルチップモードのみ)
*****	チャンネル9		TIOCA9/PK0(シングルチップモードのみ)
*****	チャンネル10		TIOCA10/PK4(シングルチップモードのみ)
*****	チャンネル11		TIOCA11/PK6(シングルチップモードのみ)
*********************************************************************/

#define PMDDR (*((volatile unsigned char *)0xffee50))
#define PMDR  (*((volatile unsigned char *)0xffee51))
#define PORTM (*((volatile unsigned char *)0xffee52))

//分周比選択//
#define	TIM_CLOCK1		0	// 1/1
#define	TIM_CLOCK2		1	//
#define	TIM_CLOCK3		2	//
#define	TIM_CLOCK4		3	//

//PID関係//
#define Gp 30
#define Gd 0.020
#define Gpr 40
#define Gdr 0.18
#define Gpl 45
#define Gdl 0.18
#define sampling_time 0.01

//エンコーダ関係//
#define def 50000
#define Gpe 0.005
#define Gie 0.001

//LPFの係数
#define lpf 0.6 

//PID関係
int init_d = 0;
int init = 0;
double p_error = 0.0;
double d_error = 0.0;
double prev_cte = 0.0;
volatile int pre_dis = 0;
volatile int total_error;
volatile double cte;

//A/D変換
volatile int adcFR2;              //      前方のPSD(AN0)のA/D変換出力(0~1024)
volatile int adcFL2;              //      前方左側のPSD(AN1)のA/D変換出力(0~1024)  1
volatile int adcRF2;              //      前方右側のPSD(AN2)のA/D変換出力(0~1024)  2
volatile int adcRR2;              //      右端前方のPSD(AN3)のA/D変換出力(0~1024)  3
volatile int adcLF2;              //      左端前方のPSD(AN4)のA/D変換出力(0~1024)  4
volatile int adcLR2;              //      左端後方のPSD(AN5)のA/D変換出力(0~1024)  5
volatile int adcPT;               // フォトトランジスタ(AN6)のA/D変換出力(0~1024)  

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

//1サンプル前のPSD値
volatile int pre_adcFR = 0;
volatile int pre_adcFL = 0;
volatile int pre_adcRF = 0;
volatile int pre_adcRR = 0;
volatile int pre_adcLF = 0;
volatile int pre_adcLR = 0;

//中心線追従//
volatile int y_now_l; ////壁と重心間の距離(left)         
volatile int y_now_r; ////壁と重心間の距離(right)

//エンコーダ関係//
volatile long encCurrent = 0;
volatile long pre_encCurrent = 0;
volatile double omega = 33.0;
volatile double error_enc = 0.0;  
volatile double i_error_enc = 0.0;  
volatile int total_error_enc = 0;

//mode切り替え
int k;

//カウント待機//
void wait(unsigned long count)
{
	unsigned long x;
	for (x = 0; x < count; x++)
		;
}

//A/D変換初期化
void adInit() {
	PAdInit(0);
	PAdInit(1);
	PAdInit(2);
	PAdInit(3);
	PAdInit(4);
	PAdInit(5);
	PAdInit(6);
}

//A/D変換出力
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

	//前のPSD値
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


//////////////////////////////PSD2の距離変換///////////////////////////////////////////
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

/////////////////////////////PSD3の距離変換/////////////////////////////////////////////
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

//////////////////////////////////PSD4の距離変換/////////////////////////////////////
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

//////////////////////////////////PSD5の距離変換/////////////////////////////////////
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

//PSDの距離変換//
void psd_dis() {
	ad_to_distance_psd2();
	ad_to_distance_psd3();
	ad_to_distance_psd4();
	ad_to_distance_psd5();
}

//壁と重心間の距離(左の壁用）//
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

	printf("cos_l=%f y_now_l=%d\n", cos_l, y_now_l);	//なぜかこれを残しとかな動かん
}

//壁と重心間の距離(右の壁用）//
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


	printf("cos_r=%f y_now_r=%d\n", cos_r, y_now_r);	//なぜかこれを残しとかな動かん
}


//PID 右の壁追従//
double right_trace_pid(unsigned int t, int Ga, int Gb, int min_width, int max_width) {

	int e1 = 0;
	int e2 = 0;
	double cte2 = 0;

	if (Ga == Gps) {
		e1 = distance_adcRR - distance_adcLR;
		e2 = 36 - y_now_r;  //中心線のフィードバック

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

//PID 左の壁追従//
double left_trace_pid(int t, int Ga, int Gb, int min_width, int max_width) {

	int e1 = 0;
	int e2 = 0;
	double cte2;

	if (Ga == Gp) {
		e1 = distance_adcRF - distance_adcLF;  
		e2 = 36 - y_now_l;  //中心線のフィードバック

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
	MSTPCRA &= ~0x0002;		//MSTPCRA &= ~0x0002; MSTPA1 == 0 -> TPU6-11モジュールストップ状態解除
	PFCRD |= 0x80;			//PFCRD |= 0x80;      ポートファンクションコントロールレジスタ	
	TSTRB &= ~0x02;			//TSTRB &= ~0x02;     TPU7カウント動作停止
	PJICR |= 0x0C;			//PJICR |=  0x0C;     入力バッファコントロールレジスタ 入力端子として利用する場合に必要（ラズパイ入力端子でも使うかも）
	TMDR7 = 0x00;			//TMDR7 =  0;         通常動作
	TMDR7 |= 0x04;			//TMDR7 |= 0x04;      TPU7 位相係数モード1
	TCNT7 = def;          //A層B層逆かも
//	TPU7.TIER.BYTE |= 0x30;		//TIER7 |= 0x30;	  overflow,underflow interrupt permission	これを消さんとなぜか動かん
	TSTRB |= 0x02;       		//タイマースタート
}

//-----エンコーダカウント開始-----//
void encCountStart()
{
	encCurrent = TCNT7;
	TCNT7 = def;
}

//DCのPI制御// 　
double PI_plot(int aim_speed, double aim_omega) {

	omega = (double)((def - encCurrent) * 2 * 3.14 / (2087 * 0.03));  //角速度
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

	//緊急回避
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
		if (adcFR < 320) {  //左カーブ

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
	if (adcFL < adcFR) { //右カーブ
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

	case 0:	//左2段階目
		PI_plot(6000, 33);
		l_center_line();
		left_trace_pid(11500, Gpl, Gdl, 9500, 12500);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	case 1: //右二段階
		PI_plot(6000, 33);
		r_center_line();
		right_trace_pid(8250, Gpr, Gdr, 6700, 9700);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	case 2: //直進
		PI_plot(7000, 40);
		l_center_line();
		left_trace_pid(9300, Gp, Gd, 8000, 11000);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0x00;
		break;

	case 3: //右一段階目
		PI_plot(6000, 33);
		r_center_line();
		right_trace_pid(8600, Gpr, Gdr, 6700, 9700);
		servomotor(total_error);
		dcmotor(total_error_enc);
		//PMDDR = 0xff;
		break;

	case 4: //左1段階目
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