/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "Property.h"
#include "Employee.h"

void Property::writeToStream(ostream& os) const {
	Object::writeToStream(os);
	os << "Cost:" << getCost() << endl;
    os << "Upgrade Cost:" << getUpgradeCost() << endl;
    os << "Level:" << getLevel() << endl;
    os << "# of Employee:" << getNumEmployee() << " / " << getMaxNumEmployee() << endl;
}

void Property::upgrade() {
    m_level++;
}

void Property::getConstEmployeeList(const Employee**& employee) const {
	if (m_num_employee == 0) {
		employee =nullptr;
		return;
	}
	employee = new const Employee*[m_num_employee];
	for (int i = 0; i < m_num_employee; i++)
		employee[i] = m_employee_list[i];
}

// TODO: Start to implement your code.

Property::Property(int cbuild,int cupgrade,int maxEmployee)
: m_cost(cbuild), m_upgrade_cost(cupgrade), m_max_num_employee(maxEmployee){
}

Property::~Property(){
	for(int i=0;i<m_num_employee;i++){
		delete m_employee_list[i];
		m_employee_list[i] = nullptr;
	}

	delete [] m_employee_list;
	m_employee_list = nullptr;
}

int Property::getCost() const{
	return m_cost;
}

int Property::getUpgradeCost()const{
	return m_upgrade_cost;
}

int Property::getLevel() const{
	return m_level;
}

int Property::getNumEmployee() const{
	return m_num_employee;
}

int Property::getMaxNumEmployee() const{
	return m_max_num_employee;
}

void Property::setMaxNumEmployee(int mNE){
	m_max_num_employee = mNE;
}

bool Property::assignEmployee(Employee* e){

	if(m_num_employee == m_max_num_employee)
		return false;	// If reaches m_max_num_employee, it fails to assign.

	for(int i=0;i<m_num_employee;i++)
		if(e==m_employee_list[i])
			return false;	// If the employee already in the list, it fails to assign.

	if(!(this->checkEmployee(e)))
		return false;	// If the employee is unsuitable to the property, it fails to assign.

	if(m_num_employee==0){
		Employee** newlist = new Employee*[1];
		newlist[0] = e;
		int proX = 0, proY = 0;
		getXY(proX,proY);
		newlist[0]->setXY(proX+1,proY+1);
		e->setState(ObjectState::WORK);
		m_employee_list = newlist;
		m_num_employee++;


		return true;
	}

	Employee** newlist = new Employee*[m_num_employee+1];

	for(int i=0;i<m_num_employee;i++)
		newlist[i] = m_employee_list[i];
	newlist[m_num_employee] = e;
	Employee* p = m_employee_list[m_num_employee-1]; // Current employee

	int curX = 0, curY = 0;
	p->getXY(curX,curY);

	int proX = 0, proY = 0, proSizeX = 0, proSizeY = 0;
	getXY(proX,proY);
	getSize(proSizeX, proSizeY);

	if(curY==(proY+proSizeY-2))
		newlist[m_num_employee]->setXY(curX+1,proY+1);
	else
		newlist[m_num_employee]->setXY(curX,curY+1);

	e->setState(ObjectState::WORK);
	delete [] m_employee_list;
	m_employee_list = newlist;
	m_num_employee++;
	return true;
}

bool Property::fireEmployee(Employee* e){

	if(m_employee_list==nullptr)
		return false;

	else if(m_num_employee==1){
		delete m_employee_list[0];
		delete [] m_employee_list;
		m_employee_list = nullptr;
		m_num_employee--;
		return true;
	}	// One employee left

	int i = 0;
	for(;m_employee_list[i]!=e;i++);
	// finding the index of the target employee

	int targetX = 0, targetY = 0;

	if(i<m_num_employee-1)
		m_employee_list[i]->getXY(targetX,targetY);

	delete m_employee_list[i];
	m_employee_list[i] = nullptr;

	Employee** newlist = new Employee*[m_num_employee-1];
	for(int j=0;j<m_num_employee-1;j++){
		if(j==i){
			newlist[j] = m_employee_list[m_num_employee-1];
			m_employee_list[m_num_employee-1] = nullptr;
			newlist[j]->setXY(targetX,targetY);
		}
		else
			newlist[j] = m_employee_list[j];
	}



	delete [] m_employee_list;
	m_employee_list = newlist;	// Shallow copying the m_employee_list

	m_num_employee--;
	return true;
}






















