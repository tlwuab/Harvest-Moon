/*
 * File Created: Mon Mar 16 2020
 * Author:
 * SID:
*/

#include "Scene.h"
#include "Property.h"
#include "TodoFarmland.h"
#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoFarmer.h"
#include "TodoFeeder.h"
#include "TodoCow.h"

const int SCENE_W = 45;
const int SCENE_H = 10;
const int PROPERTY_X = 35;
const int SCENE_INIT_MONEY = 200;

Scene::Scene()
: m_objects(nullptr), m_num_objects(0), m_money(SCENE_INIT_MONEY) {
    nextRound();
}

Scene::~Scene() {
    //shallowremove employees
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::EMPLOYEE)
            shallowRemoveEmployee(dynamic_cast<Employee*>(m_objects[i]));
    }
    // remove properties
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::PROPERTY)
            delete m_objects[i];
    }
    delete[] m_objects;
}

void Scene::shallowRemoveEmployee(Employee* e) {
    if (m_num_objects <= 0) return;
    if (m_num_objects == 1 && e == m_objects[0]) {
        m_num_objects = 0;
        m_objects = nullptr;
        return;
    }
    int i;
	for (i = 0; i < m_num_objects; i++) {
		if (m_objects[i] == e)
			break;
	}
    Object ** newObjects = new Object*[m_num_objects--];
	for (int j = 0; j < m_num_objects; j++)
		newObjects[j] = m_objects[j];
	if (m_num_objects != i)
		newObjects[i] = m_objects[m_num_objects];
    delete [] m_objects;
    m_objects = newObjects;
}

bool Scene::checkOverlap(const Property* newproperty) {
    int x, y, sz_x, sz_y;
    newproperty->getXY(x, y);
    newproperty->getSize(sz_x, sz_y);
    for(int xx=x; xx<x+sz_x; xx++)
        for(int yy=y; yy<y+sz_y; yy++)
            if(getObjectAt(xx, yy) != nullptr) return true;
    return false;
}


void Scene::addProperty(int property, int x, int y) {
    Property* newProperty = nullptr;
    switch(property) {
    case FARMLAND:
        newProperty = new Farmland(x,y);
        break;
    case CATTLEFARM:
        newProperty = new Cattlefarm(x, y);
        break;
    }
    if (newProperty == nullptr) return;
    if (newProperty->getCost() > m_money || checkOverlap(newProperty)) {
        delete newProperty;
        return;
    }
    addObject(newProperty);
    m_money -= newProperty->getCost();
}


bool Scene::hire(Property* p, int employee) {
    Employee* newEmployee = nullptr;
    switch(employee){
    case EMPLOYEE:
        newEmployee = new Employee();
        break;
    case FARMER:
        newEmployee = new Farmer();
        break;
    case FEEDER:
        newEmployee = new Feeder();
        break;
    case COW:
        newEmployee = new Cow();
        break;
    }
    if (newEmployee == nullptr) return false;
    if (newEmployee->getCost() > m_money || ! p->assignEmployee(newEmployee)) {
        delete newEmployee;
        return false;
    }
    addObject(newEmployee);
    m_money -= newEmployee->getCost();
    return true;
}


Object* Scene::getObjectAt(int s_x, int s_y) const {
	int x, y, sz_x, sz_y;
    // If employee is at s_x, s_y, get employee
    // else, get property
    // otherwise return null
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::PROPERTY)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	return nullptr;
}

//perform shallow copying
void Scene::addObject(Object* newobj) {
	Object** newobjects = new Object*[m_num_objects + 1];
	for (int i = 0; i < m_num_objects; i++)
		newobjects[i] = m_objects[i];
	newobjects[m_num_objects++] = newobj;
	if (m_num_objects != 1)
		delete [] m_objects;
	m_objects = newobjects;
}

void Scene::getConstObjects(const Object**& obj, int& count) const {
	count = m_num_objects;
	if (m_num_objects == 0) {
		obj =nullptr;
		return;
	}
	obj = new const Object*[m_num_objects];
	for (int i = 0; i < m_num_objects; i++)
		obj[i] = m_objects[i];
}

// TODO: Start to implement your code.

bool Scene::isGameOver() const{
	return (m_money<0);
}

void Scene::removeProperty(Property* pr){

	int num_employee_pr = 0;
	const Employee** temp_pr_list = nullptr;
	for(int i=0;i<m_num_objects;i++){
		if(m_objects[i]==pr){
			pr->getConstEmployeeList(temp_pr_list);
			num_employee_pr = pr->getNumEmployee();
			break;
		}
	}

	for(int i=0;i<num_employee_pr;i++){
		shallowRemoveEmployee(const_cast<Employee*>(temp_pr_list[i]));
	}

	int newlength = m_num_objects-1;
	Object** newobjects = new Object*[newlength];

	int prindex = 0;
	for(;dynamic_cast<Property*>(m_objects[prindex])!=pr;prindex++);

	for(int i=0;i<newlength;i++){
		if(i>=prindex)
			newobjects[i] = m_objects[i+1];
		else
			newobjects[i] = m_objects[i];
	}
	delete pr;
	pr = nullptr;
	delete [] m_objects;
	m_objects = newobjects;
	delete [] temp_pr_list;
	temp_pr_list = nullptr;
	m_num_objects = newlength;
}

void Scene::nextRound(){

	int earnmoney = 0, totalsalary = 0;

	for(int i=0;i<m_num_objects;i++)
		if(m_objects[i]->getObjectType()==ObjectType::PROPERTY)
			earnmoney+=dynamic_cast<Property*>(m_objects[i])->makeMoney();

	for(int i=0;i<m_num_objects;i++)
		if(m_objects[i]->getObjectType()==ObjectType::EMPLOYEE){
			totalsalary+=dynamic_cast<Employee*>(m_objects[i])->getSalary();
			dynamic_cast<Employee*>(m_objects[i])->updateWorkAge();
			dynamic_cast<Employee*>(m_objects[i])->updateState();
		}

	for(int i=0;i<m_num_objects;i++)
		if(m_objects[i]->getSymbol()=='c'&&!dynamic_cast<Cow*>(m_objects[i])->isAlive()){
			Object ** newObjects = new Object*[m_num_objects--];
			for (int j = 0; j < m_num_objects; j++){
				if (j>=i)
					newObjects[j] = m_objects[j+1];
				else
					newObjects[j] = m_objects[j];
			}
		    delete [] m_objects;
		    m_objects = newObjects;
		    i--;
		}


	for(int i=0;i<m_num_objects;i++)
			if(m_objects[i]->getSymbol()=='C')
				dynamic_cast<Cattlefarm*>(m_objects[i])->removeDiedCow();


	m_money+=earnmoney;
	m_money-=totalsalary;
}

bool Scene::upgrade(Property* pr){

	if(pr==nullptr)
		return false;

	bool find = false;

	for(int i=0;i<m_num_objects;i++)
		if(m_objects[i]==pr){
			pr->upgrade();
			m_money-=pr->getUpgradeCost();
			find = true;
			break;
		}
	return find;
}

bool Scene::fire(Employee* e){

	if(e==nullptr)
		return false;

	int i = 0;
	bool found = false;
	for(;i<m_num_objects;i++){
		if(m_objects[i]->getObjectType()==ObjectType::PROPERTY){
			Property* p = dynamic_cast<Property*>(m_objects[i]);
			const Employee** templist = nullptr;
			p->getConstEmployeeList(templist);
			for(int j=0;j<p->getNumEmployee();j++){
				if(templist[j]==e){
					p->fireEmployee(e);
					found = true;
					break;
				}
			}
			delete [] templist;
			templist = nullptr;
			if(found)
				break;
		}
	}

    shallowRemoveEmployee(e);

	return true;
}
















