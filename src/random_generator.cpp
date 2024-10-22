#include<cstdio>
#include<cmath>
#include<random>
#include<iostream>
#include"./../inc/Eigen/Core"
std::vector<double> Normal_disribution(double mean_for_env,double stddev_for_env,double mean_for_noise,double stddev_for_noise,int times){

    std::random_device rd;
    std::mt19937 grnerator(rd());
    std::normal_distribution<double>  distribution_noise(mean_for_noise,stddev_for_noise);  
    std::normal_distribution<double>  distribution_env(mean_for_env,stddev_for_env);
    std::vector<double> N;
    for(int i=0;i<times;i++){
        N.push_back(distribution_noise(grnerator)+distribution_env(grnerator));      
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
void process(std::vector<std::vector<double> > sensors_init,int times,int num_of_narmal_sensors,int num_of_sensor_groups,std::vector<std::vector<std::vector<double > > >noise_init){
    char path[19]="./../data/out0.dat";
    Eigen::MatrixXd classify(times,num_of_sensor_groups);
    Eigen::MatrixXd final_class(times,1);        

    for(int i=0;i<num_of_sensor_groups;i++){
    freopen(path,"w",stdout);
        std::vector<double> Data[num_of_narmal_sensors+noise_init[i].size()];//assume five sensors
        Eigen::MatrixXd m(times,num_of_narmal_sensors+noise_init[i].size());
        for(int j=0;j<num_of_narmal_sensors;j++){
            Data[j]=Normal_disribution(sensors_init[i][0],sensors_init[i][1],sensors_init[i][2],sensors_init[i][3],times);
        }
        for(int k=0;k<noise_init[i].size();k++){
            Data[num_of_narmal_sensors+k]=Normal_disribution(noise_init[i][k][0],noise_init[i][k][1],noise_init[i][k][2],noise_init[i][k][3],times);
        }

        // for(int k=0;k<times;k++){
        // int sum=0;
        //     for(int j=0;j<num_of_narmal_sensors;j++){
        //         m(k,j)=Data[j][k];
        //         sum+=classify_function(sensors_init[i][0],sensors_init[i][1],0.1,m(k,j));        
        //     } 
        //     for(int j=0;j<noise_init[i].size();j++){            
        //         m(k,j+num_of_narmal_sensors)=Data[j+num_of_narmal_sensors][k];
        //         sum+=classify_function(noise_init[i][j][0],noise_init[i][j][1],0.1,m(k,j+num_of_narmal_sensors));
        //     }
        //     if(sum>(num_of_narmal_sensors+noise_init[i].size()-2)){
        //         classify(k,i)=1;
        //     } else{
        //     classify(k,i)=-1;
        //     }    

        // }
        std::cout<<m;
        path[13]++;
    }
    // freopen("./../data/classify_origin.dat","w",stdout);
    // std::cout<<classify;
    // freopen("./../data/classify.dat","w",stdout);
    // for(int i=0;i<times;i++){
    // int outclass=0;
    //     for(int j=0;j<num_of_sensor_groups;j++){
    //         outclass+=classify(i,j);
    //     }
    //     if(outclass==num_of_sensor_groups){
    //         final_class(i,0)=1;
    //     }else{
    //         final_class(i,0)=-1;
    //     }
    // }
    // std::cout<<final_class;
}
int main(){ 
    std::vector<double> push;
    std::vector<std::vector<double> >noise_init,sensors_init;
    std::vector<std::vector<std::vector<double > > > noise_pack;
    push.push_back(35);
    push.push_back(1);
    push.push_back(0);
    push.push_back(0.1);
    noise_init.push_back(push); 
    push.clear();

    push.push_back(30);
    push.push_back(5);
    push.push_back(0);
    push.push_back(0.1);
    noise_init.push_back(push); 
    push.clear();

    noise_pack.push_back(noise_init);
    noise_init.clear();

    push.push_back(1200);
    push.push_back(10);
    push.push_back(0);
    push.push_back(1);
    noise_init.push_back(push); 
    push.clear();

    push.push_back(1000);
    push.push_back(20);
    push.push_back(0);
    push.push_back(1);
    noise_init.push_back(push); 
    push.clear();

    noise_pack.push_back(noise_init);
    noise_init.clear();

    double  sensor1[4]={30,1,0,0.1};
    double  sensor2[4]={100,20,0,1};

    push.push_back(30);
    push.push_back(1);
    push.push_back(0);
    push.push_back(0.1);
    sensors_init.push_back(push); 
    push.clear();
    
    push.push_back(1000);
    push.push_back(10);
    push.push_back(0);
    push.push_back(1);
    sensors_init.push_back(push); 
    push.clear();    

    process(sensors_init,2000,5,2,noise_pack);
    return 0;
}