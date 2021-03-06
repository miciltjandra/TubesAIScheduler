#include "Course.hpp"
#include <cassert>
#include <algorithm>

const char* ScheduleNotDefined::what() const throw() {
  return "Schedule is not defined";
}

ScheduleInvalid::ScheduleInvalid(const string& variable) : message("Schedule\'s "+variable+" is not valid") {}

const char* ScheduleInvalid::what() const throw() {
  return message.c_str();
}

Course::Course(const string& n, const int d, const int ot, const int ct, const vector<shared_ptr<Classroom>>& rooms, const set<Day>& days )
:
name(n),
duration(d),
open_time(ot),
close_time(ct),
schedule(nullptr),
possible_classroom(rooms),
possible_day(days)
{

}

Course::Course(const Course& c) : Course(c.name,c.duration,c.open_time,c.close_time,c.possible_classroom,c.possible_day) {
  if( c.schedule )
    set_schedule(*c.schedule);
}

const vector<shared_ptr<Classroom>>& Course::get_possible_classroom() const {
  return possible_classroom;
}

const set<Day> Course::get_possible_day() const {
  return possible_day;
}


const Schedule& Course::get_schedule() const {
  if( !schedule )
    throw ScheduleNotDefined();
  return *schedule;
}

void Course::set_schedule(const Schedule& s){
  check_schedule(s);
  schedule.reset();
  schedule = unique_ptr<Schedule>( new Schedule(s) );
}

void Course::check_schedule() const {
  if( !schedule )
    throw ScheduleNotDefined();
  check_schedule(*schedule);
}

void Course::check_schedule(const Schedule& s) const {
  switch (get_schedule_error(s))
  {
  case StartTimeError:
    throw ScheduleInvalid("start_time");
  case EndTimeError:
    throw ScheduleInvalid("end_time");
  case DurationError:
    throw ScheduleInvalid("duration");
  case DayError:
    throw ScheduleInvalid("day");
  case ClassroomError:
    throw ScheduleInvalid("classroom");
  default:
    break;
  }
}

ScheduleInvalidErrorType Course::get_schedule_error(const Schedule & s) const
{
  const int ot = std::max(s.room->open_time, open_time);
  const int ct = std::min(s.room->close_time, close_time);

  if (s.start_time < ot) return StartTimeError;
  if (s.end_time > ct) return EndTimeError;
  if (s.end_time - s.start_time != duration) return DurationError;
  if (!(std::find(possible_day.begin(), possible_day.end(), s.day) != possible_day.end())) return DayError;
  if (!(std::find(possible_classroom.begin(), possible_classroom.end(), s.room) != possible_classroom.end())) return ClassroomError;

  return NoError;
}

std::ostream& operator<< (std::ostream& stream, const Course& course){
  stream << "Course " << course.name << ", " << *(course.schedule);
  return stream;
}
