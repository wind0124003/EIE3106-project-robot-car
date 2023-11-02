/*
 * ExplainRemoteControl.c
 *
 * Created: 3/3/2021 23:12:22
 *  Author: chanw
 */ 

// Button
// PB0		SW20	SW30
// PB1		SW21	SW31
// PB2		SW22	SW32
// GND		SW2		SW3
//			PD2		PD3

// 1. output '000' to portB and read PD2, PD3 from portD
// 2. If '11' is read, that means no key is pressed.
// 3. if one of the column has a zero, that means a key is pressed at that column
// 4. then starting with the top row, the microcontroller grounds it by providing a low to each row
// (011,101,110)  (PB2 PB1 PB0)
// 5. get the reading
// 6. if which row is not 11, the row's button is clicked

// Example - sw21
// 1. if 'SW21' is pressed, the data read should be '01'
// 2. starting with the top row, the microcontroller grounds it
// 3. - 011,101,110
// 4. reading should be 11,01,11
// 5. thus, the key 'sw21' is detected

// example - sw2
// 1. if 'sw2' is pressed, the data read should be '21'
// 2. grounds all row
// 3. - 011, 101, 110
// 4. reading should always be 01
// 5. the key 'SW2' is detected


