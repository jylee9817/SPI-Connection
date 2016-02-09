#include "WPILib.h"
#include "Timer.h"
#include "SPI.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	SPI* spi;
	/*unsigned char*/uint8_t buffer[8]; //saves input from lidar in byte array
	/*unsigned char*/uint8_t dataToSend[1] = {65};
	/*unsigned char*/uint8_t size = 2;
	int32_t distance; //distance calculated in cm

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

		//Initialize the I2C connectin on address 84
		spi = new SPI(SPI::Port::kOnboardCS0); //you can change the port; kOnboardCS0-3
		spi->SetClockRate(500000);
		spi->SetMSBFirst();
		spi->SetSampleDataOnFalling();
		spi->SetClockActiveLow();
		spi->SetChipSelectActiveLow();
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
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		spi->Transaction(dataToSend, buffer, size); //(bool initiate, uint8_t* dataReceived, uint8_t size)
		distance = (int16_t)((buffer[2] << 8) | buffer[1]);//(uint32_t)buffer;//
		SmartDashboard::PutNumber("Distance", distance);

		Wait(1);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
