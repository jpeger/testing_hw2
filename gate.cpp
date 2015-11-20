
enum    //boolean value
{
    zero,
    one,
    unknown
};

int and2(int in1, int in2)
{
    int result;
    if(in1==zero || in2==zero){
        result = zero;
    }
    else if(in1==one && in2==one){
        result = one;
    }
    else{
        result = unknown;
    }
    return result;
}

int inv(int in)
{
    int result;
    if(in==zero){
        result = one;
    }
    else if(in==one){
        result = zero;
    }
    else{
        result = unknown;
    }
    return result;
}

int nand2(int in1, int in2)
{
    int result;
    result = inv(and2(in1,in2));
    return result;
}

int nand3(int in1, int in2, int in3)
{
    int result;
    result = and2(in1,in2);
    result = inv(and2(result,in3));
    return result;
}

int nand6(int in1, int in2, int in3, int in4, int in5, int in6)
{
    int tmp1,tmp2,tmp3,result;
    tmp1 = and2(in1,in2);
    tmp2 = and2(in3,in4);
    tmp3 = and2(in5,in5);
    result = nand3(tmp1,tmp2,tmp3);
    return result;
}






