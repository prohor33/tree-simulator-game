#pragma once

USING_NS_CC;



class Tree {
public:
    
    // ветка
    using TSegment = std::pair<Vec2, Vec2>;
    
    const std::vector<TSegment>& segments() const;
    void add_segment(const TSegment& s);
    
    // возвращает точки возможного роста
    std::vector<Vec2> grow_points() const;
    
    // листья
    const std::vector<Vec2>& leafs() const;
    void add_leaf(const Vec2& leaf);
    
private:
    // просто для примера
    // ветки
    std::vector<TSegment> segments_;
    // листья
    std::vector<Vec2> leafs_;
};


////////////////////////////////////////////////////////////////////

// тип сущности лежащей внутри дерева
enum TypeOfPart
{
	Undefined = 0, // тип не определен
	Leaf = 1,      // лист
	Branch = 2,    // ветка
	Fruit = 3      // плод
};

// элемент сущности, лежащей внутри дерева
class TreeNodeInternal
{
public: 
	TreeNodeInternal() : element_id(0), type(TypeOfPart::Undefined), start_point(), end_point(), width(0.) {};

	int element_id;    // уникальный айдишник элемента
	TypeOfPart type;   // тип элемента (ветка/лист/плод)
	Vec2 start_point;  // координаты начала элемента
	Vec2 end_point;    // координаты конца элемента (при наличии)
	float width;       // ширина элемента (при наличии)
};

// элемент дерева
class TreeNode
{
public: 
	typedef std::shared_ptr<TreeNode> TreeNodePtr;
protected:
	// откуда вырос элемент
	TreeNode* parent;
	// что выросло из элемента
	std::vector<TreeNodePtr> children;
	
	// внутренности элемента
	TreeNodeInternal elem;

public:
	TreeNode() : parent(nullptr) {};

	// установить элементы записи
	void SetInternal(const TreeNodeInternal& inter);
	void SetParent(TreeNode* parent);
	void AddChild(const TreeNodePtr child);
	
	//void RemoveChild(int pos);

	// достать элементы записи
	TreeNode* GetParent() const { return parent; }
	TreeNodePtr GetChild(int pos) const { return children.at(pos); }
	const TreeNodeInternal& GetInternals() const { return elem; }

	int GetChildrenAmount() const;
};

enum AddingResult
{
	ResultUndefined = 0,
	ResultOK = 1,
	ResultNoMoreSlots = 2,
	ResultWrongDirection = 3
};

class TreeInternal
{
public:
	TreeInternal() : root(nullptr) {}
	void Initialize(const Vec2& start_point);

	const AddingResult AddElement(const int branch_id, const Vec2& point_of_grow, int& id, TypeOfPart tp);

	void GetElements(std::vector<std::pair<Vec2, Vec2>>& elems, TypeOfPart tp) const;
	void GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const;

private:
	std::shared_ptr<TreeNode> root;
	std::map<int, std::shared_ptr<TreeNode>> fast_navigation_map;	
};

class TreeInterface
{
public:
	TreeInterface() : tree(nullptr) {}

	bool MakeTree(const Vec2& start_point);

	std::shared_ptr<TreeInternal> tree;

	// функции для добавления элементов
	const AddingResult AddBranch(const int branch_id, const Vec2& point_of_grow, int& id);
	const AddingResult AddLeaf(const int branch_id, const Vec2& point_of_grow, int& id);

	// функции для получения элементов (заменить на итератор)
	void GetBranches(std::vector<std::pair<Vec2, Vec2>>& branches) const;
	void GetLeafs(std::vector<std::pair<Vec2, Vec2>>& leafs) const;


	void GetGrowPoints(std::vector<std::pair<Vec2, int>>& grow_points) const;
	//TreeInterface& operator=(const TreeInterface&) = delete;
	//TreeInterface(const TreeInterface&) = delete;
};