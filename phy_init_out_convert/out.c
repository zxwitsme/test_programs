// ####################################################
// 
// Printing values in user input structure
// 
// ####################################################
//##############################################################
//
// 4.3.1(A) Bring up VDD, VDDQ, and VAA 
//
// The power supplies can come up and stabilize in any order. 
// While the power supplies are coming up, all outputs will be unknown and 
// the values of the inputs are don't cares.
//
//############################################################## 
//
// 4.3.2(B) Start Clocks and Reset the PHY 
//
// The procedure to reset the PHY is as follows: 
// 
// 1. Drive PwrOkIn to 0. Note: Reset, DfiClk, and APBCLK can be X. 
// 2. Start DfiClk and APBCLK 
// 3. Drive Reset to 1. Note: The combination of PwrOkIn=0 and Reset=1 signals a cold reset to the PHY. 
// 4. Wait a minimum of 8 cycles. 
// 5. Drive PwrOkIn to 1. Once the PwrOkIn is asserted (and Reset is still asserted), 
//    DfiClk synchronously switches to any legal input frequency. 
// 6. Wait a minimum of 64 cycles. Note: This is the reset period for the PHY. 
// 7. Drive Reset to 0. Note: All DFI and APB inputs must be driven at valid reset states before the deassertion of Reset. 
// 8. The PHY is now in the reset state and is ready to accept APB transactions.
//
//##############################################################
//##############################################################
//
// 4.3.3(C) Initialize PHY Configuration 
//
// Load the required PHY configuration registers for the appropriate mode and memory configuration
//
//##############################################################
#define DDRPHY_200c5_14314_VAL 0xa
#define DDRPHY_1200c5_34314_VAL 0xa
#define DDRPHY_2200c5_54314_VAL 0xa
#define DDRPHY_3200c5_74314_VAL 0xa
//##############################################################
//
// Program ARdPtrInitVal based on Frequency and PLL Bypass inputs
// The values programmed here assume ideal properties of DfiClk
// and Pclk including:
// - DfiClk skew
// - DfiClk jitter
// - DfiClk PVT variations
// - Pclk skew
// - Pclk jitter
//
// PLL Bypassed mode:
//     For MemClk frequency > 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 2-6
//     For MemClk frequency < 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 1-6
//
// PLL Enabled mode:
//     For MemClk frequency > 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 1-6
//     For MemClk frequency < 933MHz, the valid range of ARdPtrInitVal_p0[3:0] is: 0-6
//
//##############################################################
#define DDRPHY_2002e_140b8_VAL 0x2
#define DDRPHY_12002e_340b8_VAL 0x2
#define DDRPHY_22002e_540b8_VAL 0x2
#define DDRPHY_32002e_740b8_VAL 0x2
#define DDRPHY_20024_14090_VAL 0xab
#define DDRPHY_2003a_140e8_VAL 0x0
#define DDRPHY_120024_34090_VAL 0xab
#define DDRPHY_2003a_140e8_VAL 0x0
#define DDRPHY_220024_54090_VAL 0xab
#define DDRPHY_2003a_140e8_VAL 0x0
#define DDRPHY_320024_74090_VAL 0xab
#define DDRPHY_2003a_140e8_VAL 0x0
#define DDRPHY_20056_14158_VAL 0x2
#define DDRPHY_120056_34158_VAL 0x2
#define DDRPHY_220056_54158_VAL 0x2
#define DDRPHY_320056_74158_VAL 0x2
#define DDRPHY_1004d_c134_VAL 0x600
#define DDRPHY_1014d_c534_VAL 0x600
#define DDRPHY_1104d_10134_VAL 0x600
#define DDRPHY_1114d_10534_VAL 0x600
#define DDRPHY_11004d_2c134_VAL 0x600
#define DDRPHY_11014d_2c534_VAL 0x600
#define DDRPHY_11104d_30134_VAL 0x600
#define DDRPHY_11114d_30534_VAL 0x600
#define DDRPHY_21004d_4c134_VAL 0x600
#define DDRPHY_21014d_4c534_VAL 0x600
#define DDRPHY_21104d_50134_VAL 0x600
#define DDRPHY_21114d_50534_VAL 0x600
#define DDRPHY_31004d_6c134_VAL 0x600
#define DDRPHY_31014d_6c534_VAL 0x600
#define DDRPHY_31104d_70134_VAL 0x600
#define DDRPHY_31114d_70534_VAL 0x600
#define DDRPHY_10049_c124_VAL 0x618
#define DDRPHY_10149_c524_VAL 0x618
#define DDRPHY_11049_10124_VAL 0x618
#define DDRPHY_11149_10524_VAL 0x618
#define DDRPHY_110049_2c124_VAL 0x618
#define DDRPHY_110149_2c524_VAL 0x618
#define DDRPHY_111049_30124_VAL 0x618
#define DDRPHY_111149_30524_VAL 0x618
#define DDRPHY_210049_4c124_VAL 0x618
#define DDRPHY_210149_4c524_VAL 0x618
#define DDRPHY_211049_50124_VAL 0x618
#define DDRPHY_211149_50524_VAL 0x618
#define DDRPHY_310049_6c124_VAL 0x618
#define DDRPHY_310149_6c524_VAL 0x618
#define DDRPHY_311049_70124_VAL 0x618
#define DDRPHY_311149_70524_VAL 0x618
#define DDRPHY_20018_14060_VAL 0x5
#define DDRPHY_20075_141d4_VAL 0x4
#define DDRPHY_20050_14140_VAL 0x0
#define DDRPHY_20008_14020_VAL 0x215
#define DDRPHY_120008_34020_VAL 0x215
#define DDRPHY_220008_54020_VAL 0x215
#define DDRPHY_320008_74020_VAL 0x215
#define DDRPHY_20088_14220_VAL 0x29
#define DDRPHY_2007c_141f0_VAL 0xa2
#define DDRPHY_12007c_341f0_VAL 0xa2
#define DDRPHY_22007c_541f0_VAL 0xa2
#define DDRPHY_32007c_741f0_VAL 0xa2
#define DDRPHY_200b2_142c8_VAL 0x204
#define DDRPHY_10043_c10c_VAL 0x5a1
#define DDRPHY_10143_c50c_VAL 0x5a1
#define DDRPHY_11043_1010c_VAL 0x5a1
#define DDRPHY_11143_1050c_VAL 0x5a1
#define DDRPHY_1200b2_342c8_VAL 0x204
#define DDRPHY_110043_2c10c_VAL 0x5a1
#define DDRPHY_110143_2c50c_VAL 0x5a1
#define DDRPHY_111043_3010c_VAL 0x5a1
#define DDRPHY_111143_3050c_VAL 0x5a1
#define DDRPHY_2200b2_542c8_VAL 0x204
#define DDRPHY_210043_4c10c_VAL 0x5a1
#define DDRPHY_210143_4c50c_VAL 0x5a1
#define DDRPHY_211043_5010c_VAL 0x5a1
#define DDRPHY_211143_5050c_VAL 0x5a1
#define DDRPHY_3200b2_742c8_VAL 0x204
#define DDRPHY_310043_6c10c_VAL 0x5a1
#define DDRPHY_310143_6c50c_VAL 0x5a1
#define DDRPHY_311043_7010c_VAL 0x5a1
#define DDRPHY_311143_7050c_VAL 0x5a1
#define DDRPHY_200fa_143e8_VAL 0x1
#define DDRPHY_1200fa_343e8_VAL 0x1
#define DDRPHY_2200fa_543e8_VAL 0x1
#define DDRPHY_3200fa_743e8_VAL 0x1
#define DDRPHY_20019_14064_VAL 0x1
#define DDRPHY_120019_34064_VAL 0x1
#define DDRPHY_220019_54064_VAL 0x1
#define DDRPHY_320019_74064_VAL 0x1
#define DDRPHY_200f0_143c0_VAL 0x0
#define DDRPHY_200f1_143c4_VAL 0x0
#define DDRPHY_200f2_143c8_VAL 0x4444
#define DDRPHY_200f3_143cc_VAL 0x0
#define DDRPHY_200f4_143d0_VAL 0x5555
#define DDRPHY_200f5_143d4_VAL 0x0
#define DDRPHY_200f6_143d8_VAL 0x0
#define DDRPHY_200f7_143dc_VAL 0xf000
#define DDRPHY_2000b_1402c_VAL 0x42
#define DDRPHY_2000c_14030_VAL 0x85
#define DDRPHY_2000d_14034_VAL 0x534
#define DDRPHY_2000e_14038_VAL 0x2c
#define DDRPHY_12000b_3402c_VAL 0x42
#define DDRPHY_12000c_34030_VAL 0x85
#define DDRPHY_12000d_34034_VAL 0x534
#define DDRPHY_12000e_34038_VAL 0x2c
#define DDRPHY_22000b_5402c_VAL 0x42
#define DDRPHY_22000c_54030_VAL 0x85
#define DDRPHY_22000d_54034_VAL 0x534
#define DDRPHY_22000e_54038_VAL 0x2c
#define DDRPHY_32000b_7402c_VAL 0x42
#define DDRPHY_32000c_74030_VAL 0x85
#define DDRPHY_32000d_74034_VAL 0x534
#define DDRPHY_32000e_74038_VAL 0x2c
#define DDRPHY_20025_14094_VAL 0x0
#define DDRPHY_2002d_140b4_VAL 0x1
#define DDRPHY_12002d_340b4_VAL 0x1
#define DDRPHY_22002d_540b4_VAL 0x1
#define DDRPHY_32002d_740b4_VAL 0x1
//##############################################################
//
// 4.3.4(D) Load the 1D IMEM image
// 
// The procedure is as follow:
// 
// 1.	Load the microcontroller memory with the provided training firmware 
// 2.	Initialize the firmware mailbox structures to be able to communicate with the firmware (see "3.4.1 Mailbox facility for firmware" in the "DesignWare Cores DDR PHY Training Application Note") 
// 
//##############################################################
//##############################################################
//
// [phyinit_sequence] Steps D to H for 1D Training of Pstate 0
//
//##############################################################
//##############################################################
//
// 4.3.5(E) Set the PHY input clocks to the desired frequency 533 
// 
// Set the PHY input clocks to the desired operating frequency. Before proceeding to the next step, 
// the clock should be stable at the new frequency. For more information on clocking requirements, see "Clocks" on page <XXX>.
// 
//##############################################################
//##############################################################
//
// 4.3.5(F) Load the 1D DMEM image and write the 1D Message Block parameters for the training firmware 
// 
// The procedure is as follows: 
// 
//##############################################################
// 1.    Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0. 
//       This allows the memory controller unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Load the firmware DMEM segment to initialize the data structures.
// 3.	Write the Firmware Message Block with the required contents detailing the training parameters.
#define DDRPHY_54000_90000_VAL 0x600
#define DDRPHY_54001_90004_VAL 0x0
#define DDRPHY_54002_90008_VAL 0x0
#define DDRPHY_54003_9000c_VAL 0x854
#define DDRPHY_54004_90010_VAL 0x1
#define DDRPHY_54005_90014_VAL 0x3c3c
#define DDRPHY_54006_90018_VAL 0x40
#define DDRPHY_54007_9001c_VAL 0x0
#define DDRPHY_54008_90020_VAL 0x7
#define DDRPHY_54009_90024_VAL 0xff
#define DDRPHY_5400a_90028_VAL 0x0
#define DDRPHY_5400b_9002c_VAL 0x1
#define DDRPHY_5400c_90030_VAL 0x1
#define DDRPHY_5400d_90034_VAL 0x0
#define DDRPHY_5400e_90038_VAL 0x0
#define DDRPHY_5400f_9003c_VAL 0x0
#define DDRPHY_54010_90040_VAL 0x0
#define DDRPHY_54011_90044_VAL 0x0
#define DDRPHY_54012_90048_VAL 0x310
#define DDRPHY_54013_9004c_VAL 0x0
#define DDRPHY_54014_90050_VAL 0x0
#define DDRPHY_54015_90054_VAL 0x0
#define DDRPHY_54016_90058_VAL 0x0
#define DDRPHY_54017_9005c_VAL 0x0
#define DDRPHY_54018_90060_VAL 0x0
#define DDRPHY_54019_90064_VAL 0x1b34
#define DDRPHY_5401a_90068_VAL 0xc1
#define DDRPHY_5401b_9006c_VAL 0x4d44
#define DDRPHY_5401c_90070_VAL 0x4d00
#define DDRPHY_5401d_90074_VAL 0x0
#define DDRPHY_5401e_90078_VAL 0x0
#define DDRPHY_5401f_9007c_VAL 0x1b34
#define DDRPHY_54020_90080_VAL 0xc1
#define DDRPHY_54021_90084_VAL 0x4d44
#define DDRPHY_54022_90088_VAL 0x4d00
#define DDRPHY_54023_9008c_VAL 0x0
#define DDRPHY_54024_90090_VAL 0x0
#define DDRPHY_54025_90094_VAL 0x0
#define DDRPHY_54026_90098_VAL 0x0
#define DDRPHY_54027_9009c_VAL 0x0
#define DDRPHY_54028_900a0_VAL 0x0
#define DDRPHY_54029_900a4_VAL 0x0
#define DDRPHY_5402a_900a8_VAL 0x0
#define DDRPHY_5402b_900ac_VAL 0x0
#define DDRPHY_5402c_900b0_VAL 0x0
#define DDRPHY_5402d_900b4_VAL 0x0
#define DDRPHY_5402e_900b8_VAL 0x0
#define DDRPHY_5402f_900bc_VAL 0x0
#define DDRPHY_54030_900c0_VAL 0x0
#define DDRPHY_54031_900c4_VAL 0x0
#define DDRPHY_54032_900c8_VAL 0x0
#define DDRPHY_54033_900cc_VAL 0x0
#define DDRPHY_54034_900d0_VAL 0x0
#define DDRPHY_54035_900d4_VAL 0x0
#define DDRPHY_54036_900d8_VAL 0x0
#define DDRPHY_54037_900dc_VAL 0x0
#define DDRPHY_54038_900e0_VAL 0x0
#define DDRPHY_54039_900e4_VAL 0x0
#define DDRPHY_5403a_900e8_VAL 0x0
#define DDRPHY_5403b_900ec_VAL 0x0
#define DDRPHY_5403c_900f0_VAL 0x0
#define DDRPHY_5403d_900f4_VAL 0x0
#define DDRPHY_5403e_900f8_VAL 0x0
#define DDRPHY_5403f_900fc_VAL 0x0
// 4.    Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
//      This allows the firmware unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x1
//##############################################################
//
// 4.3.7(G) Execute the Training Firmware 
// 
// The training firmware is executed with the following procedure: 
//
//##############################################################
// 1.  Reset the firmware microcontroller by writing the MicroReset CSR to set the StallToMicro and 
//     ResetToMicro fields to 1 (all other fields should be zero). 
//     Then rewrite the CSR so that only the StallToMicro remains set (all other fields should be zero). 
#define DDRPHY_d0000_a8000_VAL 0x1
#define DDRPHY_d0099_a8264_VAL 0x9
#define DDRPHY_d0099_a8264_VAL 0x1
// 2. Begin execution of the training firmware by setting the MicroReset CSR to 4'b0000. 
#define DDRPHY_d0099_a8264_VAL 0x0
// 3.   Wait for the training firmware to complete by following the procedure in "uCtrl Initialization and Mailbox Messaging" 
//##############################################################
//
// 4.3.8(H) Read the Message Block results
// 
// The procedure is as follows:
// 
//##############################################################
// 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Read the Firmware Message Block to obtain the results from the training 
// 3.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
#define DDRPHY_d0000_a8000_VAL 0x1
// 4.	If training is required at another frequency, repeat the operations starting at step (E). 
//##############################################################
//
// [phyinit_sequence] Steps D to H for 1D Training of Pstate 1
//
//##############################################################
//##############################################################
//
// 4.3.5(E) Set the PHY input clocks to the desired frequency 533 
// 
// Set the PHY input clocks to the desired operating frequency. Before proceeding to the next step, 
// the clock should be stable at the new frequency. For more information on clocking requirements, see "Clocks" on page <XXX>.
// 
//##############################################################
//##############################################################
//
// 4.3.5(F) Load the 1D DMEM image and write the 1D Message Block parameters for the training firmware 
// 
// The procedure is as follows: 
// 
//##############################################################
// 1.    Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0. 
//       This allows the memory controller unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Load the firmware DMEM segment to initialize the data structures.
// 3.	Write the Firmware Message Block with the required contents detailing the training parameters.
#define DDRPHY_54000_90000_VAL 0x0
#define DDRPHY_54001_90004_VAL 0x0
#define DDRPHY_54002_90008_VAL 0x1
#define DDRPHY_54003_9000c_VAL 0x854
#define DDRPHY_54004_90010_VAL 0x1
#define DDRPHY_54005_90014_VAL 0x3c3c
#define DDRPHY_54006_90018_VAL 0x0
#define DDRPHY_54007_9001c_VAL 0x0
#define DDRPHY_54008_90020_VAL 0x0
#define DDRPHY_54009_90024_VAL 0x0
#define DDRPHY_5400a_90028_VAL 0x0
#define DDRPHY_5400b_9002c_VAL 0x0
#define DDRPHY_5400c_90030_VAL 0x0
#define DDRPHY_5400d_90034_VAL 0x0
#define DDRPHY_5400e_90038_VAL 0x0
#define DDRPHY_5400f_9003c_VAL 0x0
#define DDRPHY_54010_90040_VAL 0x0
#define DDRPHY_54011_90044_VAL 0x0
#define DDRPHY_54012_90048_VAL 0x0
#define DDRPHY_54013_9004c_VAL 0x0
#define DDRPHY_54014_90050_VAL 0x0
#define DDRPHY_54015_90054_VAL 0x0
#define DDRPHY_54016_90058_VAL 0x0
#define DDRPHY_54017_9005c_VAL 0x0
#define DDRPHY_54018_90060_VAL 0x0
#define DDRPHY_54019_90064_VAL 0x0
#define DDRPHY_5401a_90068_VAL 0x0
#define DDRPHY_5401b_9006c_VAL 0x0
#define DDRPHY_5401c_90070_VAL 0x0
#define DDRPHY_5401d_90074_VAL 0x0
#define DDRPHY_5401e_90078_VAL 0x0
#define DDRPHY_5401f_9007c_VAL 0x0
#define DDRPHY_54020_90080_VAL 0x0
#define DDRPHY_54021_90084_VAL 0x0
#define DDRPHY_54022_90088_VAL 0x0
#define DDRPHY_54023_9008c_VAL 0x0
#define DDRPHY_54024_90090_VAL 0x0
#define DDRPHY_54025_90094_VAL 0x0
#define DDRPHY_54026_90098_VAL 0x0
#define DDRPHY_54027_9009c_VAL 0x0
#define DDRPHY_54028_900a0_VAL 0x0
#define DDRPHY_54029_900a4_VAL 0x0
#define DDRPHY_5402a_900a8_VAL 0x0
#define DDRPHY_5402b_900ac_VAL 0x0
#define DDRPHY_5402c_900b0_VAL 0x0
#define DDRPHY_5402d_900b4_VAL 0x0
#define DDRPHY_5402e_900b8_VAL 0x0
#define DDRPHY_5402f_900bc_VAL 0x0
#define DDRPHY_54030_900c0_VAL 0x0
#define DDRPHY_54031_900c4_VAL 0x0
#define DDRPHY_54032_900c8_VAL 0x0
#define DDRPHY_54033_900cc_VAL 0x0
#define DDRPHY_54034_900d0_VAL 0x0
#define DDRPHY_54035_900d4_VAL 0x0
#define DDRPHY_54036_900d8_VAL 0x0
#define DDRPHY_54037_900dc_VAL 0x0
#define DDRPHY_54038_900e0_VAL 0x0
#define DDRPHY_54039_900e4_VAL 0x0
#define DDRPHY_5403a_900e8_VAL 0x0
#define DDRPHY_5403b_900ec_VAL 0x0
#define DDRPHY_5403c_900f0_VAL 0x0
#define DDRPHY_5403d_900f4_VAL 0x0
#define DDRPHY_5403e_900f8_VAL 0x0
#define DDRPHY_5403f_900fc_VAL 0x0
// 4.    Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
//      This allows the firmware unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x1
//##############################################################
//
// 4.3.7(G) Execute the Training Firmware 
// 
// The training firmware is executed with the following procedure: 
//
//##############################################################
// 1.  Reset the firmware microcontroller by writing the MicroReset CSR to set the StallToMicro and 
//     ResetToMicro fields to 1 (all other fields should be zero). 
//     Then rewrite the CSR so that only the StallToMicro remains set (all other fields should be zero). 
#define DDRPHY_d0000_a8000_VAL 0x1
#define DDRPHY_d0099_a8264_VAL 0x9
#define DDRPHY_d0099_a8264_VAL 0x1
// 2. Begin execution of the training firmware by setting the MicroReset CSR to 4'b0000. 
#define DDRPHY_d0099_a8264_VAL 0x0
// 3.   Wait for the training firmware to complete by following the procedure in "uCtrl Initialization and Mailbox Messaging" 
//##############################################################
//
// 4.3.8(H) Read the Message Block results
// 
// The procedure is as follows:
// 
//##############################################################
// 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Read the Firmware Message Block to obtain the results from the training 
// 3.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
#define DDRPHY_d0000_a8000_VAL 0x1
// 4.	If training is required at another frequency, repeat the operations starting at step (E). 
//##############################################################
//
// [phyinit_sequence] Steps D to H for 1D Training of Pstate 2
//
//##############################################################
//##############################################################
//
// 4.3.5(E) Set the PHY input clocks to the desired frequency 533 
// 
// Set the PHY input clocks to the desired operating frequency. Before proceeding to the next step, 
// the clock should be stable at the new frequency. For more information on clocking requirements, see "Clocks" on page <XXX>.
// 
//##############################################################
//##############################################################
//
// 4.3.5(F) Load the 1D DMEM image and write the 1D Message Block parameters for the training firmware 
// 
// The procedure is as follows: 
// 
//##############################################################
// 1.    Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0. 
//       This allows the memory controller unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Load the firmware DMEM segment to initialize the data structures.
// 3.	Write the Firmware Message Block with the required contents detailing the training parameters.
#define DDRPHY_54000_90000_VAL 0x0
#define DDRPHY_54001_90004_VAL 0x0
#define DDRPHY_54002_90008_VAL 0x2
#define DDRPHY_54003_9000c_VAL 0x854
#define DDRPHY_54004_90010_VAL 0x1
#define DDRPHY_54005_90014_VAL 0x3c3c
#define DDRPHY_54006_90018_VAL 0x0
#define DDRPHY_54007_9001c_VAL 0x0
#define DDRPHY_54008_90020_VAL 0x0
#define DDRPHY_54009_90024_VAL 0x0
#define DDRPHY_5400a_90028_VAL 0x0
#define DDRPHY_5400b_9002c_VAL 0x0
#define DDRPHY_5400c_90030_VAL 0x0
#define DDRPHY_5400d_90034_VAL 0x0
#define DDRPHY_5400e_90038_VAL 0x0
#define DDRPHY_5400f_9003c_VAL 0x0
#define DDRPHY_54010_90040_VAL 0x0
#define DDRPHY_54011_90044_VAL 0x0
#define DDRPHY_54012_90048_VAL 0x0
#define DDRPHY_54013_9004c_VAL 0x0
#define DDRPHY_54014_90050_VAL 0x0
#define DDRPHY_54015_90054_VAL 0x0
#define DDRPHY_54016_90058_VAL 0x0
#define DDRPHY_54017_9005c_VAL 0x0
#define DDRPHY_54018_90060_VAL 0x0
#define DDRPHY_54019_90064_VAL 0x0
#define DDRPHY_5401a_90068_VAL 0x0
#define DDRPHY_5401b_9006c_VAL 0x0
#define DDRPHY_5401c_90070_VAL 0x0
#define DDRPHY_5401d_90074_VAL 0x0
#define DDRPHY_5401e_90078_VAL 0x0
#define DDRPHY_5401f_9007c_VAL 0x0
#define DDRPHY_54020_90080_VAL 0x0
#define DDRPHY_54021_90084_VAL 0x0
#define DDRPHY_54022_90088_VAL 0x0
#define DDRPHY_54023_9008c_VAL 0x0
#define DDRPHY_54024_90090_VAL 0x0
#define DDRPHY_54025_90094_VAL 0x0
#define DDRPHY_54026_90098_VAL 0x0
#define DDRPHY_54027_9009c_VAL 0x0
#define DDRPHY_54028_900a0_VAL 0x0
#define DDRPHY_54029_900a4_VAL 0x0
#define DDRPHY_5402a_900a8_VAL 0x0
#define DDRPHY_5402b_900ac_VAL 0x0
#define DDRPHY_5402c_900b0_VAL 0x0
#define DDRPHY_5402d_900b4_VAL 0x0
#define DDRPHY_5402e_900b8_VAL 0x0
#define DDRPHY_5402f_900bc_VAL 0x0
#define DDRPHY_54030_900c0_VAL 0x0
#define DDRPHY_54031_900c4_VAL 0x0
#define DDRPHY_54032_900c8_VAL 0x0
#define DDRPHY_54033_900cc_VAL 0x0
#define DDRPHY_54034_900d0_VAL 0x0
#define DDRPHY_54035_900d4_VAL 0x0
#define DDRPHY_54036_900d8_VAL 0x0
#define DDRPHY_54037_900dc_VAL 0x0
#define DDRPHY_54038_900e0_VAL 0x0
#define DDRPHY_54039_900e4_VAL 0x0
#define DDRPHY_5403a_900e8_VAL 0x0
#define DDRPHY_5403b_900ec_VAL 0x0
#define DDRPHY_5403c_900f0_VAL 0x0
#define DDRPHY_5403d_900f4_VAL 0x0
#define DDRPHY_5403e_900f8_VAL 0x0
#define DDRPHY_5403f_900fc_VAL 0x0
// 4.    Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
//      This allows the firmware unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x1
//##############################################################
//
// 4.3.7(G) Execute the Training Firmware 
// 
// The training firmware is executed with the following procedure: 
//
//##############################################################
// 1.  Reset the firmware microcontroller by writing the MicroReset CSR to set the StallToMicro and 
//     ResetToMicro fields to 1 (all other fields should be zero). 
//     Then rewrite the CSR so that only the StallToMicro remains set (all other fields should be zero). 
#define DDRPHY_d0000_a8000_VAL 0x1
#define DDRPHY_d0099_a8264_VAL 0x9
#define DDRPHY_d0099_a8264_VAL 0x1
// 2. Begin execution of the training firmware by setting the MicroReset CSR to 4'b0000. 
#define DDRPHY_d0099_a8264_VAL 0x0
// 3.   Wait for the training firmware to complete by following the procedure in "uCtrl Initialization and Mailbox Messaging" 
//##############################################################
//
// 4.3.8(H) Read the Message Block results
// 
// The procedure is as follows:
// 
//##############################################################
// 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Read the Firmware Message Block to obtain the results from the training 
// 3.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
#define DDRPHY_d0000_a8000_VAL 0x1
// 4.	If training is required at another frequency, repeat the operations starting at step (E). 
//##############################################################
//
// [phyinit_sequence] Steps D to H for 1D Training of Pstate 3
//
//##############################################################
//##############################################################
//
// 4.3.5(E) Set the PHY input clocks to the desired frequency 533 
// 
// Set the PHY input clocks to the desired operating frequency. Before proceeding to the next step, 
// the clock should be stable at the new frequency. For more information on clocking requirements, see "Clocks" on page <XXX>.
// 
//##############################################################
//##############################################################
//
// 4.3.5(F) Load the 1D DMEM image and write the 1D Message Block parameters for the training firmware 
// 
// The procedure is as follows: 
// 
//##############################################################
// 1.    Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0. 
//       This allows the memory controller unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Load the firmware DMEM segment to initialize the data structures.
// 3.	Write the Firmware Message Block with the required contents detailing the training parameters.
#define DDRPHY_54000_90000_VAL 0x0
#define DDRPHY_54001_90004_VAL 0x0
#define DDRPHY_54002_90008_VAL 0x3
#define DDRPHY_54003_9000c_VAL 0x854
#define DDRPHY_54004_90010_VAL 0x1
#define DDRPHY_54005_90014_VAL 0x3c3c
#define DDRPHY_54006_90018_VAL 0x0
#define DDRPHY_54007_9001c_VAL 0x0
#define DDRPHY_54008_90020_VAL 0x0
#define DDRPHY_54009_90024_VAL 0x0
#define DDRPHY_5400a_90028_VAL 0x0
#define DDRPHY_5400b_9002c_VAL 0x0
#define DDRPHY_5400c_90030_VAL 0x0
#define DDRPHY_5400d_90034_VAL 0x0
#define DDRPHY_5400e_90038_VAL 0x0
#define DDRPHY_5400f_9003c_VAL 0x0
#define DDRPHY_54010_90040_VAL 0x0
#define DDRPHY_54011_90044_VAL 0x0
#define DDRPHY_54012_90048_VAL 0x0
#define DDRPHY_54013_9004c_VAL 0x0
#define DDRPHY_54014_90050_VAL 0x0
#define DDRPHY_54015_90054_VAL 0x0
#define DDRPHY_54016_90058_VAL 0x0
#define DDRPHY_54017_9005c_VAL 0x0
#define DDRPHY_54018_90060_VAL 0x0
#define DDRPHY_54019_90064_VAL 0x0
#define DDRPHY_5401a_90068_VAL 0x0
#define DDRPHY_5401b_9006c_VAL 0x0
#define DDRPHY_5401c_90070_VAL 0x0
#define DDRPHY_5401d_90074_VAL 0x0
#define DDRPHY_5401e_90078_VAL 0x0
#define DDRPHY_5401f_9007c_VAL 0x0
#define DDRPHY_54020_90080_VAL 0x0
#define DDRPHY_54021_90084_VAL 0x0
#define DDRPHY_54022_90088_VAL 0x0
#define DDRPHY_54023_9008c_VAL 0x0
#define DDRPHY_54024_90090_VAL 0x0
#define DDRPHY_54025_90094_VAL 0x0
#define DDRPHY_54026_90098_VAL 0x0
#define DDRPHY_54027_9009c_VAL 0x0
#define DDRPHY_54028_900a0_VAL 0x0
#define DDRPHY_54029_900a4_VAL 0x0
#define DDRPHY_5402a_900a8_VAL 0x0
#define DDRPHY_5402b_900ac_VAL 0x0
#define DDRPHY_5402c_900b0_VAL 0x0
#define DDRPHY_5402d_900b4_VAL 0x0
#define DDRPHY_5402e_900b8_VAL 0x0
#define DDRPHY_5402f_900bc_VAL 0x0
#define DDRPHY_54030_900c0_VAL 0x0
#define DDRPHY_54031_900c4_VAL 0x0
#define DDRPHY_54032_900c8_VAL 0x0
#define DDRPHY_54033_900cc_VAL 0x0
#define DDRPHY_54034_900d0_VAL 0x0
#define DDRPHY_54035_900d4_VAL 0x0
#define DDRPHY_54036_900d8_VAL 0x0
#define DDRPHY_54037_900dc_VAL 0x0
#define DDRPHY_54038_900e0_VAL 0x0
#define DDRPHY_54039_900e4_VAL 0x0
#define DDRPHY_5403a_900e8_VAL 0x0
#define DDRPHY_5403b_900ec_VAL 0x0
#define DDRPHY_5403c_900f0_VAL 0x0
#define DDRPHY_5403d_900f4_VAL 0x0
#define DDRPHY_5403e_900f8_VAL 0x0
#define DDRPHY_5403f_900fc_VAL 0x0
// 4.    Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
//      This allows the firmware unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x1
//##############################################################
//
// 4.3.7(G) Execute the Training Firmware 
// 
// The training firmware is executed with the following procedure: 
//
//##############################################################
// 1.  Reset the firmware microcontroller by writing the MicroReset CSR to set the StallToMicro and 
//     ResetToMicro fields to 1 (all other fields should be zero). 
//     Then rewrite the CSR so that only the StallToMicro remains set (all other fields should be zero). 
#define DDRPHY_d0000_a8000_VAL 0x1
#define DDRPHY_d0099_a8264_VAL 0x9
#define DDRPHY_d0099_a8264_VAL 0x1
// 2. Begin execution of the training firmware by setting the MicroReset CSR to 4'b0000. 
#define DDRPHY_d0099_a8264_VAL 0x0
// 3.   Wait for the training firmware to complete by following the procedure in "uCtrl Initialization and Mailbox Messaging" 
//##############################################################
//
// 4.3.8(H) Read the Message Block results
// 
// The procedure is as follows:
// 
//##############################################################
// 1.	Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0.
#define DDRPHY_d0000_a8000_VAL 0x0
// 2.	Read the Firmware Message Block to obtain the results from the training 
// 3.	Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
#define DDRPHY_d0000_a8000_VAL 0x1
// 4.	If training is required at another frequency, repeat the operations starting at step (E). 
//##############################################################
//
// 4.3.9(I) Load PHY Init Engine Image 
// 
// Load the PHY Initialization Engine memory with the provided initialization sequence.
// 
// <Note: For LPDDR3/LPDDR4, this sequence will include the necessary retraining code.>
// 
//##############################################################
// Enable access to the internal CSRs by setting the MicroContMuxSel CSR to 0. 
// This allows the memory controller unrestricted access to the configuration CSRs. 
#define DDRPHY_d0000_a8000_VAL 0x0
#define DDRPHY_90000_1c000_VAL 0x0
#define DDRPHY_90001_1c004_VAL 0x0
#define DDRPHY_90002_1c008_VAL 0x8
#define DDRPHY_90003_1c00c_VAL 0x0
#define DDRPHY_90004_1c010_VAL 0x0
#define DDRPHY_90005_1c014_VAL 0x8
#define DDRPHY_90029_1c0a4_VAL 0x0
#define DDRPHY_9002a_1c0a8_VAL 0x45a
#define DDRPHY_9002b_1c0ac_VAL 0x19
#define DDRPHY_9002c_1c0b0_VAL 0x8
#define DDRPHY_9002d_1c0b4_VAL 0x478
#define DDRPHY_9002e_1c0b8_VAL 0x119
#define DDRPHY_9002f_1c0bc_VAL 0xb
#define DDRPHY_90030_1c0c0_VAL 0x480
#define DDRPHY_90031_1c0c4_VAL 0x119
#define DDRPHY_90032_1c0c8_VAL 0x8
#define DDRPHY_90033_1c0cc_VAL 0x448
#define DDRPHY_90034_1c0d0_VAL 0x139
#define DDRPHY_90035_1c0d4_VAL 0x0
#define DDRPHY_90036_1c0d8_VAL 0xe8
#define DDRPHY_90037_1c0dc_VAL 0x109
#define DDRPHY_90038_1c0e0_VAL 0x2
#define DDRPHY_90039_1c0e4_VAL 0x10
#define DDRPHY_9003a_1c0e8_VAL 0x139
#define DDRPHY_9003b_1c0ec_VAL 0xf
#define DDRPHY_9003c_1c0f0_VAL 0x7c0
#define DDRPHY_9003d_1c0f4_VAL 0x139
#define DDRPHY_9003e_1c0f8_VAL 0x44
#define DDRPHY_9003f_1c0fc_VAL 0x630
#define DDRPHY_90040_1c100_VAL 0x159
#define DDRPHY_90041_1c104_VAL 0x14f
#define DDRPHY_90042_1c108_VAL 0x630
#define DDRPHY_90043_1c10c_VAL 0x159
#define DDRPHY_90044_1c110_VAL 0x47
#define DDRPHY_90045_1c114_VAL 0x630
#define DDRPHY_90046_1c118_VAL 0x149
#define DDRPHY_90047_1c11c_VAL 0x4f
#define DDRPHY_90048_1c120_VAL 0x630
#define DDRPHY_90049_1c124_VAL 0x179
#define DDRPHY_9004a_1c128_VAL 0x8
#define DDRPHY_9004b_1c12c_VAL 0xe0
#define DDRPHY_9004c_1c130_VAL 0x109
#define DDRPHY_9004d_1c134_VAL 0x0
#define DDRPHY_9004e_1c138_VAL 0x7c8
#define DDRPHY_9004f_1c13c_VAL 0x109
#define DDRPHY_90050_1c140_VAL 0x0
#define DDRPHY_90051_1c144_VAL 0x1
#define DDRPHY_90052_1c148_VAL 0x8
#define DDRPHY_90053_1c14c_VAL 0x0
#define DDRPHY_90054_1c150_VAL 0x45a
#define DDRPHY_90055_1c154_VAL 0x39
#define DDRPHY_90056_1c158_VAL 0x0
#define DDRPHY_90057_1c15c_VAL 0x448
#define DDRPHY_90058_1c160_VAL 0x109
#define DDRPHY_90059_1c164_VAL 0x41
#define DDRPHY_9005a_1c168_VAL 0x630
#define DDRPHY_9005b_1c16c_VAL 0x179
#define DDRPHY_9005c_1c170_VAL 0x1
#define DDRPHY_9005d_1c174_VAL 0x618
#define DDRPHY_9005e_1c178_VAL 0x119
#define DDRPHY_9005f_1c17c_VAL 0x40c0
#define DDRPHY_90060_1c180_VAL 0x630
#define DDRPHY_90061_1c184_VAL 0x149
#define DDRPHY_90062_1c188_VAL 0x8
#define DDRPHY_90063_1c18c_VAL 0x4
#define DDRPHY_90064_1c190_VAL 0x48
#define DDRPHY_90065_1c194_VAL 0x4040
#define DDRPHY_90066_1c198_VAL 0x630
#define DDRPHY_90067_1c19c_VAL 0x149
#define DDRPHY_90068_1c1a0_VAL 0x0
#define DDRPHY_90069_1c1a4_VAL 0x4
#define DDRPHY_9006a_1c1a8_VAL 0x48
#define DDRPHY_9006b_1c1ac_VAL 0x40
#define DDRPHY_9006c_1c1b0_VAL 0x630
#define DDRPHY_9006d_1c1b4_VAL 0x149
#define DDRPHY_9006e_1c1b8_VAL 0x10
#define DDRPHY_9006f_1c1bc_VAL 0x4
#define DDRPHY_90070_1c1c0_VAL 0x18
#define DDRPHY_90071_1c1c4_VAL 0x0
#define DDRPHY_90072_1c1c8_VAL 0x4
#define DDRPHY_90073_1c1cc_VAL 0x78
#define DDRPHY_90074_1c1d0_VAL 0x549
#define DDRPHY_90075_1c1d4_VAL 0x630
#define DDRPHY_90076_1c1d8_VAL 0x159
#define DDRPHY_90077_1c1dc_VAL 0xd49
#define DDRPHY_90078_1c1e0_VAL 0x630
#define DDRPHY_90079_1c1e4_VAL 0x159
#define DDRPHY_9007a_1c1e8_VAL 0x94c
#define DDRPHY_9007b_1c1ec_VAL 0x630
#define DDRPHY_9007c_1c1f0_VAL 0x159
#define DDRPHY_9007d_1c1f4_VAL 0x94c
#define DDRPHY_9007e_1c1f8_VAL 0x630
#define DDRPHY_9007f_1c1fc_VAL 0x159
#define DDRPHY_90080_1c200_VAL 0x442
#define DDRPHY_90081_1c204_VAL 0x630
#define DDRPHY_90082_1c208_VAL 0x149
#define DDRPHY_90083_1c20c_VAL 0x42
#define DDRPHY_90084_1c210_VAL 0x630
#define DDRPHY_90085_1c214_VAL 0x149
#define DDRPHY_90086_1c218_VAL 0x1
#define DDRPHY_90087_1c21c_VAL 0x630
#define DDRPHY_90088_1c220_VAL 0x149
#define DDRPHY_90089_1c224_VAL 0x0
#define DDRPHY_9008a_1c228_VAL 0xe0
#define DDRPHY_9008b_1c22c_VAL 0x109
#define DDRPHY_9008c_1c230_VAL 0xa
#define DDRPHY_9008d_1c234_VAL 0x10
#define DDRPHY_9008e_1c238_VAL 0x109
#define DDRPHY_9008f_1c23c_VAL 0x9
#define DDRPHY_90090_1c240_VAL 0x3c0
#define DDRPHY_90091_1c244_VAL 0x149
#define DDRPHY_90092_1c248_VAL 0x9
#define DDRPHY_90093_1c24c_VAL 0x3c0
#define DDRPHY_90094_1c250_VAL 0x159
#define DDRPHY_90095_1c254_VAL 0x18
#define DDRPHY_90096_1c258_VAL 0x10
#define DDRPHY_90097_1c25c_VAL 0x109
#define DDRPHY_90098_1c260_VAL 0x0
#define DDRPHY_90099_1c264_VAL 0x3c0
#define DDRPHY_9009a_1c268_VAL 0x109
#define DDRPHY_9009b_1c26c_VAL 0x18
#define DDRPHY_9009c_1c270_VAL 0x4
#define DDRPHY_9009d_1c274_VAL 0x8
#define DDRPHY_9009e_1c278_VAL 0xc
#define DDRPHY_9009f_1c27c_VAL 0x10
#define DDRPHY_900a0_1c280_VAL 0x109
#define DDRPHY_900a1_1c284_VAL 0x3
#define DDRPHY_900a2_1c288_VAL 0x10
#define DDRPHY_900a3_1c28c_VAL 0x109
#define DDRPHY_900a4_1c290_VAL 0x7
#define DDRPHY_900a5_1c294_VAL 0x7c0
#define DDRPHY_900a6_1c298_VAL 0x109
#define DDRPHY_900a7_1c29c_VAL 0x10
#define DDRPHY_900a8_1c2a0_VAL 0x10
#define DDRPHY_900a9_1c2a4_VAL 0x109
#define DDRPHY_40000_18000_VAL 0x811
#define DDRPHY_40020_18080_VAL 0x880
#define DDRPHY_40040_18100_VAL 0x0
#define DDRPHY_40060_18180_VAL 0x0
#define DDRPHY_40001_18004_VAL 0x4016
#define DDRPHY_40021_18084_VAL 0x83
#define DDRPHY_40041_18104_VAL 0x4f
#define DDRPHY_40061_18184_VAL 0x0
#define DDRPHY_40002_18008_VAL 0x4040
#define DDRPHY_40022_18088_VAL 0x83
#define DDRPHY_40042_18108_VAL 0x51
#define DDRPHY_40062_18188_VAL 0x0
#define DDRPHY_40003_1800c_VAL 0x811
#define DDRPHY_40023_1808c_VAL 0x880
#define DDRPHY_40043_1810c_VAL 0x0
#define DDRPHY_40063_1818c_VAL 0x0
#define DDRPHY_40004_18010_VAL 0x71e
#define DDRPHY_40024_18090_VAL 0xf
#define DDRPHY_40044_18110_VAL 0x1740
#define DDRPHY_40064_18190_VAL 0x0
#define DDRPHY_40005_18014_VAL 0x16
#define DDRPHY_40025_18094_VAL 0x8f
#define DDRPHY_40045_18114_VAL 0x4b
#define DDRPHY_40065_18194_VAL 0x0
#define DDRPHY_40006_18018_VAL 0x716
#define DDRPHY_40026_18098_VAL 0xf
#define DDRPHY_40046_18118_VAL 0x2001
#define DDRPHY_40066_18198_VAL 0x0
#define DDRPHY_40007_1801c_VAL 0x716
#define DDRPHY_40027_1809c_VAL 0xf
#define DDRPHY_40047_1811c_VAL 0x2800
#define DDRPHY_40067_1819c_VAL 0x0
#define DDRPHY_40008_18020_VAL 0x716
#define DDRPHY_40028_180a0_VAL 0xf
#define DDRPHY_40048_18120_VAL 0xf00
#define DDRPHY_40068_181a0_VAL 0x0
#define DDRPHY_40009_18024_VAL 0x71e
#define DDRPHY_40029_180a4_VAL 0xf
#define DDRPHY_40049_18124_VAL 0x1400
#define DDRPHY_40069_181a4_VAL 0x0
#define DDRPHY_4000a_18028_VAL 0xe08
#define DDRPHY_4002a_180a8_VAL 0xc15
#define DDRPHY_4004a_18128_VAL 0x0
#define DDRPHY_4006a_181a8_VAL 0x0
#define DDRPHY_4000b_1802c_VAL 0x623
#define DDRPHY_4002b_180ac_VAL 0x15
#define DDRPHY_4004b_1812c_VAL 0x0
#define DDRPHY_4006b_181ac_VAL 0x0
#define DDRPHY_4000c_18030_VAL 0x4040
#define DDRPHY_4002c_180b0_VAL 0x80
#define DDRPHY_4004c_18130_VAL 0x0
#define DDRPHY_4006c_181b0_VAL 0x0
#define DDRPHY_4000d_18034_VAL 0xe08
#define DDRPHY_4002d_180b4_VAL 0xc1a
#define DDRPHY_4004d_18134_VAL 0x0
#define DDRPHY_4006d_181b4_VAL 0x0
#define DDRPHY_4000e_18038_VAL 0x623
#define DDRPHY_4002e_180b8_VAL 0x1a
#define DDRPHY_4004e_18138_VAL 0x0
#define DDRPHY_4006e_181b8_VAL 0x0
#define DDRPHY_4000f_1803c_VAL 0x4040
#define DDRPHY_4002f_180bc_VAL 0x80
#define DDRPHY_4004f_1813c_VAL 0x0
#define DDRPHY_4006f_181bc_VAL 0x0
#define DDRPHY_40010_18040_VAL 0x808
#define DDRPHY_40030_180c0_VAL 0xc05
#define DDRPHY_40050_18140_VAL 0x43
#define DDRPHY_40070_181c0_VAL 0x2000
#define DDRPHY_40011_18044_VAL 0x8
#define DDRPHY_40031_180c4_VAL 0x5
#define DDRPHY_40051_18144_VAL 0x43
#define DDRPHY_40071_181c4_VAL 0x2000
#define DDRPHY_40012_18048_VAL 0x840
#define DDRPHY_40032_180c8_VAL 0xc05
#define DDRPHY_40052_18148_VAL 0x43
#define DDRPHY_40072_181c8_VAL 0x2000
#define DDRPHY_40013_1804c_VAL 0x808
#define DDRPHY_40033_180cc_VAL 0xc0a
#define DDRPHY_40053_1814c_VAL 0x43
#define DDRPHY_40073_181cc_VAL 0x2000
#define DDRPHY_40014_18050_VAL 0x8
#define DDRPHY_40034_180d0_VAL 0xa
#define DDRPHY_40054_18150_VAL 0x43
#define DDRPHY_40074_181d0_VAL 0x2000
#define DDRPHY_40015_18054_VAL 0x840
#define DDRPHY_40035_180d4_VAL 0xc0a
#define DDRPHY_40055_18154_VAL 0x43
#define DDRPHY_40075_181d4_VAL 0x2000
#define DDRPHY_40016_18058_VAL 0x4040
#define DDRPHY_40036_180d8_VAL 0x80
#define DDRPHY_40056_18158_VAL 0x0
#define DDRPHY_40076_181d8_VAL 0x0
#define DDRPHY_40017_1805c_VAL 0x60a
#define DDRPHY_40037_180dc_VAL 0x15
#define DDRPHY_40057_1815c_VAL 0x1200
#define DDRPHY_40077_181dc_VAL 0x0
#define DDRPHY_40018_18060_VAL 0x61a
#define DDRPHY_40038_180e0_VAL 0x15
#define DDRPHY_40058_18160_VAL 0x1300
#define DDRPHY_40078_181e0_VAL 0x0
#define DDRPHY_40019_18064_VAL 0x60a
#define DDRPHY_40039_180e4_VAL 0x1a
#define DDRPHY_40059_18164_VAL 0x1200
#define DDRPHY_40079_181e4_VAL 0x0
#define DDRPHY_4001a_18068_VAL 0x65a
#define DDRPHY_4003a_180e8_VAL 0x1a
#define DDRPHY_4005a_18168_VAL 0x1300
#define DDRPHY_4007a_181e8_VAL 0x0
#define DDRPHY_4001b_1806c_VAL 0x4808
#define DDRPHY_4003b_180ec_VAL 0x880
#define DDRPHY_4005b_1816c_VAL 0x0
#define DDRPHY_4007b_181ec_VAL 0x0
#define DDRPHY_900aa_1c2a8_VAL 0x8
#define DDRPHY_900ab_1c2ac_VAL 0x7aa
#define DDRPHY_900ac_1c2b0_VAL 0x2a
#define DDRPHY_900ad_1c2b4_VAL 0x10
#define DDRPHY_900ae_1c2b8_VAL 0x7b2
#define DDRPHY_900af_1c2bc_VAL 0x2a
#define DDRPHY_900b0_1c2c0_VAL 0x10
#define DDRPHY_900b1_1c2c4_VAL 0x400
#define DDRPHY_900b2_1c2c8_VAL 0x10e
#define DDRPHY_900b3_1c2cc_VAL 0x10
#define DDRPHY_900b4_1c2d0_VAL 0x2a8
#define DDRPHY_900b5_1c2d4_VAL 0x129
#define DDRPHY_900b6_1c2d8_VAL 0x8
#define DDRPHY_900b7_1c2dc_VAL 0x370
#define DDRPHY_900b8_1c2e0_VAL 0x129
#define DDRPHY_900b9_1c2e4_VAL 0xa
#define DDRPHY_900ba_1c2e8_VAL 0x3c8
#define DDRPHY_900bb_1c2ec_VAL 0x1a9
#define DDRPHY_900bc_1c2f0_VAL 0x4
#define DDRPHY_900bd_1c2f4_VAL 0x790
#define DDRPHY_900be_1c2f8_VAL 0x11a
#define DDRPHY_900bf_1c2fc_VAL 0xc
#define DDRPHY_900c0_1c300_VAL 0x408
#define DDRPHY_900c1_1c304_VAL 0x119
#define DDRPHY_900c2_1c308_VAL 0x4
#define DDRPHY_900c3_1c30c_VAL 0x0
#define DDRPHY_900c4_1c310_VAL 0x18
#define DDRPHY_900c5_1c314_VAL 0x0
#define DDRPHY_900c6_1c318_VAL 0x408
#define DDRPHY_900c7_1c31c_VAL 0x119
#define DDRPHY_900c8_1c320_VAL 0x14
#define DDRPHY_900c9_1c324_VAL 0x790
#define DDRPHY_900ca_1c328_VAL 0x11a
#define DDRPHY_900cb_1c32c_VAL 0x8
#define DDRPHY_900cc_1c330_VAL 0x4
#define DDRPHY_900cd_1c334_VAL 0x18
#define DDRPHY_900ce_1c338_VAL 0xc
#define DDRPHY_900cf_1c33c_VAL 0x408
#define DDRPHY_900d0_1c340_VAL 0x119
#define DDRPHY_900d1_1c344_VAL 0x7
#define DDRPHY_900d2_1c348_VAL 0x0
#define DDRPHY_900d3_1c34c_VAL 0x18
#define DDRPHY_900d4_1c350_VAL 0x0
#define DDRPHY_900d5_1c354_VAL 0x408
#define DDRPHY_900d6_1c358_VAL 0x119
#define DDRPHY_900d7_1c35c_VAL 0x8
#define DDRPHY_900d8_1c360_VAL 0x8568
#define DDRPHY_900d9_1c364_VAL 0x108
#define DDRPHY_900da_1c368_VAL 0x18
#define DDRPHY_900db_1c36c_VAL 0x790
#define DDRPHY_900dc_1c370_VAL 0x16a
#define DDRPHY_900dd_1c374_VAL 0x8
#define DDRPHY_900de_1c378_VAL 0x1d8
#define DDRPHY_900df_1c37c_VAL 0x169
#define DDRPHY_900e0_1c380_VAL 0x10
#define DDRPHY_900e1_1c384_VAL 0x8558
#define DDRPHY_900e2_1c388_VAL 0x168
#define DDRPHY_900e3_1c38c_VAL 0x70
#define DDRPHY_900e4_1c390_VAL 0x788
#define DDRPHY_900e5_1c394_VAL 0x16a
#define DDRPHY_900e6_1c398_VAL 0x1ff8
#define DDRPHY_900e7_1c39c_VAL 0x85a8
#define DDRPHY_900e8_1c3a0_VAL 0x1e8
#define DDRPHY_900e9_1c3a4_VAL 0x50
#define DDRPHY_900ea_1c3a8_VAL 0x798
#define DDRPHY_900eb_1c3ac_VAL 0x16a
#define DDRPHY_900ec_1c3b0_VAL 0x60
#define DDRPHY_900ed_1c3b4_VAL 0x7a0
#define DDRPHY_900ee_1c3b8_VAL 0x16a
#define DDRPHY_900ef_1c3bc_VAL 0x8
#define DDRPHY_900f0_1c3c0_VAL 0x8310
#define DDRPHY_900f1_1c3c4_VAL 0x168
#define DDRPHY_900f2_1c3c8_VAL 0x8
#define DDRPHY_900f3_1c3cc_VAL 0xa310
#define DDRPHY_900f4_1c3d0_VAL 0x168
#define DDRPHY_900f5_1c3d4_VAL 0xa
#define DDRPHY_900f6_1c3d8_VAL 0x408
#define DDRPHY_900f7_1c3dc_VAL 0x169
#define DDRPHY_900f8_1c3e0_VAL 0x6e
#define DDRPHY_900f9_1c3e4_VAL 0x0
#define DDRPHY_900fa_1c3e8_VAL 0x68
#define DDRPHY_900fb_1c3ec_VAL 0x0
#define DDRPHY_900fc_1c3f0_VAL 0x408
#define DDRPHY_900fd_1c3f4_VAL 0x169
#define DDRPHY_900fe_1c3f8_VAL 0x0
#define DDRPHY_900ff_1c3fc_VAL 0x8310
#define DDRPHY_90100_1c400_VAL 0x168
#define DDRPHY_90101_1c404_VAL 0x0
#define DDRPHY_90102_1c408_VAL 0xa310
#define DDRPHY_90103_1c40c_VAL 0x168
#define DDRPHY_90104_1c410_VAL 0x1ff8
#define DDRPHY_90105_1c414_VAL 0x85a8
#define DDRPHY_90106_1c418_VAL 0x1e8
#define DDRPHY_90107_1c41c_VAL 0x68
#define DDRPHY_90108_1c420_VAL 0x798
#define DDRPHY_90109_1c424_VAL 0x16a
#define DDRPHY_9010a_1c428_VAL 0x78
#define DDRPHY_9010b_1c42c_VAL 0x7a0
#define DDRPHY_9010c_1c430_VAL 0x16a
#define DDRPHY_9010d_1c434_VAL 0x68
#define DDRPHY_9010e_1c438_VAL 0x790
#define DDRPHY_9010f_1c43c_VAL 0x16a
#define DDRPHY_90110_1c440_VAL 0x8
#define DDRPHY_90111_1c444_VAL 0x8b10
#define DDRPHY_90112_1c448_VAL 0x168
#define DDRPHY_90113_1c44c_VAL 0x8
#define DDRPHY_90114_1c450_VAL 0xab10
#define DDRPHY_90115_1c454_VAL 0x168
#define DDRPHY_90116_1c458_VAL 0xa
#define DDRPHY_90117_1c45c_VAL 0x408
#define DDRPHY_90118_1c460_VAL 0x169
#define DDRPHY_90119_1c464_VAL 0x5b
#define DDRPHY_9011a_1c468_VAL 0x0
#define DDRPHY_9011b_1c46c_VAL 0x68
#define DDRPHY_9011c_1c470_VAL 0x0
#define DDRPHY_9011d_1c474_VAL 0x408
#define DDRPHY_9011e_1c478_VAL 0x169
#define DDRPHY_9011f_1c47c_VAL 0x0
#define DDRPHY_90120_1c480_VAL 0x8b10
#define DDRPHY_90121_1c484_VAL 0x168
#define DDRPHY_90122_1c488_VAL 0x0
#define DDRPHY_90123_1c48c_VAL 0xab10
#define DDRPHY_90124_1c490_VAL 0x168
#define DDRPHY_90125_1c494_VAL 0x0
#define DDRPHY_90126_1c498_VAL 0x1d8
#define DDRPHY_90127_1c49c_VAL 0x169
#define DDRPHY_90128_1c4a0_VAL 0xa
#define DDRPHY_90129_1c4a4_VAL 0x0
#define DDRPHY_9012a_1c4a8_VAL 0x1e9
#define DDRPHY_9012b_1c4ac_VAL 0x8
#define DDRPHY_9012c_1c4b0_VAL 0x8080
#define DDRPHY_9012d_1c4b4_VAL 0x108
#define DDRPHY_9012e_1c4b8_VAL 0x84
#define DDRPHY_9012f_1c4bc_VAL 0x790
#define DDRPHY_90130_1c4c0_VAL 0x16a
#define DDRPHY_90131_1c4c4_VAL 0xc
#define DDRPHY_90132_1c4c8_VAL 0x408
#define DDRPHY_90133_1c4cc_VAL 0x169
#define DDRPHY_90134_1c4d0_VAL 0x1c
#define DDRPHY_90135_1c4d4_VAL 0x0
#define DDRPHY_90136_1c4d8_VAL 0x68
#define DDRPHY_90137_1c4dc_VAL 0x2
#define DDRPHY_90138_1c4e0_VAL 0x408
#define DDRPHY_90139_1c4e4_VAL 0x169
#define DDRPHY_9013a_1c4e8_VAL 0x0
#define DDRPHY_9013b_1c4ec_VAL 0x8080
#define DDRPHY_9013c_1c4f0_VAL 0x108
#define DDRPHY_9013d_1c4f4_VAL 0x8
#define DDRPHY_9013e_1c4f8_VAL 0x7aa
#define DDRPHY_9013f_1c4fc_VAL 0x6a
#define DDRPHY_90140_1c500_VAL 0x0
#define DDRPHY_90141_1c504_VAL 0x8568
#define DDRPHY_90142_1c508_VAL 0x108
#define DDRPHY_90143_1c50c_VAL 0xa
#define DDRPHY_90144_1c510_VAL 0x0
#define DDRPHY_90145_1c514_VAL 0x1a9
#define DDRPHY_90146_1c518_VAL 0x9
#define DDRPHY_90147_1c51c_VAL 0x0
#define DDRPHY_90148_1c520_VAL 0x68
#define DDRPHY_90149_1c524_VAL 0x8
#define DDRPHY_9014a_1c528_VAL 0x8558
#define DDRPHY_9014b_1c52c_VAL 0x168
#define DDRPHY_9014c_1c530_VAL 0xbc
#define DDRPHY_9014d_1c534_VAL 0x790
#define DDRPHY_9014e_1c538_VAL 0x16a
#define DDRPHY_9014f_1c53c_VAL 0xc
#define DDRPHY_90150_1c540_VAL 0x408
#define DDRPHY_90151_1c544_VAL 0x169
#define DDRPHY_90152_1c548_VAL 0x12
#define DDRPHY_90153_1c54c_VAL 0x0
#define DDRPHY_90154_1c550_VAL 0x68
#define DDRPHY_90155_1c554_VAL 0x2
#define DDRPHY_90156_1c558_VAL 0x408
#define DDRPHY_90157_1c55c_VAL 0x169
#define DDRPHY_90158_1c560_VAL 0x0
#define DDRPHY_90159_1c564_VAL 0x8558
#define DDRPHY_9015a_1c568_VAL 0x168
#define DDRPHY_9015b_1c56c_VAL 0x8
#define DDRPHY_9015c_1c570_VAL 0x3c8
#define DDRPHY_9015d_1c574_VAL 0x1a9
#define DDRPHY_9015e_1c578_VAL 0x3
#define DDRPHY_9015f_1c57c_VAL 0x370
#define DDRPHY_90160_1c580_VAL 0x129
#define DDRPHY_90161_1c584_VAL 0x0
#define DDRPHY_90162_1c588_VAL 0x2a8
#define DDRPHY_90163_1c58c_VAL 0x129
#define DDRPHY_90164_1c590_VAL 0x0
#define DDRPHY_90165_1c594_VAL 0x400
#define DDRPHY_90166_1c598_VAL 0x10e
#define DDRPHY_90167_1c59c_VAL 0x8
#define DDRPHY_90168_1c5a0_VAL 0xe8
#define DDRPHY_90169_1c5a4_VAL 0x109
#define DDRPHY_9016a_1c5a8_VAL 0x0
#define DDRPHY_9016b_1c5ac_VAL 0x8140
#define DDRPHY_9016c_1c5b0_VAL 0x10c
#define DDRPHY_9016d_1c5b4_VAL 0x10
#define DDRPHY_9016e_1c5b8_VAL 0x8138
#define DDRPHY_9016f_1c5bc_VAL 0x10c
#define DDRPHY_90170_1c5c0_VAL 0x8
#define DDRPHY_90171_1c5c4_VAL 0x7c8
#define DDRPHY_90172_1c5c8_VAL 0x101
#define DDRPHY_90173_1c5cc_VAL 0x8
#define DDRPHY_90174_1c5d0_VAL 0x0
#define DDRPHY_90175_1c5d4_VAL 0x8
#define DDRPHY_90176_1c5d8_VAL 0x8
#define DDRPHY_90177_1c5dc_VAL 0x448
#define DDRPHY_90178_1c5e0_VAL 0x109
#define DDRPHY_90179_1c5e4_VAL 0xf
#define DDRPHY_9017a_1c5e8_VAL 0x7c0
#define DDRPHY_9017b_1c5ec_VAL 0x109
#define DDRPHY_9017c_1c5f0_VAL 0x0
#define DDRPHY_9017d_1c5f4_VAL 0xe8
#define DDRPHY_9017e_1c5f8_VAL 0x109
#define DDRPHY_9017f_1c5fc_VAL 0x47
#define DDRPHY_90180_1c600_VAL 0x630
#define DDRPHY_90181_1c604_VAL 0x109
#define DDRPHY_90182_1c608_VAL 0x8
#define DDRPHY_90183_1c60c_VAL 0x618
#define DDRPHY_90184_1c610_VAL 0x109
#define DDRPHY_90185_1c614_VAL 0x8
#define DDRPHY_90186_1c618_VAL 0xe0
#define DDRPHY_90187_1c61c_VAL 0x109
#define DDRPHY_90188_1c620_VAL 0x0
#define DDRPHY_90189_1c624_VAL 0x7c8
#define DDRPHY_9018a_1c628_VAL 0x109
#define DDRPHY_9018b_1c62c_VAL 0x8
#define DDRPHY_9018c_1c630_VAL 0x8140
#define DDRPHY_9018d_1c634_VAL 0x10c
#define DDRPHY_9018e_1c638_VAL 0x0
#define DDRPHY_9018f_1c63c_VAL 0x1
#define DDRPHY_90190_1c640_VAL 0x8
#define DDRPHY_90191_1c644_VAL 0x8
#define DDRPHY_90192_1c648_VAL 0x7c8
#define DDRPHY_90193_1c64c_VAL 0x101
#define DDRPHY_90006_1c018_VAL 0x0
#define DDRPHY_90007_1c01c_VAL 0x0
#define DDRPHY_90008_1c020_VAL 0x8
#define DDRPHY_90009_1c024_VAL 0x0
#define DDRPHY_9000a_1c028_VAL 0x0
#define DDRPHY_9000b_1c02c_VAL 0x0
#define DDRPHY_d00e7_a839c_VAL 0x400
#define DDRPHY_90017_1c05c_VAL 0x0
#define DDRPHY_90026_1c098_VAL 0x6e
#define DDRPHY_400d0_18340_VAL 0x0
#define DDRPHY_400d1_18344_VAL 0x101
#define DDRPHY_400d2_18348_VAL 0x105
#define DDRPHY_400d3_1834c_VAL 0x107
#define DDRPHY_400d4_18350_VAL 0x10f
#define DDRPHY_400d5_18354_VAL 0x202
#define DDRPHY_400d6_18358_VAL 0x20a
#define DDRPHY_400d7_1835c_VAL 0x20b
#define DDRPHY_9000c_1c030_VAL 0x0
#define DDRPHY_9000d_1c034_VAL 0x73
#define DDRPHY_9000e_1c038_VAL 0x60
#define DDRPHY_9000f_1c03c_VAL 0x6010
#define DDRPHY_90010_1c040_VAL 0x2052
#define DDRPHY_90011_1c044_VAL 0xdfbd
#define DDRPHY_90012_1c048_VAL 0x60
#define DDRPHY_90013_1c04c_VAL 0x6052
#define DDRPHY_2003a_140e8_VAL 0x2
#define DDRPHY_20010_14040_VAL 0x0
#define DDRPHY_120010_34040_VAL 0x0
#define DDRPHY_220010_54040_VAL 0x0
#define DDRPHY_320010_74040_VAL 0x0
#define DDRPHY_400fd_183f4_VAL 0xf
#define DDRPHY_10011_c044_VAL 0x1
#define DDRPHY_10012_c048_VAL 0x1
#define DDRPHY_10013_c04c_VAL 0x180
#define DDRPHY_10018_c060_VAL 0x1
#define DDRPHY_10002_c008_VAL 0x6209
#define DDRPHY_100b2_c2c8_VAL 0x1
#define DDRPHY_101b4_c6d0_VAL 0x1
#define DDRPHY_102b4_cad0_VAL 0x1
#define DDRPHY_103b4_ced0_VAL 0x1
#define DDRPHY_104b4_d2d0_VAL 0x1
#define DDRPHY_105b4_d6d0_VAL 0x1
#define DDRPHY_106b4_dad0_VAL 0x1
#define DDRPHY_107b4_ded0_VAL 0x1
#define DDRPHY_108b4_e2d0_VAL 0x1
#define DDRPHY_11011_10044_VAL 0x1
#define DDRPHY_11012_10048_VAL 0x1
#define DDRPHY_11013_1004c_VAL 0x180
#define DDRPHY_11018_10060_VAL 0x1
#define DDRPHY_11002_10008_VAL 0x6209
#define DDRPHY_110b2_102c8_VAL 0x1
#define DDRPHY_111b4_106d0_VAL 0x1
#define DDRPHY_112b4_10ad0_VAL 0x1
#define DDRPHY_113b4_10ed0_VAL 0x1
#define DDRPHY_114b4_112d0_VAL 0x1
#define DDRPHY_115b4_116d0_VAL 0x1
#define DDRPHY_116b4_11ad0_VAL 0x1
#define DDRPHY_117b4_11ed0_VAL 0x1
#define DDRPHY_118b4_122d0_VAL 0x1
#define DDRPHY_2003a_140e8_VAL 0x2
// Disabling Ucclk (PMU)
#define DDRPHY_c0080_a4200_VAL 0x2
// Isolate the APB access from the internal CSRs by setting the MicroContMuxSel CSR to 1. 
#define DDRPHY_d0000_a8000_VAL 0x1
//##############################################################
//
// 4.3.10(J) Initialize the PHY to Mission Mode through DFI Initialization 
//
// Initialize the PHY to mission mode as follows: 
//
// 1. Set the PHY input clocks to the desired frequency. 
// 2. Initialize the PHY to mission mode by performing DFI Initialization. 
//    Please see the DFI specification for more information. See the DFI frequency bus encoding in section <XXX>.
//
//##############################################################
