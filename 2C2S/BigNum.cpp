#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <type_traits> 
#include <string>
#include <bitset>
#define BASE_SIZE (sizeof(BASE)*8)
#define MAX_BASE ((unsigned long long)1<<BASE_SIZE)

using namespace std;
typedef unsigned char BASE;
typedef conditional<BASE_SIZE<32,conditional<BASE_SIZE<16,unsigned short, unsigned int >::type, unsigned long long>::type TMP; 


class BigNum{
    public:
    BASE *coef;
    int maxLEN;
    int len;
    
        BigNum(int Len=1, bool k=0);
        BigNum(const BigNum &);
        ~BigNum(){delete[]coef; coef=NULL; len=0; maxLEN=0;}
        BigNum& operator = (const BigNum &);
        BigNum& operator = (const BASE);

        void HexInput(const char *);
        void HexOutput();

        bool operator == (const BigNum&);
        bool operator != (const BigNum&);
        bool operator >= (const BigNum&);
        bool operator <= (const BigNum&);
        bool operator > (const BigNum&);
        bool operator < (const BigNum&);

        BigNum operator + (const BigNum &);
        BigNum& operator += (const BigNum &);
        BigNum operator + (const BASE);
        BigNum& operator += (const BASE);

        BigNum operator - (const BASE);
        BigNum& operator -= (const BASE);
        BigNum operator - (const BigNum &);
        BigNum& operator -= (const BigNum &);

        BigNum operator * (const BigNum &);
        BigNum& operator *= (const BigNum &);
        BigNum operator * (const BASE);
        BigNum& operator *= (const BASE);

        BigNum operator /(const BASE);
        BigNum& operator /=(const BASE);
        BigNum operator /(const BigNum &);
        BigNum& operator /=(const BigNum &);

        BASE operator %(const BASE);
        BigNum& operator %=(const BASE);
        BigNum operator %(const BigNum &);
        BigNum& operator %=(const BigNum &);

        //friend ostream& operator << (ostream &, LargeNum&);
        //friend istream& operator >> (istream &, LargeNum&);

        void DecInput(const char *);
        void DecOutput();
        void PrintCoef();
    private:
        void ExpMaxLen(int);
        void NormLen();
        int SubLN(const BigNum&, int);
        void AddLN(const BigNum&, int);
        BigNum DivBN(const BASE , int);

};

    BigNum::BigNum(int Len, bool k){
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

    BigNum::BigNum(const BigNum &LN){
        if(&LN!=this){
            maxLEN=LN.maxLEN;
            len=LN.len;
            coef=new BASE[maxLEN];
            for(int i=0;i<maxLEN;i++) coef[i]=LN.coef[i];
        }

    }
    
    BigNum& BigNum::operator = (const BigNum &LN){
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

    BigNum& BigNum::operator = (const BASE num){
        delete[]coef;
        len=1;
        maxLEN=len;
        coef=new BASE[len];
        coef[0]=num;
        return *this;
    }

    void BigNum::HexInput(const char *S){
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
            else continue;
            coef[iRev/BS]=coef[iRev/BS]|(tmp<<((iRev%BS)*4)); 
        }
        this->NormLen();
    }

    void BigNum::HexOutput(){
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

    bool BigNum::operator == (const BigNum &LN){
        if(&LN==this) return 1;
        if(len!=LN.len) return 0;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else return false;
        }
        return true;
    }

    bool BigNum::operator != (const BigNum &LN){
        if(&LN==this) return 0;
        if(len!=LN.len) return 1;
        int i=len-1;
        while(i>=0){
            if(coef[i]==LN.coef[i]) i--;
            else return true;
        }
        return false;
    }

    bool BigNum::operator >= (const BigNum &LN){
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

    bool BigNum::operator <= (const BigNum &LN){
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

    bool BigNum::operator > (const BigNum &LN){
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

    bool BigNum::operator < (const BigNum &LN){
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

    BigNum BigNum::operator+ (const BigNum &LN){
        int min,max;
        BASE *MaxCoef;
        bool k=0;
        TMP tmp;
        if(len>LN.len) {min=LN.len; max=len+1; MaxCoef=coef;}
        else {min=len; max=LN.len+1; MaxCoef=LN.coef;}
        BigNum A(max,0);
        for(int i=0;i<min;i++){
            tmp=(TMP)LN.coef[i]+(TMP)coef[i]+k;
            A.coef[i]=(BASE)tmp;
            k=(bool)(tmp>>BASE_SIZE);
        }
        for(int i=min;i<max-1;i++){
            tmp=(TMP)MaxCoef[i]+k;
            A.coef[i]=(BASE)tmp;
            k=(bool)(tmp>>BASE_SIZE);
        }
        A.coef[max-1]=k;
        A.NormLen();
        return A;
    }

    BigNum BigNum::operator+ (const BASE num){
        TMP tmp=0;
        bool k=0;
        int Len=maxLEN;
        if(len+1>maxLEN) Len=len+1;
        BigNum A(Len,0);
        tmp=(TMP)coef[0]+(TMP)num+k;
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

    BigNum& BigNum::operator+= (const BASE num){
        *this=*this+num;
        return *this;
    }

    BigNum& BigNum::operator+= (const BigNum &LN){
        *this=*this+LN;
        return *this;
    }

    BigNum BigNum::operator - (const BASE num){
        if (*this < num) {
        throw invalid_argument("first value should be bigger than second to subtract");
        }
        TMP tmp;
        bool k=0;
        BigNum A(*this);
        tmp=A.coef[0]+MAX_BASE-num;
        A.coef[0]=(BASE)tmp;
        k=!((tmp)>>BASE_SIZE);
        for(int i=A.len;k&&i<=A.len;i++){
            tmp=A.coef[i]+MAX_BASE-k;
            A.coef[i]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        A.NormLen();
        return A;
    }
    
    BigNum& BigNum::operator -= (const BASE num){
        *this=*this-num;
        return *this;
    }

    BigNum BigNum::operator - (const BigNum &LN){
        if (*this < LN) {
        throw std::invalid_argument("first value should be bigger than second to subtract");
        }
        TMP tmp;
        bool k=0;
        BigNum A(*this);
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

    BigNum& BigNum::operator -= (const BigNum &LN){
        if (*this < LN) {
        throw invalid_argument("first value should be bigger than second to subtract");
        }
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

    BigNum BigNum::operator* (const BigNum &LN){
        TMP tmp;
        BASE k;
        BigNum A(len+LN.len,0);
        for(int i=0;i<len;i++){
            k=0;
            for(int j=0;j<LN.len;j++){
                tmp=(TMP)LN.coef[j]*(TMP)coef[i]+A.coef[i+j]+k;
                k=(tmp>>BASE_SIZE);
                A.coef[i+j]=tmp;
            }
            A.coef[LN.len+i]+=k;
        }
        A.NormLen();
        return A;
    }

    BigNum BigNum::operator* (const BASE num){
        TMP tmp=0;
        BASE k=0;
        BigNum A(len+1,0);
        for(int i=0;i<len;i++){
            tmp=(TMP)coef[i]*(TMP)num+k;
            k=(tmp>>BASE_SIZE);
            A.coef[i]=(BASE)tmp;
        }
        A.coef[len]=k;
        A.NormLen();
        return A;
    }

    BigNum& BigNum::operator*= (const BASE num){
        *this=*this*num;
        return *this;
    }
            

    BigNum& BigNum::operator *= (const BigNum &LN){
        *this=*this*LN;
        return *this;
    }

    BigNum BigNum::operator/(const BASE num){
        TMP tmp,k=0;
        BigNum A(len,0);
        for(int i=len-1;i>=0;i--){
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
            A.coef[i]=(tmp)/num;
        }
        A.NormLen();
        return A;
    }

    BigNum& BigNum::operator/=(const BASE num){
        *this=*this/num;
        return *this;
    }
    
    BASE BigNum::operator%(const BASE num){
        TMP tmp,k=0;
        for(int i=len-1;i>=0;i--){
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
        }
        return k;
    }

    BigNum& BigNum::operator%=(const BASE num){
        *this=*this%num;
        return *this;
    }

    BigNum BigNum::operator/(const BigNum &LN){
        if((LN.len==1)&&(LN.coef[0]!=0)){BigNum q=*this/LN.coef[0]; return q;}   
        if((LN.len==1)&&(LN.coef[0]==0)){BigNum q(1,0); q.coef[0]=0; return q;}   
        if(*this==LN){BigNum q(1,0); q.coef[0]=1; return q;}
        if(*this<LN){BigNum q; return q;}
        BigNum q(len-LN.len+1);
        TMP b=MAX_BASE;
        //D1
        TMP d=b/((TMP)LN.coef[LN.len-1]+1);
        BigNum dU(*this);
        BigNum dV(LN);
        dU*=d;
        dV*=d;   
         if(len==dU.len){
            if(dU.len==dU.maxLEN){
                dU.ExpMaxLen(1);
            }
            dU.coef[len]=0;
            dU.len=len+1;
        }   
        //D2
        for(int j=len-LN.len;j>=0;j--){//D7(Цикл по j)
            //D3
            int _j=j+LN.len;
            TMP ff=((TMP)dU.coef[_j]<<BASE_SIZE)+dU.coef[_j-1];
            TMP _q=(ff/dV.coef[LN.len-1]);
            TMP _r=(ff%dV.coef[LN.len-1]);
            if((_q==b)||((_q*dV.coef[LN.len-2])>(b*_r+dU.coef[_j-2]))){
                _q--; _r=_r+dV.coef[LN.len-1];
                if((_r<b)&&((_q==b)||(_q*dV.coef[LN.len-2]>b*_r+dU.coef[_j-2]))){
                    _q--; _r=_r+dV.coef[LN.len-1];
                }
            }
            //D5
            q.coef[j]=_q;   
            //D4
            int k=dU.SubLN( dV*_q,j);
            if(k){
                //D6
                q.coef[j]--;
                dU.AddLN(dV,j);
            }
        }
        //D8
        q.NormLen();
        return q;
    }

    BigNum& BigNum::operator/=(const BigNum &LN){
        *this=*this/LN;
        return *this;
    }

    BigNum BigNum::operator%(const BigNum &LN){
        if((LN.len==1)&&(LN.coef[0]!=0)){BigNum r(1,0); r.coef[0]=*this%LN.coef[0]; return r;}   
        if((LN.len==1)&&(LN.coef[0]==0)){BigNum r(*this); return r;}
        if(*this==LN){BigNum r(1,0); return r;}
        if(*this<LN){BigNum r(*this); return r;}
        BigNum r(LN.len-1);
        TMP b=MAX_BASE;
        TMP d=b/((TMP)LN.coef[LN.len-1]+1);
        BigNum dU(*this);
        BigNum dV(LN);
        dU*=d;
        dV*=d;
        if(len==dU.len){
            if(dU.len==dU.maxLEN){
                dU.ExpMaxLen(1);
            }
            dU.coef[len]=0;
            dU.len=len+1;
        }   
        for(int j=len-LN.len;j>=0;j--){
            int _j=j+LN.len;
            TMP ff=((TMP)dU.coef[_j]<<BASE_SIZE)+dU.coef[_j-1];
            TMP _q=(ff/dV.coef[LN.len-1]);
            TMP _r=(ff%dV.coef[LN.len-1]);
            if((_q==b)||((_q*dV.coef[LN.len-2])>(b*_r+dU.coef[_j-2]))){
                _q--; _r=_r+dV.coef[LN.len-1];
                if((_r<b)&&((_q==b)||(_q*dV.coef[LN.len-2]>b*_r+dU.coef[_j-2]))){
                    _q--; _r=_r+dV.coef[LN.len-1];
                }
            }
            int k=dU.SubLN( dV*_q,j);
            if(k){
                //D6
                //q.coef[j]--;
                dU.AddLN(dV,j);
            }
        }
        dU.len=LN.len;
        r=dU/d;
        
        //r.NormLen();
        return r;
    }

    BigNum& BigNum::operator%=(const BigNum &LN){
        *this=*this%LN;
        return *this;
    }

    void BigNum::DecInput(const char *S){
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
            else continue;
            (*this*=10)+=s;
        }
        NormLen();
    }

    void BigNum::DecOutput(){
        if(len==1&&coef[0]==0) cout<<0;
        else{
        BigNum A(*this);
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
    }   


    int BigNum::SubLN(const BigNum &LN, int j){
        TMP tmp;
        bool k=0;
        for(int i=0;i<LN.len;i++){
            //cout<<i+j<<" "<<len<<endl;
            tmp=(TMP)coef[i+j]+MAX_BASE-LN.coef[i]-k;
            coef[i+j]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        for(int i=LN.len;k&&(i+j)<len;i++){
            //cout<<i+j<<" "<<len<<endl;
            tmp=(TMP)coef[i+j]+MAX_BASE-k;
            coef[i+j]=(BASE)tmp;
            k=!((tmp)>>BASE_SIZE);
        }
        NormLen();
        return k;
    }


    void BigNum::AddLN(const BigNum &LN, int j){
        bool k=0;
        for(int i=0;i<LN.len;i++){
            TMP tmp=(TMP)coef[i+j]+LN.coef[i]+k;
            k=bool(tmp>>BASE_SIZE);
            coef[i+j]=tmp;
        }
        for(int i=LN.len;k&&(i+j)<len;i++){
            TMP tmp=(TMP)coef[i+j]+k;
            k=bool(tmp>>BASE_SIZE);
            coef[i+j]=tmp;
        }
        //NormLen();
    }

    BigNum BigNum::DivBN(const BASE num, int j){
        TMP tmp,k=0;
        BigNum A(j+1,0);
        for(int i=j;i>=0;i--){
            tmp=(k<<BASE_SIZE)+coef[i];
            k=tmp%num;
            A.coef[i]=(tmp)/num;
        }
        A.NormLen();
        return A;
    }




    void BigNum::ExpMaxLen(int b){
        maxLEN+=b;
        BASE *NewCoef=new BASE[maxLEN];
        for(int i=0;i<len;i++) NewCoef[i]=coef[i];
        for(int i=len;i<maxLEN;i++) NewCoef[i]=0;
        delete[]coef;
        coef=NewCoef;
    }

    void BigNum::NormLen(){
        for(len=maxLEN;coef[len-1]==0&&len>0;len--);
        if(len==0) len=1;
    }
    
    void BigNum::PrintCoef(){
        for(int i=len-1;i>=0;i--) cout<<bitset<8>(coef[i])<<" ";
        cout<<endl;

    }

int main()
{
    srand(time(0));
    BigNum a(4, 1);
    BigNum b(4, 1);
    BigNum c(3, 1);
    BigNum r;
    //BigNum d(4, 1);
    //BigNum e(3, 1);
    /*a.DecInput("ffffffffffff");
    b.DecInput("3123012391afba123123fffabc");
    a.DecOutput();
    cout<<endl;
    b.DecOutput();*/
    BigNum A,B,C,D;
    /*A.DecInput("1942639102760151127112018175129867903165646490149609316528852873075873262287291547085");
    A.DecOutput();
    cout<<endl;
    B.DecInput("5172014117664208941109153793832655");
    B.DecOutput();
    cout<<endl;
    C=A/B;
    D=A%B;
    
    C.DecOutput();
    cout<<endl;
    D.DecOutput();
    cout<<endl;
    C.PrintCoef();
    G.DecInput("375605916489162266346230558336292088568715033256988");
    G.PrintCoef();*/
    int M = 100;
    int T = 100000;
    int n, m;
    do{ 
        n = rand()%M + 1;
        m = rand()%M + 1;
        BigNum _A(n,1); 
        BigNum _B(m,1);
        A=_A; B=_B;
        /*A.DecOutput();
        cout<<endl;
        B.DecOutput();
        cout<<endl;*/
        C = A/B;
        D = A%B;
        //cout<<"T : "<<T<<endl;

        /*C.DecOutput();
        cout<<endl;
        D.DecOutput();
        cout<<endl;
        cout<<endl;*/
    }
    while((A == ((C*B) + D)) && ((A-D) == (C*B)) && (D < B) && (--T));
    if(T>0){
        if(!(A == ((C*B) + D))) cout<<"Failed #1"<<endl;
        if(!((A-D) == (C*B))) cout<<"Failed #2"<<endl;
        if(!(D < B)) cout<<"Failed #3"<<endl;
       
        cout<<"Failed: T="<<T<<endl;
        A.DecOutput();
        cout<<endl;
        B.DecOutput();
        cout<<endl;
        C.DecOutput();
        cout<<endl;
        D.DecOutput();
        cout<<endl;
        cout<<endl;
    }
    else cout<<"Successed"<<endl;
    /*A.DecInput("999995325351351353232124142");
    B.DecInput("24112412415325235235232323");
    C=A/B;
    A.DecOutput();
    cout<<endl;
    B.DecOutput();
    cout<<endl;
    C.DecOutput();*/
    /*A.DecOutput();
    cout<<endl;
    B.DecOutput();
    cout<<endl;
    C.DecOutput();
    cout<<endl;
    D.DecOutput();
    cout<<endl;
    cout<<BASE_SIZE;*/
}
