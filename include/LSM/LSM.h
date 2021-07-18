//
// Created by up on 2021/7/17.
//

#ifndef KALMAN_SWARM_LSM_H
#define KALMAN_SWARM_LSM_H

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Eigenvalues>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <deque>

using namespace std;

namespace LSM{
    static Eigen::MatrixXd solve(const deque<Eigen::Vector2d> & data,int N){

        // 创建两个vector
        vector<double> x, y;
        for(auto d : data){
            x.push_back(d[0]);
            y.push_back(d[1]);
        }

        if (x.size() != y.size()) {
            cout << "数据文件内容有误" << endl;
            exit(EXIT_FAILURE);
        }

        // 创建A矩阵
        Eigen::MatrixXd A(x.size(), N + 1);

        for (unsigned int i = 0; i < x.size(); ++i) {  // 遍历所有点

            for (int n = N, dex = 0; n >= 1; --n, ++dex) {  // 遍历N到1阶
                A(i, dex) = pow(x[i], n);
            }

            A(i, N) = 1;  //
        }

        // 创建B矩阵
        Eigen::MatrixXd B(y.size(), 1);

        for (unsigned int i = 0; i < y.size(); ++i) {
            B(i, 0) = y[i];
        }

        // 创建矩阵W
        Eigen::MatrixXd W;
        W = (A.transpose() * A).inverse() * A.transpose() * B;

        return W;

    }
}
#endif //KALMAN_SWARM_LSM_H
