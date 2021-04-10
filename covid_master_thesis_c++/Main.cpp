
#include <stdio.h>
#include<stdlib.h>
#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
//using namespace std::geometric_distribution;



float homeless_rate = 0.0005;
float initial_infected_rate = 0.01;
float initial_immune_rate = 0.01;
float unemployment_rate = 0.12;
float business_proportion = 0.01875;
float business_proportion_informal = 0.4;
int total_Wealth = 1000000000;
float public_Wealth_rate = 0.01;
float business_Wealth_rate = 0.05;
float personal_Wealth_rate = 0.04;
float public_Wealth = total_Wealth * public_Wealth_rate;
float business_Wealth = total_Wealth * business_Wealth_rate;
float personal_Wealth = total_Wealth * personal_Wealth_rate;
int public_Wealth_total = (int)(total_Wealth * public_Wealth_rate);
int business_Wealth_total = (int)(total_Wealth * business_Wealth_rate);
int personal_Wealth_total = (int)(total_Wealth * personal_Wealth_rate);
int min_income = 900;
int min_expense = 600;
int goverment_wealth_total = total_Wealth - (business_Wealth_total + personal_Wealth_total + public_Wealth_total);
int goverment_aid = 400;
int delta_business_wealth_total;
int delta_houses_wealth_total;
int delta_people_wealth_total;
float financial_reward_rate;
float reward_f;


int N = 400; // Population
int I_init = N * initial_infected_rate;  //Infected
int S_init = N - (N * initial_infected_rate);

int MostPoor_people = 0; //Most poor people
int Poor_people = 0; // Poor people
int WorkingClass_people = 0; // working class people
int Rich_people = 0; //Rich people
int MostRich_people = 0; // Most Rich people

int MostPoor_houses = 0; //Most poor house
int Poor_houses = 0; // Poor house
int WorkingClass_houses = 0; // working class house
int Rich_houses = 0; //Rich house
int MostRich_houses = 0; // Most Rich house

int MostPoor_workplaces = 0; //Most poor workplaces
int Poor_workplaces = 0; // Poor workplaces
int WorkingClass_workplaces = 0; // working class workplaces
int Rich_workplaces = 0; //Rich workplaces
int MostRich_workplaces = 0; // Most Rich workplaces

int goverment_money_Healthcare_fixed = (int)(goverment_wealth_total * 0.08); //from danish aep
int hospital_fee = 100;


int family_size_average = 3;
int no_houses = (int)(N / family_size_average);
int no_workplaces = (int)((N * business_proportion) + (N * business_proportion_informal));

//Environment

int Length = 300;  //each pixel corresponds to area 5x5 meters.
int Width = 300;
int SimulationTime = 92; // Simulation time in days.

double Qtable[30][7]; // first bracked must be the same as simulation time. 

//new 
int quarantine = 0; // Set this to 1 if people after 1 day of being "I" should do self quarantine at their house. Works for every policy.
int EcanSpread = 0;

//Pandemics Parameters

double contagion_distance = 1;
int incubation_time = 5;
int transmission_time = 8;
int recovering_time = 20;
int ICU_limit = N * 0.05;

float b = 0.001;//0.001;  // infectious rate, controls the rate of spread which represents the probability of transmitting disease between a susceptible and an infectious individual.
float q = 0.01;  // b / g // contact ratio
float g = 0.05;  // b / q // recovery rate
float e = 0.1;  // incubation rate is the rate of latent individuals becoming infectious(average duration of incubation is 1 / s)

// Policies varables

int deltaI = 0;
int deltaR = 0;
int lockdownDurationForPolicy4 = 0;
int lockdownForPolicy4 = 0;

//Constraints

int I_hosp_max = N * 0.02; // 8
int I_icu_max = N * 0.01; // 4

//Bills
int workplace_tax = 1000;
int house_tax = 500;

float reward = 0;

class House {
public:

	int dx, dy; //position move
	int x, y; //position
	int no_residents;
	int social_stratum;
	int home_wealth;
	int home_income;
	int home_expense;

	void actionPayBills() {
		home_wealth = home_wealth - house_tax;
		goverment_wealth_total = goverment_wealth_total + house_tax;

	}
};

class Workplace {
public:

	int dx, dy; //position move
	int x, y; //position
	int no_workers;
	int social_stratum;
	int workplace_wealth;
	int workplace_income;
	int workplace_expense;
	float profit;
	void actionPayBills() {
		workplace_wealth = workplace_wealth - workplace_tax;
		goverment_wealth_total = goverment_wealth_total + workplace_tax;
	}
};

class School {
public:
	int dx, dy = 5;
	int x = Length + 6;
	int y = Width + 6;
	int no_students = 0;

};

class Human {
public:
	int dx, dy; //position move
	int x, y; //position
	int group;
	int action;
	int age;
	int homeless;
	int unemployed;
	int x_home;
	int y_home;
	int x_work;
	int y_work;
	int student;
	int personal_income;
	int personal_expenses;
	int personal_wealth;
	int social_stratum;
	int essential_worker = 0;
	int shopped_today = 0;
	House house;
	Workplace work;
	int quarantined = 0;
	int inHospital = 0;

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
	void actionGoSchool(School& school)

	{
		if ((x - school.x > 200) || (x - school.x < -200) || (y - school.y > 200) || (y - school.y < -200))
		{
			x = school.x;
			y = school.y;
		}

		//IF student not in school, at this function call, move student by 1 px towards school
		if ((x < school.x - school.dx) || (x > school.x + school.dx) || (y < school.y - school.dy) || (y > school.y + school.dy))
		{
			if (x < school.x)
			{
				x = x + dx;
			}
			if (x > school.x)
			{
				x = x - dx;
			}
			if (y < school.y)
			{
				y = y + dy;
			}
			if (y > school.y)
			{
				y = y - dy;
			}

		}
	}
	void actionStayAtSchool(School& school)
	{
		//If student at school, stay at px or move with probability inside school
		if ((x >= school.x - school.dx) && (x <= school.x + school.dx) && (y >= school.y - school.dy) && (y <= school.y + school.dy))
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
			if (x > school.x + school.dx)
			{
				x = school.x + school.dx - 1;
			}

			if (x > school.x - school.dx) //check
			{
				x = school.x - school.dx + 1;
			}

			if (y > school.y + school.dy)
			{
				y = school.y + school.dy - 1;
			}

			if (y > school.y - school.dy)
			{
				y = school.y - school.dy + 1;
			}


		}

	}
	void actionPayHospitalFee()
	{
		if (group == 4 || group == 5)
		{
			personal_wealth = personal_wealth - hospital_fee;
		}
	}

};

class Hospital {
public:
	int x = -100;
	int y = -100;
	int infected_hospitalized = 0;
	int intected_severe = 0;

	void goToHospital(Human& person) {
		person.x = x;
		person.y = y;
		infected_hospitalized++;
	}
	void goToIC(Human& person) {
		person.x = x;
		person.y = y;
		intected_severe++;
	}
	void releaseFromHospital(Human& person) {
		if (person.group == 4) {
			infected_hospitalized--;
			person.x = person.x_home;
			person.y = person.y_home;
		}
		if (person.group == 5) {
			infected_hospitalized--;
			intected_severe--;
			person.x = person.x_home;
			person.y = person.y_home;
		}
	}

};

// Vectors

std::vector<Human> PPL;
std::vector<House> HOU;
std::vector<Workplace> WRP;
Hospital HOS;
School school;





//printf("\nschool x_pos = %d\n", school.x);
//printf("\nschool y_pos = %d\n", school.y);
//printf("\nschool dx_pos = %d\n", school.dx);
//printf("\nschool dy_pos = %d\n", school.dy);


void financialInterferenceHome(Human& person, House& house)
{
	//person.personal_wealth = person.personal_wealth + (person.personal_expenses / 30);
	house.home_wealth = (house.home_wealth - (person.personal_expenses / 30));


}
void actionWorkplacePays(Human& person, Workplace& workplace)
{
	workplace.workplace_wealth = workplace.workplace_wealth - person.personal_income;

}
void actionTransferSalaryToHome(Human& person, House& house)

{

	house.home_wealth = house.home_wealth + person.personal_income;

}
/*void actionShopping(Human& person, int total_works)
{
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution <int> distribution_empl(0, total_works);
	int m = distribution_empl(generator);
	// printf("\nm = %d\n", m);
	//printf("\nshop1\n");
	//printf("\npersonal money pro shopping = %d\n", person.personal_wealth);
	//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
	person.personal_wealth = person.personal_wealth - (person.personal_expenses / 60);
	WRP[m].workplace_wealth = WRP[m].workplace_wealth + (person.personal_expenses / 60);
	//printf("\nshop\n");
	//printf("\npersonal money after shopping = %d\n", person.personal_wealth);
	//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

}*/ //alternative shopping
bool actionShopping(Human& person, Workplace& workplace)
{
	if (person.x == workplace.x && person.y == workplace.y && person.x_work != workplace.x && person.y_work != workplace.y)
	{

		return true;
	}

	else
	{

		return false;
	}
}
void actionHouseSupplyBusiness(House& house, Workplace& workplace)

{

	house.home_wealth = house.home_wealth - 500;
	workplace.workplace_wealth = workplace.workplace_wealth + 500;
}
void GovermentFinanceHealthcare()

{
	goverment_wealth_total = goverment_wealth_total - goverment_money_Healthcare_fixed;

}
void GovermentAid(Human& person)

{

	goverment_wealth_total = goverment_wealth_total - goverment_aid;
	person.personal_wealth = person.personal_wealth + goverment_aid;

}
void BusinessProfit(Workplace& workplace)
{
	//workplace.workplace_wealth = (workplace.workplace_wealth + (workplace.profit * workplace.workplace_wealth));
	workplace.workplace_wealth = (workplace.workplace_wealth + ((workplace.profit * workplace.workplace_wealth)/30));
	//printf("\npercentage is equal to %f\n", (workplace.profit * workplace.workplace_wealth) / 30);
}


bool contact(Human& person1, Human& person2, int day, double contagion_probability1)
{

	if ((person1.x == person2.x) && (person1.y == person2.y))
	{
		if ((person1.group != 3 && person2.group != 3) && (((person1.group == 2 && person2.group == 0) || (person1.group == 0 && person2.group == 2)) || (EcanSpread == 1 && ((person1.group == 1 && person2.group == 0) || (person1.group == 0 && person2.group == 1)))))
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
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps <= contagion_probability1)
					{
						//printf("\nprob %f, eps %f\n", contagion_probability1, eps);
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

						if (EcanSpread == 1 && person1.group == 1)
						{
							person2.group = 1;
							person2.Eday = day;
						}

						if (EcanSpread == 1 && person2.group == 1)
						{
							person1.group = 1;
							person1.Eday = day;
						}

						return true;

					}
				}
			}


		}
		return false;
	}
	return false;
}

void EtoItransition(int N, int T) {
	for (int i = 0; i < N; i++) {
		if (PPL[i].group == 1 && T - PPL[i].Eday == 5) {
			PPL[i].group = 2;
			PPL[i].Iday = T;
			//printf("\nGroup: %d, Iday: %d", PPL[i].group, PPL[i].Iday);
			if (PPL[i].age >= 0 && PPL[i].age <= 9) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.001) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.05) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 10 && PPL[i].age <= 19) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.003) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.05) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 20 && PPL[i].age <= 29) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.012) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.05) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 30 && PPL[i].age <= 39) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.032) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.05) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 40 && PPL[i].age <= 49) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.049) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.063) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 50 && PPL[i].age <= 59) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.102) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.122) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 60 && PPL[i].age <= 69) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.166) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.274) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 70 && PPL[i].age <= 79) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.243) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.432) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
			if (PPL[i].age >= 80) {
				unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine generator(seed);
				std::uniform_real_distribution <float> distribution(0, 1);
				float eps = distribution(generator);  // uniform distribution
				if (eps < 0.273) {
					PPL[i].group = 4;
					HOS.goToHospital(PPL[i]);
					unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
					std::default_random_engine generator(seed);
					std::uniform_real_distribution <float> distribution(0, 1);
					float eps = distribution(generator);  // uniform distribution
					if (eps < 0.709) {
						PPL[i].group = 5;
						HOS.goToIC(PPL[i]);
					}
				}
			}
		}
		if (quarantine == 1) {
			if (PPL[i].group == 2 && T - PPL[i].Iday == 1 && PPL[i].inHospital == 0) {
				PPL[i].quarantined = 1;
			}
			if (PPL[i].group == 3 && PPL[i].quarantined == 1) {
				PPL[i].quarantined = 0;
			}
		}
		if ((PPL[i].group == 2 || PPL[i].group == 4 || PPL[i].group == 5) && T - PPL[i].Iday == 20) {
			HOS.releaseFromHospital(PPL[i]);
			PPL[i].group = 3;
		}
	}
}

float QReward(int delI, int hospI, int sevI) {
	float delIreward = 0.1 * delI; // should be probably bigger
	
	float economyReward = financial_reward_rate * (float)(delta_business_wealth_total / 100);
	float hospReward = -0.8 * hospI * hospI;
	float sevReward = -2.2 * sevI * sevI;


	float R = delIreward + economyReward + hospReward + sevReward;
	return R;
}

void QUpdate(int policy, int T, float R) {
	float newQ = 0;
	float alpha = 0.9;
	float gamma = 0.9;
	double maxQ = std::max(Qtable[T][0], Qtable[T][1]);
	maxQ = std::max(maxQ, Qtable[T][2]);
	maxQ = std::max(maxQ, Qtable[T][3]);
	maxQ = std::max(maxQ, Qtable[T][4]);
	maxQ = std::max(maxQ, Qtable[T][5]);
	maxQ = std::max(maxQ, Qtable[T][6]);
	newQ = (1 - alpha) * Qtable[T][policy] + alpha * (R + gamma * maxQ);
	Qtable[T][policy] = newQ;
}

int pickRandomPolicy() {
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution <int> distribution(0, 6);
	int eps = distribution(generator);  // uniform distribution
	return eps;
}

int policy0(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.9;
	int contactsPerDay = 0;
	if (hour >= 0 && hour < 8) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].homeless == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 7 && hour < 12) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].student == 1 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoSchool(school);
					PPL[i].actionStayAtSchool(school);
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 11 && hour < 13) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money pro profit = %d\n", WRP[m].workplace_wealth);
								//clients = clients + 1;
								//WRP[m].workplace_wealth = WRP[m].workplace_wealth + ((PPL[i].personal_expenses/60) * (WRP[m].profit));
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		for (int j = 0; j < N; j++) {
			PPL[j].shopped_today = 0;
		}
	}
	if (hour > 14 && hour < 19) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();

					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].student == 1 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 18 && hour < 24) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//WRP[m].workplace_wealth = WRP[m].workplace_wealth + ((PPL[i].personal_expenses / 60) * (WRP[m].profit));
								//clients = clients + 1;
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		/*for (int i = 0; i < no_workplaces; i++)
		{

			printf("\nworks's clients are %d\n", clients);
			printf("\namount are %f\n", (((PPL[i].personal_expenses)*clients)* WRP[i].profit));
			printf("\nwork money pro profit= %d\n", WRP[i].workplace_wealth);
			WRP[i].workplace_wealth = WRP[i].workplace_wealth + (((PPL[i].personal_expenses)*clients)* WRP[i].profit);
			printf("\nwork money after profit = %d\n", WRP[i].workplace_wealth);
			clients = 0;
		}*/

		for (int j = 0; j < N; j++) {
			PPL[j].shopped_today = 0;
		}
	}
	return contactsPerDay;
}
int policy1(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.8;
	int contactsPerDay = 0;
	if (hour >= 0 && hour < 8) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].homeless == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 7 && hour < 12) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].student == 1 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoSchool(school);
					PPL[i].actionStayAtSchool(school);
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 11 && hour < 13) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionWalkFree();

					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money pro profit = %d\n", WRP[m].workplace_wealth);
								//clients = clients + 1;
								//WRP[m].workplace_wealth = WRP[m].workplace_wealth + ((PPL[i].personal_expenses/60) * (WRP[m].profit));
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 14 && hour < 19) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].student == 1 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 18 && hour < 24) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money pro profit = %d\n", WRP[m].workplace_wealth);
								//clients = clients + 1;
								//WRP[m].workplace_wealth = WRP[m].workplace_wealth + ((PPL[i].personal_expenses/60) * (WRP[m].profit));
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if (PPL[i].quarantined == 1 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		for (int j = 0; j < N; j++) {
			PPL[j].shopped_today = 0;
		}
	}
	return contactsPerDay;
}
int policy2(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.9;
	int contactsPerDay = 0;

	if (hour >= 0 && hour < 8) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].homeless == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 7 && hour < 12) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 11 && hour < 13) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 14 && hour < 19) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 18 && hour < 24) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		for (int j = 0; j < N; j++) {
			PPL[j].shopped_today = 0;
		}
	}
	return contactsPerDay;
}
int policy3(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.3; // FACEMASK 0.3 is the value used in paper
	int contactsPerDay = 0;

	if (hour >= 0 && hour < 8) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].homeless == 0 && PPL[i].inHospital == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 7 && hour < 12) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 11 && hour < 13) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 14 && hour < 19) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].unemployed == 0 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
	}
	if (hour > 18 && hour < 24) {
		for (int timestamp = 0; timestamp < 200; timestamp++) {
			for (int i = 0; i < N; i++) {
				if (PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && (PPL[i].age < 60 && PPL[i].age > 16)) {
					PPL[i].actionWalkFree();
					for (int m = 0; m < no_workplaces; m++)
					{
						if (PPL[i].homeless == 0 && PPL[i].shopped_today == 0)
						{
							if (actionShopping(PPL[i], WRP[m]) == true) // works ok
							{
								//printf("\nshop1\n");
								//printf("\npersonal money pro shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money pro shopping = %d\n", WRP[m].workplace_wealth);
								PPL[i].shopped_today = 1;
								PPL[i].personal_wealth = PPL[i].personal_wealth - (PPL[i].personal_expenses / 60);
								WRP[m].workplace_wealth = WRP[m].workplace_wealth + (PPL[i].personal_expenses / 60);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);
								for (int z = 0; z < no_houses; z++)
								{
									if (HOU[z].x == PPL[i].x_home && HOU[z].y == PPL[i].y_home)
									{
										//printf("\nhome money pro shopping = %d\n", HOU[z].home_wealth);
										HOU[z].home_wealth = HOU[z].home_wealth - (PPL[i].personal_expenses / 60);
										//printf("\nhome money after shopping = %d\n", HOU[z].home_wealth);
									}
								}
								//printf("\nshop\n");
								//printf("\npersonal money after shopping = %d\n", PPL[i].personal_wealth);
								//printf("\nwork money after shopping = %d\n", WRP[m].workplace_wealth);

							}

						}

					}
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				if ((PPL[i].quarantined == 1 && PPL[i].inHospital == 0) || (PPL[i].age >= 60 || PPL[i].age <= 16)) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		for (int j = 0; j < N; j++) {
			PPL[j].shopped_today = 0;
		}
	}
	return contactsPerDay;
}
int policy4(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.2; // FACEMASK
	int contactsPerDay = 0;

	// MAKE CONDITION FOR CONDITIONAL LOCKDOWN

	for (int i = 0; i < N; i++) {
		// IF .. CONDITIONAL LOCKDOWN ELSE POLICY 3
		// lockdownForPolicy4 is set in main 24h loop.

		if (lockdownForPolicy4 == 1) {
			if (hour >= 0 && hour < 8) {
				for (int timestamp = 0; timestamp < 200; timestamp++) {
					if (PPL[i].homeless == 0) {
						PPL[i].actionGoHome();
						PPL[i].actionStayHome();
						for (int j = 0; j < N; j++) {
							if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
								if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
									contactsPerDay = contactsPerDay + 1;
								}
							}
						}
					}
				}
			}
			if (hour > 7 && hour < 17) {
				for (int timestamp = 0; timestamp < 200; timestamp++) {
					if (PPL[i].unemployed == 0 && PPL[i].essential_worker == 1 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && PPL[i].age < 60 && PPL[i].age > 16) {
						PPL[i].actionGoWork();
						PPL[i].actionStayAtWork();
						for (int j = 0; j < N; j++) {
							if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
								if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
									contactsPerDay = contactsPerDay + 1;
								}
							}
						}
					}
					else {
						PPL[i].actionGoHome();
						PPL[i].actionStayHome();
						for (int j = 0; j < N; j++) {
							if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
								if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
									contactsPerDay = contactsPerDay + 1;
								}
							}
						}
					}
				}
			}
			if (hour > 16 && hour < 24) {
				for (int timestamp = 0; timestamp < 200; timestamp++) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		else {
			return policy3(hour, T);
		}
	}

	return contactsPerDay;
}
int policy5(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.2; // FACEMASK
	int contactsPerDay = 0;
	for (int i = 0; i < N; i++) {
		if (hour >= 0 && hour < 8) {
			for (int timestamp = 0; timestamp < 200; timestamp++) {
				if (PPL[i].homeless == 0) {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		if (hour > 7 && hour < 17) {
			for (int timestamp = 0; timestamp < 200; timestamp++) {
				if (PPL[i].unemployed == 0 && PPL[i].essential_worker == 1 && PPL[i].quarantined == 0 && PPL[i].inHospital == 0 && PPL[i].age < 60 && PPL[i].age > 16) {
					PPL[i].actionGoWork();
					PPL[i].actionStayAtWork();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
				else {
					PPL[i].actionGoHome();
					PPL[i].actionStayHome();
					for (int j = 0; j < N; j++) {
						if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
							if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
								contactsPerDay = contactsPerDay + 1;
							}
						}
					}
				}
			}
		}
		if (hour > 16 && hour < 24) {
			for (int timestamp = 0; timestamp < 200; timestamp++) {
				PPL[i].actionGoHome();
				PPL[i].actionStayHome();
				for (int j = 0; j < N; j++) {
					if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
						if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
							contactsPerDay = contactsPerDay + 1;
						}
					}
				}
			}
		}
	}
	return contactsPerDay;
}
int policy6(int hour, int T) {
	contagion_distance = 1;
	double contagion_probability = 0.2; // FACEMASK
	int contactsPerDay = 0;
	for (int timestamp = 0; timestamp < 200; timestamp++) {
		for (int i = 0; i < N; i++) {
			PPL[i].actionGoHome();
			PPL[i].actionStayHome();
			for (int j = 0; j < N; j++) {
				if ((i != j) && PPL[i].x == PPL[j].x && PPL[i].y == PPL[j].y) {
					if ((contact(PPL[i], PPL[j], T, contagion_probability) == true)) {
						contactsPerDay = contactsPerDay + 1;
					}
				}
			}
		}
	}
	return contactsPerDay;
}


int main()
{

	std::ofstream excel_file_initialization; // excel file for exporting initial distribution of houses, workplaces etc.
	std::ofstream excel_file_SEIR_results; // excel file for exporting final results of SEIR model
	std::ofstream excel_file_Financial_results; // excel file for exporting final results of Economical conditions
	std::ofstream excel_file_Financial_Reward; // excel file for exporting financial reward
	std::ofstream excel_file_HOUSES; // excel file for exporting HOUSES Objects
	std::ofstream excel_file_HUMANS; // excel file for exporting HUMANS Objects
	std::ofstream excel_file_WORKPLACES; // excel file for exporting WORKPLACES Objects
	std::ofstream excel_file_SCHOOL; // excel file for exporting SCHOOL
	std::ofstream excel_file_HOSPITAL; // excel file for exporting HOSPITAL
	std::ofstream excel_qtab;

	//Random generators

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution <int> distribution_x(0, Length + 1);
	std::uniform_int_distribution <int> distribution_y(0, Width + 1);
	std::uniform_int_distribution <int> distribution_dx_house(1, 3);
	std::uniform_int_distribution <int> distribution_dy_house(1, 3);
	std::uniform_int_distribution <int> distribution_dx_school(1, 5);
	std::uniform_int_distribution <int> distribution_dy_school(1, 5);
	std::uniform_int_distribution <int> distribution_dx_workplace(1, 7);
	std::uniform_int_distribution <int> distribution_dy_workplace(1, 7);
	std::uniform_int_distribution <int> distribution_family(2, 4);
	std::uniform_int_distribution <int> distribution_employees(20, 60);
	std::uniform_real_distribution <float> distribution_homeless(0, 1);
	std::uniform_real_distribution <float> distribution_employeed(0, 1);
	std::_Beta_distribution <float> distribution_age(2, 5);
	std::uniform_int_distribution <int> distribution_social_stratum(1, 5);
	std::uniform_int_distribution <int> distribution_unemployeed_social_stratum(1, 2);
	std::uniform_real_distribution <float> distribution_immune(0, 1);
	std::uniform_real_distribution <float> essential_job(0, 1);


	char press_button_data;
	printf("\nDo you want to generate new initiallization data and objects or use the saved ones? Press n if you want new or s if you want saved...\n");
	std::cin >> press_button_data;
	if (press_button_data == 's')
	{
		std::ifstream read_excel_HUMANS("HUMAN_OBJECTS.csv");
		std::ifstream read_excel_HOUSES("HOUSES_OBJECTS.csv");
		std::ifstream read_excel_WORKPLACES("WORKPLACES_OBJECTS.csv");
		std::ifstream read_excel_SCHOOL("SCHOOL_OBJECT.csv");
		std::ifstream read_excel_HOSPITAL("HOSPITAL_OBJECT.csv");


		//Spawn houses
		for (int i = 0; i < no_houses; i++) {
			std::string x;
			std::string y;
			std::string dx;
			std::string dy;
			std::string ss;
			std::string nr;
			std::string hw;
			House house;

			std::getline(read_excel_HOUSES, x, ',');
			house.x = std::stoi(x);
			std::getline(read_excel_HOUSES, y, ',');
			house.y = std::stoi(y);
			std::getline(read_excel_HOUSES, dx, ',');
			house.dx = std::stoi(dx);
			std::getline(read_excel_HOUSES, dy, ',');
			house.dy = std::stoi(dy);
			std::getline(read_excel_HOUSES, nr, ',');
			house.no_residents = std::stoi(nr);
			std::getline(read_excel_HOUSES, ss, ',');
			house.social_stratum = std::stoi(ss);
			std::getline(read_excel_HOUSES, hw);
			house.home_wealth = std::stoi(hw);
			HOU.push_back(house);
			//printf("\nx = %d, y = %d, dx = %d, dy = %d, nr = %d, ss = %d, hw = %d\n", HOU[i].x, HOU[i].y, HOU[i].dx, HOU[i].dy, HOU[i].social_stratum, HOU[i].no_residents, HOU[i].home_wealth);
		}
		//Spawn workplaces
		for (int i = 0; i < no_workplaces; i++) {
			std::string x;
			std::string y;
			std::string dx;
			std::string dy;
			std::string ss;
			std::string profit;
			std::string nw;
			std::string ww;
			Workplace workplace;


			std::getline(read_excel_WORKPLACES, x, ',');
			workplace.x = std::stoi(x);
			std::getline(read_excel_WORKPLACES, y, ',');
			workplace.y = std::stoi(y);
			std::getline(read_excel_WORKPLACES, dx, ',');
			workplace.dx = std::stoi(dx);
			std::getline(read_excel_WORKPLACES, dy, ',');
			workplace.dy = std::stoi(dy);
			std::getline(read_excel_WORKPLACES, nw, ',');
			workplace.no_workers = std::stoi(nw);
			std::getline(read_excel_WORKPLACES, ss, ',');
			workplace.social_stratum = std::stoi(ss);
			std::getline(read_excel_WORKPLACES, profit, ',');
			workplace.profit = std::stof(profit);
			std::getline(read_excel_WORKPLACES, ww);
			workplace.workplace_wealth = std::stoi(ww);
			WRP.push_back(workplace);
				//printf("\nx = %d, y = %d, dx = %d, dy = %d, nw = %d, ss = %d,profit = %f, ww = %d\n", WRP[i].x, WRP[i].y, WRP[i].dx, WRP[i].dy, WRP[i].no_workers, WRP[i].social_stratum, WRP[i].profit, WRP[i].workplace_wealth);

		}
		//spawn people
		for (int i = 0; i < N; i++) {
			std::string x;
			std::string y;
			std::string dx;
			std::string dy;
			std::string group;
			std::string age;
			std::string homeless;
			std::string unemployed;
			std::string x_home;
			std::string y_home;
			std::string x_work;
			std::string y_work;
			std::string student;
			std::string pi;
			std::string pe;
			std::string pw;
			std::string ss;
			std::string ee;

			Human person;

			std::getline(read_excel_HUMANS, x, ',');
			person.x = std::stoi(x);
			std::getline(read_excel_HUMANS, y, ',');
			person.y = std::stoi(y);
			std::getline(read_excel_HUMANS, dx, ',');
			person.dx = std::stoi(dx);
			std::getline(read_excel_HUMANS, dy, ',');
			person.dy = std::stoi(dy);
			std::getline(read_excel_HUMANS, group, ',');
			person.group = std::stoi(group);
			std::getline(read_excel_HUMANS, age, ',');
			person.age = std::stoi(age);
			std::getline(read_excel_HUMANS, homeless, ',');
			person.homeless = std::stoi(homeless);
			std::getline(read_excel_HUMANS, unemployed, ',');
			person.unemployed = std::stoi(unemployed);
			std::getline(read_excel_HUMANS, x_home, ',');
			person.x_home = std::stoi(x_home);
			std::getline(read_excel_HUMANS, y_home, ',');
			person.y_home = std::stoi(y_home);
			std::getline(read_excel_HUMANS, x_work, ',');
			person.x_work = std::stoi(x_work);
			std::getline(read_excel_HUMANS, y_work, ',');
			person.y_work = std::stoi(y_work);
			std::getline(read_excel_HUMANS, student, ',');
			person.student = std::stoi(student);
			std::getline(read_excel_HUMANS, ss, ',');
			person.social_stratum = std::stoi(ss);
			std::getline(read_excel_HUMANS, pi, ',');
			person.personal_income = std::stoi(pi);
			std::getline(read_excel_HUMANS, pe, ',');
			person.personal_expenses = std::stoi(pe);
			std::getline(read_excel_HUMANS, pw, ',');
			person.personal_wealth = std::stoi(pw);
			std::getline(read_excel_HUMANS, ee);
			person.essential_worker = std::stoi(ee);

			PPL.push_back(person);
			//printf("\nx = %d, y = %d, dx = %d, dy = %d, group = %d, age = %d, hl = %d, ue = %d, xh = %d, yh = %d, xw = %d, yw = %d, stu = %d, ss = %d, pi = %d, pe = %d,  pw = %d, ee = %d\n", PPL[i].x, PPL[i].y, PPL[i].dx, PPL[i].dy, PPL[i].group, PPL[i].age, PPL[i].homeless, PPL[i].unemployed, PPL[i].x_home, PPL[i].y_home, PPL[i].x_work, PPL[i].y_work, PPL[i].student, PPL[i].social_stratum, PPL[i].personal_income, PPL[i].personal_expenses, PPL[i].personal_wealth, PPL[i].essential_worker);
		}

		//SCHOOL

		for (int i = 0; i < 1; i++) {
			std::string x;
			std::string y;
			std::string dx;
			std::string dy;
			std::string ns;

			std::getline(read_excel_SCHOOL, x, ',');
			school.x = std::stoi(x);
			std::getline(read_excel_SCHOOL, y, ',');
			school.y = std::stoi(y);
			std::getline(read_excel_SCHOOL, dx, ',');
			school.dx = std::stoi(dx);
			std::getline(read_excel_SCHOOL, dy, ',');
			school.dy = std::stoi(dy);
			std::getline(read_excel_SCHOOL, ns);
			school.no_students = std::stoi(ns);


			//printf("\nx = %d, y = %d, dx = %d, dy = %d, ns = %d\n", school.x, school.y, school.dx, school.dy, school.no_students);
		}


		//Hospital
		for (int i = 0; i < 1; i++) {
			std::string x;
			std::string y;
			std::string ih;
			std::string is;

			std::getline(read_excel_HOSPITAL, x, ',');
			HOS.x = std::stoi(x);
			std::getline(read_excel_HOSPITAL, y, ',');
			HOS.y = std::stoi(y);
			std::getline(read_excel_HOSPITAL, ih, ',');
			HOS.infected_hospitalized = std::stoi(ih);
			std::getline(read_excel_HOSPITAL, is);
			HOS.intected_severe = std::stoi(is);

			//printf("\nx = %d, y = %d, ih = %d, is = %d\n", HOS.x, HOS.y, HOS.infected_hospitalized, HOS.intected_severe);
		}

		// assign people to houses

		for (int i = 0; i < no_houses; i++) {
			for (int j = 0; j < N; j++) {

				if (PPL[j].x_home == HOU[i].x && PPL[j].y_home == HOU[i].y) {
					PPL[j].house = HOU[i];

				}
			}
		}

		// Check if everone has a home

		for (int i = 0; i < N; i++) {
			if (PPL[i].homeless == 0 && PPL[i].x_home == -1 && PPL[i].y_home == -1) {
				printf("\nError to houses people assignment\n");
				PPL[i].homeless = 1;
				PPL[i].unemployed = 1;
				PPL[i].social_stratum = 1;
			}
		}

		// assign people to workplaces

		for (int i = 0; i < no_workplaces; i++) {

			for (int j = 0; j < N; j++) {


				if (PPL[j].x_work == WRP[i].x && PPL[j].y_work == WRP[i].y) {
					PPL[j].work = WRP[i];
				}
			}
		}

	}
	else if (press_button_data == 'n')

	{

		//Spawn houses
		for (int i = 0; i < no_houses; i++) {
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
		for (int i = 0; i < no_workplaces; i++) {
			Workplace workplace;
			workplace.x = distribution_x(generator);
			workplace.y = distribution_y(generator);
			workplace.dx = distribution_dx_workplace(generator);
			workplace.dy = distribution_dy_workplace(generator);
			workplace.social_stratum = distribution_social_stratum(generator);
			if (workplace.social_stratum == 1 || workplace.social_stratum == 2)
			{
				workplace.profit = 0.001;

			}
			else if (workplace.social_stratum == 3)
			{

				workplace.profit = 0.005;
			}

			else if (workplace.social_stratum == 4 || workplace.social_stratum == 5)

			{
				workplace.profit = 0.01;

			}
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
		for (int i = 0; i < 1; i++){
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

		// S GROUP

		for (int i = 0; i < N - I_init; i++) {
			Human person;

			person.x = 1;
			person.y = 1;
			person.dx = 1;
			person.dy = 1;
			float l = distribution_immune(generator);
			if (l <= (1 - initial_immune_rate)) // initial immunity
			{
				person.group = 0;
			}
			else
			{
				person.group = 3;

			}

			//person.action = 0;
			person.age = (int)(distribution_age(generator) * 100);
			float w = distribution_homeless(generator);
			if (w <= (1 - homeless_rate)) {
				person.homeless = 0;
			}
			else {
				person.homeless = 1;
				person.unemployed = 1;
			}
			if (person.age < 17 || person.age > 66) {
				person.unemployed = 1;
			}
			else {
				float z = distribution_employeed(generator);
				if (z <= (1 - unemployment_rate)) {
					person.unemployed = 0;
					float z1 = essential_job(generator);
					if (z1 < 0.2) {
						person.essential_worker = 1;
					}
				}

				else {
					person.unemployed = 1;
				}
			}
			if (person.age < 17) {
				person.student = 1;
				school.no_students = school.no_students + 1;
			}
			if (person.age > 16) {
				person.student = 0;
			}

			person.x_home = -1;
			person.y_home = -1;
			person.x_work = -1;
			person.y_work = -1;
			person.house = emptyHouse;
			person.work = emptyWork;
			PPL.push_back(person);
		}

		// I GROUP

		for (int i = 0; i < I_init; i++) {
			Human person;

			person.x = 1;
			person.y = 1;
			person.dx = 1;
			person.dy = 1;
			person.group = 2;
			//person.action = 0;
			person.age = (int)(distribution_age(generator) * 100);
			person.homeless = 0;

			if (person.age < 17 || person.age > 66) {
				person.unemployed = 1;
			}
			else {
				float z = distribution_employeed(generator);
				if (z <= (1 - unemployment_rate)) {
					person.unemployed = 0;
					float z1 = essential_job(generator);
					if (z1 < 0.2) {
						person.essential_worker = 1;
					}
				}

				else {
					person.unemployed = 1;
				}
			}
			if (person.age < 17) {
				person.student = 1;
				school.no_students = school.no_students + 1;
			}
			if (person.age > 16) {
				person.student = 0;
			}

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

		// assign people to houses

		for (int i = 0; i < no_houses; i++) {
			int ff = distribution_family(generator);
			int temp = 0;
			for (int j = 0; j < N; j++) {
				if (PPL[j].homeless == 0) {
					if (PPL[j].x_home == -1) {
						PPL[j].house = HOU[i];
						PPL[j].x_home = HOU[i].x;
						PPL[j].y_home = HOU[i].y;
						PPL[j].x = HOU[i].x;
						PPL[j].y = HOU[i].y;
						HOU[i].no_residents = HOU[i].no_residents + 1;
						PPL[j].social_stratum = HOU[i].social_stratum;
						temp = temp + 1;
						if (temp == ff) {
							break;
						}
					}
				}
				if (PPL[j].homeless == 1) {
					PPL[j].social_stratum = 1;
				}
			}
		}

		// Check if everone has a home

		for (int i = 0; i < N; i++) {
			if (PPL[i].homeless == 0 && PPL[i].x_home == -1 && PPL[i].y_home == -1) {
				//printf("\nError to houses people assignment\n");
				PPL[i].homeless = 1;
				PPL[i].unemployed = 1;
				PPL[i].social_stratum = 1;
			}
		}

		// assign people to workplaces

		for (int i = 0; i < no_workplaces; i++) {
			int ff = distribution_employees(generator);
			int temp = 0;
			for (int j = 0; j < N; j++) {
				if (PPL[j].unemployed == 0) {
					if (PPL[j].social_stratum == WRP[i].social_stratum) {
						if (PPL[j].x_work == -1) {
							PPL[j].work = WRP[i];
							PPL[j].x_work = WRP[i].x;
							PPL[j].y_work = WRP[i].y;
							WRP[i].no_workers = WRP[i].no_workers + 1;
							temp = temp + 1;
							if (temp == ff) {
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

		// How many ppl are in each group

		for (int j = 0; j < N; j++) {

			if (PPL[j].social_stratum == 1)
			{
				MostPoor_people = MostPoor_people + 1;
			}

			else if (PPL[j].social_stratum == 2)
			{
				Poor_people = Poor_people + 1;
			}

			else if (PPL[j].social_stratum == 3)
			{
				WorkingClass_people = WorkingClass_people + 1;
			}

			else if (PPL[j].social_stratum == 4)
			{
				Rich_people = Rich_people + 1;
			}
			else if (PPL[j].social_stratum == 5)
			{
				MostRich_people = MostRich_people + 1;
			}
			else
			{

				printf("\nERROR!! Social Stratum dont assigned for a person\n");
				//printf("\nhas this guy home : %d\n", PPL[j].x_home);
				//printf("\nhas this guy work : %d\n", PPL[j].x_work);
			}
		}

		//printf("\nMP = %d, P = %d, wc = %d, r = %d, mr = %d \n", MostPoor_people, Poor_people, WorkingClass_people, Rich_people, MostRich_people);

		// Share money, set income, expenses

		for (int i = 0; i < N; i++)

		{
			if (PPL[i].unemployed == 1)
			{

				PPL[i].personal_income = 0;

				if (PPL[i].social_stratum == 1)
				{


					PPL[i].personal_expenses = 600;
					PPL[i].personal_wealth = (int)((personal_Wealth_total * 0.0362) / MostPoor_people);
					//printf("\nPersonal wealth of very poor person is : %d\n", PPL[i].personal_wealth);
				}

				else if (PPL[i].social_stratum == 2)
				{


					PPL[i].personal_expenses = 650;
					PPL[i].personal_wealth = (int)((personal_Wealth_total * 0.0788) / Poor_people);
					//printf("\nPersonal wealth of poor person is : %d\n", PPL[i].personal_wealth);
				}

				else if (PPL[i].social_stratum == 3)
				{


					PPL[i].personal_expenses = 900;

					PPL[i].personal_wealth = (int)((personal_Wealth_total * 0.1262) / WorkingClass_people);
					//printf("\nPersonal wealth of working class person is : %d\n", PPL[i].personal_wealth);
				}

				else if (PPL[i].social_stratum == 4)
				{


					PPL[i].personal_expenses = 1200;

					PPL[i].personal_wealth = (int)((personal_Wealth_total * 0.4388) / Rich_people);
					//printf("\nPersonal wealth of rich person is : %d\n", PPL[i].personal_wealth);
				}
				else if (PPL[i].social_stratum == 5)
				{


					PPL[i].personal_expenses = 1700;
					PPL[i].personal_wealth = (int)((personal_Wealth_total * 0.5612) / MostRich_people);
					//printf("\nPersonal wealth of very rich person is : %d\n", PPL[i].personal_wealth);
				}

			}

			else if (PPL[i].unemployed == 0)

			{

				if (PPL[i].social_stratum == 1)
				{

					PPL[i].personal_income = 900;
					PPL[i].personal_expenses = 600;
					PPL[i].personal_wealth = ((personal_Wealth_total * 0.0362) / MostPoor_people);
					//printf("\nPersonal wealth of very poor person is : %d\n", PPL[i].personal_wealth);
				}

				else if (PPL[i].social_stratum == 2)
				{

					PPL[i].personal_income = 950;
					PPL[i].personal_expenses = 650;
					PPL[i].personal_wealth = ((personal_Wealth_total * 0.0788) / Poor_people);
					//printf("\nPersonal wealth of poor person is : %d\n", PPL[i].personal_wealth);
				}

				else if (PPL[i].social_stratum == 3)
				{

					PPL[i].personal_income = 1200;
					PPL[i].personal_expenses = 900;

					PPL[i].personal_wealth = ((personal_Wealth_total * 0.1262) / WorkingClass_people);
					//printf("\nPersonal wealth of working class person is : %d\n", PPL[i].personal_wealth);
				}

				else if (PPL[i].social_stratum == 4)
				{

					PPL[i].personal_income = 1500;
					PPL[i].personal_expenses = 1200;

					PPL[i].personal_wealth = ((personal_Wealth_total * 0.4388) / Rich_people);
					//printf("\nPersonal wealth of rich person is : %d\n", PPL[i].personal_wealth);
				}
				else if (PPL[i].social_stratum == 5)
				{

					PPL[i].personal_income = 2000;
					PPL[i].personal_expenses = 1700;
					PPL[i].personal_wealth = ((personal_Wealth_total * 0.5612) / MostRich_people);
					//printf("\nPersonal wealth of very rich person is : %d\n", PPL[i].personal_wealth);
				}
			}

		}

		//wealth sharing workplaces

		for (int j = 0; j < no_workplaces; j++)

		{

			if (WRP[j].social_stratum == 1)
			{
				MostPoor_workplaces = MostPoor_workplaces + 1;
			}

			else if (WRP[j].social_stratum == 2)
			{
				Poor_workplaces = Poor_workplaces + 1;
			}

			else if (WRP[j].social_stratum == 3)
			{
				WorkingClass_workplaces = WorkingClass_workplaces + 1;
			}

			else if (WRP[j].social_stratum == 4)
			{
				Rich_workplaces = Rich_workplaces + 1;
			}
			else if (WRP[j].social_stratum == 5)
			{
				MostRich_workplaces = MostRich_workplaces + 1;
			}
		}
		//printf("\nMP = %d, P = %d, wc = %d, r = %d, mr = %d \n", MostPoor_workplaces, Poor_workplaces, WorkingClass_workplaces, Rich_workplaces, MostRich_workplaces);

		for (int i = 0; i < no_workplaces; i++)

		{

			if (WRP[i].social_stratum == 1)
			{


				WRP[i].workplace_wealth = ((business_Wealth_total * 0.0362) / MostPoor_workplaces);
				//printf("\nWealth of very poor workplace is : %d\n", WRP[i].workplace_wealth);
			}

			else if (WRP[i].social_stratum == 2)
			{


				WRP[i].workplace_wealth = ((business_Wealth_total * 0.0788) / Poor_workplaces);
				//printf("\nWealth of poor workplace is : %d\n", WRP[i].workplace_wealth);
			}

			else if (WRP[i].social_stratum == 3)
			{

				WRP[i].workplace_wealth = ((business_Wealth_total * 0.1262) / WorkingClass_workplaces);
				//printf("\nWealth of working class workplace is : %d\n", WRP[i].workplace_wealth);
			}

			else if (WRP[i].social_stratum == 4)
			{

				WRP[i].workplace_wealth = ((business_Wealth_total * 0.4388) / Rich_workplaces);
				//printf("\nWealth of rich workplace is : %d\n", WRP[i].workplace_wealth);
			}
			else if (WRP[i].social_stratum == 5)
			{
				WRP[i].workplace_wealth = ((business_Wealth_total * 0.5612) / MostRich_workplaces);
				//printf("\nWealth of very rich workplace is : %d\n", WRP[i].workplace_wealth);
			}
		}

		//wealth sharing houses

		for (int j = 0; j < no_houses; j++)

		{

			if (HOU[j].social_stratum == 1)
			{
				MostPoor_houses = MostPoor_houses + 1;
			}

			else if (HOU[j].social_stratum == 2)
			{
				Poor_houses = Poor_houses + 1;
			}

			else if (HOU[j].social_stratum == 3)
			{
				WorkingClass_houses = WorkingClass_houses + 1;
			}

			else if (HOU[j].social_stratum == 4)
			{
				Rich_houses = Rich_houses + 1;
			}
			else if (HOU[j].social_stratum == 5)
			{
				MostRich_houses = MostRich_houses + 1;
			}
		}
		//printf("\nMP = %d, P = %d, wc = %d, r = %d, mr = %d \n", MostPoor_houses, Poor_houses, WorkingClass_houses, Rich_houses, MostRich_houses);

		for (int i = 0; i < no_houses; i++)

		{

			if (HOU[i].social_stratum == 1)
			{


				HOU[i].home_wealth = ((public_Wealth_total * 0.0362) / MostPoor_houses);
				//printf("\nWealth of very poor home is : %d\n", HOU[i].home_wealth);
			}

			else if (HOU[i].social_stratum == 2)
			{


				HOU[i].home_wealth = ((public_Wealth_total * 0.0788) / Poor_houses);
				//printf("\nWealth of poor home is : %d\n", HOU[i].home_wealth);
			}

			else if (HOU[i].social_stratum == 3)
			{

				HOU[i].home_wealth = ((public_Wealth_total * 0.1262) / WorkingClass_houses);
				//printf("\nWealth of working class home is : %d\n", HOU[i].home_wealth);
			}

			else if (HOU[i].social_stratum == 4)
			{

				HOU[i].home_wealth = ((public_Wealth_total * 0.4388) / Rich_houses);
				//printf("\nWealth of rich home is : %d\n", HOU[i].home_wealth);
			}
			else if (HOU[i].social_stratum == 5)
			{
				HOU[i].home_wealth = ((public_Wealth_total * 0.5612) / MostRich_houses);
				//printf("\nWealth of very rich home is : %d\n", HOU[i].home_wealth);
			}

			//printf("\nhome wealth = %d\n", HOU[i].home_wealth);
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
		char press_button_save;
		printf("\n Do you want the initiallisation data to be saved? Press y if yes or any other button if not...\n");
		std::cin >> press_button_save;
		if (press_button_save == 'y')
		{
			excel_file_HOUSES.open("HOUSES_OBJECTS.csv");
			excel_file_HUMANS.open("HUMAN_OBJECTS.csv");
			excel_file_WORKPLACES.open("WORKPLACES_OBJECTS.csv");
			excel_file_SCHOOL.open("SCHOOL_OBJECT.csv");
			excel_file_HOSPITAL.open("HOSPITAL_OBJECT.csv");

			// DATA SAVE

			for (int i = 0; i < N; i++)

			{
				excel_file_HUMANS << PPL[i].x << "," << PPL[i].y << "," << PPL[i].dx << "," << PPL[i].dy << "," << PPL[i].group << "," << PPL[i].age << "," << PPL[i].homeless << "," << PPL[i].unemployed << "," << PPL[i].x_home << "," << PPL[i].y_home << "," << PPL[i].x_work << "," << PPL[i].y_work << "," << PPL[i].student << "," << PPL[i].social_stratum << "," << PPL[i].personal_income << "," << PPL[i].personal_expenses << "," << PPL[i].personal_wealth << "," << PPL[i].essential_worker << std::endl;

			}

			for (int i = 0; i < no_houses; i++)

			{
				excel_file_HOUSES << HOU[i].x << "," << HOU[i].y << "," << HOU[i].dx << "," << HOU[i].dy << "," << HOU[i].no_residents << "," << HOU[i].social_stratum << "," << HOU[i].home_wealth << std::endl;

			}

			for (int i = 0; i < no_workplaces; i++)

			{
				excel_file_WORKPLACES << WRP[i].x << "," << WRP[i].y << "," << WRP[i].dx << "," << WRP[i].dy << "," << WRP[i].no_workers << "," << WRP[i].social_stratum << "," << WRP[i].profit << "," << WRP[i].workplace_wealth << std::endl;

			}

			for (int i = 0; i < 1; i++)

			{
				excel_file_SCHOOL << school.x << "," << school.y << "," << school.dx << "," << school.dy << "," << school.no_students << std::endl;

			}

			for (int i = 0; i < 1; i++)

			{
				excel_file_HOSPITAL << HOS.x << "," << HOS.y << "," << HOS.infected_hospitalized << "," << HOS.intected_severe << std::endl;

			}

			excel_file_HOUSES.close();
			excel_file_HUMANS.close();
			excel_file_WORKPLACES.close();
			excel_file_SCHOOL.close();
			excel_file_HOSPITAL.close();

		}


	}

	/*else
	{
		printf("\nYou pressed wrong button..The program will exit..Please try again\n");
		getchar();
		return 0;

	}*/
	//printf("\ntotal houses are = %d\n", no_houses);
	//printf("\ntottal workplaces are = %d\n", no_workplaces);


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

		//printf("\nxhome = %d, yhome = %d \n", PPL[55].x_home, PPL[55].y_home);
		//printf("\nxwork = %d, ywork = %d \n", PPL[55].x_work, PPL[55].y_work);



	std::vector<float> Sarray;
	std::vector<float> Earray;
	std::vector<float> Iarray;
	std::vector<float> Rarray;
	std::vector<float> Iharray;
	std::vector<float> Isarray;
	std::vector<int> timepassed;
	std::vector<int> Personal_Wealth_Array;
	std::vector<int> House_Wealth_Array;
	std::vector<int> Business_Wealth_Array;
	std::vector<int> Goverment_Wealth_Array;
	std::vector<float> Financial_Reward_Array;
	std::vector<int> Policy_Array;

	// INITIAL CONDITIONS

	int contactsPerDay = 0;
	int S = 0;
	int E = 0;
	int I = 0;
	int R = 0;
	int Ih = 0;
	int Is = 0;
	int T = 0;
	int Iold = I;
	int Rold = R;
	//timepassed.push_back(T);
	int contactsPerDay1 = 0;
	int essential_workers = 0;
	int essential_workers_I = 0;
	int risk_group = 0;
	int students = 0;
	int selfQuarantined = 0;

	//initial SEIR counting

	for (int i = 0; i < N; i++) {
		if (PPL[i].group == 0) {
			S++;
		}
		if (PPL[i].group == 1) {
			E++;
		}
		if (PPL[i].group == 2) {
			I++;
		}
		if (PPL[i].group == 3) {
			R++;
		}
		if (PPL[i].group == 4) {
			Ih++;
		}
		if (PPL[i].group == 5) {
			Is++;
		}
		if (PPL[i].essential_worker == 1) {
			essential_workers++;
		}
		if (PPL[i].essential_worker == 1 && PPL[i].group == 2) {
			essential_workers_I++;
		}
		if (PPL[i].age >= 60) {
			risk_group++;
		}
		if (PPL[i].age <= 16) {
			students++;
		}
	}

	timepassed.push_back(T);
	Sarray.push_back(S);
	Earray.push_back(E);
	Iarray.push_back(I);
	Rarray.push_back(R);
	Iharray.push_back(Ih);
	Isarray.push_back(Is);

	printf("\nNumber of essential workers = %d, Number of INFECTED essential workers = %d\n", essential_workers, essential_workers_I);
	printf("\nRisk group: %d, PPL <= 16: %d, PPL in School: %d\n", risk_group, students, school.no_students);
	printf("\nS: %d, E: %d, I: %d, R: %d, Ih: %d, Is: %d, Contacts last day: %d, PPL in Hospital: %d, PPL in IC: %d", S, E, I, R, Ih, Is, contactsPerDay1, HOS.infected_hospitalized, HOS.intected_severe);


	//printf("\nS: %d, E: %d, I: %d, R: %d, Ih: %d, Is: %d, Contacts last day: %d, PPL in Hospital: %d, PPL in IC: %d", S, E, I, R, Ih, Is, contactsPerDay1, HOS.infected_hospitalized, HOS.intected_severe);

	//printf("\nTotal business wealth = %d\n", business_Wealth_total);
	business_Wealth_total = 0;
	for (int i = 0; i < no_workplaces; i++)
	{

		business_Wealth_total = business_Wealth_total + WRP[i].workplace_wealth;

	}
	//printf("\nTotal business wealth = %d\n", business_Wealth_total);

	//printf("\nTotal personal wealth = %d\n", personal_Wealth_total);
	personal_Wealth_total = 0;
	for (int i = 0; i < N; i++)
	{

		personal_Wealth_total = personal_Wealth_total + PPL[i].personal_wealth;

	}
	//printf("\nTotal personal wealth = %d\n", personal_Wealth_total);

	//printf("\nTotal house wealth = %d\n", public_Wealth_total);
	public_Wealth_total = 0;
	for (int i = 0; i < no_houses; i++)
	{

		public_Wealth_total = public_Wealth_total + HOU[i].home_wealth;

	}
	//printf("\nTotal house wealth = %d\n", public_Wealth_total);

	/*Personal_Wealth_Array.push_back(personal_Wealth_total);
	House_Wealth_Array.push_back(public_Wealth_total);
	Business_Wealth_Array.push_back(business_Wealth_total);
	Goverment_Wealth_Array.push_back(goverment_wealth_total);*/



	int policyx;
	bool choice = false;
	//printf("\nChoose starting policy...\n");
	//printf("\nPress 0 if you want to run policy0, 1 to run policy1, 2 to run policy2, 3 to run policy3, , 4 to run policy4, 5 to run policy5 or 6 to run policy6\n");
	//std::cin >> policyx;

	char inputData;
	printf("\nRead Qtable? y - yes, other - no\n");
	std::cin >> inputData;
	if (inputData == 'y') {
		std::string fileName;
		printf("\ninput file name: \n");
		std::cin >> fileName;

		std::ifstream read_excel_qtab(fileName);

		for (int i = 0; i < SimulationTime; i++) {
			std::string qForPol0;
			std::string qForPol1;
			std::string qForPol2;
			std::string qForPol3;
			std::string qForPol4;
			std::string qForPol5;
			std::string qForPol6;


			std::getline(read_excel_qtab, qForPol0, ',');
			Qtable[i][0] = std::stod(qForPol0);
			std::getline(read_excel_qtab, qForPol1, ',');
			Qtable[i][1] = std::stod(qForPol1);
			std::getline(read_excel_qtab, qForPol2, ',');
			Qtable[i][2] = std::stod(qForPol2);
			std::getline(read_excel_qtab, qForPol3, ',');
			Qtable[i][3] = std::stod(qForPol3);
			std::getline(read_excel_qtab, qForPol4, ',');
			Qtable[i][4] = std::stod(qForPol4);
			std::getline(read_excel_qtab, qForPol5, ',');
			Qtable[i][5] = std::stod(qForPol5);
			std::getline(read_excel_qtab, qForPol6);
			Qtable[i][6] = std::stod(qForPol6);


			//printf("\nx = %d, y = %d, ih = %d, is = %d\n", HOS.x, HOS.y, HOS.infected_hospitalized, HOS.intected_severe);
		}
	}
	
	Personal_Wealth_Array.push_back(personal_Wealth_total);
	House_Wealth_Array.push_back(public_Wealth_total);
	Business_Wealth_Array.push_back(business_Wealth_total);
	Goverment_Wealth_Array.push_back(goverment_wealth_total);

	// MAIN LOOP

	bool done = false;
	while (done == false)
	{
		
		
		personal_Wealth_total = 0;
		business_Wealth_total = 0;
		public_Wealth_total = 0;

		contactsPerDay1 = 0;

		policyx = pickRandomPolicy();




		EtoItransition(N, T);



		// ###################   24 H LOOP 

			// CODE FOR POLICY 4
		if (deltaI > -10 && lockdownDurationForPolicy4 > 15) {
			lockdownDurationForPolicy4 = 0;
			lockdownForPolicy4 = 0;
		}
		if (deltaI <= -10 || (deltaI > -10 && lockdownDurationForPolicy4 > 0 && lockdownDurationForPolicy4 <= 15)) {
			lockdownForPolicy4 = 1;
			if (deltaI <= -10) {
				lockdownDurationForPolicy4 = 0;
			}
			lockdownDurationForPolicy4++;
		}
		// END OF CODE FOR POLICY 4


		if (policyx == 0)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy0(hour, T);
			}
		}
		else if (policyx == 1)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy1(hour, T);
			}
		}
		else if (policyx == 2)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy2(hour, T);
			}
		}
		else if (policyx == 3)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy3(hour, T);
			}
		}
		else if (policyx == 4)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy4(hour, T);
			}
		}
		else if (policyx == 5)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy5(hour, T);
			}
		}
		else if (policyx == 6)
		{
			for (int hour = 0; hour < 24; hour++)
			{
				contactsPerDay1 = contactsPerDay1 + policy6(hour, T);
			}
		}
		else
		{

			printf("\nWrong choice, the program will exit...Try again...\n");

			getchar();
			return 0;
		}

		// ### END OF 24 H LOOP

		// finances ppl - houses, hospital fee
		for (int i = 0; i < N; i++)
		{
			/*for (int j = 0; j < no_houses; j++)
			{
				if (PPL[i].x_home == HOU[j].x && PPL[i].y_home == HOU[j].y)
				{
					//printf("\nhome_interference\n");
					//printf("\npersonal money pro home_interference = %d\n", PPL[i].personal_wealth);
					//printf("\nhome money pro home_interference = %d\n", HOU[j].home_wealth);
					financialInterferenceHome(PPL[i], HOU[j]); //works ok
					//printf("\npersonal money after home_interference = %d\n", PPL[i].personal_wealth);
					//printf("\nhome money after home_interference = %d\n", HOU[j].home_wealth);
				}
			}*/
			if (PPL[i].group == 4 || PPL[i].group == 5)
			{
				//printf("\npersonal money pro fee = %d\n", PPL[i].personal_wealth);
				PPL[i].actionPayHospitalFee();
				//printf("\npersonal money after fee = %d\n", PPL[i].personal_wealth);
			}
		}
	for (int i = 0; i < no_workplaces; i++)
		{

			if ((policyx == 0 || policyx == 1) && (T != 30 && T != 60 && T != 90 && T != 120 && T != 150 && T != 180))
			{
				//printf("\nwork money pro profit = %d\n", WRP[i].workplace_wealth);
				BusinessProfit(WRP[i]);
				//printf("\nwork money after profit = %d\n", WRP[i].workplace_wealth);
			}
		}
		
		// salaries, taxes at the end of the month
		if (T == 30 || T == 60 || T == 90 || T == 120 || T == 150 || T == 180)
			//if (T == 2)
		{
			for (int i = 0; i < no_workplaces; i++) // Business Taxes
			{
				//printf("\nwork money pro bills = %d\n", WRP[i].workplace_wealth);
				//printf("\ngov money pro bills_w = %d\n", goverment_wealth_total);
				WRP[i].actionPayBills(); // works
				/*if(policyx == 0 || policyx == 1)
				{
					//printf("\nwork money pro profit = %d\n", WRP[i].workplace_wealth);
					BusinessProfit(WRP[i]);
					//printf("\nwork money after profit = %d\n", WRP[i].workplace_wealth);
				}*/
				//printf("\nwork money after bills = %d\n", WRP[i].workplace_wealth);
				//printf("\ngov money after_w bills = %d\n", goverment_wealth_total);

			}
			for (int j = 0; j < no_houses; j++) // Houses Taxes and Supply
			{
				//printf("\nhome money before bills = %d\n", HOU[j].home_wealth);
				//printf("\ngov money pro bills_h = %d\n", goverment_wealth_total);
				HOU[j].actionPayBills(); // works ok
				//printf("\nhome money after bills = %d\n", HOU[j].home_wealth);
				//printf("\ngov money after bills_h = %d\n", goverment_wealth_total);
				std::uniform_int_distribution <int> distribution_supply(0, no_workplaces);

				int z = distribution_supply(generator);
				//printf("\nhome money before supply = %d\n", HOU[j].home_wealth);
				//printf("\nwork money pro supply = %d\n", WRP[z].workplace_wealth);
				actionHouseSupplyBusiness(HOU[j], WRP[z]);
				//printf("\nhome money after supply = %d\n", HOU[j].home_wealth);
				//printf("\nwork money after supply = %d\n", WRP[z].workplace_wealth);

			}

			for (int i = 0; i < N; i++) //salaries work to person + aid
			{
				if (PPL[i].unemployed == 0)

				{
					for (int j = 0; j < no_workplaces; j++)
					{
						if (PPL[i].x_work == WRP[j].x && PPL[i].y_work == WRP[j].y)
						{
							//printf("\nwork money pro salary = %d\n", WRP[j].workplace_wealth);
							actionWorkplacePays(PPL[i], WRP[j]); //works not ok
							//printf("\nwork money after salary = %d\n", WRP[j].workplace_wealth);
						}

					}
				}
				else if (PPL[i].unemployed == 1 /*|| PPL[i].homeless == 1*/)

				{
					//printf("\ngov money before aid = %d\n", goverment_wealth_total);
					//printf("\npoor person money pro aid = %d\n", PPL[i].personal_wealth);
					GovermentAid(PPL[i]); //works ok
					//printf("\ngov money after aid = %d\n", goverment_wealth_total);
					//printf("\npoor person money after aid = %d\n", PPL[i].personal_wealth);
				}


			}

			for (int i = 0; i < N; i++) //salaries person to house
			{
				if (PPL[i].homeless == 0)

				{
					for (int j = 0; j < no_houses; j++)
					{
						if (PPL[i].x_home == HOU[j].x && PPL[i].y_home == HOU[j].y)
						{
							//printf("\nhome money pro salary = %d\n", HOU[j].home_wealth);
							actionTransferSalaryToHome(PPL[i], HOU[j]); //works  ok
							//printf("\nhome money after salary = %d\n", HOU[j].home_wealth);
						}

					}
				}



			}


			//printf("\ngov money before healthcare = %d\n", goverment_wealth_total);
			GovermentFinanceHealthcare(); //works ok
			//printf("\ngov money after healthcare = %d\n", goverment_wealth_total);
			printf("\nFinancial processes done!\n");
		}

		// count wealth
		for (int i = 0; i < no_workplaces; i++)
		{

			business_Wealth_total = business_Wealth_total + WRP[i].workplace_wealth;

		}
		for (int i = 0; i < no_houses; i++)
		{

			public_Wealth_total = public_Wealth_total + HOU[i].home_wealth;

		}
		for (int i = 0; i < N; i++)
		{

			personal_Wealth_total = personal_Wealth_total + PPL[i].personal_wealth;

		}
		//printf("\n policy is %d\n", policyx);
		Personal_Wealth_Array.push_back(personal_Wealth_total);
		House_Wealth_Array.push_back(public_Wealth_total);
		Business_Wealth_Array.push_back(business_Wealth_total);
		Goverment_Wealth_Array.push_back(goverment_wealth_total);
	
		delta_business_wealth_total = (Business_Wealth_Array[T+1] - Business_Wealth_Array[T]);
			
		

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

		// LIMIT PPL

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

		// COUNT PPL

		Rold = R;
		Iold = I;

		S = 0;
		E = 0;
		I = 0;
		R = 0;
		Ih = 0;
		Is = 0;
		selfQuarantined = 0;

		for (int i = 0; i < N; i++) {
			if (PPL[i].group == 0) {
				S++;
			}
			if (PPL[i].group == 1) {
				E++;
			}
			if (PPL[i].group == 2) {
				I++;
			}
			if (PPL[i].group == 3) {
				R++;
			}
			if (PPL[i].group == 4) {
				Ih++;
			}
			if (PPL[i].group == 5) {
				Is++;
			}
			if (PPL[i].quarantined == 1) {
				selfQuarantined++;
			}
		}

		deltaR = R - Rold;
		deltaI = Iold - I;
		deltaI = deltaI - deltaR;
		timepassed.push_back(T);
		Sarray.push_back(S);
		Earray.push_back(E);
		Iarray.push_back(I);
		Rarray.push_back(R);
		Iharray.push_back(Ih);
		Isarray.push_back(Is);

		if ((I < 300 && Ih < 8 && Is < 4) && (T != 30 && T != 60 && T != 90 && T != 120 && T != 150 && T != 180))

		{
			financial_reward_rate = 1;
		}

		else if ((I > 300 && Ih < 8 && Is < 4) && (T != 30 && T != 60 && T != 90 && T != 120 && T != 150 && T != 180))
		{

			financial_reward_rate = 0.5;
		}

		else if ((Ih > 8 || Is > 4) && (T != 30 && T != 60 && T != 90 && T != 120 && T != 150 && T != 180))
		{
			financial_reward_rate = 0;

		}
		
		reward_f = financial_reward_rate * (float)(delta_business_wealth_total / 100);
		Financial_Reward_Array.push_back(reward_f);
		Policy_Array.push_back(policyx);
		//printf("\nDifference in business wealth is %d\n", delta_business_wealth_total);
		//printf("\nreward rate is %f\n", financial_reward_rate);
		//printf("\nreward is %f\n", reward_f);
		//delta_houses_wealth_total = House_Wealth_Array[T + 1] - House_Wealth_Array[T];
		//printf("\nDifference in house wealth is %d\n", delta_houses_wealth_total);
		//delta_people_wealth_total = Personal_Wealth_Array[T + 1] - Personal_Wealth_Array[T];
		//printf("\nDifference in personal wealth is %d\n", delta_people_wealth_total);

		//printf("\nS: %d, E: %d, I: %d, R: %d, Ih: %d, Is: %d, Contacts last day: %d, PPL in Hospital: %d, PPL in IC: %d, Delta I: %d", S, E, I, R, Ih, Is, contactsPerDay1, HOS.infected_hospitalized, HOS.intected_severe, deltaI);
		//printf("\nS: %d, E: %d, I: %d, R: %d, Ih: %d, Is: %d, Contacts last day: %d, PPL in Hospital: %d, PPL in IC: %d, Delta I: %d, Self Quarantined: %d", S, E, I, R, Ih, Is, contactsPerDay1, HOS.infected_hospitalized, HOS.intected_severe, deltaI, selfQuarantined);
		
		reward = QReward(deltaI, Ih, Is);
		
		QUpdate(policyx, T, reward);
		printf("\nS: %d, E: %d, I: %d, R: %d, Ih: %d, Is: %d, Contacts last day: %d, PPL in Hospital: %d, PPL in IC: %d, Delta I: %d, Self Quarantined: %d, Reward: %f", S, E, I, R, Ih, Is, contactsPerDay1, HOS.infected_hospitalized, HOS.intected_severe, deltaI, selfQuarantined, reward);


		T++;
		if (T == SimulationTime) {
			done = true;
		}

	}



	excel_file_SEIR_results.open("SEIR_Results.csv");
	printf("\nPreparing results excel file...\n");
	for (int i = 0; i < T; i++) {
		excel_file_SEIR_results << Sarray[i] << "," << Earray[i] << "," << Iarray[i] << "," << Rarray[i] << "," << Iharray[i] << "," << Isarray[i] << std::endl;
	}
	excel_file_SEIR_results.close();


	excel_file_Financial_results.open("Financial_Results.csv");
	printf("\nPreparing Financial results excel file...\n");
	for (int i = 0; i < T; i++) {
		excel_file_Financial_results << Personal_Wealth_Array[i] << "," << House_Wealth_Array[i] << "," << Business_Wealth_Array[i] << "," << Goverment_Wealth_Array[i] << "," << timepassed[i] << std::endl;
	}
	excel_file_Financial_results.close();

	excel_file_Financial_Reward.open("Financial_Reward.csv");
	printf("\nPreparing Financial Reward excel file...\n");
	for (int i = 0; i < T; i++) {
		excel_file_Financial_Reward << Policy_Array[i] << "," << Iarray[i] << "," << Iharray[i] << "," << Isarray[i] << "," << Financial_Reward_Array[i] << std::endl;
	}
	excel_file_Financial_Reward.close();


	printf("\nSEIR results excel file done!\n");

	printf("\nFinancial results excel file done!\n");

	/*float financial_difference_people = (((Personal_Wealth_Array[90] - Personal_Wealth_Array[0]) / Personal_Wealth_Array[0]) * 100);
	printf("\nbusiness wealth T - 1 = %d\n", Business_Wealth_Array[90]);
	printf("\nbusiness wealth 0 = %d\n", Business_Wealth_Array[0]);
	float financial_difference_houses = (((House_Wealth_Array[90] - House_Wealth_Array[0]) / House_Wealth_Array[0]) * 100);
	float financial_difference_business = (((Business_Wealth_Array[90] - Business_Wealth_Array[0]) / Business_Wealth_Array[0]) * 100);
	float financial_difference_gov = (((Goverment_Wealth_Array[90] - Goverment_Wealth_Array[0]) / Goverment_Wealth_Array[0]) * 100);

	printf("\nFinancial difference in people is %f\n", financial_difference_people);
	printf("\nFinancial difference in houses is %f\n", financial_difference_houses);
	printf("\nFinancial difference in businesses is %f\n", financial_difference_business);
	printf("\nFinancial difference in goverment is %f\n", financial_difference_gov);*/

	char press_button_data1;
	printf("\nDo you want to save Q table? y - yes, other - no\n");
	std::cin >> press_button_data1;
	if (press_button_data1 == 'y') {
		printf("\nInput file name: \n");
		std::string QtableFileName;
		std::cin >> QtableFileName;
		excel_qtab.open(QtableFileName);
		//excel_qtab.open("po.csv");
		for (int i = 0; i < SimulationTime; i++) {
			excel_qtab << Qtable[i][0] << "," << Qtable[i][1] << "," << Qtable[i][2] << "," << Qtable[i][3] << "," << Qtable[i][4] << "," << Qtable[i][5] << "," << Qtable[i][6] << std::endl;
		}
		excel_qtab.close();
	}

	printf("\nSimulation completed!\n");

	printf("\nPress a button to finish...\n");

	getchar();
};