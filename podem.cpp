#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "gate.cpp"

#define MAXIN 6
#define MAXOUT 3
#define NGATE 11
#define PI 6
#define NNODE 18

using namespace std;

enum //node
{
    x,  //0
    a,  //1
    b,
    c,
    d,
    e,
    f,
    g,
    h,
    i,
    j,
    k,
    l,
    m,
    n,
    o,
    p,
    q   //17
};

enum    //gate
{
    gx, //0
    g1, //1
    g2,
    g3,
    g4,
    g5,
    g6,
    g7,
    g8, //8
    g9,
    g10,
    g11 //11
};

class ckt
{
    public:
        ckt();
        int podem(int);
        void objective(int, int);
        void backtrace();
        int inverse(int, int);
        //void findD();
        //int goodsim(int);
        int faultsim(int);

        int numgates;           // total number of gates
        int numpri;             // number of PIs
        int *node;              // fault value of node
        int *free;              // correct value of node
        int **fanin;            // number of fanin, fanouts
        int *fanout;
        int obj_n;
        int obj_v;
        int *D_ftr;
};

ckt::ckt()
{
    numgates = NGATE+1;
    numpri = PI;

    node = (int*)malloc(sizeof(int)*NNODE);
    for(int idx=0; idx<NNODE; idx++){
        node[idx] = unknown;
    }

    free = (int*)malloc(sizeof(int)*NNODE);
    for(int idx=0; idx<NNODE; idx++){
        free[idx] = unknown;
    }

    fanin = (int**)malloc(sizeof(int*)*numgates);
    for (int idx=0; idx<numgates; idx++){
        fanin[idx] = (int*)malloc(sizeof(int)*MAXIN);
        for(int jdx=0; jdx<MAXIN; jdx++){
            fanin[idx][jdx] = x;
        }
    }

    fanout = (int*)malloc(sizeof(int)*numgates);

    D_ftr = (int*)malloc(sizeof(int)*NNODE);
    for(int idx=0; idx<NNODE; idx++){
        D_ftr[idx]=0;
    }

    fanin[g1][0] = a;
    fanin[g1][1] = b;
    fanin[g1][2] = c;
    fanin[g2][0] = a;
    fanin[g2][1] = o;
    fanin[g3][0] = d;
    fanin[g3][1] = g;
    fanin[g4][0] = b;
    fanin[g4][1] = p;
    fanin[g5][0] = e;
    fanin[g5][1] = g;
    fanin[g6][0] = c;
    fanin[g6][1] = q;
    fanin[g7][0] = f;
    fanin[g7][1] = g;
    fanin[g8][0] = h;
    fanin[g8][1] = i;
    fanin[g8][2] = j;
    fanin[g8][3] = k;
    fanin[g8][4] = l;
    fanin[g8][5] = m;
    fanin[g9][0] = d;
    fanin[g10][0] = e;
    fanin[g11][0] = f;

    fanout[g1] = g;
    fanout[g2] = h;
    fanout[g3] = i;
    fanout[g4] = j;
    fanout[g5] = k;
    fanout[g6] = l;
    fanout[g7] = m;
    fanout[g8] = n;
    fanout[g9] = o;
    fanout[g10] = p;
    fanout[g11] = q;

}

void ckt::objective(int point, int value)
{
    if(point==a && node[point]==unknown){
        obj_n = a;
        obj_v = zero;
    }
//    else if(point==d){
//        node[d] = true;
//        return;
//    }
    else{
        for(int idx=1; idx<NNODE; idx++){
            if(D_ftr[idx]==1 && (node[idx]==unknown || free[idx]==unknown)){
                obj_n = idx;
                obj_v = one;
                break;
            }
        }
    }
}

void ckt::backtrace()
{
    for(int idx=0; idx<numgates; idx++){
        for(int jdx=0; jdx<MAXIN; jdx++){
            if(fanout[idx]==obj_n && node[fanin[idx][jdx]]==unknown){
                obj_n = fanin[idx][jdx];
                obj_v = inverse(idx,obj_v);
            }
        }
    }
}

int ckt::inverse(int gate, int value)
{
    int result;
    if(gate>0 && gate<9){
        if(value==one){
//            for(int idx=0; idx<MAXIN; idx++){
//                if(free[fanin[gate][idx]]==zero)
//                node[fanin[gate][idx]];
//                free[fanin[gate][idx]];
//                result = one;
//            }
//        }
//        else if(value==zero){
            result = one;
        }
    }
    else if(gate>8 && gate<12){
        if(value==one){
            result = zero;
        }
        else if(value==zero){
            result = one;
        }
    }
    return result;
}

int ckt::podem(int fault)
{
    if(faultsim(f)){
        return 1;
    }
    /** algo start **/
/**
    objective(a,0);   //FA
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    faultsim(1);

    printf("obj_n:%c\nobj_v:%d\n",obj_n+96,obj_v);
    printf("D_frontier: ");
    for(int idx=1; idx<NNODE; idx++){
        if(D_ftr[idx]==1){
            printf("%c, ",idx+96);
        }
    }
    printf("\n");
///
    objective(a,0);   //FA
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    faultsim(1);

    printf("obj_n:%c\nobj_v:%d\n",obj_n+96,obj_v);
    printf("D_frontier: ");
    for(int idx=1; idx<NNODE; idx++){
        if(D_ftr[idx]==1){
            printf("%c, ",idx+96);
        }
    }
    printf("\n");
///
    objective(a,0);   //FA
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    faultsim(1);

    printf("obj_n:%c\nobj_v:%d\n",obj_n+96,obj_v);
    printf("D_frontier: ");
    for(int idx=1; idx<NNODE; idx++){
        if(D_ftr[idx]==1){
            printf("%c, ",idx+96);
        }
    }
    printf("\n");
///
    objective(a,0);   //FA
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    faultsim(1);

    printf("obj_n:%c\nobj_v:%d\n",obj_n+96,obj_v);
    printf("D_frontier: ");
    for(int idx=1; idx<NNODE; idx++){
        if(D_ftr[idx]==1){
            printf("%c, ",idx+96);
        }
    }
    printf("\n");
///
    objective(a,0);   //FA
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    faultsim(1);

    printf("obj_n:%c\nobj_v:%d\n",obj_n+96,obj_v);
    printf("D_frontier: ");
    for(int idx=1; idx<NNODE; idx++){
        if(D_ftr[idx]==1){
            printf("%c, ",idx+96);
        }
    }
    printf("\n");
///
    return 0;
    /** algo end**/
    if(fault==1){
        objective(a,0);   //FA
    }
    else if(fault==2){

    }
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    if(podem()==1) return 1;

    if(obj_v==zero)
        obj_v=one;
    else if(obj_v==one)
        obj_v = zero;

    node[obj_n] = obj_v;
    if(podem()==1) return 1;

    return 0;
}

//int ckt::goodsim(int fault)
//{
//    ///fault sim
//    int tmp[NNODE]={unknown};
//    tmp[a] = node[a];
//    tmp[b] = node[b];
//    tmp[c] = node[c];
//    tmp[d] = node[d];
//    tmp[e] = node[e];
//    tmp[f] = node[f];
//    if(fault==1 || fault==3){
//        tmp[g] = nand3(one,node[b],node[c]);   //g1 a2 s-a-1
//    }
//    else{
//        tmp[g] = nand3(node[a],node[b],node[c]);   //g1
//    }
//    tmp[o] = inv(node[d]); //g9
//    tmp[p] = inv(node[e]); //g10
//    tmp[q] = inv(node[f]); //g11
//    tmp[h] = nand2(node[a],node[o]);   //g2
//    tmp[i] = nand2(node[d],node[g]);   //g3
//    tmp[j] = nand2(node[b],node[p]);   //g4
//    tmp[k] = nand2(node[e],node[g]);   //g5
//    tmp[l] = nand2(node[c],node[q]);   //g6
//    tmp[m] = nand2(node[f],node[g]);   //g7
//    tmp[n] = nand6(node[h],node[i],node[j],node[k],node[l],node[m]);   //g8
//
//    ///good sim
//    node[g] = nand3(node[a],node[b],node[c]);   //g1
//    node[o] = inv(node[d]); //g9
//    node[p] = inv(node[e]); //g10
//    node[q] = inv(node[f]); //g11
//    node[h] = nand2(node[a],node[o]);   //g2
//    node[i] = nand2(node[d],node[g]);   //g3
//    node[j] = nand2(node[b],node[p]);   //g4
//    node[k] = nand2(node[e],node[g]);   //g5
//    node[l] = nand2(node[c],node[q]);   //g6
//    node[m] = nand2(node[f],node[g]);   //g7
//    node[n] = nand6(node[h],node[i],node[j],node[k],node[l],node[m]);   //g8
//
//    for(int idx=1; idx<NNODE; idx++){
//        if(node[idx]!=tmp[idx] &&
//           (node[idx]==unknown || tmp[idx]==unknown)){
//            D_ftr[idx]=1;
//        }
//        else{
//            D_ftr[idx]=0;
//        }
//    }

    ///details
//    printf("======good sim======\nnode ");
//    for(int idx=1; idx<NNODE; idx++){
//        printf("%c ",idx+96);
//    }
//    printf("\n     ");
//    for(int idx=1; idx<NNODE; idx++){
//        if(node[idx]==one){
//            printf("1 ");
//        }
//        else if(node[idx]==zero){
//            printf("0 ");
//        }
//        else if(node[idx]==unknown){
//            printf("X ");
//        }
//    }
//    printf("\n");

    /// I/O only
//    printf("\nnode ");
//    for(int idx=1; idx<PI+1; idx++){
//        printf("%c ",idx+96);
//    }
//    printf("z ");
//    printf("\n     ");
//    for(int idx=1; idx<PI+1; idx++){
//        if(node[idx]==one){
//            printf("1 ");
//        }
//        else if(node[idx]==zero){
//            printf("0 ");
//        }
//        else if(node[idx]==unknown){
//            printf("X ");
//        }
//    }
//    if(node[n]==one){
//        printf("1 ");
//    }
//    else if(node[n]==zero){
//        printf("0 ");
//    }
//    else if(node[n]==unknown){
//        printf("X ");
//    }
//    printf("\n");
//
//    return node[n];
//}

int ckt::faultsim(int fault)
{
    ///good sim
    free[g] = nand3(free[a],free[b],free[c]);   //g1
    free[o] = inv(free[d]); //g9
    free[p] = inv(free[e]); //g10
    free[q] = inv(free[f]); //g11
    free[h] = nand2(free[a],free[o]);   //g2
    free[i] = nand2(free[d],free[g]);   //g3
    free[j] = nand2(free[b],free[p]);   //g4
    free[k] = nand2(free[e],free[g]);   //g5
    free[l] = nand2(free[c],free[q]);   //g6
    free[m] = nand2(free[f],free[g]);   //g7
    free[n] = nand6(free[h],free[i],free[j],free[k],free[l],free[m]);   //g8

    ///fault sim
    if(fault==1 || fault==3){
        node[g] = nand3(one,node[b],node[c]);   //g1 a2 s-a-1
    }
    else{
        node[g] = nand3(node[a],node[b],node[c]);   //g1
    }
    node[o] = inv(node[d]); //g9
    node[p] = inv(node[e]); //g10
    node[q] = inv(node[f]); //g11
    node[h] = nand2(node[a],node[o]);   //g2
    if(fault==2 || fault==3){
        node[i] = nand2(zero,node[g]);      //g3 d2 s-a-0
    }
    else{
        node[i] = nand2(node[d],node[g]);      //g3
    }
    node[j] = nand2(node[b],node[p]);   //g4
    node[k] = nand2(node[e],node[g]);   //g5
    node[l] = nand2(node[c],node[q]);   //g6
    node[m] = nand2(node[f],node[g]);   //g7
    node[n] = nand6(node[h],node[i],node[j],node[k],node[l],node[m]);   //g8

    for(int idx=1; idx<NNODE; idx++){
        if(node[idx]!=free[idx] && (node[idx]==unknown || free[idx]==unknown)){
            D_ftr[idx]=1;
        }
        else{
            D_ftr[idx]=0;
        }
    }

    ///details
    printf("======fault sim======\nfault ");
    for(int idx=1; idx<NNODE; idx++){
        printf("%c ",idx+96);
    }
    printf("\n      ");
    for(int idx=1; idx<NNODE; idx++){
        if(node[idx]==one){
            printf("1 ");
        }
        else if(node[idx]==zero){
            printf("0 ");
        }
        else if(node[idx]==unknown){
            printf("X ");
        }
    }
    printf("\n");
    printf(" free ");
    for(int idx=1; idx<NNODE; idx++){
        printf("%c ",idx+96);
    }
    printf("\n      ");
    for(int idx=1; idx<NNODE; idx++){
        if(free[idx]==one){
            printf("1 ");
        }
        else if(free[idx]==zero){
            printf("0 ");
        }
        else if(free[idx]==unknown){
            printf("X ");
        }
    }
    printf("\n");
    /// I/O only
//    printf("\nnode ");
//    for(int idx=1; idx<PI+1; idx++){
//        printf("%c ",idx+96);
//    }
//    printf("z ");
//    printf("\n     ");
//    for(int idx=1; idx<PI+1; idx++){
//        if(node[idx]==one){
//            printf("1 ");
//        }
//        else if(node[idx]==zero){
//            printf("0 ");
//        }
//        else if(node[idx]==unknown){
//            printf("X ");
//        }
//    }
//    if(node[n]==one){
//            printf("1 ");
//    }
//    else if(node[n]==zero){
//        printf("0 ");
//    }
//    else if(node[n]==unknown){
//        printf("X ");
//    }
//    printf("\n");
    if(node[n]!=free[n] && node[n]!=unknown && free[n]!=unknown){
        return 1;
    }
    else{
        return 0;
    }

}

int main()
{
    ckt circuit = ckt();

    ///a2 s-a-1
    circuit.podem();

//    ///d2 s-a-0
//    circuit.goodsim();
//    circuit.faultsim(2);
    //circuit.podem();
    return 0;
}
