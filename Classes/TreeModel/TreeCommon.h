#pragma once

USING_NS_CC;

////////////////////////////////////////////////////////////////////

// тип сущности лежащей внутри дерева
enum TreePartType
{
	TypeUndefined = 0, // тип не определен
	Leaf = 1,      // лист
	Branch = 2,    // ветка
	Fruit = 3,     // плод
	Root = 4       // корень
};

// тип ресурса потребляемый или производимый элементом
enum TreeResourceType
{
	ResourceUndefined = 0,  // тип не определен
	Water = 1,      // вода
	SunEnergy = 2,  // энергия
	Glucose = 3     // глюкоза
};


// элемент сущности, лежащей внутри дерева
class TreeElement
{
public: 
	TreeElement() : element_id(-1), type(TreePartType::TypeUndefined), start_point(), end_point(), 
		width(0.), length(0.), height(0.), water_con(0.) {};

	int element_id;      // уникальный айдишник элемента
	TreePartType type;   // тип элемента (ветка/лист/плод)

	Vec2 start_point;    // координаты начала элемента
	Vec2 end_point;      // координаты конца элемента (при наличии)

	double width;        // ширина элемента (при наличии)
	double length;       // длина элемента (при наличии)
	double height;       // высота элемента (при наличии)

	double water_con;    // содержание воды на каждый элемент

	void MoveElement(const Vec2& move_vector);
	void StretchElement(float coef, Vec2& stretch_vector);
};

////////////////////////////////////////////////////////////////////

std::string to_str(TreeResourceType res_type);
