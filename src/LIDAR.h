/*
 * LIDAR.h
 *
 *  Created on: Feb 1, 2016
 *      Author: Admin
 */
/*
#ifndef SRC_LIDAR_H_

#define SRC_LIDAR_H_
#define LIDAR_ADDR   (0x62)
#define LIDAR_CONFIG_REGISTER   (0x00)
#define LIDAR_DISTANCE_REGISTER	(0x8f)

class LIDAR
{
public:
	LIDAR(Port *port);
	~LIDAR();
	float getDistance();
	bool hadSignal();
	void start();
	void start(int period);
	void stop();
	void update();


private:

	I2C*i2c;
	unsigned char distance[0];
	bool hasSignal;

};
*/

//#endif /* SRC_LIDAR_H_ */
