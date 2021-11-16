#pragma once

struct CParserResult
{
	unsigned int nFrameType;					//帧类型：0x55000001、0x55000002、0x55000003、0x55000004、0x55000005、0x55000006等
	unsigned int nTanksType = 0;				//车型：当前是0001、2002、5016、7030...之一，未来还有其他的
	unsigned int nTanksID = 0;					//车号：如3112、3456等
	char strTimeStamp[32] = { '\0' };			//类似：2020-04-23 12:24:30（这是nTimeStamp转换后的结果）
	__time32_t nTimeStamp = 0;					//原始的时间戳信息（此值转换成字符串形式的时间戳就是strTimeStamp）
	int nInfoCount = 0;							//myDataInfo数组有效数据元素个数
	unsigned int nInfoEncoded[200];				//信息编码（就是Excel表中的值，Excel表中的值是用16进制表示的整数）
	unsigned int nInfoContent[200];				//信息内容（应要根据Excel表中的信息说明计算实际的物理量），当此值为0xCCCCCCCC时表示未采到数据或数据无效
};



/***********************************************************************************************************************
说明：以下数据结构是TSI文件的解析结果
信息内容的字段格式是36，全部有效，具体含义参见编码字典。
TSI文件的格式与save文件不同。TSI文件的头部16字节暂时未用上，所有实际数据是要跳过这16个字节的，每个数据帧长度为160字节，
所以TSI的长度减去文件头的16字节后应该是160的整数倍



注意：TSI文件中不含车号等信息，车号的信息基本上是存储TSI为文件的文件夹名称
***********************************************************************************************************************/
struct CParserResultForTSI
{
	char strTimeStamp[32] = { '\0' };			//类似：2020-04-23 12:24:30（这是nTimeStamp转换后的结果）
	__time32_t nTimeStamp = 0;					//原始的时间戳信息（此值转换成字符串形式的时间戳就是strTimeStamp）

	unsigned long m_sLongitude;					//经度
	unsigned long m_sLatitude;					//纬度
	unsigned long Course;						//航向角

	long nInfoEncoded[36];				//信息编码（就是Excel表中的值，Excel表中的值是用16进制表示的整数）
	long nInfoContent[36];				//信息内容（应要根据Excel表中的信息说明计算实际的物理量），当此值为0xCCCCCCCC时表示未采到数据或数据无效
};

//帧类型说明
//帧类型			Excel中帧ID				时间间隔(秒)
//0x55000001			1						1
//0x55000002			2						4
//0x55000003			3						4
//0x55000004			4						4
//0x55000005			5						4
//0x55000006			6						4