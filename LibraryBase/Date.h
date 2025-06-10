#pragma once
#include "KTime.h"
#include <string>
#include <optional>

namespace dk
{
	class Date
	{
	public:
		using Year = int;
		using Month = int;
		using Day = int;

	public:
		Date();
		Date(Year year, Month month, Day day);

		Year year() const { return m_year; }
		Month month() const { return m_month; }
		Day day() const { return m_day; }

		time_t to_time_t() const;
		std::string toDateString() const;
		std::string toDateTimeString() const {
			return toDateString() + " 00:00:00";
		}

		Date from_time_t(time_t time) const
		{
			std::tm tm = Time::to_tm(time);
			return Date(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
		}

		// 날짜 문자열을 Date 객체로 변환
		Date fromDateString(const std::string& dateString) const
		{
			Date newDate = *this;
			newDate.fromDateString(dateString);
			return newDate;
		}
		// 특정 날짜에 지정한 일수를 더하기
		static Date daysAfter(const Date& date, int days)
		{
			Date newDate = date;
			newDate.m_day += days;
			return newDate;
		}
		static Date today()
		{
			return daysAfter(Date(), 0);
		}

		bool operator==(const Date& other) const
		{
			return m_year == other.m_year && m_month == other.m_month && m_day == other.m_day;
		}
		bool operator!=(const Date& other) const
		{
			return !(*this == other);
		}
		bool operator<(const Date& other) const 
		{
			if (m_year != other.m_year) return m_year < other.m_year;
			if (m_month != other.m_month) return m_month < other.m_month;
			return m_day < other.m_day;
		}
		bool operator<=(const Date& other) const 
		{
			return *this < other || *this == other;
		}

	private:
		Year m_year;
		Month m_month;
		Day m_day;
	};
}