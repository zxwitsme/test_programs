#ifndef _MNPMUSRAMMSGBLOCK_ATE_H
#define _MNPMUSRAMMSGBLOCK_ATE_H 1

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// Message block related defines
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// Basic type typedefs

#include <stdint.h>

// TestsToRun encoding

#define REVISION_CHECK_TEST   0x0001
#define IMPEDANCE_CAL_TEST    0x0002
#define PLL_LOCK_TEST         0x0004
#define LCDL_LINEARITY_TEST   0x0008
#define AC_LOOPBACK_TEST      0x0010
#define DAT_1D_LOOPBACK_TEST  0x0020
#define DAT_2D_LOOPBACK_TEST  0x0040
#define BURN_IN_TEST          0x0080
#define LCDL_OFFSET_TEST      0x0100
#define CHARACTERIZATION_TEST 0x8000


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// Message Block definition for the ATE firmware (Version 0.71a)
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

typedef struct _PMU_SMB_ATE_t {

  /////////////////////////////////////////
  // General ATE Firmware Inputs / Outputs
  /////////////////////////////////////////

  uint16_t TestsToRun;  // Byte Offset: 0x0 / CSR Address: 0x54000
                        // TestsToRun[0]    = DMEM / IMEM revision check
                        // TestsToRun[1]    = Impedance Calibration
                        // TestsToRun[2]    = PLL Lock
                        // TestsToRun[3]    = LCDL Linearity
                        // TestsToRun[4]    = Address / Command Loopback
                        // TestsToRun[5]    = Data Loopback 1D
                        // TestsToRun[6]    = Data Loopback 2D
                        // TestsToRun[7]    = Burn-In
                        // TestsToRun[8]    = LCDL Offset
                        // TestsToRun[15:9] = Reserved (must be written to 0)
  uint16_t PassFailResults;  // Byte Offset: 0x2 / CSR Address: 0x54001
                             // Bit value of 0 = fail / 1 = pass
                             // PassFailResults[0]    = DMEM / IMEM revision check
                             // PassFailResults[1]    = Impedance Calibration
                             // PassFailResults[2]    = PLL Lock
                             // PassFailResults[3]    = LCDL Linearity
                             // PassFailResults[4]    = Address / Command Loopback
                             // PassFailResults[5]    = Data Loopback 1D
                             // PassFailResults[6]    = Data Loopback 2D 
                             // PassFailResults[7]    = Burn-In
                             // PassFailResults[8]    = LCDL Offset (Internal Synopsys debug option, should never be set during ATE testing.)
                             // PassFailResults[14:9] = Reserved (will be 0)
                             // PassFailResults[15]   = Characterization (Continuous writes)
  uint32_t AteImemRevision;  // Byte Offset: 0x4 / CSR Address: 0x54002
                             // PMU firmware revision ID of the IMEM image
                             // After ATE firmware is run, this address will contain 
                             // the revision ID of the ATE firmware's IMEM image. 
                             // Please reference this revision ID when filing support 
                             // cases with Synopsys.
  uint32_t AteDmemRevision;  // Byte Offset: 0x8 / CSR Address: 0x54004
                             // PMU firmware revision ID of the DMEM image
                             // After ATE firmware is run, this address will contain 
                             // the revision ID of the ATE firmware's DMEM image. 
                             // Please reference this revision ID when filing support 
                             // cases with Synopsys.
  uint8_t  NumDbytes;  // Byte Offset: 0xc / CSR Address: 0x54006
                       // Number of Dbytes in the PHY
  uint8_t  NumAnibs;  // Byte Offset: 0xd / CSR Address: 0x54006
                      // Number of Anibs in the PHY
  uint8_t  NumBitsPerDbyte;  // Byte Offset: 0xe / CSR Address: 0x54007
                             // Number of bits per Dbyte in the PHY
  uint8_t  X4Mode;  // Byte Offset: 0xf / CSR Address: 0x54007
                    // x4 Mode
                    //    0x0 = x8 mode
                    //    0x1 = x4 mode
  uint8_t  CalDrvStr0;  // Byte Offset: 0x10 / CSR Address: 0x54008
                        // CalDrvStr0 Value
  uint8_t  Vref;  // Byte Offset: 0x11 / CSR Address: 0x54008
                  // Value used to set CSR GlobalVrefInDac.
  uint16_t DfiClkFreq;  // Byte Offset: 0x12 / CSR Address: 0x54009
                        // DFI Clock frequency in MHz
                        // For example enter 0x0320 for 800MHz.
  uint16_t TxImpedanceDq;  // Byte Offset: 0x14 / CSR Address: 0x5400a
                           // See CSR of the same name for description of contents.
  uint16_t ATxImpedance;  // Byte Offset: 0x16 / CSR Address: 0x5400b
                          // See CSR of the same name for description of contents.


  ///////////////////////////////////////////////
  // Impedance Calibration Test Inputs
  ///////////////////////////////////////////////

  uint8_t  ContinuousCal;  // Byte Offset: 0x18 / CSR Address: 0x5400c
                           // Continuous Calibration
                           //     0x0 = Do not do continuous calibration
                           //     0x1 = Do continuous calibration
  uint8_t  CalInterval;  // Byte Offset: 0x19 / CSR Address: 0x5400c
                         // CalInterval of CalRate CSR used in continuous cal
                         //     See CalRate CSR description for more information.


  ///////////////////////////////////////////////
  // Pll Lock Test Inputs
  ///////////////////////////////////////////////

  uint16_t MemClkToggle;  // Byte Offset: 0x1a / CSR Address: 0x5400d
                          // MEMCLK toggle
                          //      0x0 = MEMCLK does not toggle
                          //      0x1 = MEMCLK toggles at 2*DFICLK
                          //      0x2 = MEMCLK toggles at DFICLK / 2
                          //      0x3 = MEMCLK toggles at DFICLK / 4
                          //      0x4 = MEMCLK toggles at DFICLK / 8
                          //      0x5 = MEMCLK toggles at DFICLK / 16
                          //      0x6 = MEMCLK toggles at DFICLK / 24
                          //            All other values reserved.
  uint16_t MemClkTime;  // Byte Offset: 0x1c / CSR Address: 0x5400e
                        // Time in microseconds to toggle MEMCLK
                        // Setting to 0xFFFF will cause
                        // the clock to run infinitely. The uC must be reset
                        // to stop if in the infinite mode.


  /////////////////////////////////////////
  // LCDL Linearity Test Inputs
  /////////////////////////////////////////

  uint16_t LcdlClksToRun;  // Byte Offset: 0x1e / CSR Address: 0x5400f
                           // Clock cycles to run for each measurement
  uint16_t LcdlStartPhase;  // Byte Offset: 0x20 / CSR Address: 0x54010
                            // Start Phase (0-511 decimal)
  uint16_t LcdlEndPhase;  // Byte Offset: 0x22 / CSR Address: 0x54011
                          // End Phase (0-511 decimal)
  uint8_t  LcdlStride;  // Byte Offset: 0x24 / CSR Address: 0x54012
                        // Stride 1 to 100, 11 recommended
                        //      0x00 = Use recommended value of 11
                        // 0x01-0x64 = Use this value for stride
                        //             All other values reserved
  uint8_t  LcdlPassPercent;  // Byte Offset: 0x25 / CSR Address: 0x54012
                             // Percentage of spec values to consider pass / fail
                             //       100 (decimal) = 100% = matches spec
                             // Numbers smaller than 100 tighten the tolerance
                             // Numbers larger than 100 loosen the tolerance

  //LCDL Linearity Plot - LCDL selection inputs

  uint16_t  Lcdl0_ObserveCfg;  // Byte Offset: 0x26 / CSR Address: 0x54013
  uint16_t  Lcdl1_ObserveCfg;  // Byte Offset: 0x28 / CSR Address: 0x54014
  uint16_t  Lcdl2_ObserveCfg;  // Byte Offset: 0x2a / CSR Address: 0x54015
  uint16_t  Lcdl3_ObserveCfg;  // Byte Offset: 0x2c / CSR Address: 0x54016

  uint16_t  Lcdl_rsvd_0;  // Byte Offset: 0x2e / CSR Address: 0x54017
  uint16_t  Lcdl_rsvd_1;  // Byte Offset: 0x30 / CSR Address: 0x54018
  uint16_t  Lcdl_rsvd_2;  // Byte Offset: 0x32 / CSR Address: 0x54019
  uint16_t  Lcdl_rsvd_3;  // Byte Offset: 0x34 / CSR Address: 0x5401a


  /////////////////////////////////////////
  // AC Loopback Test Inputs
  /////////////////////////////////////////

  uint16_t AcLoopClksToRun;  // Byte Offset: 0x36 / CSR Address: 0x5401b
                             // Clock cycles to run AC loopback
  uint8_t  AcLoopDataRate;  // Byte Offset: 0x38 / CSR Address: 0x5401c
                            // Data Rate 
                            //    0x0 = Single data rate pattern
                            //    0x1 = Dual data rate pattern

  uint8_t  AcLoopCoreLoopBk;  // Byte Offset: 0x39 / CSR Address: 0x5401c
                              // Pad / Core loopback
                              //    0x0 = Pad loopback
                              //    0x1 = Core loopback
  uint8_t  AcMinEyeWidth;  // Byte Offset: 0x3a / CSR Address: 0x5401d
                           // Required Eye width in the units of 1/32 UI
  //See MinPadLpbkPwr below


  /////////////////////////////////////////
  // Data Loopback Test Inputs
  /////////////////////////////////////////

  uint8_t  DatLoopPatLen;  // Byte Offset: 0x3b / CSR Address: 0x5401d
                           // Pattern length
  uint16_t DatLoopCoreLoopBk;  // Byte Offset: 0x3c / CSR Address: 0x5401e
                               // Pad / Core loopback
                               //    0x0 = Pad loopback
                               //    0x1 = Core loopback
  uint16_t MinPadLpbkPwr;   // Byte Offset: 0x3e / CSR Address: 0x5401f
                            //  Used by Data Pad Loopback to serialize testing of the dbytes;
                            //  that is, it tests one dbyte at a time and configures unused dbytes for lower power
                            //  Also used by AC Pad Loopback to serialize testing of the anibs;
                            //  that is, it tests one anib at a time and configures unused anibs for lower power
                            //  Normal use would require DatLoopCoreLoopBk to be 0.
  uint32_t DataLoopPrbsSeed;  // Byte Offset: 0x40 / CSR Address: 0x54020
                              // PRBS seed 
  uint16_t TxSlewRate;     // Byte Offset: 0x44 / CSR Address: 0x54022
  uint16_t DqDqsRcvCntrl;  // Byte Offset: 0x46 / CSR Address: 0x54023
  uint16_t TxDqsDly;       // Byte Offset: 0x48 / CSR Address: 0x54024
  uint16_t RxEnDly;        // Byte Offset: 0x4a / CSR Address: 0x54025
  uint16_t RxPBDly;        // Byte Offset: 0x4c / CSR Address: 0x54026
  uint16_t VrefDAC0;       // Byte Offset: 0x4e / CSR Address: 0x54027
  uint8_t  DatLoop1dminEyeWidth;  // Byte Offset: 0x50 / CSR Address: 0x54028
                                 // Minimum Eye width for 1D  
  uint8_t  DatLoop2dByte;  // Byte Offset: 0x51 / CSR Address: 0x54028
                           // Byte to test for 2D loopback
  uint8_t  DatLoop2dBit;  // Byte Offset: 0x52 / CSR Address: 0x54029
                          // Bit to test for 2D loopback
  uint8_t  DatLoop2dVrefIncr;  // Byte Offset: 0x53 / CSR Address: 0x54029
                               // Vref Increment size (1-63) for 2D loopback


  /////////////////////////////////////////
  // LCDL Offset Test Inputs
  /////////////////////////////////////////

  uint16_t LcdlOffsetPercentAllowed;  // Byte Offset: 0x54 / CSR Address: 0x5402a


  /////////////////////////////////////////
  // LCDL Offset Test Results
  /////////////////////////////////////////

  uint16_t LcldlOffsetResultAverage;  // Byte Offset: 0x56 / CSR Address: 0x5402b
                                      // Average LCDL 1UI code value
  uint16_t LcdlOffsetResultsFailuresAcx4;  // Byte Offset: 0x58 / CSR Address: 0x5402c
                                           // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD0;  // Byte Offset: 0x5a / CSR Address: 0x5402d
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD1;  // Byte Offset: 0x5c / CSR Address: 0x5402e
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD2;  // Byte Offset: 0x5e / CSR Address: 0x5402f
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD3;  // Byte Offset: 0x60 / CSR Address: 0x54030
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD4;  // Byte Offset: 0x62 / CSR Address: 0x54031
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD5;  // Byte Offset: 0x64 / CSR Address: 0x54032
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD6;  // Byte Offset: 0x66 / CSR Address: 0x54033
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD7;  // Byte Offset: 0x68 / CSR Address: 0x54034
                                         // A bit set to 1 indicates that LCDL failed range
  uint16_t LcdlOffsetResultsFailuresD8;  // Byte Offset: 0x6a / CSR Address: 0x54035
                                         // A bit set to 1 indicates that LCDL failed range


  /////////////////////////////////////////
  // Impedance Calibration Test Results
  /////////////////////////////////////////

  uint16_t TxCalBinP;  // Byte Offset: 0x6c / CSR Address: 0x54036
                       // Output from Impedance Calibration
                       // See CSR of the same name for description of contents.
  uint16_t TxCalBinN;  // Byte Offset: 0x6e / CSR Address: 0x54037
                       // Output from Impedance Calibration
                       // See CSR of the same name for description of contents.
  uint16_t CalPExt;  // Byte Offset: 0x70 / CSR Address: 0x54038
                     // Output from Impedance Calibration
                     // See CSR of the same name for description of contents.
  uint16_t CalNInt;  // Byte Offset: 0x72 / CSR Address: 0x54039
                     // Output from Impedance Calibration
                     // See CSR of the same name for description of contents.
  uint16_t CalCmpr5;  // Byte Offset: 0x74 / CSR Address: 0x5403a
                      // Output from Impedance Calibration
                      // See CSR of the same name for description of contents.


  /////////////////////////////////////////
  // LCDL Linearity Test Results
  /////////////////////////////////////////

  uint16_t LcdlErrCnt_TxLn0_D0;  // Byte Offset: 0x76 / CSR Address: 0x5403b
                                 // DBYTE0 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D1;  // Byte Offset: 0x78 / CSR Address: 0x5403c
                                 // DBYTE1 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D2;  // Byte Offset: 0x7a / CSR Address: 0x5403d
                                 // DBYTE2 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D3;  // Byte Offset: 0x7c / CSR Address: 0x5403e
                                 // DBYTE3 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D4;  // Byte Offset: 0x7e / CSR Address: 0x5403f
                                 // DBYTE4 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D5;  // Byte Offset: 0x80 / CSR Address: 0x54040
                                 // DBYTE5 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D6;  // Byte Offset: 0x82 / CSR Address: 0x54041
                                 // DBYTE6 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D7;  // Byte Offset: 0x84 / CSR Address: 0x54042
                                 // DBYTE7 Transmit Lane 0 LCDL error count
  uint16_t LcdlErrCnt_TxLn0_D8;  // Byte Offset: 0x86 / CSR Address: 0x54043
                                 // DBYTE8 Transmit Lane 0 LCDL error count
 
  uint16_t LcdlErrCnt_TxLn1_D0;  // Byte Offset: 0x88 / CSR Address: 0x54044
                                 // DBYTE0 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D1;  // Byte Offset: 0x8a / CSR Address: 0x54045
                                 // DBYTE1 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D2;  // Byte Offset: 0x8c / CSR Address: 0x54046
                                 // DBYTE2 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D3;  // Byte Offset: 0x8e / CSR Address: 0x54047
                                 // DBYTE3 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D4;  // Byte Offset: 0x90 / CSR Address: 0x54048
                                 // DBYTE4 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D5;  // Byte Offset: 0x92 / CSR Address: 0x54049
                                 // DBYTE5 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D6;  // Byte Offset: 0x94 / CSR Address: 0x5404a
                                 // DBYTE6 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D7;  // Byte Offset: 0x96 / CSR Address: 0x5404b
                                 // DBYTE7 Transmit Lane 1 LCDL error count
  uint16_t LcdlErrCnt_TxLn1_D8;  // Byte Offset: 0x98 / CSR Address: 0x5404c
                                 // DBYTE8 Transmit Lane 1 LCDL error count

  uint16_t LcdlErrCnt_TxLn2_D0;  // Byte Offset: 0x9a / CSR Address: 0x5404d
                                 // DBYTE0 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D1;  // Byte Offset: 0x9c / CSR Address: 0x5404e
                                 // DBYTE1 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D2;  // Byte Offset: 0x9e / CSR Address: 0x5404f
                                 // DBYTE2 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D3;  // Byte Offset: 0xa0 / CSR Address: 0x54050
                                 // DBYTE3 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D4;  // Byte Offset: 0xa2 / CSR Address: 0x54051
                                 // DBYTE4 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D5;  // Byte Offset: 0xa4 / CSR Address: 0x54052
                                 // DBYTE5 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D6;  // Byte Offset: 0xa6 / CSR Address: 0x54053
                                 // DBYTE6 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D7;  // Byte Offset: 0xa8 / CSR Address: 0x54054
                                 // DBYTE7 Transmit Lane 2 LCDL error count
  uint16_t LcdlErrCnt_TxLn2_D8;  // Byte Offset: 0xaa / CSR Address: 0x54055
                                 // DBYTE8 Transmit Lane 2 LCDL error count

  uint16_t LcdlErrCnt_TxLn3_D0;  // Byte Offset: 0xac / CSR Address: 0x54056
                                 // DBYTE0 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D1;  // Byte Offset: 0xae / CSR Address: 0x54057
                                 // DBYTE1 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D2;  // Byte Offset: 0xb0 / CSR Address: 0x54058
                                 // DBYTE2 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D3;  // Byte Offset: 0xb2 / CSR Address: 0x54059
                                 // DBYTE3 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D4;  // Byte Offset: 0xb4 / CSR Address: 0x5405a
                                 // DBYTE4 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D5;  // Byte Offset: 0xb6 / CSR Address: 0x5405b
                                 // DBYTE5 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D6;  // Byte Offset: 0xb8 / CSR Address: 0x5405c
                                 // DBYTE6 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D7;  // Byte Offset: 0xba / CSR Address: 0x5405d
                                 // DBYTE7 Transmit Lane 3 LCDL error count
  uint16_t LcdlErrCnt_TxLn3_D8;  // Byte Offset: 0xbc / CSR Address: 0x5405e
                                 // DBYTE8 Transmit Lane 3 LCDL error count

  uint16_t LcdlErrCnt_TxLn4_D0;  // Byte Offset: 0xbe / CSR Address: 0x5405f
                                 // DBYTE0 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D1;  // Byte Offset: 0xc0 / CSR Address: 0x54060
                                 // DBYTE1 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D2;  // Byte Offset: 0xc2 / CSR Address: 0x54061
                                 // DBYTE2 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D3;  // Byte Offset: 0xc4 / CSR Address: 0x54062
                                 // DBYTE3 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D4;  // Byte Offset: 0xc6 / CSR Address: 0x54063
                                 // DBYTE4 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D5;  // Byte Offset: 0xc8 / CSR Address: 0x54064
                                 // DBYTE5 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D6;  // Byte Offset: 0xca / CSR Address: 0x54065
                                 // DBYTE6 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D7;  // Byte Offset: 0xcc / CSR Address: 0x54066
                                 // DBYTE7 Transmit Lane 4 LCDL error count
  uint16_t LcdlErrCnt_TxLn4_D8;  // Byte Offset: 0xce / CSR Address: 0x54067
                                 // DBYTE8 Transmit Lane 4 LCDL error count

  uint16_t LcdlErrCnt_TxLn5_D0;  // Byte Offset: 0xd0 / CSR Address: 0x54068
                                 // DBYTE0 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D1;  // Byte Offset: 0xd2 / CSR Address: 0x54069
                                 // DBYTE1 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D2;  // Byte Offset: 0xd4 / CSR Address: 0x5406a
                                 // DBYTE2 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D3;  // Byte Offset: 0xd6 / CSR Address: 0x5406b
                                 // DBYTE3 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D4;  // Byte Offset: 0xd8 / CSR Address: 0x5406c
                                 // DBYTE4 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D5;  // Byte Offset: 0xda / CSR Address: 0x5406d
                                 // DBYTE5 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D6;  // Byte Offset: 0xdc / CSR Address: 0x5406e
                                 // DBYTE6 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D7;  // Byte Offset: 0xde / CSR Address: 0x5406f
                                 // DBYTE7 Transmit Lane 5 LCDL error count
  uint16_t LcdlErrCnt_TxLn5_D8;  // Byte Offset: 0xe0 / CSR Address: 0x54070
                                 // DBYTE8 Transmit Lane 5 LCDL error count

  uint16_t LcdlErrCnt_TxLn6_D0;  // Byte Offset: 0xe2 / CSR Address: 0x54071
                                 // DBYTE0 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D1;  // Byte Offset: 0xe4 / CSR Address: 0x54072
                                 // DBYTE1 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D2;  // Byte Offset: 0xe6 / CSR Address: 0x54073
                                 // DBYTE2 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D3;  // Byte Offset: 0xe8 / CSR Address: 0x54074
                                 // DBYTE3 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D4;  // Byte Offset: 0xea / CSR Address: 0x54075
                                 // DBYTE4 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D5;  // Byte Offset: 0xec / CSR Address: 0x54076
                                 // DBYTE5 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D6;  // Byte Offset: 0xee / CSR Address: 0x54077
                                 // DBYTE6 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D7;  // Byte Offset: 0xf0 / CSR Address: 0x54078
                                 // DBYTE7 Transmit Lane 6 LCDL error count
  uint16_t LcdlErrCnt_TxLn6_D8;  // Byte Offset: 0xf2 / CSR Address: 0x54079
                                 // DBYTE8 Transmit Lane 6 LCDL error count

  uint16_t LcdlErrCnt_TxLn7_D0;  // Byte Offset: 0xf4 / CSR Address: 0x5407a
                                 // DBYTE0 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D1;  // Byte Offset: 0xf6 / CSR Address: 0x5407b
                                 // DBYTE1 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D2;  // Byte Offset: 0xf8 / CSR Address: 0x5407c
                                 // DBYTE2 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D3;  // Byte Offset: 0xfa / CSR Address: 0x5407d
                                 // DBYTE3 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D4;  // Byte Offset: 0xfc / CSR Address: 0x5407e
                                 // DBYTE4 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D5;  // Byte Offset: 0xfe / CSR Address: 0x5407f
                                 // DBYTE5 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D6;  // Byte Offset: 0x100 / CSR Address: 0x54080
                                 // DBYTE6 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D7;  // Byte Offset: 0x102 / CSR Address: 0x54081
                                 // DBYTE7 Transmit Lane 7 LCDL error count
  uint16_t LcdlErrCnt_TxLn7_D8;  // Byte Offset: 0x104 / CSR Address: 0x54082
                                 // DBYTE8 Transmit Lane 7 LCDL error count

  uint16_t LcdlErrCnt_TxLn8_D0;  // Byte Offset: 0x106 / CSR Address: 0x54083
                                 // DBYTE0 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D1;  // Byte Offset: 0x108 / CSR Address: 0x54084
                                 // DBYTE1 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D2;  // Byte Offset: 0x10a / CSR Address: 0x54085
                                 // DBYTE2 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D3;  // Byte Offset: 0x10c / CSR Address: 0x54086
                                 // DBYTE3 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D4;  // Byte Offset: 0x10e / CSR Address: 0x54087
                                 // DBYTE4 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D5;  // Byte Offset: 0x110 / CSR Address: 0x54088
                                 // DBYTE5 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D6;  // Byte Offset: 0x112 / CSR Address: 0x54089
                                 // DBYTE6 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D7;  // Byte Offset: 0x114 / CSR Address: 0x5408a
                                 // DBYTE7 Transmit Lane 8 LCDL error count
  uint16_t LcdlErrCnt_TxLn8_D8;  // Byte Offset: 0x116 / CSR Address: 0x5408b
                                 // DBYTE8 Transmit Lane 8 LCDL error count

  uint16_t LcdlErrCnt_TxLn9_D0;  // Byte Offset: 0x118 / CSR Address: 0x5408c
                                 // DBYTE0 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D1;  // Byte Offset: 0x11a / CSR Address: 0x5408d
                                 // DBYTE1 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D2;  // Byte Offset: 0x11c / CSR Address: 0x5408e
                                 // DBYTE2 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D3;  // Byte Offset: 0x11e / CSR Address: 0x5408f
                                 // DBYTE3 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D4;  // Byte Offset: 0x120 / CSR Address: 0x54090
                                 // DBYTE4 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D5;  // Byte Offset: 0x122 / CSR Address: 0x54091
                                 // DBYTE5 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D6;  // Byte Offset: 0x124 / CSR Address: 0x54092
                                 // DBYTE6 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D7;  // Byte Offset: 0x126 / CSR Address: 0x54093
                                 // DBYTE7 Transmit Lane 9 LCDL error count
  uint16_t LcdlErrCnt_TxLn9_D8;  // Byte Offset: 0x128 / CSR Address: 0x54094
                                 // DBYTE8 Transmit Lane 9 LCDL error count

  uint16_t LcdlErrCnt_RxEn0_D0;  // Byte Offset: 0x12a / CSR Address: 0x54095
                                 // DBYTE0 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D1;  // Byte Offset: 0x12c / CSR Address: 0x54096
                                 // DBYTE1 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D2;  // Byte Offset: 0x12e / CSR Address: 0x54097
                                 // DBYTE2 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D3;  // Byte Offset: 0x130 / CSR Address: 0x54098
                                 // DBYTE3 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D4;  // Byte Offset: 0x132 / CSR Address: 0x54099
                                 // DBYTE4 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D5;  // Byte Offset: 0x134 / CSR Address: 0x5409a
                                 // DBYTE5 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D6;  // Byte Offset: 0x136 / CSR Address: 0x5409b
                                 // DBYTE6 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D7;  // Byte Offset: 0x138 / CSR Address: 0x5409c
                                 // DBYTE7 Receive Enable 0 LCDL error count 
  uint16_t LcdlErrCnt_RxEn0_D8;  // Byte Offset: 0x13a / CSR Address: 0x5409d
                                 // DBYTE8 Receive Enable 0 LCDL error count 

  uint16_t LcdlErrCnt_RxEn1_D0;  // Byte Offset: 0x13c / CSR Address: 0x5409e
                                 // DBYTE0 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D1;  // Byte Offset: 0x13e / CSR Address: 0x5409f
                                 // DBYTE1 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D2;  // Byte Offset: 0x140 / CSR Address: 0x540a0
                                 // DBYTE2 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D3;  // Byte Offset: 0x142 / CSR Address: 0x540a1
                                 // DBYTE3 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D4;  // Byte Offset: 0x144 / CSR Address: 0x540a2
                                 // DBYTE4 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D5;  // Byte Offset: 0x146 / CSR Address: 0x540a3
                                 // DBYTE5 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D6;  // Byte Offset: 0x148 / CSR Address: 0x540a4
                                 // DBYTE6 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D7;  // Byte Offset: 0x14a / CSR Address: 0x540a5
                                 // DBYTE7 Receive Enable 1 LCDL error count 
  uint16_t LcdlErrCnt_RxEn1_D8;  // Byte Offset: 0x14c / CSR Address: 0x540a6
                                 // DBYTE8 Receive Enable 1 LCDL error count 

  uint16_t LcdlErrCnt_RxClk0C_D0;  // Byte Offset: 0x14e / CSR Address: 0x540a7
                                   // DBYTE0 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D1;  // Byte Offset: 0x150 / CSR Address: 0x540a8
                                   // DBYTE1 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D2;  // Byte Offset: 0x152 / CSR Address: 0x540a9
                                   // DBYTE2 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D3;  // Byte Offset: 0x154 / CSR Address: 0x540aa
                                   // DBYTE3 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D4;  // Byte Offset: 0x156 / CSR Address: 0x540ab
                                   // DBYTE4 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D5;  // Byte Offset: 0x158 / CSR Address: 0x540ac
                                   // DBYTE5 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D6;  // Byte Offset: 0x15a / CSR Address: 0x540ad
                                   // DBYTE6 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D7;  // Byte Offset: 0x15c / CSR Address: 0x540ae
                                   // DBYTE7 Receive Clock 0C LCDL error count 
  uint16_t LcdlErrCnt_RxClk0C_D8;  // Byte Offset: 0x15e / CSR Address: 0x540af
                                   // DBYTE8 Receive Clock 0C LCDL error count 

  uint16_t LcdlErrCnt_RxClk1C_D0;  // Byte Offset: 0x160 / CSR Address: 0x540b0
                                   // DBYTE0 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D1;  // Byte Offset: 0x162 / CSR Address: 0x540b1
                                   // DBYTE1 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D2;  // Byte Offset: 0x164 / CSR Address: 0x540b2
                                   // DBYTE2 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D3;  // Byte Offset: 0x166 / CSR Address: 0x540b3
                                   // DBYTE3 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D4;  // Byte Offset: 0x168 / CSR Address: 0x540b4
                                   // DBYTE4 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D5;  // Byte Offset: 0x16a / CSR Address: 0x540b5
                                   // DBYTE5 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D6;  // Byte Offset: 0x16c / CSR Address: 0x540b6
                                   // DBYTE6 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D7;  // Byte Offset: 0x16e / CSR Address: 0x540b7
                                   // DBYTE7 Receive Clock 1C LCDL error count 
  uint16_t LcdlErrCnt_RxClk1C_D8;  // Byte Offset: 0x170 / CSR Address: 0x540b8
                                   // DBYTE8 Receive Clock 1C LCDL error count 

  uint16_t LcdlErrCnt_RxClk0T_D0;  // Byte Offset: 0x172 / CSR Address: 0x540b9
                                   // DBYTE0 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D1;  // Byte Offset: 0x174 / CSR Address: 0x540ba
                                   // DBYTE1 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D2;  // Byte Offset: 0x176 / CSR Address: 0x540bb
                                   // DBYTE2 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D3;  // Byte Offset: 0x178 / CSR Address: 0x540bc
                                   // DBYTE3 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D4;  // Byte Offset: 0x17a / CSR Address: 0x540bd
                                   // DBYTE4 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D5;  // Byte Offset: 0x17c / CSR Address: 0x540be
                                   // DBYTE5 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D6;  // Byte Offset: 0x17e / CSR Address: 0x540bf
                                   // DBYTE6 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D7;  // Byte Offset: 0x180 / CSR Address: 0x540c0
                                   // DBYTE7 Receive Clock 0T LCDL error count 
  uint16_t LcdlErrCnt_RxClk0T_D8;  // Byte Offset: 0x182 / CSR Address: 0x540c1
                                   // DBYTE8 Receive Clock 0T LCDL error count 

  uint16_t LcdlErrCnt_RxClk1T_D0;  // Byte Offset: 0x184 / CSR Address: 0x540c2
                                   // DBYTE0 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D1;  // Byte Offset: 0x186 / CSR Address: 0x540c3
                                   // DBYTE1 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D2;  // Byte Offset: 0x188 / CSR Address: 0x540c4
                                   // DBYTE2 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D3;  // Byte Offset: 0x18a / CSR Address: 0x540c5
                                   // DBYTE3 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D4;  // Byte Offset: 0x18c / CSR Address: 0x540c6
                                   // DBYTE4 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D5;  // Byte Offset: 0x18e / CSR Address: 0x540c7
                                   // DBYTE5 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D6;  // Byte Offset: 0x190 / CSR Address: 0x540c8
                                   // DBYTE6 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D7;  // Byte Offset: 0x192 / CSR Address: 0x540c9
                                   // DBYTE7 Receive Clock 1T LCDL error count 
  uint16_t LcdlErrCnt_RxClk1T_D8;  // Byte Offset: 0x194 / CSR Address: 0x540ca
                                   // DBYTE8 Receive Clock 1T LCDL error count

  uint16_t BdlErrCnt_RxLn0_D0;  // Byte Offset: 0x196 / CSR Address: 0x540cb
                                // DBYTE0 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D1;  // Byte Offset: 0x198 / CSR Address: 0x540cc
                                // DBYTE1 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D2;  // Byte Offset: 0x19a / CSR Address: 0x540cd
                                // DBYTE2 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D3;  // Byte Offset: 0x19c / CSR Address: 0x540ce
                                // DBYTE3 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D4;  // Byte Offset: 0x19e / CSR Address: 0x540cf
                                // DBYTE4 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D5;  // Byte Offset: 0x1a0 / CSR Address: 0x540d0
                                // DBYTE5 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D6;  // Byte Offset: 0x1a2 / CSR Address: 0x540d1
                                // DBYTE6 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D7;  // Byte Offset: 0x1a4 / CSR Address: 0x540d2
                                // DBYTE7 Receive Lane 0 BDL error count
  uint16_t BdlErrCnt_RxLn0_D8;  // Byte Offset: 0x1a6 / CSR Address: 0x540d3
                                // DBYTE8 Receive Lane 0 BDL error count
 
  uint16_t BdlErrCnt_RxLn1_D0;  // Byte Offset: 0x1a8 / CSR Address: 0x540d4
                                // DBYTE0 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D1;  // Byte Offset: 0x1aa / CSR Address: 0x540d5
                                // DBYTE1 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D2;  // Byte Offset: 0x1ac / CSR Address: 0x540d6
                                // DBYTE2 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D3;  // Byte Offset: 0x1ae / CSR Address: 0x540d7
                                // DBYTE3 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D4;  // Byte Offset: 0x1b0 / CSR Address: 0x540d8
                                // DBYTE4 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D5;  // Byte Offset: 0x1b2 / CSR Address: 0x540d9
                                // DBYTE5 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D6;  // Byte Offset: 0x1b4 / CSR Address: 0x540da
                                // DBYTE6 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D7;  // Byte Offset: 0x1b6 / CSR Address: 0x540db
                                // DBYTE7 Receive Lane 1 BDL error count
  uint16_t BdlErrCnt_RxLn1_D8;  // Byte Offset: 0x1b8 / CSR Address: 0x540dc
                                // DBYTE8 Receive Lane 1 BDL error count

  uint16_t BdlErrCnt_RxLn2_D0;  // Byte Offset: 0x1ba / CSR Address: 0x540dd
                                // DBYTE0 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D1;  // Byte Offset: 0x1bc / CSR Address: 0x540de
                                // DBYTE1 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D2;  // Byte Offset: 0x1be / CSR Address: 0x540df
                                // DBYTE2 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D3;  // Byte Offset: 0x1c0 / CSR Address: 0x540e0
                                // DBYTE3 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D4;  // Byte Offset: 0x1c2 / CSR Address: 0x540e1
                                // DBYTE4 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D5;  // Byte Offset: 0x1c4 / CSR Address: 0x540e2
                                // DBYTE5 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D6;  // Byte Offset: 0x1c6 / CSR Address: 0x540e3
                                // DBYTE6 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D7;  // Byte Offset: 0x1c8 / CSR Address: 0x540e4
                                // DBYTE7 Receive Lane 2 BDL error count
  uint16_t BdlErrCnt_RxLn2_D8;  // Byte Offset: 0x1ca / CSR Address: 0x540e5
                                // DBYTE8 Receive Lane 2 BDL error count

  uint16_t BdlErrCnt_RxLn3_D0;  // Byte Offset: 0x1cc / CSR Address: 0x540e6
                                // DBYTE0 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D1;  // Byte Offset: 0x1ce / CSR Address: 0x540e7
                                // DBYTE1 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D2;  // Byte Offset: 0x1d0 / CSR Address: 0x540e8
                                // DBYTE2 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D3;  // Byte Offset: 0x1d2 / CSR Address: 0x540e9
                                // DBYTE3 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D4;  // Byte Offset: 0x1d4 / CSR Address: 0x540ea
                                // DBYTE4 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D5;  // Byte Offset: 0x1d6 / CSR Address: 0x540eb
                                // DBYTE5 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D6;  // Byte Offset: 0x1d8 / CSR Address: 0x540ec
                                // DBYTE6 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D7;  // Byte Offset: 0x1da / CSR Address: 0x540ed
                                // DBYTE7 Receive Lane 3 BDL error count
  uint16_t BdlErrCnt_RxLn3_D8;  // Byte Offset: 0x1dc / CSR Address: 0x540ee
                                // DBYTE8 Receive Lane 3 BDL error count

  uint16_t BdlErrCnt_RxLn4_D0;  // Byte Offset: 0x1de / CSR Address: 0x540ef
                                // DBYTE0 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D1;  // Byte Offset: 0x1e0 / CSR Address: 0x540f0
                                // DBYTE1 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D2;  // Byte Offset: 0x1e2 / CSR Address: 0x540f1
                                // DBYTE2 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D3;  // Byte Offset: 0x1e4 / CSR Address: 0x540f2
                                // DBYTE3 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D4;  // Byte Offset: 0x1e6 / CSR Address: 0x540f3
                                // DBYTE4 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D5;  // Byte Offset: 0x1e8 / CSR Address: 0x540f4
                                // DBYTE5 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D6;  // Byte Offset: 0x1ea / CSR Address: 0x540f5
                                // DBYTE6 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D7;  // Byte Offset: 0x1ec / CSR Address: 0x540f6
                                // DBYTE7 Receive Lane 4 BDL error count
  uint16_t BdlErrCnt_RxLn4_D8;  // Byte Offset: 0x1ee / CSR Address: 0x540f7
                                // DBYTE8 Receive Lane 4 BDL error count

  uint16_t BdlErrCnt_RxLn5_D0;  // Byte Offset: 0x1f0 / CSR Address: 0x540f8
                                // DBYTE0 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D1;  // Byte Offset: 0x1f2 / CSR Address: 0x540f9
                                // DBYTE1 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D2;  // Byte Offset: 0x1f4 / CSR Address: 0x540fa
                                // DBYTE2 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D3;  // Byte Offset: 0x1f6 / CSR Address: 0x540fb
                                // DBYTE3 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D4;  // Byte Offset: 0x1f8 / CSR Address: 0x540fc
                                // DBYTE4 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D5;  // Byte Offset: 0x1fa / CSR Address: 0x540fd
                                // DBYTE5 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D6;  // Byte Offset: 0x1fc / CSR Address: 0x540fe
                                // DBYTE6 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D7;  // Byte Offset: 0x1fe / CSR Address: 0x540ff
                                // DBYTE7 Receive Lane 5 BDL error count
  uint16_t BdlErrCnt_RxLn5_D8;  // Byte Offset: 0x200 / CSR Address: 0x54100
                                // DBYTE8 Receive Lane 5 BDL error count

  uint16_t BdlErrCnt_RxLn6_D0;  // Byte Offset: 0x202 / CSR Address: 0x54101
                                // DBYTE0 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D1;  // Byte Offset: 0x204 / CSR Address: 0x54102
                                // DBYTE1 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D2;  // Byte Offset: 0x206 / CSR Address: 0x54103
                                // DBYTE2 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D3;  // Byte Offset: 0x208 / CSR Address: 0x54104
                                // DBYTE3 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D4;  // Byte Offset: 0x20a / CSR Address: 0x54105
                                // DBYTE4 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D5;  // Byte Offset: 0x20c / CSR Address: 0x54106
                                // DBYTE5 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D6;  // Byte Offset: 0x20e / CSR Address: 0x54107
                                // DBYTE6 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D7;  // Byte Offset: 0x210 / CSR Address: 0x54108
                                // DBYTE7 Receive Lane 6 BDL error count
  uint16_t BdlErrCnt_RxLn6_D8;  // Byte Offset: 0x212 / CSR Address: 0x54109
                                // DBYTE8 Receive Lane 6 BDL error count

  uint16_t BdlErrCnt_RxLn7_D0;  // Byte Offset: 0x214 / CSR Address: 0x5410a
                                // DBYTE0 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D1;  // Byte Offset: 0x216 / CSR Address: 0x5410b
                                // DBYTE1 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D2;  // Byte Offset: 0x218 / CSR Address: 0x5410c
                                // DBYTE2 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D3;  // Byte Offset: 0x21a / CSR Address: 0x5410d
                                // DBYTE3 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D4;  // Byte Offset: 0x21c / CSR Address: 0x5410e
                                // DBYTE4 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D5;  // Byte Offset: 0x21e / CSR Address: 0x5410f
                                // DBYTE5 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D6;  // Byte Offset: 0x220 / CSR Address: 0x54110
                                // DBYTE6 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D7;  // Byte Offset: 0x222 / CSR Address: 0x54111
                                // DBYTE7 Receive Lane 7 BDL error count
  uint16_t BdlErrCnt_RxLn7_D8;  // Byte Offset: 0x224 / CSR Address: 0x54112
                                // DBYTE8 Receive Lane 7 BDL error count

  uint16_t BdlErrCnt_RxLn8_D0;  // Byte Offset: 0x226 / CSR Address: 0x54113
                                // DBYTE0 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D1;  // Byte Offset: 0x228 / CSR Address: 0x54114
                                // DBYTE1 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D2;  // Byte Offset: 0x22a / CSR Address: 0x54115
                                // DBYTE2 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D3;  // Byte Offset: 0x22c / CSR Address: 0x54116
                                // DBYTE3 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D4;  // Byte Offset: 0x22e / CSR Address: 0x54117
                                // DBYTE4 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D5;  // Byte Offset: 0x230 / CSR Address: 0x54118
                                // DBYTE5 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D6;  // Byte Offset: 0x232 / CSR Address: 0x54119
                                // DBYTE6 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D7;  // Byte Offset: 0x234 / CSR Address: 0x5411a
                                // DBYTE7 Receive Lane 8 BDL error count
  uint16_t BdlErrCnt_RxLn8_D8;  // Byte Offset: 0x236 / CSR Address: 0x5411b
                                // DBYTE8 Receive Lane 8 BDL error count

 

  uint16_t LcdlErrCnt_ACX4_0;  // Byte Offset: 0x238 / CSR Address: 0x5411c
                               // LCDL error count for ACX4 0
  uint16_t LcdlErrCnt_ACX4_1;  // Byte Offset: 0x23a / CSR Address: 0x5411d
                               // LCDL error count for ACX4 1
  uint16_t LcdlErrCnt_ACX4_2;  // Byte Offset: 0x23c / CSR Address: 0x5411e
                               // LCDL error count for ACX4 2
  uint16_t LcdlErrCnt_ACX4_3;  // Byte Offset: 0x23e / CSR Address: 0x5411f
                               // LCDL error count for ACX4 3
  uint16_t LcdlErrCnt_ACX4_4;  // Byte Offset: 0x240 / CSR Address: 0x54120
                               // LCDL error count for ACX4 4
  uint16_t LcdlErrCnt_ACX4_5;  // Byte Offset: 0x242 / CSR Address: 0x54121
                               // LCDL error count for ACX4 5
  uint16_t LcdlErrCnt_ACX4_6;  // Byte Offset: 0x244 / CSR Address: 0x54122
                               // LCDL error count for ACX4 6
  uint16_t LcdlErrCnt_ACX4_7;  // Byte Offset: 0x246 / CSR Address: 0x54123
                               // LCDL error count for ACX4 7
  uint16_t LcdlErrCnt_ACX4_8;  // Byte Offset: 0x248 / CSR Address: 0x54124
                               // LCDL error count for ACX4 8
  uint16_t LcdlErrCnt_ACX4_9;  // Byte Offset: 0x24a / CSR Address: 0x54125
                               // LCDL error count for ACX4 9
  uint16_t LcdlErrCnt_ACX4_10;  // Byte Offset: 0x24c / CSR Address: 0x54126
                               // LCDL error count for ACX4 10
  uint16_t LcdlErrCnt_ACX4_11;  // Byte Offset: 0x24e / CSR Address: 0x54127
                               // LCDL error count for ACX4 11


  // LCDL Linearity Plotting  Ring Oscillator Edge Count Values
  uint16_t Lcdl_count_values_0[512];  // Byte Offset: 0x250 / CSR Address: 0x54128
  uint16_t Lcdl_count_values_1[512];  // Byte Offset: 0x650 / CSR Address: 0x54328
  uint16_t Lcdl_count_values_2[512];  // Byte Offset: 0xa50 / CSR Address: 0x54528
  uint16_t Lcdl_count_values_3[512];  // Byte Offset: 0xe50 / CSR Address: 0x54728
                           


  /////////////////////////////////////////
  // AC Loopback Test Results
  /////////////////////////////////////////

  uint32_t UI0ErrCnt_AcLane_0;  // Byte Offset: 0x1250 / CSR Address: 0x54928
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_1;  // Byte Offset: 0x1254 / CSR Address: 0x5492a
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_2;  // Byte Offset: 0x1258 / CSR Address: 0x5492c
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_3;  // Byte Offset: 0x125c / CSR Address: 0x5492e
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_4;  // Byte Offset: 0x1260 / CSR Address: 0x54930
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_5;  // Byte Offset: 0x1264 / CSR Address: 0x54932
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_6;  // Byte Offset: 0x1268 / CSR Address: 0x54934
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_7;  // Byte Offset: 0x126c / CSR Address: 0x54936
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_8;  // Byte Offset: 0x1270 / CSR Address: 0x54938
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_9;  // Byte Offset: 0x1274 / CSR Address: 0x5493a
                                // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_10;  // Byte Offset: 0x1278 / CSR Address: 0x5493c
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_11;  // Byte Offset: 0x127c / CSR Address: 0x5493e
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI0ErrCnt_AcLane_12;  // Byte Offset: 0x1280 / CSR Address: 0x54940
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_13;  // Byte Offset: 0x1284 / CSR Address: 0x54942
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_14;  // Byte Offset: 0x1288 / CSR Address: 0x54944
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_15;  // Byte Offset: 0x128c / CSR Address: 0x54946
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_16;  // Byte Offset: 0x1290 / CSR Address: 0x54948
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_17;  // Byte Offset: 0x1294 / CSR Address: 0x5494a
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_18;  // Byte Offset: 0x1298 / CSR Address: 0x5494c
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_19;  // Byte Offset: 0x129c / CSR Address: 0x5494e
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_20;  // Byte Offset: 0x12a0 / CSR Address: 0x54950
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_21;  // Byte Offset: 0x12a4 / CSR Address: 0x54952
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_22;  // Byte Offset: 0x12a8 / CSR Address: 0x54954
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_23;  // Byte Offset: 0x12ac / CSR Address: 0x54956
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI0ErrCnt_AcLane_24;  // Byte Offset: 0x12b0 / CSR Address: 0x54958
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_25;  // Byte Offset: 0x12b4 / CSR Address: 0x5495a
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_26;  // Byte Offset: 0x12b8 / CSR Address: 0x5495c
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_27;  // Byte Offset: 0x12bc / CSR Address: 0x5495e
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_28;  // Byte Offset: 0x12c0 / CSR Address: 0x54960
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_29;  // Byte Offset: 0x12c4 / CSR Address: 0x54962
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_30;  // Byte Offset: 0x12c8 / CSR Address: 0x54964
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_31;  // Byte Offset: 0x12cc / CSR Address: 0x54966
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_32;  // Byte Offset: 0x12d0 / CSR Address: 0x54968
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_33;  // Byte Offset: 0x12d4 / CSR Address: 0x5496a
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_34;  // Byte Offset: 0x12d8 / CSR Address: 0x5496c
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_35;  // Byte Offset: 0x12dc / CSR Address: 0x5496e
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI0ErrCnt_AcLane_36;  // Byte Offset: 0x12e0 / CSR Address: 0x54970
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_37;  // Byte Offset: 0x12e4 / CSR Address: 0x54972
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_38;  // Byte Offset: 0x12e8 / CSR Address: 0x54974
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_39;  // Byte Offset: 0x12ec / CSR Address: 0x54976
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_40;  // Byte Offset: 0x12f0 / CSR Address: 0x54978
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_41;  // Byte Offset: 0x12f4 / CSR Address: 0x5497a
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_42;  // Byte Offset: 0x12f8 / CSR Address: 0x5497c
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_43;  // Byte Offset: 0x12fc / CSR Address: 0x5497e
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_44;  // Byte Offset: 0x1300 / CSR Address: 0x54980
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_45;  // Byte Offset: 0x1304 / CSR Address: 0x54982
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_46;  // Byte Offset: 0x1308 / CSR Address: 0x54984
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI0ErrCnt_AcLane_47;  // Byte Offset: 0x130c / CSR Address: 0x54986
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI1ErrCnt_AcLane_0;  // Byte Offset: 0x1310 / CSR Address: 0x54988
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_1;  // Byte Offset: 0x1314 / CSR Address: 0x5498a
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_2;  // Byte Offset: 0x1318 / CSR Address: 0x5498c
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_3;  // Byte Offset: 0x131c / CSR Address: 0x5498e
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_4;  // Byte Offset: 0x1320 / CSR Address: 0x54990
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_5;  // Byte Offset: 0x1324 / CSR Address: 0x54992
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_6;  // Byte Offset: 0x1328 / CSR Address: 0x54994
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_7;  // Byte Offset: 0x132c / CSR Address: 0x54996
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_8;  // Byte Offset: 0x1330 / CSR Address: 0x54998
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_9;  // Byte Offset: 0x1334 / CSR Address: 0x5499a
                                // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_10;  // Byte Offset: 0x1338 / CSR Address: 0x5499c
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_11;  // Byte Offset: 0x133c / CSR Address: 0x5499e
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI1ErrCnt_AcLane_12;  // Byte Offset: 0x1340 / CSR Address: 0x549a0
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_13;  // Byte Offset: 0x1344 / CSR Address: 0x549a2
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_14;  // Byte Offset: 0x1348 / CSR Address: 0x549a4
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_15;  // Byte Offset: 0x134c / CSR Address: 0x549a6
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_16;  // Byte Offset: 0x1350 / CSR Address: 0x549a8
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_17;  // Byte Offset: 0x1354 / CSR Address: 0x549aa
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_18;  // Byte Offset: 0x1358 / CSR Address: 0x549ac
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_19;  // Byte Offset: 0x135c / CSR Address: 0x549ae
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_20;  // Byte Offset: 0x1360 / CSR Address: 0x549b0
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_21;  // Byte Offset: 0x1364 / CSR Address: 0x549b2
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_22;  // Byte Offset: 0x1368 / CSR Address: 0x549b4
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_23;  // Byte Offset: 0x136c / CSR Address: 0x549b6
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI1ErrCnt_AcLane_24;  // Byte Offset: 0x1370 / CSR Address: 0x549b8
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_25;  // Byte Offset: 0x1374 / CSR Address: 0x549ba
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_26;  // Byte Offset: 0x1378 / CSR Address: 0x549bc
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_27;  // Byte Offset: 0x137c / CSR Address: 0x549be
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_28;  // Byte Offset: 0x1380 / CSR Address: 0x549c0
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_29;  // Byte Offset: 0x1384 / CSR Address: 0x549c2
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_30;  // Byte Offset: 0x1388 / CSR Address: 0x549c4
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_31;  // Byte Offset: 0x138c / CSR Address: 0x549c6
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_32;  // Byte Offset: 0x1390 / CSR Address: 0x549c8
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_33;  // Byte Offset: 0x1394 / CSR Address: 0x549ca
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_34;  // Byte Offset: 0x1398 / CSR Address: 0x549cc
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_35;  // Byte Offset: 0x139c / CSR Address: 0x549ce
                                 // loopback eye bitmap for ACX4 0

  uint32_t UI1ErrCnt_AcLane_36;  // Byte Offset: 0x13a0 / CSR Address: 0x549d0
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_37;  // Byte Offset: 0x13a4 / CSR Address: 0x549d2
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_38;  // Byte Offset: 0x13a8 / CSR Address: 0x549d4
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_39;  // Byte Offset: 0x13ac / CSR Address: 0x549d6
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_40;  // Byte Offset: 0x13b0 / CSR Address: 0x549d8
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_41;  // Byte Offset: 0x13b4 / CSR Address: 0x549da
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_42;  // Byte Offset: 0x13b8 / CSR Address: 0x549dc
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_43;  // Byte Offset: 0x13bc / CSR Address: 0x549de
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_44;  // Byte Offset: 0x13c0 / CSR Address: 0x549e0
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_45;  // Byte Offset: 0x13c4 / CSR Address: 0x549e2
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_46;  // Byte Offset: 0x13c8 / CSR Address: 0x549e4
                                 // loopback eye bitmap for ACX4 0
  uint32_t UI1ErrCnt_AcLane_47;  // Byte Offset: 0x13cc / CSR Address: 0x549e6
                                 // loopback eye bitmap for ACX4 0


  /////////////////////////////////////////
  // Data Loopback 1D Test Results
  /////////////////////////////////////////
  uint32_t DatLoop1dErrUI0_D0_Ln0;  // Byte Offset: 0x13d0 / CSR Address: 0x549e8
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 0
  uint32_t DatLoop1dErrUI0_D0_Ln1;  // Byte Offset: 0x13d4 / CSR Address: 0x549ea
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 1
  uint32_t DatLoop1dErrUI0_D0_Ln2;  // Byte Offset: 0x13d8 / CSR Address: 0x549ec
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 2
  uint32_t DatLoop1dErrUI0_D0_Ln3;  // Byte Offset: 0x13dc / CSR Address: 0x549ee
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 3
  uint32_t DatLoop1dErrUI0_D0_Ln4;  // Byte Offset: 0x13e0 / CSR Address: 0x549f0
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 4
  uint32_t DatLoop1dErrUI0_D0_Ln5;  // Byte Offset: 0x13e4 / CSR Address: 0x549f2
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 5
  uint32_t DatLoop1dErrUI0_D0_Ln6;  // Byte Offset: 0x13e8 / CSR Address: 0x549f4
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 6
  uint32_t DatLoop1dErrUI0_D0_Ln7;  // Byte Offset: 0x13ec / CSR Address: 0x549f6
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 7
  uint32_t DatLoop1dErrUI0_D0_Ln8;  // Byte Offset: 0x13f0 / CSR Address: 0x549f8
                                    // Data loopback eye bitmap in UI0 for DBYTE0 Lane 8

  uint32_t DatLoop1dErrUI0_D1_Ln0;  // Byte Offset: 0x13f4 / CSR Address: 0x549fa
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 0
  uint32_t DatLoop1dErrUI0_D1_Ln1;  // Byte Offset: 0x13f8 / CSR Address: 0x549fc
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 1
  uint32_t DatLoop1dErrUI0_D1_Ln2;  // Byte Offset: 0x13fc / CSR Address: 0x549fe
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 2
  uint32_t DatLoop1dErrUI0_D1_Ln3;  // Byte Offset: 0x1400 / CSR Address: 0x54a00
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 3
  uint32_t DatLoop1dErrUI0_D1_Ln4;  // Byte Offset: 0x1404 / CSR Address: 0x54a02
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 4
  uint32_t DatLoop1dErrUI0_D1_Ln5;  // Byte Offset: 0x1408 / CSR Address: 0x54a04
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 5
  uint32_t DatLoop1dErrUI0_D1_Ln6;  // Byte Offset: 0x140c / CSR Address: 0x54a06
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 6
  uint32_t DatLoop1dErrUI0_D1_Ln7;  // Byte Offset: 0x1410 / CSR Address: 0x54a08
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 7
  uint32_t DatLoop1dErrUI0_D1_Ln8;  // Byte Offset: 0x1414 / CSR Address: 0x54a0a
                                    // Data loopback eye bitmap in UI0 for DBYTE1 Lane 8

  uint32_t DatLoop1dErrUI0_D2_Ln0;  // Byte Offset: 0x1418 / CSR Address: 0x54a0c
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 0
  uint32_t DatLoop1dErrUI0_D2_Ln1;  // Byte Offset: 0x141c / CSR Address: 0x54a0e
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 1
  uint32_t DatLoop1dErrUI0_D2_Ln2;  // Byte Offset: 0x1420 / CSR Address: 0x54a10
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 2
  uint32_t DatLoop1dErrUI0_D2_Ln3;  // Byte Offset: 0x1424 / CSR Address: 0x54a12
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 3
  uint32_t DatLoop1dErrUI0_D2_Ln4;  // Byte Offset: 0x1428 / CSR Address: 0x54a14
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 4
  uint32_t DatLoop1dErrUI0_D2_Ln5;  // Byte Offset: 0x142c / CSR Address: 0x54a16
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 5
  uint32_t DatLoop1dErrUI0_D2_Ln6;  // Byte Offset: 0x1430 / CSR Address: 0x54a18
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 6
  uint32_t DatLoop1dErrUI0_D2_Ln7;  // Byte Offset: 0x1434 / CSR Address: 0x54a1a
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 7
  uint32_t DatLoop1dErrUI0_D2_Ln8;  // Byte Offset: 0x1438 / CSR Address: 0x54a1c
                                    // Data loopback eye bitmap in UI0 for DBYTE2 Lane 8

  uint32_t DatLoop1dErrUI0_D3_Ln0;  // Byte Offset: 0x143c / CSR Address: 0x54a1e
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 0
  uint32_t DatLoop1dErrUI0_D3_Ln1;  // Byte Offset: 0x1440 / CSR Address: 0x54a20
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 1
  uint32_t DatLoop1dErrUI0_D3_Ln2;  // Byte Offset: 0x1444 / CSR Address: 0x54a22
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 2
  uint32_t DatLoop1dErrUI0_D3_Ln3;  // Byte Offset: 0x1448 / CSR Address: 0x54a24
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 3
  uint32_t DatLoop1dErrUI0_D3_Ln4;  // Byte Offset: 0x144c / CSR Address: 0x54a26
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 4
  uint32_t DatLoop1dErrUI0_D3_Ln5;  // Byte Offset: 0x1450 / CSR Address: 0x54a28
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 5
  uint32_t DatLoop1dErrUI0_D3_Ln6;  // Byte Offset: 0x1454 / CSR Address: 0x54a2a
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 6
  uint32_t DatLoop1dErrUI0_D3_Ln7;  // Byte Offset: 0x1458 / CSR Address: 0x54a2c
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 7
  uint32_t DatLoop1dErrUI0_D3_Ln8;  // Byte Offset: 0x145c / CSR Address: 0x54a2e
                                    // Data loopback eye bitmap in UI0 for DBYTE3 Lane 8

  uint32_t DatLoop1dErrUI0_D4_Ln0;  // Byte Offset: 0x1460 / CSR Address: 0x54a30
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 0
  uint32_t DatLoop1dErrUI0_D4_Ln1;  // Byte Offset: 0x1464 / CSR Address: 0x54a32
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 1
  uint32_t DatLoop1dErrUI0_D4_Ln2;  // Byte Offset: 0x1468 / CSR Address: 0x54a34
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 2
  uint32_t DatLoop1dErrUI0_D4_Ln3;  // Byte Offset: 0x146c / CSR Address: 0x54a36
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 3
  uint32_t DatLoop1dErrUI0_D4_Ln4;  // Byte Offset: 0x1470 / CSR Address: 0x54a38
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 4
  uint32_t DatLoop1dErrUI0_D4_Ln5;  // Byte Offset: 0x1474 / CSR Address: 0x54a3a
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 5
  uint32_t DatLoop1dErrUI0_D4_Ln6;  // Byte Offset: 0x1478 / CSR Address: 0x54a3c
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 6
  uint32_t DatLoop1dErrUI0_D4_Ln7;  // Byte Offset: 0x147c / CSR Address: 0x54a3e
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 7
  uint32_t DatLoop1dErrUI0_D4_Ln8;  // Byte Offset: 0x1480 / CSR Address: 0x54a40
                                    // Data loopback eye bitmap in UI0 for DBYTE4 Lane 8

  uint32_t DatLoop1dErrUI0_D5_Ln0;  // Byte Offset: 0x1484 / CSR Address: 0x54a42
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 0
  uint32_t DatLoop1dErrUI0_D5_Ln1;  // Byte Offset: 0x1488 / CSR Address: 0x54a44
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 1
  uint32_t DatLoop1dErrUI0_D5_Ln2;  // Byte Offset: 0x148c / CSR Address: 0x54a46
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 2
  uint32_t DatLoop1dErrUI0_D5_Ln3;  // Byte Offset: 0x1490 / CSR Address: 0x54a48
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 3
  uint32_t DatLoop1dErrUI0_D5_Ln4;  // Byte Offset: 0x1494 / CSR Address: 0x54a4a
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 4
  uint32_t DatLoop1dErrUI0_D5_Ln5;  // Byte Offset: 0x1498 / CSR Address: 0x54a4c
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 5
  uint32_t DatLoop1dErrUI0_D5_Ln6;  // Byte Offset: 0x149c / CSR Address: 0x54a4e
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 6
  uint32_t DatLoop1dErrUI0_D5_Ln7;  // Byte Offset: 0x14a0 / CSR Address: 0x54a50
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 7
  uint32_t DatLoop1dErrUI0_D5_Ln8;  // Byte Offset: 0x14a4 / CSR Address: 0x54a52
                                    // Data loopback eye bitmap in UI0 for DBYTE5 Lane 8

  uint32_t DatLoop1dErrUI0_D6_Ln0;  // Byte Offset: 0x14a8 / CSR Address: 0x54a54
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 0
  uint32_t DatLoop1dErrUI0_D6_Ln1;  // Byte Offset: 0x14ac / CSR Address: 0x54a56
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 1
  uint32_t DatLoop1dErrUI0_D6_Ln2;  // Byte Offset: 0x14b0 / CSR Address: 0x54a58
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 2
  uint32_t DatLoop1dErrUI0_D6_Ln3;  // Byte Offset: 0x14b4 / CSR Address: 0x54a5a
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 3
  uint32_t DatLoop1dErrUI0_D6_Ln4;  // Byte Offset: 0x14b8 / CSR Address: 0x54a5c
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 4
  uint32_t DatLoop1dErrUI0_D6_Ln5;  // Byte Offset: 0x14bc / CSR Address: 0x54a5e
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 5
  uint32_t DatLoop1dErrUI0_D6_Ln6;  // Byte Offset: 0x14c0 / CSR Address: 0x54a60
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 6
  uint32_t DatLoop1dErrUI0_D6_Ln7;  // Byte Offset: 0x14c4 / CSR Address: 0x54a62
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 7
  uint32_t DatLoop1dErrUI0_D6_Ln8;  // Byte Offset: 0x14c8 / CSR Address: 0x54a64
                                    // Data loopback eye bitmap in UI0 for DBYTE6 Lane 8

  uint32_t DatLoop1dErrUI0_D7_Ln0;  // Byte Offset: 0x14cc / CSR Address: 0x54a66
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 0
  uint32_t DatLoop1dErrUI0_D7_Ln1;  // Byte Offset: 0x14d0 / CSR Address: 0x54a68
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 1
  uint32_t DatLoop1dErrUI0_D7_Ln2;  // Byte Offset: 0x14d4 / CSR Address: 0x54a6a
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 2
  uint32_t DatLoop1dErrUI0_D7_Ln3;  // Byte Offset: 0x14d8 / CSR Address: 0x54a6c
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 3
  uint32_t DatLoop1dErrUI0_D7_Ln4;  // Byte Offset: 0x14dc / CSR Address: 0x54a6e
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 4
  uint32_t DatLoop1dErrUI0_D7_Ln5;  // Byte Offset: 0x14e0 / CSR Address: 0x54a70
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 5
  uint32_t DatLoop1dErrUI0_D7_Ln6;  // Byte Offset: 0x14e4 / CSR Address: 0x54a72
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 6
  uint32_t DatLoop1dErrUI0_D7_Ln7;  // Byte Offset: 0x14e8 / CSR Address: 0x54a74
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 7
  uint32_t DatLoop1dErrUI0_D7_Ln8;  // Byte Offset: 0x14ec / CSR Address: 0x54a76
                                    // Data loopback eye bitmap in UI0 for DBYTE7 Lane 8

  uint32_t DatLoop1dErrUI0_D8_Ln0;  // Byte Offset: 0x14f0 / CSR Address: 0x54a78
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 0
  uint32_t DatLoop1dErrUI0_D8_Ln1;  // Byte Offset: 0x14f4 / CSR Address: 0x54a7a
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 1
  uint32_t DatLoop1dErrUI0_D8_Ln2;  // Byte Offset: 0x14f8 / CSR Address: 0x54a7c
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 2
  uint32_t DatLoop1dErrUI0_D8_Ln3;  // Byte Offset: 0x14fc / CSR Address: 0x54a7e
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 3
  uint32_t DatLoop1dErrUI0_D8_Ln4;  // Byte Offset: 0x1500 / CSR Address: 0x54a80
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 4
  uint32_t DatLoop1dErrUI0_D8_Ln5;  // Byte Offset: 0x1504 / CSR Address: 0x54a82
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 5
  uint32_t DatLoop1dErrUI0_D8_Ln6;  // Byte Offset: 0x1508 / CSR Address: 0x54a84
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 6
  uint32_t DatLoop1dErrUI0_D8_Ln7;  // Byte Offset: 0x150c / CSR Address: 0x54a86
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 7
  uint32_t DatLoop1dErrUI0_D8_Ln8;  // Byte Offset: 0x1510 / CSR Address: 0x54a88
                                    // Data loopback eye bitmap in UI0 for DBYTE8 Lane 8

  uint32_t DatLoop1dErrUI1_D0_Ln0;  // Byte Offset: 0x1514 / CSR Address: 0x54a8a
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 0
  uint32_t DatLoop1dErrUI1_D0_Ln1;  // Byte Offset: 0x1518 / CSR Address: 0x54a8c
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 1
  uint32_t DatLoop1dErrUI1_D0_Ln2;  // Byte Offset: 0x151c / CSR Address: 0x54a8e
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 2
  uint32_t DatLoop1dErrUI1_D0_Ln3;  // Byte Offset: 0x1520 / CSR Address: 0x54a90
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 3
  uint32_t DatLoop1dErrUI1_D0_Ln4;  // Byte Offset: 0x1524 / CSR Address: 0x54a92
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 4
  uint32_t DatLoop1dErrUI1_D0_Ln5;  // Byte Offset: 0x1528 / CSR Address: 0x54a94
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 5
  uint32_t DatLoop1dErrUI1_D0_Ln6;  // Byte Offset: 0x152c / CSR Address: 0x54a96
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 6
  uint32_t DatLoop1dErrUI1_D0_Ln7;  // Byte Offset: 0x1530 / CSR Address: 0x54a98
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 7
  uint32_t DatLoop1dErrUI1_D0_Ln8;  // Byte Offset: 0x1534 / CSR Address: 0x54a9a
                                    // Data loopback eye bitmap in UI1 for DBYTE0 Lane 8

  uint32_t DatLoop1dErrUI1_D1_Ln0;  // Byte Offset: 0x1538 / CSR Address: 0x54a9c
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 0
  uint32_t DatLoop1dErrUI1_D1_Ln1;  // Byte Offset: 0x153c / CSR Address: 0x54a9e
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 1
  uint32_t DatLoop1dErrUI1_D1_Ln2;  // Byte Offset: 0x1540 / CSR Address: 0x54aa0
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 2
  uint32_t DatLoop1dErrUI1_D1_Ln3;  // Byte Offset: 0x1544 / CSR Address: 0x54aa2
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 3
  uint32_t DatLoop1dErrUI1_D1_Ln4;  // Byte Offset: 0x1548 / CSR Address: 0x54aa4
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 4
  uint32_t DatLoop1dErrUI1_D1_Ln5;  // Byte Offset: 0x154c / CSR Address: 0x54aa6
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 5
  uint32_t DatLoop1dErrUI1_D1_Ln6;  // Byte Offset: 0x1550 / CSR Address: 0x54aa8
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 6
  uint32_t DatLoop1dErrUI1_D1_Ln7;  // Byte Offset: 0x1554 / CSR Address: 0x54aaa
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 7
  uint32_t DatLoop1dErrUI1_D1_Ln8;  // Byte Offset: 0x1558 / CSR Address: 0x54aac
                                    // Data loopback eye bitmap in UI1 for DBYTE1 Lane 8

  uint32_t DatLoop1dErrUI1_D2_Ln0;  // Byte Offset: 0x155c / CSR Address: 0x54aae
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 0
  uint32_t DatLoop1dErrUI1_D2_Ln1;  // Byte Offset: 0x1560 / CSR Address: 0x54ab0
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 1
  uint32_t DatLoop1dErrUI1_D2_Ln2;  // Byte Offset: 0x1564 / CSR Address: 0x54ab2
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 2
  uint32_t DatLoop1dErrUI1_D2_Ln3;  // Byte Offset: 0x1568 / CSR Address: 0x54ab4
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 3
  uint32_t DatLoop1dErrUI1_D2_Ln4;  // Byte Offset: 0x156c / CSR Address: 0x54ab6
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 4
  uint32_t DatLoop1dErrUI1_D2_Ln5;  // Byte Offset: 0x1570 / CSR Address: 0x54ab8
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 5
  uint32_t DatLoop1dErrUI1_D2_Ln6;  // Byte Offset: 0x1574 / CSR Address: 0x54aba
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 6
  uint32_t DatLoop1dErrUI1_D2_Ln7;  // Byte Offset: 0x1578 / CSR Address: 0x54abc
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 7
  uint32_t DatLoop1dErrUI1_D2_Ln8;  // Byte Offset: 0x157c / CSR Address: 0x54abe
                                    // Data loopback eye bitmap in UI1 for DBYTE2 Lane 8

  uint32_t DatLoop1dErrUI1_D3_Ln0;  // Byte Offset: 0x1580 / CSR Address: 0x54ac0
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 0
  uint32_t DatLoop1dErrUI1_D3_Ln1;  // Byte Offset: 0x1584 / CSR Address: 0x54ac2
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 1
  uint32_t DatLoop1dErrUI1_D3_Ln2;  // Byte Offset: 0x1588 / CSR Address: 0x54ac4
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 2
  uint32_t DatLoop1dErrUI1_D3_Ln3;  // Byte Offset: 0x158c / CSR Address: 0x54ac6
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 3
  uint32_t DatLoop1dErrUI1_D3_Ln4;  // Byte Offset: 0x1590 / CSR Address: 0x54ac8
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 4
  uint32_t DatLoop1dErrUI1_D3_Ln5;  // Byte Offset: 0x1594 / CSR Address: 0x54aca
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 5
  uint32_t DatLoop1dErrUI1_D3_Ln6;  // Byte Offset: 0x1598 / CSR Address: 0x54acc
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 6
  uint32_t DatLoop1dErrUI1_D3_Ln7;  // Byte Offset: 0x159c / CSR Address: 0x54ace
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 7
  uint32_t DatLoop1dErrUI1_D3_Ln8;  // Byte Offset: 0x15a0 / CSR Address: 0x54ad0
                                    // Data loopback eye bitmap in UI1 for DBYTE3 Lane 8

  uint32_t DatLoop1dErrUI1_D4_Ln0;  // Byte Offset: 0x15a4 / CSR Address: 0x54ad2
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 0
  uint32_t DatLoop1dErrUI1_D4_Ln1;  // Byte Offset: 0x15a8 / CSR Address: 0x54ad4
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 1
  uint32_t DatLoop1dErrUI1_D4_Ln2;  // Byte Offset: 0x15ac / CSR Address: 0x54ad6
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 2
  uint32_t DatLoop1dErrUI1_D4_Ln3;  // Byte Offset: 0x15b0 / CSR Address: 0x54ad8
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 3
  uint32_t DatLoop1dErrUI1_D4_Ln4;  // Byte Offset: 0x15b4 / CSR Address: 0x54ada
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 4
  uint32_t DatLoop1dErrUI1_D4_Ln5;  // Byte Offset: 0x15b8 / CSR Address: 0x54adc
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 5
  uint32_t DatLoop1dErrUI1_D4_Ln6;  // Byte Offset: 0x15bc / CSR Address: 0x54ade
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 6
  uint32_t DatLoop1dErrUI1_D4_Ln7;  // Byte Offset: 0x15c0 / CSR Address: 0x54ae0
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 7
  uint32_t DatLoop1dErrUI1_D4_Ln8;  // Byte Offset: 0x15c4 / CSR Address: 0x54ae2
                                    // Data loopback eye bitmap in UI1 for DBYTE4 Lane 8

  uint32_t DatLoop1dErrUI1_D5_Ln0;  // Byte Offset: 0x15c8 / CSR Address: 0x54ae4
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 0
  uint32_t DatLoop1dErrUI1_D5_Ln1;  // Byte Offset: 0x15cc / CSR Address: 0x54ae6
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 1
  uint32_t DatLoop1dErrUI1_D5_Ln2;  // Byte Offset: 0x15d0 / CSR Address: 0x54ae8
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 2
  uint32_t DatLoop1dErrUI1_D5_Ln3;  // Byte Offset: 0x15d4 / CSR Address: 0x54aea
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 3
  uint32_t DatLoop1dErrUI1_D5_Ln4;  // Byte Offset: 0x15d8 / CSR Address: 0x54aec
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 4
  uint32_t DatLoop1dErrUI1_D5_Ln5;  // Byte Offset: 0x15dc / CSR Address: 0x54aee
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 5
  uint32_t DatLoop1dErrUI1_D5_Ln6;  // Byte Offset: 0x15e0 / CSR Address: 0x54af0
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 6
  uint32_t DatLoop1dErrUI1_D5_Ln7;  // Byte Offset: 0x15e4 / CSR Address: 0x54af2
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 7
  uint32_t DatLoop1dErrUI1_D5_Ln8;  // Byte Offset: 0x15e8 / CSR Address: 0x54af4
                                    // Data loopback eye bitmap in UI1 for DBYTE5 Lane 8

  uint32_t DatLoop1dErrUI1_D6_Ln0;  // Byte Offset: 0x15ec / CSR Address: 0x54af6
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 0
  uint32_t DatLoop1dErrUI1_D6_Ln1;  // Byte Offset: 0x15f0 / CSR Address: 0x54af8
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 1
  uint32_t DatLoop1dErrUI1_D6_Ln2;  // Byte Offset: 0x15f4 / CSR Address: 0x54afa
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 2
  uint32_t DatLoop1dErrUI1_D6_Ln3;  // Byte Offset: 0x15f8 / CSR Address: 0x54afc
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 3
  uint32_t DatLoop1dErrUI1_D6_Ln4;  // Byte Offset: 0x15fc / CSR Address: 0x54afe
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 4
  uint32_t DatLoop1dErrUI1_D6_Ln5;  // Byte Offset: 0x1600 / CSR Address: 0x54b00
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 5
  uint32_t DatLoop1dErrUI1_D6_Ln6;  // Byte Offset: 0x1604 / CSR Address: 0x54b02
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 6
  uint32_t DatLoop1dErrUI1_D6_Ln7;  // Byte Offset: 0x1608 / CSR Address: 0x54b04
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 7
  uint32_t DatLoop1dErrUI1_D6_Ln8;  // Byte Offset: 0x160c / CSR Address: 0x54b06
                                    // Data loopback eye bitmap in UI1 for DBYTE6 Lane 8

  uint32_t DatLoop1dErrUI1_D7_Ln0;  // Byte Offset: 0x1610 / CSR Address: 0x54b08
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 0
  uint32_t DatLoop1dErrUI1_D7_Ln1;  // Byte Offset: 0x1614 / CSR Address: 0x54b0a
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 1
  uint32_t DatLoop1dErrUI1_D7_Ln2;  // Byte Offset: 0x1618 / CSR Address: 0x54b0c
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 2
  uint32_t DatLoop1dErrUI1_D7_Ln3;  // Byte Offset: 0x161c / CSR Address: 0x54b0e
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 3
  uint32_t DatLoop1dErrUI1_D7_Ln4;  // Byte Offset: 0x1620 / CSR Address: 0x54b10
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 4
  uint32_t DatLoop1dErrUI1_D7_Ln5;  // Byte Offset: 0x1624 / CSR Address: 0x54b12
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 5
  uint32_t DatLoop1dErrUI1_D7_Ln6;  // Byte Offset: 0x1628 / CSR Address: 0x54b14
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 6
  uint32_t DatLoop1dErrUI1_D7_Ln7;  // Byte Offset: 0x162c / CSR Address: 0x54b16
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 7
  uint32_t DatLoop1dErrUI1_D7_Ln8;  // Byte Offset: 0x1630 / CSR Address: 0x54b18
                                    // Data loopback eye bitmap in UI1 for DBYTE7 Lane 8

  uint32_t DatLoop1dErrUI1_D8_Ln0;  // Byte Offset: 0x1634 / CSR Address: 0x54b1a
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 0
  uint32_t DatLoop1dErrUI1_D8_Ln1;  // Byte Offset: 0x1638 / CSR Address: 0x54b1c
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 1
  uint32_t DatLoop1dErrUI1_D8_Ln2;  // Byte Offset: 0x163c / CSR Address: 0x54b1e
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 2
  uint32_t DatLoop1dErrUI1_D8_Ln3;  // Byte Offset: 0x1640 / CSR Address: 0x54b20
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 3
  uint32_t DatLoop1dErrUI1_D8_Ln4;  // Byte Offset: 0x1644 / CSR Address: 0x54b22
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 4
  uint32_t DatLoop1dErrUI1_D8_Ln5;  // Byte Offset: 0x1648 / CSR Address: 0x54b24
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 5
  uint32_t DatLoop1dErrUI1_D8_Ln6;  // Byte Offset: 0x164c / CSR Address: 0x54b26
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 6
  uint32_t DatLoop1dErrUI1_D8_Ln7;  // Byte Offset: 0x1650 / CSR Address: 0x54b28
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 7
  uint32_t DatLoop1dErrUI1_D8_Ln8;  // Byte Offset: 0x1654 / CSR Address: 0x54b2a
                                    // Data loopback eye bitmap in UI1 for DBYTE8 Lane 8

  uint32_t DatLoop1dErrUI2_D0_Ln0;  // Byte Offset: 0x1658 / CSR Address: 0x54b2c
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 0
  uint32_t DatLoop1dErrUI2_D0_Ln1;  // Byte Offset: 0x165c / CSR Address: 0x54b2e
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 1
  uint32_t DatLoop1dErrUI2_D0_Ln2;  // Byte Offset: 0x1660 / CSR Address: 0x54b30
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 2
  uint32_t DatLoop1dErrUI2_D0_Ln3;  // Byte Offset: 0x1664 / CSR Address: 0x54b32
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 3
  uint32_t DatLoop1dErrUI2_D0_Ln4;  // Byte Offset: 0x1668 / CSR Address: 0x54b34
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 4
  uint32_t DatLoop1dErrUI2_D0_Ln5;  // Byte Offset: 0x166c / CSR Address: 0x54b36
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 5
  uint32_t DatLoop1dErrUI2_D0_Ln6;  // Byte Offset: 0x1670 / CSR Address: 0x54b38
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 6
  uint32_t DatLoop1dErrUI2_D0_Ln7;  // Byte Offset: 0x1674 / CSR Address: 0x54b3a
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 7
  uint32_t DatLoop1dErrUI2_D0_Ln8;  // Byte Offset: 0x1678 / CSR Address: 0x54b3c
                                    // Data loopback eye bitmap in UI2 for DBYTE0 Lane 8

  uint32_t DatLoop1dErrUI2_D1_Ln0;  // Byte Offset: 0x167c / CSR Address: 0x54b3e
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 0
  uint32_t DatLoop1dErrUI2_D1_Ln1;  // Byte Offset: 0x1680 / CSR Address: 0x54b40
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 1
  uint32_t DatLoop1dErrUI2_D1_Ln2;  // Byte Offset: 0x1684 / CSR Address: 0x54b42
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 2
  uint32_t DatLoop1dErrUI2_D1_Ln3;  // Byte Offset: 0x1688 / CSR Address: 0x54b44
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 3
  uint32_t DatLoop1dErrUI2_D1_Ln4;  // Byte Offset: 0x168c / CSR Address: 0x54b46
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 4
  uint32_t DatLoop1dErrUI2_D1_Ln5;  // Byte Offset: 0x1690 / CSR Address: 0x54b48
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 5
  uint32_t DatLoop1dErrUI2_D1_Ln6;  // Byte Offset: 0x1694 / CSR Address: 0x54b4a
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 6
  uint32_t DatLoop1dErrUI2_D1_Ln7;  // Byte Offset: 0x1698 / CSR Address: 0x54b4c
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 7
  uint32_t DatLoop1dErrUI2_D1_Ln8;  // Byte Offset: 0x169c / CSR Address: 0x54b4e
                                    // Data loopback eye bitmap in UI2 for DBYTE1 Lane 8

  uint32_t DatLoop1dErrUI2_D2_Ln0;  // Byte Offset: 0x16a0 / CSR Address: 0x54b50
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 0
  uint32_t DatLoop1dErrUI2_D2_Ln1;  // Byte Offset: 0x16a4 / CSR Address: 0x54b52
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 1
  uint32_t DatLoop1dErrUI2_D2_Ln2;  // Byte Offset: 0x16a8 / CSR Address: 0x54b54
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 2
  uint32_t DatLoop1dErrUI2_D2_Ln3;  // Byte Offset: 0x16ac / CSR Address: 0x54b56
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 3
  uint32_t DatLoop1dErrUI2_D2_Ln4;  // Byte Offset: 0x16b0 / CSR Address: 0x54b58
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 4
  uint32_t DatLoop1dErrUI2_D2_Ln5;  // Byte Offset: 0x16b4 / CSR Address: 0x54b5a
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 5
  uint32_t DatLoop1dErrUI2_D2_Ln6;  // Byte Offset: 0x16b8 / CSR Address: 0x54b5c
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 6
  uint32_t DatLoop1dErrUI2_D2_Ln7;  // Byte Offset: 0x16bc / CSR Address: 0x54b5e
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 7
  uint32_t DatLoop1dErrUI2_D2_Ln8;  // Byte Offset: 0x16c0 / CSR Address: 0x54b60
                                    // Data loopback eye bitmap in UI2 for DBYTE2 Lane 8

  uint32_t DatLoop1dErrUI2_D3_Ln0;  // Byte Offset: 0x16c4 / CSR Address: 0x54b62
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 0
  uint32_t DatLoop1dErrUI2_D3_Ln1;  // Byte Offset: 0x16c8 / CSR Address: 0x54b64
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 1
  uint32_t DatLoop1dErrUI2_D3_Ln2;  // Byte Offset: 0x16cc / CSR Address: 0x54b66
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 2
  uint32_t DatLoop1dErrUI2_D3_Ln3;  // Byte Offset: 0x16d0 / CSR Address: 0x54b68
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 3
  uint32_t DatLoop1dErrUI2_D3_Ln4;  // Byte Offset: 0x16d4 / CSR Address: 0x54b6a
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 4
  uint32_t DatLoop1dErrUI2_D3_Ln5;  // Byte Offset: 0x16d8 / CSR Address: 0x54b6c
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 5
  uint32_t DatLoop1dErrUI2_D3_Ln6;  // Byte Offset: 0x16dc / CSR Address: 0x54b6e
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 6
  uint32_t DatLoop1dErrUI2_D3_Ln7;  // Byte Offset: 0x16e0 / CSR Address: 0x54b70
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 7
  uint32_t DatLoop1dErrUI2_D3_Ln8;  // Byte Offset: 0x16e4 / CSR Address: 0x54b72
                                    // Data loopback eye bitmap in UI2 for DBYTE3 Lane 8

  uint32_t DatLoop1dErrUI2_D4_Ln0;  // Byte Offset: 0x16e8 / CSR Address: 0x54b74
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 0
  uint32_t DatLoop1dErrUI2_D4_Ln1;  // Byte Offset: 0x16ec / CSR Address: 0x54b76
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 1
  uint32_t DatLoop1dErrUI2_D4_Ln2;  // Byte Offset: 0x16f0 / CSR Address: 0x54b78
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 2
  uint32_t DatLoop1dErrUI2_D4_Ln3;  // Byte Offset: 0x16f4 / CSR Address: 0x54b7a
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 3
  uint32_t DatLoop1dErrUI2_D4_Ln4;  // Byte Offset: 0x16f8 / CSR Address: 0x54b7c
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 4
  uint32_t DatLoop1dErrUI2_D4_Ln5;  // Byte Offset: 0x16fc / CSR Address: 0x54b7e
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 5
  uint32_t DatLoop1dErrUI2_D4_Ln6;  // Byte Offset: 0x1700 / CSR Address: 0x54b80
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 6
  uint32_t DatLoop1dErrUI2_D4_Ln7;  // Byte Offset: 0x1704 / CSR Address: 0x54b82
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 7
  uint32_t DatLoop1dErrUI2_D4_Ln8;  // Byte Offset: 0x1708 / CSR Address: 0x54b84
                                    // Data loopback eye bitmap in UI2 for DBYTE4 Lane 8

  uint32_t DatLoop1dErrUI2_D5_Ln0;  // Byte Offset: 0x170c / CSR Address: 0x54b86
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 0
  uint32_t DatLoop1dErrUI2_D5_Ln1;  // Byte Offset: 0x1710 / CSR Address: 0x54b88
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 1
  uint32_t DatLoop1dErrUI2_D5_Ln2;  // Byte Offset: 0x1714 / CSR Address: 0x54b8a
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 2
  uint32_t DatLoop1dErrUI2_D5_Ln3;  // Byte Offset: 0x1718 / CSR Address: 0x54b8c
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 3
  uint32_t DatLoop1dErrUI2_D5_Ln4;  // Byte Offset: 0x171c / CSR Address: 0x54b8e
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 4
  uint32_t DatLoop1dErrUI2_D5_Ln5;  // Byte Offset: 0x1720 / CSR Address: 0x54b90
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 5
  uint32_t DatLoop1dErrUI2_D5_Ln6;  // Byte Offset: 0x1724 / CSR Address: 0x54b92
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 6
  uint32_t DatLoop1dErrUI2_D5_Ln7;  // Byte Offset: 0x1728 / CSR Address: 0x54b94
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 7
  uint32_t DatLoop1dErrUI2_D5_Ln8;  // Byte Offset: 0x172c / CSR Address: 0x54b96
                                    // Data loopback eye bitmap in UI2 for DBYTE5 Lane 8

  uint32_t DatLoop1dErrUI2_D6_Ln0;  // Byte Offset: 0x1730 / CSR Address: 0x54b98
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 0
  uint32_t DatLoop1dErrUI2_D6_Ln1;  // Byte Offset: 0x1734 / CSR Address: 0x54b9a
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 1
  uint32_t DatLoop1dErrUI2_D6_Ln2;  // Byte Offset: 0x1738 / CSR Address: 0x54b9c
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 2
  uint32_t DatLoop1dErrUI2_D6_Ln3;  // Byte Offset: 0x173c / CSR Address: 0x54b9e
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 3
  uint32_t DatLoop1dErrUI2_D6_Ln4;  // Byte Offset: 0x1740 / CSR Address: 0x54ba0
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 4
  uint32_t DatLoop1dErrUI2_D6_Ln5;  // Byte Offset: 0x1744 / CSR Address: 0x54ba2
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 5
  uint32_t DatLoop1dErrUI2_D6_Ln6;  // Byte Offset: 0x1748 / CSR Address: 0x54ba4
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 6
  uint32_t DatLoop1dErrUI2_D6_Ln7;  // Byte Offset: 0x174c / CSR Address: 0x54ba6
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 7
  uint32_t DatLoop1dErrUI2_D6_Ln8;  // Byte Offset: 0x1750 / CSR Address: 0x54ba8
                                    // Data loopback eye bitmap in UI2 for DBYTE6 Lane 8

  uint32_t DatLoop1dErrUI2_D7_Ln0;  // Byte Offset: 0x1754 / CSR Address: 0x54baa
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 0
  uint32_t DatLoop1dErrUI2_D7_Ln1;  // Byte Offset: 0x1758 / CSR Address: 0x54bac
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 1
  uint32_t DatLoop1dErrUI2_D7_Ln2;  // Byte Offset: 0x175c / CSR Address: 0x54bae
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 2
  uint32_t DatLoop1dErrUI2_D7_Ln3;  // Byte Offset: 0x1760 / CSR Address: 0x54bb0
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 3
  uint32_t DatLoop1dErrUI2_D7_Ln4;  // Byte Offset: 0x1764 / CSR Address: 0x54bb2
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 4
  uint32_t DatLoop1dErrUI2_D7_Ln5;  // Byte Offset: 0x1768 / CSR Address: 0x54bb4
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 5
  uint32_t DatLoop1dErrUI2_D7_Ln6;  // Byte Offset: 0x176c / CSR Address: 0x54bb6
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 6
  uint32_t DatLoop1dErrUI2_D7_Ln7;  // Byte Offset: 0x1770 / CSR Address: 0x54bb8
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 7
  uint32_t DatLoop1dErrUI2_D7_Ln8;  // Byte Offset: 0x1774 / CSR Address: 0x54bba
                                    // Data loopback eye bitmap in UI2 for DBYTE7 Lane 8

  uint32_t DatLoop1dErrUI2_D8_Ln0;  // Byte Offset: 0x1778 / CSR Address: 0x54bbc
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 0
  uint32_t DatLoop1dErrUI2_D8_Ln1;  // Byte Offset: 0x177c / CSR Address: 0x54bbe
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 1
  uint32_t DatLoop1dErrUI2_D8_Ln2;  // Byte Offset: 0x1780 / CSR Address: 0x54bc0
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 2
  uint32_t DatLoop1dErrUI2_D8_Ln3;  // Byte Offset: 0x1784 / CSR Address: 0x54bc2
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 3
  uint32_t DatLoop1dErrUI2_D8_Ln4;  // Byte Offset: 0x1788 / CSR Address: 0x54bc4
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 4
  uint32_t DatLoop1dErrUI2_D8_Ln5;  // Byte Offset: 0x178c / CSR Address: 0x54bc6
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 5
  uint32_t DatLoop1dErrUI2_D8_Ln6;  // Byte Offset: 0x1790 / CSR Address: 0x54bc8
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 6
  uint32_t DatLoop1dErrUI2_D8_Ln7;  // Byte Offset: 0x1794 / CSR Address: 0x54bca
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 7
  uint32_t DatLoop1dErrUI2_D8_Ln8;  // Byte Offset: 0x1798 / CSR Address: 0x54bcc
                                    // Data loopback eye bitmap in UI2 for DBYTE8 Lane 8

 
  /////////////////////////////////////////
  // Data Loopback 2D Test Results
  /////////////////////////////////////////
  uint32_t DatLoop2dVrefErrUI0[128];  // Byte Offset: 0x179c / CSR Address: 0x54bce
                                      // Data Loopback Vref error count in UI0 
  uint32_t DatLoop2dVrefErrUI1[128];  // Byte Offset: 0x199c / CSR Address: 0x54cce
                                      // Data Loopback Vref error count in UI1 
  uint32_t DatLoop2dVrefErrUI2[128];  // Byte Offset: 0x1b9c / CSR Address: 0x54dce
                                      // Data Loopback Vref error count in UI2 
   

} __attribute__ ((packed)) PMU_SMB_ATE_t;

#endif // _MNPMUSRAMMSGBLOCK_ATE_H

