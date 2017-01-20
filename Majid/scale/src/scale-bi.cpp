#include "scale.h"
#include "math.h"
#define IDX(i,j,w) (i*w+j)
#define R(k) (3*k)
#define G(k) (3*k+1)
#define B(k) (3*k+2)
#define BILINEAR(ci, cj, nexti, nextj, previ, prevj, c_ul,c_ur,c_ll,c_lr) (1.0/((nexti-previ)*(nextj-prevj))*(c_lr*(nextj-cj)*(nexti-ci)+c_ur*(nextj-cj)*(ci-previ)+c_ll*(cj-prevj)*(nexti-ci)+c_ul*(cj-prevj)*(ci-previ)))

extern "C" float my_floor(float);
extern "C" float my_ceil(float);


extern "C" void scale_body(float factor, int * src, int sw, int sh, int * dest, int dw, int dh,  int i, int j, float si, float sj);


extern "C" void scale(float factor, int *src, int sw, int sh, int *dest, int dw, int dh){
	int i,j,k=0;
	float delta_i=1/factor, delta_j=1/factor; 
	float si, sj;
	
	if(factor >= 1){
		for(i=0; i < dh; i++, si+=delta_i){
			for(j=0, sj=0; j < dw; j++, sj+=delta_j){
			  scale_body (factor, src, sw, sh, dest, dw, dh, i, j, si, sj);
			}
		}
	}
	else{
	  
	  for(i=0, si=0; i < dh; i++, si+=delta_i){
	    for(j=0, sj=0; j < dw; j++, sj+=delta_j){
	      int previ, prevj, nexti, nextj;
	      float factor;
	      int * ll, * lr, * ul, * ur;
	      //corners
	      previ = my_floor(si); prevj = my_floor(sj);
	      nexti = my_ceil(si); nextj = my_ceil(sj);
	      lr = &src[R(IDX(previ, prevj,sw))];
	      ll = &src[R(IDX(previ, nextj,sw))];
	      ur = &src[R(IDX(nexti, prevj,sw))];
	      ul = &src[R(IDX(nexti, nextj,sw))];
	      dest[R(IDX(i,j,dw))]= ul[0];
	      dest[G(IDX(i,j,dw))]= ul[1];
	      dest[B(IDX(i,j,dw))]= ul[2];
	      
	    }
	  }

	}
}

