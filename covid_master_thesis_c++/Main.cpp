#include <stdio.h>
#include<stdlib.h>
#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>

//original

int house_bill = 150;

class House
{

public:

	int dx, dy; //position move
	int x, y; //position
	int no_residents;
	int social_stratum;
	float home_income;
	float home_expenses;
	float home_wealth;

};


int workplace_bill = 1000;

class Workplace
{
public:

	int dx, dy; //position move
	int x, y; //position
	int no_workers;
	int social_stratum;
	float workplace_income;
	float workplace_expenses;
	float workplace_wealth;

	void actionPayBills()
	{

		workplace_wealth = workplace_wealth - workplace_bill;
	}
	

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
	int personal_income;
	float personal_expenses;
	float personal_wealth;
	int social_stratum;
	
	House house;
	Workplace work;

	int Eday;
	int Iday;
	int Ihospday;
	int IICUday;
	int Rday;


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
			std::uniform_real_distribution <float> distribution(0, 1);
			float eps = distribution(generator);  // uniform distribution	


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

	void actionShopping()
	{

		personal_wealth = personal_wealth - (personal_expenses / 60); // share the money to the workplaces

	}

	void actionGetPaid()
	{
		work.workplace_wealth = work.workplace_wealth - personal_income;
		personal_wealth = personal_wealth + personal_income;
		
	}

	void actionPayBills()
	{
		personal_wealth = personal_wealth - (house_bill / house.no_residents);

	}

		
	

};


float homeless_rate = 0.0005;
float initial_infected_rate = 0.01;
float unemployment_rate = 0.12;
float business_proportion = 0.01875;
float business_proportion_informal = 0.4;
float total_Wealth = 1000000000;
float public_Wealth_rate = 0.01;
float business_Wealt_rate = 0.05;
float personal_Wealth_rate = 0.04;
float public_Wealth_total = total_Wealth * public_Wealth_rate;
float business_Wealth_total = total_Wealth * business_Wealt_rate;
float personal_Wealth_total = total_Wealth * personal_Wealth_rate;
float min_income = 900;
float min_expense = 600;



int N = 400; // Population
int I_init = N * initial_infected_rate;  //Infected
int S_init = N - (N * initial_infected_rate);


int MostPoor = 0; //Most poor
int Poor = 0; // Poor
int WorkingClass = 0; // working class
int Rich = 0; //Rich
int MostRich = 0; // Most Rich


int family_size_average = 3;
//int no_houses = (N - N * homeless_rate) / family_size;
//int no_workplaces = 20;

int no_houses = (int)(N / family_size_average);
int no_workplaces = (int)((N * business_proportion) + (N * business_proportion_informal));

//Environment

int Length = 300;  //each pixel corresponds to area 5x5 meters.
int Width = 300;

//for n in range(N) :
//    PPL[n, 0] = random.uniform(0, Width)
//    PPL[n, 1] = random.uniform(0, Length)

//Pandemics Parameters

int contagion_distance = 1;
float contagion_probability = 0.9;
int incubation_time = 5;
int transmission_time = 8;
int recovering_time = 20;
int ICU_limit = N * 0.05;



float b = 0.001;//0.001;  // infectious rate, controls the rate of spread which represents the probability of transmitting disease between a susceptible and an infectious individual.
float q = 0.01;  // b / g // contact ratio
float g = 0.05;  // b / q // recovery rate
float e = 0.1;  // incubation rate is the rate of latent individuals becoming infectious(average duration of incubation is 1 / s)


//Constraints

int I_hosp_max = N * 0.02;
int I_icu_max = N * 0.01;


bool contact(Human& person1, Human& person2, int day)
{
	if ((person1.x == person2.x) && (person1.y == person2.y))
	{
		if ((person1.group != 3 && person2.group != 3) && ((person1.group == 2 && person2.group == 0) || (person1.group == 0 && person2.group == 2)))//question
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
						person2.group = 1;
						person2.Eday = day;
					}

					if (person2.group == 2)
					{
						person1.group = 1;
						person1.Eday = day;
					}

					return true;
				}

			}


		}
		return false;
	}
	return false;
}

std::vector<Human> PPL;
std::vector<House> HOU;
std::vector<Workplace> WRP;

void EtoItransition(int N, int T) {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	for (int i = 0; i < N; i++) {
		if (PPL[i].group == 1 && T - PPL[i].Eday == 5) {
			std::default_random_engine generator(seed);
			std::uniform_real_distribution <float> distribution(0, 1);
			float eps = distribution(generator);  // uniform distribution
			if (eps <= contagion_probability) {
				PPL[i].group = 2;
				PPL[i].Iday = T;
				//printf("\nGroup: %d, Iday: %d", PPL[i].group, PPL[i].Iday);
				if (PPL[i].age >= 0 && PPL[i].age <= 9) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.001) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.05) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 10 && PPL[i].age <= 19) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.003) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.05) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 20 && PPL[i].age <= 29) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.012) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.05) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 30 && PPL[i].age <= 39) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.032) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.05) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 40 && PPL[i].age <= 49) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.049) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.063) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 50 && PPL[i].age <= 59) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.102) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.122) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 60 && PPL[i].age <= 69) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.166) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.274) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 70 && PPL[i].age <= 79) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.243) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.432) {
							PPL[i].group = 5;
						}
					}
				}
				if (PPL[i].age >= 80) {
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.273) {
						PPL[i].group = 4;
						std::default_random_engine generator(seed);
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution(generator);  // uniform distribution
						if (eps < 0.709) {
							PPL[i].group = 5;
						}
					}
				}
			}
		}
		if ((PPL[i].group == 2 || PPL[i].group == 4 || PPL[i].group == 5) && T - PPL[i].Iday == 20) {
			PPL[i].group = 3;
		}
	}
}






int main()
{

	std::ofstream excel_file_initialization; // excel file for exporting initial distribution of houses, workplaces etc.
	std::ofstream exce_file_results; // excel file for exporting final results of SEIR model


	printf("\ntotal houses are = %d\n", no_houses);
	printf("\ntottal workplaces are = %d\n", no_workplaces);
	//Variables 
		/*
		float homeless_rate = 0.0005;
		float initial_infected_rate = 0.01;
		float unemployment_rate = 0.12;
		float business_proportion = 0.01875;
		float business_proportion_informal = 0.4;
		float total_Wealth = 1000000000;
		float public_Wealth_rate = 0.01;
		float business_Wealt_rate = 0.05;
		float personal_Wealth_rate = 0.04;
		float public_Wealth = total_Wealth * public_Wealth_rate;
		float business_Wealth = total_Wealth * business_Wealt_rate;
		float personal_Wealth = total_Wealth * personal_Wealth_rate;
		float min_income = 900;
		float min_expense = 600;
		*/
		/* Social Stratum explanation
		each workplace and house are given a social stratum
		according to the house stratum, the respective stratum is assigned to the people that stay at the home --> the family members

		1: Most Poor
		2:Poor
		3:Working class
		4:Rich
		5:Most Rich
		*/

		//Groups

			/*
			int S = 399;  //Susceptible //maybe parameterize
			int E = 0;   //Exposed
			int I = 1;  //Infected
			int	I_hosp = 0;  // Infected hospitalized
			int	I_icu = 0;  // Infected ICU(ventilator)
			int	R = 0;  // Recovered
			int	D = 0;  // Deceased
			int	Q = 0;  // Quarantined
			int	N = S + E + I + R;  // Population
			*/
			/*
			int N = 400; // Population
			int I_init = N * initial_infected_rate;  //Infected
			int S_init = N - (N * initial_infected_rate);


			int MostPoor = 0; //Most poor
			int Poor = 0; // Poor
			int WorkingClass = 0; // working class
			int Rich = 0; //Rich
			int MostRich = 0; // Most Rich

			*/

			//Agents

				//std::vector<Human> PPL;
				//std::vector<House> HOU;
				//std::vector<Workplace> WRP;
				/*
				int family_size_average = 3;
				//int no_houses = (N - N * homeless_rate) / family_size;
				//int no_workplaces = 20;

				int no_houses = (int)(N / family_size_average);
				int no_workplaces = (int)((N * business_proportion) + (N * business_proportion_informal));
				printf("\ntotal houses are = %d\n", no_houses);
				printf("\ntottal workplaces are = %d\n", no_workplaces);

				*/
				/*
			//Environment

				int Length = 300;  //each pixel corresponds to area 5x5 meters.
				int Width = 300;

					//for n in range(N) :
					//    PPL[n, 0] = random.uniform(0, Width)
					//    PPL[n, 1] = random.uniform(0, Length)

			//Pandemics Parameters

				int contagion_distance = 1;
				float contagion_probability = 0.9;
				int incubation_time = 5;
				int transmission_time = 8;
				int recovering_time = 20;
				int ICU_limit = N * 0.05;



				float b = 0.001;//0.001;  // infectious rate, controls the rate of spread which represents the probability of transmitting disease between a susceptible and an infectious individual.
				float q = 0.01;  // b / g // contact ratio
				float g = 0.05;  // b / q // recovery rate
				float e = 0.1;  // incubation rate is the rate of latent individuals becoming infectious(average duration of incubation is 1 / s)


			//Constraints

				int I_hosp_max = N * 0.02;
				int I_icu_max = N * 0.01;
				*/


				//Random generators

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution <int> distribution_x(0, Length + 1);
	std::uniform_int_distribution <int> distribution_y(0, Width + 1);
	std::uniform_int_distribution <int> distribution_dx_house(1, 3);
	std::uniform_int_distribution <int> distribution_dy_house(1, 3);
	std::uniform_int_distribution <int> distribution_dx_workplace(1, 7);
	std::uniform_int_distribution <int> distribution_dy_workplace(1, 7);
	std::uniform_int_distribution <int> distribution_family(2, 4);
	std::uniform_int_distribution <int> distribution_employees(20, 60);
	std::uniform_real_distribution <float> distribution_homeless(0, 1);
	std::uniform_real_distribution <float> distribution_employeed(0, 1);
	std::_Beta_distribution <float> distribution_age(2, 5);
	std::uniform_int_distribution <int> distribution_social_stratum(1, 5);
	std::uniform_int_distribution <int> distribution_unemployeed_social_stratum(1, 2);


	//Spawn houses
	for (int i = 0; i < no_houses; i++)
	{
		House house;
		house.x = distribution_x(generator);
		house.y = distribution_y(generator);
		house.dx = distribution_dx_house(generator);
		house.dy = distribution_dy_house(generator);
		house.social_stratum = distribution_social_stratum(generator);
		house.no_residents = 0;
		HOU.push_back(house);
	}


	//Spawn workplaces
	for (int i = 0; i < no_workplaces; i++)
	{
		Workplace workplace;
		workplace.x = distribution_x(generator);
		workplace.y = distribution_y(generator);
		workplace.dx = distribution_dx_workplace(generator);
		workplace.dy = distribution_dy_workplace(generator);
		workplace.social_stratum = distribution_social_stratum(generator);
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

	/*
	for (int i = 0; i < 1; i++)
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

	}
	*/

	for (int i = 0; i < N - I_init; i++)

	{
		Human person;

		person.x = 1;
		person.y = 1;
		person.dx = 1;
		person.dy = 1;
		person.group = 0;
		person.action = 0;
		person.age = (int)(distribution_age(generator) * 100);
		float w = distribution_homeless(generator);
		if (w <= (1 - homeless_rate))

		{
			person.homeless = 0;
		}
		else
		{
			person.homeless = 1;
			person.unemployed = 1;
		}
		if (person.age < 15 || person.age > 66)
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


	for (int i = 0; i < I_init; i++)
	{
		Human person;

		person.x = 1;
		person.y = 1;
		person.dx = 1;
		person.dy = 1;
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
		person.Iday = 0;
		PPL.push_back(person);

	}

	shuffle(PPL.begin(), PPL.end(), std::default_random_engine(seed));


	for (int i = 0; i < no_houses; i++) //assign people to houses
	{
		int ff = distribution_family(generator);
		int temp = 0;
		//for (Human person : PPL)
		for (int j = 0; j < N; j++)
		{
			if (PPL[j].homeless == 0)

			{
				if (PPL[j].x_home == -1)
				{
					PPL[j].house = HOU[i];
					PPL[j].x_home = HOU[i].x;
					PPL[j].y_home = HOU[i].y;
					PPL[j].x = HOU[i].x;
					PPL[j].y = HOU[i].y;
					HOU[i].no_residents = HOU[i].no_residents + 1;
					PPL[j].social_stratum = HOU[i].social_stratum;

					temp = temp + 1;
					if (temp == ff)
					{
						break;
					}
				}
			}
		}
	}



	for (int i = 0; i < no_workplaces; i++) //assign people to workplaces
	{
		int ff = distribution_employees(generator);
		int temp = 0;
		//for (Human person : PPL)
		for (int j = 0; j < N; j++)
		{
			if (PPL[j].unemployed == 0)
			{
				if (PPL[j].social_stratum == WRP[i].social_stratum)
				{
					if (PPL[j].x_work == -1)
					{
						PPL[j].work = WRP[i];
						PPL[j].x_work = WRP[i].x;
						PPL[j].y_work = WRP[i].y;
						WRP[i].no_workers = WRP[i].no_workers + 1;
						temp = temp + 1;
						if (temp == ff)
						{
							break;
						}
					}
				}
			}
		}
	}

	std::vector <int> housesx;
	std::vector <int> housesy;
	std::vector <int> worksx;
	std::vector <int> worksy;


	for (int j = 0; j < no_houses; j++)

	{

		if (HOU[j].social_stratum == 1)
		{
			MostPoor = MostPoor + 1;
		}

		else if (HOU[j].social_stratum == 2)
		{
			Poor = Poor + 1;
		}

		else if (HOU[j].social_stratum == 3)
		{
			WorkingClass = WorkingClass + 1;
		}

		else if (HOU[j].social_stratum == 4)
		{
			Rich = Rich + 1;
		}
		else if (HOU[j].social_stratum == 5)
		{
			MostRich = MostRich + 1;
		}
	}
	printf("\nMP = %d, P = %d, wc = %d, r = %d, mr = %d \n", MostPoor, Poor, WorkingClass, Rich, MostRich);

	for (int i = 0; i < N; i++)

	{
		if (PPL[i].unemployed == 1)
		{

			PPL[i].personal_income = 0;
		}

		else if (PPL[i].unemployed == 0)

		{

			if (PPL[i].social_stratum == 1)
			{

				PPL[i].personal_income = 900;
				PPL[i].personal_expenses = 600;
				PPL[i].personal_wealth = (personal_Wealth_total * 0.0362) / MostPoor;
			}

			else if (PPL[i].social_stratum == 2)
			{

				PPL[i].personal_income = 950;
				PPL[i].personal_expenses = 650;
				PPL[i].personal_wealth = (personal_Wealth_total * 0.0788) / Poor;
			}

			else if (PPL[i].social_stratum == 3)
			{

				PPL[i].personal_income = 1200;
				PPL[i].personal_expenses = 800;

				PPL[i].personal_wealth = (personal_Wealth_total * 0.1262) / WorkingClass;
			}

			else if (PPL[i].social_stratum == 4)
			{

				PPL[i].personal_income = 1500;
				PPL[i].personal_expenses = 1000;

				PPL[i].personal_wealth = (personal_Wealth_total * 0.4388) / Rich;
			}
			else if (PPL[i].social_stratum == 5)
			{

				PPL[i].personal_income = 2000;
				PPL[i].personal_expenses = 1300;
				PPL[i].personal_wealth = (personal_Wealth_total * 0.5612) / MostRich;
			}
		}

	}

	/*for (House house : HOU)
	{
		housesx.push_back(house.x);
		housesy.push_back(house.y);

		}

	for (Workplace workplace : WRP)
	{
		worksx.push_back(workplace.x);
		worksy.push_back(workplace.y);

		}*/

		//Plotdata z(-3.0, 3.0), u = sin(z) - 0.5 * z;
		//plot(z, u);

		/*for (House house : HOU)

		{
			//printf("\n position_house_x = %d\n", house.x);
			printf("\n house residents = %d\n", house.no_residents);

		}*/

		/*for (Workplace workplace : WRP)

		{
			//printf("\n position_work_x = %d\n", workplace.x);
			printf("\n employees = %d\n", workplace.no_workers);
		}
		*/
		/*for (Human person1 : PPL)
		{

			//if person.x_work > 300 & person.x_work < 0 & person.y_work > 300 & person.y_work < 0 & person.x_home > 300 & person.x_home < 0 & person.y_home > 300 & person.y_home < 0:
			printf("\n position_x = %d\n", person1.x_work);

		}*/


		// ################### EXCEL FILE

	excel_file_initialization.open("initiallisation_houses_works.csv");  //excel_file.open("initiallisation_houses_works.csv", std::ios::app);  this way if we dont want to erase old data

	for (int i = 0; i < N; i++)

	{
		if (i < (no_houses - 1))

		{
			excel_file_initialization << PPL[i].x_home << "," << PPL[i].y_home << "," << WRP[i].x << "," << WRP[i].y << "," << HOU[i].x << "," << HOU[i].y << std::endl;


		}

		if (i > (no_houses - 1) && i < (no_workplaces - 1))
		{
			excel_file_initialization << PPL[i].x_home << "," << PPL[i].y_home << "," << WRP[i].x << "," << WRP[i].y << std::endl;
		}

		if (i > (no_workplaces - 1))
		{
			excel_file_initialization << PPL[i].x_home << "," << PPL[i].y_home << std::endl;
		}

	}

	// debbuging

	printf("\nxhome = %d, yhome = %d \n", PPL[55].x_home, PPL[55].y_home);
	printf("\nxwork = %d, ywork = %d \n", PPL[55].x_work, PPL[55].y_work);



	// plots
	std::vector<float> Sarray;
	std::vector<float> Earray;
	std::vector<float> Iarray;
	std::vector<float> Rarray;
	std::vector<int> timepassed;


	// INITIAL CONDITION
	int contactsPerDay = 0;
	int S = S_init;
	int E = 0;
	int I = I_init;
	int R = 0;
	int Ih = 0;
	int Is = 0;
	int T = 0;


	bool done = false;
	while (done == false)
	{
		timepassed.push_back(T);
		Sarray.push_back(S);
		Earray.push_back(E);
		Iarray.push_back(I);
		Rarray.push_back(R);
		//printf("\nS: %d, E: %d, I: %d, R: %d, time: %d\n", S, E, I, R, T);
		EtoItransition(N, T);

		/*
		// ###################   E TO I TRANSITION & I distribution & I to R transition
		for (int i = 0; i < N; i++)
		{
			if (PPL[i].group == 1 && T - PPL[i].Eday == 5)
			{
				std::default_random_engine generator1;
				std::uniform_real_distribution <float> distribution1(0, 1);
				float eps = distribution1(generator1);  // uniform distribution
				if (eps > 0.1)
				{
					PPL[i].group = 2;
					PPL[i].Iday = T;
					//printf("\nGroup: %d, Iday: %d", PPL[personcheck].group, PPL[personcheck].Iday);
					if (PPL[i].age >= 0 && PPL[i].age <= 9) {
						std::default_random_engine generator;
						std::uniform_real_distribution <float> distribution(0, 1);
						float eps = distribution1(generator);  // uniform distribution
						if (eps < 0.001) {
							PPL[i].group = 4;
							std::default_random_engine generator;
							std::uniform_real_distribution <float> distribution(0, 1);
							float eps = distribution1(generator);  // uniform distribution
							if (eps < 0.05) {
								PPL[i].group = 5;
							}
						}
					}
				}
			}
			if ((PPL[i].group == 2 || PPL[i].group == 4 || PPL[i].group == 5) && T - PPL[i].Iday == 20) {
				PPL[i].group = 3;
			}
		}
		*/




		// ################## PRINT NUMBER OF PEOPLE IN GROUPS 

		S = 0;
		E = 0;
		I = 0;
		R = 0;
		Ih = 0;
		Is = 0;

		for (int i = 0; i < N; i++)
		{
			if (PPL[i].group == 0)
			{
				S++;
			}
			if (PPL[i].group == 1)
			{
				E++;
			}
			if (PPL[i].group == 2)
			{
				I++;
			}
			if (PPL[i].group == 3)
			{
				R++;
			}
			if (PPL[i].group == 4)
			{
				Ih++;
			}
			if (PPL[i].group == 5)
			{
				Is++;
			}
		}
		printf("\nS: %d, E: %d, I: %d, R: %d, Ih: %d, Is: %d, Contacts last day: %d", S, E, I, R, Ih, Is, contactsPerDay);
		contactsPerDay = 0;




		// ###################   24 H LOOP 

		for (int hour = 0; hour < 24; hour++)
		{

			//printf("\nx = %d, y = %d \n", PPL[55].x, PPL[55].y);
			if (hour >= 0 && hour < 8)

			{
				for (int timestamp = 0; timestamp < 200; timestamp++)
				{
					for (int i = 0; i < N; i++)
					{
						if (PPL[i].homeless == 0)
						{
							PPL[i].actionGoHome();

							PPL[i].actionStayHome();

							for (int j = 0; j < N; j++)
							{
								if ((i != j) && (contact(PPL[i], PPL[j], T) == true)) //explain
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

				for (int timestamp = 0; timestamp < 200; timestamp++)
				{
					for (int i = 0; i < N; i++)
					{
						if (PPL[i].unemployed == 0)
						{
							PPL[i].actionGoWork();
							PPL[i].actionStayAtWork();
							for (int j = 0; j < N; j++)
							{
								if ((i != j) && (contact(PPL[i], PPL[j], T) == true)) //explain
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

				for (int timestamp = 0; timestamp < 200; timestamp++)
				{
					for (int i = 0; i < N; i++)
					{
						PPL[i].actionWalkFree();
						if (PPL[i].homeless == 0)

						{
							PPL[i].actionShopping();

						}
						for (int j = 0; j < N; j++)
						{
							if ((i != j) && (contact(PPL[i], PPL[j], T) == true)) //explain
							{
								contactsPerDay = contactsPerDay + 1;
							}

						}

					}

				}

			}

			if (hour > 14 && hour < 19)
			{

				for (int timestamp = 0; timestamp < 200; timestamp++)
				{
					for (int i = 0; i < N; i++)
					{
						if (PPL[i].unemployed == 0)
						{
							PPL[i].actionGoWork();
							PPL[i].actionStayAtWork();
							for (int j = 0; j < N; j++)
							{
								if ((i != j) && (contact(PPL[i], PPL[j], T) == true)) //explain
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

				for (int timestamp = 0; timestamp < 200; timestamp++)
				{
					for (int i = 0; i < N; i++)
					{
						PPL[i].actionWalkFree();

						if (PPL[i].homeless == 0)

						{
							PPL[i].actionShopping();

						}

						for (int j = 0; j < N; j++)
						{
							if ((i != j) && (contact(PPL[i], PPL[j], T) == true)) //explain
							{
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}


		}
		// ### END OF 24 H LOOP

		
		if (T == 30 || T == 60 || T == 90 || T == 120 || T == 150 || T == 180)

		{

			for (int j = 0; j < no_houses; j++)
			{
				for (int i = 0; i < N; i++)
				{

					if (PPL[i].x_home == HOU[j].x && PPL[i].y_home == HOU[j].y)
					{
						PPL[i].actionGetPaid();
						PPL[i].actionPayBills();
						//HOU[j].home_wealth = HOU[j].home_wealth + PPL[i].personal_wealth;
					}

				}

			}
		}

		//printf("\nContacts : %d\n", contactsPerDay);

		/*
		int S1 = S;
		int E1 = E;
		int I1 = I;
		int R1 = R;

		float Sdot = -b * contactsPerDay;
		float Edot = b * contactsPerDay - e * E;
		float Idot = e * E - g * I;
		float Rdot = g * I;

		S = S1 + Sdot;
		E = E1 + Edot;
		I = I1 + Idot;

		R = R1 + Rdot;

		*/

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

		T++;
		if (T == 1) {
			done = true;
		}

	}

	exce_file_results.open("SEIR_Results.csv");

	printf("\nPreparing results excel file...\n");
	for (int i = 0; i < T; i++)

	{
		exce_file_results << Sarray[i] << "," << Earray[i] << "," << Iarray[i] << "," << Rarray[i] << "," << timepassed[i] << std::endl;

	}

	exce_file_results.close();


	printf("\nResults excel file done!\n");

	printf("\nSimulation completed!\n");

	printf("\nPress a button to finish...\n");

	getchar();
};