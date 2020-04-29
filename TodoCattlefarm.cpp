/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoCow.h"
#include <cstdlib>

// Cattlefarm cost 80 upgrade cost 16
// Cattlefarm size 6, 6
// Cattlefarm init max num. of employee is CATTLEFARM_MAX_NUM_EMPLOYEE_MIN
// Cattlefarm Upgrade:
//      level increases by 1;
//      max num. of employees increases by 1 until CATTLEFARM_MAX_NUM_EMPLOYEE_MAX
// Cattlefarm Makemoney:
//      min(num. of cows, num. of feeders in work) x level x 10;

const int CATTLEFARM_COST = 80;
const int CATTLEFARM_UPGRADE_COST = 16;
const int CATTLEFARM_SIZE_X = 6; const int CATTLEFARM_SIZE_Y = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MIN = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MAX = (CATTLEFARM_SIZE_X-2) * (CATTLEFARM_SIZE_Y-2);

// TODO: Start to implement your code.

Cattlefarm::Cattlefarm(int x, int y)
: Property(CATTLEFARM_COST, CATTLEFARM_UPGRADE_COST, CATTLEFARM_MAX_NUM_EMPLOYEE_MIN){
	Object::setSize(CATTLEFARM_SIZE_X, CATTLEFARM_SIZE_Y);
	Object::setXY(x,y);
}

bool Cattlefarm::checkEmployee(Employee* e) const{
	if(e==nullptr)
		return false;
	return (e->getSymbol()=='d'||e->getSymbol()=='c');
}

void Cattlefarm::upgrade(){
	Property::upgrade();
	if(this->getMaxNumEmployee()<CATTLEFARM_MAX_NUM_EMPLOYEE_MAX)
		this->setMaxNumEmployee(getMaxNumEmployee()+1);
}

int Cattlefarm::makeMoney() const{
	int l = getLevel();
	int cnum = 0, dinwork = 0;
	const Employee** wp = nullptr;
	getConstEmployeeList(wp);
	for(int i=0;i<getNumEmployee();i++){
		switch(wp[i]->getSymbol()){
		case 'c':
			cnum++;
			break;
		case 'd':
			if(wp[i]->getState()==ObjectState::WORK)
				dinwork++;
			break;
		default:
			break;
		}
	}

	delete [] wp;
	wp = nullptr;

	int min = (cnum>dinwork)?dinwork:cnum;
	return min*l*10;
}

void Cattlefarm::removeDiedCow(){

	const Employee** templist = nullptr;
	getConstEmployeeList(templist);

	int index = 0;

	Employee* diedCow = nullptr;

	for(;index<getNumEmployee();index++){
		if(templist[index]->getSymbol()=='c'&&!dynamic_cast<const Cow*>(templist[index])->isAlive()){
			diedCow = const_cast<Employee*>(templist[index]);
			fireEmployee(diedCow);	// Removing the died cow
			delete [] templist;
			getConstEmployeeList(templist);	// Updating working employee_list
			index--;
		}
	}

	diedCow = nullptr;
	delete [] templist;
	templist = nullptr;

}

string Cattlefarm::getName() const{
	return "Cattlefarm";
}

char Cattlefarm::getSymbol() const{
	return 'C';
}


