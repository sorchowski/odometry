#ifndef ODOMETRY_PAYLOAD_H
#define ODOMETRY_PAYLOAD_H

#define ODOMETRY_PAYLOAD_LENGTH 12

struct __attribute__ ((packed)) OdometryPayload
{
    uint32_t count1;
    uint32_t count2;
    byte status1;
    byte status2;
};


#endif // ODOMETRY_PAYLOAD_H