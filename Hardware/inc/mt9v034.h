#ifndef MT9V034_H
#define MT9V034_H

#define MT_WRITE_ID	0x90
#define MT_READ_ID	0X91

#define SCCB_SDA GPIOE,GPIO_Pin_6
#define SCCB_SCL GPIOE,GPIO_Pin_5
#define PWDN_PIN GPIOE,GPIO_Pin_2
#define RESET_PIN GPIOE,GPIO_Pin_3
#define DCMI_HSYNC_PIN			GPIOA,GPIO_Pin_4
#define DCMI_HSYNC_PIN_S		GPIOA,GPIO_PinSource4
#define DCMI_PIXCLK_PIN			GPIOA,GPIO_Pin_6
#define DCMI_PIXCLK_PIN_S		GPIOA,GPIO_PinSource6
#define DCMI_VSYNC_PIN			GPIOB,GPIO_Pin_7
#define DCMI_VSYNC_PIN_S		GPIOB,GPIO_PinSource7
#define DCMI_D0_PIN					GPIOC,GPIO_Pin_6				
#define DCMI_D0_PIN_S				GPIOC,GPIO_PinSource6
#define DCMI_D1_PIN					GPIOC,GPIO_Pin_7				
#define DCMI_D1_PIN_S				GPIOC,GPIO_PinSource7
#define DCMI_D2_PIN					GPIOE,GPIO_Pin_0				
#define DCMI_D2_PIN_S				GPIOE,GPIO_PinSource0
#define DCMI_D3_PIN					GPIOE,GPIO_Pin_1				
#define DCMI_D3_PIN_S				GPIOE,GPIO_PinSource1
#define DCMI_D4_PIN					GPIOE,GPIO_Pin_4				
#define DCMI_D4_PIN_S				GPIOE,GPIO_PinSource4
#define DCMI_D5_PIN					GPIOB,GPIO_Pin_6				
#define DCMI_D5_PIN_S				GPIOB,GPIO_PinSource6
#define DCMI_D6_PIN					GPIOB,GPIO_Pin_8				
#define DCMI_D6_PIN_S				GPIOB,GPIO_PinSource8
#define DCMI_D7_PIN					GPIOB,GPIO_Pin_9				
#define DCMI_D7_PIN_S				GPIOB,GPIO_PinSource9

extern int mtv_frame;

void DCMI_Start(void);
void MTV_DMA_LineCircular(int PrePriority,int SubPriority);
extern unsigned char MTV_FrameBuf[120][160];
int check_camera(void);
void MTV_DCMI_Init(unsigned short DCMI_CaptureMode,int PrePriority,int SubPriority);
void MTV_Init();
unsigned short SCCB_RD_HalfWord(unsigned char ReadAddr);
unsigned short SCCB_WR_HalfWord(unsigned char WriteAddr,unsigned short dat);

//=======================REG=========================
#define MTREG_ChipVersion 										0x00
#define MTREG_ColumnStart 										0x01
#define MTREG_RowStartContextA 								0x02
#define MTREG_WindowHeightContextA 						0x03
#define MTREG_WindowWidthContextA 						0x04
#define MTREG_HorizontalBlankingContextA 			0x05
#define MTREG_VerticalBlankingContextA 				0x06
#define MTREG_ChipControl 										0x07
#define MTREG_CoarseShutterWidth1ContextA 		0x08
#define MTREG_CoarseShutterWidth2ContextA 		0x09
#define MTREG_ShutterWidthCtrlContextA 				0x0a
#define MTREG_CoarseTotalShutterWidthContextA 0x0b
#define MTREG_Reset 													0x0c
#define MTREG_ReadModeContextA 								0x0d
#define MTREG_ReadModeContextB 								0x0e
#define MTREG_SensorTypeHDREnable 						0x0f
#define MTREG_LED_OUTCtrl 										0x1b
#define MTREG_Companding 											0x1c
#define MTREG_VREF_ADCControl 								0x2c
#define MTREG_V1ContextA 											0x31
#define MTREG_V2ContextA 											0x32
#define MTREG_V3ContextA 											0x33
#define MTREG_V4ContextA 											0x34
#define MTREG_AnalogGainContextA 							0x35
#define MTREG_AnalogGainContextB 							0x36
#define MTREG_V1ControlContextB 							0x39
#define MTREG_V2ControlContextB 							0x3a
#define MTREG_V3ControlContextB 							0x3b
#define MTREG_V4ControlContextB 							0x3c
#define MTREG_FrameDarkAverage 								0x42
#define MTREG_DarkAvgThresholds 							0x46
#define MTREG_BLCalibControl 									0x47
#define MTREG_BlackLevelCalibrationValue 			0x48
#define MTREG_BLCalibStepSize 								0x4c
#define MTREG_RowNoiseCorrControl 						0x70
#define MTREG_RowNoiseConstant 								0x71
#define MTREG_PixclkFVLVCtrl 									0x72
#define MTREG_DigitalTestPattern 							0x7f
#define MTREG_TileWeight_GainX0_Y0 						0x80
#define MTREG_TileWeight_GainX1_Y0 						0x81
#define MTREG_TileWeight_GainX2_Y0 						0x82
#define MTREG_TileWeight_GainX3_Y0 						0x83
#define MTREG_TileWeight_GainX4_Y0 						0x84
#define MTREG_TileWeight_GainX0_Y1 						0x85
#define MTREG_TileWeight_GainX1_Y1 						0x86
#define MTREG_TileWeight_GainX2_Y1 						0x87
#define MTREG_TileWeight_GainX3_Y1 						0x88
#define MTREG_TileWeight_GainX4_Y1 						0x89
#define MTREG_TileWeight_GainX0_Y2 						0x8a
#define MTREG_TileWeight_GainX1_Y2 						0x8b
#define MTREG_TileWeight_GainX2_Y2 						0x8c
#define MTREG_TileWeight_GainX3_Y2 						0x8d
#define MTREG_TileWeight_GainX4_Y2 						0x8e
#define MTREG_TileWeight_GainX0_Y3 						0x8f
#define MTREG_TileWeight_GainX1_Y3 						0x90
#define MTREG_TileWeight_GainX2_Y3 						0x91
#define MTREG_TileWeight_GainX3_Y3 						0x92
#define MTREG_TileWeight_GainX4_Y3 						0x93
#define MTREG_TileWeight_GainX0_Y4 						0x94
#define MTREG_TileWeight_GainX1_Y4 						0x95
#define MTREG_TileWeight_GainX2_Y4 						0x96
#define MTREG_TileWeight_GainX3_Y4 						0x97
#define MTREG_TileWeight_GainX4_Y4 						0x98
#define MTREG_TileCoordX0_5 									0x99
#define MTREG_TileCoordX1_5 									0x9a
#define MTREG_TileCoordX2_5 									0x9b
#define MTREG_TileCoordX3_5 									0x9c
#define MTREG_TileCoordX4_5 									0x9d
#define MTREG_TileCoordX5_5 									0x9e
#define MTREG_TileCoordY0_5 									0x9f
#define MTREG_TileCoordY1_5 									0xa0
#define MTREG_TileCoordY2_5 									0xa1
#define MTREG_TileCoordY3_5 									0xa2
#define MTREG_TileCoordY4_5 									0xa3
#define MTREG_TileCoordY5_5 									0xa4
#define MTREG_AEC_AGC_DesiredBin 							0xa5
#define MTREG_AEC_UpdateFrequency 						0xa6
#define MTREG_AEC_LPF 												0xa8
#define MTREG_AGC_UpdateFrequency 						0xa9
#define MTREG_AGC_LPF 												0xaa
#define MTREG_MaxAnalogGain 									0xab
#define MTREG_AEC_MInimumExposure 						0xac
#define MTREG_AEC_MaximumExposure 						0xad
#define MTREG_BinDifferenceThreshold 					0xae
#define MTREG_AEC_AGC_EnableA_B 							0xaf
#define MTREG_AEC_AGC_PixCount 								0xb0
#define MTREG_LVDSMasterCtrl 									0xb1
#define MTREG_DataStreamLatency 							0xb4
#define MTREG_StereoscopErrorCtrl 						0xb7
#define MTREG_StereoscopErrorFlag 						0xb8
#define MTREG_AGC_GainOutput 									0xba
#define MTREG_AEC_GainOutput 									0xbb
#define MTREG_AGC_AEC_CurrentBin	 						0xbc
#define MTREG_InterlaceFieldBlank 						0xbf
#define MTREG_MonModeCaptureCtrl 							0xc0
#define MTREG_AntiEclipseControls 						0xc2
#define MTREG_NTSV_FV_LV_Control 							0xc6
#define MTREG_NTSC_HorizBlankCtrl 						0xc7
#define MTREG_NTSC_VertBlankCtrl 							0xc8
#define MTREG_ColumnStartContextB 						0xc9
#define MTREG_RowStartContextB 								0xca
#define MTREG_WindowHeightContextB 						0xcb
#define MTREG_WindowWidthContextB 						0xcc
#define MTREG_HorizontalBlankingContextB 			0xcd
#define MTREG_VerticalBlankingContextB 				0xce
#define MTREG_CoarseSW1ContextB 							0xcf
#define MTREG_CoarseSW2ContextB 							0xd0
#define MTREG_ShutterWidthCtrlContextB 				0xd1
#define MTREG_CoarseShutterWidthTotalContextB 0xd2
#define MTREG_FineSW1ContextA 								0xd3
#define MTREG_FineSW2ContextA 								0xd4
#define MTREG_FineShutterWidthTotalContextA 	0xd5
#define MTREG_FineSW1ContextB 								0xd6
#define MTREG_FineSW2ContextB 								0xd7
#define MTREG_FineShutterWidthTotalContextB 	0xd8
#define MTREG_MonitorMode 										0xd9
#define MTREG_BytewiseAddr 										0xf0
#define MTREG_RegisterLock 										0xfe
 
#endif
