#include <stdio.h> 
#include <math.h> 
int main() 
{
    int m[101];
    m[1]=5/2*3;
    m[2]=pow(2 , pow(3 , 2 )  ) ;
    m[3]=11*3-pow(5 , 4 ) *2+pow(3 , 4 ) ;
    m[4]=2+m[2];
    m[5]=5-m[2]-6;
    printf("%d",84848492);
    printf("%d",m[5]);
    printf("%d",m[3]);
    goto label1; 
    m[5]=5-m[2]-6;
    printf("%d",124324);
    printf("%d",m[5]);
    label1: 
    m[6]=5-(m[5]-6);
    m[7]=3*11-4/pow(2 , 2 ) ;
    m[8]=2+m[7];
    m[9]=pow(3 , 2 ) -pow(2 , pow(2 , 2 )  ) -5;
    m[11]=5-m[11-6];
    m[12]=5-(4-6);
    m[13]=5-(pow(6 , 8 ) -m[11]);
    m[14]=5/2*3;
    printf("%d",m[13]);
    printf("%d",m[14]);
    m[pow(15 , 1 ) ]=2+m[13];
    m[16]=5-m[14-2]-6;
    label2: 
    m[17]=5-(m[16]-6);
    if(m[16])
        goto label4; goto label3; 
    m[15]=2+m[7];
    m[16]=5-m[4*2-2+7]-6;
    label3: 
    m[15]=5-(m[15]+m[7]-6);
    m[16]=5/2*3;
    goto label5; 
    label4: 
    m[25]=5-(m[4]-6);
    printf("%d",m[25]);
    label5: 
    printf("%d",m[25]);
    printf("%d",m[15]);
    printf("%d",m[16]);
    return 0;
}