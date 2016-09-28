#include <iostream>

#include "Course.hpp"
#include "Classroom.hpp"
#include "State.hpp"
#include "GA.hpp"

#include <memory>
#include <ctime>
#include <stdlib.h>

using namespace std;

int main(){
  srand((unsigned int) time(0));
  RoomVector rooms;
  rooms.emplace_back(new Classroom("7606",8,16));
  rooms.emplace_back(new Classroom("7602",8,16));

  CourseVector courses;
  courses.emplace_back(new Course("AI",2, 7, 15, rooms));
  courses.emplace_back(new Course("TBFO",3, 7, 12, rooms));

  shared_ptr<Classroom> room (new Classroom("7606",8,16));
  Course course("AI",2,7,15, {room});
  course.set_schedule(Schedule(room,Day::Tuesday,8,10));
  course.get_schedule();

  Course course2(course);

  cout << course2.get_schedule().start_time << endl;
  cout << course2.get_possible_classroom()[0]->open_time << endl;

//  course->set_schedule(Schedule(room,Day::Monday,8,9));
//  course->get_schedule();

//  Schedule s1(room,Day::Monday,8,9);
//  Schedule s2(room2,Day::Monday,7,9);

//  cout << Schedule::intersect(s1,s2) << endl;

  //genetic_algorithm(rooms, courses, 10, 0.1, 0.3);
  /*
  State s(rooms,courses);
  s.init_random_schedule();

  State u(rooms,courses);
  u.init_random_schedule();

  printf("Conflict: %d\n", s.fitness_score());
  printf("Conflict: %d\n", u.fitness_score());
  crossover(s, u);
*/
  course.printdata();
  course2.printdata();
  State s(rooms,courses);
  s.init_random_schedule();
  for (int i=0 ; i<10 ; i++)
    s = s.mutate();

  return 0;
}
