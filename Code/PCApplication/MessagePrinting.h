//#pragma once
//
//#include <iostream>
//
//#include "../Arduino/RobotCommand.h"
//#include "../Arduino/UnitStatus.h"
//#include "../Arduino/Enums.h"
//
//using namespace std;
//
//namespace RobotCommunication
//{
//	static ostream& operator<< (ostream &out, RobotCommand &msg)
//	{
//		if (msg.SendAcc)
//			out << "Sending acknowledge" << endl;
//		else
//			out << "Not sending acknowledge" << endl;
//
//		out << "Sent command type: ";
//		switch (msg.Type)
//		{
//		case CMD_GET_CURRENT_STATE:
//			out << "Get status";
//			break;
//		case CMD_FORWARD:
//			out << "Forward";
//			break;
//		case CMD_BACKWARD:
//			out << "Reversing";
//			break;
//		case CMD_LEFT:
//			out << "Left";
//			break;
//		case CMD_RIGHT:
//			out << "Right";
//			break;
//		case CMD_STOP:
//			out << "Stopping";
//			break;
//		default:
//			out << "UNKNOWN COMMAND";
//			break;
//		}
//		out << " at " << static_cast<float>(msg.Speed) / 2.55f << " %";
//
//		return out;
//	}
//	static ostream& operator<< (ostream &out, UnitStatus &msg)
//	{
//		out << "Heading: ";
//		if (msg.Heading == ST_RIGHT)
//			out << "Right";
//		else
//			out << "Left";
//		out << " at " << static_cast<float>(msg.TurnAngle) / 2.55f << " %" << endl;
//
//		out << "Direction: ";
//		switch (msg.Direction)
//		{
//		case ST_FORWARD:
//			out << "Forward";
//			break;
//		case ST_FREE:
//			out << "Slowing down";
//			break;
//		case ST_BACKWARD:
//			out << "Reversing";
//			break;
//		case ST_STALL:
//			out << "Braking";
//			break;
//		default:
//			out << "UNKNOWN DIRECTION";
//			break;
//		}
//		out << " at " << static_cast<float>(msg.Speed) / 2.55f << " %" << endl;
//		return out;
//	}
//}