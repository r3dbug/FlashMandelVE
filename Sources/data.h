
#ifndef DATA_H
#define DATA_H

/* Defines the offsets (FMO = FlashMandel Offsets) 
 * to store fractal calculation data in NewMANDChunk 
 * and NewRGBMANDChunk. 
 */

/* algorithms (ULONG) (max: 4) */
#define FMO_ALGORITHM_BASE	 	0
#define FMO_ALGORITHM_BUDDHA 	1
#define FMO_ALGORITHM_3	 		3
#define FMO_ALGORITHM_4 		4

/* switches (BOOL) (max: 4) */
#define FMO_JULIA 		0
#define FMO_SWITCH_2	1
#define FMO_SWITCH_3 	3					
#define FMO_SWITCH_4 	4

/* bits: FMO_SWITCH_2 */

#define FMO_BIT_ANTIBUDDHA 			0   /* bit 0: ANTIBUDDHA */
#define FMO_BIT_BUDDHA              1   /* bit 1: BUDDHA */

/* bits: FMO_SWITCH_3 */

#define FMO_BIT_BOUNDARY_FILL		0	/* bit 0: BOUNDARY_FILL */
#define FMO_BIT_SYMMETRY			1   /* bit 1: USE_SYMMETRY */
#define FMO_BIT_PERIODICITY         2   /* bit 2: USE_PERIODICITY */
#define FMO_BIT_HITMAP				3   /* bit 3: USE_HAMMAP */
#define FMO_BIT_BUDDHA_EDGES		4   /* bit 4: BUDDHA_EDGES */
#define FMO_BIT_BUDDHA_LEMNISCATES	5   /* bit 5: BUDDHA_LEMNISCATES */
#define FMO_BIT_BUDDHA_RANDOM       6   /* bit 6: BUDDHA_RANDOM */

/* integer data (ULONG) (max: 20) */

/* MULTIINT */
#define FMO_MULTIPOWER 				0

/* BUDDHA */
#define FMO_FM_REDMIN 				0	
#define FMO_FM_REDITER 				1
#define FMO_FM_GREENMIN 			2
#define FMO_FM_GREENITER 			3
#define FMO_FM_BLUEMIN 				4
#define FMO_FM_BLUEITER 			5
#define FMO_BUDDHA_MIN_ITERATIONS 	6

/* ANTIBUDDHA */
#define FMO_PRIME_RED				8
#define FMO_PRIME_GREEN				9
#define FMO_PRIME_BLUE				10
#define FMO_PRIME_DIVIDER			11

/* BOTH BUDDA & ANTIBUDDHA */
/* FMO_HITMAP_EXTENSION:
 * bits 0-8: HITMAP_EXTENSION 
 * bits: 9-15: HITMAP_SHIFT 
 * bit 16: HITMAP_DURING_CALCULATION
 * bit 17: HITMAP_AT_THE_END
 */
#define FMO_HITMAP_EXTENSION		12	
#define FMO_HITMAP_COLOR0           13
#define FMO_HITMAP_COLOR1			14
#define FMO_HITMAP_COLOR2           15
#define FMO_HITMAP_COLOR3			16

/* SMOOTH_COLORING */

#define FMO_SMOOTH_COLORING         7

/* ALL */

#define FMO_PROCESSOR_INFO			19

/* float data */

/* MULTIFLOAT */
#define FMO_MULTIPOWERFLOAT			0
#define FMO_BAILOUT_VALUE			1
#define FMO_PERIODICITY_EPSILON		2	/* for USE_PERIODICITY */
#define FMO_BUDDHA_RANDOMIZE_X		3
#define FMO_BUDDHA_RANDOMIZE_Y		4

/* ALL */

#define FMO_ELAPSEDTIME				19

#endif /* DATA_H */
