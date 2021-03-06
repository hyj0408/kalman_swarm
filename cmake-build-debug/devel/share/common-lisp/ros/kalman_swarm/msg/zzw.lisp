; Auto-generated. Do not edit!


(cl:in-package kalman_swarm-msg)


;//! \htmlinclude zzw.msg.html

(cl:defclass <zzw> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (pose
    :reader pose
    :initarg :pose
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (id
    :reader id
    :initarg :id
    :type cl:fixnum
    :initform 0))
)

(cl:defclass zzw (<zzw>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <zzw>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'zzw)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name kalman_swarm-msg:<zzw> is deprecated: use kalman_swarm-msg:zzw instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <zzw>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kalman_swarm-msg:header-val is deprecated.  Use kalman_swarm-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <zzw>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kalman_swarm-msg:pose-val is deprecated.  Use kalman_swarm-msg:pose instead.")
  (pose m))

(cl:ensure-generic-function 'id-val :lambda-list '(m))
(cl:defmethod id-val ((m <zzw>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader kalman_swarm-msg:id-val is deprecated.  Use kalman_swarm-msg:id instead.")
  (id m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <zzw>) ostream)
  "Serializes a message object of type '<zzw>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'id)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <zzw>) istream)
  "Deserializes a message object of type '<zzw>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'id)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<zzw>)))
  "Returns string type for a message object of type '<zzw>"
  "kalman_swarm/zzw")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'zzw)))
  "Returns string type for a message object of type 'zzw"
  "kalman_swarm/zzw")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<zzw>)))
  "Returns md5sum for a message object of type '<zzw>"
  "89238672f679ed6f7ea2ed0df8ca42e6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'zzw)))
  "Returns md5sum for a message object of type 'zzw"
  "89238672f679ed6f7ea2ed0df8ca42e6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<zzw>)))
  "Returns full string definition for message of type '<zzw>"
  (cl:format cl:nil "std_msgs/Header header~%geometry_msgs/Pose pose~%uint8 id~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'zzw)))
  "Returns full string definition for message of type 'zzw"
  (cl:format cl:nil "std_msgs/Header header~%geometry_msgs/Pose pose~%uint8 id~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <zzw>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <zzw>))
  "Converts a ROS message object to a list"
  (cl:list 'zzw
    (cl:cons ':header (header msg))
    (cl:cons ':pose (pose msg))
    (cl:cons ':id (id msg))
))
