#pragma once

#include <Common.hpp>

namespace MTP {

	inline int GetPeriods(const char *Exchange, period_t* pPeriods, uint8_t *pPeriodsCount)
	{
		int i, j;
		uint8_t PeriodsCount = 0;		// 交易时段数量
		period_t Periods[4] = {0}; // 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
		if (std::strcmp(Exchange, XFE_ZZ) == 0)
		{
			PeriodsCount = 4;
			Periods[0][0] = 21 * 60 + 0;
			Periods[0][1] = 2 * 60 + 30;
			Periods[1][0] = 9 * 60 + 0;
			Periods[1][1] = 10 * 60 + 15;
			Periods[2][0] = 10 * 60 + 30;
			Periods[2][1] = 11 * 60 + 30;
			Periods[3][0] = 13 * 60 + 0;
			Periods[3][1] = 15 * 60 + 0;
		}
		else if (std::strcmp(Exchange, XFE_CF) == 0)
		{
			PeriodsCount = 2;
			Periods[0][0] = 9 * 60 + 15;
			Periods[0][1] = 11 * 60 + 30;
			Periods[1][0] = 13 * 60 + 0;
			Periods[1][1] = 15 * 60 + 15;
		}
		else if (std::strcmp(Exchange, XFE_DL) == 0)
		{
			PeriodsCount = 4;
			Periods[0][0] = 21 * 60 + 0;
			Periods[0][1] = 2 * 60 + 30;
			Periods[1][0] = 9 * 60 + 0;
			Periods[1][1] = 10 * 60 + 15;
			Periods[2][0] = 10 * 60 + 30;
			Periods[2][1] = 11 * 60 + 30;
			Periods[3][0] = 13 * 60 + 0;
			Periods[3][1] = 15 * 60 + 0;
		}
		else if (std::strcmp(Exchange, XFE_SH) == 0)
		{
			PeriodsCount = 4;
			Periods[0][0] = 21 * 60 + 0;
			Periods[0][1] = 2 * 60 + 30;
			Periods[1][0] = 9 * 60 + 0;
			Periods[1][1] = 10 * 60 + 15;
			Periods[2][0] = 10 * 60 + 30;
			Periods[2][1] = 11 * 60 + 30;
			Periods[3][0] = 13 * 60 + 0;
			Periods[3][1] = 15 * 60 + 0;
		}
		else if (std::strcmp(Exchange, XFE_NY) == 0)
		{
			PeriodsCount = 2;
			Periods[0][0] = 9 * 60 + 15;
			Periods[0][1] = 11 * 60 + 30;
			Periods[1][0] = 13 * 60 + 0;
			Periods[1][1] = 15 * 60 + 15;
		}
		else if (std::strcmp(Exchange, XSE_SH) == 0)
		{
			PeriodsCount = 2;
			Periods[0][0] = 9 * 60 + 30;
			Periods[0][1] = 11 * 60 + 30;
			Periods[1][0] = 13 * 60 + 0;
			Periods[1][1] = 15 * 60 + 0;
		}
		else if (std::strcmp(Exchange, XSE_SZ) == 0)
		{
			PeriodsCount = 2;
			Periods[0][0] = 9 * 60 + 30;
			Periods[0][1] = 11 * 60 + 30;
			Periods[1][0] = 13 * 60 + 0;
			Periods[1][1] = 15 * 60 + 0;
		}
		else
		{
			ASSERT(0);
		}
		if (pPeriods && pPeriodsCount)
		{
			for (i = 0; i < PeriodsCount && i < *pPeriodsCount; i++)
			{
				pPeriods[i][0] = Periods[i][0];
				pPeriods[i][1] = Periods[i][1];
			}
			*pPeriodsCount = i;
			return i;
		}
		return PeriodsCount;
	}
	
inline void UpdateExchangePeriod(EXCHANGEINFO* exchange, period_t* Periods, uint8_t PeriodsCount)
{
	for (size_t i = 0; i < PeriodsCount; i++)
	{
		bool add = true;
		for (size_t j = 0; j < exchange->PeriodsCount; j++)
		{
			add = !MergePeriod(exchange->Periods[j], Periods[i]);
			if (!add)
			{
				break;
			}
		}
		if (add) {
			exchange->Periods[exchange->PeriodsCount][0] = Periods[i][0];
			exchange->Periods[exchange->PeriodsCount][1] = Periods[i][1];
			exchange->PeriodsCount += 1;
		}
	}
}

inline void UpdateProductPeriod(PRODUCTINFO* product, period_t* Periods, uint8_t PeriodsCount)
{
	for (size_t i = 0; i < PeriodsCount; i++)
	{
		bool add = true;
		for (size_t j = 0; j < product->PeriodsCount; j++)
		{
			add = !MergePeriod(product->Periods[j], Periods[i]);
			if (!add)
			{
				break;
			}
		}
		if (add) {
			product->Periods[product->PeriodsCount][0] = Periods[i][0];
			product->Periods[product->PeriodsCount][1] = Periods[i][1];
			product->PeriodsCount += 1;
		}
	}
}

}
