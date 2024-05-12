
/* defines the offsets (FMO ...) to store fractal calculation data 
   in NewMANDChunk and NewRGBMANDChunk 
*/

/* algorithms (ULONG) (max: 4) */
#define FMO_ALGORITHM_BASE	 	0
#define FMO_ALGORITHM_BUDDHA 	1
#define FMO_ALGORITHM_3	 		3
#define FMO_ALGORITHM_4 		4

/* switches (BOOL) (max: 4) */
#define FMO_JULIA 		0
#define FMO_ANTIBUDDHA 	1
#define FMO_SWITCH_3 	3
#define FMO_SWITCH_4 	4

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

/* SMOOTH_COLORING */

#define FMO_SMOOTH_COLORING         7

/* float data */

/* MULTIFLOAT */
#define FMO_MULTIPOWERFLOAT			0
#define FMO_BAILOUT_VALUE			1
