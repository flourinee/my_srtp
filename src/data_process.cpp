#include<algorithm>
#include<fstream>
#include<iostream>
#include<string>
#include<cmath>
#include"./../inc/Eigen/Core"
char buf[1024];
char path1[19]="./../data/out0.dat";
char path2[22]="./../data/result0.dat";
std::vector<std::vector<double> > string_to_num(){
    std::ifstream  file;
    std::vector<std::vector<double> > N;
    std::vector<double> M;
    file.open(path1);
    while (file.getline(buf,sizeof(buf))){
        int num=0,dot=0;
        int flag=0,cnt=1;
        buf[strlen(buf)]=' ';
        for(int i=0;i<strlen(buf);i++){
            if(buf[i]=='.') {
                flag=1;
                continue;
            }
            if(buf[i]==' '){
                if(num>0||dot>0){
                    M.push_back((double)(num)+(double)((double)dot/(cnt)));
                    flag=0;
                    num=0;
                    dot=0;
                    cnt=1;                                
                }
            continue;    
            }
            if(flag==0){
                num=num*10+(buf[i]-48);
            }else{
                dot=dot*10+(buf[i]-48);
                cnt*=10;
            }
        }
        N.push_back(M);
        M.clear();
    }
    return N;
}
bool classify_function(double mean,double stddev,double rate,double x){
    double left_edge;
    double right_edge;
    if(rate<=0.5) {
        left_edge=rate/2;
        right_edge=1-rate/2;
    }else{
        left_edge=1-rate/2;
        right_edge=rate/2;        
    }
    double num=(x-mean)/(sqrt(2)*stddev);
    double p;
    if(num>0) p=0.5+std::erf(num)/2;
    else p=0.5-std::erf(-num)/2;
    if(p>=left_edge&&p<=right_edge) return 1;
    else return 0;
}
std::vector<bool> classify(std::vector<double> Data){
    double u=0,d=0;
    int count=Data.size();
    std::vector<bool> label;
    for(int i=0;i<count;i++){
        u+=Data[i]/count;
        d+=Data[i]*Data[i]/count;
    }
    d-=u*u;
    for(int i=0;i<count;i++){
        label.push_back(classify_function(u,d,0.1,Data[i]));
    }
    return label;
}
void final_judge(std::vector<std::vector<bool > > Data){
    freopen("./../data/classify.dat","w",stdout);
    for(int i=0;i<Data[0].size();i++){
    int sum=0;
        for(int j=0;j<Data.size();j++){
           sum+=Data[j][i];
        }
        if(sum==Data.size()) std::cout<<"1"<<"\n";
        else std::cout<<"-1"<<"\n";
    }
}
double weight(std::vector<double> data,double mean){
    std::vector<double> info;
    double max=data[0],sum=0,ans=0; 
    for(int i=0;i<data.size();i++){
        double tmp=pow(data[i]-mean,2);
        info.push_back(tmp);
        if(max<tmp) max=tmp;
    }   
     
    for(int i=0;i<info.size();i++){
        info[i]/=max;
        double tmp=(1+std::erf(info[i]))/2;
        info[i]=(-1)*tmp*log2(tmp);
        sum+=info[i];
    }
    for(int i=0;i<info.size();i++){
        ans+=info[i]*data[i];
    }
    return ans/sum;
}
double weight(std::vector<double> data){
    std::vector<double> info;
    double max=data[0],mean=0,sum=0,ans=0; 
    mean/=data.size();
    for(int i=0;i<data.size();i++){
        double tmp=pow(data[i]-mean,2);
        info.push_back(tmp);
        if(max<tmp) max=tmp;
    }   
     
    for(int i=0;i<info.size();i++){
        info[i]/=max;
        double tmp=(1+std::erf(info[i]))/2;
        info[i]=(-1)*tmp*log2(tmp);
        sum+=info[i];
    }
    for(int i=0;i<info.size();i++){
        ans+=info[i]*data[i];
    }
    return ans/sum;
}
std::vector<double> kalman(std::vector<std::vector<double> > data,int d,int size){
    std::vector<double> Kn;
    std::vector<double>Xnn;
    std::vector<double>Xnn_1;
    std::vector<double>Pnn;
    std::vector<double>Pnn_1;
    std::vector<double>q;
    std::vector<double>r;
    std::vector<double>E_1;
    std::vector<double>E2_1;
    std::vector<double>E;
    std::vector<double>E2;
    for(int i=0;i<d;i++){
        Pnn_1.push_back(10000);
        Xnn_1.push_back(30);
        E_1.push_back(data[0][i]);
        E2_1.push_back(data[0][i]*data[0][i]);
        q.push_back(E2_1[i]-E_1[i]*E_1[i]);
        r.push_back(0.1*0.1);
    }
    std::vector<double> X,P;
    std::vector<double> ans1,ans2,ans3,ans4;
    for(int j=0;j<size;j++){
    double sum1=0,sum2=0,mean=0;
        for(int i=0;i<d;i++){
            E.push_back(E_1[i]+(data[j][i]-E_1[i])/(j+1));
            mean+=E[i];
            E2.push_back(E2_1[i]+(data[j][i]*data[j][i]-E2_1[i])/(j+1));
            q[i]=(E2[i]-E[i]*E[i]);
            Kn.push_back(Pnn_1[i]/(Pnn_1[i]+r[i]));
            Xnn.push_back(Xnn_1[i]+Kn[i]*(data[j][i]-Xnn_1[i]));
            Pnn.push_back((1-Kn[i])*Pnn_1[i]);
            X.push_back(Xnn[i]);
            P.push_back(Pnn[i]+q[i]);
        }//kalman filter
        for(int i=0;i<Xnn.size();i++){
            sum1+=Xnn[i];
            sum2+=data[j][i];
        }
        ans1.push_back(weight(Xnn,mean/Xnn.size()));
        ans2.push_back(weight(Xnn));
        ans3.push_back(sum1/Xnn.size());
        ans4.push_back(sum2/Xnn.size());
        E_1=E;
        E2_1=E2;
        E.clear();
        E2.clear();
        Kn.clear();
        Xnn_1=Xnn;
        Xnn.clear();
        Pnn_1=P;
        Pnn.clear();
        P.clear();
    }
    // freopen(path2,"w",stdout);
    // for(int i=0;i<size;i++){
    //     // std::cout<<"result_info:"<<ans1[i]<<" ";
    //     // std::cout<<"result_kalman:"<<ans2[i]<<" ";
    //     // std::cout<<"result_average:"<<ans3[i]<<"\n";
    //     //std::cout<< ans1[i] <<" "<<ans2[i]<<" "<<ans3[i]<<" "<<ans4[i]<<"\n";
    //     std::cout<<ans1[i]<<"\n";
    // }
    return ans1;
}
int main(){
    std::vector<std::vector<double> > N=string_to_num();
    std::vector<std::vector<double> > M;
    std::vector<std::vector<bool> > label;
    M.push_back(kalman(N,5,2000));
    label.push_back(classify(kalman(N,5,2000)));
    path1[13]++;
    N.clear();
    N=string_to_num();
    M.push_back(kalman(N,5,2000));
    label.push_back(classify(kalman(N,5,2000)));
    freopen(path2,"w",stdout);
    for(int i=0;i<2000;i++){
        std::cout<<M[0][i]<<" "<<M[1][i]<<"\n";
    }
    final_judge(label);
    return 0;
}