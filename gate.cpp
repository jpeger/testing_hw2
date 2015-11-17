
int myobjective=0;
bool objvalue=0;

enum
{
    nand_g,         /* 0 */
    not_g          /* 1 */
};

bool ctrl_val(int gate_type)
{
    bool ctrl_v;
    if(gate_type==nand_g){
        ctrl_v = false;
    }
    else if(gate_type==not_g){
        ctrl_v =
    }

}

void gateLevelCkt::getobjective(int gate, bool value)
{
    objvalue=1;

    if()
    {
        myobjective=gate;
        objvalue=!value;
        return;
    }

    int nextGate=D_frontier[0];
    for(int i=0;i<fanin[nextGate];i++)
    {
        nextGate=D_frontier[0];
         = ctrl_val();
        if((value1[inlist[nextGate][i]] && !value2[inlist[nextGate][i]]) || (!value1[inlist[nextGate][i]] && value2[inlist[nextGate][i]]))
        {
            myobjective=inlist[nextGate][i];
        }
    }
    return;

}

