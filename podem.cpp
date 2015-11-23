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

int arr_v[6]={unknown};
int arr_n[6]={unknown};
int cnt=0;

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
        int inverse(int, int, int);
        int faultsim(int);
        void display(int);

        int numgates;           // total number of gates
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
    else if(point==d && node[point]==unknown){
        obj_n = d;
        obj_v = one;
    }
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
                //cout << "backtrace::node:"<< obj_n<<endl;
                obj_v = inverse(idx,obj_n,obj_v);
                if(obj_n > f){
                    idx=0;
                    jdx=0;
                }
            }
        }
    }
}

int ckt::inverse(int gate, int point, int value)
{
    int result;
    if(gate>0 && gate<9){
        if(value==one){
            for(int idx=0; idx<MAXIN; idx++){
                if(free[fanin[gate][idx]]!=node[fanin[gate][idx]] || free[fanout[gate]]!=node[fanout[gate]]){
                    result = one;
                    //cout << 1<<endl;
                    return result;
                }
            }
            if(fanin[gate][0]==d){
                result = one;
                return result;
            }
            for(int idx=0; idx<MAXIN; idx++){
                for(int jdx=0; jdx<MAXIN; jdx++){
                    if( idx!=jdx && fanin[gate][idx]!=x && fanin[gate][jdx]!=x){
                        if(node[fanin[gate][idx]]==one || node[fanin[gate][jdx]]==one){
                            result = zero;
                            //cout << 2<<endl;
                            return result;
                        }
                    }
                }
            }
            result = one;
            //cout << 3<<endl;
            return result;
        }
        else if(value==zero){
            result = one;
            //cout << 4<<endl;
            return result;
        }
    }
    else if(gate>8 && gate<12){
        if(value==one){
            result = zero;
            //cout << 5<<endl;
            return result;
        }
        else if(value==zero){
            result = one;
            //cout << 6<<endl;
            return result;
        }
    }

}

int ckt::podem(int fault)
{
    if(faultsim(fault)==1){
        return 1;
    }
    else if(node[a]!=unknown && node[b]!=unknown && node[c]!=unknown
        && node[d]!=unknown && node[e]!=unknown && node[f]!=unknown){
        return 0;
    }
    /** algo start **/

    if(fault==1){
        objective(a,0);   //FA
    }
    else if(fault==2){
        objective(d,1);
    }
    backtrace();
    node[obj_n] = obj_v;
    free[obj_n] = obj_v;

    arr_v[cnt] = obj_v;
    arr_n[cnt] = obj_n;
    cnt++;
    if(podem(fault)==1) return 1;

    cnt--;
    obj_v = arr_v[cnt];
    obj_n = arr_n[cnt];
    if(obj_v==zero)
        obj_v=one;
    else if(obj_v==one)
        obj_v = zero;

    node[obj_n] = obj_v;
    free[obj_n] = obj_v;
    if(podem(fault)==1) return 1;

    /** algo end**/
    return 0;
}

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
//    printf("======fault sim======\n");
//    printf(" free ");
//    for(int idx=1; idx<NNODE; idx++){
//        printf("%c ",idx+96);
//    }
//    printf("\n      ");
//    for(int idx=1; idx<NNODE; idx++){
//        if(free[idx]==one){
//            printf("1 ");
//        }
//        else if(free[idx]==zero){
//            printf("0 ");
//        }
//        else if(free[idx]==unknown){
//            printf("X ");
//        }
//    }
//    printf("\n");
//    printf("fault ");
//    for(int idx=1; idx<NNODE; idx++){
//        printf("%c ",idx+96);
//    }
//    printf("\n      ");
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
    printf("input  ");
    for(int idx=1; idx<PI+1; idx++){
        printf("%c ",idx+96);
    }
    printf("\n       ");
    for(int idx=1; idx<PI+1; idx++){
        display(node[idx]);
        printf(" ");
    }
    printf("\n\noutput ");
    printf("z/zf\n");
    printf("       ");
    display(free[n]);
    printf("/");
    display(node[n]);
    printf("\n-------------------\n");

    if(node[n]!=free[n] && node[n]!=unknown && free[n]!=unknown){
        return 1;
    }
    else{
        return 0;
    }

}

void ckt::display(int value)
{
    if(value==zero){
        printf("0");
    }
    else if(value==one){
        printf("1");
    }
    else{
        printf("X");
    }
}

int main()
{
    ckt circuit = ckt();
    ckt circuit2 = ckt();

    ///a2 s-a-1
    printf("========a2 s-a-1========\n");
    circuit.podem(1);

    printf("========d2 s-a-0========\n");
    ///d2 s-a-0
    circuit2.podem(2);
    return 0;
}

