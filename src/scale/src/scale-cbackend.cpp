#include <stdbool.h>
//#include "rely.h"

extern "C" float my_rand();

extern "C" float my_rand_float();

extern "C" float my_floor(float v);

extern "C" float my_ceil(float v);

extern "C" void scale_body(float factor, int * src, int sw, int sh, int * transformed, int dw, int dh,  int i, int j, float si, float sj)
{
  int previ = 0;
  int prevj = 0;
  int nexti = 0;
  int nextj = 0;
  int lr0 = 0;
  int lr1 = 0;
  int lr2 = 0;
  int ll0 = 0;
  int ll1 = 0;
  int ll2 = 0;
  int ul0 = 0;
  int ul1 = 0;
  int ul2 = 0;
  int ur0 = 0;
  int ur1 = 0;
  int ur2 = 0;
  {
    previ = ((int) (my_floor(si)));
    prevj = ((int) (my_floor(sj)));
    nexti = ((int) (my_ceil(si)));
    nextj = ((int) (my_ceil(sj)));
    if (sh<=previ) {
      {
        previ = (sh-1);
      
      }
    } else {
      {

      
      }    }
    if (sw<=prevj) {
      {
        prevj = (sw-1);
      
      }
    } else {
      {

      
      }    }
    if (sh<=nexti) {
      {
        nexti = (sh-1);
      
      }
    } else {
      {

      
      }    }
    if (sw<=nextj) {
      {
        nextj = (sw-1);
      
      }
    } else {
      {

      
      }    }
    if (previ==nexti) {
      {
        if (0==previ) {
          {
            if ((sw-1)==nexti) {
              {

              
              }
            } else {
              {
                nexti = (nexti+1);
              
              }            }
          
          }
        } else {
          {
            previ = (previ-1);
          
          }        }
      
      }
    } else {
      {

      
      }    }
    if (prevj==nextj) {
      {
        if (0==prevj) {
          {
            if ((sh-1)==nextj) {
              {

              
              }
            } else {
              {
                nextj = (nextj+1);
              
              }            }
          
          }
        } else {
          {
            prevj = (prevj-1);
          
          }        }
      
      }
    } else {
      {

      
      }    }

/*
    lr0 = RELY_ARRAY_READ_1D(src, (3*((previ*sw)+prevj)));
    lr1 = RELY_ARRAY_READ_1D(src, ((3*((previ*sw)+prevj))+1));
    lr2 = RELY_ARRAY_READ_1D(src, ((3*((previ*sw)+prevj))+2));
    ll0 = RELY_ARRAY_READ_1D(src, (3*((previ*sw)+nextj)));
    ll1 = RELY_ARRAY_READ_1D(src, ((3*((previ*sw)+nextj))+1));
    ll2 = RELY_ARRAY_READ_1D(src, ((3*((previ*sw)+nextj))+2));
    ur0 = RELY_ARRAY_READ_1D(src, (3*((nexti*sw)+prevj)));
    ur1 = RELY_ARRAY_READ_1D(src, ((3*((nexti*sw)+prevj))+1));
    ur2 = RELY_ARRAY_READ_1D(src, ((3*((nexti*sw)+prevj))+2));
    ul0 = RELY_ARRAY_READ_1D(src, (3*((nexti*sw)+nextj)));
    ul1 = RELY_ARRAY_READ_1D(src, ((3*((nexti*sw)+nextj))+1));
    ul2 = RELY_ARRAY_READ_1D(src, ((3*((nexti*sw)+nextj))+2));
*/
    
    lr0 = src [(3*((previ*sw)+prevj))];
    lr1 = src [((3*((previ*sw)+prevj))+1)];
    lr2 = src [((3*((previ*sw)+prevj))+2)];
    ll0 = src [(3*((previ*sw)+nextj))];
    ll1 = src [((3*((previ*sw)+nextj))+1)];
    ll2 = src [((3*((previ*sw)+nextj))+2)];
    ur0 = src [(3*((nexti*sw)+prevj))];
    ur1 = src [((3*((nexti*sw)+prevj))+1)];
    ur2 = src [((3*((nexti*sw)+prevj))+2)];
    ul0 = src [(3*((nexti*sw)+nextj))];
    ul1 = src [((3*((nexti*sw)+nextj))+1)];
    ul2 = src [((3*((nexti*sw)+nextj))+2)];
    
/*    
    RELY_ARRAY_WRITE_1D(transformed, (3*((i*dw)+j)), ((int) (((1.f/((float) (((nexti-previ)*(nextj-prevj)))))*(((((((float) (lr0))*(((float) (nextj))-sj))*(((float) (nexti))-si))+((((float) (ur0))*(((float) (nextj))-sj))*(si-((float) (previ)))))+((((float) (ll0))*(sj-((float) (prevj))))*(((float) (nexti))-si)))+((((float) (ul0))*(sj-((float) (prevj))))*(si-((float) (previ)))))))))
    RELY_ARRAY_WRITE_1D(transformed, ((3*((i*dw)+j))+1), ((int) (((1.f/((float) (((nexti-previ)*(nextj-prevj)))))*(((((((float) (lr1))*(((float) (nextj))-sj))*(((float) (nexti))-si))+((((float) (ur1))*(((float) (nextj))-sj))*(si-((float) (previ)))))+((((float) (ll1))*(sj-((float) (prevj))))*(((float) (nexti))-si)))+((((float) (ul1))*(sj-((float) (prevj))))*(si-((float) (previ)))))))))
    RELY_ARRAY_WRITE_1D(transformed, ((3*((i*dw)+j))+2), ((int) (((1.f/((float) (((nexti-previ)*(nextj-prevj)))))*(((((((float) (lr2))*(((float) (nextj))-sj))*(((float) (nexti))-si))+((((float) (ur2))*(((float) (nextj))-sj))*(si-((float) (previ)))))+((((float) (ll2))*(sj-((float) (prevj))))*(((float) (nexti))-si)))+((((float) (ul2))*(sj-((float) (prevj))))*(si-((float) (previ)))))))))
*/    
    
    transformed [(3*((i*dw)+j))] =  ((int) (((1.f/((float) (((nexti-previ)*(nextj-prevj)))))*(((((((float) (lr0))*(((float) (nextj))-sj))*(((float) (nexti))-si))+((((float) (ur0))*(((float) (nextj))-sj))*(si-((float) (previ)))))+((((float) (ll0))*(sj-((float) (prevj))))*(((float) (nexti))-si)))+((((float) (ul0))*(sj-((float) (prevj))))*(si-((float) (previ))))))));
    
    transformed [((3*((i*dw)+j))+1)] = ((int) (((1.f/((float) (((nexti-previ)*(nextj-prevj)))))*(((((((float) (lr1))*(((float) (nextj))-sj))*(((float) (nexti))-si))+((((float) (ur1))*(((float) (nextj))-sj))*(si-((float) (previ)))))+((((float) (ll1))*(sj-((float) (prevj))))*(((float) (nexti))-si)))+((((float) (ul1))*(sj-((float) (prevj))))*(si-((float) (previ))))))));
    
    transformed [((3*((i*dw)+j))+2)] = ((int) (((1.f/((float) (((nexti-previ)*(nextj-prevj)))))*(((((((float) (lr2))*(((float) (nextj))-sj))*(((float) (nexti))-si))+((((float) (ur2))*(((float) (nextj))-sj))*(si-((float) (previ)))))+((((float) (ll2))*(sj-((float) (prevj))))*(((float) (nexti))-si)))+((((float) (ul2))*(sj-((float) (prevj))))*(si-((float) (previ))))))));
    
    return;
  
  }
}

