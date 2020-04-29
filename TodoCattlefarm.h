/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#ifndef CATTLEFARM_H_
#define CATTLEFARM_H_

#include "Property.h"
extern const int CATTLEFARM_SIZE_X;
extern const int CATTLEFARM_SIZE_Y;

// HINT: You may seek help from Property::fireEmployee()
//       when you implement your Cattlefarm::removeDiedCow().
// TODO: Start to implement your code.
class Cattlefarm : public Property{
public:
	Cattlefarm(int, int);
	bool checkEmployee(Employee*) const override;
	void upgrade() override;
	int makeMoney() const override;
	void removeDiedCow();
	string getName() const override; //return the "Cattlefarm"
	char getSymbol() const override; //return the "C"
};
#endif /*CATTLEFARM_H_*/
