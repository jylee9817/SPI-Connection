#include "WPILib.h"
#include "Timer.h"
#include "SPI.h"

#include "iostream"
#include "fstream"

using namespace std;

class Robot: public IterativeRobot
{
private:

	Joystick* joy;

	SPI* spi;
	uint8_t buffer[8] = {1,2,3,4,5,6,7,8}; //saves input from lidar in byte array
	uint8_t dataToSend[1] = {37};
	//uint8_t Data2[1] = {45};

	//uint8_t buff2[8] = {1,2,3,4,5,6,7,8};

	uint8_t size = 2;//changed it to 4, works after this change, 0 or 1 does not work

	uint16_t distance;

	double HZ = 10000;

	void RobotInit()
	{

		/*ofstream myfile("example.txt", ios::in);
		myfile.open("example.txt");
		myfile << "This is a line.\n";
		myfile.close();*/
		joy = new Joystick(0);

		//Initialize the I2C connection on address 84
		spi = new SPI(SPI::Port::kOnboardCS3); //you can change the port; kOnboardCS0-3
		spi->SetClockRate(10000);
		spi->SetMSBFirst();
		spi->SetSampleDataOnFalling();//correct, do not use Rising
		spi->SetClockActiveLow();//correct, do not use High
		spi->SetChipSelectActiveLow();//correct, do not use High
		//spilib_setspeed(spilib_open("/dev/spidev0.0"), HZ);


	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

		/*ofstream myfile("example.txt", ios::in);
		myfile.open("example.txt");
		myfile << "test numbers";
		myfile.close();*/
		joy = new Joystick(0);

		dataToSend[0] = -(joy->GetY()*90) + 90;
		SmartDashboard::PutNumber("data to send", dataToSend[0]);

		spi->Transaction(dataToSend, buffer, size);

		//distance = buffer[0];//packet 2//(PK1 << 8) | PK2;

		SmartDashboard::PutNumber("Distance 0", buffer[0]);
		SmartDashboard::PutNumber("Distance 1", buffer[1]);
		SmartDashboard::PutNumber("Distance 2", buffer[2]);
		SmartDashboard::PutNumber("Distance 3", buffer[3]);
		SmartDashboard::PutNumber("Distance 4", buffer[4]);

		Wait(0);
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot)
