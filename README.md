# 添加了IMU的观测器  
**主要是为了解决imu漂移量不准且较为严重的问题**  
采用*滑动时间窗口*的形式，用一个双向队列deque来储存一个连续时间段内的数据，数据类型是state类  
只有aruco读取到数据，才会进队列，一旦读到一个为0的aruco值，队列清空  
队列满的时候，进行仅预测操作和预测+更新操作，做出其差，得到imu积分与imu+arcuo之间的差值

### 采用的是相对位置和相对imu的加速度值

### rosbag截取
通过时间截取bag包。  
`rosbag filter bigxxx.bag outmy.bag "t.to_sec() >= 0.00 and t.to_sec() <= 10.00"`  
`rosbag filter '/home/up/rosbag/2021-06-16-21-03-09/uav3_2021-06-16-20-58-17.bag.active' uav3_handle.bag "t.to_sec() >= 1623848344 and t.to_sec() <=1623848405"`

### 将非图片数据提取为.csv格式存储  
`rostopic echo -b xx.bag -p /mynteye/left/image_mono > cam0data.csv`

### 相比较imu_observer的改进
1. 在小距离看不见aruco时，在下一次看见的时候对距离进行矫正
2. 可以尝试对imu进行低通滤波
3. 小范围imu观测器修正的时候，可以按照低通的思路考虑历史信息
4. 长时间观测不到aruco时，用线性去拟合补偿