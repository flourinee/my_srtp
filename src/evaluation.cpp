#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include"./../inc/Eigen/Core"
#include"./../inc/Eigen/LU"
char buf[1024];
std::vector<std::vector<double> > string_to_num(char path[]){
    std::ifstream  file;
    std::vector<std::vector<double> > N;
    std::vector<double> M;
    file.open(path);
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
Eigen::MatrixXd to_matrix(std::vector<std::vector<double> > data){
    Eigen::MatrixXd M(data.size(),data[0].size());
    for(int i=0;i<data.size();i++){
        for(int j=0;j<data[0].size();j++){
            M(i,j)=data[i][j];
        }
    }
    return M;
}
double train(Eigen::MatrixXd X,Eigen::MatrixXd Y){
    
}
double svm(){

}
int main(){
    Eigen::MatrixXd X,Y;
    X=to_matrix(string_to_num("./../data/out.dat"));
    Y=to_matrix(string_to_num("./../data/classify.dat"));
    return 0;
}