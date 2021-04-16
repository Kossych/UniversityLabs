#include <iostream>
#include <ctime>
#include <stdbool.h>
#include <string.h>
#include <fstream>

using namespace std;


class Boolv
{ unsigned int *b;
 int nbit, m; // nbit – количество бит вектора, m – количество элементов массива b
 public:
 Boolv (int nn = 1); // формируется булев вектор из nn бит; все биты со значениями 0
 Boolv (const char*); // формируется булев вектор по строке из нулей и единиц
 Boolv (const Boolv &);
 ~Boolv ();

 void Set1 (int); // устанавливает указанный бит в 1
 void Set0 (int); // устанавливает указанный бит в 0
 int operator[] (int);
 Boolv& operator = (const Boolv&);
 Boolv operator | (const Boolv &);
 Boolv operator & (const Boolv &);
 Boolv operator ~ ();

 // Дополнительно
 int weight(); //вес вектора
 Boolv operator << (int); //сдвиг на k бит влево
 Boolv operator >> (int); //сдвиг на k бит вправо
 friend ostream& operator << (ostream &, Boolv &);
 friend istream& operator >> (istream &, Boolv &);
};


class BoolM
{ Boolv *v;
 int m, n; // m – количество строк, n – количество столбцов
 public:
 BoolM(int k=1, int l=1); // формируется нулевая булева матрица размера kl
 BoolM(BoolM &);

 Boolv & operator[] (int);
 BoolM operator = (BoolM &);
 friend ostream & operator << (ostream &, BoolM &);
 // Дополнительно
 void diagonal0(); // обнулить главную диагональ
 int *matrix_max(); // найти количество единиц в каждом столбце матрицы
 int BuildMatrix(int);
 Boolv DisLine();
};


Boolv::Boolv(int nn){
    nbit=nn;
    m=((nbit-1)/32)+1;
    b=new unsigned int[m];
    for(int i=0;i<m;i++) b[i]=0;
}

Boolv::Boolv(const char*c){
    nbit=strlen(c);
    m=((nbit-1)/32)+1;
    b=new unsigned int[m];
    for(int i=0;i<m;i++) b[i]=0;
    for(int i=0;i<nbit;i++) if(c[i]!='0') b[i/32]=b[i/32]|(1<<(i%32));
}

Boolv::Boolv(const Boolv &B){
    nbit=B.nbit;
    m=B.m;
    b=new unsigned int[m];
    for(int i=0;i<m;i++) b[i]=B.b[i];
}

Boolv::~Boolv (){
    delete[]b;
}

void Boolv::Set1(int k){
    if((k<nbit)&&(k>=0)){
        b[k/32]=b[k/32]|(1<<(k%32));
    }else
    {cout<<"Индекс за пределами";}
}

void Boolv::Set0(int k){
    if((k<nbit)&&(k>=0)){
        b[k/32]=b[k/32]&(~(1<<(k%32)));
    }else
    {cout<<"Индекс за пределами";}
}

int Boolv::operator[] (int k){
    if((k<nbit)&&(k>=0)){
    if((b[k/32]&(1<<(k%32)))==0) return 0;
    else return 1;
    }else{cout<<"Индекс за пределами"; return -1;}
}

Boolv& Boolv::operator = (const Boolv &B){
    if(this!=&B){
    m=B.m;
    nbit=B.nbit;
    delete[]b;
    b=new unsigned int[m];
    for(int i=0;i<m;i++) b[i]=B.b[i];
    }
    return *this;
}

Boolv Boolv::operator | (const Boolv &B){
    Boolv copy;
    if(nbit>B.nbit){ 
        copy=*this;
        for(int i=0;i<B.m;i++) copy.b[i]=copy.b[i]|B.b[i];
    }
    else{
        copy=B;
        for(int i=0;i<m;i++) copy.b[i]=copy.b[i]|b[i];
    }
    return copy;
}

Boolv Boolv::operator & (const Boolv &B){
    Boolv copy;
    if(nbit>B.nbit){ 
        copy=*this;
        for(int i=0;i<B.m;i++) copy.b[i]=copy.b[i]&B.b[i];
        for(int i=B.m;i<m;i++) copy.b[i]=0;
    }
    else{
        copy=B;
        for(int i=0;i<m;i++) copy.b[i]=copy.b[i]&b[i];
        for(int i=m;i<B.m;i++) copy.b[i]=0;
    }
    return copy; 
}

Boolv Boolv::operator ~ (){
    Boolv copy(*this);
    for(int i=0;i<m;i++) copy.b[i]=~copy.b[i];
    return copy;
}

ostream & operator << (ostream &out, Boolv &B){
    for(int i=0;i<B.nbit;i++)
        if((B.b[i/32]&(1<<(i%32)))==0) out<<0;
        else out<<1;
    return out;
}

int Boolv::weight(){
    int weight=0;
    for(int j=0;j<m-1;j++)
    for(int i=0;i<32;i++) if((b[j]&(1<<(i)))!=0) weight++;
    for(int i=0;i<nbit%32;i++) if((b[m-1]&(1<<(i)))!=0) weight++;
    return weight;
}

Boolv Boolv::operator >> (int k){
    Boolv copy(*this);
    if(k>0){
    int kc=k/32;
    k=k%32;
    unsigned int mask=~(0xFFFFFFFF>>k);
    int i=m-1;
    for(;i>kc;i--){
    copy.b[i]=(copy.b[i-kc]<<k);
    copy.b[i]=copy.b[i]|((copy.b[i-1-kc]&mask)>>(32-k));
    }
    copy.b[i]=copy.b[i-kc]<<k;
    for(i=i-1;i>=0;i--)
    copy.b[i]=copy.b[i]&0;
    }
    else if(k<0) cout<<"Ошибка: Значение cдвига отрицательное\n";
    return copy;
}

Boolv Boolv::operator << (int k){
    Boolv copy(*this);
    if(k>=0){
    int kc=k/32;
    k=k%32;
    unsigned int mask=~(0xFFFFFFFF<<k);
    int i=0;
    for(;i<m-kc;i++){
    copy.b[i]=(copy.b[i+kc]>>k);
    copy.b[i]=copy.b[i]|((copy.b[i+1+kc]&mask)<<(32-k));
    }
    copy.b[i]=copy.b[m-kc]>>k;
    for(int i=i+1;i<m;i++)
    copy.b[i]=copy.b[i]&0;
    }
    else if(k<0) cout<<"Ошибка: Значение cдвига отрицательное\n";
    return copy;
}

BoolM::BoolM(int k, int l){
    m=l;
    v=new Boolv[m];
    n=k;
    Boolv b(n);
    for(int i=0;i<m;i++){
        v[i]=b;
    }
}

BoolM::BoolM(BoolM &M){
    n=M.n;
    v=new Boolv[m=M.m];
    for(int i=0;i<m;i++){
        v[i]=M[i];
    }
}

Boolv& BoolM::operator[] (int k){
    if((k<m)&&(k>=0))
    return this->v[k];
    else{
        cout<<"Индекс за пределами";
        exit(0);
    }
}

BoolM BoolM::operator = (BoolM &M){
    if(this!=&M){
    n=M.n;
    delete[]v;
    v=new Boolv[m=M.m];
    for(int i=0;i<m;i++){
        Boolv b(n);
        v[i]=M[i];
    }
    }
    return *this;
}

ostream & operator << (ostream &out, BoolM &M){
    for(int i=0;i<M.m;i++) out<<M.v[i]<<'\n';
    return out;
}

void BoolM::diagonal0(){
    if(m==n)
    for(int i=0;i<m;i++){
        v[i].Set0(i);
    }
}

int *BoolM::matrix_max(){
    int *mm=new int[n];
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            if(v[j][i]==1) mm[i]++;
    return mm;
}


int BoolM::BuildMatrix(int mn){
    ifstream fin("Graf.txt");
    if (fin.is_open()){
        int a,b;
        fin>>mn;
        BoolM BM(mn,mn);
        do{
            fin>>a;
            fin>>b;
            BM.v[a-1].Set1(b-1);
        }while(!fin.eof());
        fin.close();
        *this=BM;
        return mn;
    }
    return 0;
}

void Topsort2(BoolM &M, int m, int **a){
    Boolv DL=M.DisLine();
    Boolv BV(m);
    Boolv NullV(m);
    int k=0;
    *a=new int[m];
    for(int i=0;i<m;i++)
    (*a)[i]=0;
    while((DL.weight()!=m-k)&&(BV.weight()!=m)){
        for(int i=0;i<m;i++){
            if((DL[i]==0)&&(BV[i]!=1)){
                M[i]=M[i]&NullV;
                (*a)[k]=i+1;
                k++;
            }
        }
        BV=BV|(~DL);
        DL=M.DisLine();
    }
    if(BV.weight()!=m){
        delete[](*a);
        *a=NULL;
        cout<<"Граф циклический"<<endl;
    }
} 


Boolv BoolM::DisLine(){
    Boolv DL(n);
    for(int i=0;i<m;i++){
        DL=DL|this->v[i];
    }
    return DL;
}

int main(){
   // Boolv A("11111111"); 
    //Boolv B("00011");
    //B=B<<2;cout<<B;

    //BoolM M(5,5);
    //int k=M[0][0];
    //cout<<k<<endl;

   // Boolv a("110111011101110111011101110111011101010101010101010101111111010101");
   // int k=31;
   // Boolv b; b = a<<k;
  // // cout<<a<<endl;
  //  cout<<'\n';
   // cout<<b;
    //M[1]=C;
    //M.diagonal0();
    //B=A|C;
    //B=A&C;
    //B=A;
    //B=~B;
    //cout<<B;
    //int weight=A.weight();
    //cout<<mm<<'\n';
   // cout<<M<<'\n';
   
    BoolM M;
    int *a;
    int mm;
    mm=M.BuildMatrix(mm);
    cout<<M;
    Topsort2(M, mm, &a);
    if(a!=NULL)
    for(int i=0;i<mm;i++)
    cout<<a[i]<<' ';
    cout<<endl;

    system("pause");
}