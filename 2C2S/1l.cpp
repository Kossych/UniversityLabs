#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <type_traits> 
#define BASE_SIZE (sizeof(BASE)*8)
#define MAX_BASE ((unsigned long long)1<<BASE_SIZE)

using namespace std;
typedef unsigned char BASE;
typedef conditional<BASE_SIZE<32,conditional<BASE_SIZE<16,unsigned short, unsigned int >::type, unsigned long long>::type TMP; 
//typedef unsigned long long TMP;

class LargeNum{
    BASE *coef;
    int maxLEN;
    int len;
    public:
        LargeNum(int Len=1, bool k=0);
        LargeNum(const LargeNum &);
        ~LargeNum(){if(coef!=NULL) delete[]coef;}
        LargeNum& operator = (const LargeNum &);

        void HexInput(const char *);
        void HexOutput();

        bool operator == (const LargeNum&);
        bool operator != (const LargeNum&);
        bool operator >= (const LargeNum&);
        bool operator <= (const LargeNum&);
        bool operator > (const LargeNum&);
        bool operator < (const LargeNum&);

        LargeNum operator + (const LargeNum &);
        LargeNum& operator += (const LargeNum &);
        LargeNum operator + (const BASE);
        LargeNum& operator += (const BASE);

        LargeNum operator - (const LargeNum &);
        LargeNum& operator -= (const LargeNum &);

        LargeNum operator * (const LargeNum &);
        LargeNum& operator *= (const LargeNum &);
        LargeNum operator * (const BASE);
        LargeNum& operator *= (const BASE);

        LargeNum operator /(const BASE);
        LargeNum& operator /=(const BASE);
        LargeNum operator /(const LargeNum &);
        LargeNum& operator /=(const LargeNum &);
        BASE operator %(const BASE);
        //friend ostream& operator << (ostream &, LargeNum&);
        //friend istream& operator >> (istream &, LargeNum&);
        void DecInput(const char *);
        void DecOutput();
        void PrintCoef();
    private:
        void ExpMaxLen(int);
        void NormLen();
};

    LargeNum::LargeNum(int Len, bool k){
        maxLEN=Len;
        coef=new BASE[maxLEN];
        if(k==0){
            len=1;
            for(int i=0;i<Len;i++) coef[i]=0;
        }
        else if(k==1){
            len=Len;
            int a;
            for(int i=0;i<Len;i++) 
            for(int j=0;j<BASE_SIZE;j=j+8){
                a=rand()%256;
                coef[i]=coef[i]|(a<<j);
            }
            this->NormLen();
        }

    }

    LargeNum::LargeNum(const LargeNum &LN){
        if(&LN!=this){
            maxLEN=LN.maxLEN;
            len=LN.len;
            coef=new BASE[maxLEN];
            for(int i=0;i<maxLEN;i++) coef[i]=LN.coef[i];
        }

    }
    
    LargeNum& LargeNum::operator = (const LargeNum &LN){
        if(&LN!=this){
            maxLEN=LN.maxLEN;
            len=LN.len;
            delete[]coef;
            coef=new BASE[maxLEN];
            for(int i=0;i<maxLEN;i++) coef[i]=LN.coef[i];
            return *this;
        }
        return *this;
    }

    void LargeNum::HexInput(const char *S){
        int BS=BASE_SIZE/4;
        int Slen=strlen(S);
        len=(Slen-1)/BS+1;
        maxLEN=len+1;
        delete[]coef;
        coef=new BASE[maxLEN];
        for(int i=0;i<maxLEN;i++) coef[i]=0;
        for(int i=Slen-1;i>=0;i--){
            char tmp=0;
            int iRev=Slen-1-i;
            if((S[i]>='0')&&(S[i]<='9')) tmp=S[i]-'0';
            else if((S[i]>='a')&&(S[i]<='f')) tmp=S[i]-'a'+10;
            else if((S[i]>='A')&&(S[i]<='F')) tmp=S[i]-'A'+10;
            coef[iRev/BS]=coef[iRev/BS]|(tmp<<((iRev%BS)*4)); 
        }
        this->NormLen();
    }

    void LargeNum::HexOutput(){
        bool k=true;
        for(int i=len-1;i>=0;i--){
            for(int j=BASE_SIZE-4;j>=0;j=j-4){
                char tmp=((coef[i]&(0xF<<j))>>j);
                if(k&&(tmp==0)) continue;
                else k=false;
                if(tmp<10){
                    tmp=tmp+'0';
                    cout<<tmp;
                }
                else if(tmp<16){
                    tmp=tmp+'a'-10;
                    cout<<tmp;
                }
            }
        }
        if(k) cout<<0;
    }

    bool LargeNum::operator == (const LargeNum &LN){
        if(&LN==this) return 1;
        if(len!=LN.len) return 0;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else return false;
        }
        return true;
    }

    bool LargeNum::operator != (const LargeNum &LN){
        if(&LN==this) return 0;
        if(len!=LN.len) return 1;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else return true;
        }
        return false;
    }

    bool LargeNum::operator >= (const LargeNum &LN){
        if(&LN==this) return 1;
        if(len>LN.len) return 1;
        if(len<LN.len) return 0;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]>LN.coef[i]) return 1;
            else return 0;
        }
        return true;
    }

    bool LargeNum::operator <= (const LargeNum &LN){
        if(&LN==this) return 1;
        if(len>LN.len) return 0;
        if(len<LN.len) return 1;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]<LN.coef[i]) return 1;
            else return 0;
        }
        return true;
    }

    bool LargeNum::operator > (const LargeNum &LN){
        if(&LN==this) return 0;
        if(len>LN.len) return 1;
        if(len<LN.len) return 0;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]>LN.coef[i]) return true;
            else return false;
        }
        return false;
    }

    bool LargeNum::operator < (const LargeNum &LN){
        if(&LN==this) return 0;
        if(len>LN.len) return 0;
        if(len<LN.len) return 1;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else if(coef[i]<LN.coef[i]) return true;
            else return false;
        }
        return false;
    }

    LargeNum LargeNum::operator+ (const LargeNum &LN){
        int min,max;
        BASE *MaxCoef;
        bool k=0;
        TMP tmp;
        if(len>LN.len) {min=LN.len; max=len+1; MaxCoef=coef;}
        else {min=len; max=LN.len+1; MaxCoef=LN.coef;}
        LargeNum A(max,0);
        for(int i=0;i<min;i++){
            tmp=LN.coef[i]+coef[i]+k;
            A.coef[i]=(BASE)tmp;
            k=(bool)(tmp>>BASE_SIZE);
        }
        for(int i=min;i<max-1;i++){
            tmp=MaxCoef[i]+k;
            A.coef[i]=(BASE)tmp;
            k=(bool)(tmp>>BASE_SIZE);
        }
        A.coef[max-1]=k;
        A.NormLen();
        return A;
    }

    LargeNum LargeNum::operator+ (const BASE num){
        TMP tmp=0;
        bool k=0;
        int Len=maxLEN;
        if(len+1>maxLEN) Len=len+1;
        LargeNum A(Len,0);
        tmp=coef[0]+num+k;
        k=(bool)(tmp>>BASE_SIZE);
        A.coef[0]=tmp;
        for(int i=1;i<len;i++){
            tmp=coef[i]+k;
            k=bool(tmp>>BASE_SIZE);
            A.coef[i]=tmp;
        }
        A.coef[len]=k;
        A.NormLen();
        return A;
    }

    LargeNum& LargeNum::operator+= (const BASE num){
        *this=*this+num;
        return *this;
    }

    LargeNum& LargeNum::operator+= (const LargeNum &LN){
        *this=*this+LN;
        return *this;
    }

    LargeNum LargeNum::operator - (const LargeNum &LN){
        if(*this<LN){cout<<"error\n"; return *this;}
        TMP tmp;
        bool k=0;
        LargeNum A(*this);
        for(int i=0;i<LN.len;i++){
            tmp=A.coef[i]+MAX_BASE-LN.coef[i]-k;
            A.coef[i]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        for(int i=LN.len;k&&i<=A.len;i++){
            tmp=A.coef[i]+MAX_BASE-k;
            A.coef[i]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        A.NormLen();
        return A;
    }

    LargeNum& LargeNum::operator -= (const LargeNum &LN){
        if(*this<LN){cout<<"error\n"; return *this;}
        int min;
        bool k=0;
        TMP tmp;
        for(int i=0;i<LN.len;i++){
            tmp=coef[i]+MAX_BASE-LN.coef[i]-k;
            k=!((tmp)>>BASE_SIZE);
            coef[i]=(BASE)tmp;
        }
        for(int i=LN.len;k&&i<=len;i++){
            tmp=coef[i]+MAX_BASE-k;
            k=!((tmp)>>BASE_SIZE);
            coef[i]=(BASE)tmp;
        }
        this->NormLen();
        return *this;
    }

    LargeNum LargeNum::operator* (const LargeNum &LN){
        TMP tmp;
        TMP k;
        LargeNum A(len+LN.len,0);
        for(int i=0;i<len;i++){
            k=0;
            for(int j=0;j<LN.len;j++){
                tmp=(TMP)LN.coef[j]*coef[i]+A.coef[i+j]+k;
                k=(tmp>>BASE_SIZE);
                A.coef[i+j]=tmp;
            }
            A.coef[LN.len+i]+=k;
        }
        A.NormLen();
        return A;
    }

    LargeNum LargeNum::operator* (const BASE num){
        TMP tmp=0;
        TMP k=0;
        LargeNum A(len+1,0);
        for(int i=0;i<len;i++){
            tmp=coef[i]*num+k;
            k=(BASE)(tmp>>BASE_SIZE);
            A.coef[i]=(BASE)tmp;
        }
        A.coef[len]=k;
        A.NormLen();
        return A;
    }

    LargeNum& LargeNum::operator*= (const BASE num){
        *this=*this*num;
        return *this;
    }
            

    LargeNum& LargeNum::operator *= (const LargeNum &LN){
        *this=*this*LN;
        return *this;
    }

    LargeNum LargeNum::operator/(const BASE num){
        TMP tmp,k=0;
        LargeNum A(len,0);
        for(int i=len-1;i>=0;i--){
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
            A.coef[i]=(tmp)/num;
        }
        A.NormLen();
        return A;
    }

    LargeNum& LargeNum::operator/=(const BASE num){
        *this=*this/num;
        return *this;
    }

    void LargeNum::DecInput(const char *S){
        TMP tmp=0;
        BASE k=0;
        int BS=BASE_SIZE/4;
        int Slen=strlen(S);
        maxLEN=(Slen-1)/BS+2;
        len=1;
        delete[]coef;
        coef=new BASE[maxLEN];
        for(int i=0;i<maxLEN;i++) coef[i]=0;
        
        for(int i=0;i<Slen;i++){
            char s=0;
            if((S[i]>='0')&&(S[i]<='9')) s=S[i]-'0';
            (*this*=10)+=s;
            //cout<<coef[1]<<' '<<coef[0]<<'\n';
        }
        NormLen();
    }

    void LargeNum::DecOutput(){
        LargeNum A(*this);
        int SLen=len*10;
        int k=0;
        //unsigned char tmp;
        char* S=new char[SLen];
        while(A.coef[0]!=0||A.len>1){
            S[k]=(A%10)+'0';
            //cout<<S[k]<<' ';
            A/=10;
            k++;
        }
        k--;
        for(;k>=0;k--) cout<<S[k];
        delete[]S;
    }

    

    BASE LargeNum::operator%(const BASE num){
        BASE r=0;
        TMP MAX=MAX_BASE%num;
        for(int i=0;i<len;i++){
            BASE rn=coef[i]%num;
            for(int j=0;j<i;j++) rn=(rn*MAX)%num;
            r=(r+rn)%num;
        }
        return r;
    }

    void LargeNum::ExpMaxLen(int b){
        maxLEN+=b;
        BASE *NewCoef=new BASE[maxLEN];
        for(int i=0;i<len;i++) NewCoef[i]=coef[i];
        for(int i=len;i<maxLEN;i++) NewCoef[i]=0;
        delete[]coef;
        coef=NewCoef;
    }

    void LargeNum::NormLen(){
        for(len=maxLEN;coef[len-1]==0&&len>0;len--);
        if(len==0) len=1;
    }
    
    void LargeNum::PrintCoef(){
        for(int i=len-1;i>=0;i--) cout<<coef[i];
        cout<<'\n';
                for(int i=0;i<len;i++) cout<<coef[i];

    }

int main()
{
    srand(time(0));
    LargeNum A(4,0);    
    LargeNum B(4,0);
    LargeNum C;
 
    //A.HexInput("ffffff");
    A.DecInput("2573904118");
    A.PrintCoef();
    //B.DecInput("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
   // C=A*B;
    //A.HexOutput();
    //B.HexInput("301A");
    //A.HexInput("A");
    //B*=243;
    //B.DecInput("1234123412");
   // C.DecOutput();
    //B.DecOutput();
    //A.HexInput("ffff642");
    //A.HexOutput();
    //cout<<'\n';
    //B.HexOutput();
    //cout<<'\n';
   // LargeNum C;
    //C=A+B;
    //C.HexOutput();
    //cout<<'\n';
    //C.HexOutput();
    //cout<<'\n';
   // C-=A;
    //C.HexOutput();
    //cout<<'\n';
    
    
    //C=A+B;
    //C=A-B;
    //A+=B;
    //A-=B;
    //C=A*B;
    //C=A/15;
    //A*=B;


   // C.HexOutput();
    //cout<<'\n';
    //A.HexOutput();
    //cout<<'\n';
    //B.HexOutput();
    //cout<<'\n';

  //  cout<<(A==B);

    //system("pause");
}