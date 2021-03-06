#pragma once

#ifdef CYTDATAPARSER_EXPORTS
#define CYTDATAPARSER_API __declspec(dllexport)
#else
#define CYTDATAPARSER_API __declspec(dllimport)
#endif

#include "PublicDefine.h"

//错误码值定义：
// 0, 成功
// 1, 错误:未找到或无法打开cytDataParser.cfg
// 2, 错误 : 尚未运行OnInit函数
// 3, 错误 : 函数参数pFrameBuf为空或nBufSize小于1012
// 4, 错误 : 函数参数pParserResult为空
// 5, 错误 : 参数指针为空

extern "C"
{
	/*******************************************************************************************************************
	描述：初始化解析库
	参数：pConfigFolderName，字符串，配置文件所在文件夹
	返回值：参见错误码定义
	备注：当前版本不需要设置配置文件夹，传入""即可，未来版本有可能使用；
	*******************************************************************************************************************/
	CYTDATAPARSER_API int cyt_OnInit(const char *pConfigFolderName);

	/*******************************************************************************************************************
	描述：获取解析库版本
	参数：pVer，整型指针，指向版本号
	返回值：参见错误码定义
	备注：无论是否初始化均可使用此函数
	*******************************************************************************************************************/
	CYTDATAPARSER_API int cyt_GetVerInfo(int *pVer);

	/*******************************************************************************************************************
	描述：解析数据
	参数：pFrameBuf，字符指针，指向数据帧缓冲区
			nBufSize，数据帧缓冲区大小
			pParserResult，指针，指向结果的指针
	返回值：参见错误码定义
	*******************************************************************************************************************/
	CYTDATAPARSER_API int cyt_OnParser(const unsigned char * pFrameBuf, int nBufSize, CParserResult *pParserResult);

	/*******************************************************************************************************************
	描述：解析数据（针对TSI格式文件）
	参数：pFrameBuf，字符指针，指向数据帧缓冲区
			nBufSize，数据帧缓冲区大小
			pParserResult，指针，指向结果的指针
	返回值：参见错误码定义
	*******************************************************************************************************************/
	CYTDATAPARSER_API int cyt_OnParserForTSI(const unsigned char * pFrameBuf, int nBufSize, CParserResultForTSI *pParserResultTSI);

	/*******************************************************************************************************************
	描述：关闭解析库
	返回值：参见错误码定义
	*******************************************************************************************************************/
	CYTDATAPARSER_API int cyt_OnClose();
}