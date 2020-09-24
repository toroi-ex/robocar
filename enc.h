#ifndef ENC_H
#define ENC_H

#define Resolution 500 		//Encorder Resolution
#define GearProportion 2.74 	//encorder-tire ギア比
#define SAMPLING_INTERVAL 20 	//[ms]
#define DISPLAY_INTERVAL 500 	//[ms]
#define TIME_COUNT_INTERVAL 100	//[ms]

#define PULSE_PER_REV 1370	//車輪1周あたりのパルス
#define WHEEL_ROUND 0.1642	//ホイールの周囲長[m]

#define ON 1
#define OFF 0

#define Ppara_L 0.35	
#define Ppara_R 0.3

#define	MSTPCRA	(*((volatile unsigned short *)0xFFFDC8))
#define	MSTPCRB	(*((volatile unsigned short *)0xFFFDCA))
#define	MSTPCRC	(*((volatile unsigned short *)0xFFFDCC))

//割込み関係
#define	IPRA	(*((volatile unsigned short *)0xFFFD40))
#define	IPRB	(*((volatile unsigned short *)0xFFFD42))
#define	IPRC	(*((volatile unsigned short *)0xFFFD44))
#define	IPRD	(*((volatile unsigned short *)0xFFFD46))
#define	IPRE	(*((volatile unsigned short *)0xFFFD48))
#define	IPRF	(*((volatile unsigned short *)0xFFFD4A))
#define	IPRG	(*((volatile unsigned short *)0xFFFD4C))
#define	IPRH	(*((volatile unsigned short *)0xFFFD4E))
#define	IPRI	(*((volatile unsigned short *)0xFFFD50))
#define	IPRJ	(*((volatile unsigned short *)0xFFFD52))
#define	IPRK	(*((volatile unsigned short *)0xFFFD54))
#define	IPRL	(*((volatile unsigned short *)0xFFFD56))
#define	IPRM	(*((volatile unsigned short *)0xFFFD58))
#define	IPRN	(*((volatile unsigned short *)0xFFFD5A))
#define	IPRO	(*((volatile unsigned short *)0xFFFD5C))
#define	IPRP	(*((volatile unsigned short *)0xFFFD5E))
#define	IPRQ	(*((volatile unsigned short *)0xFFFD60))
#define	IPRR	(*((volatile unsigned short *)0xFFFD62))

//ポート関連
#define	PFCR6	(*((volatile unsigned char *)0xFFFBC6))
#define	PFCR9	(*((volatile unsigned char *)0xFFFBC9))
#define	PFCRC	(*((volatile unsigned char *)0xFFFBCC))
#define	PFCRD	(*((volatile unsigned char *)0xFFFBCD))

#define	P1ICR	(*((volatile unsigned char *)0xFFFB90))
#define	P2ICR	(*((volatile unsigned char *)0xFFFB91))
#define	P5ICR	(*((volatile unsigned char *)0xFFFB94))
#define	P6ICR	(*((volatile unsigned char *)0xFFFB95))
#define	PJICR	(*((volatile unsigned char *)0xFFFBAE))
#define	PKICR	(*((volatile unsigned char *)0xFFFBAF))
#define	PMICR	(*((volatile unsigned char *)0xFFEE53))

#define	TSTR	(*((volatile unsigned char *)0xFFFFBC))
#define	TSYR	(*((volatile unsigned char *)0xFFFFBD))
#define	TSTRB	(*((volatile unsigned char *)0xFFFB00))
#define	TSYRB	(*((volatile unsigned char *)0xFFFB01))

//CH0
#define	TCR0	(*((volatile unsigned char *)0xFFFFC0))
#define	TMDR0	(*((volatile unsigned char *)0xFFFFC1))
#define	TIOR0H	(*((volatile unsigned char *)0xFFFFC2))
#define	TIOR0L	(*((volatile unsigned char *)0xFFFFC3)
#define	TIER0	(*((volatile unsigned char *)0xFFFFC4))
#define	TSR0	(*((volatile unsigned char *)0xFFFFC5))
#define	TCNT0	(*((volatile unsigned short *)0xFFFFC6))
#define	TGR0A	(*((volatile unsigned short *)0xFFFFC8))
#define	TGR0B	(*((volatile unsigned short *)0xFFFFCA))
#define	TGR0C	(*((volatile unsigned short *)0xFFFFCC))
#define	TGR0D	(*((volatile unsigned short *)0xFFFFCE))
//CH1
#define	TCR1	(*((volatile unsigned char *)0xFFFFD0))
#define	TMDR1	(*((volatile unsigned char *)0xFFFFD1))
#define	TIOR1	(*((volatile unsigned char *)0xFFFFD2))
#define	TIER1	(*((volatile unsigned char *)0xFFFFD4))
#define	TSR1	(*((volatile unsigned char *)0xFFFFD5))
#define	TCNT1	(*((volatile unsigned short *)0xFFFFD6))
#define	TGR1A	(*((volatile unsigned short *)0xFFFFD8))
#define	TGR1B	(*((volatile unsigned short *)0xFFFFDA))
//CH2
#define	TCR2	(*((volatile unsigned char *)0xFFFFE0))
#define	TMDR2	(*((volatile unsigned char *)0xFFFFE1))
#define	TIOR2	(*((volatile unsigned char *)0xFFFFE2))
#define	TIER2	(*((volatile unsigned char *)0xFFFFE4))
#define	TSR2	(*((volatile unsigned char *)0xFFFFE5))
#define	TCNT2	(*((volatile unsigned short *)0xFFFFE6))
#define	TGR2A	(*((volatile unsigned short *)0xFFFFE8))
#define	TGR2B	(*((volatile unsigned short *)0xFFFFEA))
//CH3
#define	TCR3	(*((volatile unsigned char *)0xFFFFF0))
#define	TMDR3	(*((volatile unsigned char *)0xFFFFF1))
#define	TIOR3H	(*((volatile unsigned char *)0xFFFFF2))
#define	TIOR3L	(*((volatile unsigned char *)0xFFFFF3)
#define	TIER3	(*((volatile unsigned char *)0xFFFFF4))
#define	TSR3	(*((volatile unsigned char *)0xFFFFF5))
#define	TCNT3	(*((volatile unsigned short *)0xFFFFF6))
#define	TGR3A	(*((volatile unsigned short *)0xFFFFF8))
#define	TGR3B	(*((volatile unsigned short *)0xFFFFFA))
#define	TGR3C	(*((volatile unsigned short *)0xFFFFFC))
#define	TGR3D	(*((volatile unsigned short *)0xFFFFFE))
//CH4
#define	TCR4	(*((volatile unsigned char *)0xFFFEE0))
#define	TMDR4	(*((volatile unsigned char *)0xFFFEE1))
#define	TIOR4	(*((volatile unsigned char *)0xFFFEE2))
#define	TIER4	(*((volatile unsigned char *)0xFFFEE4))
#define	TSR4	(*((volatile unsigned char *)0xFFFEE5))
#define	TCNT4	(*((volatile unsigned short *)0xFFFEE6))
#define	TGR4A	(*((volatile unsigned short *)0xFFFEE8))
#define	TGR4B	(*((volatile unsigned short *)0xFFFEEA))
//CH5
#define	TCR5	(*((volatile unsigned char *)0xFFFEF0))
#define	TMDR5	(*((volatile unsigned char *)0xFFFEF1))
#define	TIOR5	(*((volatile unsigned char *)0xFFFEF2))
#define	TIER5	(*((volatile unsigned char *)0xFFFEF4))
#define	TSR5	(*((volatile unsigned char *)0xFFFEF5))
#define	TCNT5	(*((volatile unsigned short *)0xFFFEF6))
#define	TGR5A	(*((volatile unsigned short *)0xFFFEF8))
#define	TGR5B	(*((volatile unsigned short *)0xFFFEFA))

//CH6
#define	TCR6	(*((volatile unsigned char *)0xFFFB10))
#define	TMDR6	(*((volatile unsigned char *)0xFFFB11))
#define	TIOR6H	(*((volatile unsigned char *)0xFFFB12))
#define	TIOR6L	(*((volatile unsigned char *)0xFFFB13))
#define	TIER6	(*((volatile unsigned char *)0xFFFB14))
#define	TSR6	(*((volatile unsigned char *)0xFFFB15))
#define	TCNT6	(*((volatile unsigned short *)0xFFFB16))
#define	TGR6A	(*((volatile unsigned short *)0xFFFB18))
#define	TGR6B	(*((volatile unsigned short *)0xFFFB1A))
#define	TGR6C	(*((volatile unsigned short *)0xFFFB1C))
#define	TGR6D	(*((volatile unsigned short *)0xFFFB1E))
//CH7
#define	TCR7	(*((volatile unsigned char *)0xFFFB20))
#define	TMDR7	(*((volatile unsigned char *)0xFFFB21))
#define	TIOR7	(*((volatile unsigned char *)0xFFFB22))
#define	TIER7	(*((volatile unsigned char *)0xFFFB24))
#define	TSR7	(*((volatile unsigned char *)0xFFFB25))
#define	TCNT7	(*((volatile unsigned short *)0xFFFB26))
#define	TGR7A	(*((volatile unsigned short *)0xFFFB28))
#define	TGR7B	(*((volatile unsigned short *)0xFFFB2A))
//CH8
#define	TCR8	(*((volatile unsigned char *)0xFFFB30))
#define	TMDR8	(*((volatile unsigned char *)0xFFFB31))
#define	TIOR8	(*((volatile unsigned char *)0xFFFB32))
#define	TIER8	(*((volatile unsigned char *)0xFFFB34))
#define	TSR8	(*((volatile unsigned char *)0xFFFB35))
#define	TCNT8	(*((volatile unsigned short *)0xFFFB36))
#define	TGR8A	(*((volatile unsigned short *)0xFFFB38))
#define	TGR8B	(*((volatile unsigned short *)0xFFFB3A))
//CH9
#define	TCR9	(*((volatile unsigned char *)0xFFFB40))
#define	TMDR9	(*((volatile unsigned char *)0xFFFB41))
#define	TIOR9H	(*((volatile unsigned char *)0xFFFB42))
#define	TIOR9L	(*((volatile unsigned char *)0xFFFB43))
#define	TIER9	(*((volatile unsigned char *)0xFFFB44))
#define	TSR9	(*((volatile unsigned char *)0xFFFB45))
#define	TCNT9	(*((volatile unsigned short *)0xFFFB46))
#define	TGR9A	(*((volatile unsigned short *)0xFFFB48))
#define	TGR9B	(*((volatile unsigned short *)0xFFFB4A))
#define	TGR9C	(*((volatile unsigned short *)0xFFFB4C))
#define	TGR9D	(*((volatile unsigned short *)0xFFFB4E))
//CH10
#define	TCR10	(*((volatile unsigned char *)0xFFFB50))
#define	TMDR10	(*((volatile unsigned char *)0xFFFB51))
#define	TIOR10	(*((volatile unsigned char *)0xFFFB52))
#define	TIER10	(*((volatile unsigned char *)0xFFFB54))
#define	TSR10	(*((volatile unsigned char *)0xFFFB55))
#define	TCNT10	(*((volatile unsigned short *)0xFFFB56))
#define	TGR10A	(*((volatile unsigned short *)0xFFFB58))
#define	TGR10B	(*((volatile unsigned short *)0xFFFB5A))
//CH11
#define	TCR11	(*((volatile unsigned char *)0xFFFB60))
#define	TMDR11	(*((volatile unsigned char *)0xFFFB61))
#define	TIOR11	(*((volatile unsigned char *)0xFFFB62))
#define	TIER11	(*((volatile unsigned char *)0xFFFB64))
#define	TSR11	(*((volatile unsigned char *)0xFFFB65))
#define	TCNT11	(*((volatile unsigned short *)0xFFFB66))
#define	TGR11A	(*((volatile unsigned short *)0xFFFB68))
#define	TGR11B	(*((volatile unsigned short *)0xFFFB6A))


struct PulseCount {
	unsigned int L;
	unsigned int R;
};

int enc_AB_init(void);
int enc_CD_init(void);
int enc_EF_init(void);
int enc_GH_init(void);

int enc_AB_count(void);
int enc_CD_count(void);
int enc_EF_count(void);
int enc_GH_count(void);

long read_enc_AB_count(void);
long read_enc_CD_count(void);
long read_enc_EF_count(void);
long read_enc_GH_count(void);

double encPulse2Speed(long pulseCnt);

#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           