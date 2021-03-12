//#define WITHOUT_NUMPY
//#include "Python.h"
#include <stdio.h>
#include<stdlib.h>
#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
//#include "matplotlibcpp.h"

//#include <boost/math/distributions/beta.hpp>
//#include "koolplot.h"
//#include "PlotData.h"
//#include "Plotstream.h"

//official
//namespace plt = matplotlibcpp;

class House
{

public:
	
	int dx, dy; //position move
	int x, y; //position
	int no_residents;

};

class Workplace
{
public:

	int dx, dy; //position move
	int x, y; //position
	int no_workers;

};
class Human

{

public:
	int dx, dy; //position move
	int x, y; //position
	int group;
	int action;
	int age;
	bool homeless;
	bool unemployed;
	int x_home;
	int y_home;
	int x_work;
	int y_work;
	House house;
	Workplace work;


	void actionGoHome()

	{
		if ((x - house.x > 200) || (x - house.x < -200) || (y - house.y > 200) || (y - house.y < -200))
		{
			x = house.x;
			y = house.y;
		}
		
	 //IF person not in home, at this function call, move person by 1 px towards home
		if ((x < house.x - house.dx) || (x > house.x + house.dx) || (y < house.y - house.dy) || (y > house.y + house.dy))
		{
			if (x < x_home)
			{
				x = x + dx;
			}
			if (x > x_home)
			{
				x = x - dx;
			}
			if (y < y_home)
			{
				y = y + dy;
			}
			if (y > y_home)
			{
				y = y - dy;
			}
		}
	
	}
	void actionStayHome()

	{
		//If person at home, stay at px or move with probability inside home
		if ((x >= house.x - house.dx) && (x <= house.x + house.dx) && (y >= house.y - house.dy) && (y <= house.y + house.dy))
		{
			unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine generator(seed);
			std::uniform_real_distribution <float> distribution(0, 1);
			float eps = distribution(generator);  // uniform distribution
			//printf("\neps = %f\n",eps);
			if (eps > 0.6 && eps < 0.7)
			{
				x = x + dx;
				y = y + dy;
			}

			if (eps > 0.7 && eps < 0.8)
			{
				x = x - dx;
				y = y - dy;
			}
			if (eps > 0.8 && eps < 0.85)
			{
				x = x - dx;
			}
			if (eps > 0.85 && eps < 0.9)
			{
				x = x + dx;
			}
			if (eps > 0.9 && eps < 0.95)
			{
				y = y - dy;
			}
			if (eps > 0.95)
			{
				y = y + dy;
			}
			if (x > house.x + house.dx)
			{
				x = house.x + house.dx - 1;
			}
			if (x > house.x - house.dx)
			{
				x = house.x - house.dx + 1;
			}
			if (y > house.y + house.dy)
			{
				y = house.y + house.dy - 1;
			}
			
			if (y > house.y - house.dy)
			{
				y = house.y - house.dy + 1;
			}

			}
	}

	void actionGoWork()

	{
		if ((x - work.x > 200) || (x - work.x < -200) || (y - work.y > 200) || (y - work.y < -200))
		{
			x = work.x;
			y = work.y;
		}

		//IF person not in work, at this function call, move person by 1 px towards work
		if ((x < work.x - work.dx) || (x > work.x + work.dx) || (y < work.y - work.dy) || (y > work.y + work.dy))
		{
			if (x < x_work)
			{
				x = x + dx;
			}
			if (x > x_work)
			{
				x = x - dx;
			}
			if (y < y_work)
			{
				y = y + dy;
			}
			if (y > y_work)
			{
				y = y - dy;
			}

		}
	}

	void actionStayAtWork()

	{
		//If person at work, stay at px or move with probability inside work
		if ((x >= work.x - work.dx) && (x <= work.x + work.dx) && (y >= work.y - work.dy) && (y <= work.y + work.dy))
		{
			unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine generator(seed);
			std::uniform_real_distribution <float> distributionfg(0, 1);
			float eps = distributionfg(generator);  // uniform distribution	
			//printf("\neps = %f\n", eps);

			if (eps > 0.6 and eps < 0.7)
			{
				x = x + dx;
				y = y + dy;
			}
			if (eps > 0.7 and eps < 0.8)
			{
				x = x - dx;
				y = y - dy;
			}
			if (eps > 0.8 and eps < 0.85)
			{
				x = x - dx;
			}
			if (eps > 0.85 and eps < 0.9)
			{
				x = x + dx;
			}
			if (eps > 0.9 and eps < 0.95)
			{
				y = y - dy;
			}
			if (eps > 0.95)
			{
				y = y + dy;
			}
			if (x > work.x + work.dx)
			{
				x = work.x + work.dx - 1;
			}

			if (x > work.x - work.dx)
			{
				x = work.x - work.dx + 1;
			}

			if (y > work.y + work.dy)
			{
				y = work.y + work.dy - 1;
			}

			if (y > work.y - work.dy)
			{
				y = work.y - work.dy + 1;
			}


		}
	}
	void actionWalkFree()

	{    
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_real_distribution <float> distribution(0, 1);
		float eps = distribution(generator);  // uniform distribution

		if (eps < 0.25)
		{
			x = x + dx;
		}

		if (eps >= 0.25 && eps <= 0.5)

		{
			y = y + dy;
		}

		if (eps > 0.5 and eps <= 0.75)
		{
			x = x - dx;
		}

		if (eps > 0.75)

		{
			y = y - dy;
		}
	}

};

bool contact(Human& person1, Human& person2)
{
	if ((person1.group != 3 && person2.group != 3) && ((person1.group == 2 && person2.group == 0) || (person1.group == 0 && person2.group == 2)))//question
	{
		if ((person1.x == person2.x) && (person1.y == person2.y))
		{
			if ((person1.x < person1.house.x + person1.house.dx) && (person1.x > person1.house.x - person1.house.dx) && (person1.y < person1.house.y + person1.house.dy) && (person1.y > person1.house.y - person1.house.dy))
			{
				return false;
             }
			else
			{
				if ((person2.x < person2.house.x + person2.house.dx) && (person2.x > person2.house.x - person2.house.dx) && (person2.y < person2.house.y + person2.house.dy) && (person2.y > person2.house.y - person2.house.dy))
				{
					return false;
				}
				else {

					if (person1.group == 2)
					{
						person2.group = 2;
					}
					
					if (person2.group == 2)
					{
						person1.group = 2;
					}
					//printf("\nAdequate Contact\n");
					return true;
				}
				
			}
         
			
		}
		return false;
	}
	return false;
}


int main()

{

	std::ofstream excel_file_initialization; // excel file for exporting initial distribution of houses, workplaces etc.
	std::ofstream exce_file_results; // excel file for exporting final results of SEIR model


//Variables 

	float homeless_rate = 0.0005;
	float initial_infected_rate = 0.01;
	float unemployment_rate = 0.12;
	float business_proportion = 0.01875;
	float business_proportion_informal = 0.4;

//Groups


	int	N = 400;  // Population
	int S = N - (N*initial_infected_rate);  //Susceptible //maybe parameterize
	//printf("\nSinit = %d\n",S);
	int E = 0;   //Exposed
	int I = N*initial_infected_rate;  //Infected
	int	I_hosp = 0;  // Infected hospitalized
	int	I_icu = 0;  // Infected ICU(ventilator)
	int	R = 0;  // Recovered
	int	D = 0;  // Deceased
	int	Q = 0;  // Quarantined
	

	std::vector<float> Sarray;
	std::vector<float> Earray;
	std::vector<float> Iarray;
	std::vector<float> Rarray;

	int contactsPerDay = 0;

//Agents

	std::vector<Human> PPL;  // [x, y, group, action]
	std::string ACT [] = { "Home", "Work", "WalkFree", "Movement" };
	std::vector<House> HOU;
	std::vector<Workplace> WRP;
	
	int family_size_average = 3;
	int no_houses = (int) (N / family_size_average);
	int no_workplaces = (int)((N * business_proportion) + (N * business_proportion_informal));
	printf("\ntotal houses are = %d\n", no_houses);
	printf("\ntottal workplaces are = %d\n", no_workplaces);


//Environment

	int Length = 300;  //each pixel corresponds to area 5x5 meters.
	int Width = 300;

		//for n in range(N) :
		//    PPL[n, 0] = random.uniform(0, Width)
		//    PPL[n, 1] = random.uniform(0, Length)

//Parameters

	int contagion_distance = 1;
	float contagion_probability = 0.9;
	int incubation_time = 5;
	int transmission_time = 8;
	int recovering_time = 20;
	int ICU_limit = N * 0.05;




	float b = 0.9;//0.001;  // infectious rate, controls the rate of spread which represents the probability of transmitting disease between a susceptible and an infectious individual.
	float q = 0.01;  // b / g // contact ratio
	float g = 0.05;  // b / q // recovery rate
	float e = 0.1;  // incubation rate is the rate of latent individuals becoming infectious(average duration of incubation is 1 / s)


//Sim Time

	int T = 0;  // Time horizon
	std::vector<int> timepassed;

//Constraints

	int I_hosp_max = N * 0.02;
	int I_icu_max = N * 0.01;

	timepassed.push_back(T);
	Sarray.push_back(S);
	Earray.push_back(E);
	Iarray.push_back(I);
	Rarray.push_back(R);

	//Random generators

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution <int> distribution_x(0, Length + 1);
	std::uniform_int_distribution <int> distribution_y(0, Width + 1);
	std::uniform_int_distribution <int> distribution_dx_house(1, 3);
	std::uniform_int_distribution <int> distribution_dy_house(1, 3);
	std::uniform_int_distribution <int> distribution_dx_workplace(1, 7);
	std::uniform_int_distribution <int> distribution_dy_workplace(1, 7);
	std::uniform_int_distribution <int> distribution_family(2, 5);
	std::uniform_int_distribution <int> distribution_employees(20, 61);
	std::uniform_real_distribution <float> distribution_homeless(0, 1);
	std::uniform_real_distribution <float> distribution_employeed(0, 1);
	std::_Beta_distribution <float> distribution_age(2, 5);
	
	
	


//Spawn houses
	int f = 0;
	for (f = 0; f < no_houses; f++)
	{
			House house;
			house.x = distribution_x(generator); 		
			house.y = distribution_y(generator);
			house.dx = distribution_dx_house(generator);
			house.dy= distribution_dy_house(generator);
			house.no_residents = 0;
			HOU.push_back(house);
	
	}
		

//Spawn workplaces
	int k = 0;
	for (k = 0; k < no_workplaces; k++)
	{
	
			Workplace workplace;
			workplace.x = distribution_x(generator);
			workplace.y = distribution_y(generator);
			workplace.dx = distribution_dx_workplace(generator);
			workplace.dy = distribution_dy_workplace(generator);
			workplace.no_workers = 0;
			WRP.push_back(workplace);
	}

//Spawn people

	Workplace emptyWork;
	emptyWork.x = 0;
	emptyWork.y = 0;
	emptyWork.dx = 0;
	emptyWork.dy = 0;
	emptyWork.no_workers = 0;

	House emptyHouse;
	emptyHouse.x = 0;
	emptyHouse.y = 0;
	emptyHouse.dx = 0;
	emptyHouse.dy = 0;
	emptyHouse.no_residents = 0;

	int i = 0;
	int j = 0;
	int p = 0;

	/*for (j = 0; j < 1; j++)
	{
		Human person;

		person.x = 1;
		person.y = 1;
		person.dx = 1;
		person.dy = 1;
		person.group = 0;
		person.action = 0;
		person.homeless = 1;
		person.unemployed = 1;
		person.x_home = -1;
		person.y_home = -1;
		person.x_work = -1;
		person.y_work = -1;
		person.house = emptyHouse;
		person.work = emptyWork;
		PPL.push_back(person);

	}*/

	
	for (i = 0; i < N - I; i++)

	{
		Human person;

		person.x = 1;
		person.dx = 1;
		person.dy = 1;
		person.y = 1;
		person.group = 0;
		person.action = 0;
		person.age = (int)(distribution_age(generator) * 100);
		float w = distribution_homeless(generator); //homeless / unemployed
		if (w <= (1 - homeless_rate))

		{
			person.homeless = 0;
		}
		else
		{
			person.homeless = 1;

		}
		if (person.homeless == 1)
		{

			person.unemployed = 1;
		}
		else if (person.age < 15 || person.age > 66)
		{
			person.unemployed = 1;

		}
		else
		{
			float z = distribution_employeed(generator);
			if (z <= (1 - unemployment_rate))

			{
				person.unemployed = 0;
			}
			else
			{
				person.unemployed = 1;

			}
		}
		person.x_home = -1;
		person.y_home = -1;
		person.x_work = -1;
		person.y_work = -1;
		person.house = emptyHouse;
		person.work = emptyWork;
		PPL.push_back(person);


	}


	for (p = 0; p < I; p++)
	{
		Human person;

		person.x = 1;
		person.dx = 1;
		person.dy = 1;
		person.y = 1;
		person.group = 2;
		person.action = 0;
		person.age = 30;
		person.homeless = 0;
		person.unemployed = 0;
		person.x_home = -1;
		person.y_home = -1;
		person.x_work = -1;
		person.y_work = -1;
		person.house = emptyHouse;
		person.work = emptyWork;
		PPL.push_back(person);

	}
						
	shuffle(PPL.begin(), PPL.end(), std::default_random_engine(seed));

	i = 0;
	j = 0;
	p = 0;

	for (j = 0; j < no_houses; j++) //assign people to houses
	{
		int ff = distribution_family(generator);
		int temp = 0;
		//for (Human person : PPL)
		for(i = 0; i < N; i++)
		{
			if (PPL[i].homeless == 0)

			{
				if (PPL[i].x_home == -1)
				{
					PPL[i].house = HOU[j];
					PPL[i].x_home = HOU[j].x;
					PPL[i].y_home = HOU[j].y;
					PPL[i].x = HOU[j].x;
					PPL[i].y = HOU[j].y;
					HOU[j].no_residents = HOU[j].no_residents + 1;
						
						temp = temp + 1;
						if (temp == ff)
						{
							break;
						}
			     }
			}
		}
	}
	
		
	i = 0;
	j = 0;
	for (j = 0; j < no_workplaces; j++) //assign people to workplaces
	{
		int ff = distribution_employees(generator);
		int temp = 0;
		//for (Human person : PPL)
			for (i = 0; i < N; i++)
		{
			if (PPL[i].unemployed == 0)
			{
				if (PPL[i].x_work == -1)
				{
					PPL[i].work = WRP[j];
					PPL[i].x_work = WRP[j].x;
					PPL[i].y_work = WRP[j].y;
					WRP[j].no_workers = WRP[j].no_workers + 1;
					temp = temp + 1;
					if (temp == ff)
					{
						break;
					}
                 }
             }
		}
		}

	std::vector <int> housesx;
	std::vector <int> housesy;
	std::vector <int> worksx;
	std::vector <int> worksy;

	

	/*for (House house : HOU)
	{
		housesx.push_back(house.x);
		housesy.push_back(house.y);
		
		}*/
	
	//plt::plot(housesx, housesy, "r.");
	//plt::show();

	/*for (Workplace workplace : WRP)
	{
		worksx.push_back(workplace.x);
		worksy.push_back(workplace.y);

		}
		*/
	//Plotdata z(-3.0, 3.0), u = sin(z) - 0.5 * z;
	//plot(z, u);

	/*for (House house : HOU)

	{
		//printf("\n position_house_x = %d\n", house.x);
		//printf("\n position_house_y = %d\n", house.y);
		//printf("\n house residents = %d\n", house.no_residents);

	}*/
	
	/*for (Workplace workplace : WRP)

	{
		//printf("\n position_work_x = %d\n", workplace.x);
		//printf("\n employees = %d\n", workplace.no_workers);
		//printf("\n work_x_pos = %d\n", workplace.x);
		//printf("\n work_y_pos = %d\n", workplace.y);
	}
	*/
	/*for (Human person1 : PPL)
	{

		//if person.x_work > 300 & person.x_work < 0 & person.y_work > 300 & person.y_work < 0 & person.x_home > 300 & person.x_home < 0 & person.y_home > 300 & person.y_home < 0:
		//printf("\n position_x_home = %d\n", person1.x_home);
		//printf("\n position_y_home = %d\n", person1.y_home);
		//printf("\n position_x_work = %d\n", person1.x_work);
		//printf("\n position_y_work = %d\n", person1.y_work);
		//printf("\n homeless = %d\n", person1.homeless);
		//printf("\n unemployeed = %d\n", person1.unemployed);
		//printf("\n person_age = %d\n", person1.age);


	
	}*/
	
	i = 0;

	excel_file_initialization.open("initiallisation_houses_works.csv");  //excel_file.open("initiallisation_houses_works.csv", std::ios::app);  this way if we dont want to erase old data
	
		for (i = 0; i < N; i++)

		{
			if (i < (no_houses - 1))

			{
				excel_file_initialization << PPL[i].x_home << "," << PPL[i].y_home << "," << WRP[i].x << "," << WRP[i].y << "," << HOU[i].x << "," << HOU[i].y << std::endl;


			}

			if(i > (no_houses - 1) && i < (no_workplaces - 1))
			{
				excel_file_initialization << PPL[i].x_home << "," << PPL[i].y_home << "," << WRP[i].x << "," << WRP[i].y << std::endl;
			}

			if (i > (no_workplaces - 1))
			{
				excel_file_initialization << PPL[i].x_home << "," << PPL[i].y_home << std::endl;
			}

		}

	
	//excel_file << house.x << "," << house.y << std::endl;
	

	//experiment
	int timestamp = 0;
	int hour = 0;
	i = 0;
	j = 0;
	bool done = false;

	while (done == false)
	{
		printf("\nS: %d, E: %d, I: %d, R: %d, time: %d\n", S, E, I, R, T);

		for (hour = 0; hour < 24; hour++)
		{
			if (hour >= 0 && hour < 8)

			{
				for (timestamp = 0; timestamp < 200; timestamp++)
				{
					for (i = 0; i < N; i++)
					{
						if (PPL[i].homeless == 0)
						{

							
							PPL[i].actionGoHome();
							if (i == 50)
							{
								//printf("\ncurrent position of person 1: %d\n", PPL[i].x_home);
								//printf("\ncurrent position of person 1: %d\n", PPL[i].x);

							}
							PPL[i].actionStayHome();
							
							if (i == 50)
							{
								//printf("\ncurrent position of person 1: %d\n", PPL[i].y_home);
								//printf("\ncurrent position of person 1: %d\n", PPL[i].y);

							}

							for (j = 0; j < N; j++)
							{
								if ((i != j) && (contact(PPL[i], PPL[j]) == true)) //explain
								{

									contactsPerDay = contactsPerDay + 1;
								}

							}

						}

					}

				}

			}
			if (hour > 7 && hour < 12)
			{
				timestamp = 0;
				i = 0;
				j = 0;

				for (timestamp = 0; timestamp < 200; timestamp++)
				{
					for (i = 0; i < N; i++)
					{
						if (PPL[i].unemployed == 0)
						{
							
							PPL[i].actionGoWork();
							if (i == 30)
							{
								//printf("\nwork position of person 1: %d\n", PPL[i].x_work);
								//printf("\ncurrent position of person 1: %d\n", PPL[i].x);
//
							}
							PPL[i].actionStayAtWork();
							if (i == 80)
							{
								//printf("\nwork position of person 1: %d\n", PPL[i].x_work);
								//printf("\ncurrent position of person 1: %d\n", PPL[i].x);

							}
							for (j = 0; j < N; j++)
							{
								if ((i != j) && (contact(PPL[i], PPL[j]) == true)) //explain
								{

									contactsPerDay = contactsPerDay + 1;
								}

							}

						}

					}

				}

			}

			if (hour > 11 && hour < 13)
			{
				timestamp = 0;
				i = 0;
				j = 0;

				for (timestamp = 0; timestamp < 200; timestamp++)
				{
					for (i = 0; i < N; i++)
					{
						
						PPL[i].actionWalkFree();
						if (i == 50)
						{
							//printf("\ncurrent position of person 1_x: %d\n", PPL[i].x);
							//printf("\ncurrent position of person 1_y: %d\n", PPL[i].y);

						}

						for (j = 0; j < N; j++)
						{
							if ((i != j) && (contact(PPL[i], PPL[j]) == true)) //explain
							{
								contactsPerDay = contactsPerDay + 1;
							}

						}

					}

				}

			}

			if (hour > 14 && hour < 19)
			{
				timestamp = 0;
				i = 0;
				j = 0;

				for (timestamp = 0; timestamp < 200; timestamp++)
				{
					for (i = 0; i < N; i++)
					{
						if (PPL[i].unemployed == 0)
						{
							
							PPL[i].actionGoWork();
							PPL[i].actionStayAtWork();
							for (j = 0; j < N; j++)
							{
								if ((i != j) && (contact(PPL[i], PPL[j]) == true)) //explain
								{
									contactsPerDay = contactsPerDay + 1;
								}
							}
						}
					}
				}
			}
			if (hour > 18 && hour < 0)
			{
				timestamp = 0;
				i = 0;
				j = 0;

				for (timestamp = 0; timestamp < 200; timestamp++)
				{
					for (i = 0; i < N; i++)
					{
						
						PPL[i].actionWalkFree();
						for (j = 0; j < N; j++)
						{
							if ((i != j) && (contact(PPL[i], PPL[j]) == true)) //explain
							{
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}

			//printf("\nHour passed: %d\n", hour);
		}


		        T = T + 1;
			    int S1 = S;
				int E1 = E;
				int I1 = I;
				int R1 = R;

				float Sdot = (-b * contactsPerDay*S*I)/N;
				float Edot = ((b * contactsPerDay*S*I) /N) - e * E;
				float Idot = e * E - g * I;
				float Rdot = g * I;

				S = S1 + Sdot;
				E = E1 + Edot;
				I = I1 + Idot;
				R = R1 + Rdot;

				if (S < 0)
				{
					S = 0;
			     }

				if (E > N)
				{
					E = N;
				}

				if (I > N)
				{
					I = N;
				}

				if (R > N)
				{
					R = N;
				}
		

				timepassed.push_back(T);
				Sarray.push_back(S);
				Earray.push_back(E);
				Iarray.push_back(I);
				Rarray.push_back(R);

				contactsPerDay = 0;

				
				
				//excel_file << Sarray[0] << std:: endl;
				std::vector <int> people_group;
				

				/*for (Human person : PPL)
				{


					people_group.push_back(person.group);
					if (person.group == 2)
					{
						
						printf("\nInfected\n");
					}

					}*/

				if (T == 60) //150
				{

					done = true;
				}
		
	}
	

	exce_file_results.open("SEIR_Results.csv");
	i = 0;

	printf("\nPreparing results excel file...\n");
	for (i = 0; i < T; i++)

	{
		exce_file_results << Sarray[i] << "," << Earray[i] << "," << Iarray[i] << "," << Rarray[i] << "," << timepassed[i] << std::endl;

	}

	exce_file_results.close();


	printf("\nResults excel file done!\n");

	printf("\nSimulation completed!\n");

	printf("\nPress a button to finish...\n");

	getchar();
};