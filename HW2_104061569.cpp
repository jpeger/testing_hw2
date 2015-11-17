
class gateLevelCkt
{
    // circuit information
    int numgates;               // total number of gates (faulty included)
    unsigned int *value1;
    unsigned int *value2;       // good value of gate

    //for podem
    unsigned int *faultValue1;
    unsigned int *faultValue2;  // faulty value of gate
    int *faultGate;             // Gate of current fault
    int *REplaceType;           // tye of replaced gate
    bool *StuckAt;              // gate substitution fault modelled as stuck at fault
    int *OriginalGateType;      //type of gate before replacing
    int *D_frontier;            // Array storing D-Frontier

public:
    gateLevelCkt(char *);       // constructor

// simulator information
    int  retrieveEvent();
    void goodsim();             // logic sim (no faults inserted)

// Functions related to  PODEM
    void setupFaults();         // gate substitution faults modelled as stuck at faults and stored in an array
    void setDontCares();        // Applies Dont care to all gates
    void Podem(FILE *);      // calls PODEM for different faults
    bool backtrace(int,bool);   // Back-trace function
    bool podemREcursion(int,bool);  // execute PODEM to find vector
    bool CheckPathtoPO (int);       // check x-path from D-frontier to PO
    void faultsim();              // Faulty simulation
    void getobjective(int,bool);      // Get Objective function
    void getDFrontier(int);   // get d-frontier
    void replacegate(int,int);// Replaces the gate to insert fault in the circuit.
    bool checkInputsForExcitation(int);//to make sure that fault is getting excited
};

bool gateLevelCkt::backtrace(int gate, bool value);

//code for PODEM
void gateLevelCkt::Podem(FILE * vecFile)
{

    for( myCount=0;myCount<TotalFaults;myCount++)
    {
        //all values are st to dont cares before starting PODEM
        setDontCares();
        //Inserting the fault by replacing the gate
        replacegate(faultGate[myCount],REplaceType[myCount]);
        //PODEM algorithm for finding the inputs needed to detect the fault
        if((podemREcursion(faultGate[myCount],StuckAt[myCount])) == true)
        {
        }
        replacegate(faultGate[myCount],oldTYpe);
    }

}

bool gateLevelCkt::podemREcursion(int fgate, bool fvalue)
{
    //compute the D-frontier
    getDFrontier(fgate);
    isDetected=false;
    bool isPathAvailable=false;
    bool Cin;
    int successorGate;

    for(int i=0;i<numout;i++)
    {
        if()
        {
            isDetected=true;
            return true;
        }
    }
        //getobjective function for exciting and propogating the fault
        getobjective(fgate,fvalue);
        //backtrace to reach the PI
        Cin=backtrace(myobjective, objvalue);
        // Assign values to PI

        //Function to ensure that the fault is excited
        key=checkInputsForExcitation(fgate);

        //simulating the value found by backtrace method
        goodsim();
        //simulating with faulty value at gate
        faultsim();

        if(podemREcursion(fgate,fvalue) && key)
        {
            return true;
        }

        // wrong assignment at previous point so backtrack
        Cin=!Cin;

        goodsim();
        faultsim();

        if(podemREcursion(fgate,fvalue) && key)
        {
            return true;
        }

        return false;
}

int main()
{
    setupFaults();
    Podem();

    return 0;
}
