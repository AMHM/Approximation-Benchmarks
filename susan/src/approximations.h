//AMHM Start
#include "/home/amir/AMHM_Approximation_Benchmarks/Majid/susan/src/m5op.h"
//AMHM End

#define get_image_IN  // MJ : Approximable ==> Smoothing, Edges, Corner
#define setup_brightness_lut_BP  // MJ : Approximable ==> Smoothing, Edges, Corner (Very Interesting!! Negligible impact in all of them, good for demo!)
#define susan_smoothing_TMP_IMAGE // MJ : Approximable ==> Smoothing
#define susan_smoothing_DP  // MJ : Approximable ==> Smoothing, any Effect?
#define susan_corners_CGX  // MJ : Approximable ==> Corner, any Effect?
#define susan_corners_CGY  // MJ : Approximable ==> Corner, any Effect?
#define main_EDGES_R  // MJ : Approximable ==> Edges
//#define main_MID  // MJ : Critical ==> Loop for ever in susan_thin
#define main_CORNERS_R // MJ : Approximable ==> Corner

//AMHM Start
int reliability_level;
//AMHM End
