//
// Created by huangyujun on 4/16/21.
//

#ifndef SRC_KALMAN_H
#define SRC_KALMAN_H

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <queue>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Pose.h>
#include "LSM/LSM.h"


class Kalman {
public:
    Eigen::MatrixXd A_trans, B_input, Q_cov;
    Eigen::MatrixXd C_obs, R_noise;
    Eigen::MatrixXd P_cov_0;
    double imu_rate = 50;
    double imu_dt=1/imu_rate;
    int lamda=1;


    Kalman() {
        A_trans=Eigen::Matrix<double, 6, 6>::Identity();
        A_trans.block(0,3,3,3)=imu_dt*Eigen::Matrix<double, 3, 3>::Identity();
        B_input=Eigen::Matrix<double, 6, 3>::Zero();
        B_input.block(0,0,3,3)=0.5*imu_dt*imu_dt*Eigen::Matrix<double, 3, 3>::Identity();
        B_input.block(3,0,3,3)=imu_dt*Eigen::Matrix<double, 3, 3>::Identity();
        C_obs=Eigen::Matrix<double, 3, 6>::Zero();
        C_obs.block(0,0,3,3)=Eigen::Matrix<double, 3, 3>::Identity();
        Q_cov = 0.01 * Eigen::Matrix<double, 6,6>::Identity();//要调参
        R_noise = 0.0001 * Eigen::Matrix<double, 3,3>::Identity();//要调参
        P_cov_0=0.2*Eigen::Matrix<double, 6, 6>::Identity();
    }

    struct State
    {
        int index=0;
        //uint32_t sys_time;
        std_msgs::Header head;
        Eigen::VectorXd x;
        Eigen::MatrixXd P_cov;
        Eigen::VectorXd u; //use input u to calculate x
        Eigen::Vector3d y;
        bool update_flag;

        State(){
//            x<<0,0,0,0;
//            y<<0,0;
            x=Eigen::VectorXd::Zero(6);

            y(0)=0;
            y(1)=0;
            y(2)=0;

            P_cov=0.2 *Eigen::Matrix<double, 6, 6>::Identity();

            update_flag= false;
        }
        State(Eigen::VectorXd x, Eigen::MatrixXd P_cov, Eigen::VectorXd u):
             x(x), P_cov(P_cov), u(u), update_flag(false){}
        ~State()
        {
        }
    };

    //int state_index=0;

    std::deque<State> states;
    std::deque<State> time_series;
    Eigen::VectorXd  outcome_difference=Eigen::VectorXd::Zero(6);

    //State state;
    void predict(Eigen::VectorXd u,std_msgs::Header head)
    {
        std::cout<<"go into predict"<<std::endl;
        State state_now;
        std::cout<<"state_now success"<<std::endl;
        state_now.u=u;
        state_now.head=head;
        int state_index=0;
        std::cout<<"state_now.u=u success"<<std::endl;
        std::cout<<states.size()<<std::endl;
        //std::cout<<1<<' ';
        //std::cout<<states.size()<<' ';
        if (states.size()==0){
            std::cout<<"if predict"<<std::endl;
            state_now.index=0;
            states.push_back(state_now);
            std::cout<<"after push"<<std::endl;
            //states[0]=state_now;
            state_index=0;
            std::cout<<states.size()<<std::endl;
        }
        else{
            std::cout<<"else predict"<<std::endl;
            state_index= states.back().index;
            std::cout<<"after states.back().index"<<std::endl;
            state_now.index=state_index+1;
            std::cout<<"after state_now.index=state_index+1;"<<std::endl;
            states.push_back(state_now);
            std::cout<<"after states.push_back(state_now);"<<std::endl;
            state_index++;
            std::cout<<"after state_index++;"<<std::endl;
            predict(states,state_index);
            std::cout<<"after predict"<<std::endl;
        }

//        std::cout<<"states.size="<<states.size()<<' ';
//        std::cout<<"state_index="<<state_index<<std::endl;

        //predict(state_index);
        //update(state_index);
    }

    void update(Eigen::Vector3d y,std_msgs::Header head)
    {
        int state_index=0;
        if (y(0)!=0&&y(1)!=0&&y(2)!=0)
        {
            std::cout<<"go into update"<<std::endl;
            state_index = states.back().index;
            std::cout<<"state_index"<<std::endl;
            states[state_index].head=head;
            std::cout<<"states[state_index].head=head;"<<std::endl;
            std::cout<<states.size()<<std::endl;
            states[state_index].y=y;
        }
        std::cout<<"after the first if"<<std::endl;

        if (y(0)!=0&&y(1)!=0&&y(2)!=0&&time_series.size()<20)
        {
            time_series.push_back(states[state_index]);
        }
        else if(y(0)==0||y(1)==0||y(2)==0)
        {
            time_series.clear();
        }
        else if(time_series.size()==20)
        {
            //time_series[0].x=Eigen::VectorXd::Zero(6);
            outcome_difference=get_difference(time_series);
        }
        std::cout<<"after the second if"<<std::endl;

        if (state_index==0)
        {
            std::cout<<"if update"<<std::endl;
        } else if (y(0)!=0&&y(1)!=0&&y(2)!=0)
        {
            Eigen::VectorXd x_last=states[state_index].x;
            std::cout<<"else update"<<std::endl;
            update(states,state_index);
            states[state_index].update_flag = true;
            compare(state_index,x_last);
        }
        //states[state_index].y=y;
    }


    std_msgs::Header getheader()
    {
        std::cout<<"into getheader()"<<std::endl;
        std_msgs::Header head1;
        if(states.size()!=0)
        {
            head1=states.back().head;
        }
        return head1;
    }

    geometry_msgs::Pose getpose()
    {
        //std::cout<<states.size()<<std::endl;
        geometry_msgs::Pose pose;
        if(states.size()!=0)
        {
            if(states.back().update_flag)
            {
                //std::cout<<states.back().x<<std::endl;
                pose.position.x=states.back().x(0);
                pose.position.y=states.back().x(1);
                pose.position.z=states.back().x(2);
            }
            else
            {
                pose.position.x=states.back().x(0)+outcome_difference(0);
                pose.position.y=states.back().x(1)+outcome_difference(1);
                pose.position.z=states.back().x(2)+outcome_difference(2);
//                pose.position.x=states.back().x(0);
//                pose.position.y=states.back().x(1);
//                pose.position.z=states.back().x(2);
            }

        }
        return pose;
    }


private:
    void predict(std::deque<State> & onestatedeque , int state_index)
    {
        std::cout<<"private predict"<<std::endl;
        //std::cout<<B_input<<std::endl;

// es[state_index].x=states[state_index].A*states[state_index-1].x + states[state_index-1].B*states[state_index-1].u;
//    states[state_index].P_cov=states[state_index].A*states[state_index-1].P_cov*states[state_index-1].A.transpose()+Q_cov;
        onestatedeque[state_index].x=A_trans*onestatedeque[state_index-1].x + B_input*onestatedeque[state_index-1].u;
//    std::cout<<states[state_index].x<<std::endl;
        std::cout<<onestatedeque.back().x<<std::endl;
        onestatedeque[state_index].P_cov=A_trans*onestatedeque[state_index-1].P_cov*A_trans.transpose()+Q_cov;
        std::cout<<"finish private predict"<<std::endl;
    }

    void update(std::deque<State> & onestatedeque , int & state_index)
    {

    std::cout<<"private update"<<std::endl;
    Eigen::MatrixXd K_gain;
//    K_gain=states[state_index].P_cov*C_obs.transpose()*(C_obs*states[state_index].P_cov*C_obs.transpose()+R_noise).inverse();
//    states[state_index].x=states[state_index].x+K_gain*(states[state_index].y-C_obs*states[state_index].x);
//    states[state_index].P_cov=states[state_index].P_cov-K_gain*C_obs*states[state_index].P_cov;
    K_gain=onestatedeque[state_index].P_cov*C_obs.transpose()*(C_obs*onestatedeque[state_index].P_cov*C_obs.transpose()+R_noise).inverse();
    onestatedeque[state_index].x=onestatedeque[state_index].x+K_gain*(onestatedeque[state_index].y-C_obs*onestatedeque[state_index].x);
    onestatedeque[state_index].P_cov=onestatedeque[state_index].P_cov-K_gain*C_obs*onestatedeque[state_index].P_cov;
    std::cout<<"private update done"<<std::endl;

//    std::cout<<states[state_index].x<<std::endl;
//    std::cout<<states.back().x<<std::endl;
    //state_index++;
    }

    void compare(int state_index,Eigen::VectorXd x_last)
    {
        outcome_difference=(1-lamda)*outcome_difference+lamda*(states[state_index].x-x_last);
    }

    Eigen::VectorXd  get_difference(std::deque<State> temp)
    {
        std::cout<<"get_difference"<<std::endl;
        Eigen::VectorXd  temp_outcome_difference;
        temp_outcome_difference=Eigen::VectorXd::Zero(6);
        std::deque<State> only_pre;
        std::deque<State> pre_upd;
        for (int i=0;i<20;i++)
        {
            only_pre.push_back(temp[i]);

            pre_upd.push_back(temp[i]);

        }
        only_pre[0].x=Eigen::VectorXd::Zero(6);
        only_pre[0].x[0]=only_pre[0].y[0];
        only_pre[0].x[1]=only_pre[0].y[1];
        only_pre[0].x[2]=only_pre[0].y[2];

        pre_upd[0].x=Eigen::VectorXd::Zero(6);
        pre_upd[0].x[0]=pre_upd[0].y[0];
        pre_upd[0].x[1]=pre_upd[0].y[1];
        pre_upd[0].x[2]=pre_upd[0].y[2];

        //int k=0;
        //update(pre_upd,k);
        //temp_outcome_difference=pre_upd[0].x-only_pre[0].x;
        std::cout<<"before the second for loop"<<std::endl;
        for(int j=1;j<20;j++)
        {
            std::cout<<"into the second for loop"<<std::endl;
            predict(only_pre,j);
            std::cout<<"after the first predict"<<std::endl;
            predict(pre_upd,j);
            std::cout<<"after the second predict"<<std::endl;
            update(pre_upd,j);
            std::cout<<"after update"<<std::endl;
        }
        for(int l=11;l<20;l++)
        {
            temp_outcome_difference=temp_outcome_difference+pre_upd[l].x-only_pre[l].x;
        }
        temp_outcome_difference=temp_outcome_difference/10;

        return temp_outcome_difference;
    }



};

#endif //SRC_KALMAN_H
