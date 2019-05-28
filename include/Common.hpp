#pragma once

#include <XUtil/XDateTime.hpp>
#include <DataDefEx.h>

namespace MTP {

inline uint16_t GetTimePointCount(period_t* Periods, uint8_t PeriodsCount)
{
	int i, j;
	period_t *pRealPeriods = Periods;
	uint8_t nRealPeriodsCount = PeriodsCount;
	uint16_t TimePointCount = 0;
	for (i = 0, j = 0; i < nRealPeriodsCount; i++)
	{
		if (pRealPeriods[i][1] < pRealPeriods[i][0]) {
			//跨天
			uint16_t PeriodMax = 24 * 60 + 0;
			j += (PeriodMax - pRealPeriods[i][0]);
			j += (pRealPeriods[i][1] - 0);
		}
		else {
			j += (pRealPeriods[i][1] - pRealPeriods[i][0]);
		}
	}
	TimePointCount = j;
	return TimePointCount;
}

inline uint32_t GetHHMMSSByTimePoint(uint16_t TimePoint, uint16_t MaxTimePoint, period_t* Periods, uint8_t PeriodsCount)
{
	uint16_t Minute;
	uint16_t TimePointCount = MaxTimePoint;
	int i;

	for (i = 0; i < PeriodsCount; i++)
	{
		if (Periods[i][1] < Periods[i][0]) {
			//跨天
			uint16_t PeriodMax = 24 * 60 + 0;
			Minute = Periods[i][0];
			if ((Minute + TimePoint) >= PeriodMax) {
				TimePoint -= (PeriodMax - Periods[i][0]);
				//第二天开始
				Minute = 0;
				if ((Minute + TimePoint) >= Periods[i][1])
				{
					TimePoint -= (Periods[i][1] - 0);
					continue;
				}
			}
		}
		else {
			Minute = Periods[i][0];
			if ((Minute + TimePoint) >= Periods[i][1]) {
				TimePoint -= (Periods[i][1] - Periods[i][0]);
				continue;
			}
		}
		Minute += TimePoint;
		break;
	}

	return XUtil::MinutesToTime(Minute);
}

inline uint8_t GetPeriodPosByMinute(uint16_t Minute, period_t* Periods, uint8_t PeriodsCount)
{
	uint8_t	i = 0;
	uint16_t* pPeriods = &Periods[0][0];
	uint8_t nPeriodsCount = PeriodsCount * 2;
	for (i = 0; i < nPeriodsCount; i++)
	{
		if (i >= nPeriodsCount - 1)
		{
			return i;
		}
		if (pPeriods[i+1] < pPeriods[i]) {
			//跨天
			uint16_t PeriodMax = 24 * 60 + 0;
			if ((Minute >= pPeriods[i]) && (Minute < PeriodMax))
			{
				return i;
			}
			else if (Minute < pPeriods[i + 1])
			{
				return i;
			}
		}
		else {
			if ((Minute >= pPeriods[i])
				&& (Minute < pPeriods[i + 1]))
			{
				return i;
			}
		}
	}
	ASSERT(0);
	return i;
}

inline uint16_t GetTimePointByHHMMSS(uint32_t HHMMSS, period_t* Periods, uint8_t PeriodsCount)
{
	uint16_t TimePoint = 0;
	uint16_t Minute = XUtil::TimeToMinutes(HHMMSS);
	uint8_t	pos = GetPeriodPosByMinute(Minute, Periods, PeriodsCount);
	uint8_t i = 0, j = (pos + (2 - 1)) / 2, k = pos % 2;
	for (; i < j; i++)
	{
		if (Periods[i][1] < Periods[i][0]) {
			//跨天
			uint16_t PeriodMax = 24 * 60 + 0;
			TimePoint += (uint16_t)(PeriodMax - Periods[i][0]);
			TimePoint += (uint16_t)(Periods[i][1] - 0);
		}
		else {
			TimePoint += Periods[i][1] - Periods[i][0];
		}
	}
	if (pos % 2 == 0)
	{
		if (Periods[i][1] < Periods[i][0]) {
			//跨天
			uint16_t PeriodMax = 24 * 60 + 0;
			if ((Minute >= Periods[i][0]) && (Minute < PeriodMax))
			{
				TimePoint += (uint16_t)(Minute - Periods[i][0]);
			}
			else if (Minute <= Periods[i][1]/*这里<=判断是防止这一节交易结束后还有盘后行情过来，这样可以都算到这一节末*/)
			{
				TimePoint += (uint16_t)(PeriodMax - Periods[i][0]);
				TimePoint += (uint16_t)(Minute - 0);
			}
		}
		else {
			if ((Minute >= Periods[i][0])
				&& (Minute <= Periods[i][1])/*这里<=判断是防止这一节交易结束后还有盘后行情过来，这样可以都算到这一节末*/)
			{
				TimePoint += (uint16_t)(Minute - Periods[i][0]);
			}
		}
	}
	else
	{
		TimePoint -= 1; //空闲时间段向后靠
	}
	return TimePoint;
	//uint16_t TimePoint;
	//uint16_t Minute = HHMMSS_TO_MINUTE(HHMMSS);
	//uint8_t	i;

	//TimePoint = 0;
	//for (i = 0; i < PeriodsCount; i++)
	//{
	//	if (Periods[i][1] < Periods[i][0]) {
	//		//跨天
	//		uint16_t PeriodMax = 24 * 60 + 0;
	//		if ((Minute >= Periods[i][0]) && (Minute < PeriodMax))
	//		{
	//			TimePoint += (uint16_t)(Minute - Periods[i][0]);
	//			break;
	//		}
	//		else if (Minute <= Periods[i][1]/*这里<=判断是防止这一节交易结束后还有盘后行情过来，这样可以都算到这一节末*/)
	//		{
	//			TimePoint += (uint16_t)(PeriodMax - Periods[i][0]);
	//			TimePoint += (uint16_t)(Minute - 0);
	//			break;
	//		}
	//		else
	//		{
	//			TimePoint += (uint16_t)(PeriodMax - Periods[i][0]);
	//			TimePoint += (uint16_t)(Periods[i][1] - 0);
	//		}
	//	}
	//	else {
	//		if (Minute < Periods[i][0])
	//		{
	//			break;
	//		}
	//		else if ((Minute >= Periods[i][0])
	//			&& (Minute <= Periods[i][1])/*这里<=判断是防止这一节交易结束后还有盘后行情过来，这样可以都算到这一节末*/)
	//		{
	//			TimePoint += (uint16_t)(Minute - Periods[i][0]);
	//			break;
	//		}
	//		else if (Minute > Periods[i][1])
	//		{
	//			TimePoint += (uint16_t)(Periods[i][1] - Periods[i][0]);
	//		}
	//		else
	//		{
	//			ASSERT(0);
	//			break;
	//		}
	//	}
	//}

	//return TimePoint;
}

inline bool MergePeriod(period_t dst, period_t src)
{
	bool merge = false;
	if (src[1] < src[0]) {
		//跨天
		unsigned short PeriodMax = 24 * 60 + 0;
		if (dst[1] < dst[0]) {
			//跨天
			merge = true;
			dst[0] = std::min<>(dst[0], src[0]);
			dst[1] = std::max<>(dst[1], src[1]);
		}
		else if(src[0] <= dst[1]) {
			merge = true;
			dst[1] = src[1];
		}
		else if (src[1] >= dst[0]) {
			merge = true;
			dst[0] = src[0];
		}
		else {
			//
		}
	}
	else {
		if (dst[1] < dst[0]) {
			//跨天 
			if(dst[0] <= src[1]) {
				merge = true;
				dst[0] = src[0];
			}
			else if (dst[1] >= src[0]) {
				merge = true;
				dst[1] = src[1];
			}
			else {
				//
			}
		}
		else {
			if ((dst[0] >= src[0] && dst[0] <= src[1]) || (dst[1] >= src[0] && dst[1] <= src[1])) {
				merge = true;
				dst[0] = std::min<>(dst[0], src[0]);
				dst[1] = std::max<>(dst[1], src[1]);
			}
			else {
				//
			}
		}
	}
	return merge;
}

	struct tagCalcParam
	{
		PERIODTYPE Period;
		int PeriodEx;
		PERIODTYPE BasePeriod;
		int Multiple;
	};

	inline PERIODTYPE CalcBasePeriod(PERIODTYPE Period, int PeriodEx, int* pMultiple)
	{
		PERIODTYPE BasePeriod = Period;
		int Multiple = 1;
		switch (Period)
		{
		case CYC_TICK:
			break;
		case CYC_5SEC:
			break;
		case CYC_ANYSEC:
			BasePeriod = CYC_5SEC;
			Multiple = PeriodEx / 5;
			break;
		case CYC_1MIN:
			break;
		case CYC_5MIN:
			BasePeriod = CYC_1MIN;
			Multiple = 5;
			break;
		case CYC_15MIN:
			BasePeriod = CYC_1MIN;
			Multiple = 15;
			break;
		case CYC_30MIN:
			BasePeriod = CYC_1MIN;
			Multiple = 30;
			break;
		case CYC_60MIN:
			BasePeriod = CYC_1MIN;
			Multiple = 60;
			break;
		case CYC_ANYMIN:
			BasePeriod = CYC_1MIN;
			Multiple = PeriodEx;
			break;
		case CYC_DAY:
			break;
		case CYC_WEEK:
			BasePeriod = CYC_DAY;
			Multiple = 5;
			break;
		case CYC_MONTH:
			BasePeriod = CYC_DAY;
			Multiple = 21;
			break;
		case CYC_SEASON:
			BasePeriod = CYC_DAY;
			Multiple = 21 * 3;
			break;
		case CYC_YEAR:
			BasePeriod = CYC_DAY;
			Multiple = 21 * 12;
			break;
		case CYC_ANYDAY:
			BasePeriod = CYC_DAY;
			Multiple = PeriodEx;
		default:
			break;
		}
		if (pMultiple) {
			*pMultiple = Multiple;
		}
		return BasePeriod;
	}

	inline size_t CalcSecondData(tagCalcParam* pParam, KDATA *pOutData, size_t nOutDataCount, KDATA *pSourceData, size_t nSourceCount)
	{
		if (pParam == NULL || pOutData == NULL || pSourceData == NULL) {
			return 0;
		}

		PERIODTYPE Period = pParam->Period;			// 用户周期类型
		uint16_t PeriodEx = pParam->PeriodEx;		// 用户自设周期
		PERIODTYPE BasePeriod = pParam->BasePeriod;	// 用户周期类型
		uint16_t Multiple = pParam->Multiple;		// 用户周期由多少个基本周期组成

		size_t i = 0;
		size_t Result = 0;
		size_t nFirstPos = 0;
		bool bFirst = true;
		for (i = 0; i < nSourceCount; i++)
		{
			bool bSame;
			if (bFirst) {
				bSame = false;
				bFirst = false;
			}
			else {
				bSame = (i - nFirstPos) < Multiple;
			}

			if (!bSame) {
				Result++;
				if (Result > nOutDataCount) {
					break;
				}

				nFirstPos = i;

				if ((Result - 1) < i) { //这样判断是防止pSourceData和pOutData是同一块内存，造成把自身数据清0了，并清理pOutData[Result-1]数据
					memset(&pOutData[Result - 1], 0, sizeof(KDATA));
				}

				pOutData[Result - 1].Date = pSourceData[i].Date;
				pOutData[Result - 1].Time = pSourceData[i].Time;
			}

			if (Result == 0) { // 保护一下，虽几乎不可能Result == 0的
				continue;
			}

			//pOutData[Result-1].Date = pSourceData[i].Date;
			//pOutData[Result-1].Time = pSourceData[i].Time;

			// 第一个内容：处理历史标准数据
			if (pOutData[Result - 1].Open == 0) {
				pOutData[Result - 1].Open = pSourceData[i].Open;
			}
			pOutData[Result - 1].Close = pSourceData[i].Close;
			if (pOutData[Result - 1].High == 0 || pOutData[Result - 1].High < pSourceData[i].High) {
				pOutData[Result - 1].High = pSourceData[i].High;
			}
			if (pOutData[Result - 1].Low == 0 || pOutData[Result - 1].Low > pSourceData[i].Low) {
				pOutData[Result - 1].Low = pSourceData[i].Low;
			}

			pOutData[Result - 1].Volume += pSourceData[i].Volume;
			pOutData[Result - 1].Amount += pSourceData[i].Amount;
		}

		return Result;
	}

	inline size_t CalcMinuteData(tagCalcParam* pParam, KDATA *pOutData, size_t OutDataCount, KDATA *pSourceData, size_t nSourceCount)
	{
		if (pParam == NULL || pOutData == NULL || pSourceData == NULL) {
			return 0;
		}

		PERIODTYPE Period = pParam->Period;			// 用户周期类型
		uint16_t PeriodEx = pParam->PeriodEx;		// 用户自设周期
		PERIODTYPE BasePeriod = pParam->BasePeriod;	// 用户周期类型
		uint16_t Multiple = pParam->Multiple;		// 用户周期由多少个基本周期组成

		if (nSourceCount == 0 || OutDataCount == 0)
			return 0;

		size_t i = 0, j = 0;
		size_t count = 0;
		size_t nLastPos = 0;
		bool First = true;
		for (i = 0; i < nSourceCount && count < OutDataCount; i++)
		{
			if (i == 0) {	// 第一根数据
				First = true;
				//j 用于记录下一根k线的位置
				j = i + Multiple;
			}
			else if (pSourceData[i].Date != pSourceData[nLastPos].Date) { // 用分钟线拼接时日期变化
				if ((count + 1) > OutDataCount) {
					break;
				}
				count++;
				First = true;
				j = i + Multiple;
			}
			else if (BasePeriod == CYC_1MIN) { // 需要用1分钟K线拼接的数据
				if (i >= j) {
					if ((count + 1) > OutDataCount) {
						break;
					}
					count++;
					First = true;
					j = i + Multiple;
				}
			}
			else {
				ASSERT(0);
				return 0;
			}

			if (First) {
				if (count < i) { //这样判断是防止pSourceData和pOutData是同一块内存，造成把自身数据清0了，并清理pOutData[count]数据
					memset(&pOutData[count], 0, sizeof(KDATA));
				}

				//pOutData[count].Date = pSourceData[i].Date;
				//pOutData[count].Time = pSourceData[i].Time;
			}

			// 处理基本数据
			pOutData[count].Date = pSourceData[i].Date;
			pOutData[count].Time = pSourceData[i].Time;
			if (First)
				pOutData[count].Open = pSourceData[i].Open;
			if (pSourceData[i].High > pOutData[count].High)
				pOutData[count].High = pSourceData[i].High;
			if (First || pSourceData[i].Low < pOutData[count].Low)
				pOutData[count].Low = pSourceData[i].Low;
			pOutData[count].Close = pSourceData[i].Close;
			pOutData[count].Volume += pSourceData[i].Volume;
			pOutData[count].Amount += pSourceData[i].Amount;

			First = false;
			nLastPos = i;
		}

		return (count + 1);
	}

}
