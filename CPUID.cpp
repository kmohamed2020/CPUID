#include<stdio.h>
#include <stdint.h>
#include <string>
#include <algorithm>
#include <math.h>

#define BIT(x) (1ULL << (x))

/*
    Author: Khaled Fawzy
*/

int a[10];


/*
    Function to perform AND function with the targeted BIT in specific REG to detect if specific feature is supported
    reg     ->  the REG (EBX, ECX, EDX)
    val     ->  the targeted BIT index
    feature ->  the feature string name
*/
void featureTest( int reg, int  val , const char* feature)
{

    //the below block of code, is just added to capitalize the feature name, as it's added after the whole program was written - for saving time-
    std::string featureStr = feature;
    std::for_each(featureStr.begin(), featureStr.end(), [](char & c) {
        c = ::toupper(c);
    });

    //Checking if the passed value ANDed with the targeted feature bit is equal to 1 -> Feature is supported, else -> not supported.
    if ((reg & BIT(val)) == 0) printf("\t\t%s not supported\n",featureStr.c_str());
    else printf("\t\t%s supported\n",featureStr.c_str());

}

/*
Function to run CPID instruction using the global array set values which maps to the values of the registers 
    a[0]    ->  EAX
    a[1]    ->  EBX
    a[2]    ->  ECX
    a[3]    ->  EDX
    bool ecx represent if the ECX value will be sent to the inline Assembly or not
*/
void runCPUID(  bool ecx)
{
    if(ecx)// Case of ECX will be sent to the inline Assembly
    {
        asm ("cpuid\n\t"
        : "=a" (a[0]), "=b" (a[1]), "=c" (a[2]), "=d" (a[3])
        : "0" (a[0]), "2" (a[2]));
    }
    else // Case of only using EAX as input to the inline Assembly
    {

        asm ("cpuid\n\t"
        : "=a" (a[0]), "=b" (a[1]), "=c" (a[2]), "=d" (a[3])
        : "0" (a[0]));
    }
}

/*
    Function to convert Decimal value into Binary represented array, Maily used to convert the EAX returned value into binary represetned array.
*/
void decToBinary(int n, int* binArr)
{
    // counter for binary array
    int i = 0;
    while (n > 0) {

        // storing remainder in binary array
        binArr[i] = n % 2;
        n = n / 2;
        i++;
    }
}

/*
    Function to Convert Binary slice of binary array into Decimal and return the value
*/
int BinArrToDec(int* binArr,int start, int end)
{
    int decValue=0; // To hold the converted decimal value.
    int j = 0; // counter for the binary to decimal transformation.
    
    // Lopping over slice of the passed Binary array to converted into decimal.
    for(int i = start ; i <= end ; i++)
    {
        //Converting Binary bit to the equivilant decimal value and appended to the decimal container var.
        if (binArr[i] == 1) decValue+=1*pow(2,j);
        j++;
    }
    return decValue;
}

/*
    Function to parse the values in a[0] -> EAX , after calling CPUID with setting EAX to 0x1 to get the Processor Info
*/
void printProcessorVersionInformation()
{
    // a[0] -> EAX
    int eax = a[0]; // for ease of tracking.
    int binArr[32]; // to hold the Binary representation of EAX reg.
    int step = -1;  // to hold the parsed Stepping ID value.
    int model = -1; // to hold the parsed Model value.
    int familyID = -1; // to hold the parsed Family ID parsed value.
    int processorType = -1; // to hold the parsed Processor Type value.

    const char* processorType_0 = "Original equipment manufacturer (OEM) Processor.";
    const char* processorType_1 = "Intel Overdrive Processor.";
    const char* processorType_2 ="Dual processor (not applicable to Intel486 processors.";

    // Getting the Binary representation of EAX
    decToBinary(eax, binArr);

    // Getting the Stepping ID value by Converting the Binary bits 0-4 from EAX
    step = BinArrToDec(binArr,0,3);
    printf("\tStepping ID: %d\n",step);

    // Getting the Model value by Converting the Binary bits 4-7 from EAX
    model = BinArrToDec(binArr,4,7);
    printf("\tModel: %d\n",model);

    // Getting the familyID value by Converting the Binary bits 8-11 from EAX
    familyID = BinArrToDec(binArr,8,11);
    printf("\tFamily ID: %d\n",familyID);

    // Getting the familyID value by Converting the Binary bits 12-13 from EAX
    processorType = BinArrToDec(binArr,12,13);

    switch(processorType)
    {
        case 0:
            printf("\tProcessor Type: %s\n",processorType_0);
            break;
        case 1:
            printf("\tProcessor Type: %s\n",processorType_1);
            break;
        case 2:
            printf("\tProcessor Type: %s\n",processorType_2);
            break;
    }
    

}

/*
    Function to List CPUID features by calling runCPUID() function with different settings for 
*/
void getCPUIDFeatureList(){

    
    a[0]=0x01;
    runCPUID(false);

    printf("\nEAX=1, Processor Info and Feature Bits:\n");

    printf("\nProcessor Information:\n");
    printProcessorVersionInformation();
    
    printf("\nECX Flags, Feature Information:\n");
    featureTest(a[3], 0	,"fpu");
    featureTest(a[3], 1	,"vme");
    featureTest(a[3], 2	,"de");
    featureTest(a[3], 3	,"pse");
    featureTest(a[3], 4	,"tsc");
    featureTest(a[3], 5	,"msr");
    featureTest(a[3], 6	,"pae");
    featureTest(a[3], 7	,"mce");
    featureTest(a[3], 8	,"cx8");
    featureTest(a[3], 9	,"apic");
    featureTest(a[3], 11	,"sep");
    featureTest(a[3], 12	,"mtrr");
    featureTest(a[3], 13	,"peg");
    featureTest(a[3], 14	,"mca");
    featureTest(a[3], 15	,"cmov");
    featureTest(a[3], 16	,"pat");
    featureTest(a[3], 17	,"pse-36");
    featureTest(a[3], 18	,"psn");
    featureTest(a[3], 19	,"clfsh");
    featureTest(a[3], 21	,"ds");
    featureTest(a[3], 22	,"acpi");
    featureTest(a[3], 23	,"mmx");
    featureTest(a[3], 24	,"fxsr");
    featureTest(a[3], 25	,"sse");
    featureTest(a[3], 26	,"sse2");
    featureTest(a[3], 27	,"ss");
    featureTest(a[3], 28	,"htt");
    featureTest(a[3], 29	,"tm");
    featureTest(a[3], 30	,"ia64");
    featureTest(a[3], 31	,"pbe");

    printf("\nEDX Flags, Feature Information:\n");
    featureTest(a[2], 0	,"sse3");
    featureTest(a[2], 1	,"pclmulqdq");
    featureTest(a[2], 2	,"dtes64");
    featureTest(a[2], 3	,"monitor");
    featureTest(a[2], 4	,"ds-cpl");
    featureTest(a[2], 5	,"vmx");
    featureTest(a[2], 6	,"smx");
    featureTest(a[2], 7	,"est");
    featureTest(a[2], 8	,"tm2");
    featureTest(a[2], 9	,"ssse3");
    featureTest(a[2], 10	,"cnxt-id");
    featureTest(a[2], 11	,"sdbg");
    featureTest(a[2], 12	,"fma");
    featureTest(a[2], 13	,"cx16");
    featureTest(a[2], 14	,"xtpr");
    featureTest(a[2], 15	,"pdcm");
    featureTest(a[2], 17	,"pcid");
    featureTest(a[2], 18	,"dca");
    featureTest(a[2], 19	,"sse4.1");
    featureTest(a[2], 20	,"sse4.2");
    featureTest(a[2], 21	,"x2apic");
    featureTest(a[2], 22	,"movbe");
    featureTest(a[2], 23	,"popcnt");
    featureTest(a[2], 24	,"tsc-deadline");
    featureTest(a[2], 25	,"aes");
    featureTest(a[2], 26	,"xsave");
    featureTest(a[2], 27	,"osxsave");
    featureTest(a[2], 28	,"avx");
    featureTest(a[2], 29	,"f16c");
    featureTest(a[2], 30	,"rdrnd");
    featureTest(a[2], 31	,"hypervisor");


    printf("\nEAX=7 & ECX=0x0, Extended Feature Information:\n");
    a[0]=0x07;
    a[2]=0x0;
    runCPUID(true);
    printf("\n\tEBX Flags, CPUID Features Information:\n");
    featureTest(a[1],0,"fsgsbase");
    featureTest(a[1],2,"sgx");
    featureTest(a[1],3,"bmi1");
    featureTest(a[1],4,"hle");
    featureTest(a[1],5,"avx2");
    featureTest(a[1],7,"smep");
    featureTest(a[1],8,"bmi2");
    featureTest(a[1],9,"erms");
    featureTest(a[1],10,"invpcid");
    featureTest(a[1],11,"rtm");
    featureTest(a[1],12,"pqm");
    featureTest(a[1],14,"mpx");
    featureTest(a[1],15,"pqe");
    featureTest(a[1],16,"avx512_f");
    featureTest(a[1],17,"avx512_dq");
    featureTest(a[1],18,"rdseed");
    featureTest(a[1],19,"adx");
    featureTest(a[1],20,"smap");
    featureTest(a[1],21,"avx512_ifma");
    featureTest(a[1],22,"pcommit");
    featureTest(a[1],23,"clflushopt");
    featureTest(a[1],24,"clwb");
    featureTest(a[1],25,"intel_pt");
    featureTest(a[1],26,"avx512_pf");
    featureTest(a[1],27,"avx512_er");
    featureTest(a[1],28,"avx512_cd");
    featureTest(a[1],29,"sha");
    featureTest(a[1],30,"avx512_bw");
    featureTest(a[1],31,"avx512_vl");

    printf("\n\tECX Flags, CPUID Features Information:\n");
    featureTest(a[2],0,"prefetchwt1");
    featureTest(a[2],1,"avx512_vbmi");
    featureTest(a[2],2,"umip");
    featureTest(a[2],3,"pku");
    featureTest(a[2],4,"ospke");
    featureTest(a[2],5,"waitpkg");
    featureTest(a[2],6,"avx512_vbmi2");
    featureTest(a[2],7,"cet_ss");
    featureTest(a[2],8,"gfni");
    featureTest(a[2],9,"vaes");
    featureTest(a[2],10,"vpclmulqdq");
    featureTest(a[2],11,"avx512_vnni");
    featureTest(a[2],12,"avx512_bitalg");
    featureTest(a[2],13,"TME_EN");
    featureTest(a[2],14,"avx512_vpopcntdq");
    featureTest(a[2],22,"rdpid");
    featureTest(a[2],23,"KL");
    featureTest(a[2],25,"cldemote");
    featureTest(a[2],27,"MOVDIRI");
    featureTest(a[2],28,"MOVDIR64B");
    featureTest(a[2],29,"ENQCMD");
    featureTest(a[2],30,"sgx_lc");
    featureTest(a[2],31,"pks");

    printf("\n\tEDX Flags, CPUID Features Information:\n");
    featureTest(a[3],2,"avx512_4vnniw");
    featureTest(a[3],3,"avx512_4fmaps");
    featureTest(a[3],4,"fsrm");
    featureTest(a[3],8,"avx512_vp2intersect");
    featureTest(a[3],9,"SRBDS_CTRL");
    featureTest(a[3],10,"md_clear");
    featureTest(a[3],13,"tsx_force_abort");
    featureTest(a[3],14,"SERIALIZE");
    featureTest(a[3],15,"Hybrid");
    featureTest(a[3],16,"TSXLDTRK");
    featureTest(a[3],18,"pconfig");
    featureTest(a[3],19,"lbr");
    featureTest(a[3],20,"cet_ibt");
    featureTest(a[3],22,"amx-bf16");
    featureTest(a[3],23,"AVX512_FP16");
    featureTest(a[3],24,"amx-tile");
    featureTest(a[3],25,"amx-int8");
    featureTest(a[3],26,"IBRS_IBPB / spec_ctrl");
    featureTest(a[3],27,"stibp");
    featureTest(a[3],28,"L1D_FLUSH");
    featureTest(a[3],29,"IA32_ARCH_CAPABILITIES");
    featureTest(a[3],30,"IA32_CORE_CAPABILITIES");
    featureTest(a[3],31,"ssbd");
 
    printf("\nEAX=80000001h: Extended Processor Info and Feature Bits:\n");
    a[0]=0x80000001;
    runCPUID(false);
    printf("\n\tEAX=80000001h CPUID feature bits:\n");
    
    featureTest(a[3],0,"fpu");
    featureTest(a[3],1,"vme");
    featureTest(a[3],2,"de");
    featureTest(a[3],3,"pse");
    featureTest(a[3],4,"tsc");
    featureTest(a[3],5,"msr");
    featureTest(a[3],6,"pae");
    featureTest(a[3],7,"mce");
    featureTest(a[3],8,"cx8");
    featureTest(a[3],9,"apic");
    featureTest(a[3],11,"syscall");
    featureTest(a[3],12,"mtrr");
    featureTest(a[3],13,"pge");
    featureTest(a[3],14,"mca");
    featureTest(a[3],15,"cmov");
    featureTest(a[3],16,"pat");
    featureTest(a[3],17,"pse36");
    featureTest(a[3],19,"mp");
    featureTest(a[3],20,"nx");
    featureTest(a[3],22,"mmxext");
    featureTest(a[3],23,"mmx");
    featureTest(a[3],24,"fxsr");
    featureTest(a[3],25,"fxsr_opt");
    featureTest(a[3],26,"pdpe1gb");
    featureTest(a[3],27,"rdtscp");
    featureTest(a[3],29,"lm");
    featureTest(a[3],30,"3dnowext");
    featureTest(a[3],31,"3dnow");

    featureTest(a[2],0,"lahf_lm");
    featureTest(a[2],1,"cmp_legacy");
    featureTest(a[2],2,"svm");
    featureTest(a[2],3,"extapic");
    featureTest(a[2],4,"cr8_legacy");
    featureTest(a[2],5,"abm");
    featureTest(a[2],6,"sse4a");
    featureTest(a[2],7,"misalignsse");
    featureTest(a[2],8,"3dnowprefetch");
    featureTest(a[2],9,"osvw");
    featureTest(a[2],10,"ibs");
    featureTest(a[2],11,"xop");
    featureTest(a[2],12,"skinit");
    featureTest(a[2],13,"wdt");
    featureTest(a[2],15,"lwp");
    featureTest(a[2],16,"fma4");
    featureTest(a[2],17,"tce");
    featureTest(a[2],19,"nodeid_msr");
    featureTest(a[2],21,"tbm");
    featureTest(a[2],22,"topoext");
    featureTest(a[2],23,"perfctr_core");
    featureTest(a[2],24,"perfctr_nb");
    featureTest(a[2],26,"dbx");
    featureTest(a[2],27,"perftsc");
    featureTest(a[2],28,"pcx_l2i");



}
/*
    Function to print the Brand String by calling runCPID() function 3 times  with setting the global var a[0] --> EAX with values 0x80000002,0x80000003,0x80000004
*/
void printBrandString()
{
    
    printf("Brand string is ");
    
    a[0] = 0x80000002;
    runCPUID(false);
    printf("%s", &a[0]);

    a[0] = 0x80000003;
    runCPUID(false);
    printf("%s", &a[0]);

    a[0] = 0x80000004;
    runCPUID(false);
    printf("%s", &a[0]);

}

/*
    Function to Print the Processor Manfucture Information, it's called from getCPUIDFeatureList() after calling runCPUID() with a[0]->EAX is set to 0x01
*/
void printProcessorManf()
{
    a[0]=0;
    runCPUID(false);
    // a[0] -> EAX
    // a[1] -> EBX
    // a[2] -> ECX
    // a[3] -> EDX

    int x[3]; // New array to hold the value of the below REG in that order: EBX->EDX->ECX
    x[0]=a[1];
    x[1]=a[3];
    x[2]=a[2];

    printf ("\nThe Processor's Manufacture: %s\n", &x);
}

int main(){
    printProcessorManf();
    printBrandString();
    getCPUIDFeatureList();
	return  0;
}

