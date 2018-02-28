Copyright (c) 2017 Synopsys, Inc. 
This IP and the associated documentation are proprietary to Synopsys, Inc. 
This IP may only be used in accordance with the terms and conditions 
of a written license agreement with Synopsys, Inc. All other use, 
reproduction, or distribution of this IP is strictly prohibited.
------------------------------------------------------------------

LPDDR4_MULTIPHY_V2 ATE FIRMWARE

Version 0.71a_patch1
Feb 03, 2017

Changes from version 0.71a
(mandatory)

Bug Fixes
---------
- ATE FW: LCDL Linearity testing improvements
- Re-aligned wait time start to be after Dll Lock sequence begins
- Changed the MemClkTime parameter to be in microseconds rather than
  nanoseconds to allow a longer time for the clock to be output.
- Added the loopback run time for case where the testing is done serially on the ACX4 or DBYTE instances when MinPadLpbkPwr is set to 1.
- Adding the testing order / encoding of the LCDLs


Enhancements
------------
- ATE FW: Add option to test each dbyte and anib loopback serially to reduce
  power
- ATE FW: Restore csrs to reset values at end of ATE


Special Notes
-------------
- ATE firmware status pending feedback from customer on requested enhancements.
- This version of ATE firmware will report revision number 0.72a (0x0720).

Minimum Required Versions
-------------------------
- PUB: 1.00a

------------------------------------------------------------------

Version 0.71a
December 1, 2016

Changes from version 0.70b
(mandatory)

Bug Fixes
---------
- Addressed Tx Vref calculation for LPDDR4.
- Addressed calculation of RxVref margin.
- Addressed a timing violation for LPDDR3 during training for writes to precharge.
- Addressed a parity error for SRE during DDDR4 training when parity is enabled.
- Addressed an issue in 2D training where the results were not showing a violation of the margin requirement.
- Addressed an issue where the MRL was being reset when DM training was enabled.
- Addressed issue with DDC post training in LPDDR4 that could lead to bubble violation.

Enhancements
------------
- Added a feature to perform CA training during the 2D training if requested
- Improvements to the 1D and 2D training algorithms due to silicon learning.
- Improvements to the DDR3/LPDDR3 write leveling algorithm.
- Improved DevInit procedure for LPDDR4.
- Changed LPDDR4 training to send MR24.
- Added ability to train CA Vref in LPDDR4.
- ATE firmware improvements.
- Improvements to the debug messages used by training firmware.
- General stability and code improvements.

Special Notes
-------------
- ATE firmware has known issues with the following and therefore is not
  officially supported in this release
  * Firmware image must be reloaded between runs
  * Tests must be run individually. 
  * These issues will be addressed in a subsequent release.
This Firmware Image is not considered final and updates are to be expected.

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.20a

------------------------------------------------------------------

Version 0.70b
October 6, 2016

Changes from version 0.70a
(recommended)

Bug Fixes
---------
- Clarification from 0.70a Readme
  * csrDFIMRL no longer reset by 2D training when DDR4 DM enabled

Enhancements
------------
- None

Special Notes
-------------
- ATE firmware has known issues with the following tests and therefore
  are not supported in this release
  * AC Loopback
  * Data 1D/2D Loopback
  * Burn-in

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.20a

------------------------------------------------------------------

Version 0.70a
Sep 16, 2016

Changes from version 0.61a
(mandatory)

Bug Fixes
---------
- csrDFIMRL no longer reset by 2D training

Enhancements
------------
- ATE firmware message block and interaction updated to be much closer to
  training firmware interaction.
- Update internal revision ID to match release.
- General product improvements in 1D and 2D training based on simulation and
  lab learnings.

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.20a

------------------------------------------------------------------

Version 0.61a
Aug 15, 2016

Changes from version 0.60a
(recommended)

Bug Fixes
---------
- Improvements for customer enablement.
- Improvement to RxEnb training for large board delays
- Improvement to DM training
- Addressed training issue when Channel A not fully populated
- Addressed DDC issue for multiple power states.
- Improvements to CA training in LPDDR4
- Addressed training issue in LPDDR3 and LPDDR4 when DQ bits swizzled.

Enhancements
------------
- Firmware can disable all mailbox message, including errors

Special Notes
-------------
- Firmware improvements/enhancements ongoing. Updates to firmware should be expected.
  This is not a tapeout gating item.

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.03a

------------------------------------------------------------------

Version 0.60a
Jul 22, 2016

Changes from version 0.51a
(recommended)

Bug Fixes
---------
- Addressed out-of-bound issue in LPDDR4 WrDq2D training.
- Addressed ANIB delay setting in LPDDR3/LPDDR4 CA training in case of CLK inversion.

Enhancements
------------
- 2D training switches between ranges in order to expand VDDQ range coverage

Special Notes
-------------
- Firmware improvements/enhancements ongoing. Updates to firmware should be expected.
  This is not a tapeout gating item.

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.03a

------------------------------------------------------------------

Version 0.51a 
Jul 11, 2016

Changes from version 0.50a
(recommended)

Bug Fixes
---------
- Improvements for customer enablement.

Enhancements
------------
- Improvements for customer enablement.

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.03a

------------------------------------------------------------------

Version 0.50a 
Jun 22, 2016

Changes from version 0.40a
(mandatory)

- Updated ATE firmware with improvements. ATE tests 3, 4, 5, 6, and 254 
  report errors in simulation on some configurations. The issue(s) will 
  be address in a future ATE release.
- Updated 1D firmware for LPDDR3, LPDDR4,DDR3 and DDR4, with improvements 
  and bug fixes.
- Updated 2D firmware for DDR4 and LPDDR4, with improvements and bug fixes.
- DDR3 message block updated to remove unused fields (marked as reserved).
- Improvements to documentation and comments related to firmware 
  message block.
- Bug fixes and improvements to the PhyInit process to enable the 
  training firmware.

Minimum Required Versions
-------------------------
- PUB: 1.00a
- Phyinit: 1.01a

------------------------------------------------------------------

Version 0.40a 
Apr 18, 2016

Changes from version 0.30a
(mandatory)

- Released initial ATE firmware (replaces placeholder version in prior
  releases).
- Updated 1D firmware for DDR3/DDR4/LPDDR3/LPDDR4, supporting all training
  steps.
- Released initial 2D firmware for DDR4/LPDDR4 (replaces placeholder
  version in prior releases).

------------------------------------------------------------------

Version 0.30a 
Mar 21, 2016

Changes from version 0.20a
(recommended)

- Reorganized and expanded firmware deliverables as follows:-
-- ate (placeholder only - will be populated in later release)
-- ddr3 (previously called ddr3_udimm)
-- ddr4 (previously called ddr4_udimm)
-- ddr4_2d (placeholder only - will be populated in later release)
-- lpddr3
-- lpddr4
-- lpddr4_2d (placeholder only - will be populated in later release)

------------------------------------------------------------------

Version 0.20a
Oct 23, 2015

Initial release

------------------------------------------------------------------


Special Notes
-------------
- Please expect a future, updated firmware release based on Testchip
  evaluation.  Not a tapeout gating item.

